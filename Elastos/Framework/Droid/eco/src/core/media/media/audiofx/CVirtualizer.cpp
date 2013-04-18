#include "media/audiofx/CVirtualizer.h"
#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <stdlib.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CVirtualizer::CVirtualizer()
{
    mStrengthSupported = false;
    CAudioEffect::New((IUUID*) 0,(IUUID*) 0,0,0,(IAudioEffect**)&obj);
}

PInterface CVirtualizer::BaseParameterListener::Probe(
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

UInt32 CVirtualizer::BaseParameterListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CVirtualizer::BaseParameterListener::Release()
{
    return ElRefBase::Release();
}

ECode CVirtualizer::BaseParameterListener::GetInterfaceID(
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

ECode CVirtualizer::constructor(
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    /*
        super(EFFECT_TYPE_VIRTUALIZER, EFFECT_TYPE_NULL, priority, audioSession);
    */
    ArrayOf_<Int32,1> value;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx2(Virtualizer_PARAM_STRENGTH_SUPPORTED, &value, &status));
    mStrengthSupported = (value[0] != 0);
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetStrengthSupported(
    /* [out] */ Boolean* strengthSupported)
{
    VALIDATE_NOT_NULL(strengthSupported);
    *strengthSupported = mStrengthSupported;
    return NOERROR;
}

ECode CVirtualizer::SetStrength(
    /* [in] */ Int16 strength)
{
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx2(Virtualizer_PARAM_STRENGTH, strength, &status));
    return NOERROR;
}

ECode CVirtualizer::GetRoundedStrength(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ArrayOf_<Int16,1> value;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(Virtualizer_PARAM_STRENGTH, &value, &status));
    *result = value[0];
    return NOERROR;
}

ECode CVirtualizer::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value)
{
    AutoPtr<IVirtualizerOnParameterChangeListener> l;

    Mutex::Autolock lock(&mHost->mParamListenerLock);
    if (mHost->mParamListener != NULL) {
        l = mHost->mParamListener;
    }
    if (l != NULL) {
        Int32 p = -1;
        Int16 v = -1;

        if (param->GetLength() == 4) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New((IUUID*) 0,(IUUID*) 0,0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(param, 0, &p);
            obj->Release();
        }
        if (value->GetLength() == 2) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New((IUUID*) 0,(IUUID*) 0,0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt16Ex(value, 0, &v);
            obj->Release();
        }
        if (p != -1 && v != -1) {
            l->OnParameterChange((IVirtualizer*) this->Probe(EIID_IVirtualizer), status, p, v);
        }
    }
    return NOERROR;
}

ECode CVirtualizer::SetParameterListenerEx(
    /* [in] */ IVirtualizerOnParameterChangeListener* listener)
{
    Mutex::Autolock lock(&mParamListenerLock);
    if (mParamListener != NULL) {
        mParamListener = listener;
        mBaseParamListener = new BaseParameterListener();
        AutoPtr<IAudioEffect> obj;
        CAudioEffect::New((IUUID*) 0,(IUUID*) 0,0,0,(IAudioEffect**)&obj);
        obj->SetParameterListener(mBaseParamListener);
        obj->Release();
    }
    return NOERROR;
}

ECode CVirtualizer::Settings::constructor(
    /* [in] */ String* settings)
{
    StringTokenizer st(*settings, String("=;"));
    //Int32 tokens = st.GetCount();
    if (st.GetCount() != 3) {
//        throw new IllegalArgumentException("settings: " + settings);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String key = st.NextToken();
    if (!key.Equals("Virtualizer")) {
//        throw new IllegalArgumentException("invalid settings for Virtualizer: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
/*
    try {
*/
    key = st.NextToken();
    if (!key.Equals("strength")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mStrength = st.NextToken().ToInt32();
/*
     } catch (NumberFormatException nfe) {
        throw new IllegalArgumentException("invalid value for key: " + key);
     }
*/
    return NOERROR;
}

ECode CVirtualizer::Settings::toString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = String("Virtualizer") +
            String(";strength=") + String::FromInt32(mStrength);
    return NOERROR;
}

ECode CVirtualizer::Settings::GetStrength(
    /* [out] */  Int16* strength)
{
    VALIDATE_NOT_NULL(strength);

    *strength = mStrength;
    return NOERROR;
}

ECode CVirtualizer::Settings::SetStrength(
    /* [in] */ Int16 strength)
{
    mStrength = strength;
    return NOERROR;
}

ECode CVirtualizer::GetProperties(
    /* [out] */ IVirtualizerSettings** properties)
{
    VALIDATE_NOT_NULL(properties);

    AutoPtr<IVirtualizerSettings> settings;
    ArrayOf_<Int16,1> value;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(Virtualizer_PARAM_STRENGTH, &value, &status));
    settings->SetStrength(value[0]);
    *properties = settings;
    return NOERROR;
}

ECode CVirtualizer::SetProperties(
    /* [in] */ IVirtualizerSettings* settings)
{
    Int16 tempInt16;
    settings->GetStrength(&tempInt16);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx2(Virtualizer_PARAM_STRENGTH, tempInt16, &status));
    return NOERROR;
}

const CString CVirtualizer::TAG = "Virtualizer";

// IAudioEffect
ECode CVirtualizer::ReleaseResources()
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetDescriptor(
    /* [out] */ IAudioEffectDescriptor** descriptor)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetEnabled(
    /* [in]  */ Boolean enabled,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetParameterEx(
    /* [in] */ Int32 param,
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetParameterEx2(
    /* [in] */ Int32 param,
    /* [in] */ Int16 value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetParameterEx3(
    /* [in] */ Int32 param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetParameterEx4(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetParameterEx5(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetParameterEx6(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetParameterEx(
    /* [in] */ const Int32 param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetParameterEx2(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetParameterEx3(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetParameterEx4(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetParameterEx5(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetParameterEx6(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ ArrayOf<Byte>* command,
    /* [out] */ ArrayOf<Byte>* reply,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetId(
    /* [out] */ Int32* Id)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::GetEnabled(
    /* [out] */ Boolean* getenable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::HasControl(
    /* [out] */ Boolean* control)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetEnableStatusListener(
    /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetControlStatusListener(
    /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::SetParameterListener(
    /* [in] */ IAudioEffectOnParameterChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::CheckState(
    /* [in] */ CString methodName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::CheckStatus(
    /* [in] */ Int32 status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::ByteArrayToInt32(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::ByteArrayToInt32Ex(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::Int32ToByteArray(
    /* [in] */ Int32 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::ByteArrayToInt16(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int16* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::ByteArrayToInt16Ex(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int16* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::Int16ToByteArray(
    /* [in] */ Int16 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVirtualizer::ConcatArrays(
    /* [in] */ ArrayOf<Byte>* array1,
    /* [in] */ ArrayOf<Byte>* array2,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}
