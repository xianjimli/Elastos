
#ifndef __CBUNDLE_H__
#define __CBUNDLE_H__

#include "ext/frameworkext.h"
#include "_CBundle.h"
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


CarClass(CBundle)
{
public:
    CBundle();

    ~CBundle();

public:

    /**
     * TODO: optimize this later (getting just the value part of a Bundle
     * with a single pair) once Bundle.forPair() above is implemented
     * with a special single-value Map implementation/serialization.
     *
     * Note: value in single-pair Bundle may be null.
     *
     * @hide
     */
    CARAPI GetPairValue(
        /* [out] */ String* result);

    /**
     * Changes the ClassLoader this Bundle uses when instantiating objects.
     *
     * @param loader An explicit ClassLoader to use when instantiating objects
     * inside of the Bundle.
     */
    CARAPI SetClassLoader(
        /* [in] */ IClassLoader* loader);

    /**
     * Clones the current Bundle. The internal map is cloned, but the keys and
     * values to which it refers are copied by reference.
     */
    // @Override
    CARAPI Clone(
        /* [out] */ IBundle** result);

    /**
     * Returns the number of mappings contained in this Bundle.
     *
     * @return the number of mappings as an int.
     */
    CARAPI GetSize(
        /* [out] */ Int32* result);

    /**
     * Returns true if the mapping of this Bundle is empty, false otherwise.
     */
    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    /**
     * Removes all elements from the mapping of this Bundle.
     */
    CARAPI Clear();

    /**
     * Returns true if the given key is contained in the mapping
     * of this Bundle.
     *
     * @param key a const String& key
     * @return true if the key is part of the mapping, false otherwise
     */
    CARAPI ContainsKey(
        /* [in] */ const String& key,
        /* [out] */ Boolean* result);

    /**
     * Returns the entry with the given key as an object.
     *
     * @param key a const String& key
     * @return an Object, or null
     */
    CARAPI Get(
        /* [in] */ const String& key,
        /* [out] */ IInterface** value);

    /**
     * Removes any entry with the given key from the mapping of this Bundle.
     *
     * @param key a const String& key
     */
    CARAPI Remove(
        /* [in] */ const String& key);

    /**
     * Inserts all mappings from the given Bundle into this Bundle.
     *
     * @param map a Bundle
     */
    CARAPI PutAll(
        /* [in] */ IBundle* map);

    /**
     * Returns a Set containing the Strings used as keys in this Bundle.
     *
     * @return a Set of const String& keys
     */
    CARAPI KeySet(
        /* [out] */ IObjectContainer** result);

    /**
     * Reports whether the bundle contains any parcelled file descriptors.
     */
    CARAPI HasFileDescriptors(
        /* [out] */ Boolean* result);

    CARAPI PutBoolean(
        /* [in] */ const String& key,
        /* [in] */ Boolean value);

    CARAPI PutByte(
        /* [in] */ const String& key,
        /* [in] */ Byte value);

    CARAPI PutChar(
        /* [in] */ const String& key,
        /* [in] */ Char32 value);

    CARAPI PutInt16(
        /* [in] */ const String& key,
        /* [in] */ Int16 value);

    CARAPI PutInt32(
        /* [in] */ const String& key,
        /* [in] */ Int32 value);

    CARAPI PutInt64(
        /* [in] */ const String& key,
        /* [in] */ Int64 value);

    CARAPI PutFloat(
        /* [in] */ const String& key,
        /* [in] */ Float value);

    CARAPI PutDouble(
        /* [in] */ const String& key,
        /* [in] */ Double value);

    CARAPI PutString(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    /**
     * Inserts a CharSequence value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a CharSequence, or null
     */
    CARAPI PutCharSequence(
        /* [in] */ const String& key,
        /* [in] */ ICharSequence* value);

    /**
     * Inserts a Parcelable value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a Parcelable object, or null
     */
    CARAPI PutParcelable(
        /* [in] */ const String& key,
        /* [in] */ IParcelable* value);

    /**
     * Inserts an array of Parcelable values into the mapping of this Bundle,
     * replacing any existing value for the given key.  Either key or value may
     * be null.
     *
     * @param key a const String&, or null
     * @param value an array of Parcelable objects, or null
     */
    CARAPI PutParcelableArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<IParcelable*>* value);

    /**
     * Inserts a List of Parcelable values into the mapping of this Bundle,
     * replacing any existing value for the given key.  Either key or value may
     * be null.
     *
     * @param key a const String&, or null
     * @param value an ArrayList of Parcelable objects, or null
     */
    CARAPI PutParcelableArrayList(
        /* [in] */ const String& key,
        /* [in] */ IObjectContainer* value);

    /**
     * Inserts a SparceArray of Parcelable values into the mapping of this
     * Bundle, replacing any existing value for the given key.  Either key
     * or value may be null.
     *
     * @param key a const String&, or null
     * @param value a SparseArray of Parcelable objects, or null
     */
    // public void putSparseParcelableArray(const String& key,
    //         SparseArray<? extends Parcelable> value);

    /**
     * Inserts an ArrayList<Integer> value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value an ArrayList<Integer> object, or null
     */
    CARAPI PutIntegerArrayList(
        /* [in] */ const String& key,
        /* [in] */ IObjectContainer* value);

    /**
     * Inserts an ArrayList<String> value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value an ArrayList<String> object, or null
     */
    CARAPI PutStringArrayList(
        /* [in] */ const String& key,
        /* [in] */ IObjectContainer* value);

    /**
     * Inserts an ArrayList<CharSequence> value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value an ArrayList<CharSequence> object, or null
     */
    CARAPI PutCharSequenceArrayList(
        /* [in] */ const String& key,
        /* [in] */ IObjectContainer* value);

    /**
     * Inserts a Serializable value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a Serializable object, or null
     */
    // PutSerializable(
    //     /* [in] */ const String& key,
    //     /* [in] */ ISerializable* value);

    /**
     * Inserts a boolean array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a boolean array object, or null
     */
    CARAPI PutBooleanArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Boolean>* value);

    /**
     * Inserts a byte array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a byte array object, or null
     */
    CARAPI PutByteArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Byte>* value);

    /**
     * Inserts a short array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a short array object, or null
     */
    CARAPI PutInt16Array(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Int16>* value);

    /**
     * Inserts a char array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a char array object, or null
     */
    CARAPI PutCharArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Char32>* value);

    /**
     * Inserts an int array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value an int array object, or null
     */
    CARAPI PutInt32Array(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Int32>* value);

    /**
     * Inserts a long array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a long array object, or null
     */
    CARAPI PutInt64Array(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Int64>* value);

    /**
     * Inserts a float array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a float array object, or null
     */
    CARAPI PutFloatArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Float>* value);

    /**
     * Inserts a double array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a double array object, or null
     */
    CARAPI PutDoubleArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Double>* value);

    /**
     * Inserts a const String& array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a const String& array object, or null
     */
    CARAPI PutStringArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<String>* value);

    /**
     * Inserts a CharSequence array value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a CharSequence array object, or null
     */
    CARAPI PutCharSequenceArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<ICharSequence*>* value);

    /**
     * Inserts a Bundle value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value a Bundle object, or null
     */
    CARAPI PutBundle(
        /* [in] */ const String& key,
        /* [in] */ IBundle* value);

    /**
     * Inserts an IBinder value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a const String&, or null
     * @param value an IBinder object, or null
     *
     * @deprecated
     * @hide
     */
    // @Deprecated
    CARAPI PutIBinder(
        /* [in] */ const String& key,
        /* [in] */ IBinder* value);

    CARAPI GetBoolean(
        /* [in] */ const String& key,
        /* [out] */ Boolean* value);

    /**
     * Returns the value associated with the given key, or defaultValue if
     * no mapping of the desired type exists for the given key.
     *
     * @param key a const String&
     * @return a boolean value
     */
    CARAPI GetBooleanEx(
        /* [in] */ const String& key,
        /* [in] */ Boolean defaultValue,
        /* [out] */ Boolean* value);

    CARAPI GetByte(
        /* [in] */ const String& key,
        /* [out] */ Byte* value);

    /**
     * Returns the value associated with the given key, or defaultValue if
     * no mapping of the desired type exists for the given key.
     *
     * @param key a const String&
     * @return a byte value
     */
    CARAPI GetByteEx(
        /* [in] */ const String& key,
        /* [in] */ Byte defaultValue,
        /* [out] */ Byte* value);

    CARAPI GetChar(
        /* [in] */ const String& key,
        /* [out] */ Char32* value);

    /**
     * Returns the value associated with the given key, or (char) 0 if
     * no mapping of the desired type exists for the given key.
     *
     * @param key a const String&
     * @return a char value
     */
    CARAPI GetCharEx(
        /* [in] */ const String& key,
        /* [in] */ Char32 defaultValue,
        /* [out] */ Char32* value);

    CARAPI GetInt16(
        /* [in] */ const String& key,
        /* [out] */ Int16* value);

    /**
     * Returns the value associated with the given key, or defaultValue if
     * no mapping of the desired type exists for the given key.
     *
     * @param key a const String&
     * @return a short value
     */
    CARAPI GetInt16Ex(
        /* [in] */ const String& key,
        /* [in] */ Int16 defaultValue,
        /* [out] */ Int16* value);

    CARAPI GetInt32(
        /* [in] */ const String& key,
        /* [out] */ Int32* value);

    /**
     * Returns the value associated with the given key, or defaultValue if
     * no mapping of the desired type exists for the given key.
     *
     * @param key a const String&
     * @return an int value
     */
    CARAPI GetInt32Ex(
        /* [in] */ const String& key,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    CARAPI GetInt64(
        /* [in] */ const String& key,
        /* [out] */ Int64* value);

    /**
     * Returns the value associated with the given key, or defaultValue if
     * no mapping of the desired type exists for the given key.
     *
     * @param key a const String&
     * @return a long value
     */
    CARAPI GetInt64Ex(
        /* [in] */ const String& key,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    CARAPI GetFloat(
        /* [in] */ const String& key,
        /* [out] */ Float* value);

    /**
     * Returns the value associated with the given key, or defaultValue if
     * no mapping of the desired type exists for the given key.
     *
     * @param key a const String&
     * @return a float value
     */
    CARAPI GetFloatEx(
        /* [in] */ const String& key,
        /* [in] */ Float defaultValue,
        /* [out] */ Float* value);

    CARAPI GetDouble(
        /* [in] */ const String& key,
        /* [out] */ Double* value);

    /**
     * Returns the value associated with the given key, or defaultValue if
     * no mapping of the desired type exists for the given key.
     *
     * @param key a const String&
     * @return a double value
     */
    CARAPI GetDoubleEx(
        /* [in] */ const String& key,
        /* [in] */ Double defaultValue,
        /* [out] */ Double* value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return a String value, or null
     */
    CARAPI GetString(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a CharSequence value, or null
     */
    CARAPI GetCharSequence(
        /* [in] */ const String& key,
        /* [out] */ ICharSequence** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a Bundle value, or null
     */
    CARAPI GetBundle(
        /* [in] */ const String& key,
        /* [out] */ IBundle** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a Parcelable value, or null
     */
    CARAPI GetParcelable(
        /* [in] */ const String& key,
        /* [out] */ IParcelable** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a Parcelable[] value, or null
     */
    CARAPI GetParcelableArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<IParcelable*>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return an ArrayList<T> value, or null
     */
    CARAPI GetParcelableArrayList(
        /* [in] */ const String& key,
        /* [out] */ IObjectContainer** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     *
     * @return a SparseArray of T values, or null
     */
    // public <T extends Parcelable> SparseArray<T> getSparseParcelableArray(const String& key);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a Serializable value, or null
     */
    // GetSerializable(
    //     /* [in] */ const String& key,
    //     /* [out] */ ISerializable** result);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return an ArrayList<String> value, or null
     */
    CARAPI GetIntegerArrayList(
        /* [in] */ const String& key,
        /* [out] */ IObjectContainer** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return an ArrayList<String> value, or null
     */
    CARAPI GetStringArrayList(
        /* [in] */ const String& key,
        /* [out] */ IObjectContainer** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return an ArrayList<CharSequence> value, or null
     */
    CARAPI GetCharSequenceArrayList(
        /* [in] */ const String& key,
        /* [out] */ IObjectContainer** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a boolean[] value, or null
     */
    CARAPI GetBooleanArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Boolean>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a byte[] value, or null
     */
    CARAPI GetByteArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Byte>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a short[] value, or null
     */
    CARAPI GetInt16Array(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Int16>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a char[] value, or null
     */
    CARAPI GetCharArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Char32>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return an int[] value, or null
     */
    CARAPI GetInt32Array(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Int32>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a long[] value, or null
     */
    CARAPI GetInt64Array(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Int64>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a float[] value, or null
     */
    CARAPI GetFloatArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Float>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a double[] value, or null
     */
    CARAPI GetDoubleArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Double>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a const String&[] value, or null
     */
    CARAPI GetStringArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<String>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return a CharSequence[] value, or null
     */
    CARAPI GetCharSequenceArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<ICharSequence*>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a const String&, or null
     * @return an IBinder value, or null
     *
     * @deprecated
     * @hide
     */
    // @Deprecated
    CARAPI GetIBinder(
        /* [in] */ const String& key,
        /* [out] */ IBinder** value);

    // @Override
    // public synchronized const String& toString();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI ReadFromParcelInner(
        /* [in] */ IParcel *source,
        /* [in] */ Int32 length);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IBundle* bundle);

    CARAPI constructor(
        /* [in] */ IParcel* parcelledData);

    CARAPI constructor(
        /* [in] */ IParcel* parcelledData,
        /* [in] */ Int32 length);

    CARAPI constructor(
        /* [in] */ Int32 capacity);

public:
    static CARAPI ReadFromParcel(
        /* [in] */ IParcel* source,
        /* [out] */ IBundle** bundle);

    static CARAPI WriteToParcel(
        /* [in] */ IBundle* bundle,
        /* [in] */ IParcel* dest);

private:
    /**
     * If the underlying data are stored as a Parcel, unparcel them
     * using the currently assigned class loader.
     */
    /* package */ CARAPI_(void) Unparcel();

    CARAPI_(void) TypeWarning(
        /* [in] */ const String& key,
        /* [in] */ IInterface* value,
        /* [in] */ CString className);

    CARAPI_(void) ReadMapInternal(
        /* [in] */ Handle32 source,
        /* [in] */ Int32 size,
        /* [in] */ IClassLoader* classLoader);

    CARAPI_(void) WriteMapInternal(
        /* [in] */ IParcel* dest);

    CARAPI WriteValue(
        /* [in] */ IParcel* dest,
        /* [in] */ IInterface* obj);

    CARAPI_(AutoPtr<IInterface>) ReadValue(
        /* [in] */ Handle32 source);

public:
    static AutoPtr<CBundle> EMPTY;

private:
    static const CString TAG;

    // Invariant - exactly one of mMap / mParcelledData will be null
    // (except inside a call to unparcel)

    /* package */ HashMap<String, AutoPtr<IInterface> >* mMap;

    /*
     * If mParcelledData is non-null, then mMap will be null and the
     * data are stored as a Parcel containing a Bundle.  When the data
     * are unparcelled, mParcelledData willbe set to null.
     */
    /* package */ Handle32 mParcelledData;

    Boolean mHasFds;
    Boolean mFdsKnown;

    /**
     * The ClassLoader used when unparcelling data from mParcelledData.
     */
    AutoPtr<IClassLoader> mClassLoader;

    static const Int32 VAL_NULL = -1;
    static const Int32 VAL_STRING = 0;
    static const Int32 VAL_INTEGER = 1;
    static const Int32 VAL_MAP = 2;
    static const Int32 VAL_BUNDLE = 3;
    static const Int32 VAL_PARCELABLE = 4;
    static const Int32 VAL_SHORT = 5;
    static const Int32 VAL_LONG = 6;
    static const Int32 VAL_FLOAT = 7;
    static const Int32 VAL_DOUBLE = 8;
    static const Int32 VAL_BOOLEAN = 9;
    static const Int32 VAL_CHARSEQUENCE = 10;
    static const Int32 VAL_LIST  = 11;
    static const Int32 VAL_SPARSEARRAY = 12;
    static const Int32 VAL_BYTEARRAY = 13;
    static const Int32 VAL_STRINGARRAY = 14;
    static const Int32 VAL_IBINDER = 15;
    static const Int32 VAL_PARCELABLEARRAY = 16;
    static const Int32 VAL_OBJECTARRAY = 17;
    static const Int32 VAL_INTARRAY = 18;
    static const Int32 VAL_LONGARRAY = 19;
    static const Int32 VAL_BYTE = 20;
    static const Int32 VAL_SERIALIZABLE = 21;
    static const Int32 VAL_SPARSEBOOLEANARRAY = 22;
    static const Int32 VAL_BOOLEANARRAY = 23;
    static const Int32 VAL_CHARSEQUENCEARRAY = 24;
};

#endif // __CBUNDLE_H__
