
#include "server/CActivityManagerService.h"
#include "server/ReceiverList.h"

ReceiverList::ReceiverList() :
    mCurBroadcast(NULL)
{}

ReceiverList::ReceiverList(
    /* [in] */ IActivityManager* owner,
    /* [in] */ ProcessRecord* callerApp,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ IIntentReceiver* receiver) :
    mOwner(owner),
    mApp(callerApp),
    mPid(pid),
    mUid(uid),
    mReceiver(receiver),
    mCurBroadcast(NULL)
{}
