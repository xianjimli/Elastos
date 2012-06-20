
#ifndef __NATIVEMESSAGEQUEUE_H__
#define __NATIVEMESSAGEQUEUE_H__

#ifdef _linux
#include <utils/Looper.h>
#endif
#include "ext/frameworkext.h"

class NativeMessageQueue
{
public:
    NativeMessageQueue();
    ~NativeMessageQueue();

#ifdef _linux
    inline android::sp<android::Looper> GetLooper()
    { return mLooper; }
#endif

    CARAPI_(void) PollOnce(
        /* [in] */ Millisecond32 timeoutMillis);

    CARAPI_(void) Wake();

private:
#ifdef _linux
    android::sp<android::Looper> mLooper;
#endif
};

#endif //__NATIVEMESSAGEQUEUE_H__
