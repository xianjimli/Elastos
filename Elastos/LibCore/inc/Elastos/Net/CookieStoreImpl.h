
#ifndef __COOKIESTOREIMPL_H__
#define __COOKIESTOREIMPL_H__

#include "cmdef.h"
#include "Elastos.Net_server.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/List.h>
#include <elastos/Map.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class CookieStoreImpl : public ElRefBase, ICookieStore
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
    Map<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*> mMap;

    Mutex* mLock;
};

#endif //__COOKIESTOREIMPL_H__
