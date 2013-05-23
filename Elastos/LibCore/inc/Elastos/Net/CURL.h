
#ifndef __CURL_H__
#define __CURL_H__

#include "cmdef.h"
#include "_CURL.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class URLStreamHandler;

/**
 * A URL instance specifies the location of a resource on the internet as
 * specified by RFC 1738. Such a resource can be a simple file or a service
 * which generates the output dynamically. A URL is divided in its parts
 * protocol, host name, port, path, file, user-info, query, reference and
 * authority. However, not each of this parts has to be defined.
 */
CarClass(CURL)
{
    friend class URLStreamHandler;

public:
    CURL();

    /**
     * Creates a new URL instance by parsing the string {@code spec}.
     *
     * @param spec
     *            the URL string representation which has to be parsed.
     * @throws MalformedURLException
     *             if the given string {@code spec} could not be parsed as a
     *             URL.
     */
    CARAPI constructor(
        /* [in] */ const String& spec);

    /**
     * Creates a new URL to the specified resource {@code spec}. This URL is
     * relative to the given {@code context}. If the protocol of the parsed URL
     * does not match with the protocol of the context URL, then the newly
     * created URL is absolute and bases only on the given URL represented by
     * {@code spec}. Otherwise the protocol is defined by the context URL.
     *
     * @param context
     *            the URL which is used as the context.
     * @param spec
     *            the URL string representation which has to be parsed.
     * @throws MalformedURLException
     *             if the given string {@code spec} could not be parsed as a URL
     *             or an invalid protocol has been found.
     */
    CARAPI constructor(
        /* [in] */ IURL* context,
        /* [in] */ const String& spec);

    /**
     * Creates a new URL to the specified resource {@code spec}. This URL is
     * relative to the given {@code context}. The {@code handler} will be used
     * to parse the URL string representation. If this argument is {@code null}
     * the default {@code URLStreamHandler} will be used. If the protocol of the
     * parsed URL does not match with the protocol of the context URL, then the
     * newly created URL is absolute and bases only on the given URL represented
     * by {@code spec}. Otherwise the protocol is defined by the context URL.
     *
     * @param context
     *            the URL which is used as the context.
     * @param spec
     *            the URL string representation which has to be parsed.
     * @param handler
     *            the specific stream handler to be used by this URL.
     * @throws MalformedURLException
     *             if the given string {@code spec} could not be parsed as a URL
     *             or an invalid protocol has been found.
     */
    CARAPI constructor(
        /* [in] */ IURL* context,
        /* [in] */ const String& spec,
        /* [in] */ IURLStreamHandler* handler);

    /**
     * Creates a new URL instance using the given arguments. The URL uses the
     * default port for the specified protocol.
     *
     * @param protocol
     *            the protocol of the new URL.
     * @param host
     *            the host name or IP address of the new URL.
     * @param file
     *            the name of the resource.
     * @throws MalformedURLException
     *             if the combination of all arguments do not represent a valid
     *             URL or the protocol is invalid.
     */
    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ const String& file);

    /**
     * Creates a new URL instance using the given arguments. The URL uses the
     * specified port instead of the default port for the given protocol.
     *
     * @param protocol
     *            the protocol of the new URL.
     * @param host
     *            the host name or IP address of the new URL.
     * @param port
     *            the specific port number of the URL. {@code -1} represents the
     *            default port of the protocol.
     * @param file
     *            the name of the resource.
     * @throws MalformedURLException
     *             if the combination of all arguments do not represent a valid
     *             URL or the protocol is invalid.
     */
    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file);

    /**
     * Creates a new URL instance using the given arguments. The URL uses the
     * specified port instead of the default port for the given protocol.
     *
     * @param protocol
     *            the protocol of the new URL.
     * @param host
     *            the host name or IP address of the new URL.
     * @param port
     *            the specific port number of the URL. {@code -1} represents the
     *            default port of the protocol.
     * @param file
     *            the name of the resource.
     * @param handler
     *            the stream handler to be used by this URL.
     * @throws MalformedURLException
     *             if the combination of all arguments do not represent a valid
     *             URL or the protocol is invalid.
     * @throws SecurityException
     *             if {@code handler} is non-{@code null}, and a security
     *             manager is installed that disallows user-defined protocol
     *             handlers.
     */
    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ IURLStreamHandler* handler);

    /**
     * Sets the {@code URLStreamHandlerFactory} which creates protocol specific
     * stream handlers. This method can be invoked only once during an
     * application's lifetime. If the {@code URLStreamHandlerFactory} is already
     * set an {@link Error} will be thrown.
     * <p>
     * A security check is performed to verify whether the current policy allows
     * to set the stream handler factory.
     *
     * @param streamFactory
     *            the factory to be used for creating stream protocol handlers.
     */
    static CARAPI SetURLStreamHandlerFactory(
        /* [in] */ IURLStreamHandlerFactory* streamFactory);

    CARAPI_(void) FixURL(
        /* [in] */ Boolean fixHost);

    /**
     * Returns whether this URL refers to the same resource as the given
     * argument {@code otherURL}. All URL components except the reference field
     * are compared.
     *
     * @param otherURL
     *            the URL to compare against.
     * @return {@code true} if both instances refer to the same resource,
     *         {@code false} otherwise.
     */
    CARAPI IsSameFile(
        /* [in] */ IURL* otherURL,
        /* [out] */ Boolean* isSame);

    /**
     * Gets the hashcode value of this URL instance.
     *
     * @return the appropriate hashcode value.
     */
    //@Override
    CARAPI_(Int32) HashCode();

    /**
     * Sets the receiver's stream handler to one which is appropriate for its
     * protocol. Throws a MalformedURLException if no reasonable handler is
     * available.
     * <p>
     * Note that this will overwrite any existing stream handler with the new
     * one. Senders must check if the strmHandler is null before calling the
     * method if they do not want this behavior (a speed optimization).
     */
    CARAPI_(void) SetupStreamHandler();

    /**
     * Gets the content of the resource which is referred by this URL. By
     * default one of the following object types will be returned:
     * <p>
     * <li>Image for pictures</li>
     * <li>AudioClip for audio sequences</li>
     * <li>{@link InputStream} for all other data</li>
     *
     * @return the content of the referred resource.
     * @throws IOException
     *             if an error occurs obtaining the content.
     */
    CARAPI GetContent(
        /* [out] */ IInterface** content);

    /**
     * Gets the content of the resource which is referred by this URL. The
     * argument {@code types} is an array of allowed or expected object types.
     * {@code null} will be returned if the obtained object type does not match
     * with one from this list. Otherwise the first type that matches will be
     * used.
     *
     * @param types
     *            the list of allowed or expected object types.
     * @return the object representing the resource referred by this URL,
     *         {@code null} if the content does not match to a specified content
     *         type.
     * @throws IOException
     *             if an error occurs obtaining the content.
     */
    // Param not generic in spec
