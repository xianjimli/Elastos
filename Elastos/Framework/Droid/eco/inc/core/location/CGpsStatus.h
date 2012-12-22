
#ifndef __CGPSSTATUS_H__
#define __CGPSSTATUS_H__

#include "ext/frameworkdef.h"
#include "_CGpsStatus.h"
#include <elastos/AutoPtr.h>

CarClass(CGpsStatus)
{
private:
    static const Int32 NUM_SATELLITES = 255;

public:
    CGpsStatus();

    CARAPI SetStatus(
        /* [in] */ Int32 svCount,
        /* [in] */ const ArrayOf<Int32>& prns,
        /* [in] */ const ArrayOf<Float>& snrs,
        /* [in] */ const ArrayOf<Float>& elevations,
        /* [in] */ const ArrayOf<Float>& azimuths,
        /* [in] */ Int32 ephemerisMask,
        /* [in] */ Int32 almanacMask,
        /* [in] */ Int32 usedInFixMask);

    CARAPI SetStatusEx(
        /* [in] */ IGpsStatus* status);

    CARAPI SetTimeToFirstFix(
        /* [in] */ Int32 ttff);

    CARAPI GetTimeToFirstFix(
        /* [out] */ Int32* time);

    CARAPI GetSatellites(
        /* [out] */ IObjectEnumerator** satellites);

    CARAPI GetMaxSatellites(
        /* [out] */ Int32* maxNumber);

    CARAPI constructor();

private:
    /* These package private values are modified by the LocationManager class */
    Int32 mTimeToFirstFix;
    AutoPtr<IObjectContainer> mSatellites;//GpsSatellite mSatellites[] = new GpsSatellite[NUM_SATELLITES];
    AutoPtr<IObjectEnumerator> mSatellitesEnum;
};

#endif // __CGPSSTATUS_H__

