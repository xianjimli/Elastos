
#include "ext/frameworkdef.h"
#include "view/inputmethod/CExtractedTextRequest.h"

ECode CExtractedTextRequest::GetToken(
    /* [out] */ Int32* token)
{
    VALIDATE_NOT_NULL(token);
    *token = mToken;
    return NOERROR;
}

ECode CExtractedTextRequest::SetToken(
    /* [in] */ Int32 token)
{
    mToken = token;
    return NOERROR;
}

ECode CExtractedTextRequest::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CExtractedTextRequest::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CExtractedTextRequest::GetHintMaxLines(
    /* [out] */ Int32* hintMaxLines)
{
    VALIDATE_NOT_NULL(hintMaxLines);
    *hintMaxLines = mHintMaxLines;
    return NOERROR;
}

ECode CExtractedTextRequest::SetHintMaxLines(
    /* [in] */ Int32 hintMaxLines)
{
    mHintMaxLines = hintMaxLines;
    return NOERROR;
}

ECode CExtractedTextRequest::GetHintMaxChars(
    /* [out] */ Int32* hintMaxChars)
{
    VALIDATE_NOT_NULL(hintMaxChars);
    *hintMaxChars = mHintMaxChars;
    return NOERROR;
}

ECode CExtractedTextRequest::SetHintMaxChars(
    /* [in] */ Int32 hintMaxChars)
{
    mHintMaxChars = hintMaxChars;
    return NOERROR;
}

ECode CExtractedTextRequest::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CExtractedTextRequest::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}
