
#ifndef __CALERTDIALOG_H__
#define __CALERTDIALOG_H__

#include "_CAlertDialog.h"
#include "app/AlertDialog.h"
#include "app/AlertDialogMacro.h"
#include "app/DialogMacro.h"
#include "view/ViewMacro.h"
#include "view/WindowCallbackMacro.h"

CarClass(CAlertDialog), public AlertDialog
{
public:
    IALERTDIALOG_METHODS_DECL();

    IDIALOG_METHODS_DECL();

    IWINDOWCALLBACK_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

private:
    // TODO: Add your private member variables here.
};

#endif // __CALERTDIALOG_H__
