
#ifndef __CINPUTBINDRESULT_H__
#define __CINPUTBINDRESULT_H__

#include "_CInputBindResult.h"

CarClass(CInputBindResult)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CINPUTBINDRESULT_H__
