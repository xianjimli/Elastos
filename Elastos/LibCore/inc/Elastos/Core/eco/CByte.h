
#ifndef __CBYTE_H__
#define __CBYTE_H__

#include "_CByte.h"

CarClass(CByte)
{
public:
    CARAPI constructor(
        /* [in] */ Byte value);

    CARAPI GetValue(
        /* [out] */ Byte* value);

private:
    Byte mValue;
};


#endif //__CBYTE_H__
