#ifndef __IDOWNLOADLISTENER_H__
#define __IDOWNLOADLISTENER_H__

class DownloadListener {

    /**
     * Notify the host application that a file should be downloaded
     * @param url The full url to the content that should be downloaded
     * @param userAgent the user agent to be used for the download.
     * @param contentDisposition Content-disposition http header, if 
     *                           present.
     * @param mimetype The mimetype of the content reported by the server
     * @param contentLength The file size reported by the server
     */
	virtual CARAPI_(void) onDownloadStart(
		/* [in] */ const String& url, 
		/* [in] */ const String& userAgent,
		/* [in] */ const String& contentDisposition, 
		/* [in] */ const String& mimetype, 
		/* [in] */ Int64 contentLength) = 0;

};

#endif //__IDOWNLOADLISTENER_H__