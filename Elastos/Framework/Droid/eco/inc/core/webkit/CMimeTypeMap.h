
#ifndef __CMIMETYPEMAP_H__
#define __CMIMETYPEMAP_H__

#include "_CMimeTypeMap.h"

CarClass(CMimeTypeMap)
{
public:
    CARAPI GetFileExtensionFromUrl(
        /* [in] */ CString inUrl,
        /* [out] */ String * pOutUrl);

    CARAPI HasMimeType(
        /* [in] */ CString mimeType,
        /* [out] */ Boolean * pFlag);

    CARAPI GetMimeTypeFromExtension(
        /* [in] */ CString extension,
        /* [out] */ String * pMimeType);

    CARAPI HasExtension(
        /* [in] */ CString extension,
        /* [out] */ Boolean * pFlag);

    CARAPI GetExtensionFromMimeType(
        /* [in] */ CString mimeType,
        /* [out] */ String * pExtensionFrom);

    CARAPI GetSingleton(
        /* [out] */ IMimeTypeMap ** ppInstance);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMIMETYPEMAP_H__
