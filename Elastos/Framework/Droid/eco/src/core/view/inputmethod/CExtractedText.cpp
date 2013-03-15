
#include "ext/frameworkdef.h"
#include "view/inputmethod/CExtractedText.h"


const Int32 CExtractedText::FLAG_SINGLE_LINE;
const Int32 CExtractedText::FLAG_SELECTING;

CExtractedText::CExtractedText()
	: mStartOffset(0)
	, mPartialStartOffset(0)
	, mPartialEndOffset(0)
	, mSelectionStart(0)
	, mSelectionEnd(0)
	, mFlags(0)
{}

ECode CExtractedText::GetStartOffset(
    /* [out] */ Int32* startOffset)
{
    VALIDATE_NOT_NULL(startOffset);
    *startOffset = mStartOffset;
    return NOERROR;
}

ECode CExtractedText::SetStartOffset(
    /* [in] */ Int32 startOffset)
{
    mStartOffset = startOffset;
    return NOERROR;
}

ECode CExtractedText::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CExtractedText::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}
