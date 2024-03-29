#include "media/audiofx/CEnvironmentalReverb.h"
#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <stdlib.h>
#include <elastos/AutoFree.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CEnvironmentalReverb::CEnvironmentalReverb()
{
    EnvironmentalReverb_PARAM_PROPERTIES = 10;
    EnvironmentalReverb_PROPERTY_SIZE = 26;
    CAudioEffect::New((IUUID*) 0,(IUUID*) 0,0,0,(IAudioEffect**)&obj);
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
    AutoFree< ArrayOf<Byte> > param;
    obj->Int16ToByteArray(room, (ArrayOf<Byte>**)&param);
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_ROOM_LEVEL, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetRoomLevel(
    /* [out] */ Int16* room)
{
    VALIDATE_NOT_NULL(room);

    ArrayOf_<Byte,2> param;
    Int32 status = 0;;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_ROOM_LEVEL, &param, &status)));
    *room = obj->ByteArrayToInt16(&param, room);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetRoomHFLevel(
    /* [in] */ Int16 roomHF)
{
    AutoFree< ArrayOf<Byte> > param;
    Int32 status = 0;
    obj->Int16ToByteArray(roomHF, (ArrayOf<Byte>**)&param);
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_ROOM_HF_LEVEL, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetRoomHFLevel(
    /* [out] */ Int16* roomHF)
{
    VALIDATE_NOT_NULL(roomHF);

    ArrayOf_<Byte,2> param;
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_ROOM_HF_LEVEL, &param, &status)));
    *roomHF = obj->ByteArrayToInt16(&param, roomHF);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetDecayTime(
    /* [in] */ Int32 decayTime)
{
    AutoFree< ArrayOf<Byte> > param;
    Int32 status = 0;
    obj->Int32ToByteArray(decayTime, (ArrayOf<Byte>**)&param);
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_DECAY_TIME, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetDecayTime(
    /* [out] */ Int32* decayTime)
{
    VALIDATE_NOT_NULL(decayTime);

    ArrayOf_<Byte,4> param;
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_DECAY_TIME, &param, &status)));
    *decayTime = obj->ByteArrayToInt32(&param, decayTime);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetDecayHFRatio(
    /* [in] */ Int16 decayHFRatio)
{
    AutoFree< ArrayOf<Byte> > param;
    obj->Int16ToByteArray(decayHFRatio, (ArrayOf<Byte>**)&param);
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_DECAY_HF_RATIO, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetDecayHFRatio(
    /* [out] */ Int16* decayHFRatio)
{
    VALIDATE_NOT_NULL(decayHFRatio);

    ArrayOf_<Byte,2> param;
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_DECAY_HF_RATIO, &param, &status)));
    *decayHFRatio = obj->ByteArrayToInt16(&param, decayHFRatio);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetReflectionsLevel(
    /* [in] */ Int16 reflectionsLevel)
{
    AutoFree< ArrayOf<Byte> > param;
    obj->Int16ToByteArray(reflectionsLevel, (ArrayOf<Byte>**)&param);
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_REFLECTIONS_LEVEL, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetReflectionsLevel(
    /* [out] */ Int16* reflectionsLevel)
{
    VALIDATE_NOT_NULL(reflectionsLevel);

    ArrayOf_<Byte,2> param;
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_REFLECTIONS_LEVEL, &param, &status)));
    *reflectionsLevel = obj->ByteArrayToInt16(&param, reflectionsLevel);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetReflectionsDelay(
    /* [in] */ Int32 reflectionsDelay)
{
    AutoFree< ArrayOf<Byte> > param;
    obj->Int32ToByteArray(reflectionsDelay, (ArrayOf<Byte>**)&param);
    Int32 status;
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_REFLECTIONS_DELAY, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetReflectionsDelay(
    /* [out] */ Int32* reflectionsDelay)
{
    VALIDATE_NOT_NULL(reflectionsDelay);

    ArrayOf_<Byte,4> param;
    Int32 status;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_REFLECTIONS_DELAY, &param, &status)));
    *reflectionsDelay = obj->ByteArrayToInt32(&param, reflectionsDelay);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetReverbLevel(
    /* [in] */ Int16 reverbLevel)
{
    AutoFree< ArrayOf<Byte> > param;
    obj->Int16ToByteArray(reverbLevel, (ArrayOf<Byte>**)&param);
    Int32 status;
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_REVERB_LEVEL, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetReverbLevel(
    /* [out] */ Int16* reverbLevel)
{
    VALIDATE_NOT_NULL(reverbLevel);

    ArrayOf_<Byte,2> param;
    Int32 status;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_REVERB_LEVEL, &param, &status)));
    *reverbLevel = obj->ByteArrayToInt16(&param, reverbLevel);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetReverbDelay(
    /* [in] */ Int32 reverbDelay)
{
    AutoFree< ArrayOf<Byte> > param;
    obj->Int32ToByteArray(reverbDelay, (ArrayOf<Byte>**)&param);
    Int32 status;
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_REVERB_DELAY, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetReverbDelay(
    /* [out] */ Int32* reverbDelay)
{
    VALIDATE_NOT_NULL(reverbDelay);

    ArrayOf_<Byte,4> param;
    Int32 status;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_REVERB_DELAY, &param, &status)));
    *reverbDelay = obj->ByteArrayToInt32(&param, reverbDelay);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetDiffusion(
    /* [in] */ Int16 diffusion)
{
    AutoFree< ArrayOf<Byte> > param;
    obj->Int16ToByteArray(diffusion, (ArrayOf<Byte>**)&param);
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_DIFFUSION, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetDiffusion(
    /* [out] */ Int16* diffusion)
{
    VALIDATE_NOT_NULL(diffusion);

    ArrayOf_<Byte,2> param;
    Int32 status = 0;
    FAIL_RETURN(obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_DIFFUSION, &param, &status)));
    *diffusion = obj->ByteArrayToInt16(&param, diffusion);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetDensity(
    /* [in] */ Int16 density)
{
    AutoFree< ArrayOf<Byte> > param;
    obj->Int16ToByteArray(density, (ArrayOf<Byte>**)&param);
    Int32 status;
    FAIL_RETURN(obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_DENSITY, param.Get(), &status)));
    return NOERROR;
}

