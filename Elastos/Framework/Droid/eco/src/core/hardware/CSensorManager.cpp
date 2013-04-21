
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Mutex.h>
#include <elastos/Math.h>
#include "os/Process.h"
#include "utils/SparseArray.h"
#include "hardware/CSensorManager.h"
#include "hardware/CSensorManagerListenerDelegate.h"
#include "hardware/CSensor.h"

using namespace Elastos::Core;

static const String TAG("SensorManager");
static ArrayOf_<Float, 16> mTempMatrix;
static Mutex mTempMatrixLock;
static const Int32 SENSOR_DISABLE = -1;
static Boolean sSensorModuleInitialized = FALSE;
static AutoPtr<IObjectContainer> sFullSensorsList;
static Mutex sFullSensorListLock;
static SparseArray sSensorListByType;
static AutoPtr<IWindowManager> sWindowManager;
static Int32 sRotation = Surface_ROTATION_0;
/* The thread and the sensor list are global to the process
 * but the actual thread is spawned on demand */
// TODO: ALEX
static CSensorManager::CSensorThread* sSensorThread;
static Int32 sQueue;

// Used within this module from outside SensorManager, don't make private
static SparseArray sHandleToSensor;
static List<AutoPtr<ISensorManagerListenerDelegate> > sListeners;
static Mutex sListenersLock;

class CStatic
{
public:
    CStatic()
    {
        CObjectContainer::New((IObjectContainer**)&sFullSensorsList);
    }
};

CSensorManager::CSensorThread::CSensorThreadRunnable::CSensorThreadRunnable(
    /* [in] */ CSensorManager::CSensorThread* sensorThread)
{
    mSensorThread = sensorThread;
}

CSensorManager::CSensorThread::CSensorThreadRunnable::~CSensorThreadRunnable()
{
}

ECode CSensorManager::CSensorThread::CSensorThreadRunnable::Run()
{
    //Log.d(TAG, "entering main sensor thread");
    ArrayOf_<Float, 3> values;
    ArrayOf_<Float, 1> status;
    ArrayOf_<Int64, 1> timestamp;

    Process::SetThreadPriority(Process::THREAD_PRIORITY_URGENT_DISPLAY);

    Boolean result;
    FAIL_RETURN(Open(&result));
    if (!result) {
        return NOERROR;
    }

    Lock();
    // we've open the driver, we're ready to open the sensors
    mSensorThread->mSensorsReady = TRUE;
    this->Notify();
    Unlock();

    while (TRUE) {
        // wait for an event
        // TODO: ALEX fix it
        Int32 sensor; // = sensors_data_poll(sQueue, values, status, timestamp);

        Int32 accuracy = status[0];
        sListenersLock.Lock();

        if (sensor == -1 || sListeners.IsEmpty()) {
            // we lost the connection to the event stream. this happens
            // when the last listener is removed or if there is an error
            if (sensor == -1 && !sListeners.IsEmpty()) {
                // log a warning in case of abnormal termination
                // Log.e(TAG, "_sensors_data_poll() failed, we bail out: sensors=" + sensor);
            }
            // we have no more listeners or polling failed, terminate the thread
            // TODO: ALEX fix it
            // sensors_destroy_queue(sQueue);
            sQueue = 0;
            mSensorThread->mThread = NULL;
            sListenersLock.Unlock();
            break;
        }

        AutoPtr<ISensor> sensorObject = (ISensor*)sHandleToSensor.Get(sensor);
        if (sensorObject != NULL) {
            // report the sensor event to all listeners that
            // care about it.
            ECode ec = NOERROR;
            AutoPtr<ISensorManagerListenerDelegate> l;
            List<AutoPtr<ISensorManagerListenerDelegate> >::Iterator it;
            for (it = sListeners.Begin(); it != sListeners.End(); it++) {
                AutoPtr<ISensorManagerListenerDelegate> t = *it;
                CSensorManagerListenerDelegate* c = (CSensorManagerListenerDelegate*)(ISensorManagerListenerDelegate*)t;
                Boolean hasSensor;
                ec = c->HasSensor(sensorObject, &hasSensor);
                if (FAILED(ec)) {
                    sListenersLock.Unlock();
                    return ec;
                }

                // this is asynchronous (okay to call
                // with sListeners lock held).
                c->OnSensorChangedLocked(
                    sensorObject, values, timestamp, accuracy);
            }
        }

        sListenersLock.Unlock();
    }
    //Log.d(TAG, "exiting main sensor thread");

    return NOERROR;
}

