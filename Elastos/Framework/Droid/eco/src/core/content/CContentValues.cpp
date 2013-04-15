
#include "content/CContentValues.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

#define PUT_VALUE(key, type, value) \
    do { \
        if ((key).IsNull()) { \
            return E_INVALID_ARGUMENT; \
        } \
        DataWrapper * p = new DataWrapper((type), (value)); \
        if (!p) { \
            return E_OUT_OF_MEMORY; \
        } \
        mValues[(key)] = p; \
    } while (0)

#define GET_DEFAULT_VALUE(key, defaultValue, type, member) \
    do { \
        HashMap<String, DataWrapper*>::Iterator it = mValues.Find((key)); \
        if (it == mValues.End()) { \
            *value = (defaultValue); \
            return NOERROR; \
        } \
        DataWrapper * data = it->mSecond; \
        if (data->mType != (type)) { \
            *value = (defaultValue); \
        } \
        else { \
            *value = data->mValue.member; \
        } \
    } while (0)

#define GET_DEFAULT_VALUE2(key, defaultValue, type, member) \
    do { \
        HashMap<String, DataWrapper*>::Iterator it = mValues.Find((key)); \
        if (it == mValues.End()) { \
            *value = (defaultValue); \
            return NOERROR; \
        } \
        DataWrapper * data = it->mSecond; \
        if (data->mType != (type)) { \
            *value = (defaultValue); \
        } \
        else { \
            *value = data->mValue.member; \
            if (*value) (*value)->AddRef(); \
        } \
    } while (0)

#define GET_DEFAULT_VALUE3(key, defaultValue, type, member) \
    do { \
        HashMap<String, DataWrapper*>::Iterator it = mValues.Find((key)); \
        if (it == mValues.End()) { \
            *value = (defaultValue); \
            return NOERROR; \
        } \
        DataWrapper * data = it->mSecond; \
        if (data->mType != (type)) { \
            *value = (defaultValue); \
        } \
        else { \
            *value = data->member; \
        } \
    } while (0)

#define GET_ARRAY(key, type, valueType, member) \
    do { \
        HashMap<String, DataWrapper*>::Iterator it = mValues.Find((key)); \
        if (it == mValues.End()) { \
            *value = NULL; \
            return NOERROR; \
        } \
        DataWrapper * data = it->mSecond; \
        if (data->mType != (type)) { \
            *value = NULL; \
        } \
        else { \
            ArrayOf<valueType>* array = \
                ArrayOf<valueType>::Alloc(data->mValue.member->GetLength()); \
            if (!array) { \
                *value = NULL; \
                return E_OUT_OF_MEMORY; \
            } \
            for (Int32 i = 0; i < data->mValue.member->GetLength(); ++i) { \
                (*array)[i] = (*data->mValue.member)[i]; \
            } \
            *value = array; \
        } \
    } while (0)

#define GET_ARRAY2(key, type, valueType, member) \
    do { \
        HashMap<String, DataWrapper*>::Iterator it = mValues.Find((key)); \
        if (it == mValues.End()) { \
            *value = NULL; \
            return NOERROR; \
        } \
        DataWrapper * data = it->mSecond; \
        if (data->mType != (type)) { \
            *value = NULL; \
        } \
        else { \
            ArrayOf<valueType>* array = \
                ArrayOf<valueType>::Alloc(data->mValue.member->GetLength()); \
            if (!array) { \
                *value = NULL; \
                return E_OUT_OF_MEMORY; \
            } \
            for (Int32 i = 0; i < data->mValue.member->GetLength(); ++i) { \
                (*array)[i] = (*data->mValue.member)[i]; \
                if ((*array)[i]) (*array)[i]->AddRef(); \
            } \
            *value = array; \
        } \
    } while (0)

class DataWrapper
{
public:
#define BOOLEAN_T (1)
#define INT32_T (2)
#define BYTE_T (3)
#define INT16_T (4)
#define INT64_T (5)
#define FLOAT_T (6)
#define DOUBLE_T (7)
#define STRING_T (8)
#define ARRAYOF_BYTE_T (9)
#define ARRAYLIST_STRING_T (10)

    ~DataWrapper()
    {
        switch (mType) {
            case ARRAYLIST_STRING_T:
                if (!mValue.mObjectContainer) {
                    break;
                }
                mValue.mObjectContainer->Release();
                break;
            case ARRAYOF_BYTE_T:
                if (!mValue.mArrayOfByte) {
                    break;
                }
                ArrayOf<Byte>::Free(mValue.mArrayOfByte);
                break;
            default:
                break;
        }
    }

    DataWrapper(Int32 type, Boolean value)
    {
        mType = type;
        if (type == BYTE_T) {
            mValue.mByte = value;
        }
        else {
            mValue.mBoolean = value;
        }
    }

    DataWrapper(Int32 type, Int32 value)
    {
        mType = type;
        mValue.mInt32 = value;
    }

    DataWrapper(Int32 type, Int16 value)
    {
        mType = type;
        mValue.mInt16 = value;
    }

    DataWrapper(Int32 type, Int64 value)
    {
        mType = type;
        mValue.mInt64 = value;
    }