ECode CEnvironmentalReverb::GetDensity(
    /* [out] */ Int16* density)
{
    VALIDATE_NOT_NULL(density);

    ArrayOf_<Byte,2> param;
    Int32 status = 0;
    obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_DENSITY, &param, &status));
    *density = obj->ByteArrayToInt16(&param, density);
    return NOERROR;
}

ECode CEnvironmentalReverb::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value)
{
    AutoPtr<IEnvironmentalReverbOnParameterChangeListener> l;

    Mutex::Autolock lock(&mHost->mParamListenerLock);
    if (mHost->mParamListener != NULL) {
        l = mHost->mParamListener;
    }
    if (l != NULL) {
        Int32 p = -1;
        Int32 v = -1;

        if (param->GetLength() == 4) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New((IUUID*) 0,(IUUID*) 0,0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(param, 0, &p);
            obj->Release();
        }
        if (value->GetLength() == 2) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New((IUUID*) 0,(IUUID*) 0,0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(value, 0, &v);
            obj->Release();
        }
        if (p != -1 && v != -1) {
            l->OnParameterChange((IEnvironmentalReverb*) this->Probe(EIID_IEnvironmentalReverb), status, p, v);
        }
    }
    return NOERROR;
}

ECode CEnvironmentalReverb::SetParameterListenerEx(
    /* [in] */ IEnvironmentalReverbOnParameterChangeListener* listener)
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

