
#include "StrictModeViolationDialog.h"

CString const StrictModeViolationDialog::TAG = "StrictModeViolationDialog";
Int32 const StrictModeViolationDialog::StrictModeViolationDialog_ACTION_OK = 0;
Int32 const StrictModeViolationDialog::StrictModeViolationDialog_ACTION_OK_AND_REPORT = 1;
Int64 const StrictModeViolationDialog::StrictModeViolationDialog_DISMISS_TIMEOUT = 1000 * 60 * 1;

StrictModeViolationDialog::StrictModeViolationDialog(
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
    //             com.android.internal.R.string.smv_application,
    //             name.toString(), app.info.processName));
    // }
    // else {
    res->GetString(0x01040309/*com.android.internal.R.string.smv_process*/,
            &(app->mProcessName));
    AutoPtr<ICharSequence> name;
    CStringWrapper::New(app->mProcessName, (ICharSequence**)&name);
    SetMessage(cs);
    //}

    SetCancelable(FALSE);

    AutoPtr<ICharSequence> text;
    res->GetText(0x0104033e/*com.android.internal.R.string.dlg_ok*/,
            (ICharSequence**)&text);
    PositiveButtonOnClickListener* listener = new PositiveButtonOnClickListener(this);
    SetButton(DialogInterface_BUTTON_POSITIVE, text,
            (IDialogInterfaceOnClickListener*)listener/*,mHandler.obtainMessage(ACTION_OK)*/);

    // if (app.errorReportReceiver != null) {
    //     setButton(DialogInterface.BUTTON_NEGATIVE,
    //             res.getText(com.android.internal.R.string.report),
    //             mHandler.obtainMessage(ACTION_OK_AND_REPORT));
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
    params->WriteInt32(StrictModeViolationDialog_ACTION_OK);
    void (STDCALL StrictModeViolationDialog::*pHandlerFunc)(Int32);
    pHandlerFunc = &BaseErrorDialog::HandleOnActionOkMessage;
    mApartment->PostCppCallbackDelayed((Handle32)this, pHandlerFunc, params, 0,
            StrictModeViolationDialog_DISMISS_TIMEOUT);
}

void StrictModeViolationDialog::HandleOnActionOkMessage(
    /* [in] */ Int32 type)
{
    // {
    //     Mutex::Autolock lock(mProcLock);
    //     if (mProc != NULL && mProc->mCrashDialog == StrictModeViolationDialog.this) {
    //         mProc.crashDialog = null;
    //     }
    // }
    mResult->SetResult(type);

    // If this is a timeout we won't be automatically closed, so go
    // ahead and explicitly dismiss ourselves just in case.
    Dismiss();
}

StrictModeViolationDialog::PositiveButtonOnClickListener::PositiveButtonOnClickListener(
    /* [in] */ StrictModeViolationDialog* host)
    : mHost(host)
{}

PInterface StrictModeViolationDialog::PositiveButtonOnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (PInterface)(IDialogInterfaceOnClickListener*)this;
    }

    return NULL;
}

UInt32 StrictModeViolationDialog::PositiveButtonOnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 StrictModeViolationDialog::PositiveButtonOnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode StrictModeViolationDialog::PositiveButtonOnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode StrictModeViolationDialog::PositiveButtonOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mHost->HandleOnActionOkMessage(StrictModeViolationDialog_ACTION_OK);
}
