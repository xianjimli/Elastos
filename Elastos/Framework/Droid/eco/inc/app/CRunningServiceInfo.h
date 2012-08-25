
#ifndef __CRUNNINGSERVICEINFO_H__
#define __CRUNNINGSERVICEINFO_H__

#include "_CRunningServiceInfo.h"

CarClass(CRunningServiceInfo)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CRUNNINGSERVICEINFO_H__
