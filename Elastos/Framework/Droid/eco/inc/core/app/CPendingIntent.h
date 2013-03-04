
#ifndef __CPENDINGINTENT_H__
#define __CPENDINGINTENT_H__

#include "_CPendingIntent.h"
#include <elastos/AutoPtr.h>

CarClass(CPendingIntent)
{
public:
    CARAPI GetIntentSender(
        /* [out] */ IIntentSender** intentSender);

    CARAPI Cancel();

    CARAPI Send();

    CARAPI Send2(
        /* [in] */ Int32 code);

    CARAPI Send3(
        /* [in] */ IContext* context,
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent);

    CARAPI Send4(
        /* [in] */ Int32 code,
        /* [in] */ IOnFinished* onFinished,
        /* [in] */ IHandler* handler);

    CARAPI Send5(
        /* [in] */ IContext* context,
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent,
        /* [in] */ IOnFinished* onFinished,
        /* [in] */ IHandler* handler);

    CARAPI GetTargetCapsule(
        /* [out] */ String* targetCapsule);

    CARAPI GetTarget(
        /* [out] */ IIntentSender** target);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IIntentSender* target);

    CARAPI constructor(
        /* [in] */ IBinder* target);

private:
    AutoPtr<IIntentSender> mTarget;
};

#endif //__CPENDINGINTENT_H__
