
#include "server/BroadcastRecord.h"

const Int32 BroadcastRecord::IDLE;
const Int32 BroadcastRecord::APP_RECEIVE;
const Int32 BroadcastRecord::CALL_IN_RECEIVE;
const Int32 BroadcastRecord::CALL_DONE_RECEIVE;

BroadcastRecord::BroadcastRecord(
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
    /* [in] */ Boolean initialSticky) :
    mIntent((CIntent*)intent),
    mCallerApp(callerApp),
    mCallerCapsule(callerCapsule),
    mCallingPid(callingPid),
    mOrdered(serialized),
    mSticky(sticky),
    mInitialSticky(initialSticky),
    mReceivers(receivers),
    mResultTo(resultTo)
{
    assert(mReceivers != NULL);
    mNextReceiver = mReceivers->Begin();
}

ECode BroadcastRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
