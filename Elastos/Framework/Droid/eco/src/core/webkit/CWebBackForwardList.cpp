
#include "webkit/CWebBackForwardList.h"

#include "webkit/DebugFlags.h"

ECode CWebBackForwardList::constructor(
        /* [in] */ ICallbackProxy* proxy)
{
    mCurrentIndex = -1;
    //JAVA:mArray = new ArrayList<WebHistoryItem>();
    mCallbackProxy = proxy;
    return NOERROR;
}

ECode CWebBackForwardList::GetCurrentItem(
    /* [out] */ IWebHistoryItem ** item)
{
    VALIDATE_NOT_NULL(item);
    Mutex::Autolock lock(mLock);
    GetItemAtIndex(mCurrentIndex,item);
    return NOERROR;
}

ECode CWebBackForwardList::GetCurrentIndex(
    /* [out] */ Int32 * index)
{
    VALIDATE_NOT_NULL(index);
    Mutex::Autolock lock(mLock);
    *index = mCurrentIndex;
    return NOERROR;
}

ECode CWebBackForwardList::GetItemAtIndex(
    /* [in] */ Int32 index,
    /* [out] */ IWebHistoryItem ** item)
{
    VALIDATE_NOT_NULL(item);
    Mutex::Autolock lock(mLock);
    Int32 nSize;
    GetSize(&nSize);
    if (index < 0 || index >= nSize) {
        *item = NULL;
    }
    *item = (mArray[index]).Get();
    return NOERROR;
}

ECode CWebBackForwardList::GetSize(
    /* [out] */ Int32 * size)
{
    VALIDATE_NOT_NULL(size);
    Mutex::Autolock lock(mLock);
    *size = mArray.GetSize();
    return NOERROR;
}

ECode CWebBackForwardList::SetClearPending()
{
    Mutex::Autolock lock(mLock);
    mClearPending = TRUE;
    return NOERROR;
}

ECode CWebBackForwardList::GetClearPending(
    /* [out] */ Boolean* clearPending)
{
    Mutex::Autolock lock(mLock);
    *clearPending = mClearPending;
    return NOERROR;
}

ECode CWebBackForwardList::AddHistoryItem(
    /* [in] */ IWebHistoryItem* item)
{    
    Mutex::Autolock lock(mLock);
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
    if (mCallbackProxy.Get() != NULL) {
        mCallbackProxy -> OnNewHistoryItem(item);
    }
    return NOERROR;
}

ECode CWebBackForwardList::Close(
    /* [in] */ Int32 nativeFrame)
{
    Mutex::Autolock lock(mLock);
    // Clear the array first because nativeClose will call addHistoryItem
    // with the current item.
    mArray.Clear();
    mCurrentIndex = -1;
    NativeClose(nativeFrame);
    // Reset the clear flag
    mClearPending = FALSE;
    return NOERROR;
}

ECode CWebBackForwardList::RemoveHistoryItem(
    /* [in] */ Int32 index)
{
    Mutex::Autolock lock(mLock);
    // XXX: This is a special case. Since the callback is only triggered
    // when removing the first item, we can assert that the index is 0.
    // This lets us change the current index without having to query the
    // native BackForwardList.
    if (DebugFlags::sWEB_BACK_FORWARD_LIST && (index != 0))  {
        //throw new AssertionError();
        return E_ASSERTION_ERROR;
    }
    mArray.Remove(index);
    // XXX: If we ever add another callback for removing history items at
    // any index, this will no longer be valid.
    mCurrentIndex--;
    return NOERROR;
}

ECode CWebBackForwardList::Clone(
    /* [in] */ IWebBackForwardList** webBackForwardList)
{
    Mutex::Autolock lock(mLock);
    AutoPtr<CWebBackForwardList> l = new CWebBackForwardList();
    if (mClearPending)  {
        // If a clear is pending, return a copy with only the current item.
        AutoPtr<IWebHistoryItem> webHistoryItem;
        GetCurrentItem((IWebHistoryItem**)&webHistoryItem);
        l-> AddHistoryItem(webHistoryItem.Get());
        *webBackForwardList = (IWebBackForwardList*)(l.Get());
        return NOERROR;
    }
    l -> mCurrentIndex = mCurrentIndex;
    int size = 0;
    GetSize(&size);
    for (int i = 0; i < size; i++)  {
        // Add a copy of each WebHistoryItem
        (l -> mArray).PushBack(mArray[i]);
    }
    *webBackForwardList = (IWebBackForwardList*)(l.Get());
    return NOERROR;
}

ECode CWebBackForwardList::SetCurrentIndex(
    /* [in] */ Int32 newIndex)
{
    mCurrentIndex = newIndex;
    if (mCallbackProxy != NULL) {
        AutoPtr<IWebHistoryItem> webHistoryItem;
        GetItemAtIndex(newIndex,(IWebHistoryItem**)&webHistoryItem);
        mCallbackProxy -> OnIndexChanged(webHistoryItem.Get(), newIndex);
    }
    return NOERROR;
}

ECode CWebBackForwardList::RestoreIndex(
    /* [in] */ Int32 nativeFrame,
    /* [in] */ Int32 index)
{//=0(virtual)
    return E_NOT_IMPLEMENTED;    
}

ECode CWebBackForwardList::NativeClose(
    /* [in] */ Int32 nativeFrame)
{//=0(virtual)
    return E_NOT_IMPLEMENTED;
}

