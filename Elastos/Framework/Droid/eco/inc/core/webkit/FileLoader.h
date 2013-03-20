#ifndef __FILELOADER_H__
#define __FILELOADER_H__

#include "StreamLoader.h"

#include "ext/frameworkext.h"

class LoadListener;

/**
 * This class is a concrete implementation of StreamLoader that uses a
 * file or asset as the source for the stream.
 *
 */
class FileLoader : public StreamLoader {

public:
    // used for files under asset directory
    static const Int32 TYPE_ASSET = 1;
    // used for files under res directory
    static const Int32 TYPE_RES = 2;
    // generic file
    static const Int32 TYPE_FILE = 3;
    
public:
    /**
     * Construct a FileLoader with the file URL specified as the content
     * source.
     *
     * @param url Full file url pointing to content to be loaded
     * @param loadListener LoadListener to pass the content to
     * @param asset true if url points to an asset.
     * @param allowFileAccess true if this WebView is allowed to access files
     *                        on the file system.
     */
    FileLoader(
    	/* [in] */ const String& url, 
    	/* [in] */ /*const*/ LoadListener* loadListener, 
    	/* [in] */ Int32 type,
    	/* [in] */ Boolean allowFileAccess);

protected:
    //@Override
	virtual CARAPI_(Boolean) SetupStreamAndSendStatus();

    //@Override
    virtual CARAPI_(void) BuildHeaders(
        /* [in] */ const IHeaders* headers);

private:
//    CString ErrString(
//    	/* [in] */ Exception ex);

private:
	String mPath;  // Full path to the file to load
	Int32 mType;  // Indicates the type of the load
	Boolean mAllowFileAccess; // Allow/block file system access

	static const CString LOGTAG;// = "webkit";
};

#endif //__FILELOADER_H__