ECode CSensorManager::CSensorThread::CSensorThreadRunnable::Open(
    /* [out] */ Boolean* result)
{
    // NOTE: this cannot synchronize on sListeners, since
    // it's held in the main thread at least until we
    // return from here.
    // TODO: ALEX fix it
    // sQueue = sensors_create_queue();
    if (*result) *result = TRUE;

    return NOERROR;
}

ECode CSensorManager::CSensorThread::StartLocked(
    /* [out] */ Boolean* result)
{
    // try {
    if (mThread == NULL) {
        mSensorsReady = FALSE;
        AutoPtr<CSensorThreadRunnable> runnable =
            new CSensorThreadRunnable(this);
        if (runnable == NULL)
            return E_OUT_OF_MEMORY;

        AutoPtr<IThread> thread;
        FAIL_RETURN(CThread::New(
            IRunnable::Probe(runnable), (IThread**)&thread));
        thread->Start();

        runnable->Lock();
        while (mSensorsReady == FALSE) {
            runnable->Wait();
        }
        runnable->Unlock();

        mThread = thread;
    }
    // } catch (InterruptedException e) {
    // }
    if (result) *result = (mThread == NULL) ? FALSE : TRUE;

    return NOERROR;
}

ECode CSensorManager::GetSensorList(
    /* [in] */ Int32 type,
    /* [out] */ IObjectContainer** sensorList)
{
    VALIDATE_NOT_NULL(sensorList);

    // cache the returned lists the first time
    AutoPtr<IObjectContainer> list;
    AutoPtr<IObjectContainer> fullList = sFullSensorsList;

    sFullSensorListLock.Lock();

    PInterface p = sSensorListByType.Get(type);
    if (p == NULL) {
        if (type == Sensor_TYPE_ALL) {
            list = fullList;
        } else {
            if (FAILED(CObjectContainer::New((IObjectContainer**)&list))) {
                sFullSensorListLock.Unlock();
                return E_OUT_OF_MEMORY;
            }

            AutoPtr<IObjectEnumerator> objEnum;
            ECode ec = fullList->GetObjectEnumerator(
                (IObjectEnumerator**)&objEnum);
            if (FAILED(ec)) {
                sFullSensorListLock.Unlock();
                return ec;
            }

            Boolean hasNext;
            while (SUCCEEDED(objEnum->MoveNext(&hasNext)) && hasNext) {
                AutoPtr<IInterface> obj;
                ec = objEnum->Current((IInterface**)&obj);
                if (FAILED(ec)) {
                    sFullSensorListLock.Unlock();
                    return ec;
                }

                ISensor* i = (ISensor*)obj->Probe(EIID_ISensor);
                if (i == NULL) {
                    sFullSensorListLock.Unlock();
                    return E_NO_INTERFACE;
                }

                Int32 temp;
                ec = i->GetType(&temp);
                if (FAILED(ec)) {
                    sFullSensorListLock.Unlock();
                    return ec;
                }

                if (temp == type) {
                    ec = list->Add((PInterface)i);
                    if (FAILED(ec)) {
                        sFullSensorListLock.Unlock();
                        return ec;
                    }
                }
            }
        }

        // TODO: ALEX check it
        // list = Collections.unmodifiableList(list);
        sSensorListByType.Append(type, (PInterface)list);
    }
    else {
        list = (IObjectContainer*)p->Probe(EIID_IObjectContainer);
        if (list == NULL) {
            sFullSensorListLock.Unlock();
            return E_NO_INTERFACE;
        }
    }

    sFullSensorListLock.Unlock();

    *sensorList = list;
    (*sensorList)->AddRef();

    return NOERROR;
}

