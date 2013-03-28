#ifndef __WEBHISTORYITEM_H__
#define __WEBHISTORYITEM_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include <elastos/Mutex.h>
/**
 * A convenience class for accessing fields in an entry in the back/forward list
 * of a WebView. Each WebHistoryItem is a snapshot of the requested history
 * item. Each history item may be updated during the load of a page.
 * @see WebBackForwardList
 */
class WebHistoryItem {

public:

    /**
     * Construct a new WebHistoryItem with initial flattened data.
     * @param data The pre-flattened data coming from restoreState.
     */
    /*package*/ 
    WebHistoryItem(
        /* [in] */ ArrayOf<Byte>* data);

public:
    /**
     * Return an identifier for this history item. If an item is a copy of
     * another item, the identifiers will be the same even if they are not the
     * same object.
     * @return The id for this item.
     */
    virtual CARAPI_(Int32) GetId();

    /**
     * Return the url of this history item. The url is the base url of this
     * history item. See getTargetUrl() for the url that is the actual target of
     * this history item.
     * @return The base url of this history item.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    virtual CARAPI_(String) GetUrl();

    /**
     * Return the original url of this history item. This was the requested
     * url, the final url may be different as there might have been 
     * redirects while loading the site.
     * @return The original url of this history item.
     */
    virtual CARAPI_(String) GetOriginalUrl();
    
    /**
     * Return the document title of this history item.
     * @return The document title of this history item.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    virtual CARAPI_(String) GetTitle();

    /**
     * Return the favicon of this history item or null if no favicon was found.
     * @return A Bitmap containing the favicon for this history item or null.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    virtual CARAPI_(IBitmap*) GetFavicon();

    /**
     * Return the touch icon url.
     * @hide
     */
    virtual CARAPI_(String) GetTouchIconUrl();

    /**
     * Return the custom data provided by the client.
     * @hide
     */
    virtual CARAPI_(IInterface*) GetCustomData();

    /**
     * Set the custom data field.
     * @param data An Object containing any data the client wishes to associate
     *             with the item.
     * @hide
     */
    virtual CARAPI_(void) SetCustomData(
        /* [in] */ IInterface* data);

    /**
     * Set the favicon.
     * @param icon A Bitmap containing the favicon for this history item.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/ 
    virtual CARAPI_(void) SetFavicon(
        /* [in] */ IBitmap* icon);

    /**
     * Set the touch icon url.
     * @hide
     */
    /*package*/ 
    virtual CARAPI_(void) SetTouchIconUrl(
        /* [in] */ CString url);

    /**
     * Get the pre-flattened data.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/ 
    virtual CARAPI_(ArrayOf<Byte>*) GetFlattenedData();

    /**
     * Inflate this item.
     * Note: The VM ensures 32-bit atomic read/write operations so we don't have
     * to synchronize this method.
     */
    /*package*/ 
    virtual CARAPI_(void) Inflate(
        /* [in] */ Int32 nativeFrame);

protected:
    /**
     * Clone the history item for use by clients of WebView.
     */
    virtual CARAPI_(WebHistoryItem*) Clone();


private:

    /* Natively inflate this item, this method is called in the WebCore thread.
     */
    CARAPI_(void) Inflate(
        /* [in] */ Int32 nativeFrame, 
        /* [in] */ ArrayOf<Byte>* data);

    /* Called by jni when the item is updated */
    CARAPI_(void) Update(
        /* [in] */ CString url, 
        /* [in] */ CString originalUrl, 
        /* [in] */ String title,
        /* [in] */ IBitmap* favicon,
        /* [in] */ ArrayOf<Byte>* data);

    /**
     * Basic constructor that assigns a unique id to the item. Called by JNI
     * only.
     */
    WebHistoryItem();

    /**
     * Construct a clone of a WebHistoryItem from the given item.
     * @param item The history item to clone.
     */
    WebHistoryItem(
        /* [in] */ WebHistoryItem* item);

private:
    // Global identifier count.
    static Int32 sNextId;
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
    AutoFree < ArrayOf<Byte> > mFlattenedData;
    // The apple-touch-icon url for use when adding the site to the home screen
    String mTouchIconUrl;
    // Custom client data that is not flattened or read by native code.
    IInterface* mCustomData;

    Elastos::Core::Threading::Mutex mLock;
    static Elastos::Core::Threading::Mutex sLock;

};

#endif //__WEBHISTORYITEM_H__