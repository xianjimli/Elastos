
#ifndef __CCOOKIEPOLICYHELPER_H__
#define __CCOOKIEPOLICYHELPER_H__

#include "_CCookiePolicyHelper.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

CarClass(CCookiePolicyHelper)
{
private:
    /**
     * A pre-defined policy, accepts all cookies.
     */
    class CookiePolicyAcceptAll
        : public ElRefBase
        , public ICookiePolicy
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI ShouldAccept(
            /* [in] */ IURI* uri,
            /* [in] */ IHttpCookie* cookie,
            /* [out] */ Boolean* result);
    };

    /**
     * A pre-defined policy, accepts no cookies at all.
     */
    class CookiePolicyAcceptNone
        : public ElRefBase
        , public ICookiePolicy
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI ShouldAccept(
            /* [in] */ IURI* uri,
            /* [in] */ IHttpCookie* cookie,
            /* [out] */ Boolean* result);
    };

    /**
     * A pre-defined policy, only accepts cookies from original server.
     */
    class CookiePolicyAcceptOriginalServer
        : public ElRefBase
        , public ICookiePolicy
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [out] */ InterfaceID* pIID);

        CARAPI ShouldAccept(
            /* [in] */ IURI* uri,
            /* [in] */ IHttpCookie* cookie,
            /* [out] */ Boolean* result);
    };

public:
    CARAPI GetCookiePolicy(
        /* [in] */ CookiePolicyKind kind,
        /* [out] */ ICookiePolicy** policy);

    static CARAPI_(AutoPtr<ICookiePolicy>) GetCookiePolicy(
        /* [in] */ CookiePolicyKind kind);

private:
    static AutoPtr<ICookiePolicy> ACCEPT_ALL;
    static AutoPtr<ICookiePolicy> ACCEPT_NONE;
    static AutoPtr<ICookiePolicy> ACCEPT_ORIGINAL_SERVER;
};

#endif //__CCOOKIEPOLICYHELPER_H__
