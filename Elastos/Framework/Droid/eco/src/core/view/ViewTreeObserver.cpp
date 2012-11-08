
#include "view/ViewTreeObserver.h"
#ifdef _FRAMEWORK_CORE
#include "graphics/CRect.h"
#endif

const Int32 ViewTreeObserver::InternalInsetsInfo::TOUCHABLE_INSETS_FRAME;
const Int32 ViewTreeObserver::InternalInsetsInfo::TOUCHABLE_INSETS_CONTENT;
const Int32 ViewTreeObserver::InternalInsetsInfo::TOUCHABLE_INSETS_VISIBLE;

ViewTreeObserver::InternalInsetsInfo::InternalInsetsInfo()
{
    assert(SUCCEEDED(CRect::New((IRect**)&mContentInsets)));
    assert(SUCCEEDED(CRect::New((IRect**)&mVisibleInsets)));
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

    Boolean isEqual;
    if (mContentInsets->Equals(other->mContentInsets, &isEqual), !isEqual) {
        return FALSE;
    }

    if (mVisibleInsets->Equals(other->mVisibleInsets, &isEqual), !isEqual) {
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
	: mAlive(TRUE)
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
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveOnScrollChangedListener(
    /* [in] */ IOnScrollChangedListener* victim)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnTouchModeChangeListener(
    /* [in] */ IOnTouchModeChangeListener* listener)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveOnTouchModeChangeListener(
    /* [in] */ IOnTouchModeChangeListener* victim)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::Merge(
    /* [in] */ IViewTreeObserver* observerObj)
{
    ViewTreeObserver* observer = (ViewTreeObserver*)observerObj;

    // if (observer.mOnGlobalFocusListeners != NULL) {
    //     if (mOnGlobalFocusListeners != NULL) {
    //         mOnGlobalFocusListeners.addAll(observer.mOnGlobalFocusListeners);
    //     } else {
    //         mOnGlobalFocusListeners = observer.mOnGlobalFocusListeners;
    //     }
    // }

    // if (observer.mOnGlobalLayoutListeners != NULL) {
    //     if (mOnGlobalLayoutListeners != NULL) {
    //         mOnGlobalLayoutListeners.addAll(observer.mOnGlobalLayoutListeners);
    //     } else {
    //         mOnGlobalLayoutListeners = observer.mOnGlobalLayoutListeners;
    //     }
    // }

    if (observer->mOnPreDrawListeners.GetSize() > 0) {
        // if (mOnPreDrawListeners != NULL) {
        //     //mOnPreDrawListeners.addall(observer->mOnPreDrawListeners);
        // } else {
        //     mOnPreDrawListeners = observer->mOnPreDrawListeners;
        // }
        List<AutoPtr<IOnPreDrawListener> >::Iterator iter = observer->mOnPreDrawListeners.Begin();
        for (; iter != observer->mOnPreDrawListeners.End(); ++iter) {
            AutoPtr<IOnPreDrawListener> item = (AutoPtr<IOnPreDrawListener>)(*iter);

            mOnPreDrawListeners.PushBack(item);
        }
    }

    // if (observer.mOnTouchModeChangeListeners != NULL) {
    //     if (mOnTouchModeChangeListeners != NULL) {
    //         mOnTouchModeChangeListeners.addAll(observer.mOnTouchModeChangeListeners);
    //     } else {
    //         mOnTouchModeChangeListeners = observer.mOnTouchModeChangeListeners;
    //     }
    // }

    // if (observer.mOnComputeInternalInsetsListeners != NULL) {
    //     if (mOnComputeInternalInsetsListeners != NULL) {
    //         mOnComputeInternalInsetsListeners.addAll(observer.mOnComputeInternalInsetsListeners);
    //     } else {
    //         mOnComputeInternalInsetsListeners = observer.mOnComputeInternalInsetsListeners;
    //     }
    // }

    observer->Kill();

    return NOERROR;
}

ECode ViewTreeObserver::AddOnGlobalFocusChangeListener(
    /* [in] */ IOnGlobalFocusChangeListener* listener)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveOnGlobalFocusChangeListener(
    /* [in] */ IOnGlobalFocusChangeListener* victim)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnGlobalLayoutListener(
    /* [in] */ IOnGlobalLayoutListener* listener)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveGlobalOnLayoutListener(
    /* [in] */ IOnGlobalLayoutListener*  victim)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::IsAlive(
    /* [out] */ Boolean* alive)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::DispatchOnGlobalFocusChange(
    /* [in] */ IView* oldFocus,
    /* [in] */ IView* newFocus)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::DispatchOnGlobalLayout()
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::DispatchOnPreDraw(
    /* [out] */ Boolean* cancelDraw)
{
    CheckIsAlive();

    *cancelDraw = FALSE;
    ///final CopyOnWriteArrayList<OnPreDrawListener> listeners = mOnPreDrawListeners;
    if (mOnPreDrawListeners.GetSize() > 0) {
        // for (OnPreDrawListener listener : listeners) {
        //     *cancelDraw |= !listener.onPreDraw();
        // }

        List<AutoPtr<IOnPreDrawListener> >::Iterator iter = mOnPreDrawListeners.Begin();
        for (; iter != mOnPreDrawListeners.End(); ++iter) {
            AutoPtr<IOnPreDrawListener> listener = (AutoPtr<IOnPreDrawListener>)(*iter);

            Boolean isPreDraw = FALSE;
            listener->OnPreDraw(&isPreDraw);
            *cancelDraw |= !isPreDraw;
        }
    }

    return NOERROR;
}

ECode ViewTreeObserver::DispatchOnTouchModeChanged(
    /* [in] */ Boolean inTouchMode)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::DispatchOnScrollChanged()
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::HasComputeInternalInsetsListeners(
    /* [out] */ Boolean* has)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnPreDrawListener(
        /* [in] */ IOnPreDrawListener* listener)
{
    CheckIsAlive();
    mOnPreDrawListeners.PushBack(listener);

    return NOERROR;
}

ECode ViewTreeObserver::RemoveOnPreDrawListener(
        /* [in] */ IOnPreDrawListener* victim)
{
    CheckIsAlive();
    mOnPreDrawListeners.Remove(victim);

    return NOERROR;
}
