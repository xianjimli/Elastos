
#ifndef __CPACCEPTNONE_H__
#define __CPACCEPTNONE_H__

#include "cmdef.h"
#include "Elastos.Net_server.h"
#include <Com.Kortide.Platform.h>
#include <elastos/ElRefBase.h>

class CPAcceptNone : public ElRefBase, ICookiePolicy
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI ShouldAccept(
        /* [in] */ IURI* uri,
        /* [in] */ IHttpCookie* cookie,
        /* [out] */ Boolean* result);
};

#endif //__CPACCEPTNONE_H__
