
#ifndef __CMETADATA_H__
#define __CMETADATA_H__

#include "_CMetadata.h"
#include "media/Metadata.h"

CarClass(CMetadata), public Metadata
{
public:
    CARAPI Parse(
        /* [in] */ IParcel * pParcel,
        /* [out] */ Boolean * pRes);

    CARAPI Has(
        /* [in] */ Int32 metadataId,
        /* [out] */ Boolean * pPresent);

    CARAPI GetString(
        /* [in] */ Int32 key,
        /* [out] */ String * pStr);

    CARAPI GetInt32(
        /* [in] */ Int32 key,
        /* [out] */ Int32 * pRes);

    CARAPI GetBoolean(
        /* [in] */ Int32 key,
        /* [out] */ Boolean * pRes);

    CARAPI GetInt64(
        /* [in] */ Int32 key,
        /* [out] */ Int64 * pRes);

    CARAPI GetDouble(
        /* [in] */ Int32 key,
        /* [out] */ Double * pRes);

    CARAPI GetByteArray(
        /* [in] */ Int32 key,
        /* [out, callee] */ ArrayOf<Byte> ** ppResult);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CMETADATA_H__
