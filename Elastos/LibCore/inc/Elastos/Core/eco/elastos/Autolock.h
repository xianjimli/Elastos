
#ifndef __AUTOLOCK_H__
#define __AUTOLOCK_H__


#ifdef ELASTOS_CORE_ECO
#include "Elastos.Core_server.h"
#else
#include <Elastos.Core.h>
#endif
#include <elastos/AutoPtr.h>

_ELASTOS_NAMESPACE_BEGIN


class Autolock
{
public:
    Autolock(
        /* [in] */ IInterface* obj)
    {
        assert(obj != NULL);
        mSyncObj = ISynchronize::Probe(obj);
        assert(mSyncObj != NULL);
        assert(SUCCEEDED(mSyncObj->Lock()));
    }

    ~Autolock()
    {
        assert(mSyncObj != NULL);
        assert(SUCCEEDED(mSyncObj->Unlock()));
    }

private:
    AutoPtr<ISynchronize> mSyncObj;
};

_ELASTOS_NAMESPACE_END

#endif //__AUTOLOCK_H__
