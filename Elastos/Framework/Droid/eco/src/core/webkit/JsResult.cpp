
#include "webkit/JsResult.h"

/*package*/ 
JsResult::JsResult(
	/* [in] */ ICallbackProxy* proxy, 
	/* [in] */ Boolean defaultVal) : mProxy(proxy), mDefaultValue(defaultVal)
{
}

/**
 * Handle the result if the user cancelled the dialog.
 */
CARAPI_(void) JsResult::Cancel()
{
	mResult = FALSE;
	WakeUp();
}

/**
 * Handle a confirmation response from the user.
 */
CARAPI_(void) JsResult::Confirm()
{
	mResult = TRUE;
	WakeUp();
}

/*package*/ 
CARAPI_(Boolean) JsResult::GetResult()
{
	return mResult;
}

/*package*/ 
CARAPI_(void) JsResult::SetReady()
{
	mReady = TRUE;
    if (mTriedToNotifyBeforeReady) {
        WakeUp();
    }
}

/*package*/ 
CARAPI_(void) JsResult::HandleDefault()
{
	SetReady();
    mResult = mDefaultValue;
    WakeUp();
}

/* Wake up the WebCore thread. */
CARAPI_(void) JsResult::WakeUp()
{
	if (mReady) {
//        synchronized (mProxy)
        {
//            mProxy.notify();
        }
    } else {
        mTriedToNotifyBeforeReady = TRUE;
    }
}