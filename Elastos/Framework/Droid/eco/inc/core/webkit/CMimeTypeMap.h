
#ifndef __CMIMETYPEMAP_H__
#define __CMIMETYPEMAP_H__

#include "_CMimeTypeMap.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>
#include <elastos/HashMap.h>

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
     * Creates a new MIME-type map.
     */
    MimeTypeMap();

    /**
     * Load an entry into the map. This does not check if the item already
     * exists, it trusts the caller!
     */
    CARAPI_(void) LoadEntry(
        /* [in] */ const char* mimeType,
        /* [in] */ const char* extension);

    // Static method called by jni.
    static CARAPI_(void) MimeTypeFromExtension(
        /* [in] */ const String& extension,
        /* [out] */ String& str);

private:
    /**
     * Singleton MIME-type map instance:
     */
 //   static CMimeTypeMap* sMimeTypeMap;

    /**
     * MIME-type to file extension mapping:
     */
    HashMap<String, String> mMimeTypeToExtensionMap;

    /**
     * File extension to MIME type mapping:
     */
    HashMap<String, String> mExtensionToMimeTypeMap;
};

#endif // __CMIMETYPEMAP_H__
