
#ifndef __CMIMETYPEMAP_H__
#define __CMIMETYPEMAP_H__

#include "_CMimeTypeMap.h"

CarClass(CMimeTypeMap)
{
public:
    CARAPI GetFileExtensionFromUrl(
        /* [out] */ String * pOutUrl,
        /* [in] */ CString inUrl);

    CARAPI HasMimeType(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString mimeType);

    CARAPI GetMimeTypeFromExtension(
        /* [out] */ String * pMimeType,
        /* [in] */ CString extension);

    CARAPI HasExtension(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString extension);

    CARAPI GetExtensionFromMimeType(
        /* [out] */ String * pExtensionFrom,
        /* [in] */ CString mimeType);

    CARAPI GetSingleton(
        /* [out] */ IMimeTypeMap ** ppInstance);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMIMETYPEMAP_H__
