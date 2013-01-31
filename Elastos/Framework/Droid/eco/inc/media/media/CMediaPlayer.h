
#ifndef __CMEDIAPLAYER_H__
#define __CMEDIAPLAYER_H__

#include "_CMediaPlayer.h"
#include "media/MediaPlayer.h"

CarClass(CMediaPlayer), public MediaPlayer
{
public:
    CARAPI NewRequest(
        /* [out] */ IParcel ** ppParcel);

    CARAPI Invoke(
        /* [in] */ IParcel * pRequest,
        /* [in] */ IParcel * pReply,
        /* [out] */ Int32 * pStatus);

    CARAPI SetDisplay(
        /* [in] */ ISurfaceHolder * pSh);

    CARAPI Create(
        /* [in] */ IContext * pContext,
        /* [in] */ IUri * pUri,
        /* [out] */ IMediaPlayer ** ppPlayer);

    CARAPI CreateEx(
        /* [in] */ IContext * pContext,
        /* [in] */ IUri * pUri,
        /* [in] */ ISurfaceHolder * pHolder,
        /* [out] */ IMediaPlayer ** ppPlayer);

    CARAPI CreateEx2(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 resid,
        /* [out] */ IMediaPlayer ** ppPlayer);

    CARAPI SetDataSource(
        /* [in] */ IContext * pContext,
        /* [in] */ IUri * pUri);

    CARAPI SetDataSourceEx2(
        /* [in] */ IFileDescriptor * pFd);

    CARAPI Start();

    CARAPI Stop();

    CARAPI Pause();

    CARAPI SetWakeMode(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 mode);

    CARAPI SetScreenOnWhilePlaying(
        /* [in] */ Boolean screenOn);

    CARAPI GetMetadata(
        /* [in] */ Boolean update_only,
        /* [in] */ Boolean apply_filter,
        /* [out] */ IMetadata ** ppMetadata);

    CARAPI ReleaseEx();

    CARAPI Reset();

    CARAPI Suspend(
        /* [out] */ Boolean * pRes);

    CARAPI Resume(
        /* [out] */ Boolean * pRes);

    CARAPI SetOnPreparedListener(
        /* [in] */ IOnPreparedListener * pListener);

    CARAPI SetOnCompletionListener(
        /* [in] */ IOnCompletionListener * pListener);

    CARAPI SetOnBufferingUpdateListener(
        /* [in] */ IOnBufferingUpdateListener * pListener);

    CARAPI SetOnSeekCompleteListener(
        /* [in] */ IOnSeekCompleteListener * pListener);

    CARAPI SetOnVideoSizeChangedListener(
        /* [in] */ IOnVideoSizeChangedListener * pListener);

    CARAPI SetOnErrorListener(
        /* [in] */ IOnErrorListener * pListener);

    CARAPI SetOnInfoListener(
        /* [in] */ IOnInfoListener * pListener);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CMEDIAPLAYER_H__
