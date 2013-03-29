
#ifndef __CWEBBACKFORWARDLIST_H__
#define __CWEBBACKFORWARDLIST_H__

#include "_CWebBackForwardList.h"

//#include "webkit/CWebHistoryItem.h"
//#include "webkit/CCallbackProxy.h"
#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>

CarClass(CWebBackForwardList)
{
public:
    //synchronized
    CARAPI GetCurrentItem(
        /* [out] */ IWebHistoryItem** item);

    CARAPI GetCurrentIndex(
        /* [out] */ Int32* index);

    CARAPI GetItemAtIndex(
        /* [in] */ Int32 index,
        /* [out] */ IWebHistoryItem** item);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI constructor(
        /* [in] */ ICallbackProxy* proxy);

    /**
     * Mark the back/forward list as having a pending clear. This is used on the
     * UI side to mark the list as being invalid during the clearHistory method.
     */
    /*package*/ 
    //synchronized 
    CARAPI SetClearPending();

    /**
     * Return the status of the clear flag. This is used on the UI side to
     * determine if the list is valid for checking things like canGoBack.
     */
    /*package*/ 
    //synchronized 
    CARAPI GetClearPending(
        /* [out] */ Boolean* clearPending);

    /**
     * Add a new history item to the list. This will remove all items after the
     * current item and append the new item to the end of the list. Called from
     * the WebCore thread only. Synchronized because the UI thread may be
     * reading the array or the current index.
     * @param item A new history item.
     */
    /*package*/ 
    //synchronized
    CARAPI AddHistoryItem(
        /* [in] */ IWebHistoryItem* item);

    /**
     * Clear the back/forward list. Called from the WebCore thread.
     */
    /*package*/ 
    //synchronized 
    CARAPI Close(
        /* [in] */ Int32 nativeFrame);

    /**
     * Set the new history index.
     * @param newIndex The new history index.
     */
    /*package*/ 
    //synchronized 
    CARAPI SetCurrentIndex(
        /* [in] */ Int32 newIndex);

    /**
     * Restore the history index.
     */
    /*package*/ 
    //synchronized
    /*virtual*/ static CARAPI RestoreIndex(
        /* [in] */ Int32 nativeFrame,
        /* [in] */ Int32 index);// = 0 ;

protected:
    /**
     * Clone the entire object to be used in the UI thread by clients of
     * WebView. This creates a copy that should never be modified by any of the
     * webkit package classes.
     */
    //synchronized 
    CARAPI Clone(
        /* [in] */ IWebBackForwardList** webBackForwardList); 

private:
    /* Remove the item at the given index. Called by JNI only. */
    //synchronized 
    CARAPI RemoveHistoryItem(
        /* [in] */ Int32 index);

    /* Close the native list. */
    /*virtual*/ static CARAPI NativeClose(
        /* [in] */ Int32 nativeFrame);// = 0 ;

private:
    // Current position in the list.
    Int32 mCurrentIndex;
    // ArrayList of WebHistoryItems for maintaining our copy.
    List< AutoPtr<IWebHistoryItem> > mArray;
    // Flag to indicate that the list is invalid
    Boolean mClearPending;
    // CallbackProxy to issue client callbacks.
    /*const*/ AutoPtr<ICallbackProxy> mCallbackProxy;
};

#endif // __CWEBBACKFORWARDLIST_H__
