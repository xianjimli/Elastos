
#include "ext/frameworkdef.h"
#include "widget/CTextViewCommitSelectionReceiver.h"
#include "widget/TextView.h"
#include "text/Selection.h"
#include <elastos/Math.h>

using namespace Elastos::Core;


ECode CTextViewCommitSelectionReceiver::constructor()
{
    return NOERROR;
}

ECode CTextViewCommitSelectionReceiver::constructor(
    /* [in] */ Int32 prevStart,
    /* [in] */ Int32 prevEnd,
    /* [in] */ Handle32 host)
{
    FAIL_RETURN(ResultReceiver::Init(((TextView*)host)->GetHandler()));
    mPrevStart = prevStart;
    mPrevEnd = prevEnd;
    mHost = host;
    return NOERROR;
}

ECode CTextViewCommitSelectionReceiver::OnReceiveResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    // If this tap was actually used to show the IMM, leave cursor or selection unchanged
    // by restoring its previous position.
    if (resultCode == InputMethodManager_RESULT_SHOWN) {
        Int32 len = 0;
        ((TextView*)mHost)->mText->GetLength(&len);
        Int32 start = Math::Min(len, mPrevStart);
        Int32 end = Math::Min(len, mPrevEnd);
        Selection::SetSelection(ISpannable::Probe(((TextView*)mHost)->mText), start, end);

        Boolean selectAllGotFocus = ((TextView*)mHost)->mSelectAllOnFocus && ((TextView*)mHost)->mTouchFocusSelected;
        if (((TextView*)mHost)->HasSelection() && !selectAllGotFocus) {
            ((TextView*)mHost)->StartTextSelectionMode();
        }
    }
    return NOERROR;
}

ECode CTextViewCommitSelectionReceiver::Send(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    return ResultReceiver::Send(resultCode, resultData);
}

ECode CTextViewCommitSelectionReceiver::DescribeContents(
    /* [out] */ Int32* contents)
{
    return ResultReceiver::DescribeContents(contents);
}

ECode CTextViewCommitSelectionReceiver::ReadFromParcel(
    /* [in] */ IParcel *dest)
{
    return ResultReceiver::ReadFromParcel(dest);
}

ECode CTextViewCommitSelectionReceiver::WriteToParcel(
    /* [in] */ IParcel* out)
{
    return ResultReceiver::WriteToParcel(out);
}

Mutex* CTextViewCommitSelectionReceiver::GetSelfLock()
{
    return &_m_syncLock;
}
