
#include "webkit/FileLoader.h"
#include "webkit/CURLUtil.h"
#include "utils/CTypedValue.h"

const CString FileLoader::LOGTAG = "webkit";

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
FileLoader::FileLoader(
	/* [in] */ const String& url, 
	/* [in] */ /*const*/ LoadListener* loadListener, 
	/* [in] */ Int32 type,
	/* [in] */ Boolean allowFileAccess) : StreamLoader(loadListener)
{	
    mType = type;
    mAllowFileAccess = allowFileAccess;
    AutoPtr<IURLUtil> URL;
    CURLUtil::AcquireSingleton((IURLUtil**)&URL);

    assert(URL);

    String str1, str2;
    // clean the Url
    Int32 index = url.IndexOf('?');
    if (mType == TYPE_ASSET) {
    	URL->StripAnchor(url.Substring(strlen(CURLUtil::ASSET_BASE), index), &str1);
    	URL->StripAnchor(url.Substring(strlen(CURLUtil::ASSET_BASE)), &str2);
        mPath = index > 0 ? str1 : str2;
    } else if (mType == TYPE_RES) {
    	URL->StripAnchor(url.Substring(strlen(CURLUtil::RESOURCE_BASE), index), &str1);
    	URL->StripAnchor(url.Substring(strlen(CURLUtil::RESOURCE_BASE)), &str2);
        mPath = index > 0 ? str1 : str2;
    } else {
    	URL->StripAnchor(url.Substring(strlen(CURLUtil::FILE_BASE), index), &str1);
    	URL->StripAnchor(url.Substring(strlen(CURLUtil::FILE_BASE)), &str2);
        mPath = index > 0 ? str1 : str2;
    }
}

//@Override
CARAPI_(Boolean) FileLoader::SetupStreamAndSendStatus()
{
	if (mType == TYPE_ASSET) {
		IAssetManager* assetManager = NULL;
//        mContext->GetAssets(&assetManager)
        assetManager->Open(mPath, (IInputStream**)&mDataStream);
    } else if(mType == TYPE_RES) {
        // get the resource id from the path. e.g. for the path like
        // drawable/foo.png, the id is located at field "foo" of class
        // "<package>.R$drawable"
        if (mPath.GetLength() == 0) {
//            Log.e(LOGTAG, "Need a path to resolve the res file");
//            mLoadListener.error(EventHandler.FILE_ERROR, mContext
//                    .getString(R.string.httpErrorFileNotFound));
            return FALSE;

        }
        Int32 slash = mPath.IndexOf('/');
        Int32 dot = mPath.IndexOf('.', slash);
        if (slash == -1 || dot == -1) {
//            Log.e(LOGTAG, "Incorrect res path: " + mPath);
//            mLoadListener.error(EventHandler.FILE_ERROR, mContext
//                    .getString(R.string.httpErrorFileNotFound));
            return FALSE;
        }
        String subClassName = mPath.Substring(0, slash);
        String fieldName = mPath.Substring(slash + 1, dot);
        String errorMsg;
        IContext* pContext = NULL;
        IClassLoader* pClassLoader = NULL;
        Handle32 d;
        String classname;
        String packagename;

//        mContext->GetPackageName(&packagename);
        classname = packagename;
        classname += ".R$";
        classname += subClassName;
        
        mContext->GetApplicationContext(&pContext);
        pContext->GetClassLoader(&pClassLoader);
        pClassLoader->LoadClass(classname, &d);

//        final Field field = d.getField(fieldName);
//        final int id = field.getInt(null);
        AutoPtr<ITypedValue> value;
        CTypedValue::New((ITypedValue**)&value);
        assert(value);

//        mContext.getResources().getValue(id, value, true);
//        if (((CTypedValue*)(ITypedValue*)value)->mType == CTypedValue::TypedValue_TYPE_STRING)
        {
//            mDataStream = mContext.getAssets().openNonAsset(
//                    value.assetCookie, value.string.toString(),
//                    AssetManager.ACCESS_STREAMING);
        }
//        else
        {
            errorMsg = "Only support TYPE_STRING for the res files";
        }
        
        if (errorMsg.GetLength() != 0)
        {
//            mLoadListener.error(EventHandler.FILE_ERROR, mContext
//                    .getString(R.string.httpErrorFileNotFound));
            return FALSE;
        }
    }
    else
    {
        if (!mAllowFileAccess)
        {
//            mLoadListener.error(EventHandler.FILE_ERROR,
//                    mContext.getString(R.string.httpErrorFileNotFound));
            return FALSE;
        }

//        mDataStream = new FileInputStream(mPath);
//        mContentLength = (new File(mPath)).length();
    }
//    mLoadListener->Status(1, 1, 200, "OK");

    return TRUE;
}

//@Override
CARAPI_(void) FileLoader::BuildHeaders(
    /* [in] */ const IHeaders* headers)
{}