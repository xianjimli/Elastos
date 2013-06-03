
#include "webkit/CWebBackForwardList.h"

#include "webkit/DebugFlags.h"

ECode CWebBackForwardList::constructor(
        /* [in] */ ICallbackProxy* proxy)
{
    WebBackForwardList::Init(proxy);
}

ECode CWebBackForwardList::GetCurrentItem(
    /* [out] */ IWebHistoryItem ** item)
{
    VALIDATE_NOT_NULL(item);
    *item = WebBackForwardList::GetCurrentItem();
    return NOERROR;
}

ECode CWebBackForwardList::GetCurrentIndex(
    /* [out] */ Int32 * index)
{
    VALIDATE_NOT_NULL(index);
    *index = WebBackForwardList::GetCurrentIndex();
    return NOERROR;
}

ECode CWebBackForwardList::GetItemAtIndex(
    /* [in] */ Int32 index,
    /* [out] */ IWebHistoryItem ** item)
{
    VALIDATE_NOT_NULL(item);
    *item = WebBackForwardList::GetItemAtIndex(index);
    return NOERROR;
}

ECode CWebBackForwardList::GetSize(
    /* [out] */ Int32 * size)
{
    VALIDATE_NOT_NULL(size);
    *size = WebBackForwardList::GetSize();
    return NOERROR;
}

ECode CWebBackForwardList::Clone(
    /* [in] */ IWebBackForwardList** webBackForwardList)
{
    Mutex::Autolock lock(_m_syncLock);
    AutoPtr<CWebBackForwardList> l = new CWebBackForwardList();
    if (mClearPending)  {
        // If a clear is pending, return a copy with only the current item.
        AutoPtr<IWebHistoryItem> webHistoryItem;
        GetCurrentItem((IWebHistoryItem**)&webHistoryItem);
        l->AddHistoryItem(webHistoryItem.Get());
        *webBackForwardList = (IWebBackForwardList*)(l.Get());
        return NOERROR;
    }
    l->mCurrentIndex = mCurrentIndex;
    Int32 size = 0;
    GetSize(&size);
    for (Int32 i = 0; i < size; i++)  {
        // Add a copy of each WebHistoryItem
        (l->mArray).PushBack(mArray[i]);
    }
    *webBackForwardList = (IWebBackForwardList*)(l.Get());
    return NOERROR;
}