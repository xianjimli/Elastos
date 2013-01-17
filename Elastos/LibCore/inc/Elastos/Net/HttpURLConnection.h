
#ifndef __HTTPURLCONNECTION_H__
#define __HTTPURLCONNECTION_H__

#include "Elastos.Net_server.h"
#include "URLConnection.h"

using namespace Elastos;

//extern "C" const InterfaceID EIID_HttpURLConnection;

class HttpURLConnection : public URLConnection
{
public:
    /**
     * Closes the connection to the HTTP server.
     *
     * @see URLConnection#connect()
     * @see URLConnection#connected
     */
    virtual CARAPI Disconnect() = 0;

    /**
     * Returns an input stream from the server in the case of an error such as
     * the requested file has not been found on the remote server. This stream
     * can be used to read the data the server will send back.
     *
     * @return the error input stream returned by the server.
     */
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

    /**
     * Returns the request method which will be used to make the request to the
     * remote HTTP server. All possible methods of this HTTP implementation is
     * listed in the class definition.
     *
     * @return the request method string.
     * @see #method
     * @see #setRequestMethod
     */
    virtual CARAPI GetRequestMethod(
        /* [out] */ String* method);

    /**
     * Returns the response code returned by the remote HTTP server.
     *
     * @return the response code, -1 if no valid response code.
     * @throws IOException
     *             if there is an IO error during the retrieval.
     * @see #getResponseMessage
     */
    virtual CARAPI GetResponseCode(
        /* [out] */ Int32* responseCode);

    /**
     * Returns the response message returned by the remote HTTP server.
     *
     * @return the response message. {@code null} if no such response exists.
     * @throws IOException
     *             if there is an error during the retrieval.
     * @see #getResponseCode()
     */
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
    static CARAPI SetFollowRedirects(
        /* [in] */ Boolean followRedirects);

    /**
     * Sets the request command which will be sent to the remote HTTP server.
     * This method can only be called before the connection is made.
     *
     * @param method
     *            the string representing the method to be used.
     * @throws ProtocolException
     *             if this is called after connected, or the method is not
     *             supported by this HTTP implementation.
     * @see #getRequestMethod()
     * @see #method
     */
    virtual CARAPI SetRequestMethod(
        /* [in] */ String method);

    /**
     * Returns whether this connection uses a proxy server or not.
     *
     * @return {@code true} if this connection passes a proxy server, false
     *         otherwise.
     */
    virtual CARAPI UsingProxy(
        /* [out] */ Boolean* usingProxy) = 0;

    /**
     * Returns the encoding used to transmit the response body over the network.
     * This is null or "identity" if the content was not encoded, or "gzip" if
     * the body was gzip compressed. Most callers will be more interested in the
     * {@link #getContentType() content type}, which may also include the
     * content's character encoding.
     */
    CARAPI GetContentEncoding(
        /* [out] */ String* encoding);

    /**
     * Returns whether this connection follows redirects.
     *
     * @return {@code true} if this connection follows redirects, false
     *         otherwise.
     */
    virtual CARAPI GetInstanceFollowRedirects(
        /* [out] */ Boolean* followRedirects);

    /**
     * Sets whether this connection follows redirects.
     *
     * @param followRedirects
     *            {@code true} if this connection will follows redirects, false
     *            otherwise.
     */
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

    /**
     * If the length of a HTTP request body is known ahead, sets fixed length to
     * enable streaming without buffering. Sets after connection will cause an
     * exception.
     *
     * @see #setChunkedStreamingMode
     * @param contentLength
     *            the fixed length of the HTTP request body.
     * @throws IllegalStateException
     *             if already connected or an other mode already set.
     * @throws IllegalArgumentException
     *             if {@code contentLength} is less than zero.
     */
    virtual CARAPI SetFixedLengthStreamingMode(
        /* [in] */ Int32 contentLength);

    /**
     * Stream a request body whose length is not known in advance. Old HTTP/1.0
     * only servers may not support this mode.
     *
     * <p>When HTTP chunked encoding is used, the stream is divided into
     * chunks, each prefixed with a header containing the chunk's size. Setting
     * a large chunk length requires a large internal buffer, potentially
     * wasting memory. Setting a small chunk length increases the number of
     * bytes that must be transmitted because of the header on every chunk.
     * Most caller should use {@code 0} to get the system default.
     *
     * @see #setFixedLengthStreamingMode
     * @param chunkLength the length to use, or {@code 0} for the default chunk
     *     length.
     * @throws IllegalStateException if already connected or another mode
     *     already set.
     */
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
    CARAPI Init(
        /* [in] */ IURL* url);

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
    static Boolean sFollowRedirects;
};

#endif //__HTTPURLCONNECTION_H__
