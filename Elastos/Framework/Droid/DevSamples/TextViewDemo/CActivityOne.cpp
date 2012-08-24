
#include "CActivityOne.h"
#include "CUri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>
#include <StringBuffer.h>

using namespace Elastos::Core;

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
    else if (riid == EIID_IOnItemClickListener) {
        return (IOnItemClickListener*)this;
    }
    else if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }
    else if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (IDialogInterfaceOnClickListener*)this;
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
    printf("OnTouch\n");
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String("中文"), (ICharSequence**)&csq);
    ITextView::Probe(v)->SetText(csq);

    if (result) {
        *result = false;;
    }

    return NOERROR;
}

ECode CActivityOne::MyListener::OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id)
{
	printf("OnItemClick position = %d, id = %d\n", position, id);
	return NOERROR;
}

ECode CActivityOne::MyListener::OnClick(
        /* [in] */ IView* view)
{
    if (view == mHost->mButton1.Get()) {
        printf("ok\n");
        mHost->DismissDialog(0);
    }
    else if (view == mHost->mButton2.Get()) {
        printf("cancel\n");
        mHost->DismissDialog(0);
    }
    else if (view == mHost->mButton3.Get()) {
        printf("neutral\n");
        mHost->DismissDialog(0);
    }
    else if (view == mHost->mDialogButton.Get()) {
        printf("Show Dilaog!\n");
        mHost->ShowDialog(0);
    }
    else if (view == mHost->mAnimationButton.Get()) {
        static count = 0;
        count = count % 4;
        if (count == 0) {
            mHost->mContent->StartAnimation(mHost->mAlphaAnimation);
        }
        else if (count == 1) {
            mHost->mContent->StartAnimation(mHost->mRotateAnimation);
        }
        else if (count == 2) {
            mHost->mContent->StartAnimation(mHost->mScaleAnimation);
        }
        else {
            mHost->mContent->StartAnimation(mHost->mTranslateAnimation);
        }
        count++;
    }
    return NOERROR;
}

ECode CActivityOne::MyListener::OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which)
{
    switch (which) {
    case DialogInterface_BUTTON_POSITIVE:
            printf("点击了确定按钮\n");
        break;
    case DialogInterface_BUTTON_NEGATIVE:
            printf("点击了取消按钮\n");
        break;
    case DialogInterface_BUTTON_NEUTRAL:
            printf("点击了中立按钮\n");
        break;
    default:
        break;
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
        *result = false;
    }

    return NOERROR;
}

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    printf("CActivityOne::OnCreate\n");
    SetContentView(0x7f030002);

    AutoPtr<IView> view = FindViewById(0x7f050007);
    assert(view != NULL);

    AutoPtr<IObjectContainer> strs;
    CParcelableObjectContainer::New((IObjectContainer**)&strs);

    AutoPtr<ICharSequence> cs;
    for (Int32 i=0; i<100; i++) {
        CStringWrapper::New(String(StringBuffer("item") + i), (ICharSequence**)&cs);
        strs->Add(cs);
        cs = NULL;
    }

    AutoPtr<IArrayAdapter> adapter;
    CArrayAdapter::New(this, 0x7f030001, strs, (IArrayAdapter**)&adapter);
    assert(adapter != NULL);

    view = FindViewById(0x7f050008);
    assert(view != NULL);

    AutoPtr<IListView> listView = IListView::Probe(view);
    listView->SetAdapter(adapter.Get());
    AutoPtr<IColorDrawable> drawable;
    CColorDrawable::New(0xFF0000FF, (IColorDrawable**)&drawable);
    assert(drawable != NULL);
    listView->SetDivider(drawable);
    listView->SetDividerHeight(1);

    AutoPtr<MyListener> l = new MyListener(this);
    listView->SetOnItemClickListener((IOnItemClickListener*)l.Get());

    mAnimationButton = FindViewById(0x7f050009);
    assert(mAnimationButton != NULL);

    mAnimationButton->SetOnClickListener((IViewOnClickListener*)l.Get());

    mDialogButton = FindViewById(0x7f05000a);
    assert(mDialogButton != NULL);

    mDialogButton->SetOnClickListener((IViewOnClickListener*)l.Get());

    AutoPtr<IViewParent> parent;
    view->GetParent((IViewParent**)&parent);
    mContent = IView::Probe(parent);

    CAlphaAnimation::New(0.3f, 1.0f, (IAnimation**)&mAlphaAnimation);
    mAlphaAnimation->SetDuration(3000);

    CRotateAnimation::New(0.0f, +350.0f, Animation_RELATIVE_TO_SELF,
        0.5f,Animation_RELATIVE_TO_SELF, 0.5f, (IAnimation**)&mRotateAnimation);
    mRotateAnimation->SetDuration(3000);

    CScaleAnimation::New(0.2f, 1.4f, 0.2f, 1.4f, Animation_RELATIVE_TO_SELF,
        0.5f, Animation_RELATIVE_TO_SELF, 0.5f, (IAnimation**)&mScaleAnimation);
    mScaleAnimation->SetDuration(3000);

    CTranslateAnimation::New(300.0f, -20.0f, -10.0f, 30.0f,
        (IAnimation**)&mTranslateAnimation);
    mTranslateAnimation->SetDuration(3000);

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

