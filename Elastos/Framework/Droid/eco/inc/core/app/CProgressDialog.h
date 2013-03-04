
#ifndef __CPROGRESSDIALOG_H__
#define __CPROGRESSDIALOG_H__

#include "_CProgressDialog.h"
#include "app/ProgressDialog.h"
#include "app/AlertDialogMacro.h"
#include "app/DialogMacro.h"
#include "view/ViewMacro.h"
#include "view/WindowCallbackMacro.h"

CarClass(CProgressDialog), public ProgressDialog
{
public:

    IDIALOG_METHODS_DECL();

    IALERTDIALOG_METHODS_DECL();

    IWINDOWCALLBACK_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetProgress(
        /* [in] */ Int32 value);

    CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI GetSecondaryProgress(
        /* [out] */ Int32* secondaryProgress);

    CARAPI GetMax(
        /* [out] */ Int32* max);

    CARAPI SetMax(
        /* [in] */ Int32 max);

    CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    CARAPI SetProgressDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    CARAPI IsIndeterminate(
        /* [out] */ Boolean* isIndeterminate);

    CARAPI SetProgressStyle(
        /* [in] */ Int32 style);

    CARAPI SetProgressNumberFormat(
        /* [in] */ const String& format);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme);

public:

    static CARAPI Show(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* message,
        /* [out] */ IProgressDialog** dialog);

    static CARAPI Show2(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* message,
        /* [in] */ Boolean indeterminate,
        /* [out] */ IProgressDialog** dialog);

    static CARAPI Show3(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* message,
        /* [in] */ Boolean indeterminate,
        /* [in] */ Boolean cancelable,
        /* [out] */ IProgressDialog** dialog);

    static CARAPI Show4(
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

#endif // __CPROGRESSDIALOG_H__
