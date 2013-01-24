
#include "os/CBundle.h"
#include "ext/frameworkdef.h"
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
        mData[(key)] = p; \
    } while (0)

#define GET_DEFAULT_VALUE(key, defaultValue, type, member) \
    do { \
        HashMap<String, DataWrapper*>::Iterator it = mData.Find((key)); \
        if (it == mData.End()) { \
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
        HashMap<String, DataWrapper*>::Iterator it = mData.Find((key)); \
        if (it == mData.End()) { \
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
        HashMap<String, DataWrapper*>::Iterator it = mData.Find((key)); \
        if (it == mData.End()) { \
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
        HashMap<String, DataWrapper*>::Iterator it = mData.Find((key)); \
        if (it == mData.End()) { \
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
        HashMap<String, DataWrapper*>::Iterator it = mData.Find((key)); \
        if (it == mData.End()) { \
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

CBundle::CBundle() :
    mData(5),
    mHasFds(FALSE),
    mFdsKnown(TRUE)
{
}

CBundle::~CBundle()
{
    mData.Clear();
}

ECode CBundle::GetPairValue(
    /* [out] */ String* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::SetClassLoader(
    /* [in] */ IClassLoader* loader)
{
    return NOERROR;
}

ECode CBundle::Clone(
    /* [out] */ IInterface** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::Size(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // unparcel();
    *result = mData.GetSize();

    return NOERROR;
}

ECode CBundle::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // unparcel();
    *result = mData.IsEmpty();
    mHasFds = FALSE;
    mFdsKnown = TRUE;

    return NOERROR;
}

ECode CBundle::Clear()
{
    // unparcel();
    mData.Clear();

    return NOERROR;
}

ECode CBundle::ContainsKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // unparcel();
    HashMap<String, DataWrapper*>::Iterator it = mData.Find(key);
    *result = (it != mData.End());

    return NOERROR;
}

ECode CBundle::Get(
    /* [in] */ const String& key,
    /* [out] */ IInterface** value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::Remove(
    /* [in] */ const String& key)
{
    // unparcel();
    HashMap<String, DataWrapper*>::Iterator it = mData.Find(key);
    if (it == mData.End()) {
        return E_DOES_NOT_EXIST;
    }

    mData.Erase(it);

    return NOERROR;
}

ECode CBundle::PutAll(
    /* [in] */ IBundle* map)
{
    VALIDATE_NOT_NULL(map);

//    unparcel();
//    map.unparcel();
    AutoPtr<CBundle> obj = (CBundle*)map;
    HashMap<String, DataWrapper*>::Iterator it;
    for (it = obj->mData.Begin(); it != obj->mData.End(); ++it) {
        mData[it->mFirst] = it->mSecond;
    }

    // fd state is now known if and only if both bundles already knew
//    mHasFds |= map.mHasFds;
//    mFdsKnown = mFdsKnown && map.mFdsKnown;
    return NOERROR;
}

ECode CBundle::KeySet(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    // unparcel();
    FAIL_RETURN(CObjectContainer::New(result));

    HashMap<String, DataWrapper*>::Iterator it;
    for (it = mData.Begin(); it != mData.End(); ++it) {
        AutoPtr<ICharSequence> str;
        if (FAILED(CStringWrapper::New(it->mFirst, (ICharSequence**)&str))) {
            (*result)->Release();
            *result = NULL;
            return E_OUT_OF_MEMORY;
        }

        (*result)->Add((ICharSequence*)str);
    }

    return NOERROR;
}

ECode CBundle::HasFileDescriptors(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{
    // unparcel();
    PUT_VALUE(key, BOOLEAN_T, value);

    return NOERROR;
}

ECode CBundle::PutByte(
    /* [in] */ const String& key,
    /* [in] */ Byte value)
{
    // unparcel();
    PUT_VALUE(key, BYTE_T, value);

    return NOERROR;
}

ECode CBundle::PutChar(
    /* [in] */ const String& key,
    /* [in] */ Char32 value)
{
    // unparcel();
    PUT_VALUE(key, CHAR_T, value);

    return NOERROR;
}

ECode CBundle::PutInt16(
    /* [in] */ const String& key,
    /* [in] */ Int16 value)
{
    // unparcel();
    PUT_VALUE(key, INT16_T, value);

    return NOERROR;
}

ECode CBundle::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    // unparcel();
    PUT_VALUE(key, INT32_T, value);

    return NOERROR;
}

ECode CBundle::PutInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 value)
{
    // unparcel();
    PUT_VALUE(key, INT64_T, value);

    return NOERROR;
}

ECode CBundle::PutFloat(
    /* [in] */ const String& key,
    /* [in] */ Float value)
{
    // unparcel();
    PUT_VALUE(key, FLOAT_T, value);

    return NOERROR;
}

ECode CBundle::PutDouble(
    /* [in] */ const String& key,
    /* [in] */ Double value)
{
    // unparcel();
    PUT_VALUE(key, DOUBLE_T, value);

    return NOERROR;
}

ECode CBundle::PutString(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    // unparcel();
    PUT_VALUE(key, STRING_T, value);

    return NOERROR;
}

ECode CBundle::PutCharSequence(
    /* [in] */ const String& key,
    /* [in] */ ICharSequence* value)
{
    // unparcel();
    PUT_VALUE(key, CHARSEQUENCE_T, value);

    return NOERROR;
}

ECode CBundle::PutParcelable(
    /* [in] */ const String& key,
    /* [in] */ IParcelable* value)
{
    // unparcel();
    PUT_VALUE(key, PARCELABLE_T, value);
    mFdsKnown = FALSE;

    return NOERROR;
}

ECode CBundle::PutParcelableArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<IParcelable*>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_PARCELABLE_T, value);
    mFdsKnown = FALSE;

    return NOERROR;
}

ECode CBundle::PutParcelableArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYLIST_PARCELABLE_T, value);
    mFdsKnown = FALSE;

    return NOERROR;
}

ECode CBundle::PutIntegerArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYLIST_INTEGER_T, value);

    return NOERROR;
}

