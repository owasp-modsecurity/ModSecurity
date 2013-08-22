package org.modsecurity.loader;

import java.io.File;

public class ModSecurityLoader {

    //private static final String MODSECURITYLIBSDIR_PATH = "c:\\work\\mod_security\\java\\libs\\"; //directory with ModSecurity native libraries

    static {
        System.out.println("ModSecurity loader static block executed.");
//        File modSecDir = new File(MODSECURITYLIBSDIR_PATH);
//        File[] flibs = modSecDir.listFiles();
//        loadLib(flibs, "zlib1");
//        loadLib(flibs, "libxml2");
//        loadLib(flibs, "pcre");
//        loadLib(flibs, "libapr-1");
//        loadLib(flibs, "libapriconv-1");
//        loadLib(flibs, "libaprutil-1");
//        loadLib(flibs, "ModSecurityJNI");

        //alternative load, this requires native libraries to be in java.library.path, you can set it
        //by specifying server VM start-up option: -Djava.library.path=path/to/libs/
        try {
            System.loadLibrary("zlib1"); //needed for libxml2 in Windows
        } catch(UnsatisfiedLinkError ex) {
        }
        System.loadLibrary("libxml2");
        System.loadLibrary("pcre");
        System.loadLibrary("libapr-1");
        try {
        System.loadLibrary("libapriconv-1");
        } catch(UnsatisfiedLinkError ex) { //needed for libaprutil-1 in Windows
        }
        System.loadLibrary("libaprutil-1");
        System.loadLibrary("ModSecurityJNI");
        
        System.out.println("ModSecurity native libraries loaded.");
    }

    private static void loadLib(File[] files, String lib) {
        for (int i = 0; i < files.length; i++) {
            if (files[i].getName().startsWith(lib)) {
                System.load(files[i].getAbsolutePath());
                break;
            }
        }
    }
    
    public void main(String[] args) {
        
    }
}
