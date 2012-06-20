
#ifndef __CSERVICEAPARTMENT_H__
#define __CSERVICEAPARTMENT_H__

#include "_CServiceApartment.h"
#include "utils/CApartment.h"
#include <elastos/AutoPtr.h>

CarClass(CServiceApartment)
{
public:
    CServiceApartment();
    
    ~CServiceApartment();
    
    CARAPI ScheduleInvokeService(
        /* [in] */ IInterface* service,
        /* [in] */ Handle32 data,
        /* [in] */ IParcel* params);

private:
    CARAPI PerformInvokeService(
        /* [in] */ IInterface* service,
        /* [in] */ Handle32 data,
        /* [in] */ IParcel* params);

private:
    AutoPtr<IApartment>  mApartment;
};

#endif //__CSERVICEAPARTMENT_H__
