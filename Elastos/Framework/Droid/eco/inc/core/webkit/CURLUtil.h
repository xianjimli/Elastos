
#ifndef __CURLUTIL_H__
#define __CURLUTIL_H__

#include "_CURLUtil.h"

CarClass(CURLUtil)
{
public:
    CARAPI GuessUrl(
        /* [in] */ CString inUrl,
        /* [out] */ String * pOutUrl);

    CARAPI ComposeSearchUrl(
        /* [in] */ CString inQuery,
        /* [in] */ CString strTemplate,
        /* [in] */ CString queryPlaceHolder,
        /* [out] */ String * pUrl);

    CARAPI Decode(
        /* [in] */ const ArrayOf<Byte> & url,
        /* [out] */ ArrayOf<Byte> * pOutUrl);

    CARAPI IsAssetUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsResourceUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsCookielessProxyUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsFileUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsAboutUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsDataUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsJavaScriptUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsHttpUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsHttpsUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsNetworkUrl(
        /* [in] */ const String& url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsContentUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI IsValidUrl(
        /* [in] */ CString url,
        /* [out] */ Boolean * pFlag);

    CARAPI StripAnchor(
        /* [in] */ const String& url,
        /* [out] */ String * pOutUrl);

    CARAPI GuessFileName(
        /* [in] */ CString url,
        /* [in] */ CString contentDisposition,
        /* [in] */ CString mimeType,
        /* [out] */ String * pName);

private:
    // TODO: Add your private member variables here.
};

#endif // __CURLUTIL_H__
