package com.github.tanukkii007.php.core;


import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Vector;

public class PHP {
    static {
        if (!isLibraryLoaded()) {
           try {
               load("bridge");
           } catch (Exception ex) {
               throw new RuntimeException(ex);
           }
        }
    }

    private static native int evalInternal(String code, String... argv);

    private static native boolean evalInternalBoolean(String code, String... argv);

    private static native long evalInternalLong(String code, String... argv);

    private static native double evalInternalDouble(String code, String... argv);

//    private static native String evalInternalString(String code, String... argv);

    static String scriptName = "PHP#eval";

    public static int eval(String code, String... argv) {
        return evaluate(code, scriptName, argv);
    }

    public static boolean evaluateBoolean(String code, String scriptName, String... argv) {
        String[] argvWithScript = new String[1 + argv.length];
        argvWithScript[0] = "PHP#evaluateBoolean";
        System.arraycopy(argv, 0, argvWithScript, 1, argv.length);
        return evalInternalBoolean(code, argvWithScript);
    }

    public static long evaluateLong(String code, String scriptName, String... argv) {
        String[] argvWithScript = new String[1 + argv.length];
        argvWithScript[0] = "PHP#evaluateLong";
        System.arraycopy(argv, 0, argvWithScript, 1, argv.length);
        return evalInternalLong(code, argvWithScript);
    }

    public static double evaluateDouble(String code, String scriptName, String... argv) {
        String[] argvWithScript = new String[1 + argv.length];
        argvWithScript[0] = "PHP#evaluateDouble";
        System.arraycopy(argv, 0, argvWithScript, 1, argv.length);
        return evalInternalDouble(code, argvWithScript);
    }

    private static int evaluate(String code, String scriptName, String... argv) {
        String[] argvWithScript = new String[1 + argv.length];
        argvWithScript[0] = scriptName;
        System.arraycopy(argv, 0, argvWithScript, 1, argv.length);
        return evalInternal(code, argvWithScript);
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

    // helpful implementation
    // https://github.com/jodersky/sbt-jni/blob/master/macros/src/main/scala/ch/jodersky/jni/annotations.scala
    private static void loadPackaged(String libName) throws IOException {
        String lib = System.mapLibraryName(libName);
        Path tmp = Files.createTempDirectory("jni-");
        String platform = "darwin";
        String resourcePath = "/native/" + platform + "/" + lib;
        InputStream resourceStream = PHP.class.getResourceAsStream(resourcePath);
        if (resourceStream == null) {
            throw new UnsatisfiedLinkError(
                    "Native library " + lib + " (" + resourcePath + ") cannot be found on the classpath.");
        }
        Path extractedPath = tmp.resolve(lib);
        try {
            Files.copy(resourceStream, extractedPath);
        } catch (Exception ex) {
            throw new UnsatisfiedLinkError("Error while extracting native library: " + ex);
        }
        System.load(extractedPath.toAbsolutePath().toString());
    }

    private static void load(String libName) throws IOException {
        try {
            System.loadLibrary(libName);
        } catch (UnsatisfiedLinkError ex) {
            loadPackaged(libName);
        }
    }
}
