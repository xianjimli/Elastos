
#ifndef __COBJECT_H__
#define __COBJECT_H__

#include <elastos.h>

using namespace Elastos;

class CBaseObject : public IObject
{
public:
    CBaseObject();

    virtual ~CBaseObject();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

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

protected:
    Int32 mRef;
};

#endif //__COBJECT_H__
