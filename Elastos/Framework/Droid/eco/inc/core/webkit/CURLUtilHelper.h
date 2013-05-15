
#ifndef __CURLUTILHELPER_H__
#define __CURLUTILHELPER_H__

#include "_CURLUtilHelper.h"

CarClass(CURLUtilHelper)
{
public:
    CARAPI GuessUrl(
        /* [in] */ const String& inUrl,
        /* [out] */ String * outUrl);

    CARAPI ComposeSearchUrl(
        /* [in] */ const String& inQuery,
        /* [in] */ const String& strTemplate,
        /* [in] */ const String& queryPlaceHolder,
        /* [out] */ String * url);

    CARAPI Decode(
        /* [in] */ const ArrayOf<Byte> & url,
        /* [out, callee] */ ArrayOf<Byte> ** outUrl);

    CARAPI IsAssetUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsResourceUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsCookielessProxyUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsFileUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsAboutUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsDataUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsJavaScriptUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsHttpUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsHttpsUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsNetworkUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsContentUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI IsValidUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * flag);

    CARAPI StripAnchor(
        /* [in] */ const String& url,
        /* [out] */ String * outUrl);

    CARAPI GuessFileName(
        /* [in] */ const String& url,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType,
        /* [out] */ String * name);

private:
    // TODO: Add your private member variables here.
};

#endif // __CURLUTILHELPER_H__
