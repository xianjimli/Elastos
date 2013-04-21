
#include "hardware/CSensorManagerListenerDelegate.h"
#include "hardware/CSensorEvent.h"
#include "hardware/CSensor.h"

IHandler* CSensorManagerListenerDelegate::CHandler::New()
{
    IHandler* handler = new CSensorManagerListenerDelegate::CHandler();
    if (!handler) return NULL;

    handler->AddRef();

    return handler;
}

UInt32 CSensorManagerListenerDelegate::CHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSensorManagerListenerDelegate::CHandler::Release()
{
    return ElRefBase::Release();
}

PInterface CSensorManagerListenerDelegate::CHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IHandler) {
        return (IHandler*)this;
    }

    return NULL;
}

ECode CSensorManagerListenerDelegate::CHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IHandler*)this) {
        *pIID = EIID_IHandler;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CSensorManagerListenerDelegate::CHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    // TODO: ALEX need IMessage.obj
    return E_NOT_IMPLEMENTED;
#if 0
    AutoPtr<ISensorEvent> t = (ISensorEvent*)msg.obj;
    final int handle = t.sensor.getHandle();

    switch (t.sensor.getType()) {
        // Only report accuracy for sensors that support it.
        case Sensor.TYPE_MAGNETIC_FIELD:
        case Sensor.TYPE_ORIENTATION:
            // call onAccuracyChanged() only if the value changes
            final int accuracy = mSensorAccuracies.get(handle);
            if ((t.accuracy >= 0) && (accuracy != t.accuracy)) {
                mSensorAccuracies.put(handle, t.accuracy);
                mSensorEventListener.onAccuracyChanged(t.sensor, t.accuracy);
            }
            break;
        default:
            // For other sensors, just report the accuracy once
            if (mFirstEvent.get(handle) == false) {
                mFirstEvent.put(handle, true);
                mSensorEventListener.onAccuracyChanged(
                        t.sensor, SENSOR_STATUS_ACCURACY_HIGH);
            }
            break;
    }

    mSensorEventListener.onSensorChanged(t);
    returnToPool(t);
#endif
}

ECode CSensorManagerListenerDelegate::CreateSensorEvent(
    /* [out] */ ISensorEvent** sensorEvent)
{
    return CSensorEvent::New(3, sensorEvent);
}

ECode CSensorManagerListenerDelegate::GetFromPool(
    /* [out] */ ISensorEvent** sensorEvent)
{
    VALIDATE_NOT_NULL(sensorEvent);

    AutoPtr<ISensorEvent> t;

    SelfLock();
    // remove the array from the pool
    t = mValuesPool;
    mValuesPool = NULL;
    SelfUnlock();

    if (t == NULL) {
        // the pool was empty, we need a new one
        FAIL_RETURN(CreateSensorEvent((ISensorEvent**)&t));
    }

    *sensorEvent = t;
    (*sensorEvent)->AddRef();

    return NOERROR;
}

ECode CSensorManagerListenerDelegate::ReturnToPool(
    /* [in] */ ISensorEvent* t)
{
    SelfLock();
    // put back the array into the pool
    if (mValuesPool == NULL) {
        mValuesPool = t;
    }
    SelfUnlock();

    return NOERROR;
}

ECode CSensorManagerListenerDelegate::GetListener(
    /* [out] */ IInterface** listener)
{
    VALIDATE_NOT_NULL(listener);

    *listener = mSensorEventListener;
    (*listener)->AddRef();

    return NOERROR;
}

ECode CSensorManagerListenerDelegate::AddSensor(
    /* [in] */ ISensor* sensor)
{
    Int32 key;
    FAIL_RETURN(((CSensor*)sensor)->GetHandle(&key));

    mSensors.Put(key, TRUE);
    FAIL_RETURN(mSensorList->Add(sensor));

    return NOERROR;
}

ECode CSensorManagerListenerDelegate::RemoveSensor(
    /* [in] */ ISensor* sensor,
    /* [out] */ Int32* size)
{
    Int32 key;
    FAIL_RETURN(((CSensor*)sensor)->GetHandle(&key));

    mSensors.Delete(key);
    mSensorList->Remove(sensor);

    if (size) *size = mSensors.Size();

    return NOERROR;
}

ECode CSensorManagerListenerDelegate::HasSensor(
    /* [in] */ ISensor* sensor,
    /* [out] */ Boolean* hasSensor)
{
    VALIDATE_NOT_NULL(hasSensor);

    Int32 key;
    FAIL_RETURN(((CSensor*)sensor)->GetHandle(&key));

    *hasSensor = mSensors.Get(key, FALSE);

    return NOERROR;
}

ECode CSensorManagerListenerDelegate::GetSensors(
    /* [out] */ IObjectContainer** list)
{
    VALIDATE_NOT_NULL(list);

    *list = mSensorList;
    (*list)->AddRef();

    return NOERROR;
}

ECode CSensorManagerListenerDelegate::OnSensorChangedLocked(
    /* [in] */ ISensor* sensor,
    /* [in] */ const ArrayOf<Float>& values,
    /* [in] */ const ArrayOf<Int64>& timestamp,
    /* [in] */ Int32 accuracy)
{
    AutoPtr<ISensorEvent> t;
    FAIL_RETURN(GetFromPool((ISensorEvent**)&t));

    // TODO: ALEX need Message.what
    return E_NOT_IMPLEMENTED;
#if 0
    final float[] v = t.values;
    v[0] = values[0];
    v[1] = values[1];
    v[2] = values[2];
    t.timestamp = timestamp[0];
    t.accuracy = accuracy;
    t.sensor = sensor;
    Message msg = Message.obtain();
    msg.what = 0;
    msg.obj = t;
    mHandler.sendMessage(msg);
#endif
}

ECode CSensorManagerListenerDelegate::constructor(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ IHandler* handler)
{
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&mSensorList));

    mSensorEventListener = listener;

    // TODO: ALEX need ILooper
    return E_NOT_IMPLEMENTED;
    // ILooper looper = (handler != null) ? handler.getLooper() : mMainLooper;

    // currently we create one Handler instance per listener, but we could
    // have one per looper (we'd need to pass the ListenerDelegate
    // instance to handleMessage and keep track of them separately).
    mHandler = CHandler::New();
    if (mHandler == NULL) return E_OUT_OF_MEMORY;

    AddSensor(sensor);

    return NOERROR;
}

