
#ifndef __CDATESORTER_H__
#define __CDATESORTER_H__

#include "_CDateSorter.h"

CarClass(CDateSorter)
{
public:
    CARAPI GetIndex(
        /* [in] */ Int64 time,
        /* [out] */ Int32 * pLastDay);

    CARAPI GetLabel(
        /* [in] */ Int32 index,
        /* [out] */ String * pLabel);

    CARAPI GetBoundary(
        /* [in] */ Int32 index,
        /* [out] */ Int64 * pBoundary);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    // TODO: Add your private member variables here.
};

#endif // __CDATESORTER_H__
