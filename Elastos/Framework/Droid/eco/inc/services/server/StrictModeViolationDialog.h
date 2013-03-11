
#ifndef __STRICTMODEVIOLATIONDIALOG_H__
#define __STRICTMODEVIOLATIONDIALOG_H__

#include "BaseErrorDialog.h"
#include "AppErrorResult.h"
#include "ProcessRecord.h"
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

class StrictModeViolationDialog : public BaseErrorDialog
{
private:
    class PositiveButtonOnClickListener
        : public ElRefBase
        , public IDialogInterfaceOnClickListener
    {
    public:
        PositiveButtonOnClickListener(
            /* [in] */ StrictModeViolationDialog* host);

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
        StrictModeViolationDialog* mHost;
    };

public:
    friend class PositiveButtonOnClickListener;

    StrictModeViolationDialog(
        /* [in] */ IContext* context,
        /* [in] */ AppErrorResult* result,
        /* [in] */ ProcessRecord* app);

private:
    CARAPI_(void) HandleOnActionOkMessage(
        /* [in] */ Int32 type);

private:
    static const CString TAG;

public:
    // Event 'what' codes
    static const Int32 StrictModeViolationDialog_ACTION_OK;
    static const Int32 StrictModeViolationDialog_ACTION_OK_AND_REPORT;

    // 5-minute timeout, then we automatically dismiss the crash dialog
    static const Int64 StrictModeViolationDialog_DISMISS_TIMEOUT;

private:
    AppErrorResult* mResult;
    ProcessRecord* mProc;
    Mutex mProcLock;
};
#endif //__STRICTMODEVIOLATIONDIALOG_H__
