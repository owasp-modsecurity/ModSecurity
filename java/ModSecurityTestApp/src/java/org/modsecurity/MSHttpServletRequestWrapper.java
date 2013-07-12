package org.modsecurity;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;

public class MSHttpServletRequestWrapper extends HttpServletRequestWrapper {

    private HttpServletRequest request;
    private MSServletInputStream servletStream;
    private Map<String, String[]> parameterMap = null;

    public MSHttpServletRequestWrapper(HttpServletRequest request) {
        super(request);
        this.request = request;
    }

    private void resetInputStream() throws IOException {
        if (this.servletStream == null) {
            this.servletStream = new MSServletInputStream();
            this.servletStream.stream = request.getInputStream();
            this.servletStream.outStream = new ByteArrayOutputStream();
            // Read the parameters first, because they can get reachless after the inputStream is read.  
            getParameterMap();
        } else {
            while (servletStream.read() != -1) { //finish the stream
            }
            this.servletStream.stream.close();
            servletStream.close();
            this.servletStream.outStream.flush();
            this.servletStream.stream = new ByteArrayInputStream(this.servletStream.outStream.toByteArray());
            this.servletStream.outStream.close();
            this.servletStream.outStream = new ByteArrayOutputStream();
        }
        //System.out.println("getinputstream");
    }

    @Override
    public ServletInputStream getInputStream() throws IOException {
        resetInputStream();

        return servletStream;
    }

    @Override
    public BufferedReader getReader() throws IOException {
        resetInputStream();

        return new BufferedReader(new InputStreamReader(servletStream));
    }

    @Override
    public String getParameter(String name) {
        String[] a = parameterMap.get(name);
        if (a == null || a.length == 0) {
            return null;
        }
        return a[0];
    }

    @Override
    public Map getParameterMap() {
        if (parameterMap == null) {
            parameterMap = new HashMap<String, String[]>();
            parameterMap.putAll(super.getParameterMap());
        }
        return parameterMap;
    }

    @Override
    public Enumeration getParameterNames() {
        return Collections.enumeration(parameterMap.values());
    }

    @Override
    public String[] getParameterValues(String name) {
        return parameterMap.get(name);
    }

    private class MSServletInputStream extends ServletInputStream {

        private InputStream stream;
        private ByteArrayOutputStream outStream;

        @Override
        public int read() throws IOException {
            int out = stream.read();
            if (out != -1) {
                outStream.write(out); //writing in an out stream as the in stream is read, rather than storing it all in a buffer, to prevent DOS attack
            }
            System.out.println("read=" + out);
            return out;
        }

        @Override
        public int read(byte[] b, int off, int len) throws IOException {
            int out = stream.read(b, off, len);
            if (out != -1) {
                outStream.write(b, off, len);
            }
            return out;
        }
    }
}
