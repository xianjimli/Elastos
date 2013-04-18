
#include "media/CamcorderProfile.h"

CamcorderProfile::CamcorderProfile()
{

}

/**
 * Returns the camcorder profile for the first back-facing camera on the
 * device at the given quality level. If the device has no back-facing
 * camera, this returns null.
 * @param quality the target quality level for the camcorder profile
 */
AutoPtr<ICamcorderProfile> CamcorderProfile::Get(
    /* [in] */ Int32 quality)
{
    /*Int32 numberOfCameras = Camera.getNumberOfCameras();
    CameraInfo cameraInfo = new CameraInfo();
    for (Int32 i = 0; i < numberOfCameras; i++) {
        Camera.getCameraInfo(i, cameraInfo);
        if (cameraInfo.facing == CameraInfo.CAMERA_FACING_BACK) {
            return get(i, quality);
        }
    }*/
    return NULL;
}

/**
 * Returns the camcorder profile for the given camera at the given
 * quality level.
 * @param cameraId the id for the camera
 * @param quality the target quality level for the camcorder profile
 */
AutoPtr<ICamcorderProfile> CamcorderProfile::Get(
    /* [in] */ Int32 cameraId, 
    /* [in] */ Int32 quality) 
{
    if (quality < QUALITY_LOW || quality > QUALITY_HIGH) {
        //String errMessage = "Unsupported quality level: " + quality;
        //throw new IllegalArgumentException(errMessage);
    }
    return native_get_camcorder_profile(cameraId, quality);
}

//static {
//    System.loadLibrary("media_jni");
//    native_init();
//}

// Private constructor called by JNI
CamcorderProfile::CamcorderProfile(
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
    /* [in] */ Int32 audioChannels)
{

    this->duration         = duration;
    this->quality          = quality;
    this->fileFormat       = fileFormat;
    this->videoCodec       = videoCodec;
    this->videoBitRate     = videoBitRate;
    this->videoFrameRate   = videoFrameRate;
    this->videoFrameWidth  = videoWidth;
    this->videoFrameHeight = videoHeight;
    this->audioCodec       = audioCodec;
    this->audioBitRate     = audioBitRate;
    this->audioSampleRate  = audioSampleRate;
    this->audioChannels    = audioChannels;
}

// Methods implemented by JNI
void CamcorderProfile::native_init()
{
    return;
}

AutoPtr<ICamcorderProfile> CamcorderProfile::native_get_camcorder_profile(
    /* [in] */ Int32 cameraId, 
    /* [in] */ Int32 quality)
{
    return NULL;
}
