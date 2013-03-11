#ifndef __IVALUECALLBACK_H__
#define __IVALUECALLBACK_H__

/**
 * A callback interface used to returns values asynchronously
 */
template<typename T>
class ValueCallback {
    /**
     * Invoked when we have the result
     */
	virtual CARAPI_(void) OnReceiveValue(
		/* [in] */ T value) = 0;
};

#endif //__IVALUECALLBACK_H__