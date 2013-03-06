#include "media/audiofx/CEqualizer.h"
#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <stdlib.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CEqualizer::CEqualizer()
{
    Equalizer_PARAM_PROPERTIES = 9;
    mNumBands = 0;
    CAudioEffect::New(0,0,(IAudioEffect**)&obj);
}
PInterface CEqualizer::BaseParameterListener::Probe(
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

UInt32 CEqualizer::BaseParameterListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CEqualizer::BaseParameterListener::Release()
{
    return ElRefBase::Release();
}

ECode CEqualizer::BaseParameterListener::GetInterfaceID(
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

ECode CEqualizer::constructor(
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    /*
    super(EFFECT_TYPE_EQUALIZER, EFFECT_TYPE_NULL, priority, audioSession);
    */
    Int16 numOfBands;
    GetNumberOfBands(&numOfBands);
    Int16 tempInt16;
    GetNumberOfPresets(&tempInt16);
    mNumPresets = (Int32) tempInt16;
    /*

        if (mNumPresets != 0) {
            mPresetNames = new String[mNumPresets];
            byte[] value = new byte[PARAM_STRING_SIZE_MAX];
            int[] param = new int[2];
            param[0] = PARAM_GET_PRESET_NAME;
            for (int i = 0; i < mNumPresets; i++) {
                param[1] = i;
                checkStatus(getParameter(param, value));
                int length = 0;
                while (value[length] != 0) length++;
                try {
                    mPresetNames[i] = new String(value, 0, length, "ISO-8859-1");
                } catch (java.io.UnsupportedEncodingException e) {
                    Log.e(TAG, "preset name decode error");
                }
            }
        }
    */
}

ECode CEqualizer::GetNumberOfBands(
    /* [out] */ Int16* numBands)
{
    if (mNumBands != 0) {
        *numBands = mNumBands;
    }
    ArrayOf_<Int32,1> param;
    param[0] = Equalizer_PARAM_NUM_BANDS;
    ArrayOf_<Int16,1> result;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx5(param, &result, &status));
    mNumBands = result[0];
    *numBands = mNumBands;
    return NOERROR;
}

ECode CEqualizer::GetBandLevelRange(
    /* [out] */ ArrayOf<Int16>* bandLevelRange)
{
    ArrayOf_<Int16,2> result;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(Equalizer_PARAM_LEVEL_RANGE, &result, &status));
    bandLevelRange = &result;
    return NOERROR;
}

ECode CEqualizer::SetBandLevel(
    /* [in] */ Int16 band,
    /* [in] */ Int16 level)
{
    ArrayOf_<Int32,2> param;
    ArrayOf_<Int16,1> value;

    param[0] = Equalizer_PARAM_BAND_LEVEL;
    param[1] = (Int32) band;
    value[0] = level;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx5(param, &value, &status));
    return NOERROR;
}

ECode CEqualizer::GetBandLevel(
    /* [in] */ Int16 band,
    /* [out] */ Int16* bandLevel)
{
    ArrayOf_<Int32,2> param;
    ArrayOf_<Int16,1> result;

    param[0] = Equalizer_PARAM_BAND_LEVEL;
    param[1] = (Int32) band;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx5(param, &result, &status));
    *bandLevel = result[0];
    return NOERROR;
}

ECode CEqualizer::GetCenterFreq(
    /* [in] */ Int16 band,
    /* [out] */ Int32* centerFreq)
{
    ArrayOf_<Int32,2> param;
    ArrayOf_<Int32,1> result;

    param[0] = Equalizer_PARAM_CENTER_FREQ;
    param[1] = (Int32) band;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx4(param, &result, &status));
    *centerFreq = result[0];
    return NOERROR;
}

ECode CEqualizer::GetBandFreqRange(
    /* [in] */ Int16 band,
    /* [out] */ ArrayOf<Int32>* bandFreqRange)
{
    ArrayOf_<Int32,2> param;
    ArrayOf_<Int32,2> result;

    param[0] = Equalizer_PARAM_BAND_FREQ_RANGE;
    param[1] = (Int32) band;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx4(param, &result, &status));
    bandFreqRange = &result;
    return NOERROR;
}

