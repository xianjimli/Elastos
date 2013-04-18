
#include "app/ProgressDialog.h"
#include "view/LayoutInflater.h"

ProgressDialog::ProgressDialog()
    : mProgressStyle(ProgressDialog_STYLE_SPINNER)
    , mMax(0)
    , mProgressVal(0)
    , mSecondaryProgressVal(0)
    , mIncrementBy(0)
    , mIncrementSecondaryBy(0)
    , mIndeterminate(FALSE)
    , mHasStarted(FALSE)
{
}

ProgressDialog::ProgressDialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
    : mProgressStyle(ProgressDialog_STYLE_SPINNER)
    , mMax(0)
    , mProgressVal(0)
    , mSecondaryProgressVal(0)
    , mIncrementBy(0)
    , mIncrementSecondaryBy(0)
    , mIndeterminate(FALSE)
    , mHasStarted(FALSE)
{
    AlertDialog::Init(context, theme);
}

// @Override
void ProgressDialog::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    AutoPtr<ILayoutInflater> inflater;
    LayoutInflater::From(mContext, (ILayoutInflater**)&inflater);

    if (mProgressStyle == ProgressDialog_STYLE_HORIZONTAL) {
#if 0
        /* Use a separate handler to update the text views as they
         * must be updated on the same thread that created them.
         */
        mViewUpdateHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);

                /* Update the number and percent */
                int progress = mProgress.getProgress();
                int max = mProgress.getMax();
                double percent = (double) progress / (double) max;
                String format = mProgressNumberFormat;
                mProgressNumber.setText(String.format(format, progress, max));
                SpannableString tmp = new SpannableString(mProgressPercentFormat.format(percent));
                tmp.setSpan(new StyleSpan(android.graphics.Typeface.BOLD),
                        0, tmp.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                mProgressPercent.setText(tmp);
            }
        };
        View view = inflater.inflate(R.layout.alert_dialog_progress, null);
        mProgress = (ProgressBar) view.findViewById(R.id.progress);
        mProgressNumber = (TextView) view.findViewById(R.id.progress_number);
        mProgressNumberFormat = "%d/%d";
        mProgressPercent = (TextView) view.findViewById(R.id.progress_percent);
        mProgressPercentFormat = NumberFormat.getPercentInstance();
        mProgressPercentFormat.setMaximumFractionDigits(0);
        setView(view);
#endif
        // TODO: ALEX need Handler
    } else {
        AutoPtr<IView> view;
        inflater->Inflate(
            0x0109004f /* R.layout.progress_dialog */, NULL, (IView**)&view);

        AutoPtr<IView> temp1;
        view->FindViewById(
            0x0102000d /* R.id.progress */, (IView**)&temp1);
        mProgress = (IProgressBar*)temp1->Probe(EIID_IProgressBar);

        AutoPtr<IView> temp2;
        view->FindViewById(
            0x0102000b /* R.id.message */, (IView**)&temp2);
        mMessageView = (ITextView*)temp2->Probe(EIID_ITextView);

        AlertDialog::SetView(view);
    }
    if (mMax > 0) {
        SetMax(mMax);
    }
    if (mProgressVal > 0) {
        SetProgress(mProgressVal);
    }
    if (mSecondaryProgressVal > 0) {
        SetSecondaryProgress(mSecondaryProgressVal);
    }
    if (mIncrementBy > 0) {
        IncrementProgressBy(mIncrementBy);
    }
    if (mIncrementSecondaryBy > 0) {
        IncrementSecondaryProgressBy(mIncrementSecondaryBy);
    }
    if (mProgressDrawable != NULL) {
        SetProgressDrawable(mProgressDrawable);
    }
    if (mIndeterminateDrawable != NULL) {
        SetIndeterminateDrawable(mIndeterminateDrawable);
    }
    if (mMessage != NULL) {
        SetMessage(mMessage);
    }
    SetIndeterminate(mIndeterminate);
    OnProgressChanged();
    AlertDialog::OnCreate(savedInstanceState);

}

// @Override
void ProgressDialog::OnStart()
{
    AlertDialog::OnStart();
    mHasStarted = TRUE;
}

// @Override
void ProgressDialog::OnStop()
{
    AlertDialog::OnStop();
    mHasStarted = FALSE;
}

