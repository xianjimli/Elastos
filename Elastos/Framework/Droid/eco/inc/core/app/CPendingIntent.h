
#ifndef __CPENDINGINTENT_H__
#define __CPENDINGINTENT_H__

#include "_CPendingIntent.h"

CarClass(CPendingIntent)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CPENDINGINTENT_H__
