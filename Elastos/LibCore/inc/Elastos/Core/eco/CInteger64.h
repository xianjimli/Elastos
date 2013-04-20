
#ifndef __CINTEGER64_H__
#define __CINTEGER64_H__

#include "_CInteger64.h"


CarClass(CInteger64)
{
public:
    CARAPI constructor(
        /* [in] */ Int64 value);

    CARAPI GetValue(
        /* [out] */ Int64* value);

private:
    Int64 mValue;
};


#endif //__CINTEGER64_H__
