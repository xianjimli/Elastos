
#include "content/CContentValues.h"
#include <elastos/AutoPtr.h>

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
#define IBUNDLE_T (3)
#define BYTE_T (4)
#define CHAR_T (5)
#define INT16_T (6)
#define INT64_T (7)
#define FLOAT_T (8)
#define DOUBLE_T (9)
#define STRING_T (10)
#define PARCELABLE_T (11)
#define CHARSEQUENCE_T (12)
#define ARRAYOF_BOOLEAN_T (13)
#define ARRAYOF_INT32_T (14)
#define ARRAYOF_IBUNDLE_T (15)
#define ARRAYOF_BYTE_T (16)
#define ARRAYOF_CHAR_T (17)
#define ARRAYOF_INT16_T (18)
#define ARRAYOF_INT64_T (19)
#define ARRAYOF_FLOAT_T (20)
#define ARRAYOF_DOUBLE_T (21)
#define ARRAYOF_STRING_T (22)
#define ARRAYOF_PARCELABLE_T (23)
#define ARRAYOF_CHARSEQUENCE_T (24)
#define ARRAYLIST_PARCELABLE_T (25)
#define ARRAYLIST_INTEGER_T (26)
#define ARRAYLIST_STRING_T (27)
#define ARRAYLIST_CHARSEQUENCE_T (28)

    ~DataWrapper()
    {
        switch (mType) {
            case IBUNDLE_T:
                if (!mValue.mBundle) {
                    break;
                }
                mValue.mBundle->Release();
                break;
            case PARCELABLE_T:
                if (!mValue.mParcelable) {
                    break;
                }
                mValue.mParcelable->Release();
                break;
            case CHARSEQUENCE_T:
                if (!mValue.mCharSequence) {
                    break;
                }
                mValue.mCharSequence->Release();
                break;
            case ARRAYLIST_PARCELABLE_T:
            case ARRAYLIST_INTEGER_T:
            case ARRAYLIST_STRING_T:
            case ARRAYLIST_CHARSEQUENCE_T:
                if (!mValue.mObjectContainer) {
                    break;
                }
                mValue.mObjectContainer->Release();
                break;
            case ARRAYOF_BOOLEAN_T:
                if (!mValue.mArrayOfBoolean) {
                    break;
                }
                ArrayOf<Boolean>::Free(mValue.mArrayOfBoolean);
                break;
            case ARRAYOF_INT32_T:
                if (!mValue.mArrayOfInt32) {
                    break;
                }
                ArrayOf<Int32>::Free(mValue.mArrayOfInt32);
                break;
            case ARRAYOF_IBUNDLE_T:
                if (!mValue.mArrayOfBundle) {
                    break;
                }
                for (Int32 i = 0; i < mValue.mArrayOfBundle->GetLength(); ++i) {
                    if ((*mValue.mArrayOfBundle)[i]) {
                        (*mValue.mArrayOfBundle)[i]->Release();
                    }
                }
                ArrayOf<IBundle*>::Free(mValue.mArrayOfBundle);
                break;
            case ARRAYOF_BYTE_T:
                if (!mValue.mArrayOfByte) {
                    break;
                }
                ArrayOf<Byte>::Free(mValue.mArrayOfByte);
                break;
            case ARRAYOF_CHAR_T:
                if (!mValue.mArrayOfChar) {
                    break;
                }
                ArrayOf<Char32>::Free(mValue.mArrayOfChar);
                break;
            case ARRAYOF_INT16_T:
                if (!mValue.mArrayOfInt16) {
                    break;
                }
                ArrayOf<Int16>::Free(mValue.mArrayOfInt16);
                break;
            case ARRAYOF_INT64_T:
                if (!mValue.mArrayOfInt64) {
                    break;
                }
                ArrayOf<Int64>::Free(mValue.mArrayOfInt64);
                break;
            case ARRAYOF_FLOAT_T:
                if (!mValue.mArrayOfFloat) {
                    break;
                }
                ArrayOf<Float>::Free(mValue.mArrayOfFloat);
                break;
            case ARRAYOF_DOUBLE_T:
                if (!mValue.mArrayOfDouble) {
                    break;
                }
                ArrayOf<Double>::Free(mValue.mArrayOfDouble);
                break;
            case ARRAYOF_STRING_T:
                if (!mValue.mArrayOfString) {
                    break;
                }
                ArrayOf<String>::Free(mValue.mArrayOfString);
                break;
            case ARRAYOF_PARCELABLE_T:
                if (!mValue.mArrayOfParcelable) {
                    break;
                }
                for (Int32 i = 0; i < mValue.mArrayOfParcelable->GetLength(); ++i) {
                    if ((*mValue.mArrayOfParcelable)[i]) {
                        (*mValue.mArrayOfParcelable)[i]->Release();
                    }
                }
                ArrayOf<IParcelable*>::Free(mValue.mArrayOfParcelable);
                break;
            case ARRAYOF_CHARSEQUENCE_T:
                if (!mValue.mArrayOfCharSequence) {
                    break;
                }
                for (Int32 i = 0; i < mValue.mArrayOfCharSequence->GetLength(); ++i) {
                    if ((*mValue.mArrayOfCharSequence)[i]) {
                        (*mValue.mArrayOfCharSequence)[i]->Release();
                    }
                }
                ArrayOf<ICharSequence*>::Free(mValue.mArrayOfCharSequence);
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

    DataWrapper(Int32 type, IBundle* value)
    {
        mType = type;
        mValue.mBundle = value;
        if (value) {
            value->AddRef();
        }
    }

    DataWrapper(Int32 type, Char32 value)
    {
        mType = type;
        mValue.mChar = value;
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

    DataWrapper(Int32 type, IParcelable* value)
    {
        mType = type;
        mValue.mParcelable = value;
        if (value) {
            value->AddRef();
        }
    }

    DataWrapper(Int32 type, ICharSequence* value)
    {
        mType = type;
        mValue.mCharSequence = value;
        if (value) {
            value->AddRef();
        }
    }

    DataWrapper(Int32 type, IObjectContainer* value)
    {
        mType = type;
        mValue.mObjectContainer = value;
        if (value) {
            value->AddRef();
        }
    }

    DataWrapper(Int32 type, ArrayOf<Boolean>* value)
    {
        mType = type;
        if (type == ARRAYOF_BYTE_T) {
            mValue.mArrayOfByte = value;
        }
        else {
            mValue.mArrayOfBoolean = value;
        }
    }

    DataWrapper(Int32 type, ArrayOf<Int32>* value)
    {
        mType = type;
        mValue.mArrayOfInt32 = value;
    }

    DataWrapper(Int32 type, ArrayOf<IBundle*>* value)
    {
        mType = type;
        mValue.mArrayOfBundle = value;
    }

    DataWrapper(Int32 type, ArrayOf<Char32>* value)
    {
        mType = type;
        mValue.mArrayOfChar = value;
    }

    DataWrapper(Int32 type, ArrayOf<Int16>* value)
    {
        mType = type;
        mValue.mArrayOfInt16 = value;
    }

    DataWrapper(Int32 type, ArrayOf<Int64>* value)
    {
        mType = type;
        mValue.mArrayOfInt64 = value;
    }

    DataWrapper(Int32 type, ArrayOf<Float>* value)
    {
        mType = type;
        mValue.mArrayOfFloat = value;
    }

    DataWrapper(Int32 type, ArrayOf<Double>* value)
    {
        mType = type;
        mValue.mArrayOfDouble = value;
    }

    DataWrapper(Int32 type, ArrayOf<String>* value)
    {
        mType = type;
        mValue.mArrayOfString = value;
    }

    DataWrapper(Int32 type, ArrayOf<IParcelable*>* value)
    {
        mType = type;
        mValue.mArrayOfParcelable = value;
    }

    DataWrapper(Int32 type, ArrayOf<ICharSequence*>* value)
    {
        mType = type;
        mValue.mArrayOfCharSequence = value;
    }

public:
    Int32 mType;
    union {
        Boolean mBoolean;
        Int32 mInt32;
        IBundle* mBundle;
        Byte mByte;
        Char32 mChar;
        Int16 mInt16;
        Int64 mInt64;
        Float mFloat;
        Double mDouble;
        IParcelable* mParcelable;
        ICharSequence* mCharSequence;
        IObjectContainer* mObjectContainer;
        ArrayOf<Boolean>* mArrayOfBoolean;
        ArrayOf<Int32>* mArrayOfInt32;
        ArrayOf<IBundle*>* mArrayOfBundle;
        ArrayOf<Byte>* mArrayOfByte;
        ArrayOf<Char32>* mArrayOfChar;
        ArrayOf<Int16>* mArrayOfInt16;
        ArrayOf<Int64>* mArrayOfInt64;
        ArrayOf<Float>* mArrayOfFloat;
        ArrayOf<Double>* mArrayOfDouble;
        ArrayOf<String>* mArrayOfString;
        ArrayOf<IParcelable*>* mArrayOfParcelable;
        ArrayOf<ICharSequence*>* mArrayOfCharSequence;
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
    /* [in] */ ArrayOf<Byte> * value)
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
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);

    HashMap<String, DataWrapper*>::Iterator it = mValues.Find(key);

    *result = (it != mValues.End());

    return NOERROR;
}

ECode CContentValues::GetString(
    /* [in] */ const String& key,
    /* [out] */ String * value)
{
    VALIDATE_NOT_NULL(value);

    HashMap<String, DataWrapper*>::Iterator it = mValues.Find(key);

    if (it == mValues.End())
    {
        return E_DOES_NOT_EXIST;
    }

    DataWrapper * data = it->mSecond;

    if (data->mType != (STRING_T))
    {
        // Todo: check the type and convert to String
        return E_DOES_NOT_EXIST;
    }
    else
    {
        *value = data->mString;
    }

    return NOERROR;
}

ECode CContentValues::GetInt64(
    /* [in] */ const String& key,
    /* [out] */ Int64 * value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0L, INT64_T, mInt64);
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
    /* [out] */ Int16 * value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0, INT16_T, mInt16);
}

ECode CContentValues::GetByte(
    /* [in] */ const String& key,
    /* [out] */ Byte * value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0, BYTE_T, mByte);
}

ECode CContentValues::GetDouble(
    /* [in] */ const String& key,
    /* [out] */ Double * value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0.0f, DOUBLE_T, mDouble);
}

ECode CContentValues::GetFloat(
    /* [in] */ const String& key,
    /* [out] */ Float * value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, 0.0f, FLOAT_T, mFloat);
}

ECode CContentValues::GetBoolean(
    /* [in] */ const String& key,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);

    GET_DEFAULT_VALUE(key, FALSE, BOOLEAN_T, mBoolean);
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
