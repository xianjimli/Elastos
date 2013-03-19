#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <StringBuffer.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;
const CString CAudioEffect::TAG = "AudioEffect-JAVA";

ECode CAudioEffect::Descriptor::constructor(
    /* [in] */ String type1,
    /* [in] */ String uuid1,
    /* [in] */ String connectMode1,
    /* [in] */ String name1,
    /* [in] */ String implementor1)
{
/*
    this.type = UUID.fromString(type);
    this.uuid = UUID.fromString(uuid);
*/
    connectMode = connectMode1;
    name = name1;
    implementor = implementor1;
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::Descriptor::GetParameterString(
    /* [in] */ String param,
    /* [out] */  String* result)
{
    VALIDATE_NOT_NULL(result);

    if (param.Equals("connectMode")) {
        *result = connectMode;
    }

    if (param.Equals("name")) {
        *result = name;
    }

    if (param.Equals("implementor")) {
        *result = implementor;
    }
    return NOERROR;
}

ECode CAudioEffect::Descriptor::SetParameterString(
    /* [in] */ String param,
    /* [in] */ String result)
{
    if (param.Equals("connectMode")) {
        connectMode = result ;
    }

    if (param.Equals("name")) {
        name = result ;
    }

    if (param.Equals("implementor")) {
        implementor = result ;
    }
    return NOERROR;
}

ECode CAudioEffect::Descriptor::GetParameterUUID(
    /* [in] */ String param)
//    /* [out] */  UUID* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::Descriptor::SetParameterUUID(
    /* [in] */ String param)
//    /* [in] */ UUID result);
{
    return E_NOT_IMPLEMENTED;
}

CAudioEffect::CAudioEffect()
{

}

ECode CAudioEffect::constructor(
    /* [in] */ //IUUID* type,
    /* [in] */ //IUUID* uuid,
    /* [in] */ Int32 proiority,
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
    Int32 initResult;
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
    Mutex::Autolock lock(mStateLock);
    mState = AudioEffect_STATE_INITIALIZED;
    return NOERROR;
}

ECode CAudioEffect::ReleaseIt()
{
    Mutex::Autolock lock(mStateLock);
    Native_Release();
    mState = AudioEffect_STATE_UNINITIALIZED;
    return NOERROR;
}

void CAudioEffect::Finalize()
{
    Native_Finalize();
}

ECode CAudioEffect::GetDescriptor(
    /* [out] */ IAudioEffectDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    FAIL_RETURN(CheckState("GetDescriptor()"));
    *descriptor = mDescriptor;
    if (*descriptor != NULL) (*descriptor)->AddRef();

    return NOERROR;
}

ECode CAudioEffect::QueryEffects(
    /* [out, callee] */ ArrayOf<IAudioEffectDescriptor>** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    *descriptor = Native_Query_Effects();

    return NOERROR;
}

ECode CAudioEffect::SetEnabled(
    /* [in]  */ Boolean enabled,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    CheckState("SetEnabled()");
    return Native_SetEnabled(enabled);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ const ArrayOf<Byte>& param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    CheckState("SetParameter()");
    ArrayOf<Byte>* tempResult1 = param.Clone();
    ArrayOf<Byte>* tempResult2 = value.Clone();
    Native_SetParameter(param.GetLength(), tempResult1, value.GetLength(), tempResult2);
    return NOERROR;
}

ECode CAudioEffect::SetParameterEx(
    /* [in] */ Int32 param,
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ArrayOf<Byte>* p;
    Int32ToByteArray(param, p);
    ArrayOf<Byte>* v;
    Int32ToByteArray(value, v);
    SetParameter(*p, *v, result);
    return NOERROR;
}

ECode CAudioEffect::SetParameterEx2(
    /* [in] */ Int32 param,
    /* [in] */ Int16 value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ArrayOf<Byte>* p;
    Int32ToByteArray(param, p);
    ArrayOf<Byte>* v;
    Int16ToByteArray(value, v);
    SetParameter(*p, *v, result);
    return NOERROR;
}

ECode CAudioEffect::SetParameterEx3(
    /* [in] */ Int32 param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ArrayOf<Byte>* p;
    Int32ToByteArray(param, p);
    SetParameter(*p, value, result);
    return NOERROR;
}

