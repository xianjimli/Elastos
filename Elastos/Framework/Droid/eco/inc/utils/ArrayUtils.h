
#ifndef __ARRAYUTILS_H__
#define __ARRAYUTILS_H__

#include "ext/frameworkext.h"

class ArrayUtils
{
public:
    static CARAPI_(Int32) IdealByteArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealBooleanArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealInt16ArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealChar8ArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealInt32ArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealFloatArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealObjectArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealInt64ArraySize(
        /* [in] */ Int32 need);
};

#endif //__ARRAYUTILS_H__