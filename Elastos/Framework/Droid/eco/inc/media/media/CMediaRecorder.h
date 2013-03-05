
#ifndef __CMEDIARECORDER_H__
#define __CMEDIARECORDER_H__

#include "_CMediaRecorder.h"

#include "media/MediaRecorder.h"

CarClass(CMediaRecorder), public MediaRecorder
{
public:
    CARAPI SetPreviewDisplay(
        /* [in] */ ISurface * pSv);

    CARAPI GetAudioSourceMax(
        /* [out] */ Int32 * pMaximun);

    CARAPI SetProfile(
        /* [in] */ ICamcorderProfile * pProfile);

    CARAPI SetOrientationHint(
        /* [in] */ Int32 degrees);

    CARAPI SetAudioSamplingRate(
        /* [in] */ Int32 samplingRate);

    CARAPI SetAudioChannels(
        /* [in] */ Int32 numChannels);

    CARAPI SetAudioEncodingBitRate(
        /* [in] */ Int32 bitRate);

    CARAPI SetVideoEncodingBitRate(
        /* [in] */ Int32 bitRate);

    CARAPI SetOutputFile(
        /* [in] */ IFileDescriptor * pFd);

    CARAPI SetOutputFileEx(
        /* [in] */ const String& path);

    CARAPI Prepare();

    CARAPI Reset();

    CARAPI SetOnErrorListener(
        /* [in] */ IMediaRecorderOnErrorListener * pL);

    CARAPI SetOnInfoListener(
        /* [in] */ IMediaRecorderOnInfoListener * pListener);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CMEDIARECORDER_H__
