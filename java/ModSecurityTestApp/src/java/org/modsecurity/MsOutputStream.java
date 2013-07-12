package org.modsecurity;

import java.io.IOException;
import java.io.ByteArrayOutputStream;
import java.io.UnsupportedEncodingException;
import javax.servlet.ServletOutputStream;

public class MsOutputStream extends ServletOutputStream {

    private boolean buffering = false;

    private ServletOutputStream sos;

    private ByteArrayOutputStream buffer;

    public MsOutputStream(ServletOutputStream sos) {
        super();
        this.sos = sos;
        buffer = new ByteArrayOutputStream();
    }

    public void setBuffering(boolean buffering) {
        this.buffering = buffering;
    }

    public String toString(String encoding) {
        String s = null;
        try {
            s = buffer.toString(encoding);
        } catch(UnsupportedEncodingException e) {
            // TODO error
            e.printStackTrace(System.err);
        }
        return s;
    }

    public byte[] toByteArray() {
        return buffer.toByteArray();
    }

    public void reset() {
        buffer.reset();
    }

    public void commit() throws IOException {
        if (!buffering) return;
        buffer.writeTo(sos);
        sos.close();
    }

    public void setSuspended(boolean suspended) {
        // TODO
    }

    @Override
    public void write(int i) throws IOException {
        if (!buffering) sos.write(i);
        print(Integer.toString(i));
    }

    @Override
    public void write(byte[] b) throws IOException {
        if (!buffering) sos.write(b, 0, b.length);
        buffer.write(b, 0, b.length);
    }

    @Override
    public void write(byte[] b, int off, int len) throws IOException {
        if (!buffering) sos.write(b, off, len);
        buffer.write(b, off, len);
    }

    @Override
    public void flush() throws IOException {
        if (!buffering) sos.flush();
        // we can't flush our buffer
    }

    @Override
    public void close() throws IOException {
        if (!buffering) sos.close();
    }

    @Override
    public void print(String s) throws IOException {
         if (!buffering) sos.print(s);
         byte[] bytes = s.getBytes();
         buffer.write(bytes, 0, bytes.length);
    }
}