ECode CEnvironmentalReverb::Settings::constructor(
    /* [in] */ String* settings)
{
    StringTokenizer st(*settings, String("=;"));
    //Int32 tokens = st.GetCount();
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
    mRoomLevel = st.NextToken().ToInt32();

    if (!key.Equals("roomHFLevel")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mRoomHFLevel = st.NextToken().ToInt32();

    if (!key.Equals("decayTime")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDecayTime = st.NextToken().ToInt32();

    if (!key.Equals("decayHFRatio")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDecayHFRatio = st.NextToken().ToInt32();

    if (!key.Equals("reflectionsLevel")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mReflectionsLevel = st.NextToken().ToInt32();

    if (!key.Equals("reflectionsDelay")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mReflectionsDelay = st.NextToken().ToInt32();

    if (!key.Equals("reverbLevel")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mReverbLevel = st.NextToken().ToInt32();

    if (!key.Equals("reverbDelay")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mReverbDelay = st.NextToken().ToInt32();

    if (!key.Equals("diffusion")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDiffusion = st.NextToken().ToInt32();

    if (!key.Equals("density")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDensity = st.NextToken().ToInt32();

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
    VALIDATE_NOT_NULL(result);

    *result = String("EnvironmentalReverb") +
            String(";roomLevel=") + String::FromInt32(mRoomLevel) +
            String(";roomHFLevel=") + String::FromInt32(mRoomHFLevel) +
            String(";decayTime=") + String::FromInt32(mDecayTime) +
            String(";decayHFRatio=") + String::FromInt32(mDecayHFRatio) +
            String(";reflectionsLevel=") + String::FromInt32(mReflectionsLevel) +
            String(";reflectionsDelay=") + String::FromInt32(mReflectionsDelay) +
            String(";reverbLevel=") + String::FromInt32(mReverbLevel) +
            String(";reverbDelay=") + String::FromInt32(mReverbDelay) +
            String(";diffusion=") + String::FromInt32(mDiffusion) +
            String(";density=") + String::FromInt32(mDensity);
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetRoomLevel(
    /* [out] */ Int16* roomLevel)
{
    VALIDATE_NOT_NULL(roomLevel);
    *roomLevel = mRoomLevel;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetRoomLevel(
    /* [in] */ Int16 roomLevel)
{
    mRoomLevel = roomLevel;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetRoomHFLevel(
    /* [out] */ Int16* roomHFLevel)
{
    VALIDATE_NOT_NULL(roomHFLevel);
    *roomHFLevel = mRoomHFLevel;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetRoomHFLevel(
    /* [in] */ Int16 roomHFLevel)
{
    mRoomHFLevel = roomHFLevel;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetDecayTime(
    /* [out] */ Int32* decayTime)
{
    VALIDATE_NOT_NULL(decayTime);
    *decayTime = mDecayTime;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetDecayTime(
    /* [in] */ Int32 decayTime)
{
    mDecayTime = decayTime;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetDecayHFRatio(
    /* [out] */ Int16* decayHFRatio)
{
    VALIDATE_NOT_NULL(decayHFRatio);
    *decayHFRatio = mDecayHFRatio;
    return NOERROR;
}
ECode CEnvironmentalReverb::Settings::SetDecayHFRatio(
    /* [in] */ Int16 decayHFRatio)
{
    mDecayHFRatio = decayHFRatio;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetReflectionsLevel(
    /* [out] */ Int16* reflectionsLevel)
{
    VALIDATE_NOT_NULL(reflectionsLevel);
    *reflectionsLevel = mReflectionsLevel;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetReflectionsLevel(
    /* [in] */ Int16 reflectionsLevel)
{
    mReflectionsLevel = reflectionsLevel;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetReflectionsDelay(
    /* [out] */ Int32* reflectionsDelay)
{
    VALIDATE_NOT_NULL(reflectionsDelay);
    *reflectionsDelay = mReflectionsDelay;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetReflectionsDelay(
    /* [in] */ Int32 reflectionsDelay)
{
    mReflectionsDelay = reflectionsDelay;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetReverbLevel(
    /* [out] */ Int16* reverbLevel)
{
    VALIDATE_NOT_NULL(reverbLevel);
    *reverbLevel = mReverbLevel;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetReverbLevel(
    /* [in] */ Int16 reverbLevel)
{
    mReverbLevel = reverbLevel;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetReverbDelay(
    /* [out] */ Int16* reverbDelay)
{
    VALIDATE_NOT_NULL(reverbDelay);
    *reverbDelay = mReverbDelay;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetReverbDelay(
    /* [in] */ Int16 reverbDelay)
{
    mReverbDelay = reverbDelay;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetDiffusion(
    /* [out] */ Int16* diffusion)
{
    VALIDATE_NOT_NULL(diffusion);
    *diffusion = mDiffusion;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetDiffusion(
    /* [in] */ Int16 diffusion)
{
    mDiffusion = diffusion;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::GetDensity(
    /* [out] */ Int16* density)
{
    VALIDATE_NOT_NULL(density);
    *density = mDensity;
    return NOERROR;
}

ECode CEnvironmentalReverb::Settings::SetDensity(
    /* [in] */ Int16 density)
{
    mDensity = density;
    return NOERROR;
}

ECode CEnvironmentalReverb::GetProperties(
        /* [out] */ IEnvironmentalReverbSettings** properties)
{
    VALIDATE_NOT_NULL(properties);

    // ArrayOf<Byte>* param = ArrayOf<Byte>::Alloc(EnvironmentalReverb_PROPERTY_SIZE);
    // Int32 status;
    // obj->CheckStatus(obj->GetParameterEx(EnvironmentalReverb_PARAM_PROPERTIES,*param,&status));

    // AutoPtr<IEnvironmentalReverbSettings> settings;
    // Int16 statusInt16;
    // obj->ByteArrayToInt16Ex(*param, 0, &statusInt16);
    // settings->SetParameterInt16(String("roomLevel"), statusInt16);
    // obj->ByteArrayToInt16Ex(*param, 2, &statusInt16);
    // settings->SetParameterInt16(String("roomHFLevel"), statusInt16);
    // obj->ByteArrayToInt32Ex(*param, 4, &status);
    // settings->SetParameterInt32(String("decayTime"), status);
    // obj->ByteArrayToInt16Ex(*param, 8, &statusInt16);
    // settings->SetParameterInt16(String("decayHFRatio"), statusInt16);
    // obj->ByteArrayToInt16Ex(*param, 10, &statusInt16);
    // settings->SetParameterInt16(String("reflectionsLevel"), statusInt16);
    // obj->ByteArrayToInt32Ex(*param, 12, &status);
    // settings->SetParameterInt32(String("reflectionsDelay"), status);
    // obj->ByteArrayToInt16Ex(*param, 16, &statusInt16);
    // settings->SetParameterInt16(String("reverbLevel"), statusInt16);
    // obj->ByteArrayToInt32Ex(*param, 18, &status);
    // settings->SetParameterInt32(String("reverbDelay"), status);
    // obj->ByteArrayToInt16Ex(*param, 22, &statusInt16);
    // settings->SetParameterInt16(String("diffusion"), statusInt16);
    // obj->ByteArrayToInt16Ex(*param, 24, &statusInt16);
    // settings->SetParameterInt16(String("density"), statusInt16);
    // *properties = settings;
    // ArrayOf<Byte>::Free(param);
    return NOERROR;
}

ECode CEnvironmentalReverb::SetProperties(
    /* [in] */ IEnvironmentalReverbSettings* settings)
{
    // ArrayOf<Byte>* tempResult1 = NULL;
    // ArrayOf<Byte>* tempResult2 = NULL;
    // Int16 tempInt16Parameter1,tempInt16Parameter2;
    // Int32 tempInt32Parameter1;
    // ArrayOf<Byte>* tempByteArray1 = NULL;;
    // ArrayOf<Byte>* tempByteArray2 = NULL;;

    // settings->GetParameterInt16(String("roomLevel"), &tempInt16Parameter1);
    // obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    // settings->GetParameterInt16(String("roomHFLevel"), &tempInt16Parameter2);
    // obj->Int16ToByteArray(tempInt16Parameter2, tempByteArray2);
    // obj->ConcatArrays(*tempByteArray1, *tempByteArray2, &tempResult1);

    // settings->GetParameterInt32(String("decayTime"), &tempInt32Parameter1);
    // obj->Int32ToByteArray(tempInt32Parameter1, tempByteArray1);
    // obj->ConcatArrays(*tempResult1, *tempByteArray1, &tempResult2);

    // settings->GetParameterInt16(String("decayHFRatio"), &tempInt16Parameter1);
    // obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    // obj->ConcatArrays(*tempResult2, *tempByteArray1, &tempResult1);

    // settings->GetParameterInt16(String("reflectionsLevel"), &tempInt16Parameter1);
    // obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    // obj->ConcatArrays(*tempResult1, *tempByteArray1, &tempResult2);

    // settings->GetParameterInt32(String("reflectionsDelay"), &tempInt32Parameter1);
    // obj->Int32ToByteArray(tempInt32Parameter1, tempByteArray1);
    // obj->ConcatArrays(*tempResult2, *tempByteArray1, &tempResult1);

    // settings->GetParameterInt16(String("reverbLevel"), &tempInt16Parameter1);
    // obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    // obj->ConcatArrays(*tempResult2, *tempByteArray1, &tempResult1);

    // settings->GetParameterInt32(String("reverbDelay"), &tempInt32Parameter1);
    // obj->Int32ToByteArray(tempInt32Parameter1, tempByteArray1);
    // obj->ConcatArrays(*tempResult1, *tempByteArray1, &tempResult2);

    // settings->GetParameterInt16(String("diffusion"), &tempInt16Parameter1);
    // obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    // obj->ConcatArrays(*tempResult2, *tempByteArray1, &tempResult1);

    // settings->GetParameterInt16(String("density"), &tempInt16Parameter1);
    // obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    // obj->ConcatArrays(*tempResult1, *tempByteArray1, &tempResult2);
    // ArrayOf<Byte>* param = tempResult2;
    // Int32 status;
    // obj->CheckStatus(obj->SetParameterEx3(EnvironmentalReverb_PARAM_PROPERTIES, *param, &status));
    return NOERROR;
}

const CString CEnvironmentalReverb::TAG = "EnvironmentalReverb";

// IAudioEffect
ECode CEnvironmentalReverb::ReleaseResources()
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetDescriptor(
    /* [out] */ IAudioEffectDescriptor** descriptor)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetEnabled(
    /* [in]  */ Boolean enabled,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetParameterEx(
    /* [in] */ Int32 param,
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetParameterEx2(
    /* [in] */ Int32 param,
    /* [in] */ Int16 value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetParameterEx3(
    /* [in] */ Int32 param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetParameterEx4(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetParameterEx5(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetParameterEx6(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetParameterEx(
    /* [in] */ const Int32 param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetParameterEx2(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetParameterEx3(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetParameterEx4(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetParameterEx5(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetParameterEx6(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ ArrayOf<Byte>* command,
    /* [out] */ ArrayOf<Byte>* reply,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetId(
    /* [out] */ Int32* Id)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::GetEnabled(
    /* [out] */ Boolean* getenable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::HasControl(
    /* [out] */ Boolean* control)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetEnableStatusListener(
    /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetControlStatusListener(
    /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::SetParameterListener(
    /* [in] */ IAudioEffectOnParameterChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::CheckState(
    /* [in] */ CString methodName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::CheckStatus(
    /* [in] */ Int32 status)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::ByteArrayToInt32(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::ByteArrayToInt32Ex(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::Int32ToByteArray(
    /* [in] */ Int32 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::ByteArrayToInt16(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int16* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::ByteArrayToInt16Ex(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int16* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::Int16ToByteArray(
    /* [in] */ Int16 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnvironmentalReverb::ConcatArrays(
    /* [in] */ ArrayOf<Byte>* array1,
    /* [in] */ ArrayOf<Byte>* array2,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
}