    DataWrapper(Int32 type, Float value)
    {
        mType = type;
        mValue.mFloat = value;
    }

    DataWrapper(Int32 type, Double value)
    {
        mType = type;
        mValue.mDouble = value;
    }

    DataWrapper(Int32 type, const String& value)
    {
        mType = type;
        mString = value;
    }

    DataWrapper(Int32 type, IObjectContainer* value)
    {
        mType = type;
        mValue.mObjectContainer = value;
        if (value) {
            value->AddRef();
        }
    }


    DataWrapper(Int32 type, ArrayOf<Byte>* value)
    {
        mType = type;
        mValue.mArrayOfByte = value;
    }

public:
    Int32 mType;
    union {
        Boolean mBoolean;
        Int32 mInt32;
        Byte mByte;
        Int16 mInt16;
        Int64 mInt64;
        Float mFloat;
        Double mDouble;
        IObjectContainer* mObjectContainer;
        ArrayOf<Byte>* mArrayOfByte;
    } mValue;
    String mString;
};

CContentValues::CContentValues() :
        mValues(8)
{

}

CContentValues::~CContentValues(){
    mValues.Clear();
}

ECode CContentValues::constructor()
{
    return NOERROR;
}

ECode CContentValues::constructor(
    /* [in] */ IContentValues* values)
{
    return PutAll(values);
}

ECode CContentValues::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;

    IInterface* that;
    IContentValues* contentValues = IContentValues::Probe(object);

    if (contentValues != NULL)
    {
        AutoPtr<CContentValues> obj = (CContentValues*) contentValues;
        if (&mValues == &(obj->mValues))
        {
            *result = TRUE;
        }
    }

    return NOERROR;
}

ECode CContentValues::HashCode(
    /* [out] */ Int32* hashcode)
{
    VALIDATE_NOT_NULL(hashcode);
    return E_NOT_IMPLEMENTED;
}

ECode CContentValues::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    PUT_VALUE(key, INT32_T, value);

    return NOERROR;
}

ECode CContentValues::PutString(
	/* [in] */ const String& key,
	/* [in] */ const String& value) 
{
    PUT_VALUE(key, STRING_T, value);

    return NOERROR;
}

ECode CContentValues::PutAll(
    /* [in] */ IContentValues* contentvalues)
{
    VALIDATE_NOT_NULL(contentvalues);

    AutoPtr<CContentValues> obj = (CContentValues*) contentvalues;
    HashMap<String, DataWrapper*>::Iterator it;

    for (it = obj->mValues.Begin(); it != obj->mValues.End(); ++it)
    {
        mValues[it->mFirst] = it->mSecond;
    }

    return NOERROR;
}

ECode CContentValues::PutByte(
    /* [in] */ const String& key,
    /* [in] */ Byte value)
{
    PUT_VALUE(key, BYTE_T, value);

    return NOERROR;
}

ECode CContentValues::PutInt16(
    /* [in] */ const String& key,
    /* [in] */ Int16 value)
{
    PUT_VALUE(key, INT16_T, value);

    return NOERROR;
}

ECode CContentValues::PutInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 value)
{
    PUT_VALUE(key, INT64_T, value);

    return NOERROR;
}

ECode CContentValues::PutFloat(
    /* [in] */ const String& key,
    /* [in] */ Float value)
{
    PUT_VALUE(key, FLOAT_T, value);

    return NOERROR;
}

ECode CContentValues::PutDouble(
    /* [in] */ const String& key,
    /* [in] */ Double value)
{
    PUT_VALUE(key, DOUBLE_T, value);

    return NOERROR;
}

ECode CContentValues::PutBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{
    PUT_VALUE(key, BOOLEAN_T, value);

    return NOERROR;
}

ECode CContentValues::PutByteArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Byte>* value)
{
    PUT_VALUE(key, ARRAYOF_BYTE_T, value);

    return NOERROR;
}

ECode CContentValues::PutNull(
    /* [in] */ const String& key)
{
    PutInt32(key, 0);

    return NOERROR;
}

ECode CContentValues::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    *size = mValues.GetSize();

    return NOERROR;
}

ECode CContentValues::Remove(
    /* [in] */ const String& key)
{
    HashMap<String, DataWrapper*>::Iterator it = mValues.Find(key);

    if (it == mValues.End())
    {
        return E_DOES_NOT_EXIST;
    }

    mValues.Erase(it);

    return NOERROR;
}

ECode CContentValues::Clear()
{
    mValues.Clear();

    return NOERROR;
}

ECode CContentValues::ContainsKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    HashMap<String, DataWrapper*>::Iterator it = mValues.Find(key);

    *result = (it != mValues.End());

    return NOERROR;
}

