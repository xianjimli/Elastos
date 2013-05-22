
#ifndef __CINPUTBINDRESULT_H__
#define __CINPUTBINDRESULT_H__

#include "_CInputBindResult.h"
#include <elastos/AutoPtr.h>

CarClass(CInputBindResult)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInputMethodSession* _method,
        /* [in] */ const String& _id,
        /* [in] */ Int32 _sequence);

    CARAPI GetIIMSession(
        /* [out] */ IInputMethodSession** session);

    CARAPI GetId(
        /* [out] */ String* id);

    CARAPI GetSequence(
        /* [out] */ Int32* sequence);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

public:
    /**
     * The input method service.
     */
    AutoPtr<IInputMethodSession> mMethod;

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
