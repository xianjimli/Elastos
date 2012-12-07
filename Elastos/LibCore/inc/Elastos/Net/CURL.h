
#ifndef __CURL_H__
#define __CURL_H__

#include "_CURL.h"
#include "InetAddress.h"
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class URLStreamHandler;

CarClass(CURL)
{
friend class URLStreamHandler;

public:
    CURL();

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

    CARAPI SameFile(
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

    CARAPI GetContent(
        /* [out] */ IInterface** content);

//        public final Object getContent(Class[] types)

    CARAPI OpenStream(
        /* [out] */ IInputStream** is);

    CARAPI OpenConnection(
        /* [out] */ IURLConnection** connection);

    CARAPI OpenConnectionEx(
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** connection);

    CARAPI ToURI(
        /* [out] */ IURI** uri);

    CARAPI ToExternalForm(
        /* [out] */ String* url);

    CARAPI GetFile(
        /* [out] */ String* fileName);

    CARAPI GetHost(
        /* [out] */ String* hostName);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetEffectivePort(
        /* [out] */ Int32* port);

    CARAPI GetProtocol(
        /* [out] */ String* protocol);

    CARAPI GetRef(
        /* [out] */ String* ref);

    CARAPI GetQuery(
        /* [out] */ String* query);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetDefaultPort(
        /* [out] */ Int32* port);

    CARAPI constructor(
        /* [in] */ const String& spec);

    CARAPI constructor(
        /* [in] */ IURL* context,
        /* [in] */ const String& spec);

    CARAPI constructor(
        /* [in] */ IURL* context,
        /* [in] */ const String& spec,
        /* [in] */ IURLStreamHandler* handler);

    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ const String& file);

    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file);

    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ IURLStreamHandler* handler);

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
    CARAPI_(void) SetEx(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& authority,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& ref);

public:
    /**
     * The URL Stream (protocol) Handler
     */
    AutoPtr<IURLStreamHandler> mStrmHandler;

private:
    static const Int64 mSerialVersionUID;

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
    String mProtocol;// = null;

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
    Int32 mPort;// = -1;

    /**
     * The receiver's authority.
     *
     * @serial the authority of this URL
     *
     */
    String mAuthority;// = null;

    /**
     * The receiver's userInfo.
     */
    String mUserInfo;// = null;

    /**
     * The receiver's path.
     */
    String mPath;// = null;

    /**
     * The receiver's query.
     */
    String mQuery;// = null;

    /**
     * The receiver's reference.
     *
     * @serial the reference of this URL
     *
     */
    String mRef;// = null;

    /**
     * Cache for storing protocol handler
     */
    static HashMap<String, AutoPtr<IURLStreamHandler> > mStreamHandlers;

    /**
     * The factory responsible for producing URL Stream (protocol) Handler
     */
    static AutoPtr<IURLStreamHandlerFactory> mStreamHandlerFactory;

    static Mutex* mLock;
};

#endif //__CURL_H__
