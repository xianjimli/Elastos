
#include "os/BatteryStatsImpl.h"
#include "os/SystemClock.h"

PInterface
BatteryStatsImpl::Uid::Proc::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBatteryStatsUidProc) {
        return (IBatteryStatsUidProc *)this;
    }

    return NULL;
}

UInt32
BatteryStatsImpl::Uid::Proc::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32
BatteryStatsImpl::Uid::Proc::Release()
{
    return ElRefBase::Release();
}

ECode
BatteryStatsImpl::Uid::Proc::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBatteryStatsUidProc *)this) {
        *pIID = EIID_IBatteryStatsUidProc;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

BatteryStatsImpl*
BatteryStatsImpl::Uid::Proc::GetBatteryStats()
{
    //return BatteryStatsImpl.this;
    return NULL;
}

void
BatteryStatsImpl::Uid::Proc::IncStartsLocked()
{
     mStarts++;
}

PInterface
BatteryStatsImpl::Uid::Cap::Serv::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBatteryStatsUidCapServ) {
        return (IBatteryStatsUidCapServ *)this;
    }

    return NULL;
}

UInt32
BatteryStatsImpl::Uid::Cap::Serv::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32
BatteryStatsImpl::Uid::Cap::Serv::Release()
{
    return ElRefBase::Release();
}

ECode
BatteryStatsImpl::Uid::Cap::Serv::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBatteryStatsUidCapServ *)this) {
        *pIID = EIID_IBatteryStatsUidCapServ;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

void
BatteryStatsImpl::Uid::Cap::Serv::StartLaunchedLocked()
{
//    if (!mLaunched) {
//        mLaunches++;
//        mLaunchedSince = GetBatteryUptimeLocked();
//        mLaunched = TRUE;
//    }
}

void
BatteryStatsImpl::Uid::Cap::Serv::StopLaunchedLocked()
{
//    if (mLaunched) {
//        Millisecond64 time = GetBatteryUptimeLocked() - mLaunchedSince;
//        if (time > 0) {
//            mLaunchedTime += time;
//        } else {
//            mLaunches--;
//        }
//        mLaunched = FALSE;
//    }
}

void
BatteryStatsImpl::Uid::Cap::Serv::StartRunningLocked()
{
//    if (!mRunning) {
//        mStarts++;
//        mRunningSince = GetBatteryUptimeLocked();
//        mRunning = TRUE;
//    }
}

void
BatteryStatsImpl::Uid::Cap::Serv::StopRunningLocked()
{
//    if (mRunning) {
//        Millisecond64 time = GetBatteryUptimeLocked() - mRunningSince;
//        if (time > 0) {
//            mStartTime += time;
//        } else {
//            mStarts--;
//        }
//        mRunning = FALSE;
//    }
}

BatteryStatsImpl*
BatteryStatsImpl::Uid::Cap::Serv::GetBatteryStats()
{
//    return BatteryStatsImpl.this;
    return NULL;
}

PInterface
BatteryStatsImpl::Uid::Cap::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBatteryStatsUidCap) {
        return (IBatteryStatsUidCap *)this;
    }

    return NULL;
}

UInt32
BatteryStatsImpl::Uid::Cap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32
BatteryStatsImpl::Uid::Cap::Release()
{
    return ElRefBase::Release();
}

ECode
BatteryStatsImpl::Uid::Cap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBatteryStatsUidCap *)this) {
        *pIID = EIID_IBatteryStatsUidCap;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

PInterface
BatteryStatsImpl::Uid::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBatteryStatsUid) {
        return (IBatteryStatsUid *)this;
    }

    return NULL;
}

UInt32
BatteryStatsImpl::Uid::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32
BatteryStatsImpl::Uid::Release()
{
    return ElRefBase::Release();
}

ECode
BatteryStatsImpl::Uid::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBatteryStatsUid *)this) {
        *pIID = EIID_IBatteryStatsUid;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

BatteryStatsImpl::BatteryStatsImpl()
{
}

BatteryStatsImpl::~BatteryStatsImpl()
{
}

PInterface
BatteryStatsImpl::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBatteryStats) {
        return (IBatteryStats *)this;
    }

    return NULL;
}

UInt32
BatteryStatsImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32
BatteryStatsImpl::Release()
{
    return ElRefBase::Release();
}

ECode
BatteryStatsImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBatteryStats *)this) {
        *pIID = EIID_IBatteryStats;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

void BatteryStatsImpl::Lock()
{
    mLock.Lock();
}

void BatteryStatsImpl::Unlock()
{
    mLock.Unlock();
}

Boolean BatteryStatsImpl::IsOnBattery()
{
    return mOnBattery;
}

Millisecond64 BatteryStatsImpl::GetBatteryUptimeLocked(
    /* [in] */ Millisecond64 curTime)
{
    Millisecond64 time = mTrackBatteryPastUptime;
    if (mOnBatteryInternal) {
        time += curTime - mTrackBatteryUptimeStart;
    }
    return time;
}

Millisecond64 BatteryStatsImpl::GetBatteryUptimeLocked()
{
    return GetBatteryUptime(SystemClock::GetUptimeMillis() * 1000);
}

Millisecond64 BatteryStatsImpl::GetBatteryUptime(
    /* [in] */ Millisecond64 curTime)
{
    return GetBatteryUptimeLocked(curTime);
}

void BatteryStatsImpl::RemoveUidStatsLocked(
    /* [in] */ Int32 uid)
{
//    mUidStats.remove(uid);
}

/**
 * Retrieve the statistics object for a particular process, creating
 * if needed.
 */
BatteryStatsImpl::Uid::Proc*
BatteryStatsImpl::GetProcessStatsLocked(
    /* [in] */ Int32 uid,
    /* [in] */ String name)
{
//    Uid u = getUidStatsLocked(uid);
//    return u.getProcessStatsLocked(name);
    return NULL;
}

/**
 * Retrieve the statistics object for a particular service, creating
 * if needed.
 */
BatteryStatsImpl::Uid::Cap::Serv*
BatteryStatsImpl::GetServiceStatsLocked(
    /* [in] */ Int32 uid,
    /* [in] */ String pkg,
    /* [in] */ String name)
{
//    Uid u = getUidStatsLocked(uid);
//        return u.getServiceStatsLocked(pkg, name);
    return NULL;
}
