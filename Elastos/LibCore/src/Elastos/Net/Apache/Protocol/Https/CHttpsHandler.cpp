
#include "CHttpsHandler.h"
#include "CHttpsURLConnectionImpl.h"

ECode CHttpsHandler::OpenConnection(
    /* [in] */ IURL * pU,
    /* [out] */ IURLConnection ** ppUrlConnection)
{
    // TODO: Add your code here
    Int32 defaultPort;
    GetDefaultPort(&defaultPort);
    return CHttpsURLConnectionImpl::New(pU, defaultPort, (IHttpsURLConnectionImpl **) ppUrlConnection);
}

ECode CHttpsHandler::OpenConnectionEx(
    /* [in] */ IURL * pU,
    /* [in] */ IProxy * pProxy,
    /* [out] */ IURLConnection ** ppUrlConnection)
{
    // TODO: Add your code here
    if (pU == NULL || pProxy == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 defaultPort;
    GetDefaultPort(&defaultPort);

    return CHttpsURLConnectionImpl::New(pU, defaultPort, pProxy, (IHttpsURLConnectionImpl **) ppUrlConnection);
}

ECode CHttpsHandler::ParseURL(
    /* [in] */ IURL * pU,
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsHandler::SetURL(
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

ECode CHttpsHandler::SetURLEx(
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

ECode CHttpsHandler::ToExternalForm(
    /* [in] */ IURL * pUrl,
    /* [out] */ String * pS)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsHandler::Equals(
    /* [in] */ IURL * pUrl1,
    /* [in] */ IURL * pUrl2,
    /* [out] */ Boolean * pIsEquals)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsHandler::GetDefaultPort(
    /* [out] */ Int32 * pPortNum)
{
    // TODO: Add your code here
    *pPortNum = 443;
    return NOERROR;
}

ECode CHttpsHandler::GetHostAddress(
    /* [in] */ IURL * pUrl,
    /* [out] */ IInetAddress ** ppHostAddress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsHandler::GetHashCode(
    /* [in] */ IURL * pUrl,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsHandler::HostsEqual(
    /* [in] */ IURL * pA,
    /* [in] */ IURL * pB,
    /* [out] */ Boolean * pIsEqual)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsHandler::IsSameFile(
    /* [in] */ IURL * pUrl1,
    /* [in] */ IURL * pUrl2,
    /* [out] */ Boolean * pIsSame)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHttpsHandler::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

