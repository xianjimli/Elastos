
#ifndef __CINTEGER32_H__
#define __CINTEGER32_H__

#include "_CInteger32.h"

CarClass(CInteger32)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 value);

    CARAPI GetValue(
        /* [out] */ Int32* value);

private:
    Int32 mValue;
};

#endif //__CINTEGER32_H__
