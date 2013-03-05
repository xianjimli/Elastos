
#ifndef __CAUDIORECORD_H__
#define __CAUDIORECORD_H__

#include "_CAudioRecord.h"
#include "media/AudioRecord.h"

CarClass(CAudioRecord), public AudioRecord
{
public:
    CARAPI ReleaseEx();

    CARAPI GetSampleRate(
        /* [out] */ Int32 * pRate);

    CARAPI GetAudioSource(
        /* [out] */ Int32 * pSource);

    CARAPI GetAudioFormat(
        /* [out] */ Int32 * pFormat);

    CARAPI GetChannelConfiguration(
        /* [out] */ Int32 * pConfiguration);

    CARAPI GetChannelCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetState(
        /* [out] */ Int32 * pState);

    CARAPI GetRecordingState(
        /* [out] */ Int32 * pState);

    CARAPI GetNotificationMarkerPosition(
        /* [out] */ Int32 * pPosition);

    CARAPI GetPositionNotificationPeriod(
        /* [out] */ Int32 * pPeriod);

    CARAPI GetMinBufferSize(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [out] */ Int32 * pSize);

    CARAPI StartRecording();

    CARAPI Stop();

    CARAPI Read(
        /* [in] */ const ArrayOf<Byte> & audioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [out] */ Int32 * pNum);

    CARAPI ReadEx(
        /* [in] */ const ArrayOf<Int16> & audioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts,
        /* [out] */ Int32 * pNum);

    CARAPI SetRecordPositionUpdateListener(
        /* [in] */ IOnRecordPositionUpdateListener * pListener);

    CARAPI SetRecordPositionUpdateListenerEx(
        /* [in] */ IOnRecordPositionUpdateListener * pListener,
        /* [in] */ IHandler * pHandler);

    CARAPI SetNotificationMarkerPosition(
        /* [in] */ Int32 markerInFrames,
        /* [out] */ Int32 * pErr);

    CARAPI SetPositionNotificationPeriod(
        /* [in] */ Int32 periodInFrames,
        /* [out] */ Int32 * pErr);

    CARAPI constructor(
        /* [in] */ Int32 audioSource,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes);

private:
    // TODO: Add your private member variables here.
};

#endif // __CAUDIORECORD_H__
