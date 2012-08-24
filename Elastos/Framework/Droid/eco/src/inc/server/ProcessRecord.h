
#ifndef __PROCESSRECORD_H__
#define __PROCESSRECORD_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "content/CApplicationInfo.h"
#include "server/BroadcastRecord.h"
#include "server/ContentProviderRecord.h"
#include "server/CActivityRecord.h"
#include "server/CServiceRecord.h"
#include "server/ConnectionRecord.h"
#include "server/ReceiverList.h"
#include <elastos/HashMap.h>
#include <elastos/Set.h>
#include <StringBuffer.h>

using namespace Elastos;
using namespace Elastos::Core;

class BroadcastRecord;
class ContentProviderRecord;
class ConnectionRecord;
class ReceiverList;

class ProcessRecord
{
public:
    ProcessRecord(
        /* [in] */ BatteryStatsImpl::Uid::Proc* batteryStats,
        /* [in] */ IApplicationApartment* appApartment,
        /* [in] */ CApplicationInfo* info,
        /* [in] */ const String& processName);

    ~ProcessRecord();

    CARAPI_(void) SetPid(
        /* [in] */ Int32 pid);

    /*
     *  Return true if package has been added false if not
     */
    CARAPI_(Boolean) AddCapsule(
        /* [in] */ const String& cap);

    /*
     *  Delete all packages from list except the package indicated in info
     */
    CARAPI_(void) ResetCapsuleList();

    CARAPI GetShortDescription(
        /* [out] */ String* description);

    CARAPI_(void) GetShortDescription(
        /* [in] */ StringBuffer& sb);

    CARAPI GetDescription(
        /* [out] */ String* description);

public:
    AutoPtr<BatteryStatsImpl::Uid::Proc> mBatteryStats; // where to collect runtime statistics
    String mProcessName;
    AutoPtr<CApplicationInfo> mInfo;
    // List of packages running in the process
//    Set<String> mCapList;
    Int32 mPid;
    Millisecond64 mLastActivityTime;    // For managing the LRU list
    Int64 mLRUWeight;                   // Weight for ordering in LRU list
    AutoPtr<IApplicationApartment> mAppApartment;
    Int32 mMaxAdj;                  // Maximum OOM adjustment for this process
    Int32 mHiddenAdj;               // If hidden, this is the adjustment to use
    Int32 mCurRawAdj;               // Current OOM unlimited adjustment for this process
    Int32 mSetRawAdj;               // Last set OOM unlimited adjustment for this process
    Int32 mCurAdj;                  // Current OOM adjustment for this process
    Int32 mSetAdj;                  // Last set OOM adjustment for this process
    Int32 mCurSchedGroup;           // Currently desired scheduling class
    Int32 mSetSchedGroup;           // Last set to background scheduling class
    Boolean mKeeping;               // Actively running code so don't kill due to that?
    Boolean mSetIsForeground;       // Running foreground UI when last set?
    Boolean mForegroundServices;    // Running any services that are foreground?
    Boolean mBad;                   // True if disabled in the bad process list
    Boolean mKilledBackground;   // True when proc has been killed due to too many bg
    AutoPtr<IBinder> mForcingToForeground; // Token that is forcing this process to be foreground
    Int32 mAdjSeq;                 // Sequence id for identifying oom_adj assignment cycles
    Int32 mLRUSeq;                  // Sequence id for identifying LRU update cycles
    AutoPtr<IComponentName> mInstrumentationClass;// class installed to instrument app
    AutoPtr<CApplicationInfo> mInstrumentationInfo; // the application being instrumented
    String mInstrumentationProfileFile; // where to save profiling
    AutoPtr<IInstrumentationWatcher> mInstrumentationWatcher; // who is waiting
    AutoPtr<IBundle> mInstrumentationArguments; // as given to us
    BroadcastRecord* mCurReceiver; // receiver currently running in the app
    Millisecond64 mLastWakeTime;          // How long proc held wake lock at last check
    Millisecond64 mLastCpuTime;           // How long proc has run CPU at last check
    Millisecond64 mCurCpuTime;            // How long proc has run CPU most recently
    Millisecond64 mLastRequestedGc; // When we last asked the app to do a gc
    Millisecond64 mLastLowMemory;   // When we last told the app that memory is low
    Boolean mEmpty;              // Is this an empty background process?
    Boolean mHidden;             // Is this a hidden process?
    String mAdjType;             // Debugging: primary thing impacting oom_adj.
    Int32 mAdjTypeCode;            // Debugging: adj code to report to app.
    AutoPtr<IObject> mAdjSource;   // Debugging: option dependent object.
    AutoPtr<IObject> mAdjTarget;   // Debugging: target component impacting oom_adj.

    // contains CActivityRecord objects
    List<AutoPtr<CActivityRecord> > mActivities;
    // all ServiceRecord running in this process
    Set<AutoPtr<CServiceRecord> > mServices;
    // services that are currently executing code (need to remain foreground).
    Set<AutoPtr<CServiceRecord> > mExecutingServices;
    // All ConnectionRecord this process holds
    Set<ConnectionRecord*> mConnections;
    // all IIntentReceivers that are registered from this process.
    Set<ReceiverList*> mReceivers;
    // class (String) -> ContentProviderRecord
    HashMap<String, ContentProviderRecord*> mPubProviders;

    // All ContentProviderRecord process is using
    HashMap<ContentProviderRecord*, Int32> mConProviders;

    Boolean mPersistent;         // always keep this application running?
    Boolean mCrashing;           // are we in the process of crashing?
    Boolean mNotResponding;      // does the app have a not responding dialog?
    Boolean mRemoved;            // has app package been removed from device?
    Boolean mDebugging;          // was app launched for debugging?

    String mShortStringName;     // caching of toShortString() result.
    String mStringName;          // caching of toString() result.
};

#endif //__PROCESSRECORD_H__
