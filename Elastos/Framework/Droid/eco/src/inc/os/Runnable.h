
#ifndef __RUNNABLE_H__
#define __RUNNABLE_H__

#include "ext/frameworkext.h"
#include "utils/ElRefBase.h"

class Runnable : public ElRefBase, public IRunnable
{
protected:
    Runnable() {}

    virtual ~Runnable() {}

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);
};
#endif //__RUNNABLE_H__
