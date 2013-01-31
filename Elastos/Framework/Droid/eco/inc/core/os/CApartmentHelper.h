
#ifndef __CAPARTMENTHELPER_H__
#define __CAPARTMENTHELPER_H__

#include "_CApartmentHelper.h"

CarClass(CApartmentHelper)
{
public:
    CARAPI GetDefaultApartment(
        /* [out] */ IApartment** apartment);

    CARAPI GetNativeMessageQueue(
        /* [out] */ Handle32* messageQueue);
};


#endif //__CAPARTMENTHELPER_H__
