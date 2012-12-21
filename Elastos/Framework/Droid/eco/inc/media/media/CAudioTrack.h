
#ifndef __CAUDIOTRACK_H__
#define __CAUDIOTRACK_H__

#include "_CAudioTrack.h"
#include "media/AudioTrack.h"

CarClass(CAudioTrack), public AudioTrack
{
public:
    CARAPI ReleaseEx();

    CARAPI GetMinVolume(
        /* [out] */ Float * pMinVolume);

    CARAPI GetMaxVolume(
        /* [out] */ Float * pMaxVolume);

    CARAPI GetSampleRate(
        /* [out] */ Int32 * pRate);

    CARAPI GetPlaybackRate(
        /* [out] */ Int32 * pRate);

    CARAPI GetAudioFormat(
        /* [out] */ Int32 * pFormat);

    CARAPI GetStreamType(
        /* [out] */ Int32 * pType);

    CARAPI GetChannelConfiguration(
        /* [out] */ Int32 * pConfig);

    CARAPI GetChannelCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetState(
        /* [out] */ Int32 * pState);

    CARAPI GetPlayState(
        /* [out] */ Int32 * pState);

    CARAPI GetNotificationMarkerPosition(
        /* [out] */ Int32 * pPos);

    CARAPI GetPositionNotificationPeriod(
        /* [out] */ Int32 * pPeriod);

    CARAPI GetPlaybackHeadPosition(
        /* [out] */ Int32 * pPosition);

    CARAPI GetNativeOutputSampleRate(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32 * pRate);

    CARAPI GetMinBufferSize(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [out] */ Int32 * pSize);

    CARAPI GetAudioSessionId(
        /* [out] */ Int32 * pId);

    CARAPI SetPlaybackPositionUpdateListener(
        /* [in] */ IOnPlaybackPositionUpdateListener * pListener);

    CARAPI SetPlaybackPositionUpdateListenerEx(
        /* [in] */ IOnPlaybackPositionUpdateListener * pListener,
        /* [in] */ IHandler * pHandler);

    CARAPI SetStereoVolume(
        /* [in] */ Float leftVolume,
        /* [in] */ Float rightVolume,
        /* [out] */ Int32 * pCode);

    CARAPI SetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz,
        /* [out] */ Int32 * pRate);

    CARAPI SetNotificationMarkerPosition(
        /* [in] */ Int32 markerInFrames,
        /* [out] */ Int32 * pCode);

    CARAPI SetPositionNotificationPeriod(
        /* [in] */ Int32 periodInFrames,
        /* [out] */ Int32 * pCode);

    CARAPI SetPlaybackHeadPosition(
        /* [in] */ Int32 positionInFrames,
        /* [out] */ Int32 * pCode);

    CARAPI SetLoopPoints(
        /* [in] */ Int32 startInFrames,
        /* [in] */ Int32 endInFrames,
        /* [in] */ Int32 loopCount,
        /* [out] */ Int32 * pCode);

    CARAPI Play();

    CARAPI Stop();

    CARAPI Pause();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ const ArrayOf<Byte> & audioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [out] */ Int32 * pNum);

    CARAPI WriteEx(
        /* [in] */ const ArrayOf<Int16> & audioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts,
        /* [out] */ Int32 * pNum);

    CARAPI ReloadStaticData(
        /* [out] */ Int32 * pCode);

    CARAPI AttachAuxEffect(
        /* [in] */ Int32 effectId,
        /* [out] */ Int32 * pCode);

    CARAPI SetAuxEffectSendLevel(
        /* [in] */ Float level,
        /* [out] */ Int32 * pCode);

    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes,
        /* [in] */ Int32 mode);

    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 sessionId);

private:
    // TODO: Add your private member variables here.
};

#endif // __CAUDIOTRACK_H__
