
#include "app/CDialog.h"

IDIALOG_METHODS_IMPL(CDialog, Dialog, Dialog);

IWINDOWCALLBACK_METHODS_IMPL(CDialog, Dialog, Dialog);

IKeyEventCallback_METHODS_IMPL(CDialog, Dialog, Dialog);

ECode CDialog::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return Dialog::OnCreateContextMenu(menu, v, menuInfo);
}

ECode CDialog::constructor(
    /* [in] */ IContext* context)
{
    return Dialog::Init(context);
}

ECode CDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    return Dialog::Init(context, theme);
}

ECode CDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    return Dialog::Init(context, cancelable, cancelListener);
}

PInterface CDialog::Probe(
    /* [in] */ REIID riid)
{
    return _CDialog::Probe(riid);
}

