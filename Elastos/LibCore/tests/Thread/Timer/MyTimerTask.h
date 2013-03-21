
#ifndef __MyTimerTask_H__
#define __MyTimerTask_H__

#include <Elastos.Core.h>
#include <Elastos.Utility.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class MyTimerTask
     : public ElRefBase ,public ITimerTask
{
public:
    MyTimerTask();

    CARAPI_(PInterface) Probe(
       /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
       /* [in] */ IInterface *pObject,
       /* [out] */ InterfaceID *pIID);

    CARAPI Run();

    CARAPI Cancel(
            /* [out] */ Boolean * pValue);

    CARAPI ScheduledExecutionTime(
            /* [out] */ Int64 * pValue);

    CARAPI GetWhen(
            /* [out] */ Int64* when);

    CARAPI SetWhen(
            /* [in] */ Int64 when);

    CARAPI IsScheduled(
            /* [out] */ Boolean* scheduled);

    CARAPI IsCancelled(
            /* [out] */ Boolean* cancelled);

    CARAPI GetPeriod(
            /* [out] */ Int64* period);

    CARAPI SetPeriod(
            /* [in] */ Int64 period);

    CARAPI IsFixedRate(
            /* [out] */ Boolean* fixed);

    CARAPI SetFixedRate(
            /* [in] */ Boolean fixed);

    CARAPI Lock();

    CARAPI Unlock();

    CARAPI SetScheduledTime(
            /* [in] */ Int64 time);

private:
    CARAPI_(Mutex*) GetSelfLock();

private:
    Mutex mLock;
    Boolean mCancelled;
    Int64 mWhen;

    Int64 mPeriod;

    Boolean mFixedRate;

    /*
     * The time when task will be executed, or the time when task was launched
     * if this is task in progress.
     */
    Int64 mScheduledTime;
};

#endif //__MyTimerTask_H__
