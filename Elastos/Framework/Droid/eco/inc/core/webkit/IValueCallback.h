#ifndef __IVALUECALLBACK_H__
#define __IVALUECALLBACK_H__

/**
 * A callback interface used to returns values asynchronously
 */
template<class T>
class IValueCallback {
    /**
     * Invoked when we have the result
     */
	virtual CARAPA_(void) onReceiveValue(
		/* [in] */ T value) = 0;
};

#endif //__IVALUECALLBACK_H__