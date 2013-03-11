
#ifndef __CURLUTIL_H__
#define __CURLUTIL_H__

#include "_CURLUtil.h"
#include <elastos/AutoPtr.h>

CarClass(CURLUtil)
{
public:
    CARAPI GuessUrl(
        /* [in] */ const String& inUrl,
        /* [out] */ String * pOutUrl);

    CARAPI ComposeSearchUrl(
        /* [in] */ const String& inQuery,
        /* [in] */ const String& strTemplate,
        /* [in] */ const String& queryPlaceHolder,
        /* [out] */ String * pUrl);

    CARAPI Decode(
        /* [in] */ const ArrayOf<Byte> & url,
        /* [out] */ ArrayOf<Byte> * pOutUrl);

    CARAPI IsAssetUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsResourceUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsCookielessProxyUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsFileUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsAboutUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsDataUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsJavaScriptUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsHttpUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsHttpsUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsNetworkUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsContentUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsValidUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI StripAnchor(
        /* [in] */ const String& url,
        /* [out] */ String * pOutUrl);

    CARAPI GuessFileName(
        /* [in] */ const String& url,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType,
        /* [out] */ String * pName);

public:
    // to refer to bar.png under your package's asset/foo/ directory, use
    // "file:///android_asset/foo/bar.png".
    static const char* ASSET_BASE;// = "file:///android_asset/";
    // to refer to bar.png under your package's res/drawable/ directory, use
    // "file:///android_res/drawable/bar.png". Use "drawable" to refer to
    // "drawable-hdpi" directory as well.
    static const char* RESOURCE_BASE;// = "file:///android_res/";
    static const char* FILE_BASE;// = "file://";
    static const char* PROXY_BASE;// = "file:///cookieless_proxy/";

private:
    static const char* LOGTAG;  // = "webkit";

    /** Regex used to parse content-disposition headers */
    //static const AutoPtr<CPattern> CONTENT_DISPOSITION_PATTERN;   // = Pattern.compile("attachment;\\s*filename\\s*=\\s*(\"?)([^\"]*)\\1\\s*$",Pattern.CASE_INSENSITIVE);

};

#endif // __CURLUTIL_H__
