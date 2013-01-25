
#ifndef __CMOCKGEOLOCATION_H__
#define __CMOCKGEOLOCATION_H__

#include "_CMockGeolocation.h"

CarClass(CMockGeolocation)
{
public:
    CARAPI SetPosition(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Double accuracy);

    CARAPI SetError(
        /* [in] */ Int32 code,
        /* [in] */ CString message);

    CARAPI GetInstance(
        /* [out] */ IMockGeolocation ** ppInstance);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMOCKGEOLOCATION_H__