ECode CSensorManager::GetDefaultSensor(
    /* [in] */ Int32 type,
    /* [out] */ ISensor** sensor)
{
    VALIDATE_NOT_NULL(sensor);

    // TODO: need to be smarter, for now, just return the 1st sensor
    AutoPtr<IObjectContainer> l;
    FAIL_RETURN(GetSensorList(type, (IObjectContainer**)&l));

    Int32 count;
    FAIL_RETURN(l->GetObjectCount(&count));

    if (count == 0) {
        *sensor = NULL;
    }
    else {
        AutoPtr<IObjectEnumerator> objEnum;
        FAIL_RETURN(l->GetObjectEnumerator((IObjectEnumerator**)&objEnum));

        Boolean hasNext;
        if (SUCCEEDED(objEnum->MoveNext(&hasNext)) && hasNext) {
            AutoPtr<IInterface> obj;
            FAIL_RETURN(objEnum->Current((IInterface**)&obj));
            *sensor = (ISensor*)obj->Probe(EIID_ISensor);
        }
        else {
            *sensor = NULL;
        }
    }

    return NOERROR;
}

ECode CSensorManager::UnregisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor)
{
    return UnregisterListener((PInterface)listener, sensor);
}

ECode CSensorManager::UnregisterListener2(
    /* [in] */ ISensorEventListener* listener)
{
    return UnregisterListener((PInterface)listener);
}

ECode CSensorManager::RegisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 rate,
    /* [out] */ Boolean* result)
{
    return RegisterListener(listener, sensor, rate, NULL);
}

ECode CSensorManager::RegisterListener2(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 rate,
    /* [in] */ IHandler* handler,
    /* [out] */ Boolean* ret)
{
    if (listener == NULL || sensor == NULL) {
        if (ret) *ret = FALSE;
        return NOERROR;
    }

    Boolean result = TRUE;
    Int32 delay = -1;
    switch (rate) {
        case SensorManager_SENSOR_DELAY_FASTEST:
            delay = 0;
            break;
        case SensorManager_SENSOR_DELAY_GAME:
            delay = 20000;
            break;
        case SensorManager_SENSOR_DELAY_UI:
            delay = 60000;
            break;
        case SensorManager_SENSOR_DELAY_NORMAL:
            delay = 200000;
            break;
        default:
            delay = rate;
            break;
    }

    ECode ec;
    sListenersLock.Lock();

    // look for this listener in our list
    AutoPtr<ISensorManagerListenerDelegate> l;
    List<AutoPtr<ISensorManagerListenerDelegate> >::Iterator it;
    for (it = sListeners.Begin(); it != sListeners.End(); it++) {
        AutoPtr<ISensorManagerListenerDelegate> t = *it;
        CSensorManagerListenerDelegate* c = (CSensorManagerListenerDelegate*)(ISensorManagerListenerDelegate*)t;
        AutoPtr<IInterface> obj;
        ec = c->GetListener((PInterface*)&obj);
        if (FAILED(ec)) {
            sListenersLock.Unlock();
            return ec;
        }

        if ((PInterface)obj == listener) {
            l = t;
            break;
        }
    }

    // if we don't find it, add it to the list
    if (l == NULL) {
        ec = CSensorManagerListenerDelegate::New(
            listener, sensor, handler, (ISensorManagerListenerDelegate**)&l);
        if (FAILED(ec)) {
            sListenersLock.Unlock();
            return ec;
        }

        sListeners.PushBack(l);
        // if the list is not empty, start our main thread
        if (!sListeners.IsEmpty()) {
            Boolean b;
            ec = sSensorThread->StartLocked(&b);
            if (FAILED(ec)) {
                sListenersLock.Unlock();
                return ec;
            }

            if (b) {
                ec = EnableSensorLocked(sensor, delay, &b);
                if (FAILED(ec)) {
                    sListenersLock.Unlock();
                    return ec;
                }

                if (!b) {
                    // oops. there was an error
                    sListeners.Remove(l);
                    result = FALSE;
                }
            } else {
                // there was an error, remove the listener
                sListeners.Remove(l);
                result = FALSE;
            }
        } else {
            // weird, we couldn't add the listener
            result = FALSE;
        }
    } else {
        CSensorManagerListenerDelegate* c = (CSensorManagerListenerDelegate*)(ISensorManagerListenerDelegate*)l;
        ec = c->AddSensor(sensor);
        if (FAILED(ec)) {
            sListenersLock.Unlock();
            return ec;
        }

        Boolean b;
        ec = EnableSensorLocked(sensor, delay, &b);
        if (FAILED(ec)) {
            sListenersLock.Unlock();
            return ec;
        }

        if (!b) {
            // oops. there was an error
            ec = c->RemoveSensor(sensor, NULL);
            if (FAILED(ec)) {
                sListenersLock.Unlock();
                return ec;
            }
            result = FALSE;
        }
    }

    sListenersLock.Unlock();

    if (ret) *ret = result;

    return NOERROR;
}

