
#include "FactoryErrorDialog.h"

FactoryErrorDialog::FactoryErrorDialog(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* msg)
    : BaseErrorDialog(context)
{
    SetCancelable(FALSE);
    AutoPtr<ICharSequence> title;
    res->GetText(0x010402b5/*com.android.internal.R.string.factorytest_failed*/,
            (ICharSequence**)&title);
    SetTitle(title);
    SetMessage(msg);

    AutoPtr<ICharSequence> text;
    context->GetText(0x010402b8/*com.android.internal.R.string.factorytest_reboot*/,
            (ICharSequence**)&text);
    PositiveButtonOnClickListener* listener = new PositiveButtonOnClickListener(this);
    SetButton(DialogInterface_BUTTON_POSITIVE, text,
            (IDialogInterfaceOnClickListener*)listener/*mHandler.obtainMessage(0)*/);
    GetWindow()->SetTitle(String("Factory Error"));
}

void AppNotRespondingDialog::OnStop()
{}

void FactoryErrorDialog::HandleOnClickMessage()
{
    return E_RUNTIME_EXCEPTION;
}

FactoryErrorDialog::PositiveButtonOnClickListener::PositiveButtonOnClickListener(
    /* [in] */ FactoryErrorDialog* host)
    : mHost(host)
{}

PInterface FactoryErrorDialog::PositiveButtonOnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (PInterface)(IDialogInterfaceOnClickListener*)this;
    }

    return NULL;
}

UInt32 FactoryErrorDialog::PositiveButtonOnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FactoryErrorDialog::PositiveButtonOnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode FactoryErrorDialog::PositiveButtonOnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode FactoryErrorDialog::PositiveButtonOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mHost->HandleOnClickMessage();
}
