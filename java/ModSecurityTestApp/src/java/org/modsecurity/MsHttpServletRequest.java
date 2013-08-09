package org.modsecurity;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.servlet.ServletException;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
import org.apache.commons.fileupload.DefaultFileItem;
import org.apache.commons.fileupload.DiskFileUpload;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUploadException;

public class MsHttpServletRequest extends HttpServletRequestWrapper {

    public final static int BODY_NOTYETREAD = 0;
    public final static int BODY_INTERCEPT = 1;
    public final static int BODY_DISK = 2;
    public final static int BODY_MEMORY = 3;
    public final static int BODY_CONSUMED = 4;
    private HttpServletRequest req;
    private ArrayList parameters = new ArrayList();
    private byte bodyBytes[];
    private int bodyStatus = BODY_NOTYETREAD;
    private String body;
    private File bodyFile;
    private String encoding;
    private List fileItems;
    private String tmpPath;
    //private int maxContentLength = 1024 * 1024 * 1024; // 1 GB
    private boolean preserveRequestBody = true;

    private class Parameter {

        String name;
        String value;

        Parameter(String name, String value) {
            this.name = name;
            this.value = value;
        }
    }

    public MsHttpServletRequest(HttpServletRequest req) {
        super(req);
        this.req = req;

        encoding = req.getCharacterEncoding();
        if (encoding == null) {
            encoding = "UTF-8";
        }
        addUrlEncoded(req.getQueryString());
        setTmpPath(System.getProperty("java.io.tmpdir"));
    }

    public void destroy() {
        if (bodyFile != null) {
            bodyFile.delete();
        }
    }

    public static String[][] getHttpRequestHeaders(HttpServletRequest req) {

        ArrayList<String> aList = Collections.list(req.getHeaderNames());
        String[][] result = new String[aList.size()][2];

        try {
            for (int i = 0; i < aList.size(); i++) {
                result[i][0] = aList.get(i);
                result[i][1] = req.getHeader(aList.get(i));
            }
        } catch (Exception ex) {
        }

        return result;
    }

    public String getTmpPath() {
        return tmpPath;
    }

    public final void setTmpPath(String tmpPath) {
        this.tmpPath = tmpPath;
    }

    public void setPreserveRequestBody(boolean preserveRequestBody) {
        this.preserveRequestBody = preserveRequestBody;
    }

    public boolean getPreserveRequestBody() {
        return preserveRequestBody;
    }

    public int getBodyStatus() {
        return bodyStatus;
    }

    public File getBodyFile() {
        return bodyFile;
    }

    public String getBody() {
        return body;
    }

    public byte[] getBodyBytes() {
        return bodyBytes;
    }

    public void setBodyBytes(byte[] bytes) throws IOException {
        String contentType = req.getContentType();
        bodyBytes = new byte[bytes.length];
        System.arraycopy(bytes, 0, bodyBytes, 0, bytes.length);
        
        body = new String(bodyBytes, encoding);
        if ((contentType != null) && ((contentType.compareTo("application/x-www-form-urlencoded") == 0) || (contentType.compareTo("application/x-form-urlencoded") == 0))) {
            addUrlEncoded(body);
        }
    }
    
    @Override
    public int getContentLength() {
        if (bodyBytes == null)
            return req.getContentLength();
        return bodyBytes.length;
    }

    public void readBody(int maxContentLength) throws IOException, ServletException {

        String contentType = req.getContentType();

        if ((contentType != null) && (contentType.startsWith("multipart/form-data"))) {
            readBodyMultipart(maxContentLength);
        } else {
            int contentLength = req.getContentLength();
            if (contentLength != -1) {
                // known body length, we can allocate a byte
                // array of required length

                // restriction
                if (contentLength > maxContentLength) {
                    throw new ServletException("Request body too large: " + contentLength + " bytes (" + maxContentLength + " allowed)");
                }

                int count = 0;
                bodyBytes = new byte[contentLength];
                ServletInputStream sis = req.getInputStream();
                while (count < contentLength) {
                    int bytesRead = sis.read(bodyBytes, count, contentLength - count);
                    if (bytesRead == -1) {
                        return;
                    }
                    count += bytesRead;
                }

                body = new String(bodyBytes, encoding);
                if ((contentType != null) && ((contentType.compareTo("application/x-www-form-urlencoded") == 0) || (contentType.compareTo("application/x-form-urlencoded") == 0))) {
                    addUrlEncoded(body);
                }
            } else {
                // unknown body length, so we send bytes to a
                // ByteArrayOutputStream instance
                byte b[] = new byte[1024];
                ServletInputStream sis = req.getInputStream();
                ByteArrayOutputStream baos = new ByteArrayOutputStream(32768);

                int count = 0;
                for (;;) {
                    int bytesRead = sis.read(b);
                    if (bytesRead == -1) {
                        break;
                    }

                    // size restriction
                    if (count > maxContentLength) {
                        throw new ServletException("Request body too large: " + count + " bytes (" + maxContentLength + " allowed)");
                    }

                    baos.write(b, 0, bytesRead);
                    count += bytesRead;
                }
                bodyBytes = baos.toByteArray();
            }

            bodyStatus = BODY_MEMORY;
        }
    }

