
#ifndef __APPNOTRESPONDINGDIALOG_H__
#define __APPNOTRESPONDINGDIALOG_H__

#include "BaseErrorDialog.h"
#include "ProcessRecord.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

class AppNotRespondingDialog : public BaseErrorDialog
{
private:
    class PositiveButtonOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        PositiveButtonOnClickListener(
            /* [in] */ AppNotRespondingDialog* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AppNotRespondingDialog* mHost;
    };

    class NeutralButtonOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        PositiveButtonOnClickListener(
            /* [in] */ AppNotRespondingDialog* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AppNotRespondingDialog* mHost;
    };

public:
    friend class PositiveButtonOnClickListener;
    friend class NeutralButtonOnClickListener;

    AppNotRespondingDialog(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ IContext* context,
        /* [in] */ ProcessRecord* app,
        /* [in] */ CActivityRecord* activity);

    CARAPI_(void) OnStop();

private:
    CARAPI_(void) HandleOnClickMessage(
        /* [in] */ Int32 type);

private:
    static const CString TAG;

public:
    // Event 'what' codes
    static const Int32 AppNotRespondingDialog_FORCE_CLOSE;
    static const Int32 AppNotRespondingDialog_WAIT;
    static const Int32 AppNotRespondingDialog_WAIT_AND_REPORT;

    // 5-minute timeout, then we automatically dismiss the crash dialog
    static const Int64 AppNotRespondingDialog_DISMISS_TIMEOUT;

private:
    AutoPtr<CActivityManagerService> mService;
    Mutex mServiceLock;
    ProcessRecord* mProc;
};
#endif //__APPNOTRESPONDINGDIALOG_H__
