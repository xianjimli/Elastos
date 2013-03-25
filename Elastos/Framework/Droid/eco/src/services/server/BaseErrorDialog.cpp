
#include "BaseErrorDialog.h"
#include "os/CApartment.h"

IALERTDIALOG_METHODS_IMPL(BaseErrorDialog, AlertDialog, AlertDialog);

IDIALOG_METHODS_IMPL(BaseErrorDialog, AlertDialog, AlertDialog);

IWINDOWCALLBACK_METHODS_IMPL(BaseErrorDialog, AlertDialog, AlertDialog);

IKeyEventCallback_METHODS_IMPL(BaseErrorDialog, AlertDialog, AlertDialog);

BaseErrorDialog::BaseErrorDialog(
    /* [in] */ IContext* context)
    : AlertDialog(context, 0x010300a8/*com.android.internal.R.style.Theme_Dialog_AppError*/)
    , mConsuming(TRUE)
{
    GetWindow()->SetType(WindowManagerLayoutParams_TYPE_SYSTEM_ALERT);
    GetWindow()->SetFlags(WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM,
            WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM);
    GetWindow()->SetTitle(String("Error Dialog"));
    SetIcon(0x01080027/*R.drawable.ic_dialog_alert*/);

    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&h))
            && (mApartment != NULL));
}

PInterface BaseErrorDialog::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IAlertDialog) {
        return (PInterface)(IAlertDialog*)this;
    }
    else if (riid == EIID_IDialog) {
        return (PInterface)(IDialog*)this;
    }
    else if (riid == EIID_IDialogInterface) {
        return (PInterface)(IDialogInterface*)this;
    }
    else if (riid == EIID_IWindowCallback) {
        return (PInterface)(IWindowCallback*)this;
    }
    if (riid == EIID_IKeyEventCallback) {
        return (PInterface)(IKeyEventCallback*)this;
    }
    else if (riid == EIID_IViewOnCreateContextMenuListener) {
       return (PInterface)(IViewOnCreateContextMenuListener*)this;
    }

    return NULL;
}

UInt32 BaseErrorDialog::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BaseErrorDialog::Release()
{
    return ElRefBase::Release();
}

ECode BaseErrorDialog::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void BaseErrorDialog::OnStart()
{
    AlertDialog::OnStart();
    SetEnabled(FALSE);
    void (STDCALL BaseErrorDialog::*pHandlerFunc)();
    pHandlerFunc = &BaseErrorDialog::HandleOnStartMessage;
    mApartment->PostCppCallbackDelayed((Handle32)this, pHandlerFunc, NULL, 0, 1000);
}

void BaseErrorDialog::HandleOnStartMessage()
{
    mConsuming = FALSE;
    SetEnabled(TRUE);
}

Boolean BaseErrorDialog::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    if (mConsuming) {
        //Slog.i(TAG, "Consuming: " + event);
        return TRUE;
    }
    //Slog.i(TAG, "Dispatching: " + event);
    return AlertDialog::DispatchKeyEvent(event);
}

ECode BaseErrorDialog::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return AlertDialog::OnCreateContextMenu(menu, v, menuInfo);
}

void BaseErrorDialog::SetEnabled(
    /* [in] */ Boolean enabled)
{
    AutoPtr<IButton> b = (IButton*)FindViewById(0x01020019/*R.id.button1*/);
    if (b != NULL) {
        b->SetEnabled(enabled);
    }
    b = (IButton*)FindViewById(0x0102001a/*R.id.button2*/);
    if (b != NULL) {
        b->SetEnabled(enabled);
    }
    b = (IButton*)FindViewById(0x0102001b/*R.id.button3*/);
    if (b != NULL) {
        b->SetEnabled(enabled);
    }
}
