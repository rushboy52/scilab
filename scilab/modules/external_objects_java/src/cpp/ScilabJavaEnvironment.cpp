/*
 * PIMS ( http://forge.scilab.org/index.php/p/pims ) - This file is part of PIMS
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <jni.h>

#include "ScilabJavaEnvironment.hxx"
#include "JavaOptionsHelper.hxx"
#include "ScilabClassLoader.hxx"
#include "ScilabJavaClass.hxx"
#include "ScilabJavaObject.hxx"
#include "NoMoreScilabMemoryException.hxx"

//#include "ScilabJavaObjectHelper.hxx"
extern "C" {
#include "getScilabJavaVM.h"
}

namespace org_scilab_modules_external_objects_java
{

const std::string ScilabJavaEnvironment::environmentName = "Java Environment";
int ScilabJavaEnvironment::envId = -1;
ScilabJavaEnvironment * ScilabJavaEnvironment::instance = 0;
bool ScilabJavaEnvironment::usable = true;

ScilabJavaEnvironment::ScilabJavaEnvironment() :
    helper(*new JavaOptionsHelper()),
    gwOptions(*new ScilabGatewayOptions()),
    wrapper(*new ScilabJavaEnvironmentWrapper(helper)),
    traceEnabled(false),
    isInit(false),
    scilabStream(*new ScilabStream()),
    file(0) { }

//    ScilabJavaEnvironment::ScilabJavaEnvironment() {}
ScilabJavaEnvironment::~ScilabJavaEnvironment()
{
    //    delete &scope;
    delete &helper;
    delete &gwOptions;
    delete &wrapper;

    if (file)
    {
        file->flush();
        file->close();
        delete file;
        file = 0;
    }
}

int ScilabJavaEnvironment::start()
{
    /*    if (!usable)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Due to Java interpreter limitations, Scilab must be restarted"));
            }*/

    if (envId == -1)
    {
        instance = new ScilabJavaEnvironment();
        envId = ScilabEnvironments::registerScilabEnvironment(instance);
        instance->Initialize();
        /*        ScilabJavaOStream::initJavaStreams();
                ScilabJavaOStream::setStdOutStream(&instance->scilabStream);
                ScilabJavaOStream::setStdErrStream(&instance->scilabStream);*/
        instance->helper.setUseLastName(true);
        instance->helper.setNewAllowed(true);
        instance->enabletrace("/tmp/eo_java.log");
    }

    return envId;
}

void ScilabJavaEnvironment::finish()
{
    if (envId != -1)
    {
        ScilabEnvironments::unregisterScilabEnvironment(envId);
        envId = -1;
        delete instance;
        instance = 0;
        instance->Finalize();
        usable = false;
    }
}

void ScilabJavaEnvironment::Initialize()
{
    if (!isInit)
    {
        isInit = true;
        // No need to init the Java VM. Scilab is already using it
        // TODO: 1 or 0... 1 is to enable to register signal handlers
        //        Py_InitializeEx(0);
        //        initNumpy();
    }
}

void ScilabJavaEnvironment::Finalize()
{
    // Scilab cannot kill the Java VM. It would probably crash the application
    //    Py_Finalize();
}

/*
void ScilabJavaEnvironment::initNumpy()
{
    import_array();
}
*/
JavaOptionsHelper & ScilabJavaEnvironment::getOptionsHelper()
{
    return helper;
}

ScilabGatewayOptions & ScilabJavaEnvironment::getGatewayOptions()
{
    return gwOptions;
}

ScilabAbstractEnvironmentWrapper & ScilabJavaEnvironment::getWrapper()
{
    return wrapper;
}

const std::string & ScilabJavaEnvironment::getEnvironmentName()
{
    return environmentName;
}

