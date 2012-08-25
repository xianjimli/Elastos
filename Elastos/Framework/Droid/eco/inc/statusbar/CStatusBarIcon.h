
#ifndef __CSTATUSBARICON_H__
#define __CSTATUSBARICON_H__

#include "_CStatusBarIcon.h"

using namespace Elastos;

CarClass(CStatusBarIcon)
{
public:
    CStatusBarIcon();

    CARAPI Clone(
        /* [out] */ IStatusBarIcon** icon);

    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& iconCapsule,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 iconLevel);

    CARAPI constructor(
        /* [in] */ const String& iconCapsule,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 iconLevel,
        /* [in] */ Int32 number);

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    String mIconCapsule;
    Int32 mIconId;
    Int32 mIconLevel;
    Boolean mVisible;
    Int32 mNumber;
};

#endif // __CSTATUSBARICON_H__

