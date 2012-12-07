
#ifndef __URLCONNECTION_H__
#define __URLCONNECTION_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>

using namespace Elastos;

extern "C" const InterfaceID EIID_URLConnection;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IContentHandler> >
{
    size_t operator()(AutoPtr<IContentHandler> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

class URLConnection
{
public:
    class DefaultContentHandler: public ElRefBase, public IContentHandler
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
            /* [in] */ const ArrayOf<IInterface*>& types,
            /* [out] */ IInterface** obj);
    };

public:
    virtual CARAPI Connect() = 0;

    virtual CARAPI GetAllowUserInteraction(
        /* [out] */ Boolean* value);

    virtual CARAPI GetContent(
        /* [out] */ IInterface** content);

//        public Object getContent(Class[] types);

    virtual CARAPI GetContentEncoding(
        /* [out] */ String* encoding);

    virtual CARAPI GetContentLength(
        /* [out] */ Int32* length);

    virtual CARAPI GetContentType(
        /* [out] */ String* type);

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

    virtual CARAPI GetDefaultUseCaches(
        /* [out] */ Boolean* value);

    virtual CARAPI GetDoInput(
        /* [out] */ Boolean* allowed);

    virtual CARAPI GetDoOutput(
        /* [out] */ Boolean* allowed);

    virtual CARAPI GetExpiration(
        /* [out] */ Int64* expiration);

    /**
     * Returns the table which is used by all {@code URLConnection} instances to
     * determine the MIME-type according to a file extension.
     *
     * @return the file name map to determine the MIME-type.
     */
    static CARAPI_(AutoPtr<IFileNameMap>) GetFileNameMap();

    virtual CARAPI GetHeaderFieldByPosition(
        /* [in] */ Int32 pos,
        /* [out] */ String* value);

//    public Map<String, List<String>> getHeaderFields()

//    public Map<String, List<String>> getRequestProperties()

    virtual CARAPI AddRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    virtual CARAPI GetHeaderFieldByKey(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    virtual CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    virtual CARAPI GetHeaderFieldInt32(
        /* [in] */ const String& field,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    virtual CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 posn,
        /* [out] */ String* key);

    virtual CARAPI GetIfModifiedSince(
        /* [out] */ Int64* time);

    virtual CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    virtual CARAPI GetLastModified(
        /* [out] */ Int64* value);

    virtual CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

//        public java.security.Permission getPermission();

    virtual CARAPI GetRequestProperty(
        /* [in] */ const String& field,
        /* [out] */ String* property);

    virtual CARAPI GetURL(
        /* [out] */ IURL** url);

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

    virtual CARAPI SetDefaultUseCaches(
        /* [in] */ Boolean newValue);

    virtual CARAPI SetDoInput(
        /* [in] */ Boolean newValue);

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

    virtual CARAPI SetIfModifiedSince(
        /* [in] */ Int64 newValue);

    virtual CARAPI SetRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    virtual CARAPI SetUseCaches(
        /* [in] */ Boolean newValue);

    virtual CARAPI SetConnectTimeout(
        /* [in] */ Int32 timeout);

    virtual CARAPI GetConnectTimeout(
        /* [out] */ Int32* timeout);

    virtual CARAPI SetReadTimeout(
        /* [in] */ Int32 timeout);

    virtual CARAPI GetReadTimeout(
        /* [out] */ Int32* timeout);

protected:
    URLConnection();

    /**
     * Creates a new {@code URLConnection} instance pointing to the resource
     * specified by the given URL.
     *
     * @param url
     *            the URL which represents the resource this {@code
     *            URLConnection} will point to.
     */
    CARAPI_(void) Init(
        /* [in] */ IURL* url);

    virtual ~URLConnection();

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
    static HashMap<String, AutoPtr<IContentHandler> > mContentHandlers;

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

    static Boolean mDefaultAllowUserInteraction;

    static Boolean mDefaultUseCaches;

    Int64 mLastModified;

    static AutoPtr<IContentHandlerFactory> mContentHandlerFactory;

    Int32 mReadTimeout;

    Int32 mConnectTimeout;

    /**
     * A hashtable that maps the filename extension (key) to a MIME-type
     * (element)
     */
    static AutoPtr<IFileNameMap> mFileNameMap;
};

#endif //__URLCONNECTION_H__
