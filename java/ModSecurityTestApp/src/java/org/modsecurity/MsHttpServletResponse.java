package org.modsecurity;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Collection;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;

public class MsHttpServletResponse extends HttpServletResponseWrapper {

    private MsOutputStream msOutputStream;
    private MsWriter msWriter;
    private boolean destroyed = false;
    private boolean suspended;

    public MsHttpServletResponse(HttpServletResponse response) {
        super(response);
    }

    public void destroy() throws IOException {
        if (destroyed == true) {
            return;
        }

        if (msWriter != null) {
            msWriter.commit();
        }
        if (msOutputStream != null) {
            msOutputStream.commit();
        }

        destroyed = true;
    }

    public static String[][] getHttpResponseHeaders(HttpServletResponse resp) {

        Collection<String> headerNames = resp.getHeaderNames();
        String[][] result = new String[headerNames.size()][2];
        try {
            int i = 0;
            for (String headerName : headerNames) {
                result[i][0] = headerName;
                result[i][1] = resp.getHeader(headerName);
                i++;
            }
        } catch (Exception ex) {
        }
        return result;
    }

    public String getBody() {
        if (msWriter != null) {
            return msWriter.toString();
        }
        if (msOutputStream != null) {
            return msOutputStream.toString(getCharacterEncoding());
        }
        return null;
    }

    public void setSuspended(boolean suspended) {
        this.suspended = true;
        if (msWriter != null) {
            msWriter.setSuspended(suspended);
        }
        if (msOutputStream != null) {
            msOutputStream.setSuspended(suspended);
        }
    }

    @Override
    public ServletOutputStream getOutputStream() throws IllegalStateException, IOException {
        if (msWriter != null) {
            throw new IllegalStateException();
        }
        if (msOutputStream == null) {
            msOutputStream = new MsOutputStream(super.getOutputStream());
        }
        return msOutputStream;
    }

    @Override
    public PrintWriter getWriter() throws IllegalStateException, IOException {
            if (msOutputStream != null) {
                throw new IllegalStateException();
            }
            if (msWriter == null) {
                msWriter = new MsWriter(super.getWriter());
            }
            return msWriter;
    }

    public ByteArrayInputStream getByteArrayStream() throws Exception {
        ByteArrayInputStream stream = null;
        if (msOutputStream == null) {
            MsWriter writer = ((MsWriter) this.getWriter());
            stream = new ByteArrayInputStream(new String(writer.toCharArray()).getBytes());
        } else if (msWriter == null) {
            stream = new ByteArrayInputStream(((MsOutputStream) this.getOutputStream()).toByteArray());
        }

        return stream;
    }

    public void setBodyBytes(byte[] bytes) throws IOException {
        if (msOutputStream == null) {
            msWriter.reset();
            msWriter.write(new String(bytes));
        } else if (msWriter == null) {
            msOutputStream.reset();
            msOutputStream.write(bytes, 0, bytes.length);
        }
    }

}
