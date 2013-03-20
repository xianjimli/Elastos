
#ifndef __CMEDIASCANNERCONNECTION_H__
#define __CMEDIASCANNERCONNECTION_H__

#include "_CMediaScannerConnection.h"
#include "media/MediaScannerConnection.h"

CarClass(CMediaScannerConnection), public MediaScannerConnection
{
public:
    CARAPI OnServiceConnected(
        /* [in] */ IComponentName * pName,
        /* [in] */ IBinder * pService);

    CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName * pName);

    CARAPI Connect();

    CARAPI Disconnect();

    CARAPI IsConnected(
        /* [out] */ Boolean * pConnected);

    CARAPI ScanFile(
        /* [in] */ const String& path,
        /* [in] */ const String& mimeType);

    CARAPI ScanFileEx(
        /* [in] */ IContext * pContext,
        /* [in] */ const ArrayOf<String> & paths,
        /* [in] */ const ArrayOf<String> & mimeTypes,
        /* [in] */ IOnScanCompletedListener * pCb);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IMediaScannerConnectionClient * pClient);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMEDIASCANNERCONNECTION_H__
