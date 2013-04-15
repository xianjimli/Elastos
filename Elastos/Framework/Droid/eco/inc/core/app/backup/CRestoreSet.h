
#ifndef  __CRESTORESET_H__
#define  __CRESTORESET_H__

#include "_CRestoreSet.h"

CarClass(CRestoreSet)
{
public:
	CRestoreSet();

	~CRestoreSet();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& dev,
        /* [in] */ Int64 token);

    CARAPI DescribeContents(
    	/* [out] */ Int32* c);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI GetToken(
        /* [out] */ Int64* token);

    CARAPI GetDevice(
        /* [out] */ String* device);

    CARAPI GetName(
        /* [out] */ String* name);

public:
    /**
     * Name of this restore set.  May be user generated, may simply be the name
     * of the handset model, e.g. "T-Mobile G1".
     */
    String mName;

    /**
     * Identifier of the device whose data this is.  This will be as unique as
     * is practically possible; for example, it might be an IMEI.
     */
    String mDevice;

    /**
     * Token that identifies this backup set unambiguously to the backup/restore
     * transport.  This is guaranteed to be valid for the duration of a restore
     * session, but is meaningless once the session has ended.
     */
    Int64 mToken;
};

#endif  //__CRESTORESET_H__
