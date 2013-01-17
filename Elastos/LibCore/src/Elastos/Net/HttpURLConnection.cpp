
#include "cmdef.h"
#include "HttpURLConnection.h"


// extern "C" const InterfaceID EIID_HttpURLConnection =
//     { 0xfd8d9e46, 0x48f9, 0x4437, { 0x92, 0x3f, 0x6, 0xb1, 0xb3, 0x50, 0x87, 0xac } };

Boolean HttpURLConnection::sFollowRedirects = TRUE;

HttpURLConnection::HttpURLConnection()
    : URLConnection()
    , mMethod("GET")/*HttpURLConnectionImpl.GET*/
    , mResponseCode(-1)
    , mInstanceFollowRedirects(sFollowRedirects)
    , mChunkLength(-1)
    , mFixedContentLength(-1)
{}

ECode HttpURLConnection::Init(
    /* [in] */ IURL* url)
{
    return URLConnection::Init(url);
}

ECode HttpURLConnection::GetErrorStream(
    /* [out] */ IInputStream** stream)
{
    assert(stream != NULL);
    *stream = NULL;

    return NOERROR;
}

Boolean HttpURLConnection::GetFollowRedirects()
{
    return sFollowRedirects;
}

//        public java.security.Permission getPermission();

ECode HttpURLConnection::GetRequestMethod(
    /* [out] */ String* method)
{
    assert(method != NULL);
    *method = mMethod;

    return NOERROR;
}

ECode HttpURLConnection::GetResponseCode(
    /* [out] */ Int32* responseCode)
{
    assert(responseCode != NULL);

    // Call getInputStream() first since getHeaderField() doesn't return
    // exceptions
    AutoPtr<IInputStream> is;
    FAIL_RETURN(GetInputStream((IInputStream**)&is));
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
    assert(message != NULL);

    if (!mResponseMessage.IsNull()) {
        *message = mResponseMessage;
        return NOERROR;
    }
    Int32 code;
    FAIL_RETURN(GetResponseCode(&code));
    *message = mResponseMessage;
    return NOERROR;
}

ECode HttpURLConnection::SetFollowRedirects(
    /* [in] */ Boolean followRedirects)
{
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkSetFactory();
//    }
    sFollowRedirects = followRedirects;
    return NOERROR;
}

ECode HttpURLConnection::SetRequestMethod(
    /* [in] */ String method)
{
    if (mConnected) {
//        throw new ProtocolException("Connection already established");
        return E_PROTOCOL_EXCEPTION;
    }
    //Todo:
    // for (String permittedUserMethod : HttpURLConnectionImpl.PERMITTED_USER_METHODS)
    CString PERMITTED_USER_METHODS[] = {"OPTIONS", "GET", "HEAD",
            "POST", "PUT", "DELETE", "TRACE"};
    for (Int32 i = 0; i < 7; i++) {
        CString permittedUserMethod = PERMITTED_USER_METHODS[i];
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
    assert(followRedirects != NULL);
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
//        throw new IllegalStateException("Already connected");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mChunkLength > 0) {
//        throw new IllegalStateException("Already in chunked mode");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (contentLength < 0) {
//        throw new IllegalArgumentException("contentLength < 0"
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mFixedContentLength = contentLength;

    return NOERROR;
}

ECode HttpURLConnection::SetChunkedStreamingMode(
    /* [in] */ Int32 chunkLength)
{
    if (mConnected) {
//        throw new IllegalStateException("Already connected");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mFixedContentLength > 0) {
//        throw new IllegalStateException("Already in chunked mode");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (chunkLength < 0) {
//        throw new IllegalArgumentException("contentLength < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mChunkLength = chunkLength;

    return NOERROR;
}
