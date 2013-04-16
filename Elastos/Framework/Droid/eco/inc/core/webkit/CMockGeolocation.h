
#ifndef __CMOCKGEOLOCATION_H__
#define __CMOCKGEOLOCATION_H__

#include "_CMockGeolocation.h"
#include "ext/frameworkext.h"

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
        /* [out] */ IMockGeolocation** instance);

private:
    // Native functions
    static CARAPI_(void) NativeSetPosition(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Double accuracy);

    static CARAPI_(void) NativeSetError(
        /* [in] */ Int32 code,
        /* [in] */ const String& message);

private:
    // Global instance of a MockGeolocation
//    static CMockGeolocation* sMockGeolocation;
};

#endif // __CMOCKGEOLOCATION_H__
