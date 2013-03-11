
#ifndef __APPERRORDIALOG_H__
#define __APPERRORDIALOG_H__

#include "BaseErrorDialog.h"
#include "AppErrorResult.h"
#include "ProcessRecord.h"
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

class AppErrorDialog : public BaseErrorDialog
{
private:
    class PositiveButtonOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        PositiveButtonOnClickListener(
            /* [in] */ AppErrorDialog* host);

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
        AppErrorDialog* mHost;
    };

public:
    friend class PositiveButtonOnClickListener;

    AppErrorDialog(
        /* [in] */ IContext* context,
        /* [in] */ AppErrorResult* result,
        /* [in] */ ProcessRecord* app);

private:
    CARAPI_(void) HandleOnForceQuitMessage(
        /* [in] */ Int32 type);

private:
    static const CString TAG;

public:
    // Event 'what' codes
    static const Int32 AppErrorDialog_FORCE_QUIT;
    static const Int32 AppErrorDialog_FORCE_QUIT_AND_REPORT;

    // 5-minute timeout, then we automatically dismiss the crash dialog
    static const Int64 AppErrorDialog_DISMISS_TIMEOUT;

private:
    AppErrorResult* mResult;
    ProcessRecord* mProc;
    Mutex mProcLock;
};
#endif //__APPERRORDIALOG_H__