ECode CSensorManager::OnRotationChanged(
    /* [in] */ Int32 rotation)
{
    sListenersLock.Lock();

    sRotation = rotation;

    sListenersLock.Unlock();

    return NOERROR;
}

// TODO: ALEX need ILooper
#if 0
/**
 * {@hide}
 */
public SensorManager(Looper mainLooper) {
    mMainLooper = mainLooper;


    synchronized(sListeners) {
        if (!sSensorModuleInitialized) {
            sSensorModuleInitialized = TRUE;

            nativeClassInit();

            sWindowManager = IWindowManager.Stub.asInterface(
                    ServiceManager.getService("window"));
            if (sWindowManager != null) {
                // if it's null we're running in the system process
                // which won't get the rotated values
                try {
                    sRotation = sWindowManager.watchRotation(
                            new IRotationWatcher.Stub() {
                                public void onRotationChanged(int rotation) {
                                    SensorManager.this.onRotationChanged(rotation);
                                }
                            }
                    );
                } catch (RemoteException e) {
                }
            }

            // initialize the sensor list
            sensors_module_init();
            final ArrayList<Sensor> fullList = sFullSensorsList;
            int i = 0;
            do {
                Sensor sensor = new Sensor();
                i = sensors_module_get_next_sensor(sensor, i);

                if (i>=0) {
                    //Log.d(TAG, "found sensor: " + sensor.getName() +
                    //        ", handle=" + sensor.getHandle());
                    sensor.setLegacyType(getLegacySensorType(sensor.getType()));
                    fullList.add(sensor);
                    sHandleToSensor.append(sensor.getHandle(), sensor);
                }
            } while (i>0);

            sSensorThread = new SensorThread();
        }
    }
}
#endif

ECode CSensorManager::GetRotationMatrix(
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ ArrayOf<Float>* I,
    /* [in] */ ArrayOf<Float>* gravity,
    /* [in] */ ArrayOf<Float>* geomagnetic,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(R);
    VALIDATE_NOT_NULL(I);
    VALIDATE_NOT_NULL(gravity);
    VALIDATE_NOT_NULL(geomagnetic);

    // TODO: move this to native code for efficiency
    Float Ax = (*gravity)[0];
    Float Ay = (*gravity)[1];
    Float Az = (*gravity)[2];
    Float Ex = (*geomagnetic)[0];
    Float Ey = (*geomagnetic)[1];
    Float Ez = (*geomagnetic)[2];
    Float Hx = Ey*Az - Ez*Ay;
    Float Hy = Ez*Ax - Ex*Az;
    Float Hz = Ex*Ay - Ey*Ax;
    Float normH = (Float)Math::Sqrt(Hx*Hx + Hy*Hy + Hz*Hz);
    if (normH < 0.1f) {
        // device is close to free fall (or in space?), or close to
        // magnetic north pole. Typical values are  > 100.
        if (result) *result = FALSE;
        return NOERROR;
    }
    Float invH = 1.0f / normH;
    Hx *= invH;
    Hy *= invH;
    Hz *= invH;
    Float invA = 1.0f / (Float)Math::Sqrt(Ax*Ax + Ay*Ay + Az*Az);
    Ax *= invA;
    Ay *= invA;
    Az *= invA;
    Float Mx = Ay*Hz - Az*Hy;
    Float My = Az*Hx - Ax*Hz;
    Float Mz = Ax*Hy - Ay*Hx;
    if (R != NULL) {
        if (R->GetLength() == 9) {
            (*R)[0] = Hx;     (*R)[1] = Hy;     (*R)[2] = Hz;
            (*R)[3] = Mx;     (*R)[4] = My;     (*R)[5] = Mz;
            (*R)[6] = Ax;     (*R)[7] = Ay;     (*R)[8] = Az;
        } else if (R->GetLength() == 16) {
            (*R)[0]  = Hx;    (*R)[1]  = Hy;    (*R)[2]  = Hz;   (*R)[3]  = 0;
            (*R)[4]  = Mx;    (*R)[5]  = My;    (*R)[6]  = Mz;   (*R)[7]  = 0;
            (*R)[8]  = Ax;    (*R)[9]  = Ay;    (*R)[10] = Az;   (*R)[11] = 0;
            (*R)[12] = 0;     (*R)[13] = 0;     (*R)[14] = 0;    (*R)[15] = 1;
        }
    }
    if (I != NULL) {
        // compute the inclination matrix by projecting the geomagnetic
        // vector onto the Z (gravity) and X (horizontal component
        // of geomagnetic vector) axes.
        Float invE = 1.0f / (Float)Math::Sqrt(Ex*Ex + Ey*Ey + Ez*Ez);
        Float c = (Ex*Mx + Ey*My + Ez*Mz) * invE;
        Float s = (Ex*Ax + Ey*Ay + Ez*Az) * invE;
        if (I->GetLength() == 9) {
            (*I)[0] = 1;     (*I)[1] = 0;     (*I)[2] = 0;
            (*I)[3] = 0;     (*I)[4] = c;     (*I)[5] = s;
            (*I)[6] = 0;     (*I)[7] =-s;     (*I)[8] = c;
        } else if (I->GetLength() == 16) {
            (*I)[0] = 1;     (*I)[1] = 0;     (*I)[2] = 0;
            (*I)[4] = 0;     (*I)[5] = c;     (*I)[6] = s;
            (*I)[8] = 0;     (*I)[9] =-s;     (*I)[10]= c;
            (*I)[3] = (*I)[7] = (*I)[11] = (*I)[12] = (*I)[13] = (*I)[14] = 0;
            (*I)[15] = 1;
        }
    }

    if (result) *result = TRUE;

    return NOERROR;
}

ECode CSensorManager::GetInclination(
    /* [in] */ const ArrayOf<Float>& I,
    /* [out] */ Float* inclination)
{
    VALIDATE_NOT_NULL(inclination);

    if (I.GetLength() == 9) {
        *inclination = (Float)Math::Atan2(I[5], I[4]);
    } else {
        *inclination = (Float)Math::Atan2(I[6], I[5]);
    }

    return NOERROR;
}

ECode CSensorManager::RemapCoordinateSystem(
    /* [in] */ ArrayOf<Float>* inR,
    /* [in] */ Int32 X,
    /* [in] */ Int32 Y,
    /* [in] */ ArrayOf<Float>* outR,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(inR);
    VALIDATE_NOT_NULL(outR);

    if (inR == outR) {
        ArrayOf_<Float, 16>* temp = &mTempMatrix;
        mTempMatrixLock.Lock();
        // we don't expect to have a lot of contention
        Boolean ret;
        ECode ec = RemapCoordinateSystemImpl(inR, X, Y, temp, &ret);
        if (FAILED(ec)) {
            mTempMatrixLock.Unlock();
            return ec;
        }
        if (ret) {
            Int32 size = outR->GetLength();
            for (Int32 i = 0 ; i < size; i++)
                (*outR)[i] = (*temp)[i];
            if (result) *result = TRUE;
        }
        mTempMatrixLock.Unlock();
    }

    return RemapCoordinateSystemImpl(inR, X, Y, outR, result);
}

ECode CSensorManager::GetOrientation(
    /* [in] */ const ArrayOf<Float>& R,
    /* [in] */ ArrayOf<Float>* values)
{
    VALIDATE_NOT_NULL(values);

    /*
     * 4x4 (length=16) case:
     *   /  R[ 0]   R[ 1]   R[ 2]   0  \
     *   |  R[ 4]   R[ 5]   R[ 6]   0  |
     *   |  R[ 8]   R[ 9]   R[10]   0  |
     *   \      0       0       0   1  /
     *
     * 3x3 (length=9) case:
     *   /  R[ 0]   R[ 1]   R[ 2]  \
     *   |  R[ 3]   R[ 4]   R[ 5]  |
     *   \  R[ 6]   R[ 7]   R[ 8]  /
     *
     */
    if (R.GetLength() == 9) {
        (*values)[0] = (Float)Math::Atan2(R[1], R[4]);
        (*values)[1] = (Float)Math::Asin(-R[7]);
        (*values)[2] = (Float)Math::Atan2(-R[6], R[8]);
    } else {
        (*values)[0] = (Float)Math::Atan2(R[1], R[5]);
        (*values)[1] = (Float)Math::Asin(-R[9]);
        (*values)[2] = (Float)Math::Atan2(-R[8], R[10]);
    }

    return NOERROR;
}