void ScilabJavaEnvironment::getEnvironmentInfos(const ScilabStringStackAllocator & allocator)
{

    writeLog("getEnvironmentInfos", "Get information");

    /*
        std::vector<char *> version = breakInLines(std::string(Py_GetVersion()));
        std::vector<char *> platform = breakInLines(std::string(Py_GetPlatform()));
        std::vector<char *> copyright = breakInLines(std::string(Py_GetCopyright()));
        std::vector<char *> compiler = breakInLines(std::string(Py_GetCompiler()));
        std::vector<char *> buildInfo = breakInLines(std::string(Py_GetBuildInfo()));

        int nbRows = version.size() + platform.size() + copyright.size() + compiler.size() + buildInfo.size();

        std::vector<char *> all(nbRows, const_cast<char* >(""));
        all[0] = const_cast<char* >("Version");
        all[version.size()] = const_cast<char* >("Platform");
        all[version.size() + platform.size()] = const_cast<char* >("Copyright");
        all[version.size() + platform.size() + copyright.size()] = const_cast<char* >("Compiler");
        all[version.size() + platform.size() + copyright.size() + compiler.size()] = const_cast<char* >("Build info");

        all.insert(all.end(), version.begin(), version.end());
        all.insert(all.end(), platform.begin(), platform.end());
        all.insert(all.end(), copyright.begin(), copyright.end());
        all.insert(all.end(), compiler.begin(), compiler.end());
        all.insert(all.end(), buildInfo.begin(), buildInfo.end());

        allocator.allocate(nbRows, 2, &(all[0]));
    */
}

int ScilabJavaEnvironment::extract(int id, int * args, int argsSize)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < argsSize - 1; i++)
        {
            os << args[i] << ", ";
        }
        os << args[argsSize - 1];
        os.flush();

        writeLog("extract", "Extraction on object %d with arguments: %s.", id, os.str().c_str());
    }

    /*    PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        if (PyDict_Check(obj))
        {
            if (argsSize != 1)
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot extract more than one element from a dictionary"));
            }

            PyObject * key = scope.getObject(*args);
            if (!obj)
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid key object"));
            }

            if (!PyDict_Contains(obj, key))
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid key"));
            }

            PyObject * value = PyDict_GetItem(obj, key);
    	Py_INCREF(value);

            int ret = scope.addObject(value);
            writeLog("extract", "returned id: %d.", ret);

            return ret;
            }
    */

    throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot extract from Java object"));
}

void ScilabJavaEnvironment::insert(int id, int * args, int argsSize)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < argsSize - 1; i++)
        {
            os << args[i] << ", ";
        }
        os << args[argsSize - 1];
        os.flush();

        writeLog("insert", "Insertion on object %d with arguments: %s.", id, os.str().c_str());
    }
    /*
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        if (argsSize != 2)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot insert more than one element in a dictionary"));
        }

        PyObject * key = scope.getObject(args[0]);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid key object"));
        }

        PyObject * value = scope.getObject(args[1]);
        if (!value)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid value object"));
        }

        if (PyDict_Check(obj))
        {
            PyDict_SetItem(obj, key, value);

            writeLog("insert", "success.");

            return;
        }
        else
        {
            if (!PyString_Check(key))
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid key object: A string expected"));
            }

            int ret = PyObject_SetAttr(obj, key, value);
            if (ret == -1)
            {
                if (PyErr_Occurred())
                {
                    PyObject *type, *value, *traceback;
                    PyErr_Fetch(&type, &value, &traceback);
                    PyErr_NormalizeException(&type, &value, &traceback);
                    PyErr_Clear();

                    throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot evaluate the code"));
                }
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot set the attribute."));
            }

            writeLog("insert", "success.");

            return;
        }
    */
    throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot insert in Java object"));
}

void ScilabJavaEnvironment::garbagecollect() { }

void ScilabJavaEnvironment::addtoclasspath(const char * path)
{
    writeLog("addtoclasspath", "Add the path %s to syspath", path);
    /*
        PyObject * syspath = PySys_GetObject(const_cast<char *>("path"));
        PyObject * _path = PyString_FromString(path);
        PyList_Append(syspath, _path);
    */
}

void ScilabJavaEnvironment::getclasspath(const ScilabStringStackAllocator & allocator)
{
    writeLog("getclasspath", "Get syspath");
    /*
        PyObject * syspath = PySys_GetObject(const_cast<char *>("path"));
        int size = PyList_Size(syspath);
        char ** arr = new char*[size];

        for (int i = 0; i < size; i++)
        {
            PyObject * item = PyList_GetItem(syspath, i);
            arr[i] = strdup(PyString_AsString(item));
        }

        allocator.allocate(size, 1, arr);
        for (int i = 0; i < size; i++)
        {
            free(arr[i]);
        }
        delete arr;
    */
}

