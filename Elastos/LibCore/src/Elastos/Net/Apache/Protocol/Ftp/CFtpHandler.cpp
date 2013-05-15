
#include "CFtpHandler.h"
#include "CFtpURLConnection.h"

ECode CFtpHandler::OpenConnection(
    /* [in] */ IURL * pU,
    /* [out] */ IURLConnection ** ppUrlConnection)
{
    // TODO: Add your code here
    return CFtpURLConnection::New(pU, (IFtpURLConnection **)ppUrlConnection);
}

ECode CFtpHandler::OpenConnectionEx(
    /* [in] */ IURL * pU,
    /* [in] */ IProxy * pProxy,
    /* [out] */ IURLConnection ** ppUrlConnection)
{
    // TODO: Add your code here
    if (pU == NULL || pProxy == NULL) {
        return E_INVALID_ARGUMENT;
    }

    return CFtpURLConnection::New(pU, pProxy, (IFtpURLConnection **)ppUrlConnection);
}

ECode CFtpHandler::ParseURL(
    /* [in] */ IURL * pU,
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::SetURL(
    /* [in] */ IURL * pU,
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file,
    /* [in] */ const String& ref)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::SetURLEx(
    /* [in] */ IURL * pU,
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& authority,
    /* [in] */ const String& userInfo,
    /* [in] */ const String& file,
    /* [in] */ const String& query,
    /* [in] */ const String& ref)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::ToExternalForm(
    /* [in] */ IURL * pUrl,
    /* [out] */ String * pS)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::Equals(
    /* [in] */ IURL * pUrl1,
    /* [in] */ IURL * pUrl2,
    /* [out] */ Boolean * pIsEquals)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::GetDefaultPort(
    /* [out] */ Int32 * pPortNum)
{
    // TODO: Add your code here
    *pPortNum = 21;
    return NOERROR;
}

ECode CFtpHandler::GetHostAddress(
    /* [in] */ IURL * pUrl,
    /* [out] */ IInetAddress ** ppHostAddress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::GetHashCode(
    /* [in] */ IURL * pUrl,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::HostsEqual(
    /* [in] */ IURL * pA,
    /* [in] */ IURL * pB,
    /* [out] */ Boolean * pIsEqual)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::IsSameFile(
    /* [in] */ IURL * pUrl1,
    /* [in] */ IURL * pUrl2,
    /* [out] */ Boolean * pIsSame)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpHandler::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

