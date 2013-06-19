#ifndef __CINNERRECEIVER_H__
#define __CINNERRECEIVER_H__

#include "_CInnerReceiver.h"
#include "ext/frameworkext.h"
#include "app/LoadedCap.h"


CarClass(CInnerReceiver)
{
public:
    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI PerformReceive(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& data,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI constructor(
        /* [in] */ Handle32 rd,
        /* [in] */ Boolean strong);

private:
    //WeakReference
    Handle32 mDispatcher;
};

#endif // __CINNERRECEIVER_H__
