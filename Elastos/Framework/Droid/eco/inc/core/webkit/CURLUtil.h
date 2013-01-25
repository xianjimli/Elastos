
#ifndef __CURLUTIL_H__
#define __CURLUTIL_H__

#include "_CURLUtil.h"

CarClass(CURLUtil)
{
public:
    CARAPI GuessUrl(
        /* [out] */ String * pOutUrl,
        /* [in] */ CString inUrl);

    CARAPI ComposeSearchUrl(
        /* [out] */ String * pUrl,
        /* [in] */ CString inQuery,
        /* [in] */ CString strTemplate,
        /* [in] */ CString queryPlaceHolder);

    CARAPI Decode(
        /* [out] */ ArrayOf<Byte> * pOutUrl,
        /* [in] */ const ArrayOf<Byte> & url);

    CARAPI IsAssetUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsResourceUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsCookielessProxyUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsFileUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsAboutUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsDataUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsJavaScriptUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsHttpUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsHttpsUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsNetworkUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ const String& url);

    CARAPI IsContentUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI IsValidUrl(
        /* [out] */ Boolean * pFlag,
        /* [in] */ CString url);

    CARAPI StripAnchor(
        /* [out] */ String * pOutUrl,
        /* [in] */ const String& url);

    CARAPI GuessFileName(
        /* [out] */ String * pName,
        /* [in] */ CString url,
        /* [in] */ CString contentDisposition,
        /* [in] */ CString mimeType);

private:
    // TODO: Add your private member variables here.
};

#endif // __CURLUTIL_H__
