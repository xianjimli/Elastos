
#include "ext/frameworkdef.h"
#include "app/CFinishedDispatcher.h"

CFinishedDispatcher::CFinishedDispatcher()
    : mResultCode(0)
{
}

ECode CFinishedDispatcher::GetDescription(
    /* [out] */ String* description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFinishedDispatcher::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& dataStr,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky)
{
    mIntent = intent;
    mResultCode = resultCode;
    mResultData = dataStr;
    mResultExtras = extras;
    if (mHandler == NULL) {
        Run();
    }
    else {
        // TODO: ALEX need IHandler::Post
        // mHandler.Post(this);
        return E_NOT_IMPLEMENTED;
    }

    return NOERROR;
}

ECode CFinishedDispatcher::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFinishedDispatcher::Run()
{
    return mWho->OnSendFinished(
        mPendingIntent, mIntent, mResultCode, mResultData, mResultExtras);
}

ECode CFinishedDispatcher::constructor(
    /* [in] */ IPendingIntent* pi,
    /* [in] */ IOnFinished* who,
    /* [in] */ IHandler* handler)
{
    mPendingIntent = pi;
    mWho = who;
    mHandler = handler;

    return NOERROR;
}

