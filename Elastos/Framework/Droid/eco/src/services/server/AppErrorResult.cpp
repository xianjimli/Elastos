
#include "server/AppErrorResult.h"

AppErrorResult::AppErrorResult()
    : mHasResult(FALSE)
    , mResult(0)
{}

void AppErrorResult::SetResult(
    /* [in] */ Int32 res)
{
    Mutex::Autolock lock(mLock);
    mHasResult = TRUE;
    mResult = res;
    //NotifyAll();
}

Int32 AppErrorResult::GetResult()
{
    // Mutex::Autolock lock(mLock);
    // while (!mHasResult) {
    //     try {
    //         wait();
    //     } catch (InterruptedException e) {
    //     }
    // }

    return mResult;
}