void ScilabJavaEnvironment::addNamedVariable(int id, const char * varName)
{
    writeLog("addNamedVariable", "Associate the variable named %s with object with id %d.", varName, id);

    /*    PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        PyObject * _main_ = PyImport_AddModule("__main__");
        if (!_main_)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot evaluate the code since __main__ is not accessible."));
        }

        if (PyObject_SetAttrString(_main_, varName, obj) == -1)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot set the variable named %s."), varName);
        }
    */
}

int ScilabJavaEnvironment::getNamedVariable(const char * varName)
{
    writeLog("getNamedVariable", "Get the variable named %s.", varName);
    /*
        PyObject * _main_ = PyImport_AddModule("__main__");
        if (!_main_)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot evaluate the code since __main__ is not accessible."));
        }

        PyObject * var = PyObject_GetAttrString(_main_, varName);
        if (!var)
        {
            if (PyErr_Occurred())
            {
                PyObject *type, *value, *traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot get the variable value"));
            }
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot get the variable value"));
        }

        int ret = scope.addObject(var);

        writeLog("getNamedVariable", "returned id %d.", ret);

        return ret;
    */
    return 0;
}

void ScilabJavaEnvironment::evalString(const char ** code, int nbLines, ScilabStringStackAllocator * allocator)
{
    writeLog("evalString", "Evaluate code: %s...(truncated)", *code);
    /*
        std::ostringstream os;
        for (int i = 0; i < nbLines; i++)
        {
            os << code[i] << std::endl;
        }
        os.flush();

        PyObject * _main_ = PyImport_AddModule("__main__");
        if (!_main_)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot evaluate the code since __main__ is not accessible."));
        }

        std::ostream * old;
        std::ostringstream os1;
        if (allocator)
        {
            old = ScilabJavaOStream::setStdOutStream(&os1);
        }

        PyObject * dict = PyModule_GetDict(_main_);
        PyObject * ret = PyRun_StringFlags(os.str().c_str(), Py_file_input, dict, dict, 0);

        if (allocator)
        {
            ScilabJavaOStream::setStdOutStream(old);
        }

        if (!ret)
        {
            if (PyErr_Occurred())
            {
                PyObject *type, *value, *traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot evaluate the code"));
            }
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot evaluate the code"));
        }

        if (allocator)
        {
            std::vector<char *> buf = breakInLines(os1.str());
            allocator->allocate(buf.size(), 1, &(buf[0]));
            for (std::vector<char *>::iterator i = buf.begin(); i != buf.end(); i++)
            {
                free(*i);
            }
        }

        Py_DECREF(ret);
    */
}

int ScilabJavaEnvironment::createarray(char * className, int * dims, int len)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < len - 1; i++)
        {
            os << dims[i] << ", ";
        }
        os << dims[len - 1] << std::flush;

        writeLog("createarray", "Create a multi-list with dimensions %s.", os.str().c_str());
    }
    /*
        PyObject * obj = 0;
        if (len == 0)
        {
            obj = PyList_New(0);
            return scope.addObject(obj);
        }

        for (int i = 0; i < len; i++)
        {
            if (dims[i] < 0)
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid dimension in list creation"));
            }
        }

        obj = createMultiList(dims, len);
        int ret = scope.addObject(obj);

        writeLog("createarray", "returned id %d.", ret);

        return ret;
    */
    return 0;
}

int ScilabJavaEnvironment::loadclass(char * className, char * currentSciPath, bool isNamedVarCreated, bool allowReload)
{
    writeLog("loadclass", "Load the module %s and allowReload is set to %s", className, allowReload ? "true" : "false");
    JavaVM *vm = getScilabJavaVM();
    return ScilabClassLoader::loadJavaClass(vm, className, TRUE);
}

void ScilabJavaEnvironment::getrepresentation(int id, const ScilabStringStackAllocator & allocator)
{
    writeLog("getrepresentation", "Get the representation of object %d.", id);
    JavaVM *vm = getScilabJavaVM();
    char *str = ScilabJavaObject::getRepresentation(vm, id);
    allocator.allocate(1, 1, &str);
}

std::string ScilabJavaEnvironment::getrepresentation(int id)
{
    writeLog("getrepresentation", "Get the representation of object %d.", id);
    JavaVM *vm = getScilabJavaVM();
    return std::string(ScilabJavaObject::getRepresentation(vm, id));
}

