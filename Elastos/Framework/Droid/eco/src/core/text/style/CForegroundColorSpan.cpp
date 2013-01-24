
#include "ext/frameworkext.h"
#include "text/style/CForegroundColorSpan.h"

static Int32 sColor;

ECode CForegroundColorSpan::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    tp->SetColor(sColor);

    return NOERROR;
}

ECode CForegroundColorSpan::GetUnderlying(
    /* [out] */ ICharacterStyle** result)
{
    // VALIDATE_NOT_NULL(result);
    *result = ICharacterStyle::Probe((IForegroundColorSpan*)this);
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CForegroundColorSpan::GetSpanTypeId(
    /* [out] */ Int32* result)
{
    // VALIDATE_NOT_NULL(result);
    *result = TextUtils_FOREGROUND_COLOR_SPAN;

    return NOERROR;
}

ECode CForegroundColorSpan::GetForegroundColor(
    /* [out] */ Int32* result)
{
    // VALIDATE_NOT_NULL(result);
    *result = sColor;

    return NOERROR;
}

ECode CForegroundColorSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    FAIL_RETURN(source->ReadInt32(&sColor));

    return NOERROR;
}

ECode CForegroundColorSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    FAIL_RETURN(dest->WriteInt32(sColor));

    return NOERROR;
}

ECode CForegroundColorSpan::constructor(
    /* [in] */ Int32 color)
{
    sColor = color;

    return NOERROR;
}

ECode CForegroundColorSpan::constructor(
    /* [in] */ IParcel* src)
{
    VALIDATE_NOT_NULL(src);
    FAIL_RETURN(src->ReadInt32(&sColor));

    return NOERROR;
}

