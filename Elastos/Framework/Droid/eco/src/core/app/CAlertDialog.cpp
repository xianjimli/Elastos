
#include "app/CAlertDialog.h"

IALERTDIALOG_METHODS_IMPL(CAlertDialog, AlertDialog, AlertDialog);

IDIALOG_METHODS_IMPL(CAlertDialog, AlertDialog, AlertDialog);

IWINDOWCALLBACK_METHODS_IMPL(CAlertDialog, AlertDialog, AlertDialog);

IKeyEventCallback_METHODS_IMPL(CAlertDialog, AlertDialog, AlertDialog);

ECode CAlertDialog::constructor(
    /* [in] */ IContext* context)
{
    return AlertDialog::Init(context);
}

ECode CAlertDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    return AlertDialog::Init(context, theme);
}

ECode CAlertDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    return AlertDialog::Init(context, cancelable, cancelListener);
}

PInterface CAlertDialog::Probe(
    /* [in] */ REIID riid)
{
    return _CAlertDialog::Probe(riid);
}

ECode CAlertDialog::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return AlertDialog::OnCreateContextMenu(menu, v, menuInfo);
}
