
#ifndef __CGEOCODERPARAMS_H__
#define __CGEOCODERPARAMS_H__

#include "ext/frameworkdef.h"
#include "_CGeocoderParams.h"
#include <elastos/AutoPtr.h>

CarClass(CGeocoderParams)
{
public:
    CARAPI GetLocale(
        /* [out] */ ILocale** locale);

    /**
     * returns the package name of the Geocoder's client
     */
    CARAPI GetClientCapsule(
        /* [out] */ String* name);

    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ ILocale* locale);

private:
    AutoPtr<ILocale> mLocale;
    String mCapsuleName;
};

#endif // __CGEOCODERPARAMS_H__

