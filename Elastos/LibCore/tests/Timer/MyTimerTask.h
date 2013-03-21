
#ifndef __MyTimerTask_H__
#define __MyTimerTask_H__

#include <Elastos.Core.h>
#include <Elastos.Utility.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>
#include <elastos/TimerTask.h>

using namespace Elastos::Core::Threading;

class MyTimerTask
     : public ElRefBase
     , public TimerTask
     , public ITimerTask
{
public:
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
    static Int32 sCount;
    Mutex mLock;
};

#endif //__MyTimerTask_H__
