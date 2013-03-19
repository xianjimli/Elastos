#include "media/audiofx/CVisualizer.h"
#include <elastos/System.h>
#include <Logger.h>
#include <stdlib.h>
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

ECode CVisualizer::ReleaseIt()
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
    return E_NOT_IMPLEMENTED;
}

ECode CVisualizer::GetMaxCaptureRate(
    /* [out] */ Int32* captureRate)
{
    return E_NOT_IMPLEMENTED;
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
    /* [in] */ const ArrayOf<Byte>& waveform,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_ENABLED) {
//        throw(new IllegalStateException("getWaveForm() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ArrayOf<Byte>* x = waveform.Clone();
    *result = Native_GetWaveForm(x);
    return NOERROR;
}

ECode CVisualizer::GetFft(
    /* [in] */ const ArrayOf<Byte>& fft,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mStateLock);
    if (mState == Visualizer_STATE_ENABLED) {
//        throw(new IllegalStateException("getFft() called in wrong state: "+mState));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ArrayOf<Byte>* x = fft.Clone();
    *result = Native_GetFft(x);
    return NOERROR;
}

ECode CVisualizer::SetDataCaptureListener(
    /* [in] */ IVisualizerOnDataCaptureListener* listener,
    /* [in] */ Int32 rate,
    /* [out] */ Boolean* waveform,
    /* [out] */ Boolean* fft,
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
    Int32 status = Native_SetPeriodicCapture(rate, *waveform, *fft);
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
        ArrayOf<Byte>* data;
//        int samplingRate = msg.arg1;
        Int32 samplingRate;
//        switch(msg.what) {
        switch (1) {
/*
                case NATIVE_EVENT_PCM_CAPTURE:
                    l.onWaveFormDataCapture(mVisualizer, data, samplingRate);
                    break;
                case NATIVE_EVENT_FFT_CAPTURE:
                    l.onFftDataCapture(mVisualizer, data, samplingRate);
                    break;
                default:
                    Log.e(TAG,"Unknown native event: "+msg.what);
                    break;
*/
        }
    }
    return E_NOT_IMPLEMENTED;
}
void CVisualizer::Native_Init()
{
    //return E_NOT_IMPLEMENTED;
}

Int32 CVisualizer::Native_Setup(
    /* [in] */ IObject* audioeffect_this,
    /* [in] */ Int32 audioSession,
    /* [in] */ ArrayOf<Int32>* id)
{
    //return E_NOT_IMPLEMENTED;
}

void CVisualizer::Native_Finalize()
{
    //return E_NOT_IMPLEMENTED;
}

void CVisualizer::Native_Release()
{
    //return E_NOT_IMPLEMENTED;
}

Int32 CVisualizer::Native_SetEnabled(
        /* [in] */ Boolean enabled)
{
    //return E_NOT_IMPLEMENTED;
}

Boolean CVisualizer::Native_GetEnabled()
{
    //return E_NOT_IMPLEMENTED;
}

Int32 CVisualizer::Native_SetCaptureSize(
    /* [in] */ Int32 size)
{
    //return E_NOT_IMPLEMENTED;
}

Int32 CVisualizer::Native_GetCaptureSize()
{
    //return E_NOT_IMPLEMENTED;
}

Int32 CVisualizer::Native_GetSamplingRate()
{
    //return E_NOT_IMPLEMENTED;
}

Int32 CVisualizer::Native_GetWaveForm(
    /* [in] */ ArrayOf<Byte>* waveform)
{
    //return E_NOT_IMPLEMENTED;
}

Int32 CVisualizer::Native_GetFft(
    /* [in] */ ArrayOf<Byte>* fft)
{
    //return E_NOT_IMPLEMENTED;
}

Int32 CVisualizer::Native_SetPeriodicCapture(
    /* [in] */ Int32 rate,
    /* [in] */ Boolean waveForm,
    /* [in] */ Boolean fft)
{
    //return E_NOT_IMPLEMENTED;
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