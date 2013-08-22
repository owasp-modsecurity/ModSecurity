package org.modsecurity;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Locale;
import java.util.TimeZone;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;

public class MsHttpServletResponse extends HttpServletResponseWrapper {

    private static final int INTERCEPT_OFF = 0;
    private static final int INTERCEPT_ON = 1;
    private static final int INTERCEPT_OBSERVE_ONLY = 2;
    public static final String DEFAULT_CHARACTER_ENCODING = "ISO-8859-1";
    private int interceptMode = INTERCEPT_ON;
    private ArrayList<Object> headers = new ArrayList<Object>();
    private ArrayList<Object> cookies = new ArrayList<Object>();
    private int status = -1;
    private boolean committed = false;
    private boolean suspended = false;
    private boolean destroyed = false;
    private String statusMessage;
    private String contentType;
    private String characterEncoding;
    private int contentLength = -1;
    private Locale locale;
    private MsOutputStream msOutputStream;
    private MsWriter msWriter;
    protected SimpleDateFormat formats[] = {
        new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.US),
        new SimpleDateFormat("EEEEEE, dd-MMM-yy HH:mm:ss zzz", Locale.US),
        new SimpleDateFormat("EEE MMMM d HH:mm:ss yyyy", Locale.US)
    };

    private class Header {

        String name;
        String value;

        Header(String name, String value) {
            this.name = name;
            this.value = value;
        }
    }

    public MsHttpServletResponse(HttpServletResponse response) {
        super(response);

        characterEncoding = DEFAULT_CHARACTER_ENCODING;
        TimeZone GMT_ZONE = TimeZone.getTimeZone("GMT");
        formats[0].setTimeZone(GMT_ZONE);
        formats[1].setTimeZone(GMT_ZONE);
        formats[2].setTimeZone(GMT_ZONE);
        locale = Locale.getDefault();
    }

    public void destroy() throws IOException {
        if (destroyed == true) {
            return;
        }

        if (interceptMode == INTERCEPT_ON) {
            if (status != -1) {
                super.setStatus(status);
            }
            if (contentType != null) {
                super.setContentType(contentType);
            }
            if (characterEncoding != null) {
                super.setCharacterEncoding(characterEncoding);
            }
            if (contentLength != -1) {
                super.setContentLength(contentLength);
            }
            if (locale != null) {
                super.setLocale(locale);
            }

            // send cookies
            for (int i = 0, n = cookies.size(); i < n; i++) {
                super.addCookie((Cookie) cookies.get(i));
            }

            // send headers
            for (int i = 0, n = headers.size(); i < n; i++) {
                Header h = (Header) headers.get(i);
                // TODO don't send our cookie headers because
                // they are not well implemented yet. Cookies
                // are sent directly
                if (h.name.compareTo("Set-Cookie") != 0) {
                    super.addHeader(h.name, h.value);
                }
            }
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
    public String getContentType() {
        if (interceptMode != INTERCEPT_OFF) {
            return contentType;
        }
        return super.getContentType();
    }

    @Override
    public ServletOutputStream getOutputStream() throws IllegalStateException, IOException {
        if (interceptMode != INTERCEPT_OFF) {
            if (msWriter != null) {
                throw new IllegalStateException();
            }
            if (msOutputStream == null) {
                msOutputStream = new MsOutputStream(super.getOutputStream());
            }
            if (interceptMode == INTERCEPT_ON) {
                msOutputStream.setBuffering(true);
            }
            return msOutputStream;
        } else {
            return super.getOutputStream();
        }
    }

    @Override
    public PrintWriter getWriter() throws IllegalStateException, IOException {
        if (interceptMode != INTERCEPT_OFF) {
            if (msOutputStream != null) {
                throw new IllegalStateException();
            }
            if (msWriter == null) {
                msWriter = new MsWriter(super.getWriter());
            }
            if (interceptMode == INTERCEPT_ON) {
                msWriter.setBuffering(true);
            }
            return msWriter;
        } else {
            return super.getWriter();
        }
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

    @Override
    public void setCharacterEncoding(String charset) {
        if (interceptMode != INTERCEPT_ON) {
            super.setCharacterEncoding(charset);
        }
        if (interceptMode != INTERCEPT_OFF) {
            characterEncoding = charset;
        }
    }

    @Override
    public void setContentLength(int contentLength) {
        if (interceptMode != INTERCEPT_ON) {
            super.setContentLength(contentLength);
        }
        if (interceptMode != INTERCEPT_OFF) {
            this.contentLength = contentLength;
            headers.add(new Header("Content-Length", Integer.toString(contentLength)));
        }
    }

    @Override
    public void setContentType(String contentType) {
        if (interceptMode != INTERCEPT_ON) {
            super.setContentType(contentType);
        }
        if (interceptMode != INTERCEPT_OFF) {
            this.contentType = contentType;
            headers.add(new Header("Content-Type", contentType));
        }
    }

    @Override
    public void setBufferSize(int size) throws IllegalStateException {
        super.setBufferSize(size);
    }

    @Override
    public int getBufferSize() {
        return super.getBufferSize();
    }

    @Override
    public void flushBuffer() throws IOException {
        if (interceptMode != INTERCEPT_ON) {
            super.flushBuffer();
        }
        if (interceptMode != INTERCEPT_OFF) {
            committed = true;
        }
    }

    @Override
    public void resetBuffer() {
        if (interceptMode != INTERCEPT_ON) {
            super.resetBuffer();
        }
        if (interceptMode != INTERCEPT_OFF) {
            if (committed) {
                throw new IllegalStateException();
            }

            if (msWriter != null) {
                msWriter.reset();
            }
            if (msOutputStream != null) {
                msOutputStream.reset();
            }
        }
    }

    @Override
    public boolean isCommitted() {
        if (interceptMode != INTERCEPT_OFF) {
            return committed;
        }
        return super.isCommitted();
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

    @Override
    public void reset() throws IllegalStateException {
        if (interceptMode != INTERCEPT_ON) {
            super.reset();
        }
        if (interceptMode != INTERCEPT_OFF) {
            if (committed) {
                throw new IllegalStateException();
            }

            status = 200;
            characterEncoding = DEFAULT_CHARACTER_ENCODING;
            contentType = null;
            contentLength = -1;
            headers.clear();
            status = 200;
            statusMessage = null;

            if (msWriter != null) {
                msWriter.reset();
            }
            if (msOutputStream != null) {
                msOutputStream.reset();
            }
        }
    }

    @Override
    public void setLocale(Locale locale) {
        if (interceptMode != INTERCEPT_ON) {
            super.setLocale(locale);
        }
        if (interceptMode != INTERCEPT_OFF) {
            this.locale = locale;
        }
    }

    @Override
    public Locale getLocale() {
        if (interceptMode != INTERCEPT_OFF) {
            return locale;
        }
        return super.getLocale();
    }

    @Override
    public void addCookie(Cookie cookie) {
        if (interceptMode != INTERCEPT_ON) {
            super.addCookie(cookie);
        }
        if (interceptMode != INTERCEPT_OFF) {
            cookies.add(cookie);
            // TODO improve; these headers will not be
            // sent to the client
            StringBuilder sb = new StringBuilder();
            sb.append(cookie.getName());
            sb.append("=");
            if (cookie.getDomain() != null) {
                sb.append("; domain=").append(cookie.getDomain());
            }
            if (cookie.getPath() != null) {
                sb.append("; path=").append(cookie.getPath());
            }
            if (cookie.getSecure()) {
                sb.append("; secure");
            }
            headers.add(new Header("Set-Cookie", sb.toString()));
        }
    }

    @Override
    public void addDateHeader(String name, long value) {
        SimpleDateFormat format = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.US);
        format.setTimeZone(TimeZone.getTimeZone("GMT"));
        this.addHeader(name, FastHttpDateFormat.formatDate(value, format));
    }

    @Override
    public void addHeader(String name, String value) {
        if (interceptMode != INTERCEPT_ON) {
            super.addHeader(name, value);
        }
        if (interceptMode != INTERCEPT_OFF) {
            headers.add(new Header(name, value));
        }
    }

    @Override
    public void addIntHeader(String name, int value) {
        this.addHeader(name, Integer.toString(value));
    }

    @Override
    public boolean containsHeader(String name) {
        if (interceptMode == INTERCEPT_OFF) {
            return super.containsHeader(name);
        } else {
            for (int i = 0, n = headers.size(); i < n; i++) {
                Header h = (Header) headers.get(i);
                if (h.name.compareTo(name) == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    @Override
    public void setDateHeader(String name, long value) {
        SimpleDateFormat format = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.US);
        format.setTimeZone(TimeZone.getTimeZone("GMT"));
        this.setHeader(name, FastHttpDateFormat.formatDate(value, format));
    }

    @Override
    public void setHeader(String name, String value) {
        if (interceptMode != INTERCEPT_ON) {
            super.setHeader(name, value);
        }
        if (interceptMode != INTERCEPT_OFF) {
            for (int i = 0, n = headers.size(); i < n; i++) {
                Header h = (Header) headers.get(i);
                if (h.name.compareTo(name) == 0) {
                    headers.remove(i);
                    i--;
                }
            }
            headers.add(new Header(name, value));
        }
    }

    @Override
    public void setIntHeader(String name, int value) {
        this.setHeader(name, Integer.toString(value));
    }

    @Override
    public void setStatus(int status) {
        if (interceptMode != INTERCEPT_ON) {
            super.setStatus(status);
        }
        if (interceptMode != INTERCEPT_OFF) {
            this.status = status;
        }
    }

    @Override
    public void setStatus(int status, String message) {
        if (interceptMode != INTERCEPT_ON) {
            super.setStatus(status);
        }
        if (interceptMode != INTERCEPT_OFF) {
            this.status = status;
            this.statusMessage = message;
        }
    }

    @Override
    public void sendError(int status) throws IOException {
        if (interceptMode != INTERCEPT_ON) {
            super.sendError(status);
        }
        if (interceptMode != INTERCEPT_OFF) {
            this.status = status;
            this.setSuspended(true);
        }
    }

    @Override
    public void sendError(int status, String message) throws IOException {
        if (interceptMode != INTERCEPT_ON) {
            super.sendError(status);
        }
        if (interceptMode != INTERCEPT_OFF) {
            this.status = status;
            this.statusMessage = message;
            this.setSuspended(true);
        }
    }

    /* -- Inspection methods ---------------------------------------------- */
    // TODO throw exception when interceptMode set to OFF
    public Cookie[] getCookies() {
        return (Cookie[]) cookies.toArray(new Cookie[cookies.size()]);
    }

    @Override
    public int getStatus() {
        return status;
    }

    public int getContentLength() {
        return contentLength;
    }

    public long getDateHeader(String name) throws IllegalArgumentException {
        String value = this.getHeader(name);
        if (value == null) {
            return -1;
        }

        long result = FastHttpDateFormat.parseDate(value, formats);
        if (result == -1) {
            throw new IllegalArgumentException(value);
        }

        return result;
    }

    @Override
    public String getHeader(String name) {
        for (int i = 0, n = headers.size(); i < n; i++) {
            Header h = (Header) headers.get(i);
            if (h.name.compareTo(name) == 0) {
                return h.value;
            }
        }
        return null;
    }

    @Override
    public Collection<String> getHeaderNames() {
        Collection<String> headerNames = new LinkedList<String>();
        for (int i = 0, n = headers.size(); i < n; i++) {
            Header h = (Header) headers.get(i);
            headerNames.add(h.value);
        }
        return headerNames;
    }

    public int getIntHeader(String name) throws NumberFormatException {
        String value = this.getHeader(name);
        if (value == null) {
            return -1;
        }
        return Integer.parseInt(value);
    }

    @Override
    public Collection<String> getHeaders(String name) {
        Collection<String> headerValues = new LinkedList<String>();
        for (int i = 0, n = headers.size(); i < n; i++) {
            Header h = (Header) headers.get(i);
            if (h.name.compareTo(name) == 0) {
                headerValues.add(h.value);
            }
        }
        return headerValues;
    }
}

/**
 * Utility class to generate HTTP dates.
 *
 * @author Remy Maucherat
 */
final class FastHttpDateFormat {

    // -------------------------------------------------------------- Variables
    /**
     * HTTP date format.
     */
    protected static final SimpleDateFormat format =
            new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.US);
    /**
     * The set of SimpleDateFormat formats to use in getDateHeader().
     */
    protected static final SimpleDateFormat formats[] = {
        new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.US),
        new SimpleDateFormat("EEEEEE, dd-MMM-yy HH:mm:ss zzz", Locale.US),
        new SimpleDateFormat("EEE MMMM d HH:mm:ss yyyy", Locale.US)
    };
    protected final static TimeZone gmtZone = TimeZone.getTimeZone("GMT");

    /**
     * GMT timezone - all HTTP dates are on GMT
     */
    static {

        format.setTimeZone(gmtZone);

        formats[0].setTimeZone(gmtZone);
        formats[1].setTimeZone(gmtZone);
        formats[2].setTimeZone(gmtZone);

    }
    /**
     * Instant on which the currentDate object was generated.
     */
    protected static long currentDateGenerated = 0L;
    /**
     * Current formatted date.
     */
    protected static String currentDate = null;
    /**
     * Formatter cache.
     */
    protected static final HashMap<Object, Object> formatCache = new HashMap<Object, Object>();
    /**
     * Parser cache.
     */
    protected static final HashMap<Object, Object> parseCache = new HashMap<Object, Object>();

    // --------------------------------------------------------- Public Methods
    /**
     * Get the current date in HTTP format.
     */
    public static String getCurrentDate() {

        long now = System.currentTimeMillis();
        if ((now - currentDateGenerated) > 1000) {
            synchronized (format) {
                if ((now - currentDateGenerated) > 1000) {
                    currentDateGenerated = now;
                    currentDate = format.format(new Date(now));
                }
            }
        }
        return currentDate;

    }

    /**
     * Get the HTTP format of the specified date.
     */
    public static String formatDate(long value, DateFormat threadLocalformat) {

        String cachedDate = null;
        Long longValue = new Long(value);
        try {
            cachedDate = (String) formatCache.get(longValue);
        } catch (Exception e) {
        }
        if (cachedDate != null) {
            return cachedDate;
        }

        String newDate;
        Date dateValue = new Date(value);
        if (threadLocalformat != null) {
            newDate = threadLocalformat.format(dateValue);
            synchronized (formatCache) {
                updateCache(formatCache, longValue, newDate);
            }
        } else {
            synchronized (formatCache) {
                newDate = format.format(dateValue);
                updateCache(formatCache, longValue, newDate);
            }
        }
        return newDate;

    }

    /**
     * Try to parse the given date as a HTTP date.
     */
    public static long parseDate(String value,
            DateFormat[] threadLocalformats) {

        Long cachedDate = null;
        try {
            cachedDate = (Long) parseCache.get(value);
        } catch (Exception e) {
        }
        if (cachedDate != null) {
            return cachedDate.longValue();
        }

        Long date;
        if (threadLocalformats != null) {
            date = internalParseDate(value, threadLocalformats);
            synchronized (parseCache) {
                updateCache(parseCache, value, date);
            }
        } else {
            synchronized (parseCache) {
                date = internalParseDate(value, formats);
                updateCache(parseCache, value, date);
            }
        }
        if (date == null) {
            return (-1L);
        } else {
            return date.longValue();
        }

    }

    /**
     * Parse date with given formatters.
     */
    private static Long internalParseDate(String value, DateFormat[] formats) {
        Date date = null;
        for (int i = 0; (date == null) && (i < formats.length); i++) {
            try {
                date = formats[i].parse(value);
            } catch (ParseException e) {
            }
        }
        if (date == null) {
            return null;
        }
        return new Long(date.getTime());
    }

    /**
     * Update cache.
     */
    private static void updateCache(HashMap<Object, Object> cache, Object key,
            Object value) {
        if (value == null) {
            return;
        }
        if (cache.size() > 1000) {
            cache.clear();
        }
        cache.put(key, value);
    }
}
