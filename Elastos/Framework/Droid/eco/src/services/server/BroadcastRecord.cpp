
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
    mIntent(intent),
    mCallerApp(callerApp),
    mCallerCapsule(callerCapsule),
    mCallingPid(callingPid),
    mCallingUid(callingUid),
    mRequiredPermission(requiredPermission),
    mReceivers(receivers),
    mResultTo(resultTo),
    mResultCode(resultCode),
    mResultData(resultData),
    mResultExtras(resultExtras),
    mOrdered(serialized),
    mSticky(sticky),
    mInitialSticky(initialSticky),
    mState(IDLE),
    mResultAbort(FALSE),
    mCurApp(NULL)
{
    assert(mReceivers != NULL);
    mNextReceiver = mReceivers->Begin();
}

ECode BroadcastRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
