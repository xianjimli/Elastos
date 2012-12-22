
#ifndef __CGPSSATELLITE_H__
#define __CGPSSATELLITE_H__

#include "ext/frameworkdef.h"
#include "_CGpsSatellite.h"

CarClass(CGpsSatellite)
{
public:
    CGpsSatellite();

    /**
     * Used by {@link LocationManager#getGpsStatus} to copy LocationManager's
     * cached GpsStatus instance to the client's copy.
     */
    CARAPI SetStatus(
        /* [in] */ IGpsSatellite* satellite);

    /**
     * Returns the PRN (pseudo-random number) for the satellite.
     *
     * @return PRN number
     */
    CARAPI GetPrn(
        /* [out] */ Int32* prn);

    /**
     * Returns the signal to noise ratio for the satellite.
     *
     * @return the signal to noise ratio
     */
    CARAPI GetSnr(
        /* [out] */ Int32* snr);

    /**
     * Returns the elevation of the satellite in degrees.
     * The elevation can vary between 0 and 90.
     *
     * @return the elevation in degrees
     */
    CARAPI GetElevation(
        /* [out] */ Float* elevation);

    /**
     * Returns the azimuth of the satellite in degrees.
     * The azimuth can vary between 0 and 360.
     *
     * @return the azimuth in degrees
     */
    CARAPI GetAzimuth(
        /* [out] */ Float* azimuth);

    /**
     * Returns true if the GPS engine has ephemeris data for the satellite.
     *
     * @return true if the satellite has ephemeris data
     */
    CARAPI HasEphemeris(
        /* [out] */ Boolean* hasEphemeris);

    /**
     * Returns true if the GPS engine has almanac data for the satellite.
     *
     * @return true if the satellite has almanac data
     */
    CARAPI HasAlmanac(
        /* [out] */ Boolean* hasAlmanac);

    /**
     * Returns true if the satellite was used by the GPS engine when
     * calculating the most recent GPS fix.
     *
     * @return true if the satellite was used to compute the most recent fix.
     */
    CARAPI UsedInFix(
        /* [out] */ Boolean* usedInFix);

    CARAPI constructor(
        /* [in] */ Int32 prn);

public:
    /* These package private values are modified by the GpsStatus class */
    Boolean mValid;
    Boolean mHasEphemeris;
    Boolean mHasAlmanac;
    Boolean mUsedInFix;
    Int32 mPrn;
    Float mSnr;
    Float mElevation;
    Float mAzimuth;
};

#endif // __CGPSSATELLITE_H__

