
#ifndef __CASYNCPLAYER_H__
#define __CASYNCPLAYER_H__

#include "_CAsyncPlayer.h"
#include "media/AsyncPlayer.h"

CarClass(CAsyncPlayer), public AsyncPlayer
{
public:
    CARAPI Play(
        /* [in] */ IContext * pContext,
        /* [in] */ IUri * pUri,
        /* [in] */ Boolean looping,
        /* [in] */ Int32 stream);

    CARAPI Stop();

    CARAPI SetUsesWakeLock(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ const String& tag);

private:
    // TODO: Add your private member variables here.
};

#endif // __CASYNCPLAYER_H__
