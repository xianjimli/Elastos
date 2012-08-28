
#include "view/ViewTreeObserver.h"

const Int32 ViewTreeObserver::InternalInsetsInfo::TOUCHABLE_INSETS_FRAME;
const Int32 ViewTreeObserver::InternalInsetsInfo::TOUCHABLE_INSETS_CONTENT;
const Int32 ViewTreeObserver::InternalInsetsInfo::TOUCHABLE_INSETS_VISIBLE;

ViewTreeObserver::InternalInsetsInfo::InternalInsetsInfo()
{
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mContentInsets)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mVisibleInsets)));
}

void ViewTreeObserver::InternalInsetsInfo::SetTouchableInsets(
    /* [in] */ Int32 val)
{
    mTouchableInsets = val;
}

Int32 ViewTreeObserver::InternalInsetsInfo::GetTouchableInsets()
{
    return mTouchableInsets;
}

void ViewTreeObserver::InternalInsetsInfo::Reset()
{
    mContentInsets->Set(0, 0, 0, 0);
    mVisibleInsets->Set(0, 0, 0, 0);
    mTouchableInsets = TOUCHABLE_INSETS_FRAME;
}

Boolean ViewTreeObserver::InternalInsetsInfo::Equals(
    /* [in] */ InternalInsetsInfo* other)
{
    if (other == NULL) {
        return FALSE;
    }

    if (!mContentInsets->Equals(other->mContentInsets)) {
        return FALSE;
    }

    if (!mVisibleInsets->Equals(other->mVisibleInsets)) {
        return FALSE;
    }

    return mTouchableInsets == other->mTouchableInsets;
}

void ViewTreeObserver::InternalInsetsInfo::Set(
    /* [in] */ InternalInsetsInfo* other)
{
    mContentInsets->SetEx(other->mContentInsets);
    mVisibleInsets->SetEx(other->mVisibleInsets);
    mTouchableInsets = other->mTouchableInsets;
}

ViewTreeObserver::ViewTreeObserver()
{}

PInterface ViewTreeObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IViewTreeObserver*)this;
    }
    else if (riid == EIID_IViewTreeObserver) {
        return (IViewTreeObserver*)this;
    }

    return NULL;
}

UInt32 ViewTreeObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewTreeObserver::Release()
{
    return ElRefBase::Release();
}

ECode ViewTreeObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnScrollChangedListener(
    /* [in] */ IOnScrollChangedListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveOnScrollChangedListener(
    /* [in] */ IOnScrollChangedListener* victim)
{
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnTouchModeChangeListener(
    /* [in] */ IOnTouchModeChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveOnTouchModeChangeListener(
    /* [in] */ IOnTouchModeChangeListener* victim)
{
    return E_NOT_IMPLEMENTED;
}

