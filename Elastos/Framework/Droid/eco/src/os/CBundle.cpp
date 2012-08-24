
#include "os/CBundle.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>

CBundle::CBundle() :
    mData(5)
{}

CBundle::~CBundle()
{
    mData.Clear();
}

ECode CBundle::PutBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{
    mData[key] = new DataWrapper(BOOLEAN_T, value);
    return NOERROR;
}

ECode CBundle::GetBoolean(
    /* [in] */ const String& key,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    DataWrapper* data = mData[key];
    if ((data == NULL) || (data->mType != BOOLEAN_T)) return E_DOES_NOT_EXIST;

    *value = data->mValue.mBoolean;
    return NOERROR;
}

ECode CBundle::PutByte(
    /* [in] */ const String& key,
    /* [in] */ Byte value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetByte(
    /* [in] */ const String& key,
    /* [out] */ Byte* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutChar(
    /* [in] */ const String& key,
    /* [in] */ Char16 value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetChar(
    /* [in] */ const String& key,
    /* [out] */ Char16* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutInt16(
    /* [in] */ const String& key,
    /* [in] */ Int16 value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetInt16(
    /* [in] */ const String& key,
    /* [out] */ Int16* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    mData[key] = new DataWrapper(INT32_T, value);
    return NOERROR;
}

ECode CBundle::GetInt32(
    /* [in] */ const String& key,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    DataWrapper *data = mData[key];
    if ((data == NULL) || (data->mType != INT32_T)) return E_DOES_NOT_EXIST;

    *value = data->mValue.mInt32;
    return NOERROR;
}

ECode CBundle::PutInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetInt64(
    /* [in] */ const String& key,
    /* [out] */ Int64* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutFloat(
    /* [in] */ const String& key,
    /* [in] */ Float value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetFloat(
    /* [in] */ const String& key,
    /* [out] */ Float* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutDouble(
    /* [in] */ const String& key,
    /* [in] */ Double value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetDouble(
    /* [in] */ const String& key,
    /* [out] */ Double* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutString(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetString(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutStringArray(
    /* [in] */ const String& key,
    /* [in] */ const ArrayOf<String>& value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetStringArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>** value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutInt32Array(
    /* [in] */ const String& key,
    /* [in] */ const ArrayOf<Int32>& value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetInt32Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int32>** value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::HasFileDescriptors(
    /* [out] */ Boolean* hasFD)
{
    //TODO: temporary implementation
    VALIDATE_NOT_NULL(hasFD);

    *hasFD = FALSE;

    return NOERROR;
}

ECode CBundle::PutBundle(
    /* [in] */ const String& key,
    /* [in] */ IBundle* value)
{
//    Unparcel();
    mData[key] = new DataWrapper(IBUNDLE_T, value);
    return NOERROR;
}

ECode CBundle::GetBundle(
    /* [in] */ const String& key,
    /* [out] */ IBundle** value)
{
    VALIDATE_NOT_NULL(value);

//    Unparcel();
    DataWrapper *data = mData[key];
    if ((data == NULL) || (data->mType != IBUNDLE_T)) return E_DOES_NOT_EXIST;

    *value = data->mValue.mBundle;
    if (*value != NULL) (*value)->AddRef();
    return NOERROR;
}

ECode CBundle::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    Int32 size;
    source->ReadInt32(&size);
    if (size > 0) {
        ReadFromParcelInner(source, size);
    }

    return NOERROR;
}

ECode CBundle::ReadFromParcelInner(
    /* [in] */ IParcel *source,
    /* [in] */ Int32 length)
{
    String key;
    DataWrapper *data;
    for (Int32 i = 0; i < length; i++) {
        source->ReadString(&key);
        source->ReadStructPtr((Handle32 *)&data);

        if (data->mType == INT32_T) {
            mData[key] = new DataWrapper(
                data->mType, data->mValue.mInt32);
        }
        else if (data->mType == BOOLEAN_T){
            mData[key] = new DataWrapper(
                data->mType, data->mValue.mBoolean);
        }
        else if (data->mType == IBUNDLE_T){
            mData[key] = new DataWrapper(
                data->mType, data->mValue.mBundle);
        }
    }

    return NOERROR;
}

ECode CBundle::ReadFromParcel(
    /* [in] */ IParcel *source,
    /* [in] */ Int32 length,
    /* [out] */ IBundle** bundle)
{
    if (bundle == NULL) return E_INVALID_ARGUMENT;

    return CBundle::New(source, length, bundle);
}

ECode CBundle::WriteToParcel(
    /* [in] */ IBundle *bundle,
    /* [in] */ IParcel *dest)
{
    if (bundle != NULL) {
        ((CBundle *)bundle)->WriteToParcel(dest);
    }

    return NOERROR;
}

ECode CBundle::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    HashMap<String, DataWrapper*>::SizeType
    size = mData.GetSize();
    dest->WriteInt32(size);

    HashMap<String, DataWrapper*>::Iterator it = mData.Begin();
    for (; it != mData.End(); ++it) {
        dest->WriteString(it->mFirst);
        dest->WriteStruct((Handle32)(it->mSecond), sizeof(DataWrapper));
    }

    return NOERROR;
}

ECode CBundle::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::constructor()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CBundle::constructor(
    /* [in] */ IBundle * bundle)
{
    AutoPtr<CBundle> o = (CBundle*)bundle;

    if (o->mData.GetSize() > 0) {
        DataWrapper* pData;
        HashMap<String, DataWrapper*>::Iterator it = o->mData.Begin();
        for (; it != mData.End(); ++it) {
            pData = it->mSecond;
            if (pData->mType == INT32_T) {
                mData[it->mFirst] = new DataWrapper(
                    pData->mType, pData->mValue.mInt32);
            }
            else if (pData->mType == BOOLEAN_T){
                mData[it->mFirst] = new DataWrapper(
                    pData->mType, pData->mValue.mBoolean);
            }
            else if (pData->mType == IBUNDLE_T){
                mData[it->mFirst] = new DataWrapper(
                    pData->mType, pData->mValue.mBundle);
            }
        }
    }

    return NOERROR;
}

ECode CBundle::constructor(
    /* [in] */ IParcel * parcelledData)
{
    return ReadFromParcel(parcelledData);
}

ECode CBundle::constructor(
    /* [in] */ IParcel * parcelledData,
    /* [in] */ Int32 length)
{
    return ReadFromParcelInner(parcelledData, length);
}

ECode CBundle::constructor(
    /* [in] */ Int32 capacity)
{
    return E_NOT_IMPLEMENTED;
}

