
#ifndef __CAPPLICATIONERRORREPORT_H__
#define __CAPPLICATIONERRORREPORT_H__

#include "_CApplicationErrorReport.h"

CarClass(CApplicationErrorReport)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CAPPLICATIONERRORREPORT_H__
