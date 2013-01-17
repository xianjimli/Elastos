
#ifndef __CBOOLEAN_H__
#define __CBOOLEAN_H__

#include "_CBoolean.h"

CarClass(CBoolean)
{
public:
    CARAPI constructor(
        /* [in] */ Boolean value);

    CARAPI GetValue(
        /* [out] */ Boolean* value);

private:
    Boolean mValue;
};

#endif //__CBOOLEAN_H__
