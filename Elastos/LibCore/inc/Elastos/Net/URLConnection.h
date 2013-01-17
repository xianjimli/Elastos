
#ifndef __URLCONNECTION_H__
#define __URLCONNECTION_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

// extern "C" const InterfaceID EIID_URLConnection;

/**
 * A connection to a URL for reading or writing. For HTTP connections, see
 * {@link HttpURLConnection} for documentation of HTTP-specific features.
 *
 * <p>For example, to retrieve {@code
 * ftp://mirror.csclub.uwaterloo.ca/index.html}: <pre>   {@code
 *   URL url = new URL("ftp://mirror.csclub.uwaterloo.ca/index.html");
 *   URLConnection urlConnection = url.openConnection();
 *   InputStream in = new BufferedInputStream(urlConnection.getInputStream());
 *   try {
 *     readStream(in);
 *   } finally {
 *     in.close();
 *   }
 * }</pre>
 *
 * <p>{@code URLConnection} must be configured before it has connected to the
 * remote resource. Instances of {@code URLConnection} are not reusable: you
 * must use a different instance for each connection to a resource.
 *
 * <h3>Timeouts</h3>
 * {@code URLConnection} supports two timeouts: a {@link #setConnectTimeout
 * connect timeout} and a {@link #setReadTimeout read timeout}. By default,
 * operations never time out.
 *
 * <h3>Built-in Protocols</h3>
 * <ul>
 *   <li><strong>File</strong><br>
 *      Resources from the local file system can be loaded using {@code file:}
 *      URIs. File connections can only be used for input.
 *   <li><strong>FTP</strong><br>
 *      File Transfer Protocol (<a href="http://www.ietf.org/rfc/rfc959.txt">RFC
 *      959</a>) is supported, but with no public subclass. FTP connections can
 *      be used for input or output but not both.
 *      <p>By default, FTP connections will be made using {@code anonymous} as
 *      the username and the empty string as the password. Specify alternate
 *      usernames and passwords in the URL: {@code
 *      ftp://username:password@host/path}.
 *   <li><strong>HTTP and HTTPS</strong><br>
 *      Refer to the {@link HttpURLConnection} and {@link
 *      javax.net.ssl.HttpsURLConnection HttpsURLConnection} subclasses.
 *   <li><strong>Jar</strong><br>
 *      Refer to the {@link JarURLConnection} subclass.
 * </ul>
 *
 * <h3>Registering Additional Protocols</h3>
 * Use {@link URL#setURLStreamHandlerFactory} to register handlers for other
 * protocol types.
 */
class URLConnection
{
public:
    class DefaultContentHandler
        : public ElRefBase
        , public IContentHandler
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetContent(
            /* [in] */ IURLConnection* uConn,
            /* [out] */ IInterface** obj);

        CARAPI GetContentEx(
            /* [in] */ IURLConnection* uConn,
            /* [in] */ const ArrayOf<InterfaceID>& types,
            /* [out] */ IInterface** obj);
    };

public:
    /**
     * Opens a connection to the resource. This method will <strong>not</strong>
     * reconnect to a resource after the initial connection has been closed.
     *
     * @throws IOException
     *             if an error occurs while connecting to the resource.
     */
    virtual CARAPI Connect() = 0;

    /**
     * Returns the option value which indicates whether user interaction is allowed
     * on this {@code URLConnection}.
     *
     * @return the value of the option {@code allowUserInteraction}.
     * @see #allowUserInteraction
     */
    virtual CARAPI GetAllowUserInteraction(
        /* [out] */ Boolean* value);

    /**
     * Returns an object representing the content of the resource this {@code
     * URLConnection} is connected to. First, it attempts to get the content
     * type from the method {@code getContentType()} which looks at the response
     * header field "Content-Type". If none is found it will guess the content
     * type from the filename extension. If that fails the stream itself will be
     * used to guess the content type.
     *
     * @return the content representing object.
     * @throws IOException
     *             if an error occurs obtaining the content.
     */
    virtual CARAPI GetContent(
        /* [out] */ IInterface** content);

    /**
     * Returns an object representing the content of the resource this {@code
     * URLConnection} is connected to. First, it attempts to get the content
     * type from the method {@code getContentType()} which looks at the response
     * header field "Content-Type". If none is found it will guess the content
     * type from the filename extension. If that fails the stream itself will be
     * used to guess the content type. The content type must match with one of
     * the list {@code types}.
     *
     * @param types
     *            the list of acceptable content types.
     * @return the content representing object or {@code null} if the content
     *         type does not match with one of the specified types.
     * @throws IOException
     *             if an error occurs obtaining the content.
     */
    // Param is not generic in spec
