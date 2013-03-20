
#ifndef __CPARSEPOSITION_H__
#define __CPARSEPOSITION_H__

#include "_CParsePosition.h"
#include "ParsePosition.h"

CarClass(CParsePosition), public ParsePosition
{
public:
    CARAPI constructor(
        /* [in] */ Int32 index);

    CARAPI GetErrorIndex(
        /* [out] */ Int32 * pErrorIndex);

    CARAPI GetIndex(
        /* [out] */ Int32 * pIndex);

    CARAPI SetErrorIndex(
        /* [in] */ Int32 index);

    CARAPI SetIndex(
        /* [in] */ Int32 index);
private:
    // TODO: Add your private member variables here.
};

#endif // __CPARSEPOSITION_H__
