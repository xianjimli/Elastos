
#ifndef __BATTERYSTATSIMPL_H__
#define __BATTERYSTATSIMPL_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class BatteryStatsImpl : public ElRefBase, public IBatteryStats
{
public:
    class Uid : public ElRefBase, public IBatteryStatsUid
    {
    public:
        class Proc : public ElRefBase, public IBatteryStatsUidProc
        {
        public:
            CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

            CARAPI_(BatteryStatsImpl*) GetBatteryStats();

            CARAPI_(void) IncStartsLocked();

        private:
            /**
             * Number of times the process has been started.
             */
            Int32 mStarts;
        };

        class Cap : public ElRefBase, public IBatteryStatsUidCap
        {
        public:
            class Serv : public ElRefBase, public IBatteryStatsUidCapServ
            {
            public:
                CARAPI_(PInterface) Probe(
                    /* [in]  */ REIID riid);

                CARAPI_(UInt32) AddRef();

                CARAPI_(UInt32) Release();

                CARAPI GetInterfaceID(
                    /* [in] */ IInterface *pObject,
                    /* [out] */ InterfaceID *pIID);

            public:
                CARAPI_(void) StartLaunchedLocked();

                CARAPI_(void) StopLaunchedLocked();

                CARAPI_(void) StartRunningLocked();

                CARAPI_(void) StopRunningLocked();

                CARAPI_(BatteryStatsImpl*) GetBatteryStats();

            private:
                /**
                 * Total time (ms in battery uptime) the service has been left started.
                 */
                Millisecond64 mStartTime;

                /**
                 * If service has been started and not yet stopped, this is
                 * when it was started.
                 */
                Millisecond64 mRunningSince;

                /**
                 * True if we are currently running.
                 */
                Boolean mRunning;

                /**
                 * Total number of times startService() has been called.
                 */
                Int32 mStarts;

                /**
                 * Total time (ms in battery uptime) the service has been left launched.
                 */
                Millisecond64 mLaunchedTime;

                /**
                 * If service has been launched and not yet exited, this is
                 * when it was launched (ms in battery uptime).
                 */
                Millisecond64 mLaunchedSince;

                /**
                 * True if we are currently launched.
                 */
                Boolean mLaunched;

                /**
                 * Total number times the service has been launched.
                 */
                Int32 mLaunches;

                /**
                 * The amount of time spent started loaded from a previous save
                 * (ms in battery uptime).
                 */
                Millisecond64 mLoadedStartTime;

                /**
                 * The number of starts loaded from a previous save.
                 */
                Int32 mLoadedStarts;

                /**
                 * The number of launches loaded from a previous save.
                 */
                Int32 mLoadedLaunches;

                /**
                 * The amount of time spent started as of the last run (ms
                 * in battery uptime).
                 */
                Millisecond64 mLastStartTime;

                /**
                 * The number of starts as of the last run.
                 */
                Int32 mLastStarts;

                /**
                 * The number of launches as of the last run.
                 */
                Int32 mLastLaunches;

                /**
                 * The amount of time spent started when last unplugged (ms
                 * in battery uptime).
                 */
                Millisecond64 mUnpluggedStartTime;

                /**
                 * The number of starts when last unplugged.
                 */
                Int32 mUnpluggedStarts;

                /**
                 * The number of launches when last unplugged.
                 */
                Int32 mUnpluggedLaunches;
            };

        public:
            CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);
        };

    public:
        CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

public:
    BatteryStatsImpl();

    ~BatteryStatsImpl();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(void) Lock();

    CARAPI_(void) Unlock();

    CARAPI_(Boolean) IsOnBattery();

    CARAPI_(Millisecond64) GetBatteryUptimeLocked(
        /* [in] */ Millisecond64 curTime);

    CARAPI_(Millisecond64) GetBatteryUptimeLocked();

    CARAPI_(Millisecond64) GetBatteryUptime(
        /* [in] */ Millisecond64 curTime);

    CARAPI_(void) RemoveUidStatsLocked(
        /* [in] */ Int32 uid);

    Uid::Proc* GetProcessStatsLocked(
        /* [in] */ Int32 uid,
        /* [in] */ const String& name);

    Uid::Cap::Serv* GetServiceStatsLocked(
        /* [in] */ Int32 uid,
        /* [in] */ const String& pkg,
        /* [in] */ const String& name);

private:
     /**
     * These provide time bases that discount the time the device is plugged
     * in to power.
     */
    Boolean mOnBattery;
    Boolean mOnBatteryInternal;
    Millisecond64 mTrackBatteryPastUptime;
    Millisecond64 mTrackBatteryUptimeStart;
    Millisecond64 mTrackBatteryPastRealtime;
    Millisecond64 mTrackBatteryRealtimeStart;

    Millisecond64 mUnpluggedBatteryUptime;
    Millisecond64 mUnpluggedBatteryRealtime;

    Mutex mLock;
};

#endif //__BATTERYSTATSIMPL_H__
