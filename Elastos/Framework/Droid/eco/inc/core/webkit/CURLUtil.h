
#ifndef __CURLUTIL_H__
#define __CURLUTIL_H__

#include "_CURLUtil.h"
#include <elastos/AutoPtr.h>

CarClass(CURLUtil)
{
public:
    /* public */
    /* static */
    static CARAPI_(String) GuessUrl(
        /* [in] */ const String& inUrl);

    /* public */
    /* static */
    static CARAPI_(String) ComposeSearchUrl(
        /* [in] */ const String& inQuery,
        /* [in] */ const String& strTemplate,
        /* [in] */ const String& queryPlaceHolder);

    /* public */
    /* static */
    static CARAPI Decode(
        /* [in] */ const ArrayOf<Byte> & url,
        /* [out] */ ArrayOf<Byte>** outUrl);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsAssetUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsResourceUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsCookielessProxyUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsFileUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsAboutUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsDataUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsJavaScriptUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsHttpUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsHttpsUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsNetworkUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsContentUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(Boolean) IsValidUrl(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(String) StripAnchor(
        /* [in] */ const String& url);

    /* public */
    /* static */
    static CARAPI_(String) GuessFileName(
        /* [in] */ const String& url,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType);

    /**
    * @return True iff the url is correctly URL encoded
    */
    /* package(hide) */
    static CARAPI_(Boolean) VerifyURLEncoding(
        /* [in] */ const String& url);

    /*
    * Parse the Content-Disposition HTTP Header. The format of the header
    * is defined here: http://www.w3.org/Protocols/rfc2616/rfc2616-sec19.html
    * This header provides a filename for content that is going to be
    * downloaded to the file system. We only support the attachment type.
    * Note that RFC 2616 specifies the filename value must be double-quoted.
    * Unfortunately some servers do not quote the value so to maintain
    * consistent behaviour with other browsers, we allow unquoted values too.
    */
    /* package(hide) */
    static CARAPI_(String) ParseContentDisposition(
        /* [in] */ const String& contentDisposition);

private:    
    static CARAPI_(Int32) ParseHex(
        /* [in] */ Byte b);

public:
    // to refer to bar.png under your package's asset/foo/ directory, use
    // "file:///android_asset/foo/bar.png".
    static const CString ASSET_BASE;  // = "file:///android_asset/";
    // to refer to bar.png under your package's res/drawable/ directory, use
    // "file:///android_res/drawable/bar.png". Use "drawable" to refer to
    // "drawable-hdpi" directory as well.
    static const CString RESOURCE_BASE;   // = "file:///android_res/";
    static const CString FILE_BASE;   // = "file://";
    static const CString PROXY_BASE;  // = "file:///cookieless_proxy/";

private:
    static const CString LOGTAG;  // = "webkit";

    /** Regex used to parse content-disposition headers */
    //static const AutoPtr<CPattern> CONTENT_DISPOSITION_PATTERN;   // = Pattern.compile("attachment;\\s*filename\\s*=\\s*(\"?)([^\"]*)\\1\\s*$",Pattern.CASE_INSENSITIVE);

};

#endif // __CURLUTIL_H__
