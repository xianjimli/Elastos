
#ifndef __CMIMETYPEMAP_H__
#define __CMIMETYPEMAP_H__

#include "_CMimeTypeMap.h"

CarClass(CMimeTypeMap)
{
public:
    CARAPI GetFileExtensionFromUrl(
        /* [in] */ CString inUrl,
        /* [out] */ String* outUrl);

    CARAPI HasMimeType(
        /* [in] */ CString mimeType,
        /* [out] */ Boolean* flag);

    CARAPI GetMimeTypeFromExtension(
        /* [in] */ CString extension,
        /* [out] */ String* mimeType);

    CARAPI HasExtension(
        /* [in] */ CString extension,
        /* [out] */ Boolean* flag);

    CARAPI GetExtensionFromMimeType(
        /* [in] */ CString mimeType,
        /* [out] */ String* extensionFrom);

    CARAPI GetSingleton(
        /* [out] */ IMimeTypeMap** instance);

    CARAPI constructor();

private:

    /**
     * Load an entry into the map. This does not check if the item already
     * exists, it trusts the caller!
     */
    void LoadEntry(
        /* [in] */ const char * mimeType,
        /* [in] */ const char * extension);
};

#endif // __CMIMETYPEMAP_H__
