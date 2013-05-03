
#include "webkit/JsPromptResult.h"

/*package*/
JsPromptResult::JsPromptResult(
	/* [in] */ ICallbackProxy* proxy) : JsResult(proxy, FALSE)
{
}

/**
 * Handle a confirmation response from the user.
 */
CARAPI_(void) JsPromptResult::Confirm(
	/* [in] */ const String& result)
{
	mStringResult = result;
	JsResult::Confirm();
}


/*package*/ 
CARAPI_(void) JsPromptResult::GetStringResult(
	/* [out] */ String* strOut)
{
	if (strOut) {
		*strOut = mStringResult;
	}
}

//@Override
/*package*/ 
CARAPI_(void) JsPromptResult::HandleDefault()
{
	mStringResult = NULL;
	JsResult::HandleDefault();
}