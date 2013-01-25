
#ifndef __CGEOLOCATIONPERMISSIONS_H__
#define __CGEOLOCATIONPERMISSIONS_H__

#include "_CGeolocationPermissions.h"

CarClass(CGeolocationPermissions)
{
public:
    CARAPI GetInstance(
        /* [out] */ IGeolocationPermissions ** ppInstance);

    CARAPI CreateUIHandler();

    CARAPI CreateHandler();

    CARAPI GetOrigins(
        /* [in] */ IValueCallback * pCallBack);

    CARAPI GetAllowed(
        /* [in] */ CString origin,
        /* [in] */ IValueCallback * pCallBack);

    CARAPI Clear(
        /* [in] */ CString origin);

    CARAPI Allow(
        /* [in] */ CString origin);

    CARAPI ClearAll();

private:
    // TODO: Add your private member variables here.
};

#endif // __CGEOLOCATIONPERMISSIONS_H__
