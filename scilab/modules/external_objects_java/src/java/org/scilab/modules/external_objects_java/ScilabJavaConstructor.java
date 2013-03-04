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
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

/**
 * A constructor wrapper
 *
 * @author Calixte DENIZET
 */
public class ScilabJavaConstructor {

    protected Class<?> clazz;

    /**
     * Constructor
     * @param clazz the class where the constructor belongs to
     */
    public ScilabJavaConstructor(Class clazz) {
        this.clazz = clazz;
    }

    /**
     * @param args the ids of arguments
     * @return the resulting object
     */
    public Object invoke(int[] args) throws ScilabJavaException {
        int nbargs = args.length;
        Class[] cls = new Class[nbargs];
        Object[] argsO = new Object[nbargs];

        for (int i = 0; i < nbargs; i++) {
            argsO[i] = ScilabJavaObject.arraySJO[args[i]].object;
            cls[i] = ScilabJavaObject.arraySJO[args[i]].clazz;
        }

        try {
            return findConstructor(clazz, cls, argsO).newInstance(argsO);
        } catch (IllegalAccessException e) {
            throw new ScilabJavaException("Illegal access to the constructor of class " + clazz.getName() + ".");
        } catch (IllegalArgumentException e) {
            throw new ScilabJavaException("Illegal argument in the constructor of class " + clazz.getName() + " : \n" + e.getMessage());
        } catch (InstantiationException e) {
            throw new ScilabJavaException("The class " + clazz.getName() + "is abstract and cannot be instantiated.");
        } catch (ExceptionInInitializerError e) {
            throw new ScilabJavaException("Initializer error with constructor of class " + clazz.getName() + " :\n" + e.getMessage());
        } catch (InvocationTargetException e) {
            throw new ScilabJavaException("An exception has been thrown in calling the constructor of class " + clazz.getName() + " :\n" + e.getMessage());
        } catch (NoSuchMethodException e) {
            throw new ScilabJavaException("No such constructor in the class " + clazz.getName() + ".");
        }
    }

    /**
     * Find a valid constructor. An argument can be null (thanks to Fabien Viale).
     * @param clazz the class where to search the constructor
     * @param argsClass the class of the arguments
     * @param args the arguments
     * @return a valid constructor
     */
    public static Constructor findConstructor(Class clazz, Class[] argsClass, Object[] args) throws NoSuchMethodException {
        Constructor[] all = clazz.getConstructors();
        boolean modified = false;
        do {
            for (Constructor cstr : all) {
                Class[] types = cstr.getParameterTypes();
                if (types.length == argsClass.length) {
                    boolean ok = true;
                    for (int i = 0; i < types.length; i++) {
                        if (args[i] != null && !argsClass[i].equals(Void.class) && !types[i].isAssignableFrom(argsClass[i])) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        return cstr;
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
}