ECode CEqualizer::GetBand(
    /* [in] */ Int32 frequency,
    /* [out] */ Int16* band)
{
    ArrayOf_<Int32,2> param;
    ArrayOf_<Int16,1> result;

    param[0] = Equalizer_PARAM_GET_BAND;
    param[1] = frequency;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx5(param, &result, &status));
    *band = result[0];
    return NOERROR;
}

ECode CEqualizer::GetCurrentPreset(
        /* [out] */ Int16* preset)
{
    ArrayOf_<Int16,1> result;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(Equalizer_PARAM_CURRENT_PRESET, &result, &status));
    *preset = result[0];
    return NOERROR;
}

ECode CEqualizer::UsePreset(
    /* [in] */ Int16 preset)
{
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx2(Equalizer_PARAM_CURRENT_PRESET, preset, &status));
    return NOERROR;
}

ECode CEqualizer::GetNumberOfPresets(
    /* [out] */ Int16* numPresets)
{
    ArrayOf_<Int16,1> result;
    Int32 status;
    obj->CheckStatus(obj->GetParameterEx3(Equalizer_PARAM_GET_NUM_OF_PRESETS, &result, &status));
    *numPresets = result[0];
    return NOERROR;
}

ECode CEqualizer::GetPresetName(
    /* [in] */ Int16 preset,
    /* [out] */ String* presetName)
{
    if (preset >= 0 && preset < mNumPresets) {
        *presetName = (*mPresetNames)[preset];
    } else {
        *presetName = "";
    }
    return NOERROR;
}

ECode CEqualizer::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ const ArrayOf<Byte>& param,
    /* [in] */ const ArrayOf<Byte>& value)
{
    AutoPtr<IEqualizerOnParameterChangeListener> l;

    Mutex::Autolock lock(mHost->mParamListenerLock);
    if (mHost->mParamListener != NULL) {
        l = mHost->mParamListener;
    }
    if (l != NULL) {
        Int32 p1 = -1;
        Int32 p2 = -1;
        Int32 v = -1;

        if (param.GetLength() >= 4) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New(0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(param, 0, &p1);
            if (param.GetLength() >= 8) {
                obj->ByteArrayToInt32Ex(param, 4, &p2);
            }
            obj->Release();
        }
        if (value.GetLength() == 2) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New(0,0,(IAudioEffect**)&obj);
            Int16 status;
            obj->ByteArrayToInt16Ex(value, 0, &status);
            v = (Int32) status;
            obj->Release();
        } else if (value.GetLength() == 4) {
            AutoPtr<IAudioEffect> obj;
            CAudioEffect::New(0,0,(IAudioEffect**)&obj);
            obj->ByteArrayToInt32Ex(value, 0, &v);
            obj->Release();
        }
        if (p1 != -1 && v != -1) {
            l->OnParameterChange((IEqualizer*) this->Probe(EIID_IEqualizer), status, p1, p2, v);
        }
    }
    return NOERROR;
}

ECode CEqualizer::SetParameterListenerEx3(
    /* [in] */ IEqualizerOnParameterChangeListener* listener)
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

