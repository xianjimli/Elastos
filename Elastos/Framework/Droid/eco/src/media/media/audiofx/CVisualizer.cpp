#include "media/audiofx/CVisualizer.h"
#include <elastos/System.h>
#include <Logger.h>
#include <stdlib.h>
#include <media/AudioEffect.h>
#include <media/Visualizer.h>
#include <media/EffectApi.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;
const CString CVisualizer::TAG = "Visualizer-JAVA";
const Int32 Visualizer_NATIVE_EVENT_PCM_CAPTURE = 0;
const Int32 Visualizer_NATIVE_EVENT_FFT_CAPTURE = 1;

CVisualizer::CVisualizer()
{
    mState = Visualizer_STATE_UNINITIALIZED;
}

ECode CVisualizer::constructor(
    /* [in] */ Int32 audioSession)
{
    ArrayOf_<Int32, 1> id;
    Mutex::Autolock lock(mStateLock);
    mState = Visualizer_STATE_UNINITIALIZED;
    // native initialization
    /*
    int result = native_setup(new WeakReference<Visualizer>(this), audioSession, id);
    */
    Int32 result = 0;
    if (result != Visualizer_SUCCESS && result != Visualizer_ALREADY_EXISTS) {
//                Log.e(TAG, "Error code "+result+" when initializing Visualizer.");
        switch (result) {
            case Visualizer_ERROR_INVALID_OPERATION:
//                    throw (new UnsupportedOperationException("Effect library not loaded"));
                return E_UNSUPPORTED_OPERATION_EXCEPTION;
            default:
//                    throw (new RuntimeException("Cannot initialize Visualizer engine, error: "
//                            +result));
                return E_RUNTIME_EXCEPTION;
        }
    }
    mId = id[0];
    if (Native_GetEnabled()) {
        mState = Visualizer_STATE_ENABLED;
    } else {
        mState = Visualizer_STATE_INITIALIZED;
    }
    return NOERROR;
}

ECode CVisualizer::ReleaseResources()
{
    Native_Release();
    mState = Visualizer_STATE_UNINITIALIZED;
    return NOERROR;
}

void CVisualizer::Finalize()
{
    Native_Finalize();
}

ECode CVisualizer::SetEnabled(
    /* [in] */ Boolean enabled,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_UNINITIALIZED) {
//        throw(new IllegalStateException("setEnabled() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 status = Visualizer_SUCCESS;
    if ((enabled && (mState == Visualizer_STATE_INITIALIZED)) ||
        (!enabled && (mState == Visualizer_STATE_ENABLED))) {
        status = Native_SetEnabled(enabled);
        if (status == Visualizer_SUCCESS) {
            mState = enabled ? Visualizer_STATE_ENABLED : Visualizer_STATE_INITIALIZED;
        }
    }
    *result = status;
    return NOERROR;
}

ECode CVisualizer::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_UNINITIALIZED) {
//        throw(new IllegalStateException("getEnabled() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *enabled = Native_GetEnabled();
    return NOERROR;
}

ECode CVisualizer::GetCaptureSizeRange(
    /* [out] */ ArrayOf<Int32>* captureSizeRange)
{
    ArrayOf_<Int32,2> jRange;
    ArrayOf_<Int32,2>* nRange;
    (*nRange)[0] = android::Visualizer::getMinCaptureSize();
    (*nRange)[1] = android::Visualizer::getMaxCaptureSize();
    captureSizeRange = &jRange;
    return NOERROR;
}

ECode CVisualizer::GetMaxCaptureRate(
    /* [out] */ Int32* captureRate)
{
    *captureRate = android::Visualizer::getMaxCaptureRate();
    return NOERROR;
}

ECode CVisualizer::SetCaptureSize(
    /* [in] */ Int32 size,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_INITIALIZED) {
//        throw(new IllegalStateException("setCaptureSize() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = Native_SetCaptureSize(size);
    return NOERROR;

}

ECode CVisualizer::GetCaptureSize(
    /* [out] */ Int32* captureSize)
{
    VALIDATE_NOT_NULL(captureSize);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_UNINITIALIZED) {
//        throw(new IllegalStateException("getCaptureSize() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *captureSize = Native_GetCaptureSize();
    return NOERROR;
}

ECode CVisualizer::GetSamplingRate(
    /* [out] */ Int32* samplingRate)
{
    VALIDATE_NOT_NULL(samplingRate);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_UNINITIALIZED) {
//        throw(new IllegalStateException("getSamplingRate() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *samplingRate = Native_GetSamplingRate();
    return NOERROR;
}

ECode CVisualizer::GetWaveForm(
    /* [out] */ ArrayOf<Byte>* waveform,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_ENABLED) {
//        throw(new IllegalStateException("getWaveForm() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ArrayOf<Byte>* x = waveform;
    *result = Native_GetWaveForm(x);
    return NOERROR;
}

ECode CVisualizer::GetFft(
    /* [out] */ ArrayOf<Byte>* fft,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_ENABLED) {
//        throw(new IllegalStateException("getFft() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ArrayOf<Byte>* x = fft;
    *result = Native_GetFft(x);
    return NOERROR;
}

