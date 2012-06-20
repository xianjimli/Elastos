
#include "FBReaderDef.h"
#include "CMainActivity.h"
#include "CUri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>

CMainActivity::MyListener::MyListener(
    /* [in] */ CMainActivity* host) :
    mHost(host),
    mRef(0)
{
}

PInterface CMainActivity::MyListener::Probe(
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

UInt32 CMainActivity::MyListener::AddRef()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is increased
    Int32 ref = atomic_inc(&mRef);
    // so we should increase ref before return
    return ++ref;
}

UInt32 CMainActivity::MyListener::Release()
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

ECode CMainActivity::MyListener::GetInterfaceID(
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

ECode CMainActivity::MyListener::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    printf("OnTouch\n");
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String::Duplicate("中文"), (ICharSequence**)&csq);
    ITextView::Probe(v)->SetText(csq);

    if (result) {
        *result = TRUE;
    }

    return NOERROR;
}

ECode CMainActivity::MyListener::OnKey(
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

ECode CMainActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f020000);

    AutoPtr<IView> view = FindViewById(0x7f040000);
    assert(view != NULL);

    AutoPtr<ITextView> textView = (ITextView*)view->Probe(EIID_ITextView);

    textView->SetFocusable(TRUE);
    textView->SetFocusableInTouchMode(TRUE);

    AutoPtr<MyListener> l = new MyListener(this);
    textView->SetOnTouchListener(l.Get());
    textView->SetOnKeyListener(l.Get());

printf("==== File: %s, Line: %d ==== \n", __FILE__, __LINE__);
    FAIL_RETURN(CBook::New(310, 445, (IBook**)&mBook));
printf("==== File: %s, Line: %d ==== \n", __FILE__, __LINE__);

    String title;
    mBook->GetTitle(&title);

    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(title, (ICharSequence**)&csq);
    textView->SetText(csq);

    return NOERROR;
}

ECode CMainActivity::OnStart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnDestroy()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}
