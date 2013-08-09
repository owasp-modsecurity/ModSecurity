package org.modsecurity.loader;

import java.io.File;

public class ModSecurityLoader {

    private static final String MODSECURITYLIBSDIR_PATH = "c:\\work\\mod_security\\java\\libs\\"; //directory with ModSecurity native libraries

    static {
        File modSecDir = new File(MODSECURITYLIBSDIR_PATH);

        File[] flibs = modSecDir.listFiles();
        System.out.println("len" + flibs.length);

        loadLib(flibs, "zlib1");
        loadLib(flibs, "libxml2");
        loadLib(flibs, "pcre");
        loadLib(flibs, "libapr-1");
        loadLib(flibs, "libapriconv-1");
        loadLib(flibs, "libaprutil-1");
        loadLib(flibs, "ModSecurityJNI");
    }

    private static void loadLib(File[] files, String lib) {
        for (int i = 0; i < files.length; i++) {
            if (files[i].getName().startsWith(lib)) {
                System.load(files[i].getAbsolutePath());
                break;
            }
        }
    }
}
