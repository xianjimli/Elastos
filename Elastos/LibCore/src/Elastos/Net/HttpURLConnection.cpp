
#include "cmdef.h"
#include "HttpURLConnection.h"

extern "C" const InterfaceID EIID_HttpURLConnection =
    { 0xfd8d9e46, 0x48f9, 0x4437, { 0x92, 0x3f, 0x6, 0xb1, 0xb3, 0x50, 0x87, 0xac } };


Boolean HttpURLConnection::mFollowRedirects = TRUE;

HttpURLConnection::HttpURLConnection()
    : URLConnection()
    , mMethod("GET")/*HttpURLConnectionImpl.GET*/
    , mResponseCode(-1)
    , mInstanceFollowRedirects(mFollowRedirects)
    , mChunkLength(-1)
    , mFixedContentLength(-1)
{
}

void HttpURLConnection::Init(
    /* [in] */ IURL* url)
{
    mUrl = url;
}

HttpURLConnection::~HttpURLConnection()
{}

ECode HttpURLConnection::GetErrorStream(
    /* [out] */ IInputStream** stream)
{
    VALIDATE_NOT_NULL(stream);
    *stream = NULL;

    return NOERROR;
}

Boolean HttpURLConnection::GetFollowRedirects()
{
    return mFollowRedirects;
}

//        public java.security.Permission getPermission();

ECode HttpURLConnection::GetRequestMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(method);
    *method = mMethod;

    return NOERROR;
}

ECode HttpURLConnection::GetResponseCode(
    /* [out] */ Int32* responseCode)
{
    // Call getInputStream() first since getHeaderField() doesn't return
    // exceptions
    VALIDATE_NOT_NULL(responseCode);

    AutoPtr<IInputStream> is;
    GetInputStream((IInputStream**)&is);
    String response;
    GetHeaderFieldByPosition(0, &response);
    if (response.IsNull()) {
        *responseCode = -1;
        return NOERROR;
    }
    response = response.Trim();
    Int32 mark = response.IndexOf(" ") + 1;
    if (mark == 0) {
        *responseCode = -1;
        return NOERROR;
    }
    Int32 last = mark + 3;
    if ((UInt32)last > response.GetLength()) {
        last = response.GetLength();
    }
    mResponseCode = response.Substring(mark, last).ToInt32();
    if ((UInt32)(last + 1) <= response.GetLength()) {
        mResponseMessage = response.Substring(last + 1);
    }
    *responseCode = mResponseCode;

    return NOERROR;
}

ECode HttpURLConnection::GetResponseMessage(
    /* [out] */ String* message)
{
    VALIDATE_NOT_NULL(message);

    if (!mResponseMessage.IsNull()) {
        *message = mResponseMessage;
        return NOERROR;
    }
    Int32 code;
    ECode ec = GetResponseCode(&code);
    if (FAILED(ec)) {
        return ec;
    }
    *message = mResponseMessage;

    return NOERROR;
}

void HttpURLConnection::SetFollowRedirects(
    /* [in] */ Boolean followRedirects)
{
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkSetFactory();
//    }
    mFollowRedirects = followRedirects;
}

ECode HttpURLConnection::SetRequestMethod(
    /* [in] */ String method)
{
    if (mConnected) {
        return E_PROTOCOL_EXCEPTION;
//        throw new ProtocolException("Connection already established");
    }
    String PERMITTED_USER_METHODS[] = {String("OPTIONS"), String("GET"), String("HEAD"),
            String("POST"), String("PUT"), String("DELETE"), String("TRACE")};
    for (Int32 i = 0; i < 7; i++) {
        String permittedUserMethod = PERMITTED_USER_METHODS[i];
        if (permittedUserMethod.Equals(mMethod)) {
            // if there is a supported method that matches the desired
            // method, then set the current method and return
            mMethod = permittedUserMethod;
            return NOERROR;
        }
    }
    // if none matches, then throw ProtocolException
    return E_PROTOCOL_EXCEPTION;
}

ECode HttpURLConnection::GetContentEncoding(
    /* [out] */ String* encoding)
{
    return URLConnection::GetContentEncoding(encoding);
}

ECode HttpURLConnection::GetInstanceFollowRedirects(
    /* [out] */ Boolean* followRedirects)
{
    VALIDATE_NOT_NULL(followRedirects);
    *followRedirects = mInstanceFollowRedirects;

    return NOERROR;
}

ECode HttpURLConnection::SetInstanceFollowRedirects(
    /* [in] */ Boolean followRedirects)
{
    mInstanceFollowRedirects = followRedirects;

    return NOERROR;
}

ECode HttpURLConnection::GetHeaderFieldDate(
    /* [in] */ const String& field,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    return URLConnection::GetHeaderFieldDate(field, defaultValue, value);
}

ECode HttpURLConnection::SetFixedLengthStreamingMode(
    /* [in] */ Int32 contentLength)
{
    if (mConnected) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalStateException("Already connected");
    }
    if (mChunkLength > 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalStateException("Already in chunked mode");
    }
    if (contentLength < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("contentLength < 0");
    }
    mFixedContentLength = contentLength;

    return NOERROR;
}

ECode HttpURLConnection::SetChunkedStreamingMode(
    /* [in] */ Int32 chunkLength)
{
    if (mConnected) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalStateException("Already connected");
    }
    if (mFixedContentLength > 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalStateException("Already in chunked mode");
    }
    if (chunkLength < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("contentLength < 0");
    }
    mChunkLength = chunkLength;

    return NOERROR;
}
