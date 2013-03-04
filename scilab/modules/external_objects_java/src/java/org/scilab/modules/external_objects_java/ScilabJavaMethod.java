/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.external_objects_java;

import java.util.HashMap;
import java.util.Map;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.InvocationTargetException;

/**
 * A method wrapper
 *
 * @author Calixte DENIZET
 */
public class ScilabJavaMethod {

    protected Class<?> clazz;
    protected String name;

    //We keep in memory the map between class and accessible methods
    protected static Map<Class, Map<String, ScilabJavaMethod>> methsInClass = new HashMap<Class, Map<String, ScilabJavaMethod>>();

    /**
     * Constructor
     * @param clazz the class where the method belongs to
     * @param name the method name
     */
    public ScilabJavaMethod(Class clazz, String name) {
        this.clazz = clazz;
        this.name = name;
    }

    /**
     * @param obj the object where invoking method
     * @param returnType the class of the result
     * @param args the ids of arguments
     * @return the resulting object
     */
    public Object invoke(Object obj, Class[] returnType, int[] args) throws ScilabJavaException {
        int nbargs = args.length;
        Class[] cl = new Class[nbargs];
        Object[] argsO = new Object[nbargs];

        for (int i = 0; i < nbargs; i++) {
            argsO[i] = ScilabJavaObject.arraySJO[args[i]].object;
            cl[i] = ScilabJavaObject.arraySJO[args[i]].clazz;
        }

        return call(obj, returnType, argsO, cl);
    }

    /**
     * @param obj the object where invoking method
     * @param returnType the class of the result
     * @param args the Object arguments
     * @param argsClass the Class of the arguments
     * @return the resulting object
     */
    protected Object call(Object obj, Class[] returnType, Object[] args, Class[] argsClass) throws ScilabJavaException {
        try {
            Method meth = findMethod(name, clazz, argsClass, args);
            returnType[0] = meth.getReturnType();

            if (Modifier.isStatic(meth.getModifiers())) {
                return meth.invoke(null, args);
            }
            return meth.invoke(obj, args);
        } catch (IllegalAccessException e) {
            throw new ScilabJavaException("Illegal access to the method " + name + ".");
        } catch (IllegalArgumentException e) {
            throw new ScilabJavaException("Illegal argument in the method " + name + " : \n" + e.getMessage());
        } catch (NullPointerException e) {
            throw new ScilabJavaException("The method " + name + " is called on a null object.");
        } catch (ExceptionInInitializerError e) {
            throw new ScilabJavaException("Initializer error with method " + name + " :\n" + e.getMessage());
        } catch (InvocationTargetException e) {
            throw new ScilabJavaException("An exception has been thrown in calling the method " + name + " :\n" + e.getCause().toString());
        } catch (NoSuchMethodException e) {
            throw new ScilabJavaException("No method " + name + " in the class " + clazz.getName() + " or bad arguments type.");
        }
    }

    /**
     * Find a valid constructor. An argument can be null (thanks to Fabien Viale).
     * @param clazz the class where to search the constructor
     * @param argsClass the class of the arguments
     * @param args the arguments
     * @return a valid constructor
     */
    public static Method findMethod(String name, Class clazz, Class[] argsClass, Object[] args) throws NoSuchMethodException {
        Method[] all = clazz.getMethods();
        String internName = name.intern();
        boolean modified = false;
        do {
            for (Method meth : all) {
                if (meth.getName() == internName) {
                    Class[] types = meth.getParameterTypes();
                    if (types.length == argsClass.length) {
                        boolean ok = true;
                        for (int i = 0; i < types.length; i++) {
                            if (args[i] != null && !argsClass[i].equals(Void.class) && !types[i].isAssignableFrom(argsClass[i])) {
                                ok = false;
                                break;
                            }
                        }
                        if (ok) {
                            return meth;
                        }
                    }
                }
            }

            if (!modified) {
                /* In scilab it could be boring to write str.substring(int32(1),int32(5))
                   so we search if a double is an integer and we convert it into an int
                   and retry to find the method */
                for (int i = 0; i < args.length; i++) {
                    if (argsClass[i] == double.class && ((Double) args[i]).intValue() == ((Double) args[i]).doubleValue()) {
                        argsClass[i] = int.class;
                        args[i] = ((Double) args[i]).intValue();
                        modified = true;
                    }
                }
            } else {
                modified = false;
            }
        } while (modified);

        throw new NoSuchMethodException("");
    }

    /**
     * @param clazz the class where to search methods
     * @return the method belonging to clazz
     */
    public static Map<String, ScilabJavaMethod> getMethods(Class clazz) {
        Map<String, ScilabJavaMethod> hm = methsInClass.get(clazz);

        if (hm != null) {
            return hm;
        }

        hm = new HashMap<String, ScilabJavaMethod>();
        Method[] meth = clazz.getMethods();

        for (int i = 0; i < meth.length; i++) {
            int modif = meth[i].getModifiers();
            if (Modifier.isPublic(modif)) {
                String name = meth[i].getName();
                if (!hm.containsKey(name)) {
                    hm.put(name, new ScilabJavaMethod(clazz, name));
                }
            }
        }
        methsInClass.put(clazz, hm);

        return hm;
    }
}
