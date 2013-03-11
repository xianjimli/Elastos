
#include "AppErrorDialog.h"

CString const AppErrorDialog::TAG = "AppErrorDialog";
Int32 const AppErrorDialog::AppErrorDialog_FORCE_QUIT = 0;
Int32 const AppErrorDialog::AppErrorDialog_FORCE_QUIT_AND_REPORT = 1;
Int64 const AppErrorDialog::AppErrorDialog_DISMISS_TIMEOUT = 1000 * 60 * 5;

AppErrorDialog::AppErrorDialog(
    /* [in] */ IContext* context,
    /* [in] */ AppErrorResult* result,
    /* [in] */ ProcessRecord* app)
    : BaseErrorDialog(context)
    , mResult(result)
    , mProc(app)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);

    AutoPtr<ICharSequence> name;
    // if ((app->mCapList.GetSize() == 1) &&
    //         (name=context.getPackageManager().getApplicationLabel(app.info)) != null) {
    //     setMessage(res.getString(
    //             com.android.internal.R.string.aerr_application,
    //             name.toString(), app.info.processName));
    // }
    // else {
    res->GetString(0x010402fc/*com.android.internal.R.string.aerr_process*/,
            &(app->mProcessName));
    AutoPtr<ICharSequence> name;
    CStringWrapper::New(app->mProcessName, (ICharSequence**)&name);
    SetMessage(cs);
    //}

    SetCancelable(FALSE);

    AutoPtr<ICharSequence> text;
    res->GetText(0x01040302/*com.android.internal.R.string.force_close*/,
            (ICharSequence**)&text);
    PositiveButtonOnClickListener* listener = new PositiveButtonOnClickListener(this);
    SetButton(DialogInterface_BUTTON_POSITIVE, text,
            (IDialogInterfaceOnClickListener*)listener/*,mHandler.obtainMessage(FORCE_QUIT)*/);

    // if (app.errorReportReceiver != null) {
    //     setButton(DialogInterface.BUTTON_NEGATIVE,
    //             res.getText(com.android.internal.R.string.report),
    //             mHandler.obtainMessage(FORCE_QUIT_AND_REPORT));
    // }

    AutoPtr<ICharSequence> title;
    res->GetText(0x010402fa/*com.android.internal.R.string.aerr_title*/,
            (ICharSequence**)&title);
    SetTitle(title);
    GetWindow()->AddFlags(WindowManagerLayoutParams_FLAG_SYSTEM_ERROR);
    String processName;
    app->mInfo->GetProcessName(&processName);
    GetWindow()->SetTitle(String("Application Error: ") + processName);

    // After the timeout, pretend the user clicked the quit button
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(AppErrorDialog_FORCE_QUIT);
    void (STDCALL AppErrorDialog::*pHandlerFunc)(Int32);
    pHandlerFunc = &BaseErrorDialog::HandleOnForceQuitMessage;
    mApartment->PostCppCallbackDelayed((Handle32)this, pHandlerFunc, params, 0,
            AppErrorDialog_DISMISS_TIMEOUT);
}

void AppErrorDialog::HandleOnForceQuitMessage(
    /* [in] */ Int32 type)
{
    // {
    //     Mutex::Autolock lock(mProcLock);
    //     if (mProc != NULL && mProc->mCrashDialog == AppErrorDialog.this) {
    //         mProc.crashDialog = null;
    //     }
    // }
    mResult->SetResult(type);

    // If this is a timeout we won't be automatically closed, so go
    // ahead and explicitly dismiss ourselves just in case.
    Dismiss();
}

AppErrorDialog::PositiveButtonOnClickListener::PositiveButtonOnClickListener(
    /* [in] */ AppErrorDialog* host)
    : mHost(host)
{}

PInterface AppErrorDialog::PositiveButtonOnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (PInterface)(IDialogInterfaceOnClickListener*)this;
    }

    return NULL;
}

UInt32 AppErrorDialog::PositiveButtonOnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AppErrorDialog::PositiveButtonOnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode AppErrorDialog::PositiveButtonOnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AppErrorDialog::PositiveButtonOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mHost->HandleOnForceQuitMessage(FORCE_QUIT);
}