ECode CSensorManager::GetAltitude(
    /* [in] */ Float p0,
    /* [in] */ Float p,
    /* [out] */ Float* altitude)
{
    Float coef = 1.0f / 5.255f;
    if (altitude) *altitude = 44330.0f * (1.0f - (Float)Math::Pow(p/p0, coef));

    return NOERROR;
}

ECode CSensorManager::GetAngleChange(
    /* [in] */ ArrayOf<Float>* angleChange,
    /* [in] */ const ArrayOf<Float>& R,
    /* [in] */ const ArrayOf<Float>& prevR)
{
    VALIDATE_NOT_NULL(angleChange);

    Float rd1=0,rd4=0, rd6=0,rd7=0, rd8=0;
    Float ri0=0,ri1=0,ri2=0,ri3=0,ri4=0,ri5=0,ri6=0,ri7=0,ri8=0;
    Float pri0=0, pri1=0, pri2=0, pri3=0, pri4=0, pri5=0, pri6=0, pri7=0, pri8=0;
    // Int32 i, j, k;

    if (R.GetLength() == 9) {
        ri0 = R[0];
        ri1 = R[1];
        ri2 = R[2];
        ri3 = R[3];
        ri4 = R[4];
        ri5 = R[5];
        ri6 = R[6];
        ri7 = R[7];
        ri8 = R[8];
    } else if(R.GetLength() == 16) {
        ri0 = R[0];
        ri1 = R[1];
        ri2 = R[2];
        ri3 = R[4];
        ri4 = R[5];
        ri5 = R[6];
        ri6 = R[8];
        ri7 = R[9];
        ri8 = R[10];
    }

    if (prevR.GetLength() == 9) {
        pri0 = prevR[0];
        pri1 = prevR[1];
        pri2 = prevR[2];
        pri3 = prevR[3];
        pri4 = prevR[4];
        pri5 = prevR[5];
        pri6 = prevR[6];
        pri7 = prevR[7];
        pri8 = prevR[8];
    } else if(prevR.GetLength() == 16) {
        pri0 = prevR[0];
        pri1 = prevR[1];
        pri2 = prevR[2];
        pri3 = prevR[4];
        pri4 = prevR[5];
        pri5 = prevR[6];
        pri6 = prevR[8];
        pri7 = prevR[9];
        pri8 = prevR[10];
    }

    // calculate the parts of the rotation difference matrix we need
    // rd[i][j] = pri[0][i] * ri[0][j] + pri[1][i] * ri[1][j] + pri[2][i] * ri[2][j];

    rd1 = pri0 * ri1 + pri3 * ri4 + pri6 * ri7; //rd[0][1]
    rd4 = pri1 * ri1 + pri4 * ri4 + pri7 * ri7; //rd[1][1]
    rd6 = pri2 * ri0 + pri5 * ri3 + pri8 * ri6; //rd[2][0]
    rd7 = pri2 * ri1 + pri5 * ri4 + pri8 * ri7; //rd[2][1]
    rd8 = pri2 * ri2 + pri5 * ri5 + pri8 * ri8; //rd[2][2]

    (*angleChange)[0] = (Float)Math::Atan2(rd1, rd4);
    (*angleChange)[1] = (Float)Math::Asin(-rd7);
    (*angleChange)[2] = (Float)Math::Atan2(-rd6, rd8);

    return NOERROR;
}

