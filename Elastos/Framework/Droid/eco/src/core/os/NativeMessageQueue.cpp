
#include "os/NativeMessageQueue.h"

NativeMessageQueue::NativeMessageQueue()
{
    mLooper = android::Looper::getForThread();
    if (mLooper == NULL) {
        mLooper = new android::Looper(false);
        android::Looper::setForThread(mLooper);
    }
}

NativeMessageQueue::~NativeMessageQueue()
{
}

void NativeMessageQueue::PollOnce(
    /* [in] */ Millisecond32 timeoutMillis)
{
    //printf("######FILE: %s, LINE: %d, mLooper: %p######\n", __FILE__, __LINE__, mLooper.get());
    mLooper->pollOnce((int)timeoutMillis);
    //printf("######FILE: %s, LINE: %d######\n", __FILE__, __LINE__);
}

void NativeMessageQueue::Wake()
{
    mLooper->wake();
}