//        public final Object getContent(Class[] types)

    /**
     * Opens an InputStream to read the resource referred by this URL.
     *
     * @return the stream which allows to read the resource.
     * @throws IOException
     *             if an error occurs while opening the InputStream.
     */
    CARAPI OpenStream(
        /* [out] */ IInputStream** is);

    /**
     * Opens a connection to the remote resource specified by this URL. This
     * connection allows bidirectional data transfer.
     *
     * @return the connection to this URL.
     * @throws IOException
     *             if an error occurs while opening the connection.
     */
    CARAPI OpenConnection(
        /* [out] */ IURLConnection** connection);

    /**
     * Opens a connection to the remote resource specified by this URL. The
     * connection will be established through the given proxy and allows
     * bidirectional data transfer.
     *
     * @param proxy
     *            the proxy through which the connection will be established.
     * @return the appropriate URLconnection instance representing the
     *         connection to this URL.
     * @throws IOException
     *             if an I/O error occurs while opening the connection.
     * @throws SecurityException
     *             if a security manager is installed and it denies to connect
     *             to the proxy.
     * @throws IllegalArgumentException
     *             if the argument proxy is {@code null} or is an invalid type.
     * @throws UnsupportedOperationException
     *             if the protocol handler does not support opening connections
     *             through proxies.
     */
    CARAPI OpenConnectionEx(
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** connection);

    /**
     * Converts this URL instance into an equivalent URI object.
     *
     * @return the URI instance that represents this URL.
     * @throws URISyntaxException
     *             if this URL cannot be converted into a URI.
     */
    CARAPI ToURI(
        /* [out] */ IURI** uri);

    /**
     * Returns a string containing a concise, human-readable representation of
     * this URL.
     *
     * @return the string representation of this URL.
     */
    CARAPI ToExternalForm(
        /* [out] */ String* url);

    /**
     * Gets the value of the file part of this URL.
     *
     * @return the file name this URL refers to or an empty string if the file
     *         part is not set.
     */
    CARAPI GetFile(
        /* [out] */ String* fileName);

    /**
     * Gets the value of the host part of this URL.
     *
     * @return the host name or IP address of this URL.
     */
    CARAPI GetHost(
        /* [out] */ String* hostName);

    /**
     * Gets the port number of this URL or {@code -1} if the port is not set.
     *
     * @return the port number of this URL.
     */
    CARAPI GetPort(
        /* [out] */ Int32* port);

    /** @hide */
    CARAPI GetEffectivePort(
        /* [out] */ Int32* port);

    /**
     * Gets the protocol of this URL.
     *
     * @return the protocol type of this URL.
     */
    CARAPI GetProtocol(
        /* [out] */ String* protocol);

    /**
     * Gets the value of the reference part of this URL.
     *
     * @return the reference part of this URL.
     */
    CARAPI GetRef(
        /* [out] */ String* ref);

    /**
     * Gets the value of the query part of this URL.
     *
     * @return the query part of this URL.
     */
    CARAPI GetQuery(
        /* [out] */ String* query);

    /**
     * Gets the value of the path part of this URL.
     *
     * @return the path part of this URL.
     */
    CARAPI GetPath(
        /* [out] */ String* path);

    /**
     * Gets the value of the user-info part of this URL.
     *
     * @return the user-info part of this URL.
     */
    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    /**
     * Gets the value of the authority part of this URL.
     *
     * @return the authority part of this URL.
     */
    CARAPI GetAuthority(
        /* [out] */ String* authority);

    /**
     * Gets the default port number of the protocol used by this URL. If no
     * default port is defined by the protocol or the {@code URLStreamHandler},
     * {@code -1} will be returned.
     *
     * @return the default port number according to the protocol of this URL.
     * @see URLStreamHandler#getDefaultPort
     */
    CARAPI GetDefaultPort(
        /* [out] */ Int32* port);

    CARAPI ToString(
        /* [out] */ String* str);

