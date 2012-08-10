
#ifndef __CBUNDLE_H__
#define __CBUNDLE_H__

#include "_CBundle.h"

#include "ext/frameworkdef.h"
#include <elastos/HashMap.h>

using namespace Elastos;

CarClass(CBundle)
{
public:
    CBundle();

    ~CBundle();

    CARAPI PutBoolean(
        /* [in] */ const String& key,
        /* [in] */ Boolean value);

    CARAPI GetBoolean(
        /* [in] */ const String& key,
        /* [out] */ Boolean* value);

    CARAPI PutByte(
        /* [in] */ const String& key,
        /* [in] */ Byte value);

    CARAPI GetByte(
        /* [in] */ const String& key,
        /* [out] */ Byte* value);

    CARAPI PutChar(
        /* [in] */ const String& key,
        /* [in] */ Char16 value);

    CARAPI GetChar(
        /* [in] */ const String& key,
        /* [out] */ Char16* value);

    CARAPI PutInt16(
        /* [in] */ const String& key,
        /* [in] */ Int16 value);

    CARAPI GetInt16(
        /* [in] */ const String& key,
        /* [out] */ Int16* value);

    CARAPI PutInt32(
        /* [in] */ const String& key,
        /* [in] */ Int32 value);

    CARAPI GetInt32(
        /* [in] */ const String& key,
        /* [out] */ Int32* value);

    CARAPI PutInt64(
        /* [in] */ const String& key,
        /* [in] */ Int64 value);

    CARAPI GetInt64(
        /* [in] */ const String& key,
        /* [out] */ Int64* value);

    CARAPI PutFloat(
        /* [in] */ const String& key,
        /* [in] */ Float value);

    CARAPI GetFloat(
        /* [in] */ const String& key,
        /* [out] */ Float* value);

    CARAPI PutDouble(
        /* [in] */ const String& key,
        /* [in] */ Double value);

    CARAPI GetDouble(
        /* [in] */ const String& key,
        /* [out] */ Double* value);

    CARAPI PutString(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI GetString(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    CARAPI PutStringArray(
        /* [in] */ const String& key,
        /* [in] */ const ArrayOf<String>& value);

    CARAPI GetStringArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<String>** value);

    CARAPI PutInt32Array(
        /* [in] */ const String& key,
        /* [in] */ const ArrayOf<Int32>& value);

    CARAPI GetInt32Array(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Int32>** value);

    CARAPI HasFileDescriptors(
        /* [out] */ Boolean* hasFD);

    /**
     * Inserts a Bundle value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a String, or null
     * @param value a Bundle object, or null
     */
    CARAPI PutBundle(
        /* [in] */ const String& key,
        /* [in] */ IBundle* value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return a Bundle value, or null
     */
    CARAPI GetBundle(
        /* [in] */ const String& key,
        /* [out] */ IBundle** value);

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
        /* [in] */ Int32 length,
        /* [out] */ IBundle** bundle);

    static CARAPI WriteToParcel(
        /* [in] */ IBundle* bundle,
        /* [in] */ IParcel* dest);

private:
    class DataWrapper
    {
    public:
#define BOOLEAN_T 0x01
#define INT32_T 0x02
#define IBUNDLE_T 0x03

        DataWrapper(Int32 type, Boolean value)
        {
            mType = type;
            mValue.mBoolean = value;
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
            if (value != NULL) value->AddRef();
        }

    public:
        Int32 mType;
        union {
            Boolean mBoolean;
            Int32 mInt32;
            IBundle* mBundle;
        } mValue;
    };

private:
//#ifdef _MSC_VER
    HashMap<String, DataWrapper*> mData;
//#else
//    friend class CIntent;
//    void *m_pAndroidBundle;
//#endif
};

#endif // __CBUNDLE_H__
