
#ifndef __CINPUTBINDRESULT_H__
#define __CINPUTBINDRESULT_H__

#include "_CInputBindResult.h"
#include <elastos/AutoPtr.h>

CarClass(CInputBindResult)
{
public:
	CInputBindResult();

    CARAPI constructor(
    	/* [in] */ IInputMethodSessionStub* _method,
    	/* [in] */ String _id,
    	/* [in] */ Int32 _sequence);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    CARAPI constructor(
    	/* [in] */ IParcel* source);

public:
    /**
     * The input method service.
     */
    // public final IInputMethodSession method;
    AutoPtr<IInputMethodSessionStub> mMethod;

    /**
     * The ID for this input method, as found in InputMethodInfo; null if
     * no input method will be bound.
     */
    String mId;

    /**
     * Sequence number of this binding.
     */
    Int32 mSequence;
};

#endif //__CINPUTBINDRESULT_H__
