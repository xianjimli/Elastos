
#ifndef __GPSXTRADOWNLOADER_H__
#define __GPSXTRADOWNLOADER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

class GpsXtraDownloader
{
private:
    static const CString TAG;
    static const Boolean DEBUG = FALSE;

public:
    GpsXtraDownloader(
        /* [in] */ IContext* context
        /* [in]  Properties properties*/);

    CARAPI_(ArrayOf<Byte>*) DownloadXtraData();

    CARAPI_(ArrayOf<Byte>*) DoDownload(
    	/* [in] */ String url,
        /* [in] */ Boolean isProxySet,
        /* [in] */ String proxyHost,
        /* [in] */ Int32 proxyPort);

private:
    AutoPtr<IContext> mContext;
    ArrayOf<String>* mXtraServers;
    // to load balance our server requests
    Int32 mNextServerIndex;
};

#endif //__GPSXTRADOWNLOADER_H__
