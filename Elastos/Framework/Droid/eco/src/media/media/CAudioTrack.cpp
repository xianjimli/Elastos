
#include "media/CAudioTrack.h"

ECode CAudioTrack::ReleaseEx()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetMinVolume(
    /* [out] */ Float * pMinVolume)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetMaxVolume(
    /* [out] */ Float * pMaxVolume)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetSampleRate(
    /* [out] */ Int32 * pRate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetPlaybackRate(
    /* [out] */ Int32 * pRate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetAudioFormat(
    /* [out] */ Int32 * pFormat)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetStreamType(
    /* [out] */ Int32 * pType)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetChannelConfiguration(
    /* [out] */ Int32 * pConfig)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetChannelCount(
    /* [out] */ Int32 * pCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetState(
    /* [out] */ Int32 * pState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetPlayState(
    /* [out] */ Int32 * pState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetNotificationMarkerPosition(
    /* [out] */ Int32 * pPos)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetPositionNotificationPeriod(
    /* [out] */ Int32 * pPeriod)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetPlaybackHeadPosition(
    /* [out] */ Int32 * pPosition)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetNativeOutputSampleRate(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32 * pRate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [out] */ Int32 * pSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::GetAudioSessionId(
    /* [out] */ Int32 * pId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetPlaybackPositionUpdateListener(
    /* [in] */ IOnPlaybackPositionUpdateListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetPlaybackPositionUpdateListenerEx(
    /* [in] */ IOnPlaybackPositionUpdateListener * pListener,
    /* [in] */ IHandler * pHandler)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetStereoVolume(
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz,
    /* [out] */ Int32 * pRate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetPlaybackHeadPosition(
    /* [in] */ Int32 positionInFrames,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetLoopPoints(
    /* [in] */ Int32 startInFrames,
    /* [in] */ Int32 endInFrames,
    /* [in] */ Int32 loopCount,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::Play()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::Stop()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::Pause()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::Flush()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::Write(
    /* [in] */ const ArrayOf<Byte> & audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::WriteEx(
    /* [in] */ const ArrayOf<Int16> & audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::ReloadStaticData(
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::AttachAuxEffect(
    /* [in] */ Int32 effectId,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::SetAuxEffectSendLevel(
    /* [in] */ Float level,
    /* [out] */ Int32 * pCode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::constructor(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes,
    /* [in] */ Int32 mode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioTrack::constructor(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 sessionId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

