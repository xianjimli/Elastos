
#include "graphics/CPaintFontMetricsInt.h"
#include "ext/frameworkdef.h"

ECode CPaintFontMetricsInt::GetTop(
    /* [out] */ Int32* top)
{
    VALIDATE_NOT_NULL(top);

    *top = mTop;
    return NOERROR;
}

ECode CPaintFontMetricsInt::SetTop(
    /* [in] */ Int32 top)
{
    mTop = top;
    return NOERROR;
}

ECode CPaintFontMetricsInt::GetAscent(
    /* [out] */ Int32* ascent)
{
    VALIDATE_NOT_NULL(ascent);

    *ascent = mAscent;
    return NOERROR;
}

ECode CPaintFontMetricsInt::SetAscent(
    /* [in] */ Int32 ascent)
{
    mAscent = ascent;
    return NOERROR;
}

ECode CPaintFontMetricsInt::GetDescent(
    /* [out] */ Int32* descent)
{
    VALIDATE_NOT_NULL(descent);

    *descent = mDescent;
    return NOERROR;
}

ECode CPaintFontMetricsInt::SetDescent(
    /* [in] */ Int32 descent)
{
    mDescent = descent;
    return NOERROR;
}

ECode CPaintFontMetricsInt::GetBottom(
    /* [out] */ Int32* bottom)
{
    VALIDATE_NOT_NULL(bottom);

    *bottom = mBottom;
    return NOERROR;
}

ECode CPaintFontMetricsInt::SetBottom(
    /* [in] */ Int32 bottom)
{
    mBottom = bottom;
    return NOERROR;
}

ECode CPaintFontMetricsInt::GetLeading(
    /* [out] */ Int32* leading)
{
    VALIDATE_NOT_NULL(leading);

    *leading = mLeading;
    return NOERROR;
}

ECode CPaintFontMetricsInt::SetLeading(
    /* [in] */ Int32 leading)
{
    mLeading = leading;
    return NOERROR;
}
