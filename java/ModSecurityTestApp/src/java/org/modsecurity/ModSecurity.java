package org.modsecurity;

import java.io.File;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.UnknownHostException;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;

/**
 *
 * @author Mihai Pitu
 */
public final class ModSecurity {
    //From build/classes: >"c:\Program Files\Java\jdk1.7.0_05\bin\javah.exe" -classpath c:\work\apache-tomcat-7.0.39\lib\servlet-api.jar;. org.modsecurity.ModSecurity

    public static final int DONE = -2;
    public static final int DECLINED = -1;
    public static final int OK = 0;
    private FilterConfig filterConfig;
    private String confFilename;
    private long confTime;

    static {
//        try {
//            Class.forName("org.modsecurity.loader.ModSecurityLoader");
//            System.out.println("MS loader found");
//        } catch (ClassNotFoundException ex) {
//            Logger.getLogger(ModSecurity.class.getName()).log(Level.SEVERE, null, ex);
//        }

        //TODO: bad practice (if we have two webapps using ModSecurity, one will raise UnsatisfiedLinkError), 
        //native libraries should be loaded in server's root classloader
        System.load("c:\\work\\mod_security\\java\\libs\\zlib1.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\libxml2.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\pcre.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\libapr-1.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\libapriconv-1.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\libaprutil-1.dll");
        System.load("c:\\work\\mod_security\\java\\Debug\\ModSecurityJNI.dll");
    }

    public ModSecurity(FilterConfig fc, String confFile) throws ServletException {
        this.filterConfig = fc;
        this.confFilename = confFile;
        confTime = new File(confFilename).lastModified();

        this.initialize(fc.getFilterName());
        filterConfig.getServletContext().log("ModSecurity started.");
    }

    private native int initialize(String serverName);

    public native int destroy();

    public native int onRequest(String config, MsHttpTransaction httpTran, boolean reloadConfig);

    public native int onResponse(MsHttpTransaction httpTran);

    public static boolean isIPv6(String addr) {
        try {
            InetAddress inetAddress = InetAddress.getByName(addr);

            if (inetAddress instanceof Inet6Address) {
                return true;
            } else {
                return false;
            }
        } catch (UnknownHostException ex) {
            return false;
        }
    }

    public void log(int level, String msg) {
        //if (level == 1) {
        filterConfig.getServletContext().log(msg);
        //}
    }

    public boolean checkModifiedConfig() {
        if (new File(confFilename).lastModified() > confTime) {
            return true;
        } else {
            return false;
        }
    }

    public String getAppPath() {
        return filterConfig.getServletContext().getContextPath();
    }

    public String getConfFilename() {
        return this.confFilename;
    }
}
