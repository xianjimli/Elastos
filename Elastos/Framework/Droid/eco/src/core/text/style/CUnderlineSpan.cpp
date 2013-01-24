
#include "ext/frameworkext.h"
#include "text/style/CUnderlineSpan.h"

ECode CUnderlineSpan::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    tp->SetUnderlineText(TRUE);

    return NOERROR;
}

ECode CUnderlineSpan::GetUnderlying(
    /* [out] */ ICharacterStyle** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ICharacterStyle::Probe((IBackgroundColorSpan*)this);
    if (*result) {
        (*result)->AddRef();
    }

    return NOERROR;
}

ECode CUnderlineSpan::GetSpanTypeId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TextUtils_UNDERLINE_SPAN;

    return NOERROR;
}

ECode CUnderlineSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode CUnderlineSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return NOERROR;
}

ECode CUnderlineSpan::constructor()
{
    return NOERROR;
}

ECode CUnderlineSpan::constructor(
    /* [in] */ IParcel* src)
{
    return NOERROR;
}

