#include "media/audiofx/AudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <StringBuffer.h>
#include <elastos/AutoFree.h>
#include <media/AudioEffect.h>
#include <media/EffectApi.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const CString AudioEffect::TAG = "AudioEffect-JAVA";

AudioEffect::Descriptor::Descriptor(
    /* [in] */ const String& type1,
    /* [in] */ const String& uuid1,
    /* [in] */ const String& connectMode1,
    /* [in] */ const String& name1,
    /* [in] */ const String& implementor1)
    : mConnectMode(connectMode1)
    , mName(name1)
    , mImplementor(implementor1)
{
/*
    this.type = UUID.fromString(type);
    this.uuid = UUID.fromString(uuid);
*/
}

ECode AudioEffect::Descriptor::GetType(
    /* [out] */ IUUID** type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode AudioEffect::Descriptor::SetType(
    /* [in] */ IUUID* type)
{
    mType = type;
    return NOERROR;
}

ECode AudioEffect::Descriptor::GetUuid(
    /* [out] */ IUUID** uuid)
{
    VALIDATE_NOT_NULL(uuid);
    *uuid = mUuid;
    return NOERROR;
}

ECode AudioEffect::Descriptor::SetUuid(
    /* [in] */ IUUID* uuid)
{
    mUuid = uuid;
    return NOERROR;
}

ECode AudioEffect::Descriptor::GetConnectMode(
    /* [out] */ String* connectMode)
{
    VALIDATE_NOT_NULL(connectMode);
    *connectMode = mConnectMode;
    return NOERROR;
}

ECode AudioEffect::Descriptor::SetConnectMode(
    /* [in] */ String connectMode)
{
    mConnectMode = connectMode;
    return NOERROR;
}

ECode AudioEffect::Descriptor::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode AudioEffect::Descriptor::SetName(
    /* [in] */ String name)
{
    mName = name;
    return NOERROR;
}

ECode AudioEffect::Descriptor::GetImplementor(
    /* [out] */ String* implementor)
{
    VALIDATE_NOT_NULL(implementor);
    *implementor = mImplementor;
    return NOERROR;
}

ECode AudioEffect::Descriptor::SetImplementor(
    /* [in] */ String implementor)
{
    mImplementor = implementor;
    return NOERROR;
}

AudioEffect::AudioEffect()
    : mState(AudioEffect_STATE_UNINITIALIZED)
{}

AudioEffect::~AudioEffect()
{
    NativeFinalize();
}

ECode AudioEffect::Init(
    /* [in] */ IUUID* type,
    /* [in] */ IUUID* uuid,
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    ArrayOf_<Int32, 1> id;
    ArrayOf_< AutoPtr<IAudioEffectDescriptor>, 1> desc;
    // native initialization
/*
    int initResult = native_setup(new WeakReference<AudioEffect>(this),
        type.toString(), uuid.toString(), priority, audioSession, id,
        desc);
*/
    Int32 initResult = 0;
    if (initResult != AudioEffect_SUCCESS && initResult != AudioEffect_ALREADY_EXISTS) {
        /*
            Log.e(TAG, "Error code " + initResult
                    + " when initializing AudioEffect.");
        */
        switch (initResult) {
        case AudioEffect_ERROR_BAD_VALUE:
//            throw (new IllegalArgumentException("Effect type: " + type
//                    + " not supported."));
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case AudioEffect_ERROR_INVALID_OPERATION:
//            throw (new UnsupportedOperationException(
//                    "Effect library not loaded"));
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        default:
//            throw (new RuntimeException(
//                        "Cannot initialize effect engine for type: " + type
//                                + "Error: " + initResult));
            return E_RUNTIME_EXCEPTION;
        }
    }
    mId = id[0];
    mDescriptor = desc[0];

    Mutex::Autolock lock(&mStateLock);
    mState = AudioEffect_STATE_INITIALIZED;
    return NOERROR;
}

ECode AudioEffect::ReleaseResources()
{
    Mutex::Autolock lock(&mStateLock);
    NativeRelease();
    mState = AudioEffect_STATE_UNINITIALIZED;
    return NOERROR;
}


ECode AudioEffect::GetDescriptor(
    /* [out] */ IAudioEffectDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    FAIL_RETURN(CheckState("GetDescriptor()"));
    *descriptor = mDescriptor;
    if (*descriptor != NULL) (*descriptor)->AddRef();

    return NOERROR;
}

ECode AudioEffect::QueryEffects(
    /* [out, callee] */ ArrayOf<IAudioEffectDescriptor*>** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    *descriptor = NativeQueryEffects();
    return NOERROR;
}

ECode AudioEffect::SetEnabled(
    /* [in]  */ Boolean enabled,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckState("SetEnabled()"));
    return NativeSetEnabled(enabled);
}

ECode AudioEffect::SetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(param);
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckState("SetParameter()"));
    *result = NativeSetParameter(param->GetLength(), param, value->GetLength(), value);
    return NOERROR;
}

ECode AudioEffect::SetParameterEx(
    /* [in] */ Int32 param,
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray(param, (ArrayOf<Byte>**)&p);
    AutoFree< ArrayOf<Byte> > v;
    Int32ToByteArray(value, (ArrayOf<Byte>**)&v);
    *result = SetParameter(p.Get(), v.Get(), result);
    return NOERROR;
}

ECode AudioEffect::SetParameterEx2(
    /* [in] */ Int32 param,
    /* [in] */ Int16 value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray(param, (ArrayOf<Byte>**)&p);
    AutoFree< ArrayOf<Byte> > v;
    Int16ToByteArray(value, (ArrayOf<Byte>**)&v);
    *result = SetParameter(p.Get(), v.Get(), result);
    return NOERROR;
}

ECode AudioEffect::SetParameterEx3(
    /* [in] */ Int32 param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(result);

    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray(param, (ArrayOf<Byte>**)&p);
    *result = SetParameter(p.Get(), value, result);
    return NOERROR;
}

ECode AudioEffect::SetParameterEx4(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(param);
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(result);

    if (param->GetLength() > 2 || value->GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray((*param)[0],(ArrayOf<Byte>**)&p);
    if (param->GetLength() > 1) {
        AutoFree< ArrayOf<Byte> > p2;
        Int32ToByteArray((*param)[1],(ArrayOf<Byte>**)&p2);
        AutoFree< ArrayOf<Byte> > tempArrayByte;
        ConcatArrays(p, p2, (ArrayOf<Byte>**)&tempArrayByte);
        p = tempArrayByte;
    }
    AutoFree< ArrayOf<Byte> > v;
    Int32ToByteArray((*value)[0],(ArrayOf<Byte>**)&v);
    if (value->GetLength() > 1) {
        AutoFree< ArrayOf<Byte> > v2;
        Int32ToByteArray((*value)[1],(ArrayOf<Byte>**)&v2);
        AutoFree< ArrayOf<Byte> > tempArrayByte;
        ConcatArrays(v, v2, (ArrayOf<Byte>**)&tempArrayByte);
        v = tempArrayByte;
    }
    *result = SetParameter(p.Get(), v.Get(), result);
    return NOERROR;
}

ECode AudioEffect::SetParameterEx5(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(param);
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(result);

    if (param->GetLength() > 2 || value->GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray((*param)[0],(ArrayOf<Byte>**)&p);
    if (param->GetLength() > 1) {
        AutoFree< ArrayOf<Byte> > p2;
        Int32ToByteArray((*param)[1],(ArrayOf<Byte>**)&p2);
        AutoFree< ArrayOf<Byte> > tempArrayByte;
        ConcatArrays(p, p2, (ArrayOf<Byte>**)&tempArrayByte);
        p = tempArrayByte;
    }
    AutoFree< ArrayOf<Byte> > v;
    Int16ToByteArray((*value)[0],(ArrayOf<Byte>**)&v);
    if (value->GetLength() > 1) {
        AutoFree< ArrayOf<Byte> > v2;
        Int16ToByteArray((*value)[1],(ArrayOf<Byte>**)&v2);
        AutoFree< ArrayOf<Byte> > tempArrayByte;
        ConcatArrays(v, v2, (ArrayOf<Byte>**)&tempArrayByte);
        v = tempArrayByte;
    }
    *result = SetParameter(p.Get(), v.Get(), result);
    return NOERROR;
}
ECode AudioEffect::SetParameterEx6(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (param->GetLength()> 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray((*param)[0],(ArrayOf<Byte>**)&p);
    if (param->GetLength() > 1) {
        AutoFree< ArrayOf<Byte> > p2;
        Int32ToByteArray((*param)[1],(ArrayOf<Byte>**)&p2);
        AutoFree< ArrayOf<Byte> > tempArrayByte;
        ConcatArrays(p, p2, (ArrayOf<Byte>**)&tempArrayByte);
        p = tempArrayByte;
    }
    *result = SetParameter(p.Get(), value, result);
    return NOERROR;
}

ECode AudioEffect::GetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    FAIL_RETURN(CheckState("GetParameter()"));
    ArrayOf_<Int32, 1> vSize;
    vSize[0] = (*value).GetLength();
    *status = NativeGetParameter(param->GetLength(), param, &vSize, value);
    if ((*value).GetLength() > vSize[0]) {
        ArrayOf<Byte>* resizedValue = ArrayOf<Byte>::Alloc(vSize[0]);
        value->Replace(
            0, resizedValue->GetPayload(),vSize[0]);
        value = resizedValue;
        ArrayOf<Byte>::Free(resizedValue);
    }
    return NOERROR;
}

ECode AudioEffect::GetParameterEx(
    /* [in] */ const Int32 param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray(param,(ArrayOf<Byte>**)&p);
    *status = GetParameter(p.Get(), value, status);
    return NOERROR;
}

ECode AudioEffect::GetParameterEx2(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    if ((*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }

    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray(param,(ArrayOf<Byte>**)&p);

    ArrayOf<Byte>* v = ArrayOf<Byte>::Alloc((*value).GetLength() * 4);

    *status = GetParameter(p.Get(), v, status);

    Int32 tempInt32;
    ByteArrayToInt32(v, &tempInt32);
    (*value)[0] = tempInt32;
    if ((*v).GetLength() > 4) {
        ByteArrayToInt32Ex(v, 4, &tempInt32);
        (*value)[1] = tempInt32;
    }
    ArrayOf<Byte>::Free(v);
    return NOERROR;
}

ECode AudioEffect::GetParameterEx3(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    if ((*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }

    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray(param,(ArrayOf<Byte>**)&p);

    ArrayOf<Byte>* v = ArrayOf<Byte>::Alloc((*value).GetLength() * 2);

    *status = GetParameter(p.Get(), v, status);

    Int16 tempInt16;
    ByteArrayToInt16(v, &tempInt16);
    (*value)[0] = tempInt16;
    if ((*v).GetLength() > 2) {
        ByteArrayToInt16Ex(v, 2, &tempInt16);
        (*value)[1] = tempInt16;
    }
    ArrayOf<Byte>::Free(v);
    return NOERROR;
}

ECode AudioEffect::GetParameterEx4(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(param);
    VALIDATE_NOT_NULL(status);

    if (param->GetLength() > 2 || (*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray((*param)[0],(ArrayOf<Byte>**)&p);
    if (param->GetLength() > 1) {
        AutoFree< ArrayOf<Byte> > p2;
        Int32ToByteArray((*param)[1],(ArrayOf<Byte>**)&p2);
        AutoFree< ArrayOf<Byte> > tempArrayByte;
        ConcatArrays(p, p2, (ArrayOf<Byte>**)&tempArrayByte);
        p = tempArrayByte;
    }
    ArrayOf<Byte>* v = ArrayOf<Byte>::Alloc((*value).GetLength() * 4);

    *status = GetParameter(p.Get(), v, status);

    Int32 tempInt32;
    ByteArrayToInt32(v, &tempInt32);
    (*value)[0] = tempInt32;
    if ((*v).GetLength() > 4) {
        ByteArrayToInt32Ex(v, 4, &tempInt32);
        (*value)[1] = tempInt32;
    }
    ArrayOf<Byte>::Free(v);
    return NOERROR;
}

ECode AudioEffect::GetParameterEx5(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(param);
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    if (param->GetLength() > 2 || (*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray((*param)[0],(ArrayOf<Byte>**)&p);
    if (param->GetLength() > 1) {
        AutoFree< ArrayOf<Byte> > p2;
        Int32ToByteArray((*param)[1],(ArrayOf<Byte>**)&p2);
        AutoFree< ArrayOf<Byte> > tempArrayByte;
        ConcatArrays(p, p2, (ArrayOf<Byte>**)&tempArrayByte);
        p = tempArrayByte;
    }
    ArrayOf<Byte>* v = ArrayOf<Byte>::Alloc((*value).GetLength() * 2);

    *status = GetParameter(p.Get(), v, status);

    Int16 tempInt16;
    ByteArrayToInt16(v, &tempInt16);
    (*value)[0] = tempInt16;
    if ((*v).GetLength() > 2) {
        ByteArrayToInt16Ex(v, 2, &tempInt16);
        (*value)[1] = tempInt16;
    }
    ArrayOf<Byte>::Free(v);
    return NOERROR;
}

ECode AudioEffect::GetParameterEx6(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(param);
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    if (param->GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    AutoFree< ArrayOf<Byte> > p;
    Int32ToByteArray((*param)[0],(ArrayOf<Byte>**)&p);
    if (param->GetLength() > 1) {
        AutoFree< ArrayOf<Byte> > p2;
        Int32ToByteArray((*param)[1], (ArrayOf<Byte>**)&p2);
         AutoFree< ArrayOf<Byte> > tempArrayByte;
        ConcatArrays(p, p2, (ArrayOf<Byte>**)&tempArrayByte);
        p = tempArrayByte;
    }

    *status = GetParameter(p.Get(), value, status);
    return NOERROR;
}

ECode AudioEffect::Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ ArrayOf<Byte>* command,
    /* [out] */ ArrayOf<Byte>* reply,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(reply);
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckState("Command()"));
    ArrayOf<Int32>* replySize = ArrayOf<Int32>::Alloc(1);
    (*replySize)[0] = (*reply).GetLength();

    Int32 status = NativeCommand(cmdCode, command->GetLength(), command,
            replySize, reply);

    if ((*reply).GetLength() > (*replySize)[0]) {
        ArrayOf<Byte>* resizedReply = ArrayOf<Byte>::Alloc((*replySize)[0]);
        reply->Replace(
            0, resizedReply->GetPayload(),(*replySize)[0]);
        ArrayOf<Byte>::Free(resizedReply);
    }
    *result = status;
    ArrayOf<Int32>::Free(replySize);
    return NOERROR;
}

ECode AudioEffect::GetId(
    /* [out] */ Int32* Id)
{
    VALIDATE_NOT_NULL(Id);

    FAIL_RETURN(CheckState("GetId()"));
    return mId;
}

ECode AudioEffect::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    FAIL_RETURN(CheckState("GetEnabled()"));
    NativeGetEnabled();
    return NOERROR;
}

ECode AudioEffect::HasControl(
    /* [out] */ Boolean* control)
{
    VALIDATE_NOT_NULL(control);
    FAIL_RETURN(CheckState("HasControl()"));
    NativeHasControl();
    return NOERROR;
}

ECode AudioEffect::SetEnableStatusListener(
    /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)
{
    Mutex::Autolock lock(&mListenerLock);
    mEnableStatusChangeListener = listener;
    if ((listener != NULL) && (mNativeEventHandler == NULL)) {
        CreateNativeEventHandler();
    }
    return NOERROR;
}

ECode AudioEffect::SetControlStatusListener(
    /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)
{
    Mutex::Autolock lock(&mListenerLock);
    mControlChangeStatusListener = listener;
    if ((listener != NULL) && (mNativeEventHandler == NULL)) {
        CreateNativeEventHandler();
    }
    return NOERROR;
}

ECode AudioEffect::SetParameterListener(
    /* [in] */ IAudioEffectOnParameterChangeListener* listener)
{
    Mutex::Autolock lock(&mListenerLock);
    mParameterChangeListener = listener;
    if ((listener != NULL) && (mNativeEventHandler == NULL)) {
        CreateNativeEventHandler();
    }
    return NOERROR;
}

void AudioEffect::CreateNativeEventHandler()
{
    //IApartment* apartment;
/*
        Looper looper;
        if ((looper = Looper.myLooper()) != null) {
            mNativeEventHandler = new NativeEventHandler(this, looper);
        } else if ((looper = Looper.getMainLooper()) != null) {
            mNativeEventHandler = new NativeEventHandler(this, looper);
        } else {
            mNativeEventHandler = null;
        }
*/
    //E_NOT_IMPLEMENTED;
}

PInterface AudioEffect::NativeEventHandler::Probe(
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

UInt32 AudioEffect::NativeEventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioEffect::NativeEventHandler::Release()
{
    return ElRefBase::Release();
}

ECode AudioEffect::NativeEventHandler::GetInterfaceID(
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

ECode AudioEffect::NativeEventHandler::constructor(
    /* [in] */ IAudioEffect* ae,
    /* [in] */ IApartment* looper)
{
/*
    super(looper);
*/
    mAudioEffect = ae;
    return E_NOT_IMPLEMENTED;
}

ECode AudioEffect::NativeEventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if (mAudioEffect == NULL) {
        return NOERROR;
    }
//    switch (msg.what) {
    switch (1) {
    case AudioEffect_NATIVE_EVENT_ENABLED_STATUS:
        IAudioEffectOnEnableStatusChangeListener* enableStatusChangeListener;
//        synchronized (mListenerLock)
        enableStatusChangeListener = mHost->mEnableStatusChangeListener;
        if (enableStatusChangeListener != NULL) {
            /*
            enableStatusChangeListener.onEnableStatusChange(
                mAudioEffect, (boolean) (msg.arg1 != 0));
            */
        }
        return NOERROR;
    case AudioEffect_NATIVE_EVENT_CONTROL_STATUS:
        IAudioEffectOnControlStatusChangeListener* controlStatusChangeListener;
//        synchronized (mListenerLock)
        controlStatusChangeListener = mHost->mControlChangeStatusListener;
        if (controlStatusChangeListener != NULL) {
/*
            controlStatusChangeListener.onControlStatusChange(
                mAudioEffect, (boolean) (msg.arg1 != 0));
*/
        }
        return NOERROR;
    case AudioEffect_NATIVE_EVENT_PARAMETER_CHANGED:
        IAudioEffectOnParameterChangeListener* parameterChangeListener;
//        synchronized (mListenerLock)
        parameterChangeListener = mHost->mParameterChangeListener;
        if (parameterChangeListener != NULL) {
            // arg1 contains offset of parameter value from start of
            // byte array
//            int vOffset = msg.arg1;
            Int32 vOffset;
//            byte[] p = (byte[]) msg.obj;
            ArrayOf<Byte>* p;
            // See effect_param_t in EffectApi.h for psize and vsize
            // fields offsets
            Int32 status;
            AutoPtr<IAudioEffect> obj;
//            AudioEffect::New(0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(p, 0, &status);
            Int32 psize;
            obj->ByteArrayToInt32Ex(p, 4, &status);
            Int32 vsize;
            obj->ByteArrayToInt32Ex(p, 8, &status);
            ArrayOf<Byte>* param = ArrayOf<Byte>::Alloc(psize);
            ArrayOf<Byte>* value = ArrayOf<Byte>::Alloc(vsize);
            p->Replace(
                12, param->GetPayload(),psize);
            p->Replace(
                vOffset, value->GetPayload(),vsize);
            obj->Release();
        }
    }
    return E_NOT_IMPLEMENTED;
}

void AudioEffect::PostEventFromNative(
    /* [in] */ IObject* effect_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IObject* obj)
{
/*
        AudioEffect effect = (AudioEffect) ((WeakReference) effect_ref).get();
        if (effect == null) {
            return;
        }
        if (effect.mNativeEventHandler != null) {
            Message m = effect.mNativeEventHandler.obtainMessage(what, arg1,
                    arg2, obj);
            effect.mNativeEventHandler.sendMessage(m);
        }
*/
    //E_NOT_IMPLEMENTED;
}

struct effect_callback_cookie {
    //jclass      audioEffect_class;  // AudioEffect class
    //jobject     audioEffect_ref;    // AudioEffect object instance
};

class AudioEffectJniStorage {
    public:
        effect_callback_cookie mCallbackData;
    AudioEffectJniStorage() {
    }

    ~AudioEffectJniStorage() {
    }

};

#define AUDIOEFFECT_SUCCESS                      0
#define AUDIOEFFECT_ERROR                       -1
#define AUDIOEFFECT_ERROR_ALREADY_EXISTS        -2
#define AUDIOEFFECT_ERROR_NO_INIT               -3
#define AUDIOEFFECT_ERROR_BAD_VALUE             -4
#define AUDIOEFFECT_ERROR_INVALID_OPERATION     -5
#define AUDIOEFFECT_ERROR_NO_MEMORY             -6
#define AUDIOEFFECT_ERROR_DEAD_OBJECT           -7

static Int32 translateError(Int32 code) {
    switch(code) {
    case android::NO_ERROR:
        return AUDIOEFFECT_SUCCESS;
    case android::ALREADY_EXISTS:
        return AUDIOEFFECT_ERROR_ALREADY_EXISTS;
    case android::NO_INIT:
        return AUDIOEFFECT_ERROR_NO_INIT;
    case android::BAD_VALUE:
        return AUDIOEFFECT_ERROR_BAD_VALUE;
    case android::INVALID_OPERATION:
        return AUDIOEFFECT_ERROR_INVALID_OPERATION;
    case android::NO_MEMORY:
        return AUDIOEFFECT_ERROR_NO_MEMORY;
    case android::DEAD_OBJECT:
        return AUDIOEFFECT_ERROR_DEAD_OBJECT;
    default:
        return AUDIOEFFECT_ERROR;
    }
}

void AudioEffect::NativeInit()
{

}

Int32 AudioEffect::NativeSetup(
    /* [in] */ IObject* audioeffect_this,
    /* [in] */ String type,
    /* [in] */ String uuid,
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession,
    /* [in] */ ArrayOf<Int32>* id,
    /* [in] */ ArrayOf<IObject>* _desc)
{
    AudioEffectJniStorage* lpJniStorage;
    Int32 lStatus = AUDIOEFFECT_ERROR_NO_MEMORY;
    android::AudioEffect* lpAudioEffect = NULL;
    Int32* nId = NULL;
    const char *typeStr = NULL;
    const char *uuidStr = NULL;
    //effect_descriptor_t desc;
    IObject* jdesc = NULL;
    //char str[EFFECT_STRING_LEN_MAX];
    String jdescType;
    String jdescUuid;
    String jdescConnect;
    String jdescName;
    String jdescImplementor;

    if (type != NULL) {
        if (typeStr == NULL) {  // Out of memory
            goto setup_failure;
        }
    }

    if (uuid != NULL) {
        if (uuidStr == NULL) {  // Out of memory
            goto setup_failure;
        }
    }

    if (typeStr == NULL && uuidStr == NULL) {
        lStatus = AUDIOEFFECT_ERROR_BAD_VALUE;
        goto setup_failure;
    }

    lpJniStorage = new AudioEffectJniStorage();
    if (lpJniStorage == NULL) {
        goto setup_failure;
    }

    if (id == NULL) {
        lStatus = AUDIOEFFECT_ERROR_BAD_VALUE;
        goto setup_failure;
    }

    if (lpAudioEffect == NULL) {
        goto setup_failure;
    }

    if (lStatus != AUDIOEFFECT_SUCCESS && lStatus != AUDIOEFFECT_ERROR_ALREADY_EXISTS) {
        goto setup_failure;
    }

    if (nId == NULL) {
        lStatus = AUDIOEFFECT_ERROR_BAD_VALUE;
        goto setup_failure;
    }
    nId[0] = lpAudioEffect->id();
    nId = NULL;

    if (typeStr) {
        typeStr = NULL;
    }

    if (uuidStr) {
        uuidStr = NULL;
    }

    if (jdesc == NULL) {
        goto setup_failure;
    }

    return AUDIOEFFECT_SUCCESS;

    // failures:
    setup_failure:

    if (lpAudioEffect) {
        delete lpAudioEffect;
    }

    if (lpJniStorage) {
        delete lpJniStorage;
    }

    return lStatus;
}

void AudioEffect::NativeFinalize()
{
    // delete the AudioEffect object
    android::AudioEffect* lpAudioEffect = NULL;
    if (lpAudioEffect) {
        delete lpAudioEffect;
    }

    // delete the JNI data
    AudioEffectJniStorage* lpJniStorage = NULL;
    if (lpJniStorage) {
        delete lpJniStorage;
    }
}

void AudioEffect::NativeRelease()
{
    // do everything a call to finalize would
    NativeFinalize();
}

Int32 AudioEffect::NativeSetEnabled(
    /* [in] */ Boolean enabled)
{
    // retrieve the AudioEffect object
    android::AudioEffect* lpAudioEffect = NULL;

    if (lpAudioEffect == NULL) {
        return AUDIOEFFECT_ERROR_NO_INIT;
    }

    return translateError(lpAudioEffect->setEnabled(enabled));
}

Boolean AudioEffect::NativeGetEnabled()
{
    // retrieve the AudioEffect object
    android::AudioEffect* lpAudioEffect = NULL;
    if (lpAudioEffect == NULL) {
        return FALSE;
    }
    return TRUE;
}

Boolean AudioEffect::NativeHasControl()
{
    // retrieve the AudioEffect object
    android::AudioEffect* lpAudioEffect = NULL;

    if (lpAudioEffect == NULL) {
        return FALSE;
    }
    return TRUE;
}

Int32 AudioEffect::NativeSetParameter(
    /* [in] */ Int32 psize,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ Int32 vsize,
    /* [in] */ ArrayOf<Byte>* value)
{
    // retrieve the AudioEffect object
    Byte* lpValue = NULL;
    Byte* lpParam = NULL;
    Int32 lStatus = AUDIOEFFECT_ERROR_BAD_VALUE;
    effect_param_t *p;
    Int32 voffset;

    android::AudioEffect* lpAudioEffect = NULL;

    if (lpAudioEffect == NULL) {
        return AUDIOEFFECT_ERROR_NO_INIT;
    }

    if (psize == 0 || vsize == 0 || param == NULL || value == NULL) {
        return AUDIOEFFECT_ERROR_BAD_VALUE;
    }

    if (lpParam == NULL) {
        goto setParameter_Exit;
    }

    if (lpValue == NULL) {
        goto setParameter_Exit;
    }

    voffset = ((psize - 1) / sizeof(int) + 1) * sizeof(int);
    p = (effect_param_t *) malloc(sizeof(effect_param_t) + voffset + vsize);
    memcpy(p->data, lpParam, psize);
    p->psize = psize;
    memcpy(p->data + voffset, lpValue, vsize);
    p->vsize = vsize;

    lStatus = lpAudioEffect->setParameter(p);
    if (lStatus == android::NO_ERROR) {
        lStatus = p->status;
    }

    free(p);

    setParameter_Exit:
    return translateError(lStatus);
}

Int32 AudioEffect::NativeGetParameter(
    /* [in] */ Int32 psize,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Int32>* vsize,
    /* [in] */ ArrayOf<Byte>* value)
{
    // retrieve the AudioEffect object
    Byte* lpParam = NULL;
    Byte* lpValue = NULL;
    Byte* lpValueSize = NULL;
    Int32 lStatus = AUDIOEFFECT_ERROR_BAD_VALUE;
    effect_param_t *p;
    int voffset;

    android::AudioEffect* lpAudioEffect = NULL;

    if (lpAudioEffect == NULL) {
        return AUDIOEFFECT_ERROR_NO_INIT;
    }

    if (psize == 0 || vsize == NULL || param == NULL || value == NULL) {
        return AUDIOEFFECT_ERROR_BAD_VALUE;
    }

    if (lpParam == NULL) {
        goto getParameter_Exit;
    }

    if (lpValue == NULL) {
        goto getParameter_Exit;
    }

    if (lpValueSize == NULL) {
        goto getParameter_Exit;
    }

    voffset = ((psize - 1) / sizeof(int) + 1) * sizeof(int);
    p = (effect_param_t *) malloc(sizeof(effect_param_t) + voffset
            + lpValueSize[0]);
    memcpy(p->data, lpParam, psize);
    p->psize = psize;
    p->vsize = lpValueSize[0];

    if (lStatus == android::NO_ERROR) {
        lStatus = p->status;
        if (lStatus == android::NO_ERROR) {
            memcpy(lpValue, p->data + voffset, p->vsize);
            lpValueSize[0] = p->vsize;
        }
    }

    free(p);

    getParameter_Exit:
    return translateError(lStatus);
}

Int32 AudioEffect::NativeCommand(
    /* [in] */ Int32 cmdCode,
    /* [in] */ Int32 cmdSize,
    /* [in] */ ArrayOf<Byte>* cmdData,
    /* [in] */ ArrayOf<Int32>* repSize,
    /* [in] */ ArrayOf<Byte>* repData)
{
    //Byte* pCmdData = NULL;
    //Byte* pReplyData = NULL;
    //Int32* pReplySize = NULL;
    Int32 lStatus = AUDIOEFFECT_ERROR_BAD_VALUE;

    // retrieve the AudioEffect object
    android::AudioEffect* lpAudioEffect = NULL;

    if (lpAudioEffect == NULL) {
        return AUDIOEFFECT_ERROR_NO_INIT;
    }

    if ((cmdSize != 0 && cmdData == NULL) || (repSize != NULL && repData == NULL)) {
        return AUDIOEFFECT_ERROR_BAD_VALUE;
    }

    // get the pointer for the command from the java array
    if (cmdSize != 0) {
        if (cmdData == NULL) {
            goto command_Exit;
        }
    }

    // get the pointer for the reply size from the java array
    if (repSize != NULL) {
        if (repSize == NULL) {
            goto command_Exit;
        }
    }

    // get the pointer for the reply from the java array
    if (repSize != NULL && repSize[0] != 0 && repData != NULL) {
        if (repData == NULL) {
            goto command_Exit;
        }
    }
    command_Exit:
    return lStatus;
}

ArrayOf<IAudioEffectDescriptor*>* AudioEffect::NativeQueryEffects()
{
    //effect_descriptor_t desc;
    //char str[EFFECT_STRING_LEN_MAX];
    uint32_t numEffects = 0;
    uint32_t i = 0;
    String jdescType;
    String jdescUuid;
    String jdescConnect;
    String jdescName;
    String jdescImplementor;
    IObject* jdesc = NULL;

    for (i = 0; i < numEffects; i++) {
        if (jdesc == NULL) {
            goto queryEffects_failure;
        }
    }

    queryEffects_failure:
    return NULL;
}

ECode AudioEffect::CheckState(
    /* [in] */ CString methodName)
{
    Mutex::Autolock lock(&mStateLock);
    if (mState != AudioEffect_STATE_INITIALIZED) {
//        throw (new IllegalStateException(methodName
//                + " called on uninitialized AudioEffect."));
        Logger::E(TAG, StringBuffer("") + methodName + " called on uninitialized AudioEffect.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode AudioEffect::CheckStatus(
    /* [in] */ Int32 status)
{
    switch (status) {
        case AudioEffect_SUCCESS:
            return NOERROR;
        case AudioEffect_ERROR_BAD_VALUE:
//            throw (new IllegalArgumentException(
//                    "AudioEffect: bad parameter value"));
            Logger::E(TAG, StringBuffer("AudioEffect: bad parameter value"));
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case AudioEffect_ERROR_INVALID_OPERATION:
//            throw (new UnsupportedOperationException(
//                    "AudioEffect: invalid parameter operation"));
            Logger::E(TAG, StringBuffer("AudioEffect: invalid parameter operation"));
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        default:
//            throw (new RuntimeException("AudioEffect: set/get parameter error"));
            Logger::E(TAG, StringBuffer("AudioEffect: set/get parameter error"));
            return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode AudioEffect::ByteArrayToInt32(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return ByteArrayToInt32Ex(valueBuf, 0, result);
}

ECode AudioEffect::ByteArrayToInt32Ex(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IByteBuffer> converter;
    AutoPtr<IByteBufferHelper> tempByteBufferHelper;
    tempByteBufferHelper->Wrap(valueBuf,(IByteBuffer**) &converter);
    *result = converter->GetInt32Ex(offset, result);
    return NOERROR;
}

ECode AudioEffect::Int32ToByteArray(
    /* [in] */ Int32 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IByteBuffer> converter;
    AutoPtr<IByteBufferHelper> tempByteBufferHelper;
    tempByteBufferHelper->Allocate(4,(IByteBuffer**) &converter);
    ByteOrder* tempByteOrder;
    converter->GetOrder(tempByteOrder);
    converter->PutInt32(value);
    AutoFree< ArrayOf<Byte> > p;
    *result = (ArrayOf<Byte>*) converter->Array((ArrayOf<Byte>**) &p);
    return NOERROR;
}

ECode AudioEffect::ByteArrayToInt16(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);

    return ByteArrayToInt16Ex(valueBuf, 0, result);
}

ECode AudioEffect::ByteArrayToInt16Ex(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IByteBuffer> converter;
    AutoPtr<IByteBufferHelper> tempByteBufferHelper;
    tempByteBufferHelper->Wrap(valueBuf,(IByteBuffer**) &converter);
    ByteOrder* tempByteOrder;
    converter->GetOrder(tempByteOrder);
    *result = converter->GetInt16Ex(offset, result);
    return NOERROR;
}

ECode AudioEffect::Int16ToByteArray(
    /* [in] */ Int16 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IByteBuffer> converter;
    AutoPtr<IByteBufferHelper> tempByteBufferHelper;
    tempByteBufferHelper->Allocate(2,(IByteBuffer**) &converter);
    ByteOrder* tempByteOrder;
    converter->GetOrder(tempByteOrder);
    Int16 sValue = (Int16) value;
    converter->PutInt16(sValue);
    AutoFree< ArrayOf<Byte> > p;
    *result = (ArrayOf<Byte>*) converter->Array((ArrayOf<Byte>**) &p);
    return NOERROR;
}

ECode AudioEffect::ConcatArrays(
    /* [in] */ ArrayOf<Byte>* array1,
    /* [in] */ ArrayOf<Byte>* array2,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 len = array1->GetLength() + array2->GetLength();
    ArrayOf<Byte>* newArray = ArrayOf<Byte>::Alloc(len);
    if (newArray == NULL) {
        *result = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }

    memcpy(newArray->GetPayload(), array1->GetPayload(), array1->GetLength());
    memcpy(newArray->GetPayload() + array1->GetLength(), array2->GetPayload(), array2->GetLength());
    return NOERROR;
}
