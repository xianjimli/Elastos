
#include "text/CStaticLayout.h"

ILAYOUT_METHODS_IMPL(CStaticLayout, StaticLayout, StaticLayout);

PInterface CStaticLayout::Probe(
    /* [in] */ REIID riid)
{
    return _CStaticLayout::Probe(riid);
}

ECode CStaticLayout::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return StaticLayout::Init(
        source, paint, width, align, spacingmult, spacingadd, includepad);
}

ECode CStaticLayout::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return StaticLayout::Init(
        source, bufstart, bufend, paint, outerwidth,
        align, spacingmult, spacingadd, includepad);
}

ECode CStaticLayout::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 bufstart,
    /* [in] */ Int32 bufend,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    return StaticLayout::Init(
        source, bufstart, bufend, paint, outerwidth,
        align, spacingmult, spacingadd, includepad,
        ellipsize, ellipsizedWidth);
}

ECode CStaticLayout::constructor(
    /* [in] */ Boolean ellipsize)
{
    return CStaticLayout::Init(ellipsize);
}
