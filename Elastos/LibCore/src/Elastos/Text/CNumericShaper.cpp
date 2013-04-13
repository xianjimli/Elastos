
#include "CNumericShaper.h"


ECode CNumericShaper::GetRanges(
    /* [out] */ Int32 * pRanges)
{
    return NumericShaper::GetRanges(pRanges);
}

ECode CNumericShaper::IsContextual(
    /* [out] */ Boolean * pIsContextual)
{
    return NumericShaper::IsContextual(pIsContextual);
}

ECode CNumericShaper::Shape(
    /* [in] */ ArrayOf<Char32> * pText,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 context)
{
    return NumericShaper::Shape(pText, start, count, context);
}

ECode CNumericShaper::ShapeEx(
    /* [in] */ ArrayOf<Char32> * pText,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    return NumericShaper::ShapeEx(pText, start, count);
}

