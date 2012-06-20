
#include "server/CActivityManagerService.h"
#include "server/CPendingIntentRecord.h"

CPendingIntentRecord::CPendingIntentRecord() :
    mSent(FALSE),
    mCanceled(FALSE)
{}

ECode CPendingIntentRecord::Send(
    /* [in] */ Int32 code,
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ IIntentReceiver* finishedReceiver,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntentRecord::SendIntent(
    /* [in] */ IContext* context,
    /* [in] */ Int32 code,
    /* [in] */ IIntent* intent,
    /* [in] */ IOnFinished* onFinished,
    /* [in] */ IHandler* handler)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntentRecord::Initialize(
    /* [in] */ CActivityManagerService* owner,
    /* [in] */ Key* k,
    /* [in] */ Int32 u)
{
    return E_NOT_IMPLEMENTED;
}

Int32 CPendingIntentRecord::SendInner(
    /* [in] */ Int32 code,
    /* [in] */ IIntent* intent,
    /* [in] */ String resolvedType,
    /* [in] */ IIntentReceiver* finishedReceiver,
    /* [in] */ IBinder* resultTo,
    /* [in] */ String resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues)
{
    return 0;
}
