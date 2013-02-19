
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
        /* [in] */ CString url,
        /* [out] */ String * pCookie);

    CARAPI GetCookieEx(
        /* [in] */ IWebAddress * pUri,
        /* [out] */ String * pCookie);

    CARAPI RemoveSessionCookie();

    CARAPI RemoveAllCookie();

    CARAPI HasCookies(
        /* [out] */ Boolean * pFlag);

    CARAPI RemoveExpiredCookie();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCOOKIEMANAGER_H__
