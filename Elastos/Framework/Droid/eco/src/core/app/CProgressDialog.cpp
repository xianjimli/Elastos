
#include "ext/frameworkext.h"
#include "app/CProgressDialog.h"

IDIALOG_METHODS_IMPL(CProgressDialog, ProgressDialog, ProgressDialog);

IALERTDIALOG_METHODS_IMPL(CProgressDialog, ProgressDialog, ProgressDialog);

IWINDOWCALLBACK_METHODS_IMPL(CProgressDialog, ProgressDialog, ProgressDialog);

IKeyEventCallback_METHODS_IMPL(CProgressDialog, ProgressDialog, ProgressDialog);

ECode CProgressDialog::SetProgress(
    /* [in] */ Int32 value)
{
    return ProgressDialog::SetProgress(value);
}

ECode CProgressDialog::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    return ProgressDialog::SetSecondaryProgress(secondaryProgress);
}

ECode CProgressDialog::GetProgress(
    /* [out] */ Int32* progress)
{
    return ProgressDialog::GetProgress(progress);
}

ECode CProgressDialog::GetSecondaryProgress(
    /* [out] */ Int32* secondaryProgress)
{
    return ProgressDialog::GetSecondaryProgress(secondaryProgress);
}

ECode CProgressDialog::GetMax(
    /* [out] */ Int32* max)
{
    return ProgressDialog::GetMax(max);
}

ECode CProgressDialog::SetMax(
    /* [in] */ Int32 max)
{
    return ProgressDialog::SetMax(max);
}

ECode CProgressDialog::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    return ProgressDialog::IncrementProgressBy(diff);
}

ECode CProgressDialog::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    return ProgressDialog::IncrementSecondaryProgressBy(diff);
}

ECode CProgressDialog::SetProgressDrawable(
    /* [in] */ IDrawable* d)
{
    return ProgressDialog::SetProgressDrawable(d);
}

ECode CProgressDialog::SetIndeterminateDrawable(
    /* [in] */ IDrawable* d)
{
    return ProgressDialog::SetIndeterminateDrawable(d);
}

ECode CProgressDialog::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    return ProgressDialog::SetIndeterminate(indeterminate);
}

ECode CProgressDialog::IsIndeterminate(
    /* [out] */ Boolean* isIndeterminate)
{
    return ProgressDialog::IsIndeterminate(isIndeterminate);
}

ECode CProgressDialog::SetProgressStyle(
    /* [in] */ Int32 style)
{
    return ProgressDialog::SetProgressStyle(style);
}

ECode CProgressDialog::SetProgressNumberFormat(
    /* [in] */ const String& format)
{
    return ProgressDialog::SetProgressNumberFormat(format);
}

ECode CProgressDialog::constructor(
    /* [in] */ IContext* context)
{
    return ProgressDialog::Init(context);
}

ECode CProgressDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    return ProgressDialog::Init(context, theme);
}

ECode CProgressDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    return AlertDialog::Init(context, cancelable, cancelListener);
}

PInterface CProgressDialog::Probe(
    /* [in] */ REIID riid)
{
    return _CProgressDialog::Probe(riid);
}

ECode CProgressDialog::Show(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* message,
    /* [out] */ IProgressDialog** dialog)
{
    return Show2(context, title, message, FALSE, dialog);
}

ECode CProgressDialog::Show2(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* message,
    /* [in] */ Boolean indeterminate,
    /* [out] */ IProgressDialog** dialog)
{
    return Show3(context, title, message, indeterminate, FALSE, dialog);
}

ECode CProgressDialog::Show3(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* message,
    /* [in] */ Boolean indeterminate,
    /* [in] */ Boolean cancelable,
    /* [out] */ IProgressDialog** dialog)
{
    return Show4(
        context, title, message, indeterminate, cancelable, NULL, dialog);
}

ECode CProgressDialog::Show4(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* message,
    /* [in] */ Boolean indeterminate,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener,
    /* [out] */ IProgressDialog** dialog)
{
    VALIDATE_NOT_NULL(dialog);

    AutoPtr<IProgressDialog> obj;
    FAIL_RETURN(CProgressDialog::New(context, (IProgressDialog**)&obj));

    obj->SetTitle(title);
    obj->SetMessage(message);
    obj->SetIndeterminate(indeterminate);
    obj->SetCancelable(cancelable);
    obj->SetOnCancelListener(cancelListener);
    obj->Show();

    *dialog = obj;
    obj->AddRef();

    return NOERROR;
}

