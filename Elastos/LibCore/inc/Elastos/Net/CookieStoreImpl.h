
#ifndef __COOKIESTOREIMPL_H__
#define __COOKIESTOREIMPL_H__

#include "cmdef.h"
#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IURI> >
{
    size_t operator()(AutoPtr<IURI> name) const
    {
        return (size_t)name.Get();
    }
};

template<> struct EqualTo<AutoPtr<IURI> >
{
    Boolean operator()(const AutoPtr<IURI>& x,
                       const AutoPtr<IURI>& y) const
    {
        return x == y;
    }
};

_ELASTOS_NAMESPACE_END

/**
 * An in-memory cookie store.
 */
class CookieStoreImpl
    : public ElRefBase
    , public ICookieStore
{
public:
    ~CookieStoreImpl();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Add(
        /* [in] */ IURI* uri,
        /* [in] */ IHttpCookie* cookie);

    CARAPI Get(
        /* [in] */ IURI* uri,
        /* [out] */ IObjectContainer** httpCookies);

    CARAPI GetCookies(
        /* [out] */ IObjectContainer** httpCookies);

    CARAPI GetURIs(
        /* [out] */ IObjectContainer** URIs);

    CARAPI Remove(
        /* [in] */ IURI* uri,
        /* [in] */ IHttpCookie* cookie,
        /* [out] */ Boolean* succeeded);

    CARAPI RemoveAll(
        /* [out] */ Boolean* succeeded);

private:
    CARAPI_(AutoPtr<IURI>) GetCookiesUri(
        /* [in] */ IURI* uri);

private:
    /** this map may have null keys! */
    HashMap<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*> mMap;

    Mutex mLock;
};

#endif //__COOKIESTOREIMPL_H__
