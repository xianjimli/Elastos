
#include "AppNotRespondingDialog.h"
#include <elastos/System.h>

CString const AppNotRespondingDialog::TAG = "AppNotRespondingDialog";
Int32 const AppNotRespondingDialog::AppNotRespondingDialog_FORCE_CLOSE = 1;
Int32 const AppNotRespondingDialog::AppNotRespondingDialog_WAIT = 2;
Int64 const AppNotRespondingDialog::AppNotRespondingDialog_WAIT_AND_REPORT = 3;

AppNotRespondingDialog::AppNotRespondingDialog(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ IContext* context,
    /* [in] */ ProcessRecord* app,
    /* [in] */ CActivityRecord* activity)
    : BaseErrorDialog(context)
    , mService(service)
    , mProc(app)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);

    SetCancelable(FALSE);

    Int32 resid;
    AutoPtr<ICharSequence> name1;
    // if (activity != NULL) {
    //     activity->mInfo->LoadLabel(context.getPackageManager(), (ICharSequence**)&name1);
    // }

    AutoPtr<ICharSequence> name2;
    // if ((app.pkgList.size() == 1) &&
    //         (name2=context.getPackageManager().getApplicationLabel(app.info)) != null) {
    //     if (name1 != null) {
    //         resid = com.android.internal.R.string.anr_activity_application;
    //     } else {
    //         name1 = name2;
    //         name2 = app.processName;
    //         resid = com.android.internal.R.string.anr_application_process;
    //     }
    // }
    // else {
    if (name1 != NULL) {
        CStringWrapper::New(app->mProcessName, (ICharSequence**)&name2);
        resid = 0x010402ff;/*com.android.internal.R.string.anr_activity_process*/
    }
    else {
        CStringWrapper::New(app->mProcessName, (ICharSequence**)&name1);
        resid = 0x01040301;/*com.android.internal.R.string.anr_process*/
    }
    //}

    // SetMessage(name2 != null
    //         ? res.getString(resid, name1.toString(), name2.toString())
    //         : res.getString(resid, name1.toString()));

    AutoPtr<ICharSequence> text1;
    res->GetText(0x01040302/*com.android.internal.R.string.force_close*/,
            (ICharSequence**)&text1);
    PositiveButtonOnClickListener* listener1 = new PositiveButtonOnClickListener(this);
    SetButton(DialogInterface_BUTTON_POSITIVE, text1,
            (IDialogInterfaceOnClickListener*)listener1/*,mHandler.obtainMessage(FORCE_CLOSE)*/);

    AutoPtr<ICharSequence> text2;
    res->GetText(0x01040304/*com.android.internal.R.string.wait*/,
            (ICharSequence**)&text2);
    NeutralButtonOnClickListener* listener2 = new NeutralButtonOnClickListener(this);
    SetButton(DialogInterface_BUTTON_NEUTRAL, text2,
            (IDialogInterfaceOnClickListener*)listener2/*mHandler.obtainMessage(WAIT)*/);

    // if (app.errorReportReceiver != null) {
    //     setButton(DialogInterface.BUTTON_NEGATIVE,
    //             res.getText(com.android.internal.R.string.report),
    //             mHandler.obtainMessage(WAIT_AND_REPORT));
    // }

    AutoPtr<ICharSequence> title;
    res->GetText(0x010402fd/*com.android.internal.R.string.anr_title*/,
            (ICharSequence**)&title);
    SetTitle(title);
    String processName;
    app->mInfo->GetProcessName(&processName);
    GetWindow()->SetTitle("Application Not Responding: " + processName);
}

void AppNotRespondingDialog::OnStop()
{}

void AppNotRespondingDialog::HandleOnClickMessage(
    /* [in] */ Int32 type)
{
    AutoPtr<IIntent> appErrorIntent;
    switch (type) {
        case AppNotRespondingDialog_FORCE_CLOSE:
            // Kill the application.
            // todo:KillAppAtUsersRequest must be public and the function is public in android
            mService->KillAppAtUsersRequest(mProc, (IDialog*)Probe(EIID_IDialog));
            break;
        case AppNotRespondingDialog_WAIT_AND_REPORT:
        case AppNotRespondingDialog_WAIT:
            // Continue waiting for the application.
            Mutex::Autolock lock(mServiceLock);
            ProcessRecord* app = mProc;

            if (type == AppNotRespondingDialog_WAIT_AND_REPORT) {
                mService->CreateAppErrorIntentLocked(app,
                        System::GetCurrentTimeMillis(), NULL, (IIntent**)&appErrorIntent);
            }

            app->mNotResponding = FALSE;
            //app->mNotRespondingReport = null;
            // if (app.anrDialog == AppNotRespondingDialog.this) {
            //     app.anrDialog = null;
            // }
            break;
    }

    if (appErrorIntent != NULL) {
        //try {
        GetContext()->StartActivity(appErrorIntent);
        //} catch (ActivityNotFoundException e) {
        //    Slog.w(TAG, "bug report receiver dissappeared", e);
        //}
    }
}

AppNotRespondingDialog::PositiveButtonOnClickListener::PositiveButtonOnClickListener(
    /* [in] */ AppNotRespondingDialog* host)
    : mHost(host)
{}

PInterface AppNotRespondingDialog::PositiveButtonOnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (PInterface)(IDialogInterfaceOnClickListener*)this;
    }

    return NULL;
}

UInt32 AppNotRespondingDialog::PositiveButtonOnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AppNotRespondingDialog::PositiveButtonOnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode AppNotRespondingDialog::PositiveButtonOnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AppNotRespondingDialog::PositiveButtonOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mHost->HandleOnClickMessage(AppNotRespondingDialog_FORCE_CLOSE);
}

AppNotRespondingDialog::NeutralButtonOnClickListener::NeutralButtonOnClickListener(
    /* [in] */ AppNotRespondingDialog* host)
    : mHost(host)
{}

PInterface AppNotRespondingDialog::NeutralButtonOnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IDialogInterfaceOnClickListener) {
        return (PInterface)(IDialogInterfaceOnClickListener*)this;
    }

    return NULL;
}

UInt32 AppNotRespondingDialog::NeutralButtonOnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AppNotRespondingDialog::NeutralButtonOnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode AppNotRespondingDialog::NeutralButtonOnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AppNotRespondingDialog::NeutralButtonOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mHost->HandleOnClickMessage(AppNotRespondingDialog_WAIT_AND_REPORT);
}
