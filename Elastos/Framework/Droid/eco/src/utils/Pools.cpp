
#include "utils/Pools.h"

AutoPtr<IPool> Pools::AcquireSimplePool(
    /* [in] */ IPoolableManager* manager)
{
    return (IPool*)new FinitePool(manager);
}

AutoPtr<IPool> Pools::AcquireFinitePool(
    /* [in] */ IPoolableManager* manager,
    /* [in] */ Int32 limit)
{
    return (IPool*)new FinitePool(manager, limit);
}

AutoPtr<IPool> Pools::AcquireSynchronizedPool(
    /* [in] */ IPool* pool)
{
    return (IPool*)new SynchronizedPool(pool);
}

AutoPtr<IPool> Pools::AcquireSynchronizedPool(
    /* [in] */ IPool* pool,
    /* [in] */ Mutex& lock)
{
    return (IPool*)new SynchronizedPool(pool, lock);
}
