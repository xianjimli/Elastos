
#ifndef __CAMCORDERPROFILE_H__
#define __CAMCORDERPROFILE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * The CamcorderProfile class is used to retrieve the
 * predefined camcorder profile settings for camcorder applications.
 * These settings are read-only.
 *
 * The compressed output from a recording session with a given
 * CamcorderProfile contains two tracks: one for auido and one for video.
 *
 * <p>Each profile specifies the following set of parameters:
 * <ul>
 * <li> The file output format
 * <li> Video codec format
 * <li> Video bit rate in bits per second
 * <li> Video frame rate in frames per second
 * <li> Video frame width and height,
 * <li> Audio codec format
 * <li> Audio bit rate in bits per second,
 * <li> Audio sample rate
 * <li> Number of audio channels for recording.
 * </ul>
 */
class CamcorderProfile
{
public:
    CamcorderProfile();

    /**
     * Returns the camcorder profile for the first back-facing camera on the
     * device at the given quality level. If the device has no back-facing
     * camera, this returns null.
     * @param quality the target quality level for the camcorder profile
     */
    static CARAPI_(AutoPtr<ICamcorderProfile>) Get(
        /* [in] */ Int32 quality);

    /**
     * Returns the camcorder profile for the given camera at the given
     * quality level.
     * @param cameraId the id for the camera
     * @param quality the target quality level for the camcorder profile
     */
    static CARAPI_(AutoPtr<ICamcorderProfile>) Get(
        /* [in] */ Int32 cameraId, 
        /* [in] */ Int32 quality);

    /*static {
        System.loadLibrary("media_jni");
        native_init();
    }*/

private:
    // Private constructor called by JNI
    CamcorderProfile(
        /* [in] */ Int32 duration,
        /* [in] */ Int32 quality,
        /* [in] */ Int32 fileFormat,
        /* [in] */ Int32 videoCodec,
        /* [in] */ Int32 videoBitRate,
        /* [in] */ Int32 videoFrameRate,
        /* [in] */ Int32 videoWidth,
        /* [in] */ Int32 videoHeight,
        /* [in] */ Int32 audioCodec,
        /* [in] */ Int32 audioBitRate,
        /* [in] */ Int32 audioSampleRate,
        /* [in] */ Int32 audioChannels);

    // Methods implemented by JNI
    static CARAPI_(void) native_init();

    static CARAPI_(AutoPtr<ICamcorderProfile>) native_get_camcorder_profile(
        /* [in] */ Int32 cameraId, 
        /* [in] */ Int32 quality);

public:
    /**
     * The output from camcorder recording sessions can have different quality levels.
     *
     * Currently, we define two quality levels: high quality and low quality.
     * A camcorder recording session with high quality level usually has higher output bit
     * rate, better video and/or audio recording quality, larger video frame
     * resolution and higher audio sampling rate, etc, than those with low quality
     * level.
     *
     * Do not change these values/ordinals without updating their counterpart
     * in include/media/MediaProfiles.h!
     */
    static const Int32 QUALITY_LOW  = 0;
    static const Int32 QUALITY_HIGH = 1;

    /**
     * Default recording duration in seconds before the session is terminated.
     * This is useful for applications like MMS has limited file size requirement.
     */
    Int32 duration;

    /**
     * The quality level of the camcorder profile
     */
    Int32 quality;

    /**
     * The file output format of the camcorder profile
     * @see android.media.MediaRecorder.OutputFormat
     */
    Int32 fileFormat;

    /**
     * The video encoder being used for the video track
     * @see android.media.MediaRecorder.VideoEncoder
     */
    Int32 videoCodec;

    /**
     * The target video output bit rate in bits per second
     */
    Int32 videoBitRate;

    /**
     * The target video frame rate in frames per second
     */
    Int32 videoFrameRate;

    /**
     * The target video frame width in pixels
     */
    Int32 videoFrameWidth;

    /**
     * The target video frame height in pixels
     */
    Int32 videoFrameHeight;

    /**
     * The audio encoder being used for the audio track.
     * @see android.media.MediaRecorder.AudioEncoder
     */
    Int32 audioCodec;

    /**
     * The target audio output bit rate in bits per second
     */
    Int32 audioBitRate;

    /**
     * The audio sampling rate used for the audio track
     */
    Int32 audioSampleRate;

    /**
     * The number of audio channels used for the audio track
     */
    Int32 audioChannels;
};

#endif
