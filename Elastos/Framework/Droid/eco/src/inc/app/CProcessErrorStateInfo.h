
#ifndef __CPROCESSERRORSTATEINFO_H__
#define __CPROCESSERRORSTATEINFO_H__

#include "_CProcessErrorStateInfo.h"

CarClass(CProcessErrorStateInfo)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CPROCESSERRORSTATEINFO_H__
