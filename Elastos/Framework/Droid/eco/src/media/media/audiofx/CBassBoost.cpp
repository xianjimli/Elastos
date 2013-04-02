
#include "media/audiofx/CBassBoost.h"
#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <stdlib.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CBassBoost::CBassBoost()
{
    mStrengthSupported = FALSE;
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
}

PInterface CBassBoost::BaseParameterListener::Probe(
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

UInt32 CBassBoost::BaseParameterListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBassBoost::BaseParameterListener::Release()
{
    return ElRefBase::Release();
}

ECode CBassBoost::BaseParameterListener::GetInterfaceID(
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

ECode CBassBoost::constructor(
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    ArrayOf_<Int32,1> value;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx2(BassBoost_PARAM_STRENGTH_SUPPORTED, &value, &status));
    mStrengthSupported = (value[0] != 0);
    return NOERROR;
}

ECode CBassBoost::GetStrengthSupported(
    /* [out] */ Boolean* isSupported)
{
    VALIDATE_NOT_NULL(isSupported);

    *isSupported = mStrengthSupported;
    return NOERROR;
}

ECode CBassBoost::SetStrength(
    /* [in] */ Int16 strength)
{
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx2(BassBoost_PARAM_STRENGTH, strength, &status));
    return NOERROR;
}

ECode CBassBoost::GetRoundedStrength(
    /* [out] */ Int16* strength)
{
    VALIDATE_NOT_NULL(strength);

    ArrayOf_<Int16,1> value;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(BassBoost_PARAM_STRENGTH_SUPPORTED, &value, &status));
    mStrengthSupported = (value[0] != 0);
    return NOERROR;
}

ECode CBassBoost::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ const ArrayOf<Byte>& param,
    /* [in] */ const ArrayOf<Byte>& value)
{
    AutoPtr<IBassBoostOnParameterChangeListener> l;
    Mutex::Autolock lock(mHost->mParamListenerLock);
    if (mHost->mParamListener != NULL) {
        l = mHost->mParamListener;
    }
    if (l != NULL) {
        Int32 p = -1;
        Int16 v = -1;
        if (param.GetLength() == 4) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New(0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(param, 0, &p);
            obj->Release();
        }
        if (value.GetLength() == 2) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New(0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt16Ex(value, 0, &v);
            obj->Release();
        }
        if (p != -1 && v != -1) {
            l->OnParameterChange((IBassBoost*) this->Probe(EIID_IBassBoost), status, p, v);
        }
    }
    return NOERROR;
}

ECode CBassBoost::SetParameterListenerEx(
    /* [in] */ IBassBoostOnParameterChangeListener* listener)
{
    Mutex::Autolock lock(mParamListenerLock);
    if (mParamListener != NULL) {
        mParamListener = listener;
        mBaseParamListener = new BaseParameterListener();
        AutoPtr<IAudioEffect> obj;
        CAudioEffect::New(0,0,(IAudioEffect**)&obj);
        obj->SetParameterListener(mBaseParamListener);
        obj->Release();
    }
    return NOERROR;
}

ECode CBassBoost::Settings::constructor(
    /* [in] */ String* settings)
{
    StringTokenizer st(*settings, String("=;"));
    //Int32 tokesn = st.GetCount();
    if (st.GetCount() != 3) {
//        throw new IllegalArgumentException("settings: " + settings);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String key = st.NextToken();
    if (!key.Equals("BassBoost")) {
//        throw new IllegalArgumentException("invalid settings for BassBoost: " + key);
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
    strength = st.NextToken().ToInt32();
/*
     } catch (NumberFormatException nfe) {
        throw new IllegalArgumentException("invalid value for key: " + key);
     }
*/
    return NOERROR;
}

ECode CBassBoost::Settings::toString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = String("BassBoost") +
            String(";strength=") + String::FromInt32(strength);
    return NOERROR;
}

ECode CBassBoost::Settings::GetStrength(
    /* [out] */  Int16* result)
{
    VALIDATE_NOT_NULL(result);

    *result = strength;
    return NOERROR;
}

ECode CBassBoost::Settings::SetStrength(
    /* [in] */ Int16 result)
{
    strength = result;
    return NOERROR;
}

ECode CBassBoost::GetProperties(
    /* [out] */ IBassBoostSettings** properties)
{
    VALIDATE_NOT_NULL(properties);

    AutoPtr<IBassBoostSettings> settings;
    ArrayOf_<Int16, 1> value;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(BassBoost_PARAM_STRENGTH,&value,&status));
    settings->SetStrength(value[0]);
    *properties = settings;
    return NOERROR;
}

ECode CBassBoost::SetProperties(
    /* in */ IBassBoostSettings* settings)
{
    Int16 temp;
    settings->GetStrength(&temp);
    Int32 result;
    obj->CheckStatus(obj->SetParameterEx2(BassBoost_PARAM_STRENGTH,temp,&result));
    return NOERROR;
}

const CString CBassBoost::TAG = "BassBoost";

// IAudioEffect
ECode CBassBoost::ReleaseResources()
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetDescriptor(
    /* [out] */ IAudioEffectDescriptor** descriptor)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetEnabled(
    /* [in]  */ Boolean enabled,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetParameter(
    /* [in] */ const ArrayOf<Byte>& param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetParameterEx(
    /* [in] */ Int32 param,
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetParameterEx2(
    /* [in] */ Int32 param,
    /* [in] */ Int16 value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetParameterEx3(
    /* [in] */ Int32 param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetParameterEx4(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Int32>& value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetParameterEx5(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Int16>& value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetParameterEx6(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetParameter(
    /* [in] */ const ArrayOf<Byte>& param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */  Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetParameterEx(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetParameterEx2(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetParameterEx3(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetParameterEx4(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetParameterEx5(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetParameterEx6(
    /* [in] */ const ArrayOf<Int32>& param,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ const ArrayOf<Byte>& command,
    /* [out] */ ArrayOf<Byte>* reply,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetId(
    /* [out] */ Int32* Id)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::GetEnabled(
    /* [out] */ Boolean* getenable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::HasControl(
    /* [out] */ Boolean* control)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetEnableStatusListener(
    /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetControlStatusListener(
    /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::SetParameterListener(
    /* [in] */ IAudioEffectOnParameterChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::CheckState(
    /* [in] */ CString methodName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::CheckStatus(
    /* [in] */ Int32 status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::ByteArrayToInt32(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::ByteArrayToInt32Ex(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::Int32ToByteArray(
    /* [in] */ Int32 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::ByteArrayToInt16(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [out] */ Int16* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::ByteArrayToInt16Ex(
    /* [in] */ const ArrayOf<Byte>& valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int16* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::Int16ToByteArray(
    /* [in] */ Int16 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::ConcatArrays(
    /* [in] */ const ArrayOf<Byte>& array1,
    /* [in] */ const ArrayOf<Byte>& array2,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}
