
#ifndef __CSTATUSBARNOTIFICATION_H__
#define __CSTATUSBARNOTIFICATION_H__

#include "_CStatusBarNotification.h"

using namespace Elastos;

CarClass(CStatusBarNotification)
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
        /* [in] */ const String& cap,
        /* [in] */ Int32 id,
        /* [in] */ const String& tag,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 initialPid,
        /* [in] */ INotification* notification);

    CARAPI constructor(
        /* [in] */ IParcel* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSTATUSBARNOTIFICATION_H__

