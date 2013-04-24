
#include <binder/Parcel.h>
#include "os/CBundle.h"
#include "utils/CParcelableStringWrapper.h"
#include <StringBuffer.h>

using namespace Elastos::Core;

static AutoPtr<CBundle> InitEMPTY()
{
    AutoPtr<CBundle> empty;
    CBundle::NewByFriend((CBundle**)&empty);
    return empty;
}

const CString CBundle::TAG = "CBundle";
static AutoPtr<CBundle> EMPTY = InitEMPTY();

CBundle::CBundle()
    : mMap(NULL)
    , mParcelledData(0)
    , mHasFds(FALSE)
    , mFdsKnown(TRUE)
{}

CBundle::~CBundle()
{
    if (mParcelledData != 0) {
        delete (android::Parcel*)mParcelledData;
    }

    if (mMap != NULL) {
        mMap->Clear();
        delete mMap;
    }
}

ECode CBundle::constructor()
{
    mMap = new HashMap<String, AutoPtr<IInterface> >(5);
    // mClassLoader = getClass().getClassLoader();
    return NOERROR;
}

ECode CBundle::constructor(
    /* [in] */ IParcel* parcelledData)
{
    return ReadFromParcel(parcelledData);
}

ECode CBundle::constructor(
    /* [in] */ IParcel* parcelledData,
    /* [in] */ Int32 length)
{
    return ReadFromParcelInner(parcelledData, length);
}

ECode CBundle::constructor(
    /* [in] */ Int32 capacity)
{
    mMap = new HashMap<String, AutoPtr<IInterface> >(capacity);
    // mClassLoader = getClass().getClassLoader();
    return NOERROR;
}

ECode CBundle::constructor(
    /* [in] */ IBundle* bundle)
{
    CBundle* b = (CBundle*)bundle;
    if (b->mParcelledData != 0) {
        android::Parcel* p = new android::Parcel();
        p->appendFrom((android::Parcel*)b->mParcelledData, 0,
            ((android::Parcel*)b->mParcelledData)->dataSize());
        p->setDataPosition(0);
        mParcelledData = (Handle32)p;
    }
    else {
        mParcelledData = 0;
    }

    if (b->mMap != NULL) {
        mMap = new HashMap<String, AutoPtr<IInterface> >(b->mMap->Begin(), b->mMap->End());
    }
    else {
        mMap = NULL;
    }

    mHasFds = b->mHasFds;
    mFdsKnown = b->mFdsKnown;
    mClassLoader = b->mClassLoader;
    return NOERROR;
}

ECode CBundle::GetPairValue(
    /* [out] */ String* result)
{
    // unparcel();
    // int size = mMap->size();
    // if (size > 1) {
    //     Log.w(LOG_TAG, "getPairValue() used on Bundle with multiple pairs.");
    // }
    // if (size == 0) {
    //     return null;
    // }
    // Object o = mMap->values().iterator().next();
    // try {
    //     return (String) o;
    // } catch (ClassCastException e) {
    //     typeWarning("getPairValue()", o, "String", e);
    //     return null;
    // }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::SetClassLoader(
    /* [in] */ IClassLoader* loader)
{
    mClassLoader = loader;
    return NOERROR;
}

ECode CBundle::Clone(
    /* [out] */ IBundle** result)
{
    // return new Bundle(this);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

void CBundle::Unparcel()
{
    Mutex::Autolock lock(&_m_syncLock);

    if (mParcelledData == 0) {
        return;
    }

    int N = ((android::Parcel*)mParcelledData)->readInt32();
    if (N < 0) {
        return;
    }
    if (mMap == NULL) {
        mMap = new HashMap<String, AutoPtr<IInterface> >(7);
    }
    ReadMapInternal(mParcelledData, N, mClassLoader);
    ((android::Parcel*)mParcelledData)->freeData();
    delete (android::Parcel*)mParcelledData;
    mParcelledData = 0;
}

ECode CBundle::GetSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Unparcel();
    *result = mMap->GetSize();
    return NOERROR;
}

ECode CBundle::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Unparcel();
    *result = mMap->IsEmpty();
    return NOERROR;
}

ECode CBundle::Clear()
{
    Unparcel();
    mMap->Clear();
    mHasFds = FALSE;
    mFdsKnown = TRUE;
    return NOERROR;
}

ECode CBundle::ContainsKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Unparcel();
    *result = (mMap->Find(key) != mMap->End());
    return NOERROR;
}

ECode CBundle::Get(
    /* [in] */ const String& key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    *value = NULL;
    if (it != mMap->End()) {
        *value = it->mSecond;
        if (*value != NULL) (*value)->AddRef();
    }
    return NOERROR;
}

ECode CBundle::Remove(
    /* [in] */ const String& key)
{
    Unparcel();
    mMap->Erase(key);
    return NOERROR;
}

ECode CBundle::PutAll(
    /* [in] */ IBundle* map)
{
    VALIDATE_NOT_NULL(map);
    CBundle* mapObj = (CBundle*)map;

    Unparcel();
    mapObj->Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mapObj->mMap->Begin();
    while (it != mapObj->mMap->End()) {
        (*mMap)[it->mFirst] = it->mSecond;
        ++it;
    }

    // fd state is now known if and only if both bundles already knew
    mHasFds |= mapObj->mHasFds;
    mFdsKnown = mFdsKnown && mapObj->mFdsKnown;
    return NOERROR;
}

ECode CBundle::KeySet(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    Unparcel();
    FAIL_RETURN(CObjectContainer::New(result));
    HashMap<String, AutoPtr<IInterface> >::Iterator it;
    for (it = mMap->Begin(); it != mMap->End(); ++it) {
        AutoPtr<ICharSequence> str;
        CStringWrapper::New(it->mFirst, (ICharSequence**)&str);
        (*result)->Add(str.Get());
    }
    return NOERROR;
}

ECode CBundle::HasFileDescriptors(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // if (!mFdsKnown) {
    //         boolean fdFound = false;    // keep going until we find one or run out of data

    //         if (mParcelledData != null) {
    //             if (mParcelledData.hasFileDescriptors()) {
    //                 fdFound = true;
    //             }
    //         } else {
    //             // It's been unparcelled, so we need to walk the map
    //             Iterator<Map.Entry<String, Object>> iter = mMap->entrySet().iterator();
    //             while (!fdFound && iter.hasNext()) {
    //                 Object obj = iter.next().getValue();
    //                 if (obj instanceof Parcelable) {
    //                     if ((((Parcelable)obj).describeContents()
    //                             & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
    //                         fdFound = true;
    //                         break;
    //                     }
    //                 } else if (obj instanceof Parcelable[]) {
    //                     Parcelable[] array = (Parcelable[]) obj;
    //                     for (int n = array.length - 1; n >= 0; n--) {
    //                         if ((array[n].describeContents()
    //                                 & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
    //                             fdFound = true;
    //                             break;
    //                         }
    //                     }
    //                 } else if (obj instanceof SparseArray) {
    //                     SparseArray<? extends Parcelable> array =
    //                             (SparseArray<? extends Parcelable>) obj;
    //                     for (int n = array.size() - 1; n >= 0; n--) {
    //                         if ((array.get(n).describeContents()
    //                                 & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
    //                             fdFound = true;
    //                             break;
    //                         }
    //                     }
    //                 } else if (obj instanceof ArrayList) {
    //                     ArrayList array = (ArrayList) obj;
    //                     // an ArrayList here might contain either Strings or
    //                     // Parcelables; only look inside for Parcelables
    //                     if ((array.size() > 0)
    //                             && (array.get(0) instanceof Parcelable)) {
    //                         for (int n = array.size() - 1; n >= 0; n--) {
    //                             Parcelable p = (Parcelable) array.get(n);
    //                             if (p != null && ((p.describeContents()
    //                                     & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0)) {
    //                                 fdFound = true;
    //                                 break;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }

    //         mHasFds = fdFound;
    //         mFdsKnown = true;
    //     }
    //     return mHasFds;
    *result = FALSE;
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::PutBoolean(
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IBoolean> obj;
    CBoolean::New(value, (IBoolean**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutByte(
    /* [in] */ const String& key,
    /* [in] */ Byte value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IByte> obj;
    CByte::New(value, (IByte**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutChar(
    /* [in] */ const String& key,
    /* [in] */ Char32 value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IChar32> obj;
    CChar32::New(value, (IChar32**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt16(
    /* [in] */ const String& key,
    /* [in] */ Int16 value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IInteger16> obj;
    CInteger16::New(value, (IInteger16**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IInteger32> obj;
    CInteger32::New(value, (IInteger32**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt64(
    /* [in] */ const String& key,
    /* [in] */ Int64 value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IInteger64> obj;
    CInteger64::New(value, (IInteger64**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutFloat(
    /* [in] */ const String& key,
    /* [in] */ Float value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IFloat> obj;
    CFloat::New(value, (IFloat**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutDouble(
    /* [in] */ const String& key,
    /* [in] */ Double value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IDouble> obj;
    CDouble::New(value, (IDouble**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutString(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<ICharSequence> obj;
    CStringWrapper::New(value, (ICharSequence**)&obj);
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutCharSequence(
    /* [in] */ const String& key,
    /* [in] */ ICharSequence* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutParcelable(
    /* [in] */ const String& key,
    /* [in] */ IParcelable* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutParcelableArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<IParcelable*>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            obj->Put(i, (*value)[i]);
        }
    }
    (*mMap)[key] = obj.Get();
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutParcelableArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutIntegerArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutStringArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutCharSequenceArrayList(
    /* [in] */ const String& key,
    /* [in] */ IObjectContainer* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

// PutSerializable(
//     /* [in] */ const String& key,
//     /* [in] */ ISerializable* value);

ECode CBundle::PutBooleanArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Boolean>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IBoolean> bv;
            CBoolean::New((*value)[i], (IBoolean**)&bv);
            obj->Put(i, bv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutByteArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Byte>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IByte> bv;
            CByte::New((*value)[i], (IByte**)&bv);
            obj->Put(i, bv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt16Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int16>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IInteger16> iv;
            CInteger16::New((*value)[i], (IInteger16**)&iv);
            obj->Put(i, iv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutCharArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Char32>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IChar32> cv;
            CChar32::New((*value)[i], (IChar32**)&cv);
            obj->Put(i, cv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt32Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int32>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IInteger32> iv;
            CInteger32::New((*value)[i], (IInteger32**)&iv);
            obj->Put(i, iv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutInt64Array(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Int64>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IInteger64> iv;
            CInteger64::New((*value)[i], (IInteger64**)&iv);
            obj->Put(i, iv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutFloatArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Float>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IFloat> fv;
            CFloat::New((*value)[i], (IFloat**)&fv);
            obj->Put(i, fv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutDoubleArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Double>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<IDouble> dv;
            CDouble::New((*value)[i], (IDouble**)&dv);
            obj->Put(i, dv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutStringArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<String>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            AutoPtr<ICharSequence> sv;
            CStringWrapper::New((*value)[i], (ICharSequence**)&sv);
            obj->Put(i, sv.Get());
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutCharSequenceArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<ICharSequence*>* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    AutoPtr<IArray> obj;
    if (value != NULL) {
        CArray::New(value->GetLength(), (IArray**)&obj);
        for (Int32 i = 0; i < value->GetLength(); ++i) {
            obj->Put(i, (*value)[i]);
        }
    }
    (*mMap)[key] = obj.Get();
    return NOERROR;
}

ECode CBundle::PutBundle(
    /* [in] */ const String& key,
    /* [in] */ IBundle* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

ECode CBundle::PutIBinder(
    /* [in] */ const String& key,
    /* [in] */ IBinder* value)
{
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    (*mMap)[key] = value;
    return NOERROR;
}

// Log a message if the value was non-null but not of the expected type
void CBundle::TypeWarning(
    /* [in] */ const String& key,
    /* [in] */ IInterface* value,
    /* [in] */ CString className)
{
    StringBuffer sb;
    sb += "Key ";
    sb += key;
    sb += " expected ";
    sb += className;
    sb += " but value was a ";
    // sb.append(value.getClass().getName());
    sb += ".";
    // Log.w(LOG_TAG, sb.toString());
    // Log.w(LOG_TAG, "Attempt to cast generated internal exception:", e);
}

ECode CBundle::GetBoolean(
    /* [in] */ const String& key,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetBooleanEx(key, FALSE, value);
}

ECode CBundle::GetBooleanEx(
    /* [in] */ const String& key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = defaultValue;
        return NOERROR;
    }
    // try {
    IBoolean* obj = IBoolean::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "Boolean");
        *value = defaultValue;
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Boolean", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetByte(
    /* [in] */ const String& key,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetByteEx(key, 0, value);
}

ECode CBundle::GetByteEx(
    /* [in] */ const String& key,
    /* [in] */ Byte defaultValue,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = defaultValue;
        return NOERROR;
    }
    // try {
    IByte* obj = IByte::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "Byte");
        *value = defaultValue;
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Byte", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetChar(
    /* [in] */ const String& key,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetCharEx(key, 0, value);
}

ECode CBundle::GetCharEx(
    /* [in] */ const String& key,
    /* [in] */ Char32 defaultValue,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = defaultValue;
        return NOERROR;
    }
    // try {
    IChar32* obj = IChar32::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "Char32");
        *value = defaultValue;
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Character", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt16(
    /* [in] */ const String& key,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetInt16Ex(key, 0, value);
}

ECode CBundle::GetInt16Ex(
    /* [in] */ const String& key,
    /* [in] */ Int16 defaultValue,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = defaultValue;
        return NOERROR;
    }
    // try {
    IInteger16* obj = IInteger16::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "Int16");
        *value = defaultValue;
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Short", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt32(
    /* [in] */ const String& key,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetInt32Ex(key, 0, value);
}

ECode CBundle::GetInt32Ex(
    /* [in] */ const String& key,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = defaultValue;
        return NOERROR;
    }
    // try {
    IInteger32* obj = IInteger32::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "Int32");
        *value = defaultValue;
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Integer", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt64(
    /* [in] */ const String& key,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetInt64Ex(key, 0ll, value);
}

ECode CBundle::GetInt64Ex(
    /* [in] */ const String& key,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = defaultValue;
        return NOERROR;
    }
    // try {
    IInteger64* obj = IInteger64::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "Int64");
        *value = defaultValue;
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Long", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetFloat(
    /* [in] */ const String& key,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetFloatEx(key, 0.0f, value);
}

ECode CBundle::GetFloatEx(
    /* [in] */ const String& key,
    /* [in] */ Float defaultValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = defaultValue;
        return NOERROR;
    }
    // try {
    IFloat* obj = IFloat::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "Float");
        *value = defaultValue;
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Float", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetDouble(
    /* [in] */ const String& key,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    return GetDoubleEx(key, 0.0, value);
}

ECode CBundle::GetDoubleEx(
    /* [in] */ const String& key,
    /* [in] */ Double defaultValue,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = defaultValue;
        return NOERROR;
    }
    // try {
    IDouble* obj = IDouble::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "Double");
        *value = defaultValue;
        return NOERROR;
    }
    return obj->GetValue(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Double", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetString(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    ICharSequence* obj = ICharSequence::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "String");
        *value = NULL;
        return NOERROR;
    }
    return obj->ToString(value);
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "String", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetCharSequence(
    /* [in] */ const String& key,
    /* [out] */ ICharSequence** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    *value = ICharSequence::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, "ICharSequence");
        return NOERROR;
    }
    (*value)->AddRef();
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "CharSequence", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetBundle(
    /* [in] */ const String& key,
    /* [out] */ IBundle** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    *value = IBundle::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, "IBundle");
        return NOERROR;
    }
    (*value)->AddRef();
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Bundle", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetParcelable(
    /* [in] */ const String& key,
    /* [out] */ IParcelable** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    *value = IParcelable::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, "IParcelable");
        return NOERROR;
    }
    (*value)->AddRef();
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Parcelable", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetParcelableArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<IParcelable*>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<IParcelable*>* array = ArrayOf<IParcelable*>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (elem != NULL) {
            if (IParcelable::Probe(elem) == NULL) goto type_error;
            (*array)[i] = IParcelable::Probe(elem);
            (*array)[i]->AddRef();
        }
    }
    *value = array;
    return NOERROR;
type_error:
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        if ((*array)[i] != NULL) (*array)[i]->Release();
    }
    ArrayOf<IParcelable*>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-IParcelable");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "Parcelable[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetParcelableArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    *value = IObjectContainer::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, "IObjectContainer");
        return NOERROR;
    }
    (*value)->AddRef();
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ArrayList", defaultValue, e);
    //     return defaultValue;
    // }
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
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    *value = IObjectContainer::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, "IObjectContainer");
        return NOERROR;
    }
    (*value)->AddRef();
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ArrayList<Integer>", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetStringArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    *value = IObjectContainer::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, "IObjectContainer");
        return NOERROR;
    }
    (*value)->AddRef();
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ArrayList<String>", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetCharSequenceArrayList(
    /* [in] */ const String& key,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    *value = IObjectContainer::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, "IObjectContainer");
        return NOERROR;
    }
    (*value)->AddRef();
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "ArrayList<CharSequence>", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetBooleanArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Boolean>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<Boolean>* array = ArrayOf<Boolean>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IBoolean::Probe(elem) == NULL) goto type_error;
        Boolean bv;
        IBoolean::Probe(elem)->GetValue(&bv);
        (*array)[i] = bv;
    }
    *value = array;
    return NOERROR;
type_error:
    ArrayOf<Boolean>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-Boolean");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "boolean[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetByteArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<Byte>* array = ArrayOf<Byte>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IByte::Probe(elem) == NULL) goto type_error;
        Byte bv;
        IByte::Probe(elem)->GetValue(&bv);
        (*array)[i] = bv;
    }
    *value = array;
    return NOERROR;
type_error:
    ArrayOf<Byte>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-Byte");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "byte[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt16Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int16>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<Int16>* array = ArrayOf<Int16>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IInteger16::Probe(elem) == NULL) goto type_error;
        Int16 iv;
        IInteger16::Probe(elem)->GetValue(&iv);
        (*array)[i] = iv;
    }
    *value = array;
    return NOERROR;
type_error:
    ArrayOf<Int16>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-Int16");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "short[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetCharArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Char32>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<Char32>* array = ArrayOf<Char32>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IChar32::Probe(elem) == NULL) goto type_error;
        Char32 cv;
        IChar32::Probe(elem)->GetValue(&cv);
        (*array)[i] = cv;
    }
    *value = array;
    return NOERROR;
type_error:
    ArrayOf<Char32>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-Char32");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "char[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt32Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int32>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<Int32>* array = ArrayOf<Int32>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IInteger32::Probe(elem) == NULL) goto type_error;
        Int32 iv;
        IInteger32::Probe(elem)->GetValue(&iv);
        (*array)[i] = iv;
    }
    *value = array;
    return NOERROR;
type_error:
    ArrayOf<Int32>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-Int32");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "int[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetInt64Array(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Int64>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<Int64>* array = ArrayOf<Int64>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IInteger64::Probe(elem) == NULL) goto type_error;
        Int64 iv;
        IInteger64::Probe(elem)->GetValue(&iv);
        (*array)[i] = iv;
    }
    *value = array;
    return NOERROR;
type_error:
    ArrayOf<Int64>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-Int64");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "long[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetFloatArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Float>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<Float>* array = ArrayOf<Float>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IFloat::Probe(elem) == NULL) goto type_error;
        Float fv;
        IFloat::Probe(elem)->GetValue(&fv);
        (*array)[i] = fv;
    }
    *value = array;
    return NOERROR;
type_error:
    ArrayOf<Float>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-Float");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "float[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetDoubleArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<Double>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<Double>* array = ArrayOf<Double>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (IDouble::Probe(elem) == NULL) goto type_error;
        Double dv;
        IDouble::Probe(elem)->GetValue(&dv);
        (*array)[i] = dv;
    }
    *value = array;
    return NOERROR;
type_error:
    ArrayOf<Double>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-Double");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "double[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetStringArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<String>* array = ArrayOf<String>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (ICharSequence::Probe(elem) == NULL) goto type_error;
        String sv;
        ICharSequence::Probe(elem)->ToString(&sv);
        (*array)[i] = sv;
    }
    *value = array;
    return NOERROR;
type_error:
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        (*array)[i] = NULL;
    }
    ArrayOf<String>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-String");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "String[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetCharSequenceArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<ICharSequence*>** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    IArray* obj = IArray::Probe(it->mSecond);
    if (obj == NULL) {
        TypeWarning(key, it->mSecond, "IArray");
        *value = NULL;
        return NOERROR;
    }

    Int32 size;
    obj->GetLength(&size);
    ArrayOf<ICharSequence*>* array = ArrayOf<ICharSequence*>::Alloc(size);
    if (array == NULL) return E_OUT_OF_MEMORY_ERROR;

    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        obj->Get(i, (IInterface**)&elem);
        if (elem != NULL) {
            if (ICharSequence::Probe(elem) == NULL) goto type_error;
            (*array)[i] = ICharSequence::Probe(elem);
            (*array)[i]->AddRef();
        }
    }
    *value = array;
    return NOERROR;
type_error:
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        if ((*array)[i] != NULL) (*array)[i]->Release();
    }
    ArrayOf<ICharSequence*>::Free(array);
    TypeWarning(key, it->mSecond, "IArray-ICharSequence");
    *value = NULL;
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "CharSequence[]", defaultValue, e);
    //     return defaultValue;
    // }
}

ECode CBundle::GetIBinder(
    /* [in] */ const String& key,
    /* [out] */ IBinder** value)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_STRING_NOT_NULL(key);

    Unparcel();
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap->Find(key);
    if (it == mMap->End()) {
        *value = NULL;
        return NOERROR;
    }
    // try {
    *value = IBinder::Probe(it->mSecond);
    if (*value == NULL) {
        TypeWarning(key, it->mSecond, "IBinder");
        return NOERROR;
    }
    (*value)->AddRef();
    return NOERROR;
    // } catch (ClassCastException e) {
    //     typeWarning(key, o, "IBinder", defaultValue, e);
    //     return defaultValue;
    // }
}

// @Override
// public synchronized const String& toString();

ECode CBundle::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBundle::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    if (mParcelledData != 0) {
        android::Parcel* parcel;
        dest->GetElementPayload((Handle32*)&parcel);
        int length = ((android::Parcel*)mParcelledData)->dataSize();
        parcel->writeInt32(length);
        parcel->writeInt32(0x4C444E42); // 'B' 'N' 'D' 'L'
        parcel->appendFrom((android::Parcel*)mParcelledData, 0, length);
    }
    else {
        dest->WriteInt32(-1); // dummy, will hold length
        dest->WriteInt32(0x4C444E42); // 'B' 'N' 'D' 'L'

        Int32 oldPos, newPos;
        dest->GetDataPosition(&oldPos);
        WriteMapInternal(dest);
        dest->GetDataPosition(&newPos);

        // Backpatch length
        dest->SetDataPosition(oldPos - 8);
        Int32 length = newPos - oldPos;
        dest->WriteInt32(length);
        dest->SetDataPosition(newPos);
    }
    return NOERROR;
}

ECode CBundle::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    Int32 size;
    source->ReadInt32(&size);
    if (size < 0) {
        // throw new RuntimeException("Bad length in parcel: " + length);
        return E_RUNTIME_EXCEPTION;
    }
    return ReadFromParcelInner(source, size);
}

ECode CBundle::ReadFromParcelInner(
    /* [in] */ IParcel* source,
    /* [in] */ Int32 length)
{
    android::Parcel* parcel;
    source->GetElementPayload((Handle32*)&parcel);

    Int32 magic = parcel->readInt32();
    if (magic != 0x4C444E42) {
        //noinspection ThrowableInstanceNeverThrown
        // String st = Log.getStackTraceString(new RuntimeException());
        // Log.e("Bundle", "readBundle: bad magic number");
        // Log.e("Bundle", "readBundle: trace = " + st);
    }

    // Advance within this Parcel
    Int32 offset = parcel->dataPosition();
    parcel->setDataPosition(offset + length);

    android::Parcel* p = new android::Parcel();
    p->setDataPosition(0);
    p->appendFrom(parcel, offset, length);
    p->setDataPosition(0);

    mParcelledData = (Handle32)p;
    mHasFds = p->hasFileDescriptors();
    mFdsKnown = TRUE;

    return NOERROR;
}

ECode CBundle::ReadFromParcel(
    /* [in] */ IParcel *source,
    /* [in] */ Int32 length,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);
    return CBundle::New(source, length, bundle);
}

ECode CBundle::WriteToParcel(
    /* [in] */ IBundle *bundle,
    /* [in] */ IParcel *dest)
{
    if (bundle != NULL) {
        return IParcelable::Probe(bundle)->WriteToParcel(dest);
    }
    return NOERROR;
}

void CBundle::ReadMapInternal(
    /* [in] */ Handle32 source,
    /* [in] */ Int32 size,
    /* [in] */ IClassLoader* classLoader)
{
    android::Parcel* p = (android::Parcel*)source;
    while (size > 0) {
        String key = String(p->readCString());
        AutoPtr<IInterface> value = ReadValue((Handle32)p);
        (*mMap)[key] = value;
        size--;
    }
}

void CBundle::WriteMapInternal(
    /* [in] */ IParcel* dest)
{
    if (mMap == NULL) {
        dest->WriteInt32(-1);
        return;
    }
    dest->WriteInt32(mMap->GetSize());
    HashMap<String, AutoPtr<IInterface> >::Iterator it;
    for (it = mMap->Begin(); it != mMap->End(); ++it) {
        dest->WriteString(it->mFirst);
        WriteValue(dest, it->mSecond);
    }
}

ECode CBundle::WriteValue(
    /* [in] */ IParcel* dest,
    /* [in] */ IInterface* obj)
{
    if (obj == NULL) {
        dest->WriteInt32(VAL_NULL);
    }
    // else if (v instanceof String) {
    //     writeInt(VAL_STRING);
    //     writeString((String) v);
    // }
    else if (IInteger32::Probe(obj) != NULL) {
        Int32 v;
        IInteger32::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_INTEGER);
        dest->WriteInt32(v);
    }
    // else if (v instanceof Map) {
    //     writeInt(VAL_MAP);
    //     writeMap((Map) v);
    // }
    else if (IBundle::Probe(obj) != NULL) {
        // // Must be before Parcelable
        // writeInt(VAL_BUNDLE);
        // writeBundle((Bundle) v);
        assert(0);
    }
    // else if (v instanceof Parcelable) {
    //     writeInt(VAL_PARCELABLE);
    //     writeParcelable((Parcelable) v, 0);
    // }
    else if (IInteger16::Probe(obj) != NULL) {
        Int16 v;
        IInteger16::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_SHORT);
        dest->WriteInt32((Int32)v);
    }
    else if (IInteger64::Probe(obj) != NULL) {
        Int64 v;
        IInteger64::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_LONG);
        dest->WriteInt64(v);
    }
    else if (IFloat::Probe(obj) != NULL) {
        Float v;
        IFloat::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_FLOAT);
        dest->WriteFloat(v);
    }
    else if (IDouble::Probe(obj) != NULL) {
        Double v;
        IDouble::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_DOUBLE);
        dest->WriteDouble(v);
    }
    else if (IBoolean::Probe(obj) != NULL) {
        Boolean v;
        IBoolean::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_BOOLEAN);
        dest->WriteInt32(v ? 1 : 0);
    }
    else if (ICharSequence::Probe(obj) != NULL) {
        // Must be after String
        String v;
        ICharSequence::Probe(obj)->ToString(&v);
        dest->WriteInt32(VAL_CHARSEQUENCE);
        // writeCharSequence((CharSequence) v);
        dest->WriteString(v);
    }
    // else if (v instanceof List) {
    //     writeInt(VAL_LIST);
    //     writeList((List) v);
    // }
    // else if (v instanceof SparseArray) {
    //     writeInt(VAL_SPARSEARRAY);
    //     writeSparseArray((SparseArray) v);
    // }
    // else if (v instanceof boolean[]) {
    //     writeInt(VAL_BOOLEANARRAY);
    //     writeBooleanArray((boolean[]) v);
    // }
    // else if (v instanceof byte[]) {
    //     writeInt(VAL_BYTEARRAY);
    //     writeByteArray((byte[]) v);
    // }
    // else if (v instanceof String[]) {
    //     writeInt(VAL_STRINGARRAY);
    //     writeStringArray((String[]) v);
    // }
    // else if (v instanceof CharSequence[]) {
    //     // Must be after String[] and before Object[]
    //     writeInt(VAL_CHARSEQUENCEARRAY);
    //     writeCharSequenceArray((CharSequence[]) v);
    // }
    // else if (v instanceof IBinder) {
    //     writeInt(VAL_IBINDER);
    //     writeStrongBinder((IBinder) v);
    // }
    // else if (v instanceof Parcelable[]) {
    //     writeInt(VAL_PARCELABLEARRAY);
    //     writeParcelableArray((Parcelable[]) v, 0);
    // }
    // else if (v instanceof Object[]) {
    //     writeInt(VAL_OBJECTARRAY);
    //     writeArray((Object[]) v);
    // }
    // else if (v instanceof int[]) {
    //     writeInt(VAL_INTARRAY);
    //     writeIntArray((int[]) v);
    // }
    // else if (v instanceof long[]) {
    //     writeInt(VAL_LONGARRAY);
    //     writeLongArray((long[]) v);
    // }
    else if (IByte::Probe(obj) != NULL) {
        Byte v;
        IByte::Probe(obj)->GetValue(&v);
        dest->WriteInt32(VAL_BYTE);
        dest->WriteInt32((Int32)v);
    }
    // else if (v instanceof Serializable) {
    //     // Must be last
    //     writeInt(VAL_SERIALIZABLE);
    //     writeSerializable((Serializable) v);
    // }
    else {
        // throw new RuntimeException("Parcel: unable to marshal value " + v);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<IInterface> CBundle::ReadValue(
    /* [in] */ Handle32 source)
{
    android::Parcel* p = (android::Parcel*)source;

    Int32 type = p->readInt32();

    switch (type) {
    case VAL_NULL:
        return NULL;
    // case VAL_STRING:
    //     return readString();
    case VAL_INTEGER: {
        Int32 v = p->readInt32();
        AutoPtr<IInteger32> obj;
        CInteger32::New(v, (IInteger32**)&obj);
        return obj;
    }
    // case VAL_MAP:
    //     return readHashMap(loader);

    // case VAL_PARCELABLE:
    //     return readParcelable(loader);
    case VAL_SHORT: {
        Int16 v = (Int16)p->readInt32();
        AutoPtr<IInteger16> obj;
        CInteger16::New(v, (IInteger16**)&obj);
        return obj;
    }
    case VAL_LONG: {
        Int64 v = p->readInt64();
        AutoPtr<IInteger64> obj;
        CInteger64::New(v, (IInteger64**)&obj);
        return obj;
    }
    case VAL_FLOAT: {
        Float v = p->readFloat();
        AutoPtr<IFloat> obj;
        CFloat::New(v, (IFloat**)&obj);
        return obj;
    }
    case VAL_DOUBLE: {
        Double v = p->readDouble();
        AutoPtr<IDouble> obj;
        CDouble::New(v, (IDouble**)&obj);
        return obj;
    }
    case VAL_BOOLEAN: {
        Int32 v = p->readInt32();
        AutoPtr<IBoolean> obj;
        CBoolean::New(v == 1, (IBoolean**)&obj);
        return obj;
    }
    case VAL_CHARSEQUENCE: {
        String v = String(p->readCString());
        AutoPtr<ICharSequence> obj;
        CParcelableStringWrapper::New(v, (ICharSequence**)&obj);
        return obj;
    }
    // case VAL_LIST:
    //     return readArrayList(loader);
    // case VAL_BOOLEANARRAY:
    //     return createBooleanArray();
    // case VAL_BYTEARRAY:
    //     return createByteArray();
    // case VAL_STRINGARRAY:
    //     return readStringArray();
    // case VAL_CHARSEQUENCEARRAY:
    //     return readCharSequenceArray();
    // case VAL_IBINDER:
    //     return readStrongBinder();
    // case VAL_OBJECTARRAY:
    //     return readArray(loader);
    // case VAL_INTARRAY:
    //     return createIntArray();
    // case VAL_LONGARRAY:
    //     return createLongArray();
    case VAL_BYTE: {
        Byte v = (Byte)p->readInt32();
        AutoPtr<IByte> obj;
        CByte::New(v, (IByte**)&obj);
        return obj;
    }
    // case VAL_SERIALIZABLE:
    //     return readSerializable();
    // case VAL_PARCELABLEARRAY:
    //     return readParcelableArray(loader);
    // case VAL_SPARSEARRAY:
    //     return readSparseArray(loader);
    // case VAL_SPARSEBOOLEANARRAY:
    //     return readSparseBooleanArray();
    // case VAL_BUNDLE:
    //     return readBundle(loader); // loading will be deferred
    default:
        // int off = dataPosition() - 4;
        // throw new RuntimeException(
        //     "Parcel " + this + ": Unmarshalling unknown type code " + type + " at offset " + off);
        assert(0);
    }
}
