/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.commons.io.output;

import java.io.FilterOutputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * A Proxy stream which acts as expected, that is it passes the method 
 * calls on to the proxied stream and doesn't change which methods are 
 * being called. It is an alternative base class to FilterOutputStream
 * to increase reusability.
 * <p>
 * See the protected methods for ways in which a subclass can easily decorate
 * a stream with custom pre-, post- or error processing functionality.
 * 
 * @author Stephen Colebourne
 * @version $Id: ProxyOutputStream.java 1003340 2010-10-01 00:31:53Z sebb $
 */
public class ProxyOutputStream extends FilterOutputStream {

    /**
     * Constructs a new ProxyOutputStream.
     * 
     * @param proxy  the OutputStream to delegate to
     */
    public ProxyOutputStream(OutputStream proxy) {
        super(proxy);
        // the proxy is stored in a protected superclass variable named 'out'
    }

    /**
     * Invokes the delegate's <code>write(int)</code> method.
     * @param idx the byte to write
     * @throws IOException if an I/O error occurs
     */
    @Override
    public void write(int idx) throws IOException {
        try {
            beforeWrite(1);
            out.write(idx);
            afterWrite(1);
        } catch (IOException e) {
            handleIOException(e);
        }
    }

    /**
     * Invokes the delegate's <code>write(byte[])</code> method.
     * @param bts the bytes to write
     * @throws IOException if an I/O error occurs
     */
    @Override
    public void write(byte[] bts) throws IOException {
        try {
            int len = bts != null ? bts.length : 0;
            beforeWrite(len);
            out.write(bts);
            afterWrite(len);
        } catch (IOException e) {
            handleIOException(e);
        }
    }

    /**
     * Invokes the delegate's <code>write(byte[])</code> method.
     * @param bts the bytes to write
     * @param st The start offset
     * @param end The number of bytes to write
     * @throws IOException if an I/O error occurs
     */
    @Override
    public void write(byte[] bts, int st, int end) throws IOException {
        try {
            beforeWrite(end);
            out.write(bts, st, end);
            afterWrite(end);
        } catch (IOException e) {
            handleIOException(e);
        }
    }

    /**
     * Invokes the delegate's <code>flush()</code> method.
     * @throws IOException if an I/O error occurs
     */
    @Override
    public void flush() throws IOException {
        try {
            out.flush();
        } catch (IOException e) {
            handleIOException(e);
        }
    }

    /**
     * Invokes the delegate's <code>close()</code> method.
     * @throws IOException if an I/O error occurs
     */
    @Override
    public void close() throws IOException {
        try {
            out.close();
        } catch (IOException e) {
            handleIOException(e);
        }
    }

    /**
     * Invoked by the write methods before the call is proxied. The number
     * of bytes to be written (1 for the {@link #write(int)} method, buffer
     * length for {@link #write(byte[])}, etc.) is given as an argument.
     * <p>
     * Subclasses can override this method to add common pre-processing
     * functionality without having to override all the write methods.
     * The default implementation does nothing.
     *
     * @since Commons IO 2.0
     * @param n number of bytes to be written
     * @throws IOException if the pre-processing fails
     */
    protected void beforeWrite(int n) throws IOException {
    }

    /**
     * Invoked by the write methods after the proxied call has returned
     * successfully. The number of bytes written (1 for the
     * {@link #write(int)} method, buffer length for {@link #write(byte[])},
     * etc.) is given as an argument.
     * <p>
     * Subclasses can override this method to add common post-processing
     * functionality without having to override all the write methods.
     * The default implementation does nothing.
     *
     * @since Commons IO 2.0
     * @param n number of bytes written
     * @throws IOException if the post-processing fails
     */
    protected void afterWrite(int n) throws IOException {
    }

    /**
     * Handle any IOExceptions thrown.
     * <p>
     * This method provides a point to implement custom exception
     * handling. The default behaviour is to re-throw the exception.
     * @param e The IOException thrown
     * @throws IOException if an I/O error occurs
     * @since Commons IO 2.0
     */
    protected void handleIOException(IOException e) throws IOException {
        throw e;
    }

}
