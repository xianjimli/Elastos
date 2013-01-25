#ifndef __DATALOADER_H__
#define __DATALOADER_H__

/**
 * This class is a concrete implementation of StreamLoader that uses the
 * content supplied as a URL as the source for the stream. The mimetype
 * optionally provided in the URL is extracted and inserted into the HTTP
 * response headers.
 */
class DataLoader : public StreamLoader {

public:
    /**
     * Constructor uses the dataURL as the source for an InputStream
     * @param dataUrl data: URL string optionally containing a mimetype
     * @param loadListener LoadListener to pass the content to
     */
    DataLoader(
    	/* [in] */ CString dataUrl, 
    	/* [in] */ ILoadListener loadListener);

protected:
    //@Override
	virtual CARAPI_(Boolean) SetupStreamAndSendStatus();

    //@Override
    virtual CARAPI_(void) BuildHeaders(
    	/* [in] */ IHeaders* h);
};

#endif //__DATALOADER_H__