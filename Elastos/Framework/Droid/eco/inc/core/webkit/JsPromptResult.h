#ifndef __JSPROMPTRESULT_H__
#define __JSPROMPTRESULT_H__

#include "JsResult.h"

/**
 * Public class for handling javascript prompt requests. A
 * JsDialogHandlerInterface implentation will receive a jsPrompt call with a
 * JsPromptResult parameter. This parameter is used to return a result to
 * WebView. The client can call cancel() to cancel the dialog or confirm() with
 * the user's input to confirm the dialog.
 */
class JsPromptResult : public JsResult 
{
public:

    /*package*/
	JsPromptResult(
		/* [in] */ ICallbackProxy* proxy);

public:
    /**
     * Handle a confirmation response from the user.
     */
	virtual CARAPI_(void) Confirm(
		/* [in] */ const String& result);


    /*package*/ 
	virtual CARAPI_(void) GetStringResult(
        /* [in] */ String* strOut);

    //@Override
    /*package*/ 
    virtual CARAPI_(void) HandleDefault();

private:
	// String result of the prompt
	String mStringResult;

};

#endif //__JSPROMPTRESULT_H__