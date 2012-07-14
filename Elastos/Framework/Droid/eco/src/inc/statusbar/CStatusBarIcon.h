
#ifndef __CSTATUSBARICON_H__
#define __CSTATUSBARICON_H__

#include "_CStatusBarIcon.h"

using namespace Elastos;

CarClass(CStatusBarIcon)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& iconPackage,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 iconLevel);

    CARAPI constructor(
        /* [in] */ const String& iconPackage,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 iconLevel,
        /* [in] */ Int32 number);

    CARAPI constructor(
        /* [in] */ IParcel* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSTATUSBARICON_H__

