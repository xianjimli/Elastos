#ifndef __JSRESULT_H__
#define __JSRESULT_H__

#include "ext/frameworkext.h"

class JsResult {

public:
	/*package*/ 
    JsResult(
    	/* [in] */ ICallbackProxy* proxy, 
    	/* [in] */ Boolean defaultVal);

public:
    /**
     * Handle the result if the user cancelled the dialog.
     */
	virtual CARAPI_(void) Cancel();

    /**
     * Handle a confirmation response from the user.
     */
	virtual CARAPI_(void) Confirm();


    /*package*/ 
    virtual CARAPI_(Boolean) GetResult();

    /*package*/ 
    virtual CARAPI_(void) SetReady();

    /*package*/ 
    virtual CARAPI_(void) HandleDefault();

protected:
    // This is a basic result of a confirm or prompt dialog.
	Boolean mResult;
    /**
     *  This is the caller of the prompt and is the object that is waiting.
     *  @hide
     */
	const ICallbackProxy* mProxy;

    /* Wake up the WebCore thread. */
	virtual CARAPI_(void) WakeUp();

private:
	// This prevents a user from interacting with the result before WebCore is
    // ready to handle it.
	Boolean mReady;
    // Tells us if the user tried to confirm or cancel the result before WebCore
    // is ready.
	Boolean mTriedToNotifyBeforeReady;

    // This is the default value of the result.
	const Boolean mDefaultValue;
};

#endif //__JSRESULT_H__