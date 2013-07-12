/*
 * $Header: /home/cvs/jakarta-commons/fileupload/src/java/org/apache/commons/fileupload/DefaultFileItem.java,v 1.21 2003/06/24 05:45:15 martinc Exp $
 * $Revision: 1.21 $
 * $Date: 2003/06/24 05:45:15 $
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


import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;


/**
 * <p> The default implementation of the
 * {@link org.apache.commons.fileupload.FileItem FileItem} interface.
 *
 * <p> After retrieving an instance of this class from a {@link
 * org.apache.commons.fileupload.DiskFileUpload DiskFileUpload} instance (see
 * {@link org.apache.commons.fileupload.DiskFileUpload
 * #parseRequest(javax.servlet.http.HttpServletRequest)}), you may
 * either request all contents of file at once using {@link #get()} or
 * request an {@link java.io.InputStream InputStream} with
 * {@link #getInputStream()} and process the file without attempting to load
 * it into memory, which may come handy with large files.
 *
 * @author <a href="mailto:Rafal.Krzewski@e-point.pl">Rafal Krzewski</a>
 * @author <a href="mailto:sean@informage.net">Sean Legassick</a>
 * @author <a href="mailto:jvanzyl@apache.org">Jason van Zyl</a>
 * @author <a href="mailto:jmcnally@apache.org">John McNally</a>
 * @author <a href="mailto:martinc@apache.org">Martin Cooper</a>
 * @author Sean C. Sullivan
 *
 * @version $Id: DefaultFileItem.java,v 1.21 2003/06/24 05:45:15 martinc Exp $
 */
