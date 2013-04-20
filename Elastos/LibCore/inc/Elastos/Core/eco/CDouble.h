
#ifndef __CDOUBLE_H__
#define __CDOUBLE_H__

#include "_CDouble.h"


CarClass(CDouble)
{
public:
    CARAPI constructor(
        /* [in] */ Double value);

    CARAPI GetValue(
        /* [out] */ Double* value);

private:
    Double mValue;
};


#endif //__CDOUBLE_H__