//        public Object getContent(Class[] types);

    /**
     * Returns the content encoding type specified by the response header field
     * {@code content-encoding} or {@code null} if this field is not set.
     *
     * @return the value of the response header field {@code content-encoding}.
     */
    virtual CARAPI GetContentEncoding(
        /* [out] */ String* encoding);

    /**
     * Returns the content length in bytes specified by the response header field
     * {@code content-length} or {@code -1} if this field is not set.
     *
     * @return the value of the response header field {@code content-length}.
     */
    virtual CARAPI GetContentLength(
        /* [out] */ Int32* length);

    /**
     * Returns the MIME-type of the content specified by the response header field
     * {@code content-type} or {@code null} if type is unknown.
     *
     * @return the value of the response header field {@code content-type}.
     */
    virtual CARAPI GetContentType(
        /* [out] */ String* type);

    /**
     * Returns the timestamp when this response has been sent as a date in
     * milliseconds since January 1, 1970 GMT or {@code 0} if this timestamp is
     * unknown.
     *
     * @return the sending timestamp of the current response.
     */
    virtual CARAPI GetDate(
        /* [out] */ Int64* date);

    /**
     * Returns the default setting whether this connection allows user interaction.
     *
     * @return the value of the default setting {@code
     *         defaultAllowUserInteraction}.
     * @see #allowUserInteraction
     */
    static CARAPI_(Boolean) GetDefaultAllowUserInteraction();

    /**
     * Returns the default value for the specified request {@code field} or {@code
     * null} if the field could not be found. The base implementation of this
     * method returns always {@code null}.
     *
     * @param field
     *            the request field whose default value shall be returned.
     * @return the default value for the given field.
     * @deprecated Use {@link #getRequestProperty}
     */
    //@Deprecated
    static CARAPI_(String) GetDefaultRequestProperty(
        /* [in] */ const String& field);

    /**
     * Returns the default setting whether this connection allows using caches.
     *
     * @return the value of the default setting {@code defaultUseCaches}.
     * @see #useCaches
     */
    virtual CARAPI GetDefaultUseCaches(
        /* [out] */ Boolean* value);

    /**
     * Returns the value of the option {@code doInput} which specifies whether this
     * connection allows to receive data.
     *
     * @return {@code true} if this connection allows input, {@code false}
     *         otherwise.
     * @see #doInput
     */
    virtual CARAPI GetDoInput(
        /* [out] */ Boolean* allowed);

    /**
     * Returns the value of the option {@code doOutput} which specifies whether
     * this connection allows to send data.
     *
     * @return {@code true} if this connection allows output, {@code false}
     *         otherwise.
     * @see #doOutput
     */
    virtual CARAPI GetDoOutput(
        /* [out] */ Boolean* allowed);

    /**
     * Returns the timestamp when this response will be expired in milliseconds
     * since January 1, 1970 GMT or {@code 0} if this timestamp is unknown.
     *
     * @return the value of the response header field {@code expires}.
     */
    virtual CARAPI GetExpiration(
        /* [out] */ Int64* expiration);

    /**
     * Returns the table which is used by all {@code URLConnection} instances to
     * determine the MIME-type according to a file extension.
     *
     * @return the file name map to determine the MIME-type.
     */
    static CARAPI_(AutoPtr<IFileNameMap>) GetFileNameMap();

    /**
     * Returns the header value at the field position {@code pos} or {@code null}
     * if the header has fewer than {@code pos} fields. The base
     * implementation of this method returns always {@code null}.
     *
     * <p>Some implementations (notably {@code HttpURLConnection}) include a mapping
     * for the null key; in HTTP's case, this maps to the HTTP status line and is
     * treated as being at position 0 when indexing into the header fields.
     *
     * @param pos
     *            the field position of the response header.
     * @return the value of the field at position {@code pos}.
     */
    virtual CARAPI GetHeaderFieldByPosition(
        /* [in] */ Int32 pos,
        /* [out] */ String* value);

    /**
     * Returns an unchangeable map of the response-header fields and values. The
     * response-header field names are the key values of the map. The map values
     * are lists of header field values associated with a particular key name.
     *
     * <p>Some implementations (notably {@code HttpURLConnection}) include a mapping
     * for the null key; in HTTP's case, this maps to the HTTP status line and is
     * treated as being at position 0 when indexing into the header fields.
     *
     * @return the response-header representing generic map.
     * @since 1.4
     */