ECode ProgressDialog::SetProgress(
    /* [in] */ Int32 value)
{
    if (mHasStarted) {
        mProgress->SetProgress(value);
        OnProgressChanged();
    } else {
        mProgressVal = value;
    }

    return NOERROR;
}

ECode ProgressDialog::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    if (mProgress != NULL) {
        mProgress->SetSecondaryProgress(secondaryProgress);
        OnProgressChanged();
    } else {
        mSecondaryProgressVal = secondaryProgress;
    }

    return NOERROR;
}

ECode ProgressDialog::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);

    if (mProgress != NULL) {
        return mProgress->GetProgress(progress);
    }

    *progress = mProgressVal;

    return NOERROR;
}

ECode ProgressDialog::GetSecondaryProgress(
    /* [out] */ Int32* secondaryProgress)
{
    VALIDATE_NOT_NULL(secondaryProgress);

    if (mProgress != NULL) {
        return mProgress->GetSecondaryProgress(secondaryProgress);
    }

    *secondaryProgress = mSecondaryProgressVal;

    return NOERROR;
}

ECode ProgressDialog::GetMax(
    /* [out] */ Int32* max)
{
    VALIDATE_NOT_NULL(max);

    if (mProgress != NULL) {
        return mProgress->GetMax(max);
    }

    *max = mMax;

    return NOERROR;
}

ECode ProgressDialog::SetMax(
    /* [in] */ Int32 max)
{
    if (mProgress != NULL) {
        mProgress->SetMax(max);
        OnProgressChanged();
    } else {
        mMax = max;
    }

    return NOERROR;
}

ECode ProgressDialog::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    if (mProgress != NULL) {
        mProgress->IncrementProgressBy(diff);
        OnProgressChanged();
    } else {
        mIncrementBy += diff;
    }

    return NOERROR;
}

ECode ProgressDialog::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    if (mProgress != NULL) {
        mProgress->IncrementSecondaryProgressBy(diff);
        OnProgressChanged();
    } else {
        mIncrementSecondaryBy += diff;
    }

    return NOERROR;
}

ECode ProgressDialog::SetProgressDrawable(
    /* [in] */ IDrawable* d)
{
    if (mProgress != NULL) {
        mProgress->SetProgressDrawable(d);
    } else {
        mProgressDrawable = d;
    }

    return NOERROR;
}

ECode ProgressDialog::SetIndeterminateDrawable(
    /* [in] */ IDrawable* d)
{
    if (mProgress != NULL) {
        mProgress->SetIndeterminateDrawable(d);
    } else {
        mIndeterminateDrawable = d;
    }

    return NOERROR;
}

ECode ProgressDialog::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    if (mProgress != NULL) {
        mProgress->SetIndeterminate(indeterminate);
    } else {
        mIndeterminate = indeterminate;
    }

    return NOERROR;
}

ECode ProgressDialog::IsIndeterminate(
    /* [out] */ Boolean* isIndeterminate)
{
    VALIDATE_NOT_NULL(isIndeterminate);

    if (mProgress != NULL) {
        return mProgress->IsIndeterminate(isIndeterminate);
    }

    *isIndeterminate = mIndeterminate;

    return NOERROR;
}

// @Override
ECode ProgressDialog::SetMessage(
    /* [in] */ ICharSequence* message)
{
    if (mProgress != NULL) {
        if (mProgressStyle == ProgressDialog_STYLE_HORIZONTAL) {
            return AlertDialog::SetMessage(message);
        } else {
            return mMessageView->SetText(message);
        }
    } else {
        mMessage = message;
    }

    return NOERROR;
}

ECode ProgressDialog::SetProgressStyle(
    /* [in] */ Int32 style)
{
    mProgressStyle = style;

    return NOERROR;
}

ECode ProgressDialog::SetProgressNumberFormat(
    /* [in] */ const String& format)
{
    mProgressNumberFormat = format;

    return NOERROR;
}

void ProgressDialog::OnProgressChanged()
{
    if (mProgressStyle == ProgressDialog_STYLE_HORIZONTAL) {
        // TODO: ALEX need IHandler::SendEmptyMessage
        // mViewUpdateHandler->SendEmptyMessage(0);
    }
}
