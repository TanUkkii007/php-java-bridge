package com.github.tanukkii007.php.core;


import java.lang.reflect.Field;
import java.util.Vector;

public class PHP {
    static {
        if (!isLibraryLoaded()) {
            System.loadLibrary("bridge");
        }
    }

    private static native int evalInternal(String code, String... argv);

    static String scriptName = "PHP#eval";

    public static int eval(String code, String... argv) {
        return evaluate(code, scriptName, argv);
    }

    private static int evaluate(String code, String scriptName, String... argv) {
        String[] argvWithScript = new String[1 + argv.length];
        argvWithScript[0] = scriptName;
        System.arraycopy(argv, 0, argvWithScript, 1, argv.length);
        return evalInternal(code, scriptName);
    }


    private static boolean isLibraryLoaded() {
        try {
            final String libraryName = "libbridge.jnilib";
            final Field loadLibraryNames = ClassLoader.class.getDeclaredField("loadedLibraryNames");
            loadLibraryNames.setAccessible(true);
            final Vector<String> libraries = (Vector<String>) loadLibraryNames.get(PHP.class.getClass().getClassLoader());

            for (String library : libraries) {
                if (library.contains(libraryName)) return true;
            }
            return false;
        } catch (final Throwable e) {
            e.printStackTrace();
            return false;
        }
    }
}
