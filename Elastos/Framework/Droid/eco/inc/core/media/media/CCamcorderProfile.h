
#ifndef __CCAMCORDERPROFILE_H__
#define __CCAMCORDERPROFILE_H__

#include "_CCamcorderProfile.h"
#include "media/CamcorderProfile.h"

CarClass(CCamcorderProfile), public CamcorderProfile
{
public:
    /**
     * Default recording duration in seconds before the session is terminated.
     * This is useful for applications like MMS has limited file size requirement.
     */
    CARAPI GetDuration(
        /* [out] */ Int32* duration);

    /**
     * The quality level of the camcorder profile
     */
    CARAPI GetQuality(
        /* [out] */ Int32* quality);

    /**
     * The file output format of the camcorder profile
     * @see android.media.MediaRecorder.OutputFormat
     */
    CARAPI GetFileFormat(
        /* [out] */ Int32* fileFormat);

    /**
     * The video encoder being used for the video track
     * @see android.media.MediaRecorder.VideoEncoder
     */
    CARAPI GetVideoCodec(
        /* [out] */ Int32* videoCodec);

    /**
     * The target video output bit rate in bits per second
     */
    CARAPI GetVideoBitRate(
        /* [out] */ Int32* videoBitRate);

    /**
     * The target video frame rate in frames per second
     */
    CARAPI GetVideoFrameRate(
        /* [out] */ Int32* videoFrameRate);

    /**
     * The target video frame width in pixels
     */
    CARAPI GetVideoFrameWidth(
        /* [out] */ Int32* videoFrameWidth);

    /**
     * The target video frame height in pixels
     */
    CARAPI GetVideoFrameHeight(
        /* [out] */ Int32* videoFrameHeight);

    /**
     * The audio encoder being used for the audio track.
     * @see android.media.MediaRecorder.AudioEncoder
     */
    CARAPI GetAudioCodec(
        /* [out] */ Int32* audioCodec);

    /**
     * The target audio output bit rate in bits per second
     */
    CARAPI GetAudioBitRate(
        /* [out] */ Int32* audioBitRate);

    /**
     * The audio sampling rate used for the audio track
     */
    CARAPI GetAudioSampleRate(
        /* [out] */ Int32* audioSampleRate);

    /**
     * The number of audio channels used for the audio track
     */
    CARAPI GetAudioChannels(
        /* [out] */ Int32* audioChannels);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCAMCORDERPROFILE_H__
