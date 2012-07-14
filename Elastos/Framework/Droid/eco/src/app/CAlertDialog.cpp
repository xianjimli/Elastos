
#include "app/CAlertDialog.h"

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

ECode CAlertDialog::GetButton(
    /* [in] */ Int32 whichButton,
    /* [out] */ IButton** button)
{
    VALIDATE_NOT_NULL(button);
    AutoPtr<IButton> temp = AlertDialog::GetButton(whichButton);
    *button = temp;
    if (*button) {
        (*button)->AddRef();
    }

    return NOERROR;
}

ECode CAlertDialog::GetListView(
    /* [out] */ IListView** listView)
{
    VALIDATE_NOT_NULL(listView);
    AutoPtr<IListView> temp = AlertDialog::GetListView();
    *listView = temp;
    if (*listView) {
        (*listView)->AddRef();
    }

    return NOERROR;
}

ECode CAlertDialog::SetCustomTitle(
    /* [in] */ IView* customTitleView)
{
    return AlertDialog::SetCustomTitle(customTitleView);
}

ECode CAlertDialog::SetMessage(
    /* [in] */ ICharSequence* message)
{
    return AlertDialog::SetMessage(message);
}

ECode CAlertDialog::SetView(
    /* [in] */ IView* view)
{
    return AlertDialog::SetView(view);
}

ECode CAlertDialog::SetViewEx(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSpacingLeft,
    /* [in] */ Int32 viewSpacingTop,
    /* [in] */ Int32 viewSpacingRight,
    /* [in] */ Int32 viewSpacingBottom)
{
    return AlertDialog::SetView(
        view, viewSpacingLeft, viewSpacingTop,
        viewSpacingRight, viewSpacingBottom);
}

ECode CAlertDialog::SetButton(
    /* [in] */ Int32 whichButton,
    /* [in] */ ICharSequence* text)
{
    return AlertDialog::SetButton(whichButton, text);
}

ECode CAlertDialog::SetButtonEx(
    /* [in] */ Int32 whichButton,
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    return AlertDialog::SetButton(whichButton, text, listener);
}

ECode CAlertDialog::SetIcon(
    /* [in] */ Int32 resId)
{
    return AlertDialog::SetIcon(resId);
}

ECode CAlertDialog::SetIconEx(
    /* [in] */ IDrawable* icon)
{
    return AlertDialog::SetIcon(icon);
}

ECode CAlertDialog::SetInverseBackgroundForced(
    /* [in] */ Boolean forceInverseBackground)
{
    return AlertDialog::SetInverseBackgroundForced(forceInverseBackground);
}