//    public Map<String, List<String>> getHeaderFields()

    /**
     * Returns an unchangeable map of general request properties used by this
     * connection. The request property names are the key values of the map. The
     * map values are lists of property values of the corresponding key name.
     *
     * @return the request-property representing generic map.
     * @since 1.4
     */
//    public Map<String, List<String>> getRequestProperties()

    /**
     * Adds the given property to the request header. Existing properties with
     * the same name will not be overwritten by this method.
     *
     * @param field
     *            the request property field name to add.
     * @param newValue
     *            the value of the property which is to add.
     * @throws IllegalStateException
     *             if the connection has been already established.
     * @throws NullPointerException
     *             if the property name is {@code null}.
     * @since 1.4
     */
    virtual CARAPI AddRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    /**
     * Returns the value of the header field specified by {@code key} or {@code
     * null} if there is no field with this name. The base implementation of
     * this method returns always {@code null}.
     *
     * <p>Some implementations (notably {@code HttpURLConnection}) include a mapping
     * for the null key; in HTTP's case, this maps to the HTTP status line and is
     * treated as being at position 0 when indexing into the header fields.
     *
     * @param key
     *            the name of the header field.
     * @return the value of the header field.
     */
    virtual CARAPI GetHeaderFieldByKey(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    /**
     * Returns the specified header value as a date in milliseconds since January
     * 1, 1970 GMT. Returns the {@code defaultValue} if no such header field
     * could be found.
     *
     * @param field
     *            the header field name whose value is needed.
     * @param defaultValue
     *            the default value if no field has been found.
     * @return the value of the specified header field as a date in
     *         milliseconds.
     */
    virtual CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    /**
     * Returns the specified header value as a number. Returns the {@code
     * defaultValue} if no such header field could be found or the value could
     * not be parsed as an {@code Integer}.
     *
     * @param field
     *            the header field name whose value is needed.
     * @param defaultValue
     *            the default value if no field has been found.
     * @return the value of the specified header field as a number.
     */
    virtual CARAPI GetHeaderFieldInt32(
        /* [in] */ const String& field,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    /**
     * Returns the name of the header field at the given position {@code posn} or
     * {@code null} if there are fewer than {@code posn} fields. The base
     * implementation of this method returns always {@code null}.
     *
     * <p>Some implementations (notably {@code HttpURLConnection}) include a mapping
     * for the null key; in HTTP's case, this maps to the HTTP status line and is
     * treated as being at position 0 when indexing into the header fields.
     *
     * @param posn
     *            the position of the header field which has to be returned.
     * @return the header field name at the given position.
     */
    virtual CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 posn,
        /* [out] */ String* key);

    /**
     * Returns the point of time since when the data must be modified to be
     * transmitted. Some protocols transmit data only if it has been modified
     * more recently than a particular time.
     *
     * @return the time in milliseconds since January 1, 1970 GMT.
     * @see #ifModifiedSince
     */
    virtual CARAPI GetIfModifiedSince(
        /* [out] */ Int64* time);

    /**
     * Returns an {@code InputStream} for reading data from the resource pointed by
     * this {@code URLConnection}. It throws an UnknownServiceException by
     * default. This method must be overridden by its subclasses.
     *
     * @return the InputStream to read data from.
     * @throws IOException
     *             if no InputStream could be created.
     */
    virtual CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    /**
     * Returns the value of the response header field {@code last-modified} or
     * {@code 0} if this value is not set.
     *
     * @return the value of the {@code last-modified} header field.
     */
    virtual CARAPI GetLastModified(
        /* [out] */ Int64* value);

    /**
     * Returns an {@code OutputStream} for writing data to this {@code
     * URLConnection}. It throws an {@code UnknownServiceException} by default.
     * This method must be overridden by its subclasses.
     *
     * @return the OutputStream to write data.
     * @throws IOException
     *             if no OutputStream could be created.
     */
    virtual CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    /**
     * Returns a {@code Permission} object representing all needed permissions to
     * open this connection. The returned permission object depends on the state
     * of the connection and will be {@code null} if no permissions are
     * necessary. By default, this method returns {@code AllPermission}.
     * Subclasses should overwrite this method to return an appropriate
     * permission object.
     *
     * @return the permission object representing the needed permissions to open
     *         this connection.
     * @throws IOException
     *             if an I/O error occurs while creating the permission object.
     */
//        public java.security.Permission getPermission();

    /**
     * Returns the value of the request header property specified by {code field}
     * or {@code null} if there is no field with this name. The base
     * implementation of this method returns always {@code null}.
     *
     * @param field
     *            the name of the request header property.
     * @return the value of the property.
     * @throws IllegalStateException
     *             if the connection has been already established.
     */
    virtual CARAPI GetRequestProperty(
        /* [in] */ const String& field,
        /* [out] */ String* property);

    /**
     * Returns the URL represented by this {@code URLConnection}.
     *
     * @return the URL of this connection.
     */
    virtual CARAPI GetURL(
        /* [out] */ IURL** url);

    /**
     * Returns the value of the flag which specifies whether this {@code
     * URLConnection} allows to use caches.
     *
     * @return {@code true} if using caches is allowed, {@code false} otherwise.
     */
    virtual CARAPI GetUseCaches(
        /* [out] */ Boolean* allowed);

    /**
     * Determines the MIME-type of the given resource {@code url} by resolving
     * the filename extension with the internal FileNameMap. Any fragment
     * identifier is removed before processing.
     *
     * @param url
     *            the URL with the filename to get the MIME type.
     * @return the guessed content type or {@code null} if the type could not be
     *         determined.
     */
    static CARAPI_(String) GuessContentTypeFromName(
        /* [in] */ const String& url);

    /**
     * Determines the MIME-type of the resource represented by the input stream
     * {@code is} by reading its first few characters.
     *
     * @param is
     *            the resource representing input stream to determine the
     *            content type.
     * @return the guessed content type or {@code null} if the type could not be
     *         determined.
     * @throws IOException
     *             if an I/O error occurs while reading from the input stream.
     */
    //@SuppressWarnings("nls")
    static CARAPI_(String) GuessContentTypeFromStream(
        /* [in] */ IInputStream* is);

    /**
     * Sets the flag indicating whether this connection allows user interaction
     * or not. This method can only be called prior to the connection
     * establishment.
     *
     * @param newValue
     *            the value of the flag to be set.
     * @throws IllegalStateException
     *             if this method attempts to change the flag after the
     *             connection has been established.
     * @see #allowUserInteraction
     */
    virtual CARAPI SetAllowUserInteraction(
        /* [in] */ Boolean newValue);

    /**
     * Sets the internally used content handler factory. The content factory can
     * only be set if it is allowed by the security manager and only once during
     * the lifetime of the application.
     *
     * @param contentFactory
     *            the content factory to be set.
     * @throws Error
     *             if the security manager does not allow to set the content
     *             factory or it has been already set earlier ago.
     */
    static CARAPI SetContentHandlerFactory(
        /* [in] */ IContentHandlerFactory* contentFactory);

    /**
     * Sets the default value for the flag indicating whether this connection
     * allows user interaction or not. Existing {@code URLConnection}s are
     * unaffected.
     *
     * @param allows
     *            the default value of the flag to be used for new connections.
     * @see #defaultAllowUserInteraction
     * @see #allowUserInteraction
     */
    static CARAPI_(void) SetDefaultAllowUserInteraction(
        /* [in] */ Boolean allows);

    /**
     * Sets the default value of the specified request header field. This value
     * will be used for the specific field of every newly created connection.
     * The base implementation of this method does nothing.
     *
     * @param field
     *            the request header field to be set.
     * @param value
     *            the default value to be used.
     * @deprecated Use {@link #setRequestProperty} of an existing {@code
     *             URLConnection} instance.
     */
    //@Deprecated
    static CARAPI_(void) SetDefaultRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& value);

    /**
     * Sets the default value for the flag indicating whether this connection
     * allows to use caches. Existing {@code URLConnection}s are unaffected.
     *
     * @param newValue
     *            the default value of the flag to be used for new connections.
     * @see #defaultUseCaches
     * @see #useCaches
     */
    virtual CARAPI SetDefaultUseCaches(
        /* [in] */ Boolean newValue);

    /**
     * Sets the flag indicating whether this {@code URLConnection} allows input.
     * It cannot be set after the connection is established.
     *
     * @param newValue
     *            the new value for the flag to be set.
     * @throws IllegalAccessError
     *             if this method attempts to change the value after the
     *             connection has been already established.
     * @see #doInput
     */
    virtual CARAPI SetDoInput(
        /* [in] */ Boolean newValue);

    /**
     * Sets the flag indicating whether this {@code URLConnection} allows
     * output. It cannot be set after the connection is established.
     *
     * @param newValue
     *            the new value for the flag to be set.
     * @throws IllegalAccessError
     *             if this method attempts to change the value after the
     *             connection has been already established.
     * @see #doOutput
     */
    virtual CARAPI SetDoOutput(
        /* [in] */ Boolean newValue);

    /**
     * Sets the internal map which is used by all {@code URLConnection}
     * instances to determine the MIME-type according to a filename extension.
     *
     * @param map
     *            the MIME table to be set.
     */
    static CARAPI_(void) SetFileNameMap(
        /* [in] */ IFileNameMap* map);

    /**
     * Sets the point of time since when the data must be modified to be
     * transmitted. Some protocols transmit data only if it has been modified
     * more recently than a particular time. The data will be transmitted
     * regardless of its timestamp if this option is set to {@code 0}.
     *
     * @param newValue
     *            the time in milliseconds since January 1, 1970 GMT.
     * @throws IllegalStateException
     *             if this {@code URLConnection} has already been connected.
     * @see #ifModifiedSince
     */
    virtual CARAPI SetIfModifiedSince(
        /* [in] */ Int64 newValue);

    /**
     * Sets the value of the specified request header field. The value will only
     * be used by the current {@code URLConnection} instance. This method can
     * only be called before the connection is established.
     *
     * @param field
     *            the request header field to be set.
     * @param newValue
     *            the new value of the specified property.
     * @throws IllegalStateException
     *             if the connection has been already established.
     * @throws NullPointerException
     *             if the parameter {@code field} is {@code null}.
     */
    virtual CARAPI SetRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    /**
     * Sets the flag indicating whether this connection allows to use caches or
     * not. This method can only be called prior to the connection
     * establishment.
     *
     * @param newValue
     *            the value of the flag to be set.
     * @throws IllegalStateException
     *             if this method attempts to change the flag after the
     *             connection has been established.
     * @see #useCaches
     */
    virtual CARAPI SetUseCaches(
        /* [in] */ Boolean newValue);

    /**
     * Sets the timeout value in milliseconds for establishing the connection to
     * the resource pointed by this {@code URLConnection} instance. A {@code
     * SocketTimeoutException} is thrown if the connection could not be
     * established in this time. Default is {@code 0} which stands for an
     * infinite timeout.
     *
     * @param timeout
     *            the connecting timeout in milliseconds.
     * @throws IllegalArgumentException
     *             if the parameter {@code timeout} is less than zero.
     */
    virtual CARAPI SetConnectTimeout(
        /* [in] */ Int32 timeout);

    /**
     * Returns the configured connecting timeout.
     *
     * @return the connecting timeout value in milliseconds.
     */
    virtual CARAPI GetConnectTimeout(
        /* [out] */ Int32* timeout);

    /**
     * Sets the timeout value in milliseconds for reading from the input stream
     * of an established connection to the resource. A {@code
     * SocketTimeoutException} is thrown if the connection could not be
     * established in this time. Default is {@code 0} which stands for an
     * infinite timeout.
     *
     * @param timeout
     *            the reading timeout in milliseconds.
     * @throws IllegalArgumentException
     *             if the parameter {@code timeout} is less than zero.
     */
    virtual CARAPI SetReadTimeout(
        /* [in] */ Int32 timeout);

    /**
     * Returns the configured timeout for reading from the input stream of an
     * established connection to the resource.
     *
     * @return the reading timeout value in milliseconds.
     */
    virtual CARAPI GetReadTimeout(
        /* [out] */ Int32* timeout);

