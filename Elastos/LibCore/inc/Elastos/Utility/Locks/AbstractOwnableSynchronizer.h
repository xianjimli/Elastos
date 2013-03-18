#ifndef __ABSTRACTOWNABLESYNCHRONIZER_H__
#define __ABSTRACTOWNABLESYNCHRONIZER_H__

#include <elastos/Mutex.h>
#include "Elastos.Core.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class AbstractOwnableSynchronizer {
protected:
    AbstractOwnableSynchronizer();

    void SetExclusiveOwnerThread(
        /* [in] */ IThread* pThread);

    AutoPtr<IThread> GetExclusiveOwnerThread();

private:
    AutoPtr<IThread> m_ExclusiveOwnerThread;
};


#endif //__ABSTRACTOWNABLESYNCHRONIZER_H__
