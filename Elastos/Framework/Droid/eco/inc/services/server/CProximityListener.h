
#ifndef __CPROXIMITYLISTENER_H__
#define __CPROXIMITYLISTENER_H__

#include "_CProximityListener.h"

class CLocationManagerService;

// Listener for receiving locations to trigger proximity alerts
CarClass(CProximityListener)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 host);

    CARAPI OnLocationChanged(
        /* [in] */ ILocation* loc);

    CARAPI OnProviderEnabled(
        /* [in] */ const String& provider);

    CARAPI OnProviderDisabled(
        /* [in] */ const String& provider);

    CARAPI OnStatusChanged(
        /* [in] */ const String& provider,
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras);

//        CARAPI OnSendFinished(
//            /* [in] */ IPendingIntent* pendingIntent,
//            /* [in] */ IIntent* intent,
//            /* [in] */ Int32 resultCode,
//            /* [in] */ String resultData,
//            /* [in] */ IBundle* resultExtras);

public:
    CLocationManagerService* mHost;
    Boolean mIsGpsAvailable;
};


#endif //__CPROXIMITYLISTENER_H__
