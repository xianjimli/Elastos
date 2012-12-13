
#include "view/inputmethod/CExtractedText.h"

const Int32 CExtractedText::FLAG_SINGLE_LINE;
const Int32 CExtractedText::FLAG_SELECTING;


CExtractedText::CExtractedText():
	mStartOffset(0),
	mPartialStartOffset(0),
	mPartialEndOffset(0),
	mSelectionStart(0),
	mSelectionEnd(0),
	mFlags(0)
{

}

ECode CExtractedText::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CExtractedText::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}
