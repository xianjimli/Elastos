
#include "CActivityOne.h"
#include "CUri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>

#define VALIDATE_NOT_NULL(x) if (!(x)) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }

CActivityOne::MyListener::MyListener(
    /* [in] */ CActivityOne* host) :
    mHost(host),
    mRef(0)
{
}

PInterface CActivityOne::MyListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return NULL;
}

UInt32 CActivityOne::MyListener::AddRef()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is increased
    Int32 ref = atomic_inc(&mRef);
    // so we should increase ref before return
    return ++ref;
}

UInt32 CActivityOne::MyListener::Release()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is decreased
    Int32 ref = atomic_dec(&mRef);
    // so we should decrease ref
    if (--ref == 0) {
        delete this;
    }
    assert(ref >= 0);
    return ref;
}

ECode CActivityOne::MyListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IViewOnTouchListener*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CActivityOne::MyListener::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

ECode CActivityOne::MyListener::OnKey(
    /* [in] */ IView * v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    return NOERROR;
}

Int32 CActivityOne::ImageAdapter::mImage[] = {
    0x7f020001,
    0x7f020002,
    0x7f020003,
    0x7f020004,
    0x7f020005
};

Int32 CActivityOne::ImageAdapter::mImageLength = 5;

CActivityOne::ImageAdapter::ImageAdapter(
    /* [in] */ IContext* c)
    : mRef(0)
{
    mContext = c;
}

PInterface CActivityOne::ImageAdapter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBaseAdapter*)this;
    }
    else if (riid == EIID_IBaseAdapter) {
        return (IBaseAdapter*)this;
    }
    else if (riid == EIID_ISpinnerAdapter) {
        return (ISpinnerAdapter*)this;
    }

    return NULL;
}

UInt32 CActivityOne::ImageAdapter::AddRef()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is increased
    Int32 ref = atomic_inc(&mRef);
    // so we should increase ref before return
    return ++ref;
}

UInt32 CActivityOne::ImageAdapter::Release()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is decreased
    Int32 ref = atomic_dec(&mRef);
    // so we should decrease ref
    if (--ref == 0) {
        delete this;
    }
    assert(ref >= 0);
    return ref;
}

ECode CActivityOne::ImageAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IBaseAdapter*)this) {
        *pIID = EIID_IBaseAdapter;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CActivityOne::ImageAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityOne::ImageAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityOne::ImageAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mImageLength;
    return NOERROR;
}

ECode CActivityOne::ImageAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityOne::ImageAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId);
    *itemId = position;
    return NOERROR;
}

ECode CActivityOne::ImageAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds);
    *hasStableIds = FALSE;

    return NOERROR;
}

ECode CActivityOne::ImageAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* pConvertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    printf("CActivityOne::ImageAdapter::GetView\n");
    VALIDATE_NOT_NULL(view);
    AutoPtr<IImageView> i;
    CImageView::New(mContext, (IImageView**)&i);
    i->SetImageResource(mImage[position]);
    i->SetScaleType(ImageViewScaleType_FIT_CENTER);
    AutoPtr<IGalleryLayoutParams> lp;
    CGalleryLayoutParams::New(
        ViewGroupLayoutParams_FILL_PARENT, ViewGroupLayoutParams_FILL_PARENT,
        (IGalleryLayoutParams**)&lp);
    i->SetLayoutParams(lp.Get());

    *view = i;
    if (*view) {
        (*view)->AddRef();
    }

    return NOERROR;
}

ECode CActivityOne::ImageAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32 * viewType)
{
    VALIDATE_NOT_NULL(viewType);
    *viewType = 0;
    return NOERROR;
}

ECode CActivityOne::ImageAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = 1;

    return NOERROR;
}

ECode CActivityOne::ImageAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    *isEmpty = mImageLength == 0;
    return NOERROR;
}

ECode CActivityOne::ImageAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = TRUE;

    return NOERROR;
}

ECode CActivityOne::ImageAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = TRUE;

    return NOERROR;
}

ECode CActivityOne::ImageAdapter::NotifyDataSetChanged()
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityOne::ImageAdapter::NotifyDataSetInvalidated()
{
    return E_NOT_IMPLEMENTED;
}

CARAPI CActivityOne::ImageAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f030000);

    AutoPtr<IView> view = FindViewById(0x7f050000);
    assert(view != NULL);


    AutoPtr<IGallery> gallery = (IGallery*)view.Get();
    AutoPtr<IBaseAdapter> adapter = new ImageAdapter(
        (IContext*)this->Probe(EIID_IContext));
    gallery->SetAdapter(adapter.Get());

    //AutoPtr<MyListener> l = new MyListener(this);

    return NOERROR;
}

ECode CActivityOne::OnStart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnDestroy()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}