bool ScilabJavaEnvironment::isvalidobject(int id)
{
    /*    bool ret = scope.isValid(id);
        writeLog("isvalidobject", "Test the validity of object %d which is%s valid.", id, ret ? "" : " not");

        return ret;*/
    return 0;
}

int ScilabJavaEnvironment::newinstance(int id, int * args, int argsSize)
{
    JavaVM *vm = getScilabJavaVM();
    return ScilabJavaClass::newInstance(vm, id, args, argsSize);
    //    throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operation: newinstance."));
}

int ScilabJavaEnvironment::operation(int idA, int idB, const OperatorsType type)
{
    writeLog("operation", "Evalute an operation (%d) with objects with id %d and %d.", (int)type, idA, idB);
    /*
        const char * opName = getOpNameFromType(type);

        PyObject * _operator_ = PyImport_AddModule("operator");
        if (!_operator_)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot get the operator module."));
        }

        PyObject * objA = scope.getObject(idA);
        if (!objA)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), idA);
        }

        PyObject * objB = 0;
        if (idB != -1)
        {
            // Binary op
            objB = scope.getObject(idB);
            if (!objB)
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), idB);
            }
        }

        PyObject * _op_ = PyObject_GetAttrString(_operator_, opName);

        PyObject * pArgs = PyTuple_New(objB ? 2 : 1);
        Py_INCREF(objA);
        PyTuple_SetItem(pArgs, 0, objA);

        if (objB)
        {
            Py_INCREF(objB);
            PyTuple_SetItem(pArgs, 1, objB);
        }

        PyObject * result = PyObject_Call(_op_, pArgs, 0);
        Py_DECREF(pArgs);
        Py_DECREF(_op_);

        if (!result)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to make the operation (%s)."), opName);
            }
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Unable to make the operation (%s)."), opName);
        }

        int ret = scope.addObject(result);

        writeLog("operation", "returned id %d.", ret);

        return ret;
    */
    return 0;
}

int * ScilabJavaEnvironment::invoke(int id, const char * methodName, int * args, int argsSize)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < argsSize - 1; i++)
        {
            os << args[i] << ", ";
        }
        os << args[argsSize - 1];
        os.flush();

        writeLog("invoke", "Invoke the method %s on object %d with arguments: %s.", methodName, id, os.str().c_str());
    }

    JavaVM *vm = getScilabJavaVM();
    int ret = ScilabJavaObject::invoke(vm, id, methodName, args, argsSize);
    return 0;
    /*
        if (*methodName == '\0')
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid method name"));
        }

        if (!helper.getShowPrivate() && *methodName == '_')
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Private method: %s"), methodName);
        }

            PyObject * obj = scope.getObject(id);
            if (!obj)
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
            }

            if (!PyObject_HasAttrString(obj, methodName))
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid method name: %s"), methodName);
            }

            PyObject * method = PyObject_GetAttrString(obj, methodName);
            if (!method)
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid method name: %s"), methodName);
            }

            if (!PyCallable_Check(method))
            {
                Py_DECREF(method);
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid method name: %s"), methodName);
            }

            PyObject * pArgs = PyTuple_New(argsSize);
            for (int i = 0; i < argsSize; i++)
            {
                PyObject * obj = scope.getObject(args[i]);
                if (!obj)
                {
                    Py_DECREF(pArgs);
                    Py_DECREF(method);
                    throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
                }
                Py_INCREF(obj);
                PyTuple_SetItem(pArgs, i, obj);
            }

            PyObject * result = PyObject_Call(method, pArgs, 0);
            Py_DECREF(pArgs);
            Py_DECREF(method);

            if (!result)
            {
                if (PyErr_Occurred())
                {
                    PyObject * type, * value, * traceback;
                    PyErr_Fetch(&type, &value, &traceback);
                    PyErr_NormalizeException(&type, &value, &traceback);
                    PyErr_Clear();

                    throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to invoke the method: %s"), methodName);
                }
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Unable to invoke the method: %s"), methodName);
            }

            int * ret = new int[2];
            *ret = 1;

            if (result == Py_None)
            {
                ret[1] = VOID_OBJECT;
                writeLog("invoke", "The method returned void.");

                return ret;
            }

            ret[1] = scope.addObject(result);
            writeLog("invoke", "returned id %d.", ret[1]);

            return ret;
        */
    return 0;
}

