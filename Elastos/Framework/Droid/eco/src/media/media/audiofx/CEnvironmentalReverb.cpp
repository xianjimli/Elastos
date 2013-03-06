#include "media/audiofx/CEnvironmentalReverb.h"
#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <stdlib.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CEnvironmentalReverb::CEnvironmentalReverb()
{
    EnvironmentalReverb_PARAM_PROPERTIES = 10;
    EnvironmentalReverb_PROPERTY_SIZE = 26;
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
}
PInterface CEnvironmentalReverb::BaseParameterListener::Probe(
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

UInt32 CEnvironmentalReverb::BaseParameterListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CEnvironmentalReverb::BaseParameterListener::Release()
{
    return ElRefBase::Release();
}

ECode CEnvironmentalReverb::BaseParameterListener::GetInterfaceID(
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

ECode CEnvironmentalReverb::constructor(
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    /*
    super(EFFECT_TYPE_ENV_REVERB, EFFECT_TYPE_NULL, priority, audioSession);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetRoomLevel(
    /* [in] */ Int16 room)
{
    ArrayOf<Byte>* param;
    obj->Int16ToByteArray(room, param);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_ROOM_LEVEL, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetRoomLevel(
    /* [out] */ Int16* room)
{
    VALIDATE_NOT_NULL(room);
    ArrayOf_<Byte,2> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_ROOM_LEVEL, param, &status));
    obj->ByteArrayToInt16(param, room);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetRoomHFLevel(
    /* [in] */ Int16 roomHF)
{
    ArrayOf<Byte>* param;
    Int32 status;
    obj->Int16ToByteArray(roomHF, param);
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_ROOM_HF_LEVEL, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetRoomHFLevel(
    /* [out] */ Int16* roomHF)
{
    ArrayOf_<Byte,2> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_ROOM_HF_LEVEL, param, &status));
    obj->ByteArrayToInt16(param, roomHF);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetDecayTime(
    /* [in] */ Int32 decayTime)
{
    ArrayOf<Byte>* param;
    Int32 status;
    obj->Int32ToByteArray(decayTime, param);
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_DECAY_TIME, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetDecayTime(
    /* [out] */ Int32* decayTime)
{
    ArrayOf_<Byte,4> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_DECAY_TIME, param, &status));
    obj->ByteArrayToInt32(param, decayTime);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetDecayHFRatio(
    /* [in] */ Int16 decayHFRatio)
{
    ArrayOf<Byte>* param;
    obj->Int16ToByteArray(decayHFRatio, param);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_DECAY_HF_RATIO, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetDecayHFRatio(
    /* [out] */ Int16* decayHFRatio)
{
    ArrayOf_<Byte,2> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_DECAY_HF_RATIO, param, &status));
    obj->ByteArrayToInt16(param, decayHFRatio);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetReflectionsLevel(
    /* [in] */ Int16 reflectionsLevel)
{
    ArrayOf<Byte>* param;
    obj->Int16ToByteArray(reflectionsLevel, param);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_REFLECTIONS_LEVEL, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetReflectionsLevel(
    /* [out] */ Int16* reflectionsLevel)
{
    ArrayOf_<Byte,2> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_REFLECTIONS_LEVEL, param, &status));
    obj->ByteArrayToInt16(param, reflectionsLevel);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetReflectionsDelay(
    /* [in] */ Int32 reflectionsDelay)
{
    ArrayOf<Byte>* param;
    obj->Int32ToByteArray(reflectionsDelay, param);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_REFLECTIONS_DELAY, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetReflectionsDelay(
    /* [out] */ Int32* reflectionsDelay)
{
    ArrayOf_<Byte,4> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_REFLECTIONS_DELAY, param, &status));
    obj->ByteArrayToInt32(param, reflectionsDelay);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetReverbLevel(
    /* [in] */ Int16 reverbLevel)
{
    ArrayOf<Byte>* param;
    obj->Int16ToByteArray(reverbLevel, param);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_REVERB_LEVEL, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetReverbLevel(
    /* [out] */ Int16* reverbLevel)
{
    ArrayOf_<Byte,2> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_REVERB_LEVEL, param, &status));
    obj->ByteArrayToInt16(param, reverbLevel);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetReverbDelay(
    /* [in] */ Int32 reverbDelay)
{
    ArrayOf<Byte>* param;
    obj->Int32ToByteArray(reverbDelay, param);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_REVERB_DELAY, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetReverbDelay(
    /* [out] */ Int32* reverbDelay)
{
    ArrayOf_<Byte,4> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_REVERB_DELAY, param, &status));
    obj->ByteArrayToInt32(param, reverbDelay);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetDiffusion(
    /* [in] */ Int16 diffusion)
{
    ArrayOf<Byte>* param;
    obj->Int16ToByteArray(diffusion, param);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_DIFFUSION, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetDiffusion(
    /* [out] */ Int16* diffusion)
{
    ArrayOf_<Byte,2> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_DIFFUSION, param, &status));
    obj->ByteArrayToInt16(param, diffusion);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetDensity(
    /* [in] */ Int16 density)
{
    ArrayOf<Byte>* param;
    obj->Int16ToByteArray(density, param);
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_DENSITY, *param, &status));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetDensity(
    /* [out] */ Int16* density)
{
    ArrayOf_<Byte,2> param;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_DENSITY, param, &status));
    obj->ByteArrayToInt16(param, density);
    return NOERROR;
}

