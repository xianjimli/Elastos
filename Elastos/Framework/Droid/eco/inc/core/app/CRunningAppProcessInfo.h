
#ifndef __CRUNNINGAPPPROCESSINFO_H__
#define __CRUNNINGAPPPROCESSINFO_H__

#include "_CRunningAppProcessInfo.h"

CarClass(CRunningAppProcessInfo)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CRUNNINGAPPPROCESSINFO_H__
