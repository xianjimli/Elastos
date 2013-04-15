
#ifndef __CCONTENTVALUES_H__
#define __CCONTENTVALUES_H__

#include "_CContentValues.h"
#include <ext/frameworkdef.h>
#include <elastos/HashMap.h>

using namespace Elastos;

class DataWrapper;

CarClass(CContentValues)
{

public:
    CContentValues();
    ~CContentValues();

public:

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI HashCode(
        /* [out] */ Int32* hashcode);

    CARAPI PutInt32(
        /* [in] */ const String& key,
        /* [in] */ Int32 value);
	
	CARAPI PutString(
		/* [in] */ const String& key,
		/* [in] */ const String& value);

	CARAPI PutAll(
	    /* [in] */ IContentValues* contentvalues);

	CARAPI PutByte(
        /* [in] */ const String& key,
        /* [in] */ Byte value);

    CARAPI PutInt16(
        /* [in] */ const String& key,
        /* [in] */ Int16 value);

    CARAPI PutInt64(
        /* [in] */ const String& key,
        /* [in] */ Int64 value);

    CARAPI PutFloat(
        /* [in] */ const String& key,
        /* [in] */ Float value);

    CARAPI PutDouble(
        /* [in] */ const String& key,
        /* [in] */ Double value);

    CARAPI PutBoolean(
        /* [in] */ const String& key,
        /* [in] */ Boolean value);

    CARAPI PutByteArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<Byte>* value);

    CARAPI PutNull(
        /* [in] */ const String& key);

    CARAPI GetSize(
            /* [out] */ Int32* size);

    CARAPI Remove(
        /* [in] */ const String& key);

    CARAPI Clear();

    CARAPI ContainsKey(
        /* [in] */ const String& key,
        /* [out] */ Boolean* result);

    CARAPI GetString(
            /* [in] */ DataWrapper* data,
            /* [out] */ String* value);

    CARAPI GetString(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    CARAPI GetInt64(
        /* [in] */ const String& key,
        /* [out] */ Int64* value);

    CARAPI GetInt32(
        /* [in] */ const String& key,
        /* [out] */ Int32* value);

    CARAPI GetInt16(
        /* [in] */ const String& key,
        /* [out] */ Int16* value);

    CARAPI GetByte(
        /* [in] */ const String& key,
        /* [out] */ Byte* value);

    CARAPI GetDouble(
        /* [in] */ const String& key,
        /* [out] */ Double* value);

    CARAPI GetFloat(
        /* [in] */ const String& key,
        /* [out] */ Float* value);

    CARAPI GetBoolean(
        /* [in] */ const String& key,
        /* [out] */ Boolean* value);

    CARAPI GetByteArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Byte> ** value);

    CARAPI ValueSet(
        /* [out] */ IObjectStringMap** values);

    CARAPI DescribeContents(
            /* [out] */ Int32* result);

    CARAPI ToString(
            /* [out] */ String* value);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
            /* [in] */ IContentValues* values);

public:

private:
    HashMap<String, DataWrapper*> mValues;
};

#endif //__CCONTENTVALUES_H__
