
#ifndef __CWEBHISTORYITEM_H__
#define __CWEBHISTORYITEM_H__

#include "_CWebHistoryItem.h"

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include <elastos/Mutex.h>

CarClass(CWebHistoryItem)
{
public:
    friend class CWebHistoryItem;

    /*public*/
    CARAPI GetId(
        /* [out] */ Int32 * id);

    /*public*/
    CARAPI GetUrl(
        /* [out] */ String * url);

    /*public*/
    CARAPI GetOriginalUrl(
        /* [out] */ String * originalUrl);

    /*public*/
    CARAPI GetTitle(
        /* [out] */ String * title);

    /*public*/
    CARAPI GetFavicon(
        /* [out] */ IBitmap ** favicon);

    /*public*/
    CARAPI GetTouchIconUrl(
        /* [out] */ String * touchIconUrl);

    /*public*/
    CARAPI GetCustomData(
        /* [out] */ IInterface ** customData);

    /*public*/
    CARAPI SetCustomData(
        /* [in] */ IInterface * data);

    /**
     * Construct a new WebHistoryItem with initial flattened data.
     * @param data The pre-flattened data coming from restoreState.
     */
    /*package*/
    CARAPI constructor(
        /* [in] */ ArrayOf<byte>* data);

    /**
     * Set the favicon.
     * @param icon A Bitmap containing the favicon for this history item.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/ 
    CARAPI SetFavicon(
        /* [in] */ IBitmap* icon);

    /**
     * Set the touch icon url.
     * @hide
     */
    /*package*/ 
    CARAPI SetTouchIconUrl(
        /* [in] */ String url);

    /**
     * Get the pre-flattened data.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/ 
    CARAPI GetFlattenedData(
        /* [out] */ ArrayOf<byte>** data);

    /**
     * Inflate this item.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/
    CARAPI Inflate(
        /* [in] */ Int32 nativeFrame);

protected:
    /**
     * Clone the history item for use by clients of WebView.
     */
    //synchronized
    CARAPI Clone(
        /* [out] */ IWebHistoryItem** item);
        
private:
    /**
     * Basic constructor that assigns a unique id to the item. Called by JNI
     * only.
     */
    CARAPI constructor();

    /**
     * Construct a clone of a WebHistoryItem from the given item.
     * @param item The history item to clone.
     */
    //CARAPI constructor(IWebHistoryItem* item);


    CARAPI constructor(
        /* [in] */ IWebHistoryItem* item);

    /* Natively inflate this item, this method is called in the WebCore thread.
     */
    //native 
    virtual CARAPI Inflate(
        /* [in] */ Int32 nativeFrame, 
        /* [in] */ ArrayOf<byte>* data);//=0;

    /* Called by jni when the item is updated */
    CARAPI Update(
        /* [in] */ String url, 
        /* [in] */ String originalUrl, 
        /* [in] */ String title,
        /* [in] */ IBitmap* favicon,
        /* [in] */ ArrayOf<byte>* data);

    CARAPI SetId(
        /* [in] */ Int32 id);

private:
    // Global identifier count.    
    static Int32 sNextId;// = 0;
    // Unique identifier.
    /*const*/ Int32 mId;
    // The title of this item's document.
    String mTitle;
    // The base url of this item.
    String mUrl;
    // The original requested url of this item.
    String mOriginalUrl;
    // The favicon for this item.
    AutoPtr<IBitmap> mFavicon;
    // The pre-flattened data used for saving the state.
    AutoFree < ArrayOf<byte> > mFlattenedData;
    // The apple-touch-icon url for use when adding the site to the home screen
    String mTouchIconUrl;
    // Custom client data that is not flattened or read by native code.
    AutoPtr<IInterface> mCustomData;

    static Core::Threading::Mutex mMutexClass;
};

#endif // __CWEBHISTORYITEM_H__
