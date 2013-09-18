package org.modsecurity.loader;

import java.util.HashMap;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class ModSecurityLoader {

    public static final String CONFIG_FILENAME = "ModSecurityLoaderConfig.xml";
    private static final String zlibParamName = "zlib1";
    private static final String libxml2ParamName = "libxml2";
    private static final String libpcreParamName = "libpcre";
    private static final String libaprParamName = "libapr-1";
    private static final String libapriconvParamName = "libapriconv-1";
    private static final String libaprutilParamName = "libaprutil-1";
    private static final String libModSecurityJNIParamName = "libModSecurityJNI";

    static {
        //System.out.println("ModSecurity loader static block executed.");

        //try load native libraries from config file with native libs paths, otherwise get paths from the
        // server VM start-up option: -Djava.library.path=path/to/libs/
        HashMap<String, String> libPaths = new HashMap<String, String>();
        try {
            Document dom;
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            dom = db.parse(CONFIG_FILENAME);

            Element doc = dom.getDocumentElement();

            NodeList nl = doc.getElementsByTagName("init-param");

            for (int i = 0; i < nl.getLength(); i++) {
                String key = null;
                String value = null;
                
                Node initParam = nl.item(i);

                NodeList keyNl = ((Element) initParam).getElementsByTagName("param-name");
                if (keyNl.getLength() > 0 && keyNl.item(0).hasChildNodes()) {
                    key = keyNl.item(0).getFirstChild().getNodeValue();
                }
                
                NodeList valNl = ((Element) initParam).getElementsByTagName("param-value");
                if (valNl.getLength() > 0 && valNl.item(0).hasChildNodes()) {
                    value = valNl.item(0).getFirstChild().getNodeValue();
                }

                if (key != null && value != null) {
                    libPaths.put(key, value);
                }
            }
        } catch (Exception ex) {
        }

        if (!libPaths.containsKey(zlibParamName)) {
            libPaths.put(zlibParamName, null);
        }
        if (!libPaths.containsKey(libxml2ParamName)) {
            libPaths.put(libxml2ParamName, null);
        }
        if (!libPaths.containsKey(libpcreParamName)) {
            libPaths.put(libpcreParamName, null);
        }
        if (!libPaths.containsKey(libaprParamName)) {
            libPaths.put(libaprParamName, null);
        }
        if (!libPaths.containsKey(libapriconvParamName)) {
            libPaths.put(libapriconvParamName, null);
        }
        if (!libPaths.containsKey(libaprutilParamName)) {
            libPaths.put(libaprutilParamName, null);
        }
        if (!libPaths.containsKey(libModSecurityJNIParamName)) {
            libPaths.put(libModSecurityJNIParamName, null);
        }

        try {
            loadLib("zlib1", libPaths.get(zlibParamName));
        } catch (UnsatisfiedLinkError err) {
        }
        loadLib("xml2", libPaths.get(libxml2ParamName));
        loadLib("pcre", libPaths.get(libpcreParamName));
        loadLib("apr-1", libPaths.get(libaprParamName));
        try {
            loadLib("apriconv-1", libPaths.get(libapriconvParamName));
        } catch (UnsatisfiedLinkError err) {
        }
        loadLib("aprutil-1", libPaths.get(libaprutilParamName));
        loadLib("ModSecurityJNI", libPaths.get(libModSecurityJNIParamName));
    }

    private static void loadLib(String name, String absolutePath) throws UnsatisfiedLinkError {
        try {
            System.load(absolutePath);
            return;
        } catch (NullPointerException ex) {
        } catch (UnsatisfiedLinkError ex) {
            throw ex;
        }
        try {
            System.loadLibrary(name);
            return;
        } catch (UnsatisfiedLinkError ex) {
        }
        try {
            System.loadLibrary("lib" + name);
        } catch (UnsatisfiedLinkError ex) {
            throw ex;
        }
    }

    public static void main(String[] args) {
    }
}
