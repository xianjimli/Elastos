
#include "CActivityOne.h"
#include "Uri.h"
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

ECode CActivityOne::MyListener::OnClick(
    /* [in] */ IView* v)
{
    AutoPtr<IPopupWindow> window = mHost->CreatePopupWindow((IContext*)mHost);
    window->ShowAtLocation(v, Gravity_CENTER, 0, 0);

    return NOERROR;
}

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f030000);

    AutoPtr<IView> view = FindViewById(0x7f050000);
    assert(view != NULL);


    AutoPtr<MyListener> l = new MyListener(this);
    view->SetOnClickListener(l.Get());

    AutoPtr<ITypefaceFactory> tf;
    CTypefaceFactory::AcquireSingleton((ITypefaceFactory**)&tf);
    AutoPtr<ITypeface> t;
    tf->Create(String(NULL), Typeface_BOLD, (ITypeface**)&t);

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

//创建一个包含自定义view的PopupWindow
AutoPtr<IPopupWindow> CActivityOne::CreatePopupWindow(
    /* [in] */ IContext* cx)
{
    AutoPtr<IPopupWindow> window;
    CPopupWindow::New(cx, (IPopupWindow**)&window);

    AutoPtr<ITextView> contentView;
    CTextView::New(cx, (ITextView**)&contentView);
    contentView->SetGravity(Gravity_CENTER);

    contentView->SetBackgroundColor(0xffffff00);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("PopupWindow"), (ICharSequence**)&cs);
    contentView->SetText(cs);
    contentView->SetWidth(100);
    contentView->SetHeight(50);

    AutoPtr<IColorDrawable> drawable;
    CColorDrawable::New(0xffffffff, (IColorDrawable**)&drawable);
    window->SetBackgroundDrawable(drawable.Get());

    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        ViewGroupLayoutParams_WRAP_CONTENT, ViewGroupLayoutParams_WRAP_CONTENT,
        (IViewGroupLayoutParams**)&params);
    contentView->SetLayoutParams(params);

    //window.setAnimationStyle(R.style.AnimationFade);

    window->SetWidth(200);
    window->SetHeight(200);

    window->SetContentView(contentView);
    window->SetTouchable(TRUE);
    window->SetFocusable(TRUE);
    window->SetOutsideTouchable(TRUE);

    return window;
}
