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

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.Map;

/**
 * Main class to communicate with Scilab via jni interface generated with Giws.
 * @autor Calixte DENIZET
 */
public class ScilabJavaObject {

    private static final int INITIALCAPACITY = 1024;
    private static final Map<Class, Integer> unwrappableType = new HashMap<Class, Integer>(27);
    private static final Class[] returnType = new Class[1];

    private static int currentPos = 1;
    private static FreePlace freePlace = new FreePlace();

    protected static int currentCapacity = INITIALCAPACITY;
    protected static ScilabJavaObject[] arraySJO = new ScilabJavaObject[currentCapacity];

    static {
        unwrappableType.put(double.class, 0);
        unwrappableType.put(double[].class, 1);
        unwrappableType.put(double[][].class, 2);
        unwrappableType.put(String.class, 3);
        unwrappableType.put(String[].class, 4);
        unwrappableType.put(String[][].class, 5);
        unwrappableType.put(int.class, 6);
        unwrappableType.put(int[].class, 7);
        unwrappableType.put(int[][].class, 8);
        unwrappableType.put(boolean.class, 9);
        unwrappableType.put(boolean[].class, 10);
        unwrappableType.put(boolean[][].class, 11);
        unwrappableType.put(byte.class, 12);
        unwrappableType.put(byte[].class, 13);
        unwrappableType.put(byte[][].class, 14);
        unwrappableType.put(short.class, 15);
        unwrappableType.put(short[].class, 16);
        unwrappableType.put(short[][].class, 17);
        unwrappableType.put(char.class, 18);
        unwrappableType.put(char[].class, 19);
        unwrappableType.put(char[][].class, 20);
        unwrappableType.put(float.class, 21);
        unwrappableType.put(float[].class, 22);
        unwrappableType.put(float[][].class, 23);
        unwrappableType.put(long.class, 24);
        unwrappableType.put(long[].class, 25);
        unwrappableType.put(long[][].class, 26);
        arraySJO[0] = new ScilabJavaObject(null, null);
    }

    protected Map<String, ScilabJavaMethod> methods;
    protected Object object;
    protected Class clazz;
    protected int id;

    /**
     * Constructor
     * @param obj the Java Object to wrap
     */
    public ScilabJavaObject(Object obj) {
        this(obj, obj == null ? null : obj.getClass());
    }

    /**
     * Constructor
     * @param obj the Java Object to wrap
     * @param clazz the Java Object class
     */
    public ScilabJavaObject(Object obj, Class clazz) {
        this.object = obj;
        this.clazz = clazz;

        if (obj != null) {
            int fp = freePlace.getFreePlace();
            if (fp == -1) {
                this.id = currentPos;
                ++currentPos;
            } else {
                this.id = fp;
            }

            arraySJO[this.id] = this;

            if (currentPos >= currentCapacity) {
                currentCapacity = currentCapacity * 2;
                ScilabJavaObject[] arr = new ScilabJavaObject[currentCapacity];
                System.arraycopy(arraySJO, 0, arr, 0, currentPos);
                arraySJO = arr;
            }

            this.methods = ScilabJavaMethod.getMethods(clazz);
        } else {
            this.id = 0;
        }
    }

    /**
     * Do nothing !
     */
    public static void initScilabJavaObject() { }

    /**
     * {@inheritedDoc}
     */
    public String toString() {
        return object == null ? "null" : object.toString();
    }

    /**
     * @param id the Java Object id
     * @return the string to represent this object
     */
    public static String getRepresentation(final int id) {
        if (arraySJO[id] != null) {
            return arraySJO[id].toString();
        }

        return "Invalid Java object";
    }

    /**
     * @param id the Java Object id
     * @param index an array of index
     * @return the id of the element at position given by index in id
     */
    public static int getArrayElement(final int id, final int[] index) throws ScilabJavaException {
        if (id > 0) {
            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            return new ScilabJavaObject(ScilabJavaArray.get(arraySJO[id].object, index)).id;
        }
        throw new ScilabJavaException("null is not an array");
    }

    /**
     * @param id the Java Object id
     * @param index an array of index
     * @param idArg the id of an element to put at the position given by index
     */
    public static void setArrayElement(final int id, final int[] index, final int idArg) throws ScilabJavaException {
        if (id > 0) {
            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            ScilabJavaArray.set(arraySJO[id].object, index, arraySJO[idArg].object);
        } else {
            throw new ScilabJavaException("null is not an array");
        }
    }

