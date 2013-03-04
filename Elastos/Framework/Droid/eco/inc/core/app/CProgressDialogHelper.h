
#ifndef __CPROGRESSDIALOGHELPER_H__
#define __CPROGRESSDIALOGHELPER_H__

#include "_CProgressDialogHelper.h"

CarClass(CProgressDialogHelper)
{
public:
    CARAPI Show(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* message,
        /* [out] */ IProgressDialog** dialog);

    CARAPI Show2(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* message,
        /* [in] */ Boolean indeterminate,
        /* [out] */ IProgressDialog** dialog);

    CARAPI Show3(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* message,
        /* [in] */ Boolean indeterminate,
        /* [in] */ Boolean cancelable,
        /* [out] */ IProgressDialog** dialog);

    CARAPI Show4(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* message,
        /* [in] */ Boolean indeterminate,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener,
        /* [out] */ IProgressDialog** dialog);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPROGRESSDIALOGHELPER_H__
