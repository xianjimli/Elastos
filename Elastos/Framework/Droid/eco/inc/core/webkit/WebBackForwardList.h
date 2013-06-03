#ifndef __WEBBACKFORWARDLIST_H__
#define __WEBBACKFORWARDLIST_H__


#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>

/**
 * This class contains the back/forward list for a WebView.
 * WebView.copyBackForwardList() will return a copy of this class used to
 * inspect the entries in the list.
 */
class WebBackForwardList //: public Cloneable, public Serializable 
{

public:
    /**
     * Construct a back/forward list used by clients of WebView.
     */
    /*package*/ 
     WebBackForwardList(
         /* [in] */ ICallbackProxy* proxy);

    /**
     * Return the current history item. This method returns null if the list is
     * empty.
     * @return The current history item.
     *
     * synchronized
     */
     /*public*/
    virtual CARAPI_(IWebHistoryItem*) GetCurrentItem();

    /**
     * Get the index of the current history item. This index can be used to
     * directly index into the array list.
     * @return The current index from 0...n or -1 if the list is empty.
     *
     * synchronized
     */
     /*public*/
    virtual CARAPI_(Int32) GetCurrentIndex();

    /**
     * Get the history item at the given index. The index range is from 0...n
     * where 0 is the first item and n is the last item.
     * @param index The index to retrieve.
     *
     * synchronized
     */
     /*public*/
    virtual CARAPI_(IWebHistoryItem*) GetItemAtIndex(
        /* [in] */ Int32 index);

    /**
     * Get the total size of the back/forward list.
     * @return The size of the list.
     *
     * synchronized
     */
     /*public*/
    virtual CARAPI_(Int32) GetSize();

    /**
     * Mark the back/forward list as having a pending clear. This is used on the
     * UI side to mark the list as being invalid during the clearHistory method.
     *
     * synchronized
     */
    /*package*/ 
    virtual CARAPI_(void) SetClearPending();

    /**
     * Return the status of the clear flag. This is used on the UI side to
     * determine if the list is valid for checking things like canGoBack.
     *
     * synchronized
     */
    /*package*/ 
    virtual CARAPI_(Boolean) GetClearPending();

    /**
     * Add a new history item to the list. This will remove all items after the
     * current item and append the new item to the end of the list. Called from
     * the WebCore thread only. Synchronized because the UI thread may be
     * reading the array or the current index.
     * @param item A new history item.
     *
     * synchronized
     */
    /*package*/ 
     virtual CARAPI_(void) AddHistoryItem(
         /* [in] */ IWebHistoryItem* item);

    /**
     * Clear the back/forward list. Called from the WebCore thread.
     *
     * synchronized
     */
    /*package*/ 
    virtual CARAPI_(void) Close(
        /* [in] */ Int32 nativeFrame);



    /**
     * Set the new history index.
     * @param newIndex The new history index.
     *
     * synchronized
     */
    /*package*/  
    virtual CARAPI_(void) SetCurrentIndex(
        /* [in] */ Int32 newIndex);

    /**
     * Restore the history index.
     *
     * synchronized
     * native
     */
    /*package*/ 
    static CARAPI_(void) RestoreIndex(
        /* [in] */ Int32 nativeFrame,
        /* [in] */ Int32 index);

protected:
    /**
     * Clone the entire object to be used in the UI thread by clients of
     * WebView. This creates a copy that should never be modified by any of the
     * webkit package classes.
     *
     * synchronized
     */
    //virtual CARAPI_(IWebBackForwardList*) Clone();

    WebBackForwardList();
    void Init(
        /* [in] */ ICallbackProxy* proxy);

private:
    /* Remove the item at the given index. Called by JNI only. */
    /* synchronized */
    CARAPI_(void) RemoveHistoryItem(
        /* [in] */ Int32 index);

    /* Close the native list. */
    /* native */
    static CARAPI_(void) NativeClose(
        /* [in] */ Int32 nativeFrame);

protected:
    // Current position in the list.
    Int32 mCurrentIndex;
    // ArrayList of WebHistoryItems for maintaining our copy.
    List< AutoPtr<IWebHistoryItem> > mArray;    
    // Flag to indicate that the list is invalid
    Boolean mClearPending;

private:
    // CallbackProxy to issue client callbacks.
    /*const*/ AutoPtr<ICallbackProxy> mCallbackProxy;

    Elastos::Core::Threading::Mutex mMutexThis;
    //static Elastos::Core::Threading::Mutex mMutexClass;
};

#endif //__WEBBACKFORWARDLIST_H__