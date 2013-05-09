
#ifndef __CGPSSTATUSLISTENERTRANSPORT_H__
#define __CGPSSTATUSLISTENERTRANSPORT_H__

#include "ext/frameworkext.h"
#include "_CGpsStatusListenerTransport.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>


class LocationManager;

// This class is used to send GPS status events to the client's main thread.
CarClass(CGpsStatusListenerTransport)
{
private:
    class Nmea : public ElRefBase
    {
    public:
        Nmea(
            /* [in] */ Int64 timestamp,
            /* [in] */ const String& nmea);

    public:
        Int64 mTimestamp;
        String mNmea;
    };

public:
    CARAPI constructor(
        /* [in] */ Handle32 host,
        /* [in] */ ILocalGpsStatusListener* listener);

    CARAPI constructor(
        /* [in] */ Handle32 host,
        /* [in] */ ILocalGpsStatusNmeaListener* listener);

    CARAPI OnGpsStarted();

    CARAPI OnGpsStopped();

    CARAPI OnFirstFix(
        /* [in] */ Int32 ttff);

    CARAPI OnSvStatusChanged(
        /* [in] */ Int32 svCount,
        /* [in] */ const ArrayOf<Int32>& prns,
        /* [in] */ const ArrayOf<Float>& snrs,
        /* [in] */ const ArrayOf<Float>& elevations,
        /* [in] */ const ArrayOf<Float>& azimuths,
        /* [in] */ Int32 ephemerisMask,
        /* [in] */ Int32 almanacMask,
        /* [in] */ Int32 usedInFixMask);

    CARAPI OnNmeaReceived(
        /* [in] */ Int64 timestamp,
        /* [in] */ const String& nmea);

private:
    CARAPI HandleGpsChanged(
        /* [in] */ Int32 event);

    CARAPI HandleNmeaReceived();

private:
    LocationManager* mHost;
    AutoPtr<ILocalGpsStatusListener> mListener;
    AutoPtr<ILocalGpsStatusNmeaListener> mNmeaListener;
    List< AutoPtr<Nmea> > mNmeaBuffer;
    Mutex mNmeaBufferLock;
    AutoPtr<IApartment> mGpsHandler;
};


#endif //__CGPSSTATUSLISTENERTRANSPORT_H__