ECode CEqualizer::Settings::constructor(
    /* [in] */ String* settings)
{
    StringTokenizer st(*settings, String("=;"));
    Int32 tokens = st.GetCount();
    if (st.GetCount() < 5) {
//        throw new IllegalArgumentException("settings: " + settings);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String key = st.NextToken();
    if (!key.Equals("Equalizer")) {
//        throw new IllegalArgumentException("invalid settings for Equalizer: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
/*
    try {
*/
    key = st.NextToken();
    if (!key.Equals("curPreset")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    curPreset = st.NextToken().ToInt32();

    key = st.NextToken();
    if (!key.Equals("numBands")) {
//        throw new IllegalArgumentException("invalid key name: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    numBands = st.NextToken().ToInt32();
    if (st.NextToken().ToInt32() != numBands*2) {
//        throw new IllegalArgumentException("settings: " + settings);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
/*
            try {
                bandLevels = new short[numBands];
                for (int i = 0; i < numBands; i++) {
                    key = st.nextToken();
                    if (!key.equals("band"+(i+1)+"Level")) {
                        throw new IllegalArgumentException("invalid key name: " + key);
                    }
                    bandLevels[i] = Short.parseShort(st.nextToken());
                }
             } catch (NumberFormatException nfe) {
                throw new IllegalArgumentException("invalid value for key: " + key);
            }
*/
    return E_NOT_IMPLEMENTED;
}

ECode CEqualizer::Settings::toString(
    /* [out] */ String* result)
{
    *result = String("Equalizer") +
            String(";curPreset=") + String::FromInt32(curPreset) +
            String(";numBands=") + String::FromInt32(numBands);
    for (int i = 0; i < numBands; i++) {
        *result = String(";band") + String::FromInt32(i+1) + 
                    String("Level=") + String::FromInt32((*bandLevels)[i]);
    }
    return NOERROR;
}

ECode CEqualizer::Settings::GetParameterInt16(
    /* [in] */ String param,
    /* [out] */  Int16* result)
{
    if (param.Equals("curPreset")) {
        *result = curPreset;
    }

    if (param.Equals("numBands")) {
        *result = numBands;
    }
    return NOERROR;
}

ECode CEqualizer::Settings::SetParameterInt16(
    /* [in] */ String param,
    /* [in] */ Int16 result)
{
    if (param.Equals("curPreset")) {
        curPreset = result;
    }

    if (param.Equals("numBands")) {
        numBands = result;
    }
    return NOERROR;
}

ECode CEqualizer::Settings::GetParameterInt16Array(
    /* [in] */ String param,
    /* [out,callee] */  ArrayOf<Int16>* result)
{
    if (param.Equals("bandLevels")) {
        bandLevels = result;
    }
}

ECode CEqualizer::Settings::SetParameterInt16Array(
    /* [in] */ String param,
    /* [in] */ ArrayOf<Int16> result)
{
    if (param.Equals("bandLevels")) {
        bandLevels = &result;
    }
}

ECode CEqualizer::GetProperties(
    /* [out] */ IEqualizerSettings** properties)
{
/*
        byte[] param = new byte[4 + mNumBands * 2];
        checkStatus(getParameter(PARAM_PROPERTIES, param));
        Settings settings = new Settings();
        settings.curPreset = byteArrayToShort(param, 0);
        settings.numBands = byteArrayToShort(param, 2);
        settings.bandLevels = new short[mNumBands];
        for (int i = 0; i < mNumBands; i++) {
            settings.bandLevels[i] = byteArrayToShort(param, 4 + 2*i);
        }
        return settings;
*/
    return E_NOT_IMPLEMENTED;
}

ECode CEqualizer::SetProperties(
    /* [in] */ IEqualizerSettings* settings)
{
    Int16 tempInt16Parameter1,tempInt16Parameter2;
    ArrayOf<Byte>* tempByteArray1;
    ArrayOf<Byte>* tempByteArray2;
    ArrayOf<Byte>* tempResult1;
    ArrayOf<Int16>* tempInt16Array1;
    settings->GetParameterInt16(String("numBands"), &tempInt16Parameter1);
    settings->GetParameterInt16Array(String("bandLevels"), &tempInt16Array1);
    if (tempInt16Parameter1 != (*tempInt16Array1).GetLength() ||
        tempInt16Parameter1 != mNumBands) {
            //        throw new IllegalArgumentException("settings invalid band count: " +settings.numBands);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    settings->GetParameterInt16(String("curPreset"), &tempInt16Parameter1);
    obj->Int16ToByteArray(tempInt16Parameter1, tempByteArray1);
    obj->Int16ToByteArray(mNumBands, tempByteArray2);
    obj->ConcatArrays(*tempByteArray1, *tempByteArray2, &tempResult1);
    ArrayOf<Byte>* param = tempResult1;

    for (int i = 0; i < mNumBands; i++) {
        AutoPtr<IAudioEffect> obj;
        CAudioEffect::New(0,0,(IAudioEffect**)&obj);
        obj->Int16ToByteArray((*tempInt16Array1)[i], tempByteArray1);
        obj->ConcatArrays(*param, *tempByteArray1, &param);
        obj->Release();
    }
    Int32 status;
    obj->CheckStatus(obj->SetParameterEx3(Equalizer_PARAM_PROPERTIES, *param, &status));
    return NOERROR;
}
const CString CEqualizer::TAG = "Equalizer";