ECode CContentValues::GetString(
            /* [in] */ DataWrapper* data,
            /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    if(data == NULL){
        return E_DOES_NOT_EXIST;
    }

    // Check the type and convert to String
    switch(data->mType){
    default:
        // Todo:deal with: byte, float, double, bytearray
        return E_DOES_NOT_EXIST;
        break;
    case STRING_T:
        *value = data->mString;
        break;
    case INT16_T:
        *value = String::FromInt32((Int32)data->mValue.mInt16);
        break;
    case INT32_T:
        *value = String::FromInt32(data->mValue.mInt32);
        break;
    case INT64_T:
        *value = String::FromInt64(data->mValue.mInt64);
        break;
    case BOOLEAN_T:
        *value = String::FromBoolean(data->mValue.mByte);
        break;
    }

    return NOERROR;
}

ECode CContentValues::GetString(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    HashMap<String, DataWrapper*>::Iterator it = mValues.Find(key);

    if (it == mValues.End())
    {
        return E_DOES_NOT_EXIST;
    }

    DataWrapper* data = it->mSecond;

    if(data != NULL){
        return GetString(data, value);
    }

    return E_DOES_NOT_EXIST;
}

ECode CContentValues::GetInt64(
    /* [in] */ const String& key,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0L, INT64_T, mInt64);

    return NOERROR;
}

ECode CContentValues::GetInt32(
    /* [in] */ const String& key,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0, INT32_T, mInt32);

    return NOERROR;
}

ECode CContentValues::GetInt16(
    /* [in] */ const String& key,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0, INT16_T, mInt16);

    return NOERROR;
}

ECode CContentValues::GetByte(
    /* [in] */ const String& key,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0, BYTE_T, mByte);

    return NOERROR;
}

ECode CContentValues::GetDouble(
    /* [in] */ const String& key,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0.0f, DOUBLE_T, mDouble);

    return NOERROR;
}

ECode CContentValues::GetFloat(
    /* [in] */ const String& key,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0.0f, FLOAT_T, mFloat);

    return NOERROR;
}

ECode CContentValues::GetBoolean(
    /* [in] */ const String& key,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, FALSE, BOOLEAN_T, mBoolean);

    return NOERROR;
}

ECode CContentValues::GetByteArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Byte> ** value)
{
    VALIDATE_NOT_NULL(value);

    GET_ARRAY(key, ARRAYOF_BYTE_T, Byte, mArrayOfByte);

    return NOERROR;
}

ECode CContentValues::ValueSet(
    /* [out] */ IObjectStringMap** values)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentValues::DescribeContents(
    /* [out] */ Int32* result)
{
    *result = 0;
    return NOERROR;
}

ECode CContentValues::ToString(
    /* [out] */ String* value)
{
    StringBuffer buffer;
    HashMap<String, DataWrapper*>::Iterator it;

    for (it = mValues.Begin(); it != mValues.End(); ++it)
    {
        if (buffer.GetLength() > 0)
        {
            buffer += " ";
        }

        buffer += it->mFirst;
        buffer += "=";

        String tempValue;

        if(NOERROR == GetString(it->mSecond, &tempValue)){
            buffer += tempValue;
        }
    }

    *value = String(buffer);

    return NOERROR;
}

ECode CContentValues::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 size;
    source->ReadInt32(&size);

    for(int i = 0; i < size; i++)
    {
        String key;
        Int32 type;

        source->ReadString(&key); // key
        source->ReadInt32(&type);

        if(key.GetLength() > 0){
            //read value
            switch(type){
                default:
                    // Todo:deal with: byte, float, double, bytearray
                    break;
                case STRING_T:
                {
                    String strValue;
                    source->ReadString(&strValue);
                    PutString(key, strValue);
                    break;
                }
                case INT16_T:
                {
                    Int16 shortValue;
                    source->ReadInt16(&shortValue);
                    PutInt16(key, shortValue);
                    break;

                }
                case INT32_T:
                {
                    Int32 intValue;
                    source->ReadInt32(&intValue);
                    PutInt32(key, intValue);
                    break;

                }
                case INT64_T:
                {
                    Int64 longValue;
                    source->ReadInt64(&longValue);
                    PutInt64(key, longValue);
                    break;
                }
                case BOOLEAN_T:
                {
                    Boolean boolValue;
                    source->ReadBoolean(&boolValue);
                    PutBoolean(key, boolValue);
                    break;
                }
            }
        }
    }

    return NOERROR;
}

ECode CContentValues::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mValues.GetSize());

    HashMap<String, DataWrapper*>::Iterator it;

    for (it = mValues.Begin(); it != mValues.End(); ++it)
    {
        DataWrapper* data = it->mSecond;

        if(data != NULL)
        {
            dest->WriteString(it->mFirst); // key
            dest->WriteInt32(data->mType); // value type

            //Write value
            switch(data->mType){
                default:
                    // Todo:deal with: byte, float, double, bytearray
                    break;
                case STRING_T:
                    dest->WriteString(data->mString);
                    break;
                case INT16_T:
                    dest->WriteInt16(data->mValue.mInt16);
                    break;
                case INT32_T:
                    dest->WriteInt32(data->mValue.mInt32);
                    break;
                case INT64_T:
                    dest->WriteInt64(data->mValue.mInt64);
                    break;
                case BOOLEAN_T:
                    dest->WriteBoolean(data->mValue.mByte);
                    break;
            }
        }
    }
    return NOERROR;
}
