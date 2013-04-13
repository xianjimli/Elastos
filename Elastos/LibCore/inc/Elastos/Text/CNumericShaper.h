
#ifndef __CNUMERICSHAPER_H__
#define __CNUMERICSHAPER_H__

#include "_CNumericShaper.h"
#include "NumericShaper.h"


CarClass(CNumericShaper), public NumericShaper
{
public:
    CARAPI GetRanges(
        /* [out] */ Int32 * pRanges);

    CARAPI IsContextual(
        /* [out] */ Boolean * pIsContextual);

    CARAPI Shape(
        /* [in] */ ArrayOf<Char32> * pText,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 context);

    CARAPI ShapeEx(
        /* [in] */ ArrayOf<Char32> * pText,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

private:
    // TODO: Add your private member variables here.
};

#endif // __CNUMERICSHAPER_H__
