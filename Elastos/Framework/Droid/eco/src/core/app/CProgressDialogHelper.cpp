
#include "ext/frameworkext.h"
#include "app/CProgressDialogHelper.h"
#include "app/CProgressDialog.h"

ECode CProgressDialogHelper::Show(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* message,
    /* [out] */ IProgressDialog** dialog)
{
    return CProgressDialog::Show(context, title, message, dialog);
}

ECode CProgressDialogHelper::Show2(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* message,
    /* [in] */ Boolean indeterminate,
    /* [out] */ IProgressDialog** dialog)
{
    return CProgressDialog::Show2(
        context, title, message, indeterminate, dialog);
}

ECode CProgressDialogHelper::Show3(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* message,
    /* [in] */ Boolean indeterminate,
    /* [in] */ Boolean cancelable,
    /* [out] */ IProgressDialog** dialog)
{
    return CProgressDialog::Show3(
        context, title, message, indeterminate, cancelable, dialog);
}

ECode CProgressDialogHelper::Show4(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* message,
    /* [in] */ Boolean indeterminate,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener,
    /* [out] */ IProgressDialog** dialog)
{
    return CProgressDialog::Show4(
        context, title, message, indeterminate, cancelable,
        cancelListener, dialog);
}