    private void readBodyMultipart(int maxContentLength) throws IOException, ServletException {
        DiskFileUpload fu = new DiskFileUpload();
        fu.setSizeMax(maxContentLength); // max body size
        fu.setSizeThreshold(655350); // max in-memory storage for fields (not files)
        fu.setRepositoryPath(tmpPath); // tmp path on disk

        try {
            // setting bodyStatus to BODY_INTERCEPT
            // will cause the ServletRequest wrapper to
            // intercept the body and make a copy of it
            // on disk
            if (preserveRequestBody) {
                bodyStatus = BODY_INTERCEPT;
            }
            fileItems = fu.parseRequest(this);
        } catch (FileUploadException e) {
            // e.printStackTrace(System.out);
            throw new ServletException(e);
        }

        // loop through the available items and
        // add parameters to the request parameters list
        Iterator iterator = fileItems.iterator();
        while (iterator.hasNext()) {
            FileItem item = (FileItem) iterator.next();
            if (item.isFormField()) {
                addParameter(item.getFieldName(), item.getString(encoding));
            }
        }
    }

    /**
     * Parses the given URL-encoded string and adds the parameters to the
     * request parameter list.
     */
    private void addUrlEncoded(String text) {
        if (text == null) {
            return;
        }

        int flag = 0;
        int startPos = -1;

        String name = null, value;

        try {

            for (int i = 0; i < text.length(); i++) {
                char c = text.charAt(i);
                if (startPos == -1) {
                    startPos = i;
                }

                if (flag == 0) {
                    if (c == '=') {
                        name = URLDecoder.decode(text.substring(startPos, i), encoding);
                        flag = 1;
                        startPos = -1;
                    }
                } else {
                    if (c == '&') {
                        value = URLDecoder.decode(text.substring(startPos, i), encoding);
                        addParameter(name, value);
                        flag = 0;
                        startPos = -1;
                    }
                }
            }

            if (flag == 1) {
                value = "";
                if (startPos != -1) {
                    value = URLDecoder.decode(text.substring(startPos), encoding);
                }
                addParameter(name, value);
            } else {
                if (startPos != -1) {
                    name = URLDecoder.decode(text.substring(startPos), encoding);
                    addParameter(name, "");
                }
            }

        } catch (UnsupportedEncodingException e) {
            // TODO it should not happen to us, but
            // log somewhere just in case it does
            e.printStackTrace(System.err);
        }
    }

    /**
     * Adds the given parameter to the request parameters list.
     */
    private void addParameter(String name, String value) {
        parameters.add(new Parameter(name, value));
    }


    /* -- Methods for access to uploaded files ------------------------- */
    /**
     * Returns a File instance representing the uploaded file.
     */
    public File getFile(String name) {
        if (fileItems == null) {
            return null;
        }

        Iterator iterator = fileItems.iterator();
        while (iterator.hasNext()) {
            DefaultFileItem item = (DefaultFileItem) iterator.next();
            if (item.getFieldName().compareTo(name) == 0) {
                return item.getStoreLocation();
            }
        }

        return null;
    }

    /**
     * Returns the (field) names of all uploaded files as an Enumeration.
     */
    public Enumeration getFileNames() {
        Hashtable names = new Hashtable();
        if (fileItems == null) {
            return names.keys();
        }

        Iterator iterator = fileItems.iterator();
        while (iterator.hasNext()) {
            FileItem item = (FileItem) iterator.next();
            if (!item.isFormField()) {
                names.put(item.getFieldName(), item);
            }
        }

        return names.keys();
    }

