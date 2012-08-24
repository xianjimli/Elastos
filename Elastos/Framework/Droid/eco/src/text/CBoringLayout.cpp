
#include "text/CBoringLayout.h"

ILAYOUT_METHODS_IMPL(CBoringLayout, BoringLayout, BoringLayout);

PInterface CBoringLayout::Probe(
    /* [in] */ REIID riid)
{
    return _CBoringLayout::Probe(riid);
}

ECode CBoringLayout::ReplaceOrMake(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [out] */ IBoringLayout** layout)
{
    VALIDATE_NOT_NULL(layout);

    return BoringLayout::ReplaceOrMake(
        source, paint, outerwidth, align, spacingmult,
        spacingadd, metrics, includepad, layout);
}

ECode CBoringLayout::ReplaceOrMakeEx(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth,
    /* [out] */ IBoringLayout** layout)
{
    VALIDATE_NOT_NULL(layout);

    return BoringLayout::ReplaceOrMake(
        source, paint, outerwidth, align, spacingmult, spacingadd,
        metrics, includepad, ellipsize, ellipsizedWidth, layout);
}

ECode CBoringLayout::Ellipsized(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return BoringLayout::Ellipsized(start, end);
}

ECode CBoringLayout::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad)
{
    return BoringLayout::_Init(source, paint, outerwidth, align,
            spacingmult, spacingadd, metrics, includepad);
}

ECode CBoringLayout::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    return BoringLayout::_Init(source, paint, outerwidth, align, spacingmult,
            spacingadd, metrics, includepad, ellipsize, ellipsizedWidth);
}
