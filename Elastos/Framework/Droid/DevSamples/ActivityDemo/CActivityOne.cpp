
#include "CActivityOne.h"
#include "CUri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>

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
    if (v != mHost->FindViewById(0x7f050000)) {
        printf("The event is not dispatched to this view\n");
    }

    printf("ImageView is dispatched a MotionEvent\n");

    IImageView* imageView = (IImageView*)v->Probe(EIID_IImageView);
    Int32 action;
    event->GetAction(&action);
    if (action == MotionEvent_ACTION_DOWN) {
        imageView->SetImageDrawable(mHost->mDrawable2);
    }
    else if (action == MotionEvent_ACTION_UP) {
        imageView->SetImageDrawable(mHost->mDrawable1);
    }

    if (result) {
        *result = TRUE;
    }

    return NOERROR;
}

ECode CActivityOne::MyListener::OnKey(
    /* [in] */ IView * v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    if (v != mHost->FindViewById(0x7f050000)) {
        printf("The event is not dispatched to this view\n");
    }

    Int32 action;
    event->GetAction(&action);
    Char16 label;
    event->GetDisplayLabel(&label);

    if (action == KeyEvent_ACTION_DOWN) {
        printf("Key '%c' is down on ImageView\n", (char)label);
    }
    else if (action == KeyEvent_ACTION_UP) {
        printf("Key '%c' is up on ImageView\n", (char)label);
    }

    if (result) {
        *result = TRUE;
    }

    return NOERROR;
}

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f030000);

    AutoPtr<IView> view = FindViewById(0x7f050000);
    assert(view != NULL);

    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    assert(res != NULL);

    // Get string resource.
    String str;
    Int32 id = 0x7f040001; // TODO: set id here.
    res->GetString(id, &str);
    assert(!str.IsNull());
    printf("<%s, %d>string gotten, %s\n", __FILE__, __LINE__, (const char*)str);

    res->GetDrawable(0x7f020003, (IDrawable**)&mDrawable1);
    assert(mDrawable1 != NULL);

    res->GetDrawable(0x7f020000, (IDrawable**)&mDrawable2);
    assert(mDrawable1 != NULL);

    AutoPtr<IImageView> imageView = (IImageView*)view->Probe(EIID_IImageView);
    imageView->SetImageDrawable((IDrawable*)mDrawable1);

    imageView->SetFocusable(TRUE);
    imageView->SetFocusableInTouchMode(TRUE);

    AutoPtr<MyListener> l = new MyListener(this);
    imageView->SetOnTouchListener(l.Get());
    imageView->SetOnKeyListener(l.Get());

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
