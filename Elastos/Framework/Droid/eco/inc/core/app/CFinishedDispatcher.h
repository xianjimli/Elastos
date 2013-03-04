
#ifndef __CFINISHEDDISPATCHER_H__
#define __CFINISHEDDISPATCHER_H__

#include "_CFinishedDispatcher.h"
#include <elastos/AutoPtr.h>

CarClass(CFinishedDispatcher)
{
public:
    CFinishedDispatcher();

public:
    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI PerformReceive(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& dataStr,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI Run();

    CARAPI constructor(
        /* [in] */ IPendingIntent* pi,
        /* [in] */ IOnFinished* who,
        /* [in] */ IHandler* handler);

private:
    AutoPtr<IPendingIntent> mPendingIntent;
    AutoPtr<IOnFinished> mWho;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IIntent> mIntent;
    Int32 mResultCode;
    String mResultData;
    AutoPtr<IBundle> mResultExtras;
};

#endif // __CFINISHEDDISPATCHER_H__
