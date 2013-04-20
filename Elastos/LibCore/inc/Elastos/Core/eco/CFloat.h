
#ifndef __CFLOAT_H__
#define __CFLOAT_H__

#include "_CFloat.h"


CarClass(CFloat)
{
public:
    CARAPI constructor(
        /* [in] */ Float value);

    CARAPI GetValue(
        /* [out] */ Float* value);

private:
    Float mValue;
};


#endif //__CFLOAT_H__
