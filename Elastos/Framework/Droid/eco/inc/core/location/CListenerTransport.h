
#ifndef __CLISTENERTRANSPORT_H__
#define __CLISTENERTRANSPORT_H__

#include "_CListenerTransport.h"
#include <elastos/AutoPtr.h>

class LocationManager;

CarClass(CListenerTransport)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 host,
        /* [in] */ ILocalLocationListener* listener,
        /* [in] */ IApartment* apartment);

    CARAPI OnLocationChanged(
        /* [in] */ ILocation* location);

    CARAPI OnStatusChanged(
        /* [in] */ const String& provider,
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras);

    CARAPI OnProviderEnabled(
        /* [in] */ const String& provider);

    CARAPI OnProviderDisabled(
        /* [in] */ const String& provider);

private:
    CARAPI HandleLocationChanged(
        /* [in] */ ILocation* location);

    CARAPI HandleStatusChanged(
        /* [in] */ const String& provider,
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras);

    CARAPI HandleProviderEnabled(
        /* [in] */ const String& provider);

    CARAPI HandleProviderDisabled(
        /* [in] */ const String& provider);

private:
    LocationManager* mHost;
    AutoPtr<ILocalLocationListener> mListener;
    AutoPtr<IApartment> mListenerHandler;
};

#endif //__CLISTENERTRANSPORT_H__
