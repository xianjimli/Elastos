
#include "media/CameraProfile.h"


/**
 * Returns a pre-defined still image capture (jpeg) quality level
 * used for the given quality level in the Camera application for
 * the first back-facing camera on the device. If the device has no
 * back-facing camera, this returns 0.
 *
 * @param quality The target quality level
 */
Int32 CameraProfile::GetJpegEncodingQualityParameter(
    /* [in] */ Int32 quality)
{
    /*Int32 numberOfCameras = Camera.getNumberOfCameras();
    CameraInfo cameraInfo = new CameraInfo();
    for (Int32 i = 0; i < numberOfCameras; i++) {
        Camera.getCameraInfo(i, cameraInfo);
        if (cameraInfo.facing == CameraInfo.CAMERA_FACING_BACK) {
            return getJpegEncodingQualityParameter(i, quality);
        }
    }*/
    return 0;
}

/**
 * Returns a pre-defined still image capture (jpeg) quality level
 * used for the given quality level in the Camera application for
 * the specified camera.
 *
 * @param cameraId The id of the camera
 * @param quality The target quality level
 */
Int32 CameraProfile::GetJpegEncodingQualityParameter(
    /* [in] */ Int32 cameraId, 
    /* [in] */ Int32 quality) 
{
    if (quality < QUALITY_LOW || quality > QUALITY_HIGH) {
        //throw new IllegalArgumentException("Unsupported quality level: " + quality);
    }
    //synchronized (sCache) {
        ArrayOf<Int32>* levels;// = sCache.Get(cameraId);
        if (levels == NULL) {
            levels = GetImageEncodingQualityLevels(cameraId);
            //sCache.Put(cameraId, levels);
        }
        return (*levels)[quality];
    //}
}


ArrayOf<Int32>* CameraProfile::GetImageEncodingQualityLevels(
    /* [in] */ Int32 cameraId)
{
    Int32 nLevels = native_get_num_image_encoding_quality_levels(cameraId);
    if (nLevels != QUALITY_HIGH + 1) {
        //throw new RuntimeException("Unexpected Jpeg encoding quality levels " + nLevels);
    }

    ArrayOf<Int32>* levels = ArrayOf<Int32>::Alloc(nLevels);
    for (Int32 i = 0; i < nLevels; ++i) {
        (*levels)[i] = native_get_image_encoding_quality_level(cameraId, i);
    }
    //Arrays.sort(levels);  // Lower quality level ALWAYS comes before higher one
    return levels;
}

// Methods implemented by JNI
void CameraProfile::native_init()
{

}

Int32 CameraProfile::native_get_num_image_encoding_quality_levels(
    /* [in] */ Int32 cameraId)
{
    return 0;
}

Int32 native_get_image_encoding_quality_level(
    /* [in] */ Int32 cameraId, 
    /* [in] */ Int32 index)
{
    return 0;
}