ECode CVisualizer::SetDataCaptureListener(
    /* [in] */ IVisualizerOnDataCaptureListener* listener,
    /* [in] */ Int32 rate,
    /* [in] */ Boolean waveform,
    /* [in] */ Boolean fft,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(waveform);
    VALIDATE_NOT_NULL(fft);
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mListenerLock);
    mCaptureListener = listener;
    if (listener == NULL) {
        // make sure capture callback is stopped in native code
        waveform = FALSE;
        fft = FALSE;
    }
    Int32 status = Native_SetPeriodicCapture(rate, waveform, fft);
    if (status == Visualizer_SUCCESS) {
        if ((listener != NULL) && (mNativeEventHandler == NULL)) {
/*
                Looper looper;
                if ((looper = Looper.myLooper()) != null) {
                    mNativeEventHandler = new NativeEventHandler(this, looper);
                } else if ((looper = Looper.getMainLooper()) != null) {
                    mNativeEventHandler = new NativeEventHandler(this, looper);
                } else {
                    mNativeEventHandler = null;
                    status = ERROR_NO_INIT;
                }
*/
        }
    }

    *result = status;
    return E_NOT_IMPLEMENTED;
}

PInterface CVisualizer::NativeEventHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContextMenuInfo) {
        return (IContextMenuInfo*)this;
    }

    return NULL;
}

UInt32 CVisualizer::NativeEventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CVisualizer::NativeEventHandler::Release()
{
    return ElRefBase::Release();
}

ECode CVisualizer::NativeEventHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IContextMenuInfo*)this) {
        *pIID = EIID_IContextMenuInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CVisualizer::NativeEventHandler::constructor(
    /* [in] */ IVisualizer* v,
    /* [in] */ IApartment* looper)
{
/*
    super(looper);
*/
    mVisualizer = v;
    return E_NOT_IMPLEMENTED;
}

ECode CVisualizer::NativeEventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if (mVisualizer == NULL) {
        return NOERROR;
    }
    IVisualizerOnDataCaptureListener* l;
//    synchronized (mListenerLock)
    l = mHost->mCaptureListener;

    if (l != NULL) {
//        byte[] data = (byte[])msg.obj;
//        ArrayOf<Byte>* data = NULL;
//        int samplingRate = msg.arg1;
//        Int32 samplingRate = 0;
//        switch(msg.what) {
        switch (1) {
/*
                case Visualizer_NATIVE_EVENT_PCM_CAPTURE:
                    l.onWaveFormDataCapture(mVisualizer, data, samplingRate);
                    break;
                case Visualizer_NATIVE_EVENT_FFT_CAPTURE:
                    l.onFftDataCapture(mVisualizer, data, samplingRate);
                    break;
                default:
                    //Log.e(TAG,"Unknown native event: "+msg.what);
                    break;
*/
        }
    }
    return E_NOT_IMPLEMENTED;
}

struct visualizer_callback_cookie {
    //jclass      visualizer_class;  // Visualizer class
    //jobject     visualizer_ref;    // Visualizer object instance
 };

class visualizerJniStorage {
    public:
        visualizer_callback_cookie mCallbackData;
    visualizerJniStorage() {
    }

    ~visualizerJniStorage() {
    }

};

#define VISUALIZER_SUCCESS                      0
#define VISUALIZER_ERROR                       -1
#define VISUALIZER_ERROR_ALREADY_EXISTS        -2
#define VISUALIZER_ERROR_NO_INIT               -3
#define VISUALIZER_ERROR_BAD_VALUE             -4
#define VISUALIZER_ERROR_INVALID_OPERATION     -5
#define VISUALIZER_ERROR_NO_MEMORY             -6
#define VISUALIZER_ERROR_DEAD_OBJECT           -7

#define NATIVE_EVENT_PCM_CAPTURE                0
#define NATIVE_EVENT_FFT_CAPTURE                1

static Int32 translateError(Int32 code) {
    switch(code) {
    case android::NO_ERROR:
        return VISUALIZER_SUCCESS;
    case android::ALREADY_EXISTS:
        return VISUALIZER_ERROR_ALREADY_EXISTS;
    case android::NO_INIT:
        return VISUALIZER_ERROR_NO_INIT;
    case android::BAD_VALUE:
        return VISUALIZER_ERROR_BAD_VALUE;
    case android::INVALID_OPERATION:
        return VISUALIZER_ERROR_INVALID_OPERATION;
    case android::NO_MEMORY:
        return VISUALIZER_ERROR_NO_MEMORY;
    case android::DEAD_OBJECT:
        return VISUALIZER_ERROR_DEAD_OBJECT;
    default:
        return VISUALIZER_ERROR;
    }
}

void CVisualizer::Native_Init()
{

}

