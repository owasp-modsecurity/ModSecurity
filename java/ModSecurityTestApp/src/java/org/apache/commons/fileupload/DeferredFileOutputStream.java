/*
 * $Header: /home/cvs/jakarta-commons/fileupload/src/java/org/apache/commons/fileupload/DeferredFileOutputStream.java,v 1.2 2003/05/31 22:31:08 martinc Exp $
 * $Revision: 1.2 $
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

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * <p>An output stream which will retain data in memory until a specified
 * threshold is reached, and only then commit it to disk. If the stream is
 * closed before the threshold is reached, the data will not be written to
 * disk at all.</p>
 *
 * @author <a href="mailto:martinc@apache.org">Martin Cooper</a>
 *
 * @version $Id: DeferredFileOutputStream.java,v 1.2 2003/05/31 22:31:08 martinc Exp $
 */
public class DeferredFileOutputStream
    extends ThresholdingOutputStream
{

    // ----------------------------------------------------------- Data members


    /**
     * The output stream to which data will be written prior to the theshold
     * being reached.
     */
    private ByteArrayOutputStream memoryOutputStream;


    /**
     * The output stream to which data will be written after the theshold is
     * reached.
     */
    private FileOutputStream diskOutputStream;


    /**
     * The output stream to which data will be written at any given time. This
     * will always be one of <code>memoryOutputStream</code> or
     * <code>diskOutputStream</code>.
     */
    private OutputStream currentOutputStream;


    /**
     * The file to which output will be directed if the threshold is exceeded.
     */
    private File outputFile;


    // ----------------------------------------------------------- Constructors


    /**
     * Constructs an instance of this class which will trigger an event at the
     * specified threshold, and save data to a file beyond that point.
     *
     * @param threshold  The number of bytes at which to trigger an event.
     * @param outputFile The file to which data is saved beyond the threshold.
     */
    public DeferredFileOutputStream(int threshold, File outputFile)
    {
        super(threshold);
        this.outputFile = outputFile;

        memoryOutputStream = new ByteArrayOutputStream(threshold);
        currentOutputStream = memoryOutputStream;
    }


    // --------------------------------------- ThresholdingOutputStream methods


    /**
     * Returns the current output stream. This may be memory based or disk
     * based, depending on the current state with respect to the threshold.
     *
     * @return The underlying output stream.
     *
     * @exception IOException if an error occurs.
     */
    protected OutputStream getStream() throws IOException
    {
        return currentOutputStream;
    }


    /**
     * Switches the underlying output stream from a memory based stream to one
     * that is backed by disk. This is the point at which we realise that too
     * much data is being written to keep in memory, so we elect to switch to
     * disk-based storage.
     *
     * @exception IOException if an error occurs.
     */
    protected void thresholdReached() throws IOException
    {
        byte[] data = memoryOutputStream.toByteArray();
        FileOutputStream fos = new FileOutputStream(outputFile);
        fos.write(data);
        diskOutputStream = fos;
        currentOutputStream = fos;
        memoryOutputStream = null;
    }


    // --------------------------------------------------------- Public methods


    /**
     * Determines whether or not the data for this output stream has been
     * retained in memory.
     *
     * @return <code>true</code> if the data is available in memory;
     *         <code>false</code> otherwise.
     */
    public boolean isInMemory()
    {
        return (!isThresholdExceeded());
    }


    /**
     * Returns the data for this output stream as an array of bytes, assuming
     * that the data has been retained in memory. If the data was written to
     * disk, this method returns <code>null</code>.
     *
     * @return The data for this output stream, or <code>null</code> if no such
     *         data is available.
     */
    public byte[] getData()
    {
        if (memoryOutputStream != null)
        {
            return memoryOutputStream.toByteArray();
        }
        return null;
    }


    /**
     * Returns the data for this output stream as a <code>File</code>, assuming
     * that the data was written to disk. If the data was retained in memory,
     * this method returns <code>null</code>.
     *
     * @return The file for this output stream, or <code>null</code> if no such
     *         file exists.
     */
    public File getFile()
    {
        return outputFile;
    }
}