//AutoPtr<IDialog> CActivityOne::OnCreateDialog(
//    /* [in] */ Int32 id)
//{
//    AutoPtr<IAlertDialogBuilder> builder;
//    CAlertDialogBuilder::New(this, (IAlertDialogBuilder**)&builder);
//
//    AutoPtr<ICharSequence> cs;
//    CStringWrapper::New(String("普通对话框"), (ICharSequence**)&cs);
//    builder->SetTitleEx(cs);    //标题
//
//    cs = NULL;
//    CStringWrapper::New(String("这是一个普通对话框"), (ICharSequence**)&cs);
//    builder->SetMessageEx(cs);    //对话框显示内容
//
//      //设置按钮
//    AutoPtr<MyListener> l = new MyListener(this);
//
//    cs = NULL;
//    CStringWrapper::New(String("确定"), (ICharSequence**)&cs);
//    builder->SetPositiveButtonEx(cs, (IDialogInterfaceOnClickListener*)l.Get());
//
//    cs = NULL;
//    CStringWrapper::New(String("中立"), (ICharSequence**)&cs);
//      builder->SetNeutralButtonEx(cs, (IDialogInterfaceOnClickListener*)l.Get());
//
//    cs = NULL;
//    CStringWrapper::New(String("取消"), (ICharSequence**)&cs);
//      builder->SetNegativeButtonEx(cs, (IDialogInterfaceOnClickListener*)l.Get());
//
//      //创建Dialog对象
//      AutoPtr<IAlertDialog> dlg;
//      builder->Create((IAlertDialog**)&dlg);
//
//      return dlg;
//}

AutoPtr<IDialog> CActivityOne::OnCreateDialog(
    /* [in] */ Int32 id)
{
    AutoPtr<IDialog> dlg;
    CDialog::New(this, (IDialog**)&dlg);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("对话框"), (ICharSequence**)&cs);
    dlg->SetTitle(cs);
    dlg->SetCancelable(TRUE);
    dlg->SetContentView(0x7f030000);

    AutoPtr<IWindow> wnd;
    dlg->GetWindow((IWindow**)&wnd);
    wnd->SetTitleColor(0xFFFFFFFF);

    AutoPtr<IView> view;
    dlg->FindViewById(0x7f050002, (IView**)&view);
    assert(view != NULL);

    AutoPtr<ITextView> textView = ITextView::Probe(view);
    cs = NULL;
    CStringWrapper::New(String("这是一个对话框"), (ICharSequence**)&cs);
    textView->SetText(cs);    //对话框显示内容
    textView->SetTextColor(0xFFFFFFFF);

    dlg->FindViewById(0x7f050004, (IView**)&mButton1);
    assert(mButton1 != NULL);

    dlg->FindViewById(0x7f050006, (IView**)&mButton2);
    assert(mButton2 != NULL);

    dlg->FindViewById(0x7f050005, (IView**)&mButton3);
    assert(mButton3 != NULL);

    AutoPtr<MyListener> l = new MyListener(this);

    mButton1->SetOnClickListener((IViewOnClickListener*)l.Get());
    mButton2->SetOnClickListener((IViewOnClickListener*)l.Get());
    mButton3->SetOnClickListener((IViewOnClickListener*)l.Get());

    return dlg;
}
