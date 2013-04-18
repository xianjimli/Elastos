
#ifndef __PROGRESSDIALOG_H__
#define __PROGRESSDIALOG_H__

#include "app/AlertDialog.h"

class ProgressDialog : public AlertDialog
{
public:

    ProgressDialog();

    ProgressDialog(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme = 0x010300a3/*com.android.internal.R.style.Theme_Dialog_Alert*/);

protected:

    // @Override
    virtual CARAPI_(void) OnCreate(
        /* [in] */ IBundle* savedInstanceState);

public:

    // @Override
    virtual CARAPI_(void) OnStart();

    // @Override
    virtual CARAPI_(void) OnStop();

    virtual CARAPI SetProgress(
        /* [in] */ Int32 value);

    virtual CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    virtual CARAPI GetProgress(
        /* [out] */ Int32* progress);

    virtual CARAPI GetSecondaryProgress(
        /* [out] */ Int32* secondaryProgress);

    virtual CARAPI GetMax(
        /* [out] */ Int32* max);

    virtual CARAPI SetMax(
        /* [in] */ Int32 max);

    virtual CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    virtual CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    virtual CARAPI SetProgressDrawable(
        /* [in] */ IDrawable* d);

    virtual CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable* d);

    virtual CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    virtual CARAPI IsIndeterminate(
        /* [out] */ Boolean* isIndeterminate);

    // @Override
    virtual CARAPI SetMessage(
        /* [in] */ ICharSequence* message);

    virtual CARAPI SetProgressStyle(
        /* [in] */ Int32 style);

    virtual CARAPI SetProgressNumberFormat(
        /* [in] */ const String& format);

private:

    CARAPI_(void) OnProgressChanged();

protected:

    AutoPtr<IProgressBar> mProgress;
    AutoPtr<ITextView> mMessageView;

    Int32 mProgressStyle;
    AutoPtr<ITextView> mProgressNumber;
    String mProgressNumberFormat;
    AutoPtr<ITextView> mProgressPercent;
    // TODO: ALEX need NumberFormat
    // NumberFormat mProgressPercentFormat;

    Int32 mMax;
    Int32 mProgressVal;
    Int32 mSecondaryProgressVal;
    Int32 mIncrementBy;
    Int32 mIncrementSecondaryBy;
    AutoPtr<IDrawable> mProgressDrawable;
    AutoPtr<IDrawable> mIndeterminateDrawable;
    AutoPtr<ICharSequence> mMessage;
    Boolean mIndeterminate;

    Boolean mHasStarted;
    AutoPtr<IHandler> mViewUpdateHandler;
};

#endif // __PROGRESSDIALOG_H__
