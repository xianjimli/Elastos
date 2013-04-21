
#ifndef __CSENSORMANAGERLEGACYLISTENER_H__
#define __CSENSORMANAGERLEGACYLISTENER_H__

#include "_CSensorManagerLegacyListener.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

#define LmsFilter_SENSORS_RATE_MS (20)
#define LmsFilter_COUNT (12)
#define LmsFilter_PREDICTION_RATIO ((Float)(1.0f/3.0f))
#define LmsFilter_PREDICTION_TIME ((Float)(LmsFilter_SENSORS_RATE_MS*LmsFilter_COUNT/1000.0f)*LmsFilter_PREDICTION_RATIO)

class LmsFilter
{
public:
    LmsFilter()
    {
        mIndex = LmsFilter_COUNT;
    }

    Float Filter(
        /* [in] */ Int64 time,
        /* [in] */ Float in);

private:
    Float mV[LmsFilter_COUNT*2];
    Float mT[LmsFilter_COUNT*2];
    Int32 mIndex;
};

CarClass(CSensorManagerLegacyListener)
{
public:
    CSensorManagerLegacyListener();

    // @SuppressWarnings("deprecation")
    CARAPI OnSensorChanged(
        /* [in] */ ISensorEvent* event);

    // @SuppressWarnings("deprecation")
    CARAPI OnAccuracyChanged(
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 accuracy);

private:
    CARAPI RegisterSensor(
        /* [in] */ Int32 legacyType);

    CARAPI UnregisterSensor(
        /* [in] */ Int32 legacyType,
        /* [out] */ Boolean* result);

    Void MapSensorDataToWindow(
        /* [in] */ Int32 sensor,
        /* [in] */ ArrayOf<Float>& values,
        /* [in] */ Int32 orientation);

private:
    ArrayOf_<Float, 6> mValues;
    // @SuppressWarnings("deprecation")
    // AutoPtr<ISensorListener> mTarget;
    Int32 mSensors;
    LmsFilter mYawfilter;
};

#endif // __CSENSORMANAGERLEGACYLISTENER_H__
