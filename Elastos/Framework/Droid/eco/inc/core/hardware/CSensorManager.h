
#ifndef __CSENSORMANAGER_H__
#define __CSENSORMANAGER_H__

#include "_CSensorManager.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Thread.h>
#include "os/Runnable.h"

CarClass(CSensorManager)
{
public:
    class CSensorThread
    {
    public:
        class CSensorThreadRunnable
            : public Runnable
            , public Object
        {
        public:
            CSensorThreadRunnable(
                /* [in] */ CSensorManager::CSensorThread* sensorThread);

            virtual ~CSensorThreadRunnable();

            // @Override
            CARAPI Run();

        private:
            CARAPI Open(
                /* [out] */ Boolean* result);

            CSensorThreadRunnable() {}

        private:
            CSensorManager::CSensorThread* mSensorThread;
        };

    public:
        // must be called with sListeners lock
        CARAPI StartLocked(
            /* [out] */ Boolean* result);

    private:
        AutoPtr<IThread> mThread;
        Boolean mSensorsReady;
    };

public:
    CARAPI GetSensorList(
        /* [in] */ Int32 type,
        /* [out] */ IObjectContainer** sensorList);

    CARAPI GetDefaultSensor(
        /* [in] */ Int32 type,
        /* [out] */ ISensor** sensor);

    CARAPI UnregisterListener(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor);

    CARAPI UnregisterListener2(
        /* [in] */ ISensorEventListener* listener);

    CARAPI RegisterListener(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 rate,
        /* [out] */ Boolean* result);

    CARAPI RegisterListener2(
        /* [in] */ ISensorEventListener* listener,
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 rate,
        /* [in] */ IHandler* handler,
        /* [out] */ Boolean* result);

    CARAPI OnRotationChanged(
        /* [in] */ Int32 rotation);

public:
    static CARAPI GetRotationMatrix(
        /* [in] */ ArrayOf<Float>* R,
        /* [in] */ ArrayOf<Float>* I,
        /* [in] */ ArrayOf<Float>* gravity,
        /* [in] */ ArrayOf<Float>* geomagnetic,
        /* [out] */ Boolean* result);

    static CARAPI GetInclination(
        /* [in] */ const ArrayOf<Float>& I,
        /* [out] */ Float* inclination);

    static CARAPI RemapCoordinateSystem(
        /* [in] */ ArrayOf<Float>* inR,
        /* [in] */ Int32 X,
        /* [in] */ Int32 Y,
        /* [in] */ ArrayOf<Float>* outR,
        /* [out] */ Boolean* result);

    static CARAPI GetOrientation(
        /* [in] */ const ArrayOf<Float>& R,
        /* [in] */ ArrayOf<Float>* values);

    static CARAPI GetAltitude(
        /* [in] */ Float p0,
        /* [in] */ Float p,
        /* [out] */ Float* altitude);

    static CARAPI GetAngleChange(
        /* [in] */ ArrayOf<Float>* angleChange,
        /* [in] */ const ArrayOf<Float>& R,
        /* [in] */ const ArrayOf<Float>& prevR);

    static CARAPI GetRotationMatrixFromVector(
        /* [in] */ ArrayOf<Float>* R,
        /* [in] */ const ArrayOf<Float>& rotationVector);

    static CARAPI GetQuaternionFromVector(
        /* [in] */ ArrayOf<Float> * Q,
        /* [in] */ const ArrayOf<Float>& rv);

private:
    CARAPI UnregisterListener(
        /* [in] */ PInterface listener,
        /* [in] */ ISensor* sensor);

    CARAPI UnregisterListener(
        /* [in] */ PInterface listener);

    CARAPI EnableSensorLocked(
        /* [in] */ ISensor* sensor,
        /* [in] */ Int32 delay,
        /* [out] */ Boolean* ret);

    CARAPI DisableSensorLocked(
        /* [in] */ ISensor* sensor,
        /* [out] */ Boolean* result);

    static CARAPI RemapCoordinateSystemImpl(
        /* [in] */ ArrayOf<Float>* inR,
        /* [in] */ Int32 X,
        /* [in] */ Int32 Y,
        /* [in] */ ArrayOf<Float>* outR,
        /* [out] */ Boolean* result);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSENSORMANAGER_H__