ECode CSensorManager::GetRotationMatrixFromVector(
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ const ArrayOf<Float>& rotationVector)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManager::GetQuaternionFromVector(
    /* [in] */ ArrayOf<Float> * Q,
    /* [in] */ const ArrayOf<Float>& rv)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManager::UnregisterListener(
    /* [in] */ PInterface listener,
    /* [in] */ ISensor* sensor)
{
    if (listener == NULL || sensor == NULL) {
        return NOERROR;
    }

    ECode ec;
    sListenersLock.Lock();

    List<AutoPtr<ISensorManagerListenerDelegate> >::Iterator it;
    for (it = sListeners.Begin(); it != sListeners.End(); it++) {
        AutoPtr<ISensorManagerListenerDelegate> l = *it;
        CSensorManagerListenerDelegate* c = (CSensorManagerListenerDelegate*)(ISensorManagerListenerDelegate*)l;
        AutoPtr<IInterface> obj;
        ec = c->GetListener((PInterface*)&obj);
        if (FAILED(ec)) {
            sListenersLock.Unlock();
            return ec;
        }

        if ((PInterface)obj == listener) {
            Int32 size;
            ec = c->RemoveSensor(sensor, &size);
            if (FAILED(ec)) {
                sListenersLock.Unlock();
                return ec;
            }
            if (size == 0) {
                // if we have no more sensors enabled on this listener,
                // take it off the list.
                sListeners.Erase(it);
            }
            break;
        }
    }

    ec = DisableSensorLocked(sensor, NULL);

    sListenersLock.Unlock();

    return ec;
}

ECode CSensorManager::UnregisterListener(
    /* [in] */ PInterface listener)
{
    if (listener == NULL) {
        return NOERROR;
    }

    ECode ec;
    sListenersLock.Lock();

    List<AutoPtr<ISensorManagerListenerDelegate> >::Iterator it;
    for (it = sListeners.Begin(); it != sListeners.End(); it++) {
        AutoPtr<ISensorManagerListenerDelegate> l = *it;
        CSensorManagerListenerDelegate* c = (CSensorManagerListenerDelegate*)(ISensorManagerListenerDelegate*)l;
        AutoPtr<IInterface> obj;
        ec = c->GetListener((PInterface*)&obj);
        if (FAILED(ec)) {
            sListenersLock.Unlock();
            return ec;
        }

        if ((PInterface)obj == listener) {
            sListeners.Erase(it);
            // disable all sensors for this listener
            AutoPtr<IObjectContainer> sensors;
            ec = c->GetSensors((IObjectContainer**)&sensors);
            if (FAILED(ec)) {
                sListenersLock.Unlock();
                return ec;
            }

            AutoPtr<IObjectEnumerator> objEnum;
            ec = sensors->GetObjectEnumerator((IObjectEnumerator**)&objEnum);
            if (FAILED(ec)) {
                sListenersLock.Unlock();
                return ec;
            }

            Boolean hasNext;
            if (SUCCEEDED(objEnum->MoveNext(&hasNext)) && hasNext) {
                AutoPtr<IInterface> obj;
                FAIL_RETURN(objEnum->Current((IInterface**)&obj));
                ISensor* sensor = (ISensor*)obj->Probe(EIID_ISensor);
                ec = DisableSensorLocked(sensor, NULL);
                if (FAILED(ec)) {
                    sListenersLock.Unlock();
                    return ec;
                }
            }
            break;
        }
    }

    sListenersLock.Unlock();

    return ec;
}

ECode CSensorManager::EnableSensorLocked(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 delay,
    /* [out] */ Boolean* ret)
{
    Boolean result = FALSE;

    List<AutoPtr<ISensorManagerListenerDelegate> >::Iterator it;
    for (it = sListeners.Begin(); it != sListeners.End(); it++) {
        AutoPtr<ISensorManagerListenerDelegate> l = *it;
        CSensorManagerListenerDelegate* c = (CSensorManagerListenerDelegate*)(ISensorManagerListenerDelegate*)l;
        Boolean hasSensor;
        FAIL_RETURN(c->HasSensor(sensor, &hasSensor));
        if (hasSensor) {
            String name;
            FAIL_RETURN(sensor->GetName(&name));

            Int32 handle;
            FAIL_RETURN(((CSensor*)sensor)->GetHandle(&handle));

            // TODO: ALEX fix it
            // result = sensors_enable_sensor(sQueue, name, handle, delay);
            return E_NOT_IMPLEMENTED;
            break;
        }
    }

    if (ret) *ret = result;

    return NOERROR;
}

