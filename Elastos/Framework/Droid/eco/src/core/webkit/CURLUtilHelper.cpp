
#include "webkit/CURLUtilHelper.h"
#include "webkit/CURLUtil.h"

ECode CURLUtilHelper::GuessUrl(
    /* [in] */ const String& inUrl,
    /* [out] */ String * outUrl)
{
    *outUrl = CURLUtil::GuessUrl(inUrl);
    return NOERROR;
}

ECode CURLUtilHelper::ComposeSearchUrl(
    /* [in] */ const String& inQuery,
    /* [in] */ const String& strTemplate,
    /* [in] */ const String& queryPlaceHolder,
    /* [out] */ String * url)
{
    *url = CURLUtil::ComposeSearchUrl(inQuery, strTemplate, queryPlaceHolder);
    return NOERROR;
}

ECode CURLUtilHelper::Decode(
    /* [in] */ const ArrayOf<Byte> & url,
    /* [out, callee] */ ArrayOf<Byte> ** outUrl)
{
    return CURLUtil::Decode(url, outUrl);
}

ECode CURLUtilHelper::IsAssetUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsAssetUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsResourceUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsResourceUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsCookielessProxyUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsCookielessProxyUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsFileUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsFileUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsAboutUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsAboutUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsDataUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsDataUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsJavaScriptUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsJavaScriptUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsHttpUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsHttpUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsHttpsUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsHttpsUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsNetworkUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsNetworkUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsContentUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsContentUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::IsValidUrl(
    /* [in] */ const String& url,
    /* [out] */ Boolean * flag)
{
    *flag = CURLUtil::IsValidUrl(url);
    return NOERROR;
}

ECode CURLUtilHelper::StripAnchor(
    /* [in] */ const String& url,
    /* [out] */ String * outUrl)
{
    *outUrl = CURLUtil::StripAnchor(url);
    return NOERROR;
}

ECode CURLUtilHelper::GuessFileName(
    /* [in] */ const String& url,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimeType,
    /* [out] */ String * name)
{
    *name = CURLUtil::GuessFileName(url, contentDisposition, mimeType);
    return NOERROR;
}