void ScilabJavaEnvironment::setfield(int id, const char * fieldName, int idarg)
{
    writeLog("setfield", "Set the field named %s with value id %d on object with id %d.", fieldName, idarg, id);

    if (*fieldName == '\0')
    {
        throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid field name"));
    }

    if (!helper.getShowPrivate() && *fieldName == '_')
    {
        throw ScilabJavaException(__LINE__, __FILE__, gettext("Private field: %s"), fieldName);
    }

    /*
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        if (!PyObject_HasAttrString(obj, fieldName))
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid field name: %s"), fieldName);
        }

        PyObject * value = scope.getObject(idarg);
        if (!value)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        int ret = PyObject_SetAttrString(obj, fieldName, value);
        if (ret == -1)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to set the field: %s"), fieldName);
            }
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Unable to set the field: %s"), fieldName);
        }
    */
    writeLog("setfield", "Value successfully set.");
}

int ScilabJavaEnvironment::getfield(int id, const char * fieldName)
{
    writeLog("getfield", "Get the field named %s on object with id %d.", fieldName, id);

    JavaVM *vm = getScilabJavaVM();
    int ret = ScilabJavaObject::getField(vm, id, fieldName);
    return ret;
    //    allocator.allocate(1, 1, &str);



    /*
    if (*fieldName == '\0')
    {
        throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid field name"));
    }

    if (!helper.getShowPrivate() && *fieldName == '_')
    {
        throw ScilabJavaException(__LINE__, __FILE__, gettext("Private field: %s"), fieldName);
    }

        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        if (!PyObject_HasAttrString(obj, fieldName))
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid field name: %s"), fieldName);
        }

        PyObject * field = PyObject_GetAttrString(obj, fieldName);
        if (!field)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to get the field value: %s"), fieldName);
            }
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Unable to get the field value: %s"), fieldName);
        }

        int ret = scope.addObject(field);
        writeLog("getfield", "returned id %d.", ret);

        return ret;
    */
    return 0;
}

int ScilabJavaEnvironment::getfieldtype(int id, const char * fieldName)
{
    writeLog("getfieldtype", "Get the type of the field %s on object with id %d.", fieldName, id);

    if ((!helper.getShowPrivate() && *fieldName == '_') || *fieldName == '\0')
    {
        writeLog("getfieldtype", "Return NONE.");
        return -1;
    }
    /*
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        if (!PyObject_HasAttrString(obj, fieldName))
        {
            writeLog("getfieldtype", "Return NONE.");
            return -1;
        }

        PyObject * field = PyObject_GetAttrString(obj, fieldName);
        if (!field)
        {
            writeLog("getfieldtype", "Return NONE.");
            return -1;
        }

        if (PyCallable_Check(field))
        {
            Py_DECREF(field);
            writeLog("getfieldtype", "Return METHOD.");
            return 0;
        }
        else
        {
            Py_DECREF(field);
            writeLog("getfieldtype", "Return FIELD.");
            return 1;
        }
    */
    return 0;
}

int ScilabJavaEnvironment::getarrayelement(int id, int * index, int length)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < length - 1; i++)
        {
            os << index[i] << ", ";
        }
        os << index[length - 1];
        os.flush();

        writeLog("getarrayelement", "Get element from array with id %d and with index: %s.", id, os.str().c_str());
    }
    /*
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        if (!PyList_Check(obj) && !PyArray_Check(obj))
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Not a list or an array"));
        }

        if (length == 0)
        {
            return 0;
        }

        if (PyList_Check(obj))
        {
            for (int i = 0; i < length; i++)
            {
                if (index[i] < 0 || index[i] >= PyList_Size(obj))
                {
                    throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid index"));
                }
                obj = PyList_GetItem(obj, index[i]);
                if (i != length - 1 && !PyList_Check(obj))
                {
                    throw ScilabJavaException(__LINE__, __FILE__, gettext("Not a list"));
                }
            }
    	Py_INCREF(obj);
        }
        else if (PyArray_Check(obj))
        {
            PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
            npy_intp * ind = reinterpret_cast<npy_intp *>(index);

            if (length != PyArray_NDIM(arr))
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid index dimension"));
            }

            npy_intp * dims = PyArray_DIMS(arr);

            if (sizeof(int) != sizeof(npy_intp))
            {
                ind = new npy_intp[length];
                for (int i = 0; i < length; i++)
                {
                    if (index[i] < 0 || index[i] >= dims[i])
                    {
                        delete[] ind;
                        throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid index at position %d"), i + 1);
                    }

                    ind[i] = static_cast<npy_intp>(index[i]);
                }
            }

            obj = PyArray_GETITEM(arr, PyArray_GetPtr(arr, ind));

            if (sizeof(int) != sizeof(npy_intp))
            {
                delete[] ind;
            }
        }
        else
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Must be a list or a Numpy array"));
        }

        int ret = scope.addObject(obj);
        writeLog("getarrayelement", "returned id %d.", ret);

        return ret;
    */
    return 0;
}

