#ifndef __CONTENTLOADER_H__
#define __CONTENTLOADER_H__

#include "ext/frameworkext.h"
#include "StreamLoader.h"

/**
 * This class is a concrete implementation of StreamLoader that loads
 * "content:" URIs
 */
class ContentLoader : public StreamLoader
{
public:
    /**
     * Construct a ContentLoader with the specified content URI
     *
     * @param rawUrl "content:" url pointing to content to be loaded. This url
     *               is the same url passed in to the WebView.
     * @param loadListener LoadListener to pass the content to
     */
    ContentLoader(
    	/* [in] */ const String& rawUrl, 
    	/* [in] */ LoadListener* loadListener);

protected:
    //@Override
	CARAPI_(Boolean) SetupStreamAndSendStatus();

    //@Override
	CARAPI_(void) BuildHeaders(
		/* [in] */ IHeaders* headers);

private:
//	String errString(Exception ex);

private:
	String mUrl;
	String mContentType;
};

#endif //__CONTENTLOADER_H__