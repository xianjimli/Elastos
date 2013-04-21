
#ifndef __CSENSORMANAGERLISTENERDELEGATE_H__
#define __CSENSORMANAGERLISTENERDELEGATE_H__

#include "_CSensorManagerListenerDelegate.h"
#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "utils/SparseInt32Array.h"

CarClass(CSensorManagerListenerDelegate)
{
public:
    // TODO: ALEX need CHandler
    class CHandler
        : public IHandler
        , public ElRefBase
    {
    public:
        static IHandler* New();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        // @Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    };

public:
    CARAPI constructor(
        /* [in] */ ISensorEventListener * pListener,
        /* [in] */ ISensor * pSensor,
        /* [in] */ IHandler * pHandler);

public:
    CARAPI CreateSensorEvent(
        /* [out] */ ISensorEvent** sensorEvent);

    CARAPI GetFromPool(
        /* [out] */ ISensorEvent** sensorEvent);

    CARAPI ReturnToPool(
        /* [in] */ ISensorEvent* t);

    CARAPI GetListener(
        /* [out] */ IInterface** listener);

    CARAPI AddSensor(
        /* [in] */ ISensor* sensor);

    CARAPI RemoveSensor(
        /* [in] */ ISensor* sensor,
        /* [out] */ Int32* size);

    CARAPI HasSensor(
        /* [in] */ ISensor* sensor,
        /* [out] */ Boolean* hasSensor);

    CARAPI GetSensors(
        /* [out] */ IObjectContainer** list);

    CARAPI OnSensorChangedLocked(
        /* [in] */ ISensor* sensor,
        /* [in] */ const ArrayOf<Float>& values,
        /* [in] */ const ArrayOf<Int64>& timestamp,
        /* [in] */ Int32 accuracy);

private:
    AutoPtr<ISensorEventListener> mSensorEventListener;
    AutoPtr<IObjectContainer> mSensorList;
    AutoPtr<IHandler> mHandler;
    AutoPtr<ISensorEvent> mValuesPool;
    SparseInt32Array mSensors;
    SparseInt32Array mFirstEvent;
    SparseInt32Array mSensorAccuracies;
};

#endif // __CSENSORMANAGERLISTENERDELEGATE_H__