protected:
    /**
     * Sets the properties of this URL using the provided arguments. Only a
     * {@code URLStreamHandler} can use this method to set fields of the
     * existing URL instance. A URL is generally constant.
     *
     * @param protocol
     *            the protocol to be set.
     * @param host
     *            the host name to be set.
     * @param port
     *            the port number to be set.
     * @param file
     *            the file to be set.
     * @param ref
     *            the reference to be set.
     */
    CARAPI_(void) Set(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ const String& ref);

    /**
     * Sets the properties of this URL using the provided arguments. Only a
     * {@code URLStreamHandler} can use this method to set fields of the
     * existing URL instance. A URL is generally constant.
     *
     * @param protocol
     *            the protocol to be set.
     * @param host
     *            the host name to be set.
     * @param port
     *            the port number to be set.
     * @param authority
     *            the authority to be set.
     * @param userInfo
     *            the user-info to be set.
     * @param path
     *            the path to be set.
     * @param query
     *            the query to be set.
     * @param ref
     *            the reference to be set.
     */
    CARAPI_(void) Set(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& authority,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& ref);

private:
    /**
     * This method is called to restore the state of a URL object that has been
     * serialized. The stream handler is determined from the URL's protocol.
     *
     * @param stream
     *            the stream to read from.
     *
     * @throws IOException
     *             if an IO Exception occurs while reading the stream or the
     *             handler can not be found.
     */
    // private void readObject(java.io.ObjectInputStream stream)

    /**
     * This method is called to write any non-transient, non-static variables
     * into the output stream.
     * <p>
     * Note that, we really only need the readObject method but the spec that
     * says readObject will be ignored if no writeObject is present.
     *
     * @param s
     *            the stream to write on.
     * @throws IOException
     *             if an IO Exception occurs during the write.
     */
    // private void writeObject(ObjectOutputStream s)

public:
    /**
     * The URL Stream (protocol) Handler
     */
    AutoPtr<IURLStreamHandler> mStrmHandler;

private:
//    static NetPermission specifyStreamHandlerPermission = new NetPermission(
//            "specifyStreamHandler");

    Int32 mHashCode;

    /**
     * The receiver's filename.
     *
     * @serial the file of this URL
     *
     */
    String mFile;

    /**
     * The receiver's protocol identifier.
     *
     * @serial the protocol of this URL (http, file)
     *
     */
    String mProtocol;

    /**
     * The receiver's host name.
     *
     * @serial the host of this URL
     *
     */
    String mHost;

    /**
     * The receiver's port number.
     *
     * @serial the port of this URL
     *
     */
    Int32 mPort;

    /**
     * The receiver's authority.
     *
     * @serial the authority of this URL
     *
     */
    String mAuthority;

    /**
     * The receiver's userInfo.
     */
    String mUserInfo;

    /**
     * The receiver's path.
     */
    String mPath;

    /**
     * The receiver's query.
     */
    String mQuery;

    /**
     * The receiver's reference.
     *
     * @serial the reference of this URL
     *
     */
    String mRef;

    /**
     * Cache for storing protocol handler
     */
    static HashMap<String, AutoPtr<IURLStreamHandler> > sStreamHandlers;

    /**
     * The factory responsible for producing URL Stream (protocol) Handler
     */
    static AutoPtr<IURLStreamHandlerFactory> sStreamHandlerFactory;

    static Mutex sLock;
};

#endif //__CURL_H__
