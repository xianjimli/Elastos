
#ifndef __ELOBJECT_H__
#define __ELOBJECT_H__

#include <elastos/ElRefBase.h>

class ElObject : public ElRefBase, public IInterface
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);
};
#endif //__ELOBJECT_H__