ECode CEnvironmentalReverb::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ const ArrayOf<Byte>& param,
    /* [in] */ const ArrayOf<Byte>& value)
{
    AutoPtr<IEnvironmentalReverbOnParameterChangeListener> l;

    Mutex::Autolock lock(mHost->mParamListenerLock);
    if (mHost->mParamListener != NULL) {
        l = mHost->mParamListener;
    }
    if (l != NULL) {
        Int32 p = -1;
        Int32 v = -1;

        if (param.GetLength() == 4) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New(0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(param, 0, &p);
            obj->Release();
        }
        if (value.GetLength() == 2) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New(0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(value, 0, &v);
            obj->Release();
        }
        if (p != -1 && v != -1) {
            l->OnParameterChange((IEnvironmentalReverb*) this->Probe(EIID_IEnvironmentalReverb), status, p, v);
        }
    }
    return NOERROR;
}

ECode CEnvironmentalReverb::SetParameterListenerEx2(
    /* [in] */ IEnvironmentalReverbOnParameterChangeListener* listener)
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

ECode CEnvironmentalReverb::Settings::constructor(
    /* [in] */ String* settings)
{
    StringTokenizer st(*settings, String("=;"));
    Int32 tokens = st.GetCount();
    if (st.GetCount() != 21) {
//        throw new IllegalArgumentException("settings: " + settings);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String key = st.NextToken();
    if (!key.Equals("EnvironmentalReverb")) {
//        throw new IllegalArgumentException("invalid settings for EnvironmentalReverb: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
/*
    try {
*/
    key = st.NextToken();
    if (!key.Equals("roomLevel")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    roomLevel = st.NextToken().ToInt32();

    if (!key.Equals("roomHFLevel")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    roomHFLevel = st.NextToken().ToInt32();

    if (!key.Equals("decayTime")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    decayTime = st.NextToken().ToInt32();

    if (!key.Equals("decayHFRatio")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    decayHFRatio = st.NextToken().ToInt32();

    if (!key.Equals("reflectionsLevel")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    reflectionsLevel = st.NextToken().ToInt32();

    if (!key.Equals("reflectionsDelay")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    reflectionsDelay = st.NextToken().ToInt32();

    if (!key.Equals("reverbLevel")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    reverbLevel = st.NextToken().ToInt32();

    if (!key.Equals("reverbDelay")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    reverbDelay = st.NextToken().ToInt32();

    if (!key.Equals("diffusion")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    diffusion = st.NextToken().ToInt32();

    if (!key.Equals("density")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    density = st.NextToken().ToInt32();

/*
     } catch (NumberFormatException nfe) {
        throw new IllegalArgumentException("invalid value for key: " + key);
     }
*/
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::toString(
    /* [out] */ String* result)
{
    *result = String("EnvironmentalReverb") +
            String(";roomLevel=") + String::FromInt32(roomLevel) +
            String(";roomHFLevel=") + String::FromInt32(roomHFLevel) +
            String(";decayTime=") + String::FromInt32(decayTime) +
            String(";decayHFRatio=") + String::FromInt32(decayHFRatio) +
            String(";reflectionsLevel=") + String::FromInt32(reflectionsLevel) +
            String(";reflectionsDelay=") + String::FromInt32(reflectionsDelay) +
            String(";reverbLevel=") + String::FromInt32(reverbLevel) +
            String(";reverbDelay=") + String::FromInt32(reverbDelay) +
            String(";diffusion=") + String::FromInt32(diffusion) +
            String(";density=") + String::FromInt32(density);
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetParameterInt32(
    /* [in] */ String param,
    /* [out] */  Int32* result)
{
    if (param.Equals("decayTime")) {
        *result = decayTime;
    }

    if (param.Equals("reflectionsDelay")) {
        *result = reflectionsDelay;
    }

    if (param.Equals("reverbDelay")) {
        *result = reverbDelay;
    }
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetParameterInt32(
    /* [in] */ String param,
    /* [in] */ Int32 result)
{
    if (param.Equals("decayTime")) {
        decayTime = result;
    }

    if (param.Equals("reflectionsDelay")) {
        reflectionsDelay = result;
    }

    if (param.Equals("reverbDelay")) {
        reverbDelay = result;
    }
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetParameterInt16(
    /* [in] */ String param,
    /* [out] */  Int16* result)
{
    if (param.Equals("roomLevel")) {
        *result = roomLevel;
    }

    if (param.Equals("roomHFLevel")) {
        *result = roomHFLevel;
    }

    if (param.Equals("decayHFRatio")) {
        *result = decayHFRatio;
    }

    if (param.Equals("reflectionsLevel")) {
        *result = reflectionsLevel;
    }

    if (param.Equals("reverbLevel")) {
        *result = reverbLevel;
    }

    if (param.Equals("diffusion")) {
        *result = diffusion;
    }

    if (param.Equals("density")) {
        *result = density;
    }

    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetParameterInt16(
    /* [in] */ String param,
    /* [in] */ Int16 result)
{
    if (param.Equals("roomLevel")) {
        roomLevel = result;
    }

    if (param.Equals("roomHFLevel")) {
        roomHFLevel = result;
    }

    if (param.Equals("decayHFRatio")) {
        decayHFRatio = result;
    }

    if (param.Equals("reflectionsLevel")) {
        reflectionsLevel = result;
    }

    if (param.Equals("reverbLevel")) {
        reverbLevel = result;
    }

    if (param.Equals("diffusion")) {
        diffusion = result;
    }

    if (param.Equals("density")) {
        density = result;
    }
    return NOERROR;
}

ECode CEnvironmentalReverb::GetProperties(
        /* [out] */ IEnvironmentalReverbSettings** properties)
{
    ArrayOf_<Byte, 26> param; //byte[PROPERTY_SIZE]
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_PROPERTIES,param,&status));

    AutoPtr<IEnvironmentalReverbSettings> settings;
    Int16 statusInt16;
    obj->ByteArrayToInt16Ex(param, 0, &statusInt16);
    settings->SetParameterInt16(String("roomLevel"), statusInt16);
    obj->ByteArrayToInt16Ex(param, 2, &statusInt16);
    settings->SetParameterInt16(String("roomHFLevel"), statusInt16);
    obj->ByteArrayToInt32Ex(param, 4, &status);
    settings->SetParameterInt32(String("decayTime"), status);
    obj->ByteArrayToInt16Ex(param, 8, &statusInt16);
    settings->SetParameterInt16(String("decayHFRatio"), statusInt16);
    obj->ByteArrayToInt16Ex(param, 10, &statusInt16);
    settings->SetParameterInt16(String("reflectionsLevel"), statusInt16);
    obj->ByteArrayToInt32Ex(param, 12, &status);
    settings->SetParameterInt32(String("reflectionsDelay"), status);
    obj->ByteArrayToInt16Ex(param, 16, &statusInt16);
    settings->SetParameterInt16(String("reverbLevel"), statusInt16);
    obj->ByteArrayToInt32Ex(param, 18, &status);
    settings->SetParameterInt32(String("reverbDelay"), status);
    obj->ByteArrayToInt16Ex(param, 22, &statusInt16);
    settings->SetParameterInt16(String("diffusion"), statusInt16);
    obj->ByteArrayToInt16Ex(param, 24, &statusInt16);
    settings->SetParameterInt16(String("density"), statusInt16);
    *properties = settings;
    return NOERROR;
}

ECode CEnvironmentalReverb::SetProperties(
    /* [in] */ IEnvironmentalReverbSettings* settings)
{
    ArrayOf<Byte>* tempResult1;
    ArrayOf<Byte>* tempResult2;
    Int16 tempInt16Parameter1,tempInt16Parameter2;
    Int32 tempInt32Parameter1;
    ArrayOf<Byte>* tempByteArray1;
    ArrayOf<Byte>* tempByteArray2;

    settings->GetParameterInt16(String("roomLevel"), &tempInt16Parameter1);
    obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    settings->GetParameterInt16(String("roomHFLevel"), &tempInt16Parameter2);
    obj->Int16ToByteArray(tempInt16Parameter2, tempByteArray2);
    obj->ConcatArrays(*tempByteArray1, *tempByteArray2, &tempResult1);

    settings->GetParameterInt32(String("decayTime"), &tempInt32Parameter1);
    obj->Int32ToByteArray(tempInt32Parameter1, tempByteArray1);
    obj->ConcatArrays(*tempResult1, *tempByteArray1, &tempResult2);

    settings->GetParameterInt16(String("decayHFRatio"), &tempInt16Parameter1);
    obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    obj->ConcatArrays(*tempResult2, *tempByteArray1, &tempResult1);

    settings->GetParameterInt16(String("reflectionsLevel"), &tempInt16Parameter1);
    obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    obj->ConcatArrays(*tempResult1, *tempByteArray1, &tempResult2);

    settings->GetParameterInt32(String("reflectionsDelay"), &tempInt32Parameter1);
    obj->Int32ToByteArray(tempInt32Parameter1, tempByteArray1);
    obj->ConcatArrays(*tempResult2, *tempByteArray1, &tempResult1);

    settings->GetParameterInt16(String("reverbLevel"), &tempInt16Parameter1);
    obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    obj->ConcatArrays(*tempResult2, *tempByteArray1, &tempResult1);

    settings->GetParameterInt32(String("reverbDelay"), &tempInt32Parameter1);
    obj->Int32ToByteArray(tempInt32Parameter1, tempByteArray1);
    obj->ConcatArrays(*tempResult1, *tempByteArray1, &tempResult2);

    settings->GetParameterInt16(String("diffusion"), &tempInt16Parameter1);
    obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    obj->ConcatArrays(*tempResult2, *tempByteArray1, &tempResult1);

    settings->GetParameterInt16(String("density"), &tempInt16Parameter1);
    obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    obj->ConcatArrays(*tempResult1, *tempByteArray1, &tempResult2);
    ArrayOf<Byte>* param = tempResult2;
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_PROPERTIES, *param, &status));
    return NOERROR;
}

const CString CEnvironmentalReverb::TAG = "EnvironmentalReverb";