    /**
     * @param id the Java Object id
     * @return the accessibles methods in the object represented by id
     */
    public static String[] getAccessibleMethods(final int id) throws ScilabJavaException {
        if (id > 0) {
            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }
            return arraySJO[id].methods.keySet().toArray(new String[0]);
        } else {
            return new String[0];
        }
    }

    /**
     * @param id the Java Object id
     * @return the accessibles fields in the object represented by id
     */
    public static String[] getAccessibleFields(final int id) throws ScilabJavaException {
        if (id > 0) {
            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            if (arraySJO[id].clazz.isArray()) {
                return new String[] {"length"};
            }

            final Field[] f = arraySJO[id].clazz.getFields();
            final String[] sf = new String[f.length];
            for (int i = 0; i < f.length; i++) {
                sf[i] = f[i].getName();
            }
            return sf;
        } else {
            return new String[0];
        }
    }

    /**
     * @param id the Java Object id
     * @return the class name of the object represented by id
     */
    public static String getClassName(final int id) throws ScilabJavaException {
        if (id > 0) {
            if (arraySJO[id] == null) {
                throw new ScilabJavaException("Invalid Java object");
            }

            return arraySJO[id].clazz.getName();
        } else {
            return "null";
        }
    }

    /**
     * @param id the Java Object id
     * @param fieldName the field name to set
     * @param idArg the id of the element to set
     */
    public static void setField(final int id, final String fieldName, final int idarg) throws ScilabJavaException {
        if (id > 0) {
            Field f = null;
            try {
                if (arraySJO[id] == null) {
                    throw new ScilabJavaException("Invalid Java object");
                }

                f = arraySJO[id].clazz.getField(fieldName);
                f.set(arraySJO[id].object, arraySJO[idarg].object);
            } catch (NoSuchFieldException e) {
                throw new ScilabJavaException("No field " + fieldName + " in object " + getClassName(id));
            } catch (IllegalArgumentException e) {
                if (f != null && f.getType() == int.class && arraySJO[idarg].clazz == double.class
                        && ((Double) arraySJO[idarg].object).intValue() == ((Double) arraySJO[idarg].object).doubleValue()) {
                    try {
                        f.set(arraySJO[id].object, ((Double) arraySJO[idarg].object).intValue());
                        return;
                    } catch (IllegalArgumentException ee) {
                        throw new ScilabJavaException("Bad argument value for field " + fieldName + " in object " + getClassName(id));
                    } catch (IllegalAccessException ee) {
                        throw new ScilabJavaException("Cannot access to the field " + fieldName + " in object " + getClassName(id));
                    }
                }

                throw new ScilabJavaException("Bad argument value for field " + fieldName + " in object " + getClassName(id));
            } catch (IllegalAccessException e) {
                throw new ScilabJavaException("Cannot access to the field " + fieldName + " in object " + getClassName(id));
            }
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * @param id the Java Object id
     * @param fieldName the field name to set
     * @return the id of the got object
     */
    public static int getField(final int id, final String fieldName) throws ScilabJavaException {
        if (id > 0) {
            try {
                if (arraySJO[id] == null) {
                    throw new ScilabJavaException("Invalid Java object");
                }

                if (arraySJO[id].clazz.isArray() && fieldName.equals("length")) {
                    return new ScilabJavaObject(Array.getLength(arraySJO[id].object), int.class).id;
                }

                final Field f = arraySJO[id].clazz.getField(fieldName);
                final Class cl = f.getType();
                if (cl == int.class) {
                    return new ScilabJavaObject(f.getInt(arraySJO[id].object), int.class).id;
                } else if (cl == double.class) {
                    return new ScilabJavaObject(f.getDouble(arraySJO[id].object), double.class).id;
                } else if (cl == boolean.class) {
                    return new ScilabJavaObject(f.getBoolean(arraySJO[id].object), boolean.class).id;
                } else if (cl == short.class) {
                    return new ScilabJavaObject(f.getShort(arraySJO[id].object), short.class).id;
                } else if (cl == char.class) {
                    return new ScilabJavaObject(f.getChar(arraySJO[id].object), char.class).id;
                } else if (cl == float.class) {
                    return new ScilabJavaObject(f.getFloat(arraySJO[id].object), float.class).id;
                } else if (cl == byte.class) {
                    return new ScilabJavaObject(f.getByte(arraySJO[id].object), byte.class).id;
                } else if (cl == long.class) {
                    return new ScilabJavaObject(f.getLong(arraySJO[id].object), long.class).id;
                }

                return new ScilabJavaObject(f.get(arraySJO[id].object)).id;
            } catch (NoSuchFieldException e) {
                throw new ScilabJavaException("No field " + fieldName + " in object " + getClassName(id));
            } catch (IllegalArgumentException e) {
                throw new ScilabJavaException("Bad argument value for field " + fieldName + " in object " + getClassName(id));
            } catch (IllegalAccessException e) {
                throw new ScilabJavaException("Cannot access to the field " + fieldName + " in object " + getClassName(id));
            }
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * @param id the Java Object id
     * @param fieldName the field name to set
     * @return the type of the field in object represented by id:
     *         * -1 for unknown
     *         * 0 for a method
     *         * 1 for a field
     */
    public static int getFieldType(final int id, final String fieldName) {
        if (id > 0 && arraySJO[id] != null) {
            if (arraySJO[id].methods.containsKey(fieldName)) {
                return 0;
            }
            try {
                if (arraySJO[id].clazz.isArray()) {
                    if (fieldName.equals("length")) {
                        return 1;
                    } else {
                        return -1;
                    }
                }
                arraySJO[id].clazz.getField(fieldName);
                return 1;
            } catch (NoSuchFieldException e) {
                return -1;
            } catch (IllegalArgumentException e) {
                return -1;
            }
        }

        return -1;
    }

    /**
     * @param id the Java Object id
     * @param methName the method name to invoke
     * @param args an array containing the id of the arguments
     * @return the id of the invocation result
     */
    public static int invoke(final int id, final String methName, final int[] args) throws ScilabJavaException {
        if (id > 0) {
            if (arraySJO[id] != null) {
                return new ScilabJavaObject(arraySJO[id].methods.get(methName).invoke(arraySJO[id].object, returnType, args), returnType[0]).id;
            }
            throw new ScilabJavaException("Invalid Java object");
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * @param id the Java Object id
     * @param className the target class name
     * @return the id of the cast result
     */
    public static int javaCast(final int id, final String className) throws ScilabJavaException {
        if (id > 0) {
            final int idC = ScilabClassLoader.loadJavaClass(className, false);
            final Class clazz = (Class) arraySJO[idC].object;
            try {
                int idR = new ScilabJavaObject(clazz.cast(arraySJO[id].object), clazz).id;
                removeScilabJavaObject(idC);
                return idR;
            } catch (ClassCastException e) {
                throw new ScilabJavaException("Cannot cast object " + getClassName(id) + " into " + getClassName(idC));
            }
        } else {
            throw new ScilabJavaException("null is not an object");
        }
    }

    /**
     * Remove an object.
     * @param id the Java Object id
     */
    public static void removeScilabJavaObject(final int id) {
        if (id > 0 && arraySJO[id] != null) {
            freePlace.addFreePlace(id);
            if (arraySJO[id] instanceof ScilabJavaClass) {
                ScilabClassLoader.removeID(id);
            }
            arraySJO[id].methods = null;
            arraySJO[id] = null;
        }
    }

    /**
     * Remove all the objects and start a garbage collection
     */
    public static void garbageCollect() {
        currentPos = 1;
        currentCapacity = INITIALCAPACITY;
        arraySJO = new ScilabJavaObject[currentCapacity];
        arraySJO[0] = new ScilabJavaObject(null, null);
        freePlace = new FreePlace();
        ScilabClassLoader.clazz.clear();
        System.gc();
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapAsDirectByteBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x, ByteBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapAsDirectDoubleBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asDoubleBuffer(), DoubleBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapAsDirectIntBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asIntBuffer(), IntBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapAsDirectCharBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asCharBuffer(), CharBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapAsDirectFloatBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asFloatBuffer(), FloatBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapAsDirectLongBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asLongBuffer(), LongBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapAsDirectShortBuffer(final ByteBuffer x) {
        x.order(ByteOrder.nativeOrder());
        return new ScilabJavaObject(x.asShortBuffer(), ShortBuffer.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapDouble(final double x) {
        return new ScilabJavaObject(x, double.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapDouble(final double[] x) {
        return new ScilabJavaObject(x, double[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapDouble(final double[][] x) {
        return new ScilabJavaObject(x, double[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapInt(final int x) {
        return new ScilabJavaObject(x, int.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapInt(final int[] x) {
        return new ScilabJavaObject(x, int[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapInt(final int[][] x) {
        return new ScilabJavaObject(x, int[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUInt(final long x) {
        return new ScilabJavaObject(x, long.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUInt(final long[] x) {
        return new ScilabJavaObject(x, long[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUInt(final long[][] x) {
        return new ScilabJavaObject(x, long[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapByte(final byte x) {
        return new ScilabJavaObject(x, byte.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapByte(final byte[] x) {
        return new ScilabJavaObject(x, byte[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapByte(final byte[][] x) {
        return new ScilabJavaObject(x, byte[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapShort(final short x) {
        return new ScilabJavaObject(x, short.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapShort(final short[] x) {
        return new ScilabJavaObject(x, short[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapShort(final short[][] x) {
        return new ScilabJavaObject(x, short[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUShort(final int x) {
        return new ScilabJavaObject(x, int.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUShort(final int[] x) {
        return new ScilabJavaObject(x, int[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUShort(final int[][] x) {
        return new ScilabJavaObject(x, int[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUByte(final short x) {
        return new ScilabJavaObject(x, short.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUByte(final short[] x) {
        return new ScilabJavaObject(x, short[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapUByte(final short[][] x) {
        return new ScilabJavaObject(x, short[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapString(final String x) {
        return new ScilabJavaObject(x, String.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapString(final String[] x) {
        return new ScilabJavaObject(x, String[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapString(final String[][] x) {
        return new ScilabJavaObject(x, String[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapBoolean(final boolean x) {
        return new ScilabJavaObject(x, boolean.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapBoolean(final boolean[] x) {
        return new ScilabJavaObject(x, boolean[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapBoolean(final boolean[][] x) {
        return new ScilabJavaObject(x, boolean[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapChar(final char x) {
        return new ScilabJavaObject(x, char.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapChar(final char[] x) {
        return new ScilabJavaObject(x, char[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapChar(final char[][] x) {
        return new ScilabJavaObject(x, char[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapFloat(final float x) {
        return new ScilabJavaObject(x, float.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapFloat(final float[] x) {
        return new ScilabJavaObject(x, float[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapFloat(final float[][] x) {
        return new ScilabJavaObject(x, float[][].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapLong(final long x) {
        return new ScilabJavaObject(x, long.class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapLong(final long[] x) {
        return new ScilabJavaObject(x, long[].class).id;
    }

    /**
     * @param x the variable to wrap into a Java Object
     * @return the corresponding id
     */
    public static int wrapLong(final long[][] x) {
        return new ScilabJavaObject(x, long[][].class).id;
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static double unwrapDouble(final int id) {
        return ((Double) (arraySJO[id].object)).doubleValue();
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static double[] unwrapRowDouble(final int id) {
        return (double[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static double[][] unwrapMatDouble(final int id) {
        return (double[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static int unwrapInt(final int id) {
        return ((Integer) (arraySJO[id].object)).intValue();
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static int[] unwrapRowInt(final int id) {
        return (int[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static int[][] unwrapMatInt(final int id) {
        return (int[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static short unwrapShort(final int id) {
        return ((Short) (arraySJO[id].object)).shortValue();
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static short[] unwrapRowShort(final int id) {
        return (short[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static short[][] unwrapMatShort(final int id) {
        return (short[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static byte unwrapByte(final int id) {
        return ((Byte) (arraySJO[id].object)).byteValue();
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static byte[] unwrapRowByte(final int id) {
        return (byte[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static byte[][] unwrapMatByte(final int id) {
        return (byte[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static String unwrapString(final int id) {
        return (String) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static String[] unwrapRowString(final int id) {
        return (String[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static String[][] unwrapMatString(final int id) {
        return (String[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static boolean unwrapBoolean(final int id) {
        return (Boolean) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static boolean[] unwrapRowBoolean(final int id) {
        return (boolean[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static boolean[][] unwrapMatBoolean(final int id) {
        return (boolean[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static char unwrapChar(final int id) {
        return (Character) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static char[] unwrapRowChar(final int id) {
        return (char[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static char[][] unwrapMatChar(final int id) {
        return (char[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static float unwrapFloat(final int id) {
        return (Float) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static float[] unwrapRowFloat(final int id) {
        return (float[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static float[][] unwrapMatFloat(final int id) {
        return (float[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static long unwrapLong(final int id) {
        return (Long) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static long[] unwrapRowLong(final int id) {
        return (long[]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the resulting unwrapping
     */
    public static long[][] unwrapMatLong(final int id) {
        return (long[][]) (arraySJO[id].object);
    }

    /**
     * @param id the Java Object id
     * @return the identifier of the unwrappable type or -1 if nothing
     */
    public static int isUnwrappable(final int id) {
        Integer t = unwrappableType.get(arraySJO[id].clazz);
        if (t != null) {
            return t;
        } else {
            return -1;
        }
    }

    /**
     * Inner class to manage the free places
     */
    private static final class FreePlace {

        int[] fp;
        int currentPos;

        FreePlace() {
            this.fp = new int[INITIALCAPACITY];
            currentPos = -1;
        }

        final void addFreePlace(final int n) {
            if (currentPos == fp.length) {
                int[] newFp = new int[(int) (1.5 * fp.length)];
                System.arraycopy(fp, 0, newFp, 0, fp.length);
                fp = newFp;
            }
            fp[++currentPos] = n;
        }

        final int getFreePlace() {
            if (currentPos == -1) {
                return -1;
            }
            return fp[currentPos--];
        }
    }
}
