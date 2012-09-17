
#include "server/location/GpsXtraDownloader.h"

const CString GpsXtraDownloader::TAG = "GpsXtraDownloader";
const Boolean GpsXtraDownloader::DEBUG;

GpsXtraDownloader::GpsXtraDownloader(
    /* [in] */ IContext* context
    /* [in]  Properties properties*/)
    : mContext(context)
{
}

ArrayOf<Byte>* GpsXtraDownloader::DownloadXtraData()
{
	return NULL;
}

ArrayOf<Byte>* GpsXtraDownloader::DoDownload(
	/* [in] */ String url,
	/* [in] */ Boolean isProxySet,
	/* [in] */ String proxyHost,
	/* [in] */ Int32 proxyPort)
{
	return NULL;
}