void ScilabJavaEnvironment::setarrayelement(int id, int * index, int length, int idArg)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < length - 1; i++)
        {
            os << index[i] << ", ";
        }
        os << index[length - 1];
        os.flush();

        writeLog("setarrayelement", "Set element with id %d in array with id %d and with index: %s.", idArg, id, os.str().c_str());
    }
    /*
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        PyObject * value = scope.getObject(idArg);
        if (!value)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        if (PyList_Check(obj))
        {
            if (length == 0)
            {
                return;
            }

            for (int i = 0; i < length - 1; i++)
            {
                if (index[i] < 0 || index[i] >= PyList_Size(obj))
                {
                    throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid index at position %d"), i + 1);
                }
                obj = PyList_GetItem(obj, index[i]);
                if (!PyList_Check(obj))
                {
                    throw ScilabJavaException(__LINE__, __FILE__, gettext("Not a list at position %d"), index[i]);
                }
            }

            Py_INCREF(value);
            PyList_SetItem(obj, index[length - 1], value);
        }
        else if (PyArray_Check(obj))
        {
            PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
            npy_intp * ind = reinterpret_cast<npy_intp *>(index);

            if (length != PyArray_NDIM(arr))
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid index dimension"));
            }

            npy_intp * dims = PyArray_DIMS(arr);

            if (sizeof(int) != sizeof(npy_intp))
            {
                ind = new npy_intp[length];
                for (int i = 0; i < length; i++)
                {
                    if (index[i] < 0 || index[i] >= dims[i])
                    {
                        delete[] ind;
                        throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid index at position %d"), i);
                    }

                    ind[i] = static_cast<npy_intp>(index[i]);
                }
            }

            int ret = PyArray_SETITEM(arr, PyArray_GetPtr(arr, ind), value);

            if (sizeof(int) != sizeof(npy_intp))
            {
                delete[] ind;
            }

            if (ret == -1)
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot set the value in the array"));
            }
        }
    */
    writeLog("setarrayelement", "Successfully set");
}

int ScilabJavaEnvironment::cast(int id, char * className)
{
    throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operation"));
}

int ScilabJavaEnvironment::castwithid(int id, int classId)
{
    throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operation"));
}

void ScilabJavaEnvironment::removeobject(int id)
{
    writeLog("removeobject", "Remove object with id %d.", id);
    //    scope.removeObject(id);
    //    ScilabAutoCleaner::unregisterVariable(envId, id);
}

void ScilabJavaEnvironment::autoremoveobject(int id)
{
    //    scope.removeObject(id);
}

void ScilabJavaEnvironment::getaccessiblemethods(int id, const ScilabStringStackAllocator & allocator)
{
    writeLog("getaccessiblemethods", "Get accessible methods on object with id %d.", id);
    JavaVM *vm = getScilabJavaVM();
    getMethodResult(vm, "getAccessibleMethods", id, allocator);
}

void ScilabJavaEnvironment::getaccessiblefields(int id, const ScilabStringStackAllocator & allocator)
{
    writeLog("getaccessiblefields", "Get accessible fields on object with id %d.", id);
    JavaVM *vm = getScilabJavaVM();
    getMethodResult(vm, "getAccessibleFields", id, allocator);
    getAccessibleFields(id, allocator, true);
}

