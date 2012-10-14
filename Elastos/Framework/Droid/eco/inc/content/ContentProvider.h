
#ifndef __CONTENTPROVIDER_H__
#define __CONTENTPROVIDER_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#define __USE_MALLOC
#include "Elastos.Framework.Core.h"
#endif
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class ContentProvider
    : public ElRefBase
    , public IObject
    , public IContentProvider
{
public:
    virtual ~ContentProvider();

    virtual CARAPI Initialize();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

	CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI AttachInfo(
        /* [in] */ IContext* context,
        /* [in] */ IContentProviderInfo* providerInfo);

protected:
    virtual CARAPI OnCreate();

private:
    AutoPtr<IContext> mContext;
    Int32 mMyUid;
    String mReadPermission;
    String mWritePermission;
};

#endif //__CCONTENTPROVIDER_H__
