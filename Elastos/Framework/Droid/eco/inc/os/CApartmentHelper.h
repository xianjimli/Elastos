
#ifndef __CAPARTMENTHELPER_H__
#define __CAPARTMENTHELPER_H__

#include "_CApartmentHelper.h"

CarClass(CApartmentHelper)
{
public:
    CARAPI GetDefaultApartment(
        /* [out] */ IApartment** apartment);
};


#endif //__CAPARTMENTHELPER_H__
