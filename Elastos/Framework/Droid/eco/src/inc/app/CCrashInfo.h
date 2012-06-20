
#ifndef __CCRASHINFO_H__
#define __CCRASHINFO_H__

#include "_CCrashInfo.h"

CarClass(CCrashInfo)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CCRASHINFO_H__
