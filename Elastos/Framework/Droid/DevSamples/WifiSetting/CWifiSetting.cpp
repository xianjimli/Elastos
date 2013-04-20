
#include "CWifiSetting.h"
#include "Uri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>
#include <StringBuffer.h>
#include <utils/SystemClock.h>

using namespace Elastos::Core;

CWifiSetting::MyListener::MyListener(
    /* [in] */ CWifiSetting* host) :
    mHost(host),
    mRef(0)
{
}

PInterface CWifiSetting::MyListener::Probe(
    /* [in]  */ REIID riid)
{
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
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

UInt32 CWifiSetting::MyListener::AddRef()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is increased
    Int32 ref = atomic_inc(&mRef);
    // so we should increase ref before return
    return ++ref;
}

UInt32 CWifiSetting::MyListener::Release()
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

ECode CWifiSetting::MyListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IViewOnTouchListener*)this) {
        *pIID = EIID_IViewOnTouchListener;
    }
    else if (pObject == (IInterface*)(IViewOnKeyListener*)this) {
        *pIID = EIID_IViewOnKeyListener;
    }
    else if (pObject == (IInterface*)(IOnItemClickListener*)this) {
        *pIID = EIID_IOnItemClickListener;
    }
    else if (pObject == (IInterface*)(IViewOnClickListener*)this) {
        *pIID = EIID_IViewOnClickListener;
    }
    else if (pObject == (IInterface*)(IDialogInterfaceOnClickListener*)this) {
        *pIID = EIID_IDialogInterfaceOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CWifiSetting::MyListener::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    printf("OnTouch\n");
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String("中文"), (ICharSequence**)&csq);
    ITextView::Probe(v)->SetText(csq);

    if (result) {
        *result = FALSE;
    }

    return NOERROR;
}

ECode CWifiSetting::MyListener::OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id)
{
	printf("OnItemClick position = %d, id = %d\n", position, id);

    String itemText;
    AutoPtr<ITextView> textView = ITextView::Probe(view);
    assert(textView != NULL);

    AutoPtr<ICharSequence> text;
    textView->GetText((ICharSequence**) &text);
    assert(text != NULL);
    text->ToString(&itemText);
    printf("File=[%s], Line = [%d], FUNC = [%s], itemText=[%s]\n", __FILE__, __LINE__, __FUNCTION__, (const char*)itemText);

    mHost->ShowDialog(0);
    mHost->SetDialogTitle(text);

	return NOERROR;
}

ECode CWifiSetting::MyListener::OnClick(
        /* [in] */ IView* view)
{
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    if (view == mHost->mConnectButton.Get()) {
        printf("mConnectButton\n");
        //TODO Connect to wifi.
        mHost->DismissDialog(0);
    }
    else if (view == mHost->mCancelButton.Get()) {
        printf("mCancelButton\n");
        mHost->DismissDialog(0);
    }
    else if (view == mHost->mWifiStatusCheck.Get()) {
        //TODO Automaticly Scan the wifi hot point.
    }

    return NOERROR;
}

ECode CWifiSetting::MyListener::OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which)
{
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
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

ECode CWifiSetting::MyListener::OnKey(
    /* [in] */ IView * v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
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

ECode CWifiSetting::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    SetContentView(0x7f030001 /*main.xml*/);

    AutoPtr<IObjectContainer> strs;
    CParcelableObjectContainer::New((IObjectContainer**)&strs);

    AutoPtr<ICharSequence> cs;
    for (Int32 i=0; i<10; i++) {
        CStringWrapper::New(String(StringBuffer("item") + i), (ICharSequence**)&cs);
        strs->Add(cs);
        cs = NULL;
    }

    AutoPtr<IArrayAdapter> adapter;
    CArrayAdapter::New(this, 0x7f030000 /*list_item*/, strs, (IArrayAdapter**)&adapter);
    assert(adapter != NULL);

    AutoPtr<IView> view = FindViewById(0x7f050002 /*wifiitemContainer*/);
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

    //Init Wifi Windos.
    AutoPtr<IWindow> window = GetWindow();
    assert(window != NULL);
    window->GetLayoutInflater((ILayoutInflater**) &mInflater);
    assert(mInflater != NULL);

    mWifiStatusCheck = FindViewById(0x7f050001 /*wifiStatus*/);
    assert(mWifiStatusCheck != NULL);
    l = new MyListener(this);
    mWifiStatusCheck->SetOnClickListener((IViewOnClickListener*)l.Get());

    return NOERROR;
}

ECode CWifiSetting::OnStart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CWifiSetting::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CWifiSetting::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CWifiSetting::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CWifiSetting::OnDestroy()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CWifiSetting::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}

void CWifiSetting::SetDialogTitle(
    /* [in] */ ICharSequence* title)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    assert(title != NULL && mWifiDialog != NULL);
    mWifiDialog->SetTitle(title);
}

AutoPtr<IDialog> CWifiSetting::OnCreateDialog(
   /* [in] */ Int32 id)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    AutoPtr<IAlertDialogBuilder> builder;
    CAlertDialogBuilder::New(this, (IAlertDialogBuilder**)&builder);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("普通对话框"), (ICharSequence**)&cs);
    builder->SetTitleEx(cs);    //标题

    //User defined View.
    AutoPtr<IView> dialogView;
    mInflater->Inflate(0x7f030002, NULL, (IView**) &dialogView);
    assert(dialogView != NULL);

    dialogView->FindViewById(0x7f050009 /*connectButton*/, (IView**) &mConnectButton);
    assert(mConnectButton != NULL);
    AutoPtr<MyListener> l = new MyListener(this);
    mConnectButton->SetOnClickListener((IViewOnClickListener*)l.Get());

    dialogView->FindViewById(0x7f05000a /*cancelBtn*/, (IView**) &mCancelButton);
    assert(mCancelButton != NULL);
    l = new MyListener(this);
    mCancelButton->SetOnClickListener((IViewOnClickListener*)l.Get());

    builder->SetView(dialogView.Get());

    //创建Dialog对象
    builder->Create((IAlertDialog**)&mWifiDialog);

    return mWifiDialog;
}
