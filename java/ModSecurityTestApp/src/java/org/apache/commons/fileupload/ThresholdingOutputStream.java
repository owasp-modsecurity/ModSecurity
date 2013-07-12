/*
 * $Header: /home/cvs/jakarta-commons/fileupload/src/java/org/apache/commons/fileupload/ThresholdingOutputStream.java,v 1.3 2003/05/31 22:31:08 martinc Exp $
 * $Revision: 1.3 $
 * $Date: 2003/05/31 22:31:08 $
 *
 * ====================================================================
 *
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2003 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution, if
 *    any, must include the following acknowlegement:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowlegement may appear in the software itself,
 *    if and wherever such third-party acknowlegements normally appear.
 *
 * 4. The names "The Jakarta Project", "Commons", and "Apache Software
 *    Foundation" must not be used to endorse or promote products derived
 *    from this software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache"
 *    nor may "Apache" appear in their names without prior written
 *    permission of the Apache Group.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 */


package org.apache.commons.fileupload;

import java.io.IOException;
import java.io.OutputStream;


/**
 * An output stream which triggers an event when a specified number of bytes of
 * data have been written to it. The event can be used, for example, to throw
 * an exception if a maximum has been reached, or to switch the underlying
 * stream type when the threshold is exceeded.
 * <p>
 * This class overrides all <code>OutputStream</code> methods. However, these
 * overrides ultimately call the corresponding methods in the underlying output
 * stream implementation.
 * <p>
 * NOTE: This implementation may trigger the event <em>before</em> the threshold
 * is actually reached, since it triggers when a pending write operation would
 * cause the threshold to be exceeded.
 *
 * @author <a href="mailto:martinc@apache.org">Martin Cooper</a>
 *
 * @version $Id: ThresholdingOutputStream.java,v 1.3 2003/05/31 22:31:08 martinc Exp $
 */
public abstract class ThresholdingOutputStream
    extends OutputStream
{

    // ----------------------------------------------------------- Data members


    /**
     * The threshold at which the event will be triggered.
     */
    private int threshold;


    /**
     * The number of bytes written to the output stream.
     */
    private long written;


    /**
     * Whether or not the configured threshold has been exceeded.
     */
    private boolean thresholdExceeded;


    // ----------------------------------------------------------- Constructors


    /**
     * Constructs an instance of this class which will trigger an event at the
     * specified threshold.
     *
     * @param threshold The number of bytes at which to trigger an event.
     */
    public ThresholdingOutputStream(int threshold)
    {
        this.threshold = threshold;
    }


    // --------------------------------------------------- OutputStream methods


    /**
     * Writes the specified byte to this output stream.
     *
     * @param b The byte to be written.
     *
     * @exception IOException if an error occurs.
     */
    public void write(int b) throws IOException
    {
        checkThreshold(1);
        getStream().write(b);
        written++;
    }


    /**
     * Writes <code>b.length</code> bytes from the specified byte array to this
     * output stream.
     *
     * @param b The array of bytes to be written.
     *
     * @exception IOException if an error occurs.
     */
    public void write(byte b[]) throws IOException
    {
        checkThreshold(b.length);
        getStream().write(b);
        written += b.length;
    }


    /**
     * Writes <code>len</code> bytes from the specified byte array starting at
     * offset <code>off</code> to this output stream.
     *
     * @param b   The byte array from which the data will be written.
     * @param off The start offset in the byte array.
     * @param len The number of bytes to write.
     *
     * @exception IOException if an error occurs.
     */
    public void write(byte b[], int off, int len) throws IOException
    {
        checkThreshold(len);
        getStream().write(b, off, len);
        written += len;
    }


    /**
     * Flushes this output stream and forces any buffered output bytes to be
     * written out.
     *
     * @exception IOException if an error occurs.
     */
    public void flush() throws IOException
    {
        getStream().flush();
    }


    /**
     * Closes this output stream and releases any system resources associated
     * with this stream.
     *
     * @exception IOException if an error occurs.
     */
    public void close() throws IOException
    {
        try
        {
            flush();
        }
        catch (IOException ignored)
        {
            // ignore
        }
        getStream().close();
    }


    // --------------------------------------------------------- Public methods


    /**
     * Returns the threshold, in bytes, at which an event will be triggered.
     *
     * @return The threshold point, in bytes.
     */
    public int getThreshold()
    {
        return threshold;
    }


    /**
     * Returns the number of bytes that have been written to this output stream.
     *
     * @return The number of bytes written.
     */
    public long getByteCount()
    {
        return written;
    }


    /**
     * Determines whether or not the configured threshold has been exceeded for
     * this output stream.
     *
     * @return <code>true</code> if the threshold has been reached;
     *         <code>false</code> otherwise.
     */
    public boolean isThresholdExceeded()
    {
        return (written > threshold);
    }


    // ------------------------------------------------------ Protected methods


    /**
     * Checks to see if writing the specified number of bytes would cause the
     * configured threshold to be exceeded. If so, triggers an event to allow
     * a concrete implementation to take action on this.
     *
     * @param count The number of bytes about to be written to the underlying
     *              output stream.
     *
     * @exception IOException if an error occurs.
     */
    protected void checkThreshold(int count) throws IOException
    {
        if (!thresholdExceeded && (written + count > threshold))
        {
            thresholdReached();
            thresholdExceeded = true;
        }
    }


    // ------------------------------------------------------- Abstract methods


    /**
     * Returns the underlying output stream, to which the corresponding
     * <code>OutputStream</code> methods in this class will ultimately delegate.
     *
     * @return The underlying output stream.
     *
     * @exception IOException if an error occurs.
     */
    protected abstract OutputStream getStream() throws IOException;


    /**
     * Indicates that the configured threshold has been reached, and that a
     * subclass should take whatever action necessary on this event. This may
     * include changing the underlying output stream.
     *
     * @exception IOException if an error occurs.
     */
    protected abstract void thresholdReached() throws IOException;
}