public class DefaultFileItem
    implements FileItem
{

    // ----------------------------------------------------------- Data members


    /**
     * Counter used in unique identifier generation.
     */
    private static int counter = 0;


    /**
     * The name of the form field as provided by the browser.
     */
    private String fieldName;


    /**
     * The content type passed by the browser, or <code>null</code> if
     * not defined.
     */
    private String contentType;


    /**
     * Whether or not this item is a simple form field.
     */
    private boolean isFormField;


    /**
     * The original filename in the user's filesystem.
     */
    private String fileName;


    /**
     * The threshold above which uploads will be stored on disk.
     */
    private int sizeThreshold;


    /**
     * The directory in which uploaded files will be stored, if stored on disk.
     */
    private File repository;


    /**
     * Cached contents of the file.
     */
    private byte[] cachedContent;


    /**
     * Output stream for this item.
     */
    private DeferredFileOutputStream dfos;


    // ----------------------------------------------------------- Constructors


    /**
     * Constructs a new <code>DefaultFileItem</code> instance.
     *
     * @param fieldName     The name of the form field.
     * @param contentType   The content type passed by the browser or
     *                      <code>null</code> if not specified.
     * @param isFormField   Whether or not this item is a plain form field, as
     *                      opposed to a file upload.
     * @param fileName      The original filename in the user's filesystem, or
     *                      <code>null</code> if not specified.
     * @param sizeThreshold The threshold, in bytes, below which items will be
     *                      retained in memory and above which they will be
     *                      stored as a file.
     * @param repository    The data repository, which is the directory in
     *                      which files will be created, should the item size
     *                      exceed the threshold.
     */
    DefaultFileItem(String fieldName, String contentType, boolean isFormField,
                    String fileName, int sizeThreshold, File repository)
    {
        this.fieldName = fieldName;
        this.contentType = contentType;
        this.isFormField = isFormField;
        this.fileName = fileName;
        this.sizeThreshold = sizeThreshold;
        this.repository = repository;
    }


    // ------------------------------- Methods from javax.activation.DataSource


    /**
     * Returns an {@link java.io.InputStream InputStream} that can be
     * used to retrieve the contents of the file.
     *
     * @return An {@link java.io.InputStream InputStream} that can be
     *         used to retrieve the contents of the file.
     *
     * @exception IOException if an error occurs.
     */
    public InputStream getInputStream()
        throws IOException
    {
        if (!dfos.isInMemory())
        {
            return new FileInputStream(dfos.getFile());
        }

        if (cachedContent == null)
        {
            cachedContent = dfos.getData();
        }
        return new ByteArrayInputStream(cachedContent);
    }


    /**
     * Returns the content type passed by the browser or <code>null</code> if
     * not defined.
     *
     * @return The content type passed by the browser or <code>null</code> if
     *         not defined.
     */
    public String getContentType()
    {
        return contentType;
    }


    /**
     * Returns the original filename in the client's filesystem.
     *
     * @return The original filename in the client's filesystem.
     */
    public String getName()
    {
        return fileName;
    }


    // ------------------------------------------------------- FileItem methods


    /**
     * Provides a hint as to whether or not the file contents will be read
     * from memory.
     *
     * @return <code>true</code> if the file contents will be read
     *         from memory; <code>false</code> otherwise.
     */
    public boolean isInMemory()
    {
        return (dfos.isInMemory());
    }


    /**
     * Returns the size of the file.
     *
     * @return The size of the file, in bytes.
     */
    public long getSize()
    {
        if (cachedContent != null)
        {
            return cachedContent.length;
        }
        else if (dfos.isInMemory())
        {
            return dfos.getData().length;
        }
        else
        {
            return dfos.getFile().length();
        }
    }


    /**
     * Returns the contents of the file as an array of bytes.  If the
     * contents of the file were not yet cached in memory, they will be
     * loaded from the disk storage and cached.
     *
     * @return The contents of the file as an array of bytes.
     */
    public byte[] get()
    {
        if (dfos.isInMemory())
        {
            if (cachedContent == null)
            {
                cachedContent = dfos.getData();
            }
            return cachedContent;
        }

        byte[] fileData = new byte[(int) getSize()];
        FileInputStream fis = null;

        try
        {
            fis = new FileInputStream(dfos.getFile());
            fis.read(fileData);
        }
        catch (IOException e)
        {
            fileData = null;
        }
        finally
        {
            if (fis != null)
            {
                try
                {
                    fis.close();
                }
                catch (IOException e)
                {
                    // ignore
                }
            }
        }

        return fileData;
    }


    /**
     * Returns the contents of the file as a String, using the specified
     * encoding.  This method uses {@link #get()} to retrieve the
     * contents of the file.
     *
     * @param encoding The character encoding to use.
     *
     * @return The contents of the file, as a string.
     *
     * @exception UnsupportedEncodingException if the requested character
     *                                         encoding is not available.
     */
    public String getString(String encoding)
        throws UnsupportedEncodingException
    {
        return new String(get(), encoding);
    }


    /**
     * Returns the contents of the file as a String, using the default
     * character encoding.  This method uses {@link #get()} to retrieve the
     * contents of the file.
     *
     * @return The contents of the file, as a string.
     */
    public String getString()
    {
        return new String(get());
    }


    /**
     * A convenience method to write an uploaded item to disk. The client code
     * is not concerned with whether or not the item is stored in memory, or on
     * disk in a temporary location. They just want to write the uploaded item
     * to a file.
     * <p>
     * This implementation first attempts to rename the uploaded item to the
     * specified destination file, if the item was originally written to disk.
     * Otherwise, the data will be copied to the specified file.
     * <p>
     * This method is only guaranteed to work <em>once</em>, the first time it
     * is invoked for a particular item. This is because, in the event that the
     * method renames a temporary file, that file will no longer be available
     * to copy or rename again at a later time.
     *
     * @param file The <code>File</code> into which the uploaded item should
     *             be stored.
     *
     * @exception Exception if an error occurs.
     */
    public void write(File file) throws Exception
    {
        if (isInMemory())
        {
            FileOutputStream fout = null;
            try
            {
                fout = new FileOutputStream(file);
                fout.write(get());
            }
            finally
            {
                if (fout != null)
                {
                    fout.close();
                }
            }
        }
        else
        {
            File outputFile = getStoreLocation();
            if (outputFile != null)
            {
                /*
                 * The uploaded file is being stored on disk
                 * in a temporary location so move it to the
                 * desired file.
                 */
                if (!outputFile.renameTo(file))
                {
                    BufferedInputStream in = null;
                    BufferedOutputStream out = null;
                    try
                    {
                        in = new BufferedInputStream(
                            new FileInputStream(outputFile));
                        out = new BufferedOutputStream(
                                new FileOutputStream(file));
                        byte[] bytes = new byte[2048];
                        int s = 0;
                        while ((s = in.read(bytes)) != -1)
                        {
                            out.write(bytes, 0, s);
                        }
                    }
                    finally
                    {
                        try
                        {
                            in.close();
                        }
                        catch (IOException e)
                        {
                            // ignore
                        }
                        try
                        {
                            out.close();
                        }
                        catch (IOException e)
                        {
                            // ignore
                        }
                    }
                }
            }
            else
            {
                /*
                 * For whatever reason we cannot write the
                 * file to disk.
                 */
                throw new FileUploadException(
                    "Cannot write uploaded file to disk!");
            }
        }
    }


    /**
     * Deletes the underlying storage for a file item, including deleting any
     * associated temporary disk file. Although this storage will be deleted
     * automatically when the <code>FileItem</code> instance is garbage
     * collected, this method can be used to ensure that this is done at an
     * earlier time, thus preserving system resources.
     */
    public void delete()
    {
        cachedContent = null;
        File outputFile = getStoreLocation();
        if (outputFile != null && outputFile.exists())
        {
            outputFile.delete();
        }
    }


    /**
     * Returns the name of the field in the multipart form corresponding to
     * this file item.
     *
     * @return The name of the form field.
     *
     * @see #setFieldName(java.lang.String)
     *
     */
    public String getFieldName()
    {
        return fieldName;
    }


    /**
     * Sets the field name used to reference this file item.
     *
     * @param fieldName The name of the form field.
     *
     * @see #getFieldName()
     *
     */
    public void setFieldName(String fieldName)
    {
        this.fieldName = fieldName;
    }


    /**
     * Determines whether or not a <code>FileItem</code> instance represents
     * a simple form field.
     *
     * @return <code>true</code> if the instance represents a simple form
     *         field; <code>false</code> if it represents an uploaded file.
     *
     * @see #setFormField(boolean)
     *
     */
    public boolean isFormField()
    {
        return isFormField;
    }


    /**
     * Specifies whether or not a <code>FileItem</code> instance represents
     * a simple form field.
     *
     * @param state <code>true</code> if the instance represents a simple form
     *              field; <code>false</code> if it represents an uploaded file.
     *
     * @see #isFormField()
     *
     */
    public void setFormField(boolean state)
    {
        isFormField = state;
    }


    /**
     * Returns an {@link java.io.OutputStream OutputStream} that can
     * be used for storing the contents of the file.
     *
     * @return An {@link java.io.OutputStream OutputStream} that can be used
     *         for storing the contensts of the file.
     *
     * @exception IOException if an error occurs.
     */
    public OutputStream getOutputStream()
        throws IOException
    {
        if (dfos == null)
        {
            File outputFile = getTempFile();
            dfos = new DeferredFileOutputStream(sizeThreshold, outputFile);
        }
        return dfos;
    }


    // --------------------------------------------------------- Public methods


    /**
     * Returns the {@link java.io.File} object for the <code>FileItem</code>'s
     * data's temporary location on the disk. Note that for
     * <code>FileItem</code>s that have their data stored in memory,
     * this method will return <code>null</code>. When handling large
     * files, you can use {@link java.io.File#renameTo(java.io.File)} to
     * move the file to new location without copying the data, if the
     * source and destination locations reside within the same logical
     * volume.
     *
     * @return The data file, or <code>null</code> if the data is stored in
     *         memory.
     */
    public File getStoreLocation()
    {
        return dfos.getFile();
    }


    // ------------------------------------------------------ Protected methods


    /**
     * Removes the file contents from the temporary storage.
     */
    protected void finalize()
    {
        File outputFile = dfos.getFile();

        if (outputFile != null && outputFile.exists())
        {
            outputFile.delete();
        }
    }


    /**
     * Creates and returns a {@link java.io.File File} representing a uniquely
     * named temporary file in the configured repository path.
     *
     * @return The {@link java.io.File File} to be used for temporary storage.
     */
    protected File getTempFile()
    {
        File tempDir = repository;
        if (tempDir == null)
        {
            tempDir = new File(System.getProperty("java.io.tmpdir"));
        }

        String fileName = "upload_" + getUniqueId() + ".tmp";

        File f = new File(tempDir, fileName);
        f.deleteOnExit();
        return f;
    }


    // -------------------------------------------------------- Private methods


    /**
     * Returns an identifier that is unique within the class loader used to
     * load this class, but does not have random-like apearance.
     *
     * @return A String with the non-random looking instance identifier.
     */
    private static String getUniqueId()
    {
        int current;
        synchronized (DefaultFileItem.class)
        {
            current = counter++;
        }
        String id = Integer.toString(current);

        // If you manage to get more than 100 million of ids, you'll
        // start getting ids longer than 8 characters.
        if (current < 100000000)
        {
            id = ("00000000" + id).substring(id.length());
        }
        return id;
    }

}
