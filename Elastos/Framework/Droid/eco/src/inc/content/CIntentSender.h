
#ifndef __CINTENTSENDER_H__
#define __CINTENTSENDER_H__

#include "_CIntentSender.h"
#include <elastos/AutoPtr.h>

CarClass(CIntentSender)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor(
        /* [in] */ IIntentSender* target);

private:
    AutoPtr<IIntentSender> mTarget;
};

#endif //__CINTENTSENDER_H__
