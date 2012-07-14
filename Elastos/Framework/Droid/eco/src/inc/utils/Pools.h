
#ifndef __POOLS_H__
#define __POOLS_H__

#include "utils/SynchronizedPool.h"
#include "utils/FinitePool.h"

class Pools
{
private:
    Pools() {}

public:
    static CARAPI_(AutoPtr<IPool>) AcquireSimplePool(
        /* [in] */ IPoolableManager* manager);

    static CARAPI_(AutoPtr<IPool>) AcquireFinitePool(
        /* [in] */ IPoolableManager* manager,
        /* [in] */ Int32 limit);

    static CARAPI_(AutoPtr<IPool>) AcquireSynchronizedPool(
        /* [in] */ IPool* pool);

    static CARAPI_(AutoPtr<IPool>) AcquireSynchronizedPool(
        /* [in] */ IPool* pool,
        /* [in] */ Mutex& lock);
};
#endif//__POOLS_H__
