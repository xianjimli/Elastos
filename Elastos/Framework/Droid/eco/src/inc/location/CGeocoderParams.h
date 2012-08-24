
#ifndef __CGEOCODERPARAMS_H__
#define __CGEOCODERPARAMS_H__

#include "_CGeocoderParams.h"

using namespace Elastos;

CarClass(CGeocoderParams)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* ctx
        /* [in]  ILocale* locale*/);

private:
    // TODO: Add your private member variables here.
};

#endif // __CGEOCODERPARAMS_H__

