#ifndef __JSPROMPTRESULT_H__
#define __JSPROMPTRESULT_H__

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
		/* [in] */ CallbackProxy proxy);

public:
    /**
     * Handle a confirmation response from the user.
     */
	virtual CARAPI_(void) Confirm(
		/* [in] */ CString result);


    /*package*/ 
	virtual CARAPI_(CString) GetStringResult();

    //@Override
    /*package*/ 
    virtual CARAPI_(void) HandleDefault();

private:
	// String result of the prompt
	CString mStringResult;

};

#endif //__JSPROMPTRESULT_H__