Int32 CVisualizer::Native_Setup(
    /* [in] */ IObject* audioeffect_this,
    /* [in] */ Int32 audioSession,
    /* [in] */ ArrayOf<Int32>* id)
{
    visualizerJniStorage* lpJniStorage = NULL;
    Int32 lStatus = VISUALIZER_ERROR_NO_MEMORY;
    android::Visualizer* lpVisualizer = NULL;
    Int32* nId = NULL;

    lpJniStorage = new visualizerJniStorage();
    if (lpJniStorage == NULL) {
        goto setup_failure;
    }

    if (id == NULL) {
        lStatus = VISUALIZER_ERROR_BAD_VALUE;
        goto setup_failure;
    }

    if (lStatus != VISUALIZER_SUCCESS && lStatus != VISUALIZER_ERROR_ALREADY_EXISTS) {
        goto setup_failure;
    }

    if (nId == NULL) {
        lStatus = VISUALIZER_ERROR_BAD_VALUE;
        goto setup_failure;
    }
    nId[0] = lpVisualizer->id();
    nId = NULL;

    return VISUALIZER_SUCCESS;

    // failures:
    setup_failure:

    if (lpVisualizer) {
        delete lpVisualizer;
    }

    if (lpJniStorage) {
        delete lpJniStorage;
    }

    return lStatus;
}

void CVisualizer::Native_Finalize()
{
    // delete the Visualizer object
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer) {
        delete lpVisualizer;
    }

    // delete the JNI data
    visualizerJniStorage* lpJniStorage = NULL;
    if (lpJniStorage) {
        delete lpJniStorage;
    }
}

void CVisualizer::Native_Release()
{
    // do everything a call to finalize would
    Native_Finalize();
}

Int32 CVisualizer::Native_SetEnabled(
    /* [in] */ Boolean enabled)
{
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer == NULL) {
        return VISUALIZER_ERROR_NO_INIT;
    }

    return translateError(lpVisualizer->setEnabled(enabled));
}

Boolean CVisualizer::Native_GetEnabled()
{
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer == NULL) {
        return FALSE;
    }

    return TRUE;
}

Int32 CVisualizer::Native_SetCaptureSize(
    /* [in] */ Int32 size)
{
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer == NULL) {
        return VISUALIZER_ERROR_NO_INIT;
    }

    return translateError(/*lpVisualizer->setCaptureSize(*/size/*)*/);
}

Int32 CVisualizer::Native_GetCaptureSize()
{
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer == NULL) {
        return -1;
    }
    return lpVisualizer->getCaptureSize();
}

Int32 CVisualizer::Native_GetSamplingRate()
{
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer == NULL) {
        return -1;
    }
    return lpVisualizer->getSamplingRate();
}

Int32 CVisualizer::Native_GetWaveForm(
    /* [in] */ ArrayOf<Byte>* waveform)
{
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer == NULL) {
        return VISUALIZER_ERROR_NO_INIT;
    }

    Byte* nWaveform = NULL;
    if (nWaveform == NULL) {
        return VISUALIZER_ERROR_NO_MEMORY;
    }
    Int32 status = translateError(/*lpVisualizer->getWaveForm((uint8_t *)waveform)*/1);

    return status;
}

Int32 CVisualizer::Native_GetFft(
    /* [in] */ ArrayOf<Byte>* fft)
{
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer == NULL) {
        return VISUALIZER_ERROR_NO_INIT;
    }

    Byte* nFft = NULL;
    if (nFft == NULL) {
        return VISUALIZER_ERROR_NO_MEMORY;
    }
    Int32 status = translateError(/*lpVisualizer->getFft((uint8_t *)nFft)*/1);

    return status;
}

Int32 CVisualizer::Native_SetPeriodicCapture(
    /* [in] */ Int32 rate,
    /* [in] */ Boolean waveForm,
    /* [in] */ Boolean fft)
{
    android::Visualizer* lpVisualizer = NULL;
    if (lpVisualizer == NULL) {
        return VISUALIZER_ERROR_NO_INIT;
    }
    visualizerJniStorage* lpJniStorage = NULL;
    if (lpJniStorage == NULL) {
        return VISUALIZER_ERROR_NO_INIT;
    }

    Int32 flags = android::Visualizer::CAPTURE_CALL_JAVA;
    if (waveForm) flags |= android::Visualizer::CAPTURE_WAVEFORM;
    if (fft) flags |= android::Visualizer::CAPTURE_FFT;
    android::Visualizer::capture_cbk_t cbk = NULL;
    if (!waveForm && !fft) cbk = NULL;

    return translateError(/*lpVisualizer->setCaptureCallBack(cbk,
                                                &lpJniStorage->mCallbackData,
                                                flags,
                                                rate)*/1);
}

Void CVisualizer::PostEventFromNative(
    /* [in] */ IObject* effect_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IObject* obj)
{
/*
        Visualizer visu = (Visualizer)((WeakReference)effect_ref).get();
        if (visu == null) {
            return;
        }

        if (visu.mNativeEventHandler != null) {
            Message m = visu.mNativeEventHandler.obtainMessage(what, arg1, arg2, obj);
            visu.mNativeEventHandler.sendMessage(m);
        }
*/
    //return E_NOT_IMPLEMENTED;
}