ECode CBundle::PutStringArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYLIST_STRING_T, value);

    return NOERROR;
}

ECode CBundle::PutCharSequenceArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYLIST_CHARSEQUENCE_T, value);

    return NOERROR;
}

// PutSerializable(
//     /* [in] */ const String& key,
//     /* [in] */ ISerializable* value);

ECode CBundle::PutBooleanArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Boolean>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_BOOLEAN_T, value);

    return NOERROR;
}

ECode CBundle::PutByteArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Byte>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_BYTE_T, value);

    return NOERROR;
}

ECode CBundle::PutInt16Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int16>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_INT16_T, value);

    return NOERROR;
}

ECode CBundle::PutCharArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Char32>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_CHAR_T, value);

    return NOERROR;
}

ECode CBundle::PutInt32Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int32>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_INT32_T, value);

    return NOERROR;
}

ECode CBundle::PutInt64Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int64>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_INT64_T, value);

    return NOERROR;
}

ECode CBundle::PutFloatArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Float>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_FLOAT_T, value);

    return NOERROR;
}

ECode CBundle::PutDoubleArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Double>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_DOUBLE_T, value);

    return NOERROR;
}

ECode CBundle::PutStringArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<String>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_STRING_T, value);

    return NOERROR;
}

ECode CBundle::PutCharSequenceArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<ICharSequence*>* value)
{
    // unparcel();
    PUT_VALUE(key, ARRAYOF_CHARSEQUENCE_T, value);

    return NOERROR;
}

ECode CBundle::PutBundle(
    /* [in] */ const String& key,
    /* [in] */ IBundle* value)
{
    // unparcel();
    PUT_VALUE(key, IBUNDLE_T, value);

    return NOERROR;
}