    /**
     * Returns the original filename of the uploaded file.
     */
    public String getFileSystemName(String name) {
        Iterator iterator = fileItems.iterator();
        while (iterator.hasNext()) {
            DefaultFileItem item = (DefaultFileItem) iterator.next();
            if (item.getFieldName().compareTo(name) == 0) {
                return item.getName();
            }
        }

        return null;
    }


    /* -- ServletRequest wrapper --------------------------------------- */
    /**
     * This class is an ServletInputStream wrapper, which passes the input from
     * the stream to the caller and writes it to a temporary file at the same
     * time.
     */
    private class InterceptServletInputStream extends ServletInputStream {

        private ServletInputStream sis;
        private int count;
        private OutputStream os;

        private InterceptServletInputStream(ServletInputStream sis) throws IOException {
            this.sis = sis;
            bodyFile = File.createTempFile("modsec", ".tmp", new File(tmpPath));
            os = new BufferedOutputStream(new FileOutputStream(bodyFile));
        }

        @Override
        public int read() throws IOException {
            int i = sis.read();

            if (i != -1) {
                count++;
                os.write(i);
            } else {
                os.close();
                bodyStatus = BODY_DISK;
            }

            return i;
        }
    }

    /**
     * This ServletInputStream implementation reads the body from the memory.
     */
    private class BodyMemoryServletInputStream extends ServletInputStream {

        private ByteArrayInputStream bais;

        private BodyMemoryServletInputStream() {
            bais = new ByteArrayInputStream(bodyBytes);
        }

        @Override
        public int read() {
            return bais.read();
        }
    }

    /**
     * This ServletInputStream implementation reads the body from the temporary
     * file on disk.
     */
    private class BodyDiskServletInputStream extends ServletInputStream {

        private InputStream is;

        private BodyDiskServletInputStream() throws FileNotFoundException {
            is = new BufferedInputStream(new FileInputStream(bodyFile));
        }

        @Override
        public int read() throws IOException {
            return is.read();
        }
    }

    /**
     * Replacement for the ServletRequest.getInputStream() method.
     */
    @Override
    public ServletInputStream getInputStream() throws java.io.IOException {
        ServletInputStream sis;

        switch (bodyStatus) {

            case BODY_INTERCEPT:
                sis = new InterceptServletInputStream(req.getInputStream());
                break;

            case BODY_MEMORY:
                sis = new BodyMemoryServletInputStream();
                break;

            case BODY_DISK:
                sis = new BodyDiskServletInputStream();
                break;

            default:
                sis = req.getInputStream();
                break;
        }

        return sis;
    }

    /**
     * Replacement for the ServletRequest.getParameter() method.
     */
    @Override
    public String getParameter(String name) {
        for (int i = 0, n = parameters.size(); i < n; i++) {
            Parameter p = (Parameter) parameters.get(i);
            if (p.name.compareTo(name) == 0) {
                return p.value;
            }
        }
        return null;
    }

    /**
     * Replacement for the ServletRequest.getParameterMap() method.
     */
    @Override
    public Map getParameterMap() {
        HashMap map = new HashMap();
        for (int i = 0, n = parameters.size(); i < n; i++) {
            Parameter p = (Parameter) parameters.get(i);
            map.put(p.name, p.value);
        }
        return map;
    }

    /**
     * Replacement for the ServletRequest.getParameterNames() method.
     */
    @Override
    public Enumeration getParameterNames() {
        Hashtable parameterNames = new Hashtable();
        for (int i = 0, n = parameters.size(); i < n; i++) {
            Parameter p = (Parameter) parameters.get(i);
            parameterNames.put(p.name, p.value);
        }
        return parameterNames.keys();
    }

    /**
     * Replacement for the ServletRequest.getParameterValues(String) method.
     */
    @Override
    public String[] getParameterValues(String name) {
        // how many parameters are there with the
        // given name?
        int count = 0;
        for (int i = 0, n = parameters.size(); i < n; i++) {
            Parameter p = (Parameter) parameters.get(i);
            if (p.name.compareTo(name) == 0) {
                count++;
            }
        }

        // put them into a String array
        String values[] = new String[count];
        count = 0;
        for (int i = 0, n = parameters.size(); i < n; i++) {
            Parameter p = (Parameter) parameters.get(i);
            if (p.name.compareTo(name) == 0) {
                values[count++] = p.value;
            }
        }

        return values;
    }

    /**
     * Replacement for the ServletRequest.getReader() method.
     */
    @Override
    public BufferedReader getReader() throws java.io.IOException {
        return new BufferedReader(new InputStreamReader(getInputStream(), encoding));
    }
}