
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

    /**
    * @return True iff the url is correctly URL encoded
    */
    static CARAPI VerifyURLEncoding(
        /* [in] */ String url,
        /* [out] */ Boolean * retVal);

    /*
    * Parse the Content-Disposition HTTP Header. The format of the header
    * is defined here: http://www.w3.org/Protocols/rfc2616/rfc2616-sec19.html
    * This header provides a filename for content that is going to be
    * downloaded to the file system. We only support the attachment type.
    * Note that RFC 2616 specifies the filename value must be double-quoted.
    * Unfortunately some servers do not quote the value so to maintain
    * consistent behaviour with other browsers, we allow unquoted values too.
    */
    static CARAPI ParseContentDisposition(
        /* [in] */ String contentDisposition,
        /* [out] */ String * retVal);

private:    
    static CARAPI ParseHex(
        /* [in] */ Byte b,
        /* [out] */ Int32* retVal);

public:
    // to refer to bar.png under your package's asset/foo/ directory, use
    // "file:///android_asset/foo/bar.png".
    static const char* ASSET_BASE;  // = "file:///android_asset/";
    // to refer to bar.png under your package's res/drawable/ directory, use
    // "file:///android_res/drawable/bar.png". Use "drawable" to refer to
    // "drawable-hdpi" directory as well.
    static const char* RESOURCE_BASE;   // = "file:///android_res/";
    static const char* FILE_BASE;   // = "file://";
    static const char* PROXY_BASE;  // = "file:///cookieless_proxy/";

private:
    static const char* LOGTAG;  // = "webkit";

    /** Regex used to parse content-disposition headers */
    //static const AutoPtr<CPattern> CONTENT_DISPOSITION_PATTERN;   // = Pattern.compile("attachment;\\s*filename\\s*=\\s*(\"?)([^\"]*)\\1\\s*$",Pattern.CASE_INSENSITIVE);

};

#endif // __CURLUTIL_H__
