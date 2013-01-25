
#ifndef __CCOOKIEMANAGER_H__
#define __CCOOKIEMANAGER_H__

#include "_CCookieManager.h"

CarClass(CCookieManager)
{
public:
    CARAPI GetInstance(
        /* [out] */ ICookieManager ** ppInstance);

    CARAPI SetAcceptCookie(
        /* [in] */ Boolean accept);

    CARAPI AcceptCookie(
        /* [out] */ Boolean * pAcceptCookie);

    CARAPI SetCookie(
        /* [in] */ CString url,
        /* [in] */ CString value);

    CARAPI SetCookieEx(
        /* [in] */ IWebAddress * pUri,
        /* [in] */ CString value);

    CARAPI GetCookie(
        /* [out] */ String * pCookie,
        /* [in] */ CString url);

    CARAPI GetCookieEx(
        /* [out] */ String * pCookie,
        /* [in] */ IWebAddress * pUri);

    CARAPI RemoveSessionCookie();

    CARAPI RemoveAllCookie();

    CARAPI HasCookies(
        /* [out] */ Boolean * pFlag);

    CARAPI RemoveExpiredCookie();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCOOKIEMANAGER_H__
