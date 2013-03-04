
#ifndef __CPENDINGINTENTHELPER_H__
#define __CPENDINGINTENTHELPER_H__

#include "_CPendingIntentHelper.h"

CarClass(CPendingIntentHelper)
{
public:
    CARAPI GetActivity(
        /* [in] */ IContext* context,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IPendingIntent** activity);

    CARAPI GetBroadcast(
        /* [in] */ IContext* context,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IPendingIntent** broadcast);

    CARAPI GetService(
        /* [in] */ IContext* context,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [out] */ IPendingIntent** service);

    CARAPI WritePendingIntentOrNullToParcel(
        /* [in] */ IPendingIntent* sender,
        /* [in] */ IParcel* outParcel);

    CARAPI ReadPendingIntentOrNullFromParcel(
        /* [in] */ IParcel* inParcel,
        /* [out] */ IPendingIntent** service);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPENDINGINTENTHELPER_H__
