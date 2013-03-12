
#ifndef __CCONDITIONOBJECT_H__
#define __CCONDITIONOBJECT_H__

#include "_CConditionObject.h"

using namespace Elastos;

CarClass(CConditionObject)
{
public:
    CARAPI AWait();

    CARAPI AWaitUninterruptibly();

    CARAPI AWaitNanos(
        /* [in] */ Int64 nanosTimeout,
        /* [out] */ Int64 * pValue);

    CARAPI AWaitEx(
        /* [in] */ Int64 time,
        /* [in] */ ITimeUnit * pUnit,
        /* [out] */ Boolean * pValue);

    CARAPI AWaitUntil(
        /* [in] */ IDate * pDeadline,
        /* [out] */ Boolean * pValue);

    CARAPI Signal();

    CARAPI SignalAll();

    CARAPI ConditionInit();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCONDITIONOBJECT_H__
