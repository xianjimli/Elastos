
#include "ext/frameworkext.h"
#include "text/style/CBackgroundColorSpan.h"
#include "text/CTextPaint.h"

static Int32 sColor;

ECode CBackgroundColorSpan::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    CTextPaint* ctp = (CTextPaint*)tp;
    ctp->mBgColor = sColor;

    return NOERROR;
}

ECode CBackgroundColorSpan::GetUnderlying(
    /* [out] */ ICharacterStyle** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ICharacterStyle::Probe((IBackgroundColorSpan*)this);
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CBackgroundColorSpan::GetSpanTypeId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TextUtils_BACKGROUND_COLOR_SPAN;

    return NOERROR;
}

ECode CBackgroundColorSpan::GetBackgroundColor(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = sColor;

    return NOERROR;
}

ECode CBackgroundColorSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    FAIL_RETURN(source->ReadInt32(&sColor));

    return NOERROR;
}

ECode CBackgroundColorSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    FAIL_RETURN(dest->WriteInt32(sColor));

    return NOERROR;
}

ECode CBackgroundColorSpan::constructor(
    /* [in] */ Int32 color)
{
    sColor = color;

    return NOERROR;
}

ECode CBackgroundColorSpan::constructor(
    /* [in] */ IParcel * src)
{
    VALIDATE_NOT_NULL(src);
    FAIL_RETURN(src->ReadInt32(&sColor));

    return NOERROR;
}

