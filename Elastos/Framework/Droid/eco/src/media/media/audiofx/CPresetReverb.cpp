#include "media/audiofx/CPresetReverb.h"
#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <stdlib.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CPresetReverb::CPresetReverb()
{
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
}

PInterface CPresetReverb::BaseParameterListener::Probe(
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

UInt32 CPresetReverb::BaseParameterListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPresetReverb::BaseParameterListener::Release()
{
    return ElRefBase::Release();
}

ECode CPresetReverb::BaseParameterListener::GetInterfaceID(
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

ECode CPresetReverb::constructor(
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    /*
            super(EFFECT_TYPE_PRESET_REVERB, EFFECT_TYPE_NULL, priority, audioSession);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CPresetReverb::SetPreset(
    /* [in] */ Int16 preset)
{
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx2(PresetReverb_PARAM_PRESET, preset, &status));
    return NOERROR;
}

ECode CPresetReverb::GetPreset(
    /* [out] */ Int16* preset)
{
    VALIDATE_NOT_NULL(preset);

    ArrayOf_<Int16,1> value;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(PresetReverb_PARAM_PRESET, &value, &status));
    *preset = value[0];
    return NOERROR;
}

ECode CPresetReverb::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ const ArrayOf<Byte>& param,
    /* [in] */ const ArrayOf<Byte>& value)
{
    AutoPtr<IPresetReverbOnParameterChangeListener> l;

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
            l->OnParameterChange((IPresetReverb*) this->Probe(EIID_IPresetReverb), status, p, v);
        }
    }
    return NOERROR;
}

ECode CPresetReverb::SetParameterListenerEx4(
        /* [in] */ IPresetReverbOnParameterChangeListener* listener)
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

ECode CPresetReverb::Settings::constructor(
    /* [in] */ String* settings)
{
    StringTokenizer st(*settings, String("=;"));
    Int32 tokens = st.GetCount();
    if (st.GetCount() != 3) {
//        throw new IllegalArgumentException("settings: " + settings);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String key = st.NextToken();
    if (!key.Equals("PresetReverb")) {
//        throw new IllegalArgumentException("invalid settings for PresetReverb: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
/*
    try {
*/
    key = st.NextToken();
    if (!key.Equals("preset")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    preset = st.NextToken().ToInt32();
/*
     } catch (NumberFormatException nfe) {
        throw new IllegalArgumentException("invalid value for key: " + key);
     }
*/
    return NOERROR;
}

ECode CPresetReverb::Settings::toString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = String("PresetReverb") +
            String(";preset=") + String::FromInt32(preset);
    return NOERROR;
}

ECode CPresetReverb::Settings::GetPreset(
    /* [out] */  Int16* result)
{
    VALIDATE_NOT_NULL(result);

    *result = preset;
    return NOERROR;
}

ECode CPresetReverb::Settings::SetPreset(
    /* [in] */ Int16 result)
{
    preset = result;
    return NOERROR;
}

ECode CPresetReverb::GetProperties(
    /* [out] */ IPresetReverbSettings** properties)
{
    VALIDATE_NOT_NULL(properties);

    AutoPtr<IPresetReverbSettings> settings;
    ArrayOf_<Int16,1> value;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(PresetReverb_PARAM_PRESET, &value, &status));
    settings->SetPreset(value[0]);
    *properties = settings;
    return NOERROR;
}

ECode CPresetReverb::SetProperties(
    /* [in] */ IPresetReverbSettings* settings)
{
    Int16 tempInt16;
    settings->GetPreset(&tempInt16);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx2(PresetReverb_PARAM_PRESET, tempInt16, &status));
    return NOERROR;
}

const CString CPresetReverb::TAG = "PresetReverb";
