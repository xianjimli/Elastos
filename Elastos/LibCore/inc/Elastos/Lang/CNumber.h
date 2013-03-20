
#ifndef __CNUMBER_H__
#define __CNUMBER_H__

#include "_CNumber.h"
#include "Number.h"
CarClass(CNumber), public Number
{
public:
    CARAPI ByteValue(
        /* [out] */ Byte * pValue);

    CARAPI DoubleValue(
        /* [out] */ Double * pValue);

    CARAPI FloatValue(
        /* [out] */ Float * pValue);

    CARAPI Int32Value(
        /* [out] */ Int32 * pValue);

    CARAPI Int64Value(
        /* [out] */ Int64 * pValue);

    CARAPI Int16Value(
        /* [out] */ Int16 * pValue);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CNUMBER_H__