ECode CBundle::PutIBinder(
    /* [in] */ const String& key,
    /* [in] */ IBinder* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::GetBoolean(
    /* [in] */ const String& key,
    /* [out] */ Boolean* value)
{
    // unparcel();
    return GetBooleanEx(key, FALSE, value);
}

ECode CBundle::GetBooleanEx(
    /* [in] */ const String& key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE(key, defaultValue, BOOLEAN_T, mBoolean);

    return NOERROR;
}

ECode CBundle::GetByte(
    /* [in] */ const String& key,
    /* [out] */ Byte* value)
{
    // unparcel();
    return GetByteEx(key, 0, value);
}

ECode CBundle::GetByteEx(
    /* [in] */ const String& key,
    /* [in] */ Byte defaultValue,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE(key, defaultValue, BYTE_T, mByte);

    return NOERROR;
}

ECode CBundle::GetChar(
    /* [in] */ const String& key,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    return GetCharEx(key, 0, value);
}

ECode CBundle::GetCharEx(
    /* [in] */ const String& key,
    /* [in] */ Char32 defaultValue,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE(key, defaultValue, CHAR_T, mChar);

    return NOERROR;
}

ECode CBundle::GetInt16(
    /* [in] */ const String& key,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    return GetInt16Ex(key, 0, value);
}

ECode CBundle::GetInt16Ex(
    /* [in] */ const String& key,
    /* [in] */ Int16 defaultValue,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE(key, defaultValue, INT16_T, mInt16);

    return NOERROR;
}

ECode CBundle::GetInt32(
    /* [in] */ const String& key,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    return GetInt32Ex(key, 0, value);
}

ECode CBundle::GetInt32Ex(
    /* [in] */ const String& key,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE(key, defaultValue, INT32_T, mInt32);

    return NOERROR;
}

ECode CBundle::GetInt64(
    /* [in] */ const String& key,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    return GetInt64Ex(key, 0L, value);
}

ECode CBundle::GetInt64Ex(
    /* [in] */ const String& key,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE(key, defaultValue, INT64_T, mInt64);

    return NOERROR;
}

ECode CBundle::GetFloat(
    /* [in] */ const String& key,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    return GetFloatEx(key, 0.0f, value);
}

ECode CBundle::GetFloatEx(
    /* [in] */ const String& key,
    /* [in] */ Float defaultValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE(key, defaultValue, FLOAT_T, mFloat);

    return NOERROR;
}

ECode CBundle::GetDouble(
    /* [in] */ const String& key,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    return GetDoubleEx(key, 0.0f, value);
}

ECode CBundle::GetDoubleEx(
    /* [in] */ const String& key,
    /* [in] */ Double defaultValue,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE(key, defaultValue, DOUBLE_T, mDouble);

    return NOERROR;
}

ECode CBundle::GetString(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE3(key, NULL, STRING_T, mString);

    return NOERROR;
}

ECode CBundle::GetCharSequence(
    /* [in] */ const String& key,
    /* [out] */ ICharSequence** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE2(key, NULL, CHARSEQUENCE_T, mCharSequence);

    return NOERROR;
}

ECode CBundle::GetBundle(
    /* [in] */ const String& key,
    /* [out] */ IBundle** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE2(key, NULL, IBUNDLE_T, mBundle);

    return NOERROR;
}

ECode CBundle::GetParcelable(
    /* [in] */ const String& key,
    /* [out] */ IParcelable** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE2(key, NULL, PARCELABLE_T, mParcelable);

    return NOERROR;
}

ECode CBundle::GetParcelableArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<IParcelable*>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY2(key, ARRAYOF_PARCELABLE_T, IParcelable*, mArrayOfParcelable);

    return NOERROR;
}

ECode CBundle::GetParcelableArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE2(key, NULL, ARRAYLIST_PARCELABLE_T, mObjectContainer);

    return NOERROR;
}

// public <T extends Parcelable> SparseArray<T> getSparseParcelableArray(const String& key);

// GetSerializable(
//     /* [in] */ const String& key,
//     /* [out] */ ISerializable** result);

ECode CBundle::GetIntegerArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE2(key, NULL, ARRAYLIST_INTEGER_T, mObjectContainer);

    return NOERROR;
}

ECode CBundle::GetStringArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE2(key, NULL, ARRAYLIST_STRING_T, mObjectContainer);

    return NOERROR;
}

ECode CBundle::GetCharSequenceArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_DEFAULT_VALUE2(key, NULL, ARRAYLIST_CHARSEQUENCE_T, mObjectContainer);

    return NOERROR;
}

ECode CBundle::GetBooleanArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Boolean>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_BOOLEAN_T, Boolean, mArrayOfBoolean);

    return NOERROR;
}

ECode CBundle::GetByteArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_BYTE_T, Byte, mArrayOfByte);

    return NOERROR;
}

ECode CBundle::GetInt16Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int16>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_INT16_T, Int16, mArrayOfInt16);

    return NOERROR;
}

ECode CBundle::GetCharArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Char32>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_CHAR_T, Char32, mArrayOfChar);

    return NOERROR;
}

ECode CBundle::GetInt32Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int32>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_INT32_T, Int32, mArrayOfInt32);

    return NOERROR;
}

ECode CBundle::GetInt64Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int64>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_INT64_T, Int64, mArrayOfInt64);

    return NOERROR;
}

ECode CBundle::GetFloatArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Float>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_FLOAT_T, Float, mArrayOfFloat);

    return NOERROR;
}

ECode CBundle::GetDoubleArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Double>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_DOUBLE_T, Double, mArrayOfDouble);

    return NOERROR;
}

ECode CBundle::GetStringArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY(key, ARRAYOF_STRING_T, String, mArrayOfString);

    return NOERROR;
}

ECode CBundle::GetCharSequenceArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<ICharSequence*>** value)
{
    VALIDATE_NOT_NULL(value);
    // unparcel();
    GET_ARRAY2(key, ARRAYOF_CHARSEQUENCE_T, ICharSequence*, mArrayOfCharSequence);

    return NOERROR;
}

ECode CBundle::GetIBinder(
    /* [in] */ const String& key,
    /* [out] */ IBinder** value)
{
    return E_NOT_IMPLEMENTED;
}

// @Override
// public synchronized const String& toString();

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
    // TODO: ALEX
    // mClassLoader = getClass().getClassLoader();

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

