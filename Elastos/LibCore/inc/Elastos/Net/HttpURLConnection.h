
#ifndef __HTTPURLCONNECTION_H__
#define __HTTPURLCONNECTION_H__

#include "Elastos.Net_server.h"
#include "URLConnection.h"

using namespace Elastos;

extern "C" const InterfaceID EIID_HttpURLConnection;

class HttpURLConnection : public URLConnection
{
public:
    virtual CARAPI Disconnect() = 0;

    virtual CARAPI GetErrorStream(
        /* [out] */ IInputStream** stream);

    /**
     * Returns the value of {@code followRedirects} which indicates if this
     * connection follows a different URL redirected by the server. It is
     * enabled by default.
     *
     * @return the value of the flag.
     * @see #setFollowRedirects
     */
    static CARAPI_(Boolean) GetFollowRedirects();

//        public java.security.Permission getPermission();

    virtual CARAPI GetRequestMethod(
        /* [out] */ String* method);

    virtual CARAPI GetResponseCode(
        /* [out] */ Int32* responseCode);

    virtual CARAPI GetResponseMessage(
        /* [out] */ String* message);

    /**
     * Sets the flag of whether this connection will follow redirects returned
     * by the remote server. This method can only be called with the permission
     * from the security manager.
     *
     * @param auto
     *            the value to enable or disable this option.
     * @see SecurityManager#checkSetFactory()
     */
    static CARAPI_(void) SetFollowRedirects(
        /* [in] */ Boolean followRedirects);

    virtual CARAPI SetRequestMethod(
        /* [in] */ String method);

    virtual CARAPI UsingProxy(
        /* [out] */ Boolean* usingProxy) = 0;

    CARAPI GetContentEncoding(
        /* [out] */ String* encoding);

    virtual CARAPI GetInstanceFollowRedirects(
        /* [out] */ Boolean* followRedirects);

    virtual CARAPI SetInstanceFollowRedirects(
        /* [in] */ Boolean followRedirects);

    /**
     * Returns the date value in milliseconds since {@code 01.01.1970, 00:00h}
     * corresponding to the header field {@code field}. The {@code defaultValue}
     * will be returned if no such field can be found in the response header.
     *
     * @param field
     *            the header field name.
     * @param defaultValue
     *            the default value to use if the specified header field wont be
     *            found.
     * @return the header field represented in milliseconds since January 1,
     *         1970 GMT.
     */
    //@Override
    CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    virtual CARAPI SetFixedLengthStreamingMode(
        /* [in] */ Int32 contentLength);

    virtual CARAPI SetChunkedStreamingMode(
        /* [in] */ Int32 chunkLength);

protected:
    HttpURLConnection();

    /**
     * Constructs a new {@code HttpURLConnection} instance pointing to the
     * resource specified by the {@code url}.
     *
     * @param url
     *            the URL of this connection.
     * @see URL
     * @see URLConnection
     */
    CARAPI_(void) Init(
        /* [in] */ IURL* url);

    virtual ~HttpURLConnection();

protected:
    /**
     * The HTTP request method of this {@code HttpURLConnection}. The default
     * value is {@code "GET"}.
     */
    String mMethod;

    /**
     * The status code of the response obtained from the HTTP request. The
     * default value is {@code -1}.
     * <p>
     * <li>1xx: Informational</li>
     * <li>2xx: Success</li>
     * <li>3xx: Relocation/Redirection</li>
     * <li>4xx: Client Error</li>
     * <li>5xx: Server Error</li>
     */
    Int32 mResponseCode;

    /**
     * The HTTP response message which corresponds to the response code.
     */
    String mResponseMessage;

    /**
     * Flag to define whether the protocol will automatically follow redirects
     * or not. The default value is {@code true}.
     */
    Boolean mInstanceFollowRedirects;

    /**
     * If the HTTP chunked encoding is enabled this parameter defines the
     * chunk-length. Default value is {@code -1} that means the chunked encoding
     * mode is disabled.
     */
    Int32 mChunkLength;

    /**
     * If using HTTP fixed-length streaming mode this parameter defines the
     * fixed length of content. Default value is {@code -1} that means the
     * fixed-length streaming mode is disabled.
     */
    Int32 mFixedContentLength;

private:
    static Boolean mFollowRedirects;
};

#endif //__HTTPURLCONNECTION_H__