std::string ScilabJavaEnvironment::getclassname(int id)
{
    writeLog("getclassname", "Get the class name of object with id %d.", id);
    JavaVM *vm = getScilabJavaVM();
    return std::string(ScilabJavaObject::getClassName(vm, id));

    /*
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        PyObject * __name__ = PyObject_GetAttrString(obj, "__name__");
        if (!__name__)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot get the name of the object."));
        }

        char * name = PyString_AsString(__name__);
        std::string str = std::string(name);
        Py_DECREF(__name__);

        return str;
    */
    return 0;
}

VariableType ScilabJavaEnvironment::isunwrappable(int id)
{
    writeLog("isunwrappable", "Test if the object with id %d is unwrappable.", id);

    return wrapper.isunwrappable(id);
}

int ScilabJavaEnvironment::compilecode(char * className, char ** code, int size)
{
    writeLog("compilecode", "Compile the code %s...", *code);

    std::ostringstream os;
    for (int i = 0; i < size; i++)
    {
        os << code[i] << std::endl;
    }
    os.flush();
    /*
        PyObject * obj = Py_CompileString(os.str().c_str(), className, 0);
        if (!obj)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to compile the given code"));
            }
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Unable to compile the given code"));
        }

        return scope.addObject(obj);
    */
    return 0;
}

void ScilabJavaEnvironment::enabletrace(const char * filename)
{
    if (!traceEnabled)
    {
        file = new std::ofstream(filename, std::ios::out | std::ios::trunc);
        if (!file || file->fail())
        {
            if (file)
            {
                file->close();
                delete file;
            }

            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot open the given file %s."), filename);
        }
        traceEnabled = true;
    }
    else
    {
        throw ScilabJavaException(__LINE__, __FILE__, gettext("Trace already enabled"));
    }
}

void ScilabJavaEnvironment::disabletrace()
{
    if (traceEnabled)
    {
        traceEnabled = false;
        file->close();
        delete file;
        file = 0;
    }
}

void ScilabJavaEnvironment::writeLog(const std::string & fun, const std::string str, ...) const
{
    if (traceEnabled)
    {
        char _str[LOG_BUFFER_SIZE];
        va_list args;

        va_start(args, str);
        vsnprintf(_str, LOG_BUFFER_SIZE, str.c_str(), args);
        va_end(args);

        *file << fun << ": " << _str << std::endl;
    }
}

#define SCILABJAVAOBJECT "org/scilab/modules/external_objects_java/ScilabJavaObject"

void ScilabJavaEnvironment::getMethodResult(JavaVM * jvm_, const char * const methodName, int id, const ScilabStringStackAllocator & allocator)
{
    JNIEnv * curEnv = NULL;
    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
    jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);
    if (cls == NULL)
    {
        throw GiwsException::JniClassNotFoundException(curEnv, SCILABJAVAOBJECT);
    }

    jmethodID jobjectArray_getAccessibleMethodsjintID = curEnv->GetStaticMethodID(cls, methodName, "(I)[Ljava/lang/String;");
    if (jobjectArray_getAccessibleMethodsjintID == NULL)
    {
        throw GiwsException::JniMethodNotFoundException(curEnv, methodName);
    }

    jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(cls, jobjectArray_getAccessibleMethodsjintID, id));
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
    jint lenRow = curEnv->GetArrayLength(res);
    jboolean isCopy = JNI_FALSE;

    char **addr = new char*[lenRow];
    jstring *resString = new jstring[lenRow];

    for (jsize i = 0; i < lenRow; i++)
    {
        resString[i] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(res, i));
        addr[i] = const_cast<char *>(curEnv->GetStringUTFChars(resString[i], &isCopy));
    }
    int lenCol = lenRow == 0 ? 0 : 1;
    allocator.allocate(lenRow, lenCol, addr);
    /*
            SciErr err = createMatrixOfString(pvApiCtx, pos, lenCol, lenRow, addr);

            for (jsize i = 0; i < lenRow; i++)
            {
                curEnv->ReleaseStringUTFChars(resString[i], addr[i]);
                curEnv->DeleteLocalRef(resString[i]);
                }*/
    delete[] addr;
    delete[] resString;

    // if (err.iErr)
    // {
    //     throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
    // }

    curEnv->DeleteLocalRef(res);
    curEnv->DeleteLocalRef(cls);
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
};

}
