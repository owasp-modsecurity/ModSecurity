package org.modsecurity;

import java.io.File;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public final class ModSecurity {

    public static final int DONE = -2;
    public static final int DECLINED = -1;
    public static final int OK = 0;
    //From build/classes: >"c:\Program Files\Java\jdk1.7.0_05\bin\javah.exe" -classpath c:\work\apache-tomcat-7.0.39\lib\servlet-api.jar;. org.modsecurity.ModSecurity
    private FilterConfig filterConfig;
    private String confFilename;
    private long confTime;
    private final static String pathToLib = "c:\\work\\mod_security\\java\\Debug\\";

    static {
        //TODO: bad practice, native libraries should be loaded in server's classloader
        System.load("c:\\work\\mod_security\\java\\libs\\zlib1.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\libxml2.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\pcre.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\libapr-1.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\libapriconv-1.dll");
        System.load("c:\\work\\mod_security\\java\\libs\\libaprutil-1.dll");
        System.load("c:\\work\\mod_security\\java\\Debug\\ModSecurityJNI.dll");
        //java.lang.reflect.Field loadedLibraries = ClassLoader.class.getDeclaredField("loadedLibraryNames");
        //loadedLibraries.setAccessible(true);
        //final Vector<String> libraries = (Vector<String>) loadedLibraries.get(ClassLoader.getSystemClassLoader());
    }

    public ModSecurity(FilterConfig fc, String confFile) throws ServletException {
        this.filterConfig = fc;
        this.confFilename = confFile;
        confTime = new File(confFilename).lastModified();

        this.initialize();
        filterConfig.getServletContext().log("ModSecurity started.");
    }

    private native int initialize();

    public native int destroy();

    public native int onRequest(String config, MsHttpTransaction httpTran, boolean reloadConfig);

    public native int onResponse(ServletResponse response, HttpServletResponse htttpResponse, String requestID);

    public static String[][] getHttpRequestHeaders(HttpServletRequest req) {
        ArrayList<String> aList = Collections.list(req.getHeaderNames());
        String[][] result = new String[aList.size()][2];

        for (int i = 0; i < aList.size(); i++) {
            result[i][0] = aList.get(i);
            result[i][1] = req.getHeader(aList.get(i));
        }

        return result;
    }

    public static String[][] getHttpResponseHeaders(HttpServletResponse resp) {

        Collection<String> headerNames = resp.getHeaderNames();
        String[][] result = new String[headerNames.size()][2];

        int i = 0;
        for (String headerName : headerNames) {
            result[i][0] = headerName;
            result[i][1] = resp.getHeader(headerName);
            i++;
        }

        return result;
    }

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
