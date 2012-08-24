
#include "text/CDynamicLayout.h"

ILAYOUT_METHODS_IMPL(CDynamicLayout, DynamicLayout, DynamicLayout);

PInterface CDynamicLayout::Probe(
    /* [in] */ REIID riid)
{
    return _CDynamicLayout::Probe(riid);
}

ECode CDynamicLayout::constructor(
    /* [in] */ ICharSequence* base,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return DynamicLayout::Init(
        base, base, paint, width, align,
        spacingmult, spacingadd, includepad);
}

ECode CDynamicLayout::constructor(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return DynamicLayout::Init(
        base, display, paint, width, align,
        spacingmult, spacingadd, includepad);
}

ECode CDynamicLayout::constructor(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    return DynamicLayout::Init(
        base, display, paint, width, align, spacingmult,
        spacingadd, includepad, ellipsize, ellipsizedWidth);
}
