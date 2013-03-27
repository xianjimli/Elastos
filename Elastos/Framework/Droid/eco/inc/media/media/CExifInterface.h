
#ifndef __CEXIFINTERFACE_H__
#define __CEXIFINTERFACE_H__

#include "_CExifInterface.h"
#include "media/ExifInterface.h"

CarClass(CExifInterface), public ExifInterface
{
public:
    CARAPI GetAttribute(
        /* [in] */ const String& tag,
        /* [out] */ String * pValue);

    CARAPI GetAttributeInt32(
        /* [in] */ const String& tag,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * pValue);

    CARAPI GetAttributeDouble(
        /* [in] */ const String& tag,
        /* [in] */ Double defaultValue,
        /* [out] */ Double * pValue);

    CARAPI SetAttribute(
        /* [in] */ const String& tag,
        /* [in] */ const String& value);

    CARAPI SaveAttributes();

    CARAPI HasThumbnail(
        /* [out] */ Boolean * pThumbnail);

    CARAPI GetThumbnail(
        /* [out, callee] */ ArrayOf<Byte> ** ppData);

    CARAPI GetLatLong(
        /* [in] */ const ArrayOf<Float> & output,
        /* [out] */ Boolean * pAvailable);

    CARAPI GetAltitude(
        /* [in] */ Double defaultValue,
        /* [out] */ Double * pValue);

    CARAPI GetDateTime(
        /* [out] */ Int64 * pMilliseconds);

    CARAPI GetGpsDateTime(
        /* [out] */ Int64 * pDataTime);

    CARAPI constructor(
        /* [in] */ const String& fileName);

private:
    // TODO: Add your private member variables here.
};

#endif // __CEXIFINTERFACE_H__
