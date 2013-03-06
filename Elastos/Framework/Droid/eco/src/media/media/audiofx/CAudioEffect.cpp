#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <StringBuffer.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;
const CString CAudioEffect::TAG = "AudioEffect-JAVA";

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
    Int32 initResult = 0;
    if (initResult != AudioEffect_SUCCESS && initResult != AudioEffect_ALREADY_EXISTS) {
        //Logger::E(TAG, StringBuffer("Error code ") + initResult + "when initializing AudioEffect. ");
        switch (initResult) {
        case AudioEffect_ERROR_BAD_VALUE:
//            throw (new IllegalArgumentException("Effect type: " + type
//                    + " not supported."));
            /*
            Logger::E(TAG, StringBuffer("Effect type: " + type
                + " not supported.");
            */
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case AudioEffect_ERROR_INVALID_OPERATION:
//            throw (new UnsupportedOperationException(
//                    "Effect library not loaded"));
            Logger::E(TAG, StringBuffer("Effect library not loaded"));
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        default:
//            throw (new RuntimeException(
//                        "Cannot initialize effect engine for type: " + type
//                                + "Error: " + initResult));
            /*
            Logger::E(TAG,StringBuffer("Cannot initialize effect engine for type: ") + type
                + "Error: " + initResult);
            */
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
    /*
    return (Descriptor[]) native_query_effects();
    */
    //E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::SetEnabled(
    /* [in]  */ Boolean enabled,
    /* [out] */ Int32* result)
{
    CheckState("SetEnabled()");
    return Native_SetEnabled(enabled);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ const ArrayOf<Byte>& param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* result)
{
    CheckState("SetParameter()");
    /*
    return native_setParameter(param.length, param, value.length, value);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::SetParameterEx(
    /* [in] */ Int32 param,
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
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
    if (param.GetLength() > 2 || value.GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    /*
        byte[] p = intToByteArray(param[0]);
        if (param.length > 1) {
            byte[] p2 = intToByteArray(param[1]);
            p = concatArrays(p, p2);
        }
        byte[] v = intToByteArray(value[0]);
        if (value.length > 1) {
            byte[] v2 = intToByteArray(value[1]);
            v = concatArrays(v, v2);
        }
        return setParameter(p, v);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::SetParameterEx5(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Int16>& value,
    /* [out] */ Int32* result)
{
    if (param.GetLength() > 2 || value.GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    /*
        byte[] p = intToByteArray(param[0]);
        if (param.length > 1) {
            byte[] p2 = intToByteArray(param[1]);
            p = concatArrays(p, p2);
        }

        byte[] v = shortToByteArray(value[0]);
        if (value.length > 1) {
            byte[] v2 = shortToByteArray(value[1]);
            v = concatArrays(v, v2);
        }
        return setParameter(p, v);
    */
    return E_NOT_IMPLEMENTED;
}
ECode CAudioEffect::SetParameterEx6(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Byte>& value,
        /* [out] */ Int32* result)
{
    if (param.GetLength()> 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    /*
        byte[] p = intToByteArray(param[0]);
        if (param.length > 1) {
            byte[] p2 = intToByteArray(param[1]);
            p = concatArrays(p, p2);
        }
        return setParameter(p, value);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::GetParameter(
    /* [in] */ const ArrayOf<Byte>& param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    CheckState("GetParameter()");
    ArrayOf_<Int32, 1> vSize;
    vSize[0] = (*value).GetLength();
    /*
        int status = native_getParameter(param.length, param, vSize, value);
        if (value.length > vSize[0]) {
            byte[] resizedValue = new byte[vSize[0]];
            System.arraycopy(value, 0, resizedValue, 0, vSize[0]);
            value = resizedValue;
        }
        return status;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::GetParameterEx(
    /* [in] */ const Int32 param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* status)
{
    /*
        byte[] p = intToByteArray(param);

        return getParameter(p, value);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::GetParameterEx2(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    if ((*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    /*
        byte[] p = intToByteArray(param);

        byte[] v = new byte[value.length * 4];

        int status = getParameter(p, v);

        value[0] = byteArrayToInt(v);
        if (v.length > 4) {
            value[1] = byteArrayToInt(v, 4);
        }
        return status;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::GetParameterEx3(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    if ((*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    /*
        byte[] p = intToByteArray(param);

        byte[] v = new byte[value.length * 2];

        int status = getParameter(p, v);

        value[0] = byteArrayToShort(v);
        if (v.length > 2) {
            value[1] = byteArrayToShort(v, 2);
        }
        return status;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::GetParameterEx4(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    if (param.GetLength() > 2 || (*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    /*
        byte[] p = intToByteArray(param[0]);
        if (param.length > 1) {
            byte[] p2 = intToByteArray(param[1]);
            p = concatArrays(p, p2);
        }
        byte[] v = new byte[value.length * 4];

        int status = getParameter(p, v);

        value[0] = byteArrayToInt(v);
        if (v.length > 4) {
            value[1] = byteArrayToInt(v, 4);
        }
        return status;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::GetParameterEx5(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    if (param.GetLength() > 2 || (*value).GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    /*
        byte[] p = intToByteArray(param[0]);
        if (param.length > 1) {
            byte[] p2 = intToByteArray(param[1]);
            p = concatArrays(p, p2);
        }
        byte[] v = new byte[value.length * 2];

        int status = getParameter(p, v);

        value[0] = byteArrayToShort(v);
        if (v.length > 2) {
            value[1] = byteArrayToShort(v, 2);
        }
        return status;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::GetParameterEx6(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* status)
{
    if (param.GetLength() > 2) {
        return AudioEffect_ERROR_BAD_VALUE;
    }
    /*
        byte[] p = intToByteArray(param[0]);
        if (param.length > 1) {
            byte[] p2 = intToByteArray(param[1]);
            p = concatArrays(p, p2);
        }

        return getParameter(p, value);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ const ArrayOf<Byte>& command,
    /* [out] */ ArrayOf<Byte>* reply,
    /* [out] */ Int32* status)
{
    CheckState("Command()");
    ArrayOf<Int32>* replySize = ArrayOf<Int32>::Alloc(1);
    (*replySize)[0] = (*reply).GetLength();

    /*

        int status = native_command(cmdCode, command.length, command,
                replySize, reply);

        if (reply.length > replySize[0]) {
            byte[] resizedReply = new byte[replySize[0]];
            System.arraycopy(reply, 0, resizedReply, 0, replySize[0]);
            reply = resizedReply;
        }
        return status;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::GetId(
    /* [out] */ Int32* Id)
{
    CheckState("GetId()");
    return mId;
}

ECode CAudioEffect::GetEnabled(
    /* [out] */ Boolean* getenable)
{
    CheckState("GetEnabled()");
    /*
        return native_getEnabled();
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::HasControl(
    /* [out] */ Boolean* control)
{
    CheckState("HasControl()");
    /*
        return native_hasControl();
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::SetEnableStatusListener(
    /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)
{
     Mutex::Autolock lock(mListenerLock);
    /*
        synchronized (mListenerLock) {
            mEnableStatusChangeListener = listener;
        }
        if ((listener != null) && (mNativeEventHandler == null)) {
            createNativeEventHandler();
        }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::SetControlStatusListener(
    /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)
{
    /*
        synchronized (mListenerLock) {
            mControlChangeStatusListener = listener;
        }
        if ((listener != null) && (mNativeEventHandler == null)) {
            createNativeEventHandler();
        }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CAudioEffect::SetParameterListener(
    /* [in] */ IAudioEffectOnParameterChangeListener* listener)
{
    /*
        synchronized (mListenerLock) {
            mParameterChangeListener = listener;
        }
        if ((listener != null) && (mNativeEventHandler == null)) {
            createNativeEventHandler();
        }
    */
    return E_NOT_IMPLEMENTED;
}

void CAudioEffect::CreateNativeEventHandler()
{
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

void CAudioEffect::PostEventFromNative(
    /* [in] */ IObject* effect_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IObject* obj)
{
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
    /* [in] */ ArrayOf<Int32> id,
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
    /* [in] */ ArrayOf<Byte> param,
    /* [in] */ Int32 vsize,
    /* [in] */ ArrayOf<Byte> value)
{
    //E_NOT_IMPLEMENTED;
}

Int32 CAudioEffect::Native_GetParameter(
    /* [in] */ Int32 psize,
    /* [in] */ ArrayOf<Byte> param,
    /* [in] */ ArrayOf<Int32> vsize,
    /* [in] */ ArrayOf<Byte> value)
{
    //E_NOT_IMPLEMENTED;
}

Int32 CAudioEffect::Native_Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ Int32 cmdSize,
    /* [in] */ ArrayOf<Byte> cmdData,
    /* [in] */ ArrayOf<Int32> repSize,
    /* [in] */ ArrayOf<Byte> repData)
{
    return E_NOT_IMPLEMENTED;
}

ArrayOf<IObject*> CAudioEffect::Native_Query_Effects()
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
}

ECode CAudioEffect::ByteArrayToInt32(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [out] */ Int32* result)
{
    return ByteArrayToInt32Ex(valueBuf, 0, result);
}

ECode CAudioEffect::ByteArrayToInt32Ex(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
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
    return ByteArrayToInt16Ex(valueBuf, 0, result);
}

ECode CAudioEffect::ByteArrayToInt16Ex(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int16* result)
{
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
    /*
        int len = 0;
        for (byte[] a : arrays) {
            len += a.length;
        }
        byte[] b = new byte[len];

        int offs = 0;
        for (byte[] a : arrays) {
            System.arraycopy(a, 0, b, offs, a.length);
            offs += a.length;
        }
        return b;
    */
    return E_NOT_IMPLEMENTED;
}
