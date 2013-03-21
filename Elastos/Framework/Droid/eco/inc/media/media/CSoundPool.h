
#ifndef __CSOUNDPOOL_H__
#define __CSOUNDPOOL_H__

#include "_CSoundPool.h"

#include "media/SoundPool.h"

CarClass(CSoundPool), public SoundPool
{
public:
    CARAPI Load(
        /* [in] */ const String& path,
        /* [in] */ Int32 priority,
        /* [out] */ Int32 * pID);

    CARAPI LoadEx(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 resId,
        /* [in] */ Int32 priority,
        /* [out] */ Int32 * pID);

    CARAPI LoadEx2(
        /* [in] */ IAssetFileDescriptor * pAfd,
        /* [in] */ Int32 priority,
        /* [out] */ Int32 * pID);

    CARAPI LoadEx3(
        /* [in] */ IFileDescriptor * pFd,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 length,
        /* [in] */ Int32 priority,
        /* [out] */ Int32 * pID);

    CARAPI SetOnLoadCompleteListener(
        /* [in] */ IOnLoadCompleteListener * pListener);

    CARAPI constructor(
        /* [in] */ Int32 maxStreams,
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 srcQuality);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSOUNDPOOL_H__