ECode CAudioEffect::SetParameterEx4(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Int32>& value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (param.GetLength() > 2 || value.GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    ArrayOf<Byte>* p;
    Int32ToByteArray(param[0],p);
    if (param.GetLength() > 1) {
        ArrayOf<Byte>* p2;
        Int32ToByteArray(param[1],p2);
        ArrayOf<Byte>* tempArrayByte;
        ConcatArrays(*p, *p2, &tempArrayByte);
        p = tempArrayByte;
    }
    ArrayOf<Byte>* v;
    Int32ToByteArray(value[0],v);
    if (value.GetLength() > 1) {
        ArrayOf<Byte>* v2;
        Int32ToByteArray(value[1],v2);
        ArrayOf<Byte>* tempArrayByte;
        ConcatArrays(*v, *v2, &tempArrayByte);
        v = tempArrayByte;
    }
    SetParameter(*p, *v, result);
    return NOERROR;
}

ECode CAudioEffect::SetParameterEx5(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Int16>& value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (param.GetLength() > 2 || value.GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    ArrayOf<Byte>* p;
    Int32ToByteArray(param[0],p);
    if (param.GetLength() > 1) {
        ArrayOf<Byte>* p2;
        Int32ToByteArray(param[1],p2);
        ArrayOf<Byte>* tempArrayByte;
        ConcatArrays(*p, *p2, &tempArrayByte);
        p = tempArrayByte;
    }
    ArrayOf<Byte>* v;
    Int16ToByteArray(value[0],v);
    if (value.GetLength() > 1) {
        ArrayOf<Byte>* v2;
        Int16ToByteArray(value[1],v2);
        ArrayOf<Byte>* tempArrayByte;
        ConcatArrays(*v, *v2, &tempArrayByte);
        v = tempArrayByte;
    }
    SetParameter(*p, *v, result);
    return NOERROR;
}
ECode CAudioEffect::SetParameterEx6(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Byte>& value,
        /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (param.GetLength()> 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    ArrayOf<Byte>* p;
    Int32ToByteArray(param[0],p);
    if (param.GetLength() > 1) {
        ArrayOf<Byte>* p2;
        Int32ToByteArray(param[1],p2);
        ArrayOf<Byte>* tempArrayByte;
        ConcatArrays(*p, *p2, &tempArrayByte);
        p = tempArrayByte;
    }
    SetParameter(*p, value, result);
    return NOERROR;
}

ECode CAudioEffect::GetParameter(
    /* [in] */ const ArrayOf<Byte>& param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    CheckState("GetParameter()");
    ArrayOf_<Int32, 1> vSize;
    vSize[0] = (*value).GetLength();
    ArrayOf<Byte>* tempResult1 = param.Clone();
    *status = Native_GetParameter(param.GetLength(), tempResult1, &vSize, value);
    if ((*value).GetLength() > vSize[0]) {
        ArrayOf<Byte>* resizedValue = ArrayOf<Byte>::Alloc(vSize[0]);
        value->Replace(
            0, resizedValue->GetPayload(),vSize[0]);
        value = resizedValue;
        ArrayOf<Byte>::Free(resizedValue);
    }
    return NOERROR;
}

ECode CAudioEffect::GetParameterEx(
    /* [in] */ const Int32 param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    ArrayOf<Byte>* p;
    Int32ToByteArray(param,p);
    ArrayOf<Byte>* tempResult1 = value.Clone();
    GetParameter(*p, tempResult1, status);
    return NOERROR;
}

ECode CAudioEffect::GetParameterEx2(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    if ((*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }

    ArrayOf<Byte>* p;
    Int32ToByteArray(param,p);

    ArrayOf<Byte>* v = ArrayOf<Byte>::Alloc((*value).GetLength() * 4);

    GetParameter(*p, v, status);

    Int32 tempInt32;
    ByteArrayToInt32(*v, &tempInt32);
    (*value)[0] = tempInt32;
    if ((*v).GetLength() > 4) {
        ByteArrayToInt32Ex(*v, 4, &tempInt32);
        (*value)[1] = tempInt32;
    }
    ArrayOf<Byte>::Free(v);
    return NOERROR;
}

ECode CAudioEffect::GetParameterEx3(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    if ((*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }

    ArrayOf<Byte>* p;
    Int32ToByteArray(param,p);

    ArrayOf<Byte>* v = ArrayOf<Byte>::Alloc((*value).GetLength() * 2);

    GetParameter(*p, v, status);

    Int16 tempInt16;
    ByteArrayToInt16(*v, &tempInt16);
    (*value)[0] = tempInt16;
    if ((*v).GetLength() > 2) {
        ByteArrayToInt16Ex(*v, 2, &tempInt16);
        (*value)[1] = tempInt16;
    }
    ArrayOf<Byte>::Free(v);
    return NOERROR;
}

ECode CAudioEffect::GetParameterEx4(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    if (param.GetLength() > 2 || (*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    ArrayOf<Byte>* p;
    Int32ToByteArray(param[0],p);
    if (param.GetLength() > 1) {
        ArrayOf<Byte>* p2;
        Int32ToByteArray(param[1],p2);
        ArrayOf<Byte>* tempArrayByte;
        ConcatArrays(*p, *p2, &tempArrayByte);
        p = tempArrayByte;
    }
    ArrayOf<Byte>* v = ArrayOf<Byte>::Alloc((*value).GetLength() * 4);

    GetParameter(*p, v, status);

    Int32 tempInt32;
    ByteArrayToInt32(*v, &tempInt32);
    (*value)[0] = tempInt32;
    if ((*v).GetLength() > 4) {
        ByteArrayToInt32Ex(*v, 4, &tempInt32);
        (*value)[1] = tempInt32;
    }
    ArrayOf<Byte>::Free(v);
    return NOERROR;
}

ECode CAudioEffect::GetParameterEx5(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    if (param.GetLength() > 2 || (*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    ArrayOf<Byte>* p;
    Int32ToByteArray(param[0],p);
    if (param.GetLength() > 1) {
        ArrayOf<Byte>* p2;
        Int32ToByteArray(param[1],p2);
        ArrayOf<Byte>* tempArrayByte;
        ConcatArrays(*p, *p2, &tempArrayByte);
        p = tempArrayByte;
    }
    ArrayOf<Byte>* v = ArrayOf<Byte>::Alloc((*value).GetLength() * 2);

    GetParameter(*p, v, status);

    Int16 tempInt16;
    ByteArrayToInt16(*v, &tempInt16);
    (*value)[0] = tempInt16;
    if ((*v).GetLength() > 2) {
        ByteArrayToInt16Ex(*v, 2, &tempInt16);
        (*value)[1] = tempInt16;
    }
    ArrayOf<Byte>::Free(v);
    return NOERROR;
}

ECode CAudioEffect::GetParameterEx6(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    if (param.GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    ArrayOf<Byte>* p;
    Int32ToByteArray(param[0],p);
    if (param.GetLength() > 1) {
        ArrayOf<Byte>* p2;
        Int32ToByteArray(param[1],p2);
        ArrayOf<Byte>* tempArrayByte;
        ConcatArrays(*p, *p2, &tempArrayByte);
        p = tempArrayByte;
    }

    ArrayOf<Byte>* tempResult1 = value.Clone();
    GetParameter(*p, tempResult1, status);
    return NOERROR;
}

ECode CAudioEffect::Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ const ArrayOf<Byte>& command,
    /* [out] */ ArrayOf<Byte>* reply,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(reply);
    VALIDATE_NOT_NULL(result);

    CheckState("Command()");
    ArrayOf<Int32>* replySize = ArrayOf<Int32>::Alloc(1);
    (*replySize)[0] = (*reply).GetLength();

    ArrayOf<Byte>* tempValue = command.Clone();
    Int32 status = Native_Command(cmdCode, command.GetLength(), tempValue,
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

ECode CAudioEffect::GetId(
    /* [out] */ Int32* Id)
{
    VALIDATE_NOT_NULL(Id);

    CheckState("GetId()");
    return mId;
}

ECode CAudioEffect::GetEnabled(
    /* [out] */ Boolean* getenable)
{
    VALIDATE_NOT_NULL(getenable);

    CheckState("GetEnabled()");
    Native_GetEnabled();
    return NOERROR;
}

ECode CAudioEffect::HasControl(
    /* [out] */ Boolean* control)
{
    VALIDATE_NOT_NULL(control);
    CheckState("HasControl()");
    Native_HasControl();
    return NOERROR;
}

ECode CAudioEffect::SetEnableStatusListener(
    /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)
{
    Mutex::Autolock lock(mListenerLock);
    mEnableStatusChangeListener = listener;
    if ((listener != NULL) && (mNativeEventHandler == NULL)) {
        CreateNativeEventHandler();
    }
    return NOERROR;
}

ECode CAudioEffect::SetControlStatusListener(
    /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)
{
    Mutex::Autolock lock(mListenerLock);
    mControlChangeStatusListener = listener;
    if ((listener != NULL) && (mNativeEventHandler == NULL)) {
        CreateNativeEventHandler();
    }
    return NOERROR;
}

ECode CAudioEffect::SetParameterListener(
    /* [in] */ IAudioEffectOnParameterChangeListener* listener)
{
    Mutex::Autolock lock(mListenerLock);
    mParameterChangeListener = listener;
    if ((listener != NULL) && (mNativeEventHandler == NULL)) {
        CreateNativeEventHandler();
    }
    return NOERROR;
}

void CAudioEffect::CreateNativeEventHandler()
{
    IApartment* apartment;
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

PInterface CAudioEffect::NativeEventHandler::Probe(
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

UInt32 CAudioEffect::NativeEventHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAudioEffect::NativeEventHandler::Release()
{
    return ElRefBase::Release();
}

ECode CAudioEffect::NativeEventHandler::GetInterfaceID(
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

ECode CAudioEffect::NativeEventHandler::constructor(
    /* [in] */ IAudioEffect* ae,
    /* [in] */ IApartment* looper)
{
/*
    super(looper);
*/
    mAudioEffect = ae;
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::NativeEventHandler::HandleMessage(
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
            CAudioEffect::New(0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(*p, 0, &status);
            Int32 psize;
            obj->ByteArrayToInt32Ex(*p, 4, &status);
            Int32 vsize;
            obj->ByteArrayToInt32Ex(*p, 8, &status);
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

void CAudioEffect::PostEventFromNative(
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

void CAudioEffect::Native_Init()
{
    //E_NOT_IMPLEMENTED;
}

Int32 CAudioEffect::Native_Setup(
    /* [in] */ IObject* audioeffect_this,
    /* [in] */ String type,
    /* [in] */ String uuid,
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession,
    /* [in] */ ArrayOf<Int32>* id,
    /* [in] */ ArrayOf<IObject>* desc)
{
    //E_NOT_IMPLEMENTED;
}

void CAudioEffect::Native_Finalize()
{
    //E_NOT_IMPLEMENTED;
}

void CAudioEffect::Native_Release()
{
    //E_NOT_IMPLEMENTED;
}

Int32 CAudioEffect::Native_SetEnabled(
    /* [in] */ Boolean enabled)
{
    //E_NOT_IMPLEMENTED;
}

Boolean CAudioEffect::Native_GetEnabled()
{
    //E_NOT_IMPLEMENTED;
}

Boolean CAudioEffect::Native_HasControl()
{
    //E_NOT_IMPLEMENTED;
}

Int32 CAudioEffect::Native_SetParameter(
    /* [in] */ Int32 psize,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ Int32 vsize,
    /* [in] */ ArrayOf<Byte>* value)
{
    //E_NOT_IMPLEMENTED;
}

Int32 CAudioEffect::Native_GetParameter(
    /* [in] */ Int32 psize,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Int32>* vsize,
    /* [in] */ ArrayOf<Byte>* value)
{
    //E_NOT_IMPLEMENTED;
}

Int32 CAudioEffect::Native_Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ Int32 cmdSize,
    /* [in] */ ArrayOf<Byte>* cmdData,
    /* [in] */ ArrayOf<Int32>* repSize,
    /* [in] */ ArrayOf<Byte>* repData)
{
    return E_NOT_IMPLEMENTED;
}

ArrayOf<IAudioEffectDescriptor>* CAudioEffect::Native_Query_Effects()
{
    //return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::CheckState(
    /* [in] */ CString methodName)
{
    Mutex::Autolock lock(mStateLock);
    if (mState != AudioEffect_STATE_INITIALIZED) {
//        throw (new IllegalStateException(methodName
//                + " called on uninitialized AudioEffect."));
        Logger::E(TAG, StringBuffer("") + methodName + " called on uninitialized AudioEffect.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioEffect::CheckStatus(
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

ECode CAudioEffect::ByteArrayToInt32(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return ByteArrayToInt32Ex(valueBuf, 0, result);
}

ECode CAudioEffect::ByteArrayToInt32Ex(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    /*
        ByteBuffer converter = ByteBuffer.wrap(valueBuf);
        converter.order(ByteOrder.nativeOrder());
        return converter.getInt(offset);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::Int32ToByteArray(
    /* [in] */ Int32 value,
    /* [out] */ ArrayOf<Byte>* result)
{
    VALIDATE_NOT_NULL(result);

    /*
        ByteBuffer converter = ByteBuffer.allocate(4);
        converter.order(ByteOrder.nativeOrder());
        converter.putInt(value);
        return converter.array();
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::ByteArrayToInt16(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);

    return ByteArrayToInt16Ex(valueBuf, 0, result);
}

ECode CAudioEffect::ByteArrayToInt16Ex(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);

    /*
        ByteBuffer converter = ByteBuffer.wrap(valueBuf);
        converter.order(ByteOrder.nativeOrder());
        return converter.getShort(offset);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::Int16ToByteArray(
    /* [in] */ Int16 value,
    /* [out] */ ArrayOf<Byte>* result)
{
    VALIDATE_NOT_NULL(result);

    /*
        ByteBuffer converter = ByteBuffer.allocate(2);
        converter.order(ByteOrder.nativeOrder());
        short sValue = (short) value;
        converter.putShort(sValue);
        return converter.array();
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::ConcatArrays(
    /* [in] */ const ArrayOf<Byte>& array1,
    /* [in] */ const ArrayOf<byte>& array2,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 len = 0;
    len = array1.GetLength() + array2.GetLength();

    ArrayOf<Byte>* b = ArrayOf<Byte>::Alloc(len);

    Int32 offs = 0;
    ArrayOf<Byte>* tempResult1 = array1.Clone();
    ArrayOf<Byte>* tempResult2 = array2.Clone();
    tempResult1->Replace(
        0, b->GetPayload(),array1.GetLength());
    ArrayOf<Byte>::Free(b);
    *result = b;
    return NOERROR;
}
