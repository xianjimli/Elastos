
#ifndef  __CBINDER_H__
#define  __CBINDER_H__

#include "ext/frameworkext.h"
#include "_CBinder.h"

CarClass(CBinder)
{
public:
    CARAPI constructor();

    CARAPI GetDescription(
        /* [out] */ String* description);
};

#endif  //__CBINDER_H__
