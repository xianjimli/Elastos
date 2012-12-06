
#ifndef __CCURSORFILTER_H__
#define __CCURSORFILTER_H__

#include "_CCursorFilter.h"
#include "widget/CursorFilter.h"

CarClass(CCursorFilter), public CursorFilter
{
public:
    CARAPI SetDelayer(
        /* [in] */ IFilterDelayer * pDelayer);

    CARAPI DoFilter(
        /* [in] */ ICharSequence * pConstraint);

    CARAPI DoFilterEx(
        /* [in] */ ICharSequence * pConstraint,
        /* [in] */ IFilterListener * pListener);

    CARAPI ConvertResultToString(
        /* [in] */ IInterface * pResultValue,
        /* [out] */ ICharSequence ** ppCs);

    CARAPI constructor(
        /* [in] */ ICursorFilterClient * pClient);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCURSORFILTER_H__
