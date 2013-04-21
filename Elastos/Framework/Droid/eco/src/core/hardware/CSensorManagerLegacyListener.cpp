
#include "ext/frameworkext.h"
#include <elastos/Math.h>
#include "hardware/CSensorManagerLegacyListener.h"
#include "hardware/CSensorEvent.h"
#include "hardware/CSensor.h"
#include "hardware/CSensorManagerHelper.h"

using namespace Elastos::Core;

Float LmsFilter::Filter(
    /* [in] */ Int64 time,
    /* [in] */ Float in)
{
    Float v = in;
    Float ns = 1.0f / 1000000000.0f;
    Float t = time*ns;
    Float v1 = mV[mIndex];
    if ((v-v1) > 180) {
        v -= 360;
    } else if ((v1-v) > 180) {
        v += 360;
    }
    /* Manage the circular buffer, we write the data twice spaced
     * by COUNT values, so that we don't have to copy the array
     * when it's full
     */
    mIndex++;
    if (mIndex >= LmsFilter_COUNT*2)
        mIndex = LmsFilter_COUNT;
    mV[mIndex] = v;
    mT[mIndex] = t;
    mV[mIndex-LmsFilter_COUNT] = v;
    mT[mIndex-LmsFilter_COUNT] = t;

    Float A, B, C, D, E;
    Float a, b;
    Int32 i;

    A = B = C = D = E = 0;
    for (i=0 ; i<LmsFilter_COUNT-1 ; i++) {
        Int32 j = mIndex - 1 - i;
        Float Z = mV[j];
        Float T = 0.5f*(mT[j] + mT[j+1]) - t;
        Float dT = mT[j] - mT[j+1];
        dT *= dT;
        A += Z*dT;
        B += T*(T*dT);
        C +=   (T*dT);
        D += Z*(T*dT);
        E += dT;
    }
    b = (A*B + C*D) / (E*B + C*C);
    a = (E*b - A) / C;
    Float f = b + LmsFilter_PREDICTION_TIME*a;

    // Normalize
    f *= (1.0f / 360.0f);
    if (((f>=0)?f:-f) >= 0.5f)
        f = f - (Float)Math::Ceil(f + 0.5f) + 1.0f;
    if (f < 0)
        f += 1.0f;
    f *= 360.0f;
    return f;
}

CSensorManagerLegacyListener::CSensorManagerLegacyListener()
    : mSensors(0)
{
}

ECode CSensorManagerLegacyListener::OnSensorChanged(
    /* [in] */ ISensorEvent* event)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;

    ArrayOf_<Float, 6> v;
    v.Copy(&mValues);
    ArrayOf<Float>* values;
    FAIL_RETURN(event->GetValues(&values));
    v[0] = (*values)[0];
    v[1] = (*values)[1];
    v[2] = (*values)[2];

    AutoPtr<ISensor> sensor;
    FAIL_RETURN(event->GetSensor((ISensor**)&sensor));
    Int32 legacyType;
    FAIL_RETURN(((CSensor*)(ISensor*)sensor)->GetLegacyType(&legacyType));

    AutoPtr<ISensorManagerHelper> helper;
    FAIL_RETURN(CSensorManagerHelper::AcquireSingleton(
        (ISensorManagerHelper**)&helper));
    Int32 rotation;
    FAIL_RETURN(((CSensorManagerHelper*)(ISensorManagerHelper*)helper)->GetRotation(&rotation));
    MapSensorDataToWindow(legacyType, v, rotation);

    Int32 type;
    FAIL_RETURN(sensor->GetType(&type));

    if (type == Sensor_TYPE_ORIENTATION) {
        if ((mSensors & SensorManager_SENSOR_ORIENTATION_RAW)!=0) {
            // mTarget.onSensorChanged(SensorManager_SENSOR_ORIENTATION_RAW, v);
        }
        if ((mSensors & SensorManager_SENSOR_ORIENTATION)!=0) {
            // v[0] = mYawfilter.Filter(event.timestamp, v[0]);
            // mTarget.onSensorChanged(SensorManager_SENSOR_ORIENTATION, v);
        }
    } else {
        // mTarget.onSensorChanged(legacyType, v);
    }

    return NOERROR;
}

ECode CSensorManagerLegacyListener::OnAccuracyChanged(
    /* [in] */ ISensor * pSensor,
    /* [in] */ Int32 accuracy)
{
    // try {
    // TODO: ALEX need ISensorListener
    return E_NOT_IMPLEMENTED;
    // mTarget.onAccuracyChanged(sensor.getLegacyType(), accuracy);
    // } catch (AbstractMethodError e) {
        // old app that doesn't implement this method
        // just ignore it.
    // }
}

/*
 * Helper function to convert the specified sensor's data to the windows's
 * coordinate space from the device's coordinate space.
 *
 * output: 3,4,5: values in the old API format
 *         0,1,2: transformed values in the old API format
 *
 */
Void CSensorManagerLegacyListener::MapSensorDataToWindow(
    /* [in] */ Int32 sensor,
    /* [in] */ ArrayOf<Float>& values,
    /* [in] */ Int32 orientation)
{
    Float x = values[0];
    Float y = values[1];
    Float z = values[2];

    switch (sensor) {
        case SensorManager_SENSOR_ORIENTATION:
        case SensorManager_SENSOR_ORIENTATION_RAW:
            z = -z;
            break;
        case SensorManager_SENSOR_ACCELEROMETER:
            x = -x;
            y = -y;
            z = -z;
            break;
        case SensorManager_SENSOR_MAGNETIC_FIELD:
            x = -x;
            y = -y;
            break;
    }
    values[0] = x;
    values[1] = y;
    values[2] = z;
    values[3] = x;
    values[4] = y;
    values[5] = z;

    if ((orientation & Surface_ROTATION_90) != 0) {
        // handles 90 and 270 rotation
        switch (sensor) {
            case SensorManager_SENSOR_ACCELEROMETER:
            case SensorManager_SENSOR_MAGNETIC_FIELD:
                values[0] =-y;
                values[1] = x;
                values[2] = z;
                break;
            case SensorManager_SENSOR_ORIENTATION:
            case SensorManager_SENSOR_ORIENTATION_RAW:
                values[0] = x + ((x < 270) ? 90 : -270);
                values[1] = z;
                values[2] = y;
                break;
        }
    }
    if ((orientation & Surface_ROTATION_180) != 0) {
        x = values[0];
        y = values[1];
        z = values[2];
        // handles 180 (flip) and 270 (flip + 90) rotation
        switch (sensor) {
            case SensorManager_SENSOR_ACCELEROMETER:
            case SensorManager_SENSOR_MAGNETIC_FIELD:
                values[0] =-x;
                values[1] =-y;
                values[2] = z;
                break;
            case SensorManager_SENSOR_ORIENTATION:
            case SensorManager_SENSOR_ORIENTATION_RAW:
                values[0] = (x >= 180) ? (x - 180) : (x + 180);
                values[1] =-y;
                values[2] =-z;
                break;
        }
    }
}

