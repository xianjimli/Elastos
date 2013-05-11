
#include "CFileHandler.h"
#include "CURL.h"
#include "CFileURLConnection.h"
#include "CProxy.h"

ECode CFileHandler::OpenConnection(
    /* [in] */ IURL * pU,
    /* [out] */ IURLConnection ** ppUrlConnection)
{
    // TODO: Add your code here
    return OpenConnectionEx(pU, NULL, ppUrlConnection);
}

ECode CFileHandler::OpenConnectionEx(
    /* [in] */ IURL * pU,
    /* [in] */ IProxy * pProxy,
    /* [out] */ IURLConnection ** ppUrlConnection)
{
    // TODO: Add your code here
    if (pU == NULL) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = NOERROR;
    String host;
    pU->GetHost(&host);

    if (host.IsNull() || host.IsEmpty()) {
        return CFileURLConnection::New(pU, (IFileURLConnection**)ppUrlConnection);
    }

    // If a hostname is specified try to get the resource using FTP
    AutoPtr<IURL> ftpURL;
    String file;
    pU->GetFile(&file);

    ec = CURL::New(String("ftp"), host, file, (IURL**)&file);
    if (pProxy == NULL) {
        ftpURL->OpenConnection(ppUrlConnection);
    }
    else {
        ftpURL->OpenConnectionEx(pProxy, ppUrlConnection);
    }

    return ec;
}

ECode CFileHandler::ParseURL(
    /* [in] */ IURL * pU,
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // TODO: Add your code here
    if (end < start) {
        return E_INVALID_ARGUMENT;
    }

    String parseString = String("");
    if (start < end) {
//        parseString = str.Substring(start, end-start).Replace('\\', '/');
    }

    return URLStreamHandler::ParseURL(pU, parseString, 0, parseString.GetLength());
}

ECode CFileHandler::SetURL(
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

ECode CFileHandler::SetURLEx(
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

ECode CFileHandler::ToExternalForm(
    /* [in] */ IURL * pUrl,
    /* [out] */ String * pS)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileHandler::Equals(
    /* [in] */ IURL * pUrl1,
    /* [in] */ IURL * pUrl2,
    /* [out] */ Boolean * pIsEquals)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileHandler::GetDefaultPort(
    /* [out] */ Int32 * pPortNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileHandler::GetHostAddress(
    /* [in] */ IURL * pUrl,
    /* [out] */ IInetAddress ** ppHostAddress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileHandler::GetHashCode(
    /* [in] */ IURL * pUrl,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileHandler::HostsEqual(
    /* [in] */ IURL * pA,
    /* [in] */ IURL * pB,
    /* [out] */ Boolean * pIsEqual)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileHandler::IsSameFile(
    /* [in] */ IURL * pUrl1,
    /* [in] */ IURL * pUrl2,
    /* [out] */ Boolean * pIsSame)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileHandler::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

