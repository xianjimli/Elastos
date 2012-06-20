
#ifndef __CCONTENTPROVIDER_H__
#define __CCONTENTPROVIDER_H__

#include <Elastos.Framework.h>
#include "CBaseObject.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class CContentProvider : public CBaseObject, public IContentProvider 
{
public:
    virtual ~CContentProvider();
    
    virtual CARAPI Initialize();
    
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

	CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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