ECode CSensorManager::DisableSensorLocked(
    /* [in] */ ISensor* sensor,
    /* [out] */ Boolean* result)
{
    List<AutoPtr<ISensorManagerListenerDelegate> >::Iterator it;
    for (it = sListeners.Begin(); it != sListeners.End(); it++) {
        AutoPtr<ISensorManagerListenerDelegate> l = *it;
        CSensorManagerListenerDelegate* c = (CSensorManagerListenerDelegate*)(ISensorManagerListenerDelegate*)l;
        Boolean hasSensor;
        FAIL_RETURN(c->HasSensor(sensor, &hasSensor));
        if (hasSensor) {
            // not an error, it's just that this sensor is still in use
            if (result) *result = TRUE;
            return NOERROR;
        }
    }

    String name;
    FAIL_RETURN(sensor->GetName(&name));

    Int32 handle;
    FAIL_RETURN(((CSensor*)sensor)->GetHandle(&handle));

    // TODO: ALEX fix it
    // return sensors_enable_sensor(sQueue, name, handle, SENSOR_DISABLE);
    return E_NOT_IMPLEMENTED;
}

ECode CSensorManager::RemapCoordinateSystemImpl(
    /* [in] */ ArrayOf<Float>* inR,
    /* [in] */ Int32 X,
    /* [in] */ Int32 Y,
    /* [in] */ ArrayOf<Float>* outR,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(inR);
    VALIDATE_NOT_NULL(outR);

    /*
     * X and Y define a rotation matrix 'r':
     *
     *  (X==1)?((X&0x80)?-1:1):0    (X==2)?((X&0x80)?-1:1):0    (X==3)?((X&0x80)?-1:1):0
     *  (Y==1)?((Y&0x80)?-1:1):0    (Y==2)?((Y&0x80)?-1:1):0    (Y==3)?((X&0x80)?-1:1):0
     *                              r[0] ^ r[1]
     *
     * where the 3rd line is the vector product of the first 2 lines
     *
     */

    Int32 length = outR->GetLength();
    if (inR->GetLength() != length) {
        if (result) *result = FALSE;   // invalid parameter
        return NOERROR;
    }
    if ((X & 0x7C)!=0 || (Y & 0x7C)!=0) {
        if (result) *result = FALSE;   // invalid parameter
        return NOERROR;
    }
    if (((X & 0x3)==0) || ((Y & 0x3)==0)) {
        if (result) *result = FALSE;   // no axis specified
        return NOERROR;
    }
    if ((X & 0x3) == (Y & 0x3)) {
        if (result) *result = FALSE;   // same axis specified
        return NOERROR;
    }

    // Z is "the other" axis, its sign is either +/- sign(X)*sign(Y)
    // this can be calculated by exclusive-or'ing X and Y; except for
    // the sign inversion (+/-) which is calculated below.
    Int32 Z = X ^ Y;

    // extract the axis (remove the sign), offset in the range 0 to 2.
    Int32 x = (X & 0x3)-1;
    Int32 y = (Y & 0x3)-1;
    Int32 z = (Z & 0x3)-1;

    // compute the sign of Z (whether it needs to be inverted)
    Int32 axis_y = (z+1)%3;
    Int32 axis_z = (z+2)%3;
    if (((x^axis_y)|(y^axis_z)) != 0)
        Z ^= 0x80;

    Boolean sx = (X>=0x80);
    Boolean sy = (Y>=0x80);
    Boolean sz = (Z>=0x80);

    // Perform R * r, in avoiding actual muls and adds.
    Int32 rowLength = ((length==16)?4:3);
    for (Int32 j=0 ; j<3 ; j++) {
        Int32 offset = j*rowLength;
        for (int i=0 ; i<3 ; i++) {
            if (x==i)   (*outR)[offset+i] = sx ? -(*inR)[offset+0] : (*inR)[offset+0];
            if (y==i)   (*outR)[offset+i] = sy ? -(*inR)[offset+1] : (*inR)[offset+1];
            if (z==i)   (*outR)[offset+i] = sz ? -(*inR)[offset+2] : (*inR)[offset+2];
        }
    }
    if (length == 16) {
        (*outR)[3] = (*outR)[7] = (*outR)[11] = (*outR)[12] = (*outR)[13] = (*outR)[14] = 0;
        (*outR)[15] = 1;
    }

    if (result) *result = TRUE;

    return NOERROR;
}
