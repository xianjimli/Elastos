
#include "webkit/CMockGeolocation.h"

//CMockGeolocation* CMockGeolocation::sMockGeolocation;

ECode CMockGeolocation::SetPosition(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double accuracy)
{
    // Global instance of a MockGeolocation
    NativeSetPosition(latitude, longitude, accuracy);

    return NOERROR;
}

ECode CMockGeolocation::SetError(
    /* [in] */ Int32 code,
    /* [in] */ CString message)
{
    // This should only ever be called on the WebKit thread.
    NativeSetError(code, (String)message);

    return NOERROR;
}

ECode CMockGeolocation::GetInstance(
    /* [out] */ IMockGeolocation** instance)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

// Native functions
CARAPI_(void) CMockGeolocation::NativeSetPosition(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double accuracy)
{}

CARAPI_(void) CMockGeolocation::NativeSetError(
    /* [in] */ Int32 code,
    /* [in] */ const String& message)
{}