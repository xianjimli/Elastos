
#include "media/audiofx/CBassBoost.h"
#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CBassBoost::CBassBoost()
{
    mStrengthSupported = FALSE;
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
    AutoPtr<IAudioEffect> obj;
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
    obj->CheckStatus(obj->GetParameterEx2(BassBoost_PARAM_STRENGTH_SUPPORTED, &value, &status));
    obj->Release();
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
    AutoPtr<IAudioEffect> obj;
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
    obj->CheckStatus(obj->SetParameterEx2(BassBoost_PARAM_STRENGTH, strength, &status));
    obj->Release();
    return NOERROR;
}

ECode CBassBoost::GetRoundedStrength(
    /* [out] */ Int16* strength)
{
    ArrayOf_<Int16,1> value;
    Int32 status;
    AutoPtr<IAudioEffect> obj;
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
    obj->CheckStatus(obj->GetParameterEx3(BassBoost_PARAM_STRENGTH_SUPPORTED, &value, &status));
    obj->Release();
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
    Int32 tokesn = st.GetCount();
    if (st.GetCount() != 3) {
//        throw new IllegalArgumentException("settings: " + settings);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String key = st.NextToken();
    if (!key.Equals("BassBoost")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
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
    //strength = Short.parseShort(st.nextToken());
/*
     } catch (NumberFormatException nfe) {
        throw new IllegalArgumentException("invalid value for key: " + key);
     }
*/
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::Settings::toString(
    /* [out] */ String* result)
{
/*
        String str = new String (
                "BassBoost"+
                ";strength="+Short.toString(strength)
                );
        return str;
 */
    return E_NOT_IMPLEMENTED;
}

ECode CBassBoost::Settings::GetStrength(
    /* [out] */  Int16* strength)
{
    *strength = mStrength;
}

ECode CBassBoost::Settings::SetStrength(
    /* [in] */ Int16 strength)
{
    mStrength = strength;
}

ECode CBassBoost::GetProperties(
    /* [out] */ IBassBoostSettings** properties)
{
    AutoPtr<IBassBoostSettings> settings;
    ArrayOf_<Int16, 1> value;
    AutoPtr<IAudioEffect> obj;
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(BassBoost_PARAM_STRENGTH,&value,&status));
    settings->SetStrength(value[0]);
    *properties = settings;
    return NOERROR;
}

ECode CBassBoost::SetProperties(
    /* in */ IBassBoostSettings* settings)
{
    AutoPtr<IAudioEffect> obj;
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
    Int16 temp;
    settings->GetStrength(&temp);
    Int32 result;
    obj->CheckStatus(obj->SetParameterEx2(BassBoost_PARAM_STRENGTH,temp,&result));
    return NOERROR;
}

const CString CBassBoost::TAG = "BassBoost";