protected:
    URLConnection();

    virtual ~URLConnection();

    /**
     * Creates a new {@code URLConnection} instance pointing to the resource
     * specified by the given URL.
     *
     * @param url
     *            the URL which represents the resource this {@code
     *            URLConnection} will point to.
     */
    CARAPI Init(
        /* [in] */ IURL* url);

    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

private:
    /**
     * Returns the specific ContentHandler that will handle the type {@code
     * contentType}.
     *
     * @param type
     *            The type that needs to be handled
     * @return An instance of the Content Handler
     */
    CARAPI GetContentHandler(
        /* [in] */ const String& type,
        /* [out] */ IContentHandler** contentHandler);

    CARAPI CheckNotConnected();

    /**
     * Performs any necessary string parsing on the input string such as
     * converting non-alphanumeric character into underscore.
     *
     * @param typeString
     *            the parsed string
     * @return the string to be parsed
     */
    CARAPI_(String) ParseTypeString(
        /* [in] */ const String& typeString);

public:
    AutoPtr<IContentHandler> mDefaultHandler;

    /**
     * Cache for storing content handler
     */
    // static Hashtable<String, Object> contentHandlers = new Hashtable<String, Object>();
    static HashMap<String, AutoPtr<IContentHandler> > sContentHandlers;

protected:
    /**
     * The URL which represents the remote target of this {@code URLConnection}.
     */
    AutoPtr<IURL> mUrl;

    /**
     * The data must be modified more recently than this time in milliseconds
     * since January 1, 1970, GMT to be transmitted.
     */
    Int64 mIfModifiedSince;

    /**
     * Specifies whether the using of caches is enabled or the data has to be
     * recent for every request.
     */
    Boolean mUseCaches;// = defaultUseCaches;

    /**
     * Specifies whether this {@code URLConnection} is already connected to the
     * remote resource. If this field is set to {@code true} the flags for
     * setting up the connection are not changeable anymore.
     */
    Boolean mConnected;

    /**
     * Specifies whether this {@code URLConnection} allows sending data.
     */
    Boolean mDoOutput;

    /**
     * Specifies whether this {@code URLConnection} allows receiving data.
     */
    Boolean mDoInput;

    /**
     * Specifies whether this {@code URLConnection} allows user interaction as
     * it is needed for authentication purposes.
     */
    Boolean mAllowUserInteraction;

private:
    String mContentType;

    static Boolean sDefaultAllowUserInteraction;

    static Boolean sDefaultUseCaches;

    Int64 mLastModified;

    static AutoPtr<IContentHandlerFactory> sContentHandlerFactory;

    Int32 mReadTimeout;

    Int32 mConnectTimeout;

    /**
     * A hashtable that maps the filename extension (key) to a MIME-type
     * (element)
     */
    static AutoPtr<IFileNameMap> sFileNameMap;

    static Mutex sLock;
};

#endif //__URLCONNECTION_H__
