
#include "media/CMediaScannerConnection.h"

ECode CMediaScannerConnection::OnServiceConnected(
    /* [in] */ IComponentName * pName,
    /* [in] */ IBinder * pService)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaScannerConnection::OnServiceDisconnected(
    /* [in] */ IComponentName * pName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaScannerConnection::Connect()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaScannerConnection::Disconnect()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaScannerConnection::IsConnected(
    /* [out] */ Boolean * pConnected)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaScannerConnection::ScanFile(
    /* [in] */ const String& path,
    /* [in] */ const String& mimeType)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaScannerConnection::ScanFileEx(
    /* [in] */ IContext * pContext,
    /* [in] */ const ArrayOf<String> & paths,
    /* [in] */ const ArrayOf<String> & mimeTypes,
    /* [in] */ IOnScanCompletedListener * pCb)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMediaScannerConnection::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IMediaScannerConnectionClient * pClient)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

