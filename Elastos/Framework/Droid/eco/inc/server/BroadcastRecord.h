
#ifndef __BROADCASTRECORD_H__
#define __BROADCASTRECORD_H__

#include "ext/frameworkext.h"
#include "content/CIntent.h"
#include "server/ProcessRecord.h"
#include "server/BroadcastFilter.h"
#include "content/CComponentName.h"
#include "capsule/CActivityInfo.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>


class ProcessRecord;
class BroadcastFilter;

/**
 * An active intent broadcast.
 */
class BroadcastRecord
{
public:
    BroadcastRecord(
        /* [in] */ IIntent* intent,
        /* [in] */ ProcessRecord* callerApp,
        /* [in] */ const String& callerCapsule,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ const String& requiredPermission,
        /* [in] */ List<AutoPtr<IObject> >* receivers,
        /* [in] */ IIntentReceiver* resultTo,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* resultExtras,
        /* [in] */ Boolean serialized,
        /* [in] */ Boolean sticky,
        /* [in] */ Boolean initialSticky);

public:
    CARAPI GetDescription(
        /* [out] */ String* description);

public:
    static const Int32 IDLE = 0;
    static const Int32 APP_RECEIVE = 1;
    static const Int32 CALL_IN_RECEIVE = 2;
    static const Int32 CALL_DONE_RECEIVE = 3;

    AutoPtr<CIntent> mIntent;       // the original intent that generated us
    ProcessRecord* mCallerApp;      // process that sent this
    String mCallerCapsule;          // who sent this
    Int32 mCallingPid;              // the pid of who sent this
    Int32 mCallingUid;              // the uid of who sent this
    Boolean mOrdered;               // serialize the send to receivers?
    Boolean mSticky;                // originated from existing sticky data?
    Boolean mInitialSticky;         // initial broadcast from register to sticky?
    String mRequiredPermission;     // a permission the caller has required
    List<AutoPtr<IObject> >* mReceivers;  // contains BroadcastFilter and ResolveInfo
    List<AutoPtr<IObject> >::Iterator mNextReceiver; // next receiver to be executed.
    AutoPtr<IIntentReceiver> mResultTo; // who receives final result if non-null
    Millisecond64 mDispatchTime;      // when dispatch started on this set of receivers
    Millisecond64 mReceiverTime;      // when current receiver started for timeouts.
    Millisecond64 mFinishTime;        // when we finished the broadcast.
    Int32 mResultCode;         // current result code value.
    String mResultData;      // current result data value.
    AutoPtr<IBundle> mResultExtras;    // current result extra data values.
    Boolean mResultAbort;    // current result abortBroadcast value.
    AutoPtr<IBinder> mReceiver;// who is currently running, null if none.
    Int32 mState;
    Int32 mAnrCount;           // has this broadcast record hit any ANRs?

    // The following are set when we are calling a receiver (one that
    // was found in our list of registered receivers).
    BroadcastFilter* mCurFilter;

    ProcessRecord* mCurApp;         // hosting application of current receiver.
    AutoPtr<CComponentName> mCurComponent;   // the receiver class that is currently running.
    AutoPtr<CActivityInfo> mCurReceiver;   // info about the receiver that is currently running.
};

#endif //__BROADCASTRECORD_H__
