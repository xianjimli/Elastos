#include "webkit/WebBackForwardList.h"

//#include <elastos/Mutex.h>
#include "webkit/DebugFlags.h"


WebBackForwardList::WebBackForwardList(
    /* [in] */ ICallbackProxy* proxy)
{
    mCurrentIndex = -1;
    //JAVA:mArray = new ArrayList<WebHistoryItem>();
    mCallbackProxy = proxy;
    return ;
}

IWebHistoryItem* WebBackForwardList::GetCurrentItem()
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    return GetItemAtIndex(mCurrentIndex);
}

Int32 WebBackForwardList::GetCurrentIndex()
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    return mCurrentIndex;
}

IWebHistoryItem* WebBackForwardList::GetItemAtIndex(
    /* [in] */ Int32 index)
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    if (index < 0 || index >= GetSize() )  {
        return NULL;
    }
    return (mArray[index]).Get();
}

Int32 WebBackForwardList::GetSize()
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    return mArray.GetSize();
}

void WebBackForwardList::SetClearPending()
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    mClearPending = TRUE;
    return ;
}

Boolean WebBackForwardList::GetClearPending()
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    return mClearPending;
}

void WebBackForwardList::AddHistoryItem(
    /* [in] */ IWebHistoryItem* item)
{    
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    // Update the current position because we are going to add the new item
    // in that slot.
    ++mCurrentIndex;
    // If the current position is not at the end, remove all history items
    // after the current item.
    const Int32 size = mArray.GetSize();
    const Int32 newPos = mCurrentIndex;
    if (newPos != size) {
        for (int i = size - 1; i >= newPos; i--) {
            mArray.Remove(i);
        }
    }
    // Add the item to the list.
    mArray.PushBack(item);
    if (mCallbackProxy.Get() != NULL)  {
        mCallbackProxy -> OnNewHistoryItem(item);
    }
    return ;
}

void WebBackForwardList::Close(
        /* [in] */ Int32 nativeFrame)
{    
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    // Clear the array first because nativeClose will call addHistoryItem
    // with the current item.
    mArray.Clear();
    mCurrentIndex = -1;
    NativeClose(nativeFrame);
    // Reset the clear flag
    mClearPending = FALSE;
    return ;
}

void WebBackForwardList::RemoveHistoryItem(
    /* [in] */ Int32 index)
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    // XXX: This is a special case. Since the callback is only triggered
    // when removing the first item, we can assert that the index is 0.
    // This lets us change the current index without having to query the
    // native BackForwardList.
    if (DebugFlags::sWEB_BACK_FORWARD_LIST && (index != 0)) {
        //throw new AssertionError();
        return ;//E_ASSERTION_ERROR;
    }
    mArray.Remove(index);
    // XXX: If we ever add another callback for removing history items at
    // any index, this will no longer be valid.
    mCurrentIndex--;
    return ;
}

IWebBackForwardList* WebBackForwardList::Clone()
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    AutoPtr<WebBackForwardList> l = new WebBackForwardList(NULL);
    if (mClearPending)  {
        // If a clear is pending, return a copy with only the current item.
        l-> AddHistoryItem(GetCurrentItem());
        return (IWebBackForwardList*)(l.Get());
    }
    l -> mCurrentIndex = mCurrentIndex;
    for (int i = 0; i < GetSize(); i++)  {
        // Add a copy of each WebHistoryItem
        (l -> mArray).PushBack(mArray[i]);
    }
    return (IWebBackForwardList*)(l.Get());
}

void WebBackForwardList::SetCurrentIndex(
    /* [in] */ Int32 newIndex)
{
    Elastos::Core::Threading::Mutex::Autolock lock(mMutexThis);
    mCurrentIndex = newIndex;
    if (mCallbackProxy != NULL) {
        mCallbackProxy -> OnIndexChanged(GetItemAtIndex(newIndex), newIndex);
    }
    return ;    
}

void WebBackForwardList::RestoreIndex(
        /* [in] */ Int32 nativeFrame,
        /* [in] */ Int32 index)
{//=0(virtual)
    //Elastos::Core::Threading::Mutex::Autolock lock(mMutexClass);
    return ;        
}

void WebBackForwardList::NativeClose(
        /* [in] */ Int32 nativeFrame)
{//=0(virtual)
    //Elastos::Core::Threading::Mutex::Autolock lock(mMutexClass);    
    return ;    
}

