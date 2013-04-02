
#include "media/CAudioRecord.h"


ECode CAudioRecord::ReleaseResources()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetSampleRate(
    /* [out] */ Int32 * pRate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetAudioSource(
    /* [out] */ Int32 * pSource)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetAudioFormat(
    /* [out] */ Int32 * pFormat)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetChannelConfiguration(
    /* [out] */ Int32 * pConfiguration)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetChannelCount(
    /* [out] */ Int32 * pCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetState(
    /* [out] */ Int32 * pState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetRecordingState(
    /* [out] */ Int32 * pState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetNotificationMarkerPosition(
    /* [out] */ Int32 * pPosition)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetPositionNotificationPeriod(
    /* [out] */ Int32 * pPeriod)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [out] */ Int32 * pSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::StartRecording()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::Stop()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::Read(
    /* [out] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::ReadEx(
    /* [out] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::ReadEx2(
    /* [in] */ IByteBuffer* audioBuffer,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32* num)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::SetRecordPositionUpdateListener(
    /* [in] */ IOnRecordPositionUpdateListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::SetRecordPositionUpdateListenerEx(
    /* [in] */ IOnRecordPositionUpdateListener * pListener,
    /* [in] */ IApartment * pHandler)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames,
    /* [out] */ Int32 * pErr)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames,
    /* [out] */ Int32 * pErr)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioRecord::constructor(
    /* [in] */ Int32 audioSource,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

