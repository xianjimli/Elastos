#ifndef __FRAMELOADER_H__
#define __FRAMELOADER_H__

#include "WebSettings.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

class LoadListener;
class Network;

class FrameLoader {

public:
    static const char* HEADER_STR;// = "text/xml, text/html, " +
        //"application/xhtml+xml, image/png, text/plain, */*;q=0.8";

public:
    FrameLoader(
    	/* [in] */ LoadListener* listener, 
    	/* [in] */ WebSettings* settings,
    	/* [in] */ const String& method);

	virtual CARAPI_(void) SetReferrer(
		/* [in] */ const String& ref);

	virtual CARAPI_(void) SetPostData(
		/* [in] */ Vector<Byte>& postData);

	virtual CARAPI_(void) SetContentTypeForPost(
		/* [in] */ const String& postContentType);

	virtual CARAPI_(void) SetCacheMode(
		/* [in] */ Int32 cacheMode);

	virtual CARAPI_(void) SetHeaders(
		/* [in] */ /*IHashMap*/IObjectStringMap* headers);

	virtual CARAPI_(LoadListener*) GetLoadListener() const;

    /**
     * Issues the load request.
     *
     * Return value does not indicate if the load was successful or not. It
     * simply indicates that the load request is reasonable.
     *
     * @return true if the load is reasonable.
     */
	virtual CARAPI_(Boolean) ExecuteLoad();

    /* package */
    static CARAPI_(Boolean) HandleLocalFile(
    	/* [in] */ const String& url, 
    	/* [in] */ LoadListener* loadListener,
    	/* [in] */ WebSettings* settings);

    virtual CARAPI_(Boolean) HandleHTTPLoad();

private:
    /*
     * This function is used by handleCache to
     * setup a load from the byte stream in a CacheResult.
     */
    CARAPI_(void) StartCacheLoad(
    	/* [in] */ const ICacheManagerCacheResult* result);

    /*
     * This function is used by the handleHTTPLoad to setup the cache headers
     * correctly.
     * Returns true if the response was handled from the cache
     */
	CARAPI_(Boolean) HandleCache();
    
    /**
     * Add the static headers that don't change with each request.
     */
	CARAPI_(void) PopulateStaticHeaders();

    /**
     * Add the content related headers. These headers contain user private data
     * and is not used when we are proxying an untrusted request.
     */
	CARAPI_(void) PopulateHeaders();

private:
	/*const*/ LoadListener* mListener;
	/*const*/ String mMethod;
	/*const*/ WebSettings* mSettings;
	AutoPtr<IObjectStringMap> mHeaders;
	Vector<Byte> mPostData;
	Network*  mNetwork;
	Int32 mCacheMode;
	String mReferrer;
	String mContentType;

	static const Int32 URI_PROTOCOL = 0x100;

	static const char* CONTENT_TYPE;// = "content-type";

    // Contents of an about:blank page
	static const char* mAboutBlank;// =
            //"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EB\">" +
            //"<html><head><title>about:blank</title></head><body></body></html>";

	static const char* LOGTAG;// = "webkit";
};

#endif //__FRAMELOADER_H__