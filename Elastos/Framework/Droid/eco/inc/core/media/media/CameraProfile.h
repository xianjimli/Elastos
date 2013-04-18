
#ifndef __CAMERAPROFILE_H__
#define __CAMERAPROFILE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>

/**
 * The CameraProfile class is used to retrieve the pre-defined still image
 * capture (jpeg) quality levels (0-100) used for low, medium, and high
 * quality settings in the Camera application.
 *
 */
class CameraProfile
{
public:
    /**
     * Define three quality levels for JPEG image encoding.
     */
    /*
     * Don't change the values for these constants unless getImageEncodingQualityLevels()
     * method is also changed accordingly.
     */
    static const Int32 QUALITY_LOW    = 0;
    static const Int32 QUALITY_MEDIUM = 1;
    static const Int32 QUALITY_HIGH   = 2;

    /**
     * Returns a pre-defined still image capture (jpeg) quality level
     * used for the given quality level in the Camera application for
     * the first back-facing camera on the device. If the device has no
     * back-facing camera, this returns 0.
     *
     * @param quality The target quality level
     */
    static CARAPI_(Int32) GetJpegEncodingQualityParameter(
        /* [in] */ Int32 quality);

    /**
     * Returns a pre-defined still image capture (jpeg) quality level
     * used for the given quality level in the Camera application for
     * the specified camera.
     *
     * @param cameraId The id of the camera
     * @param quality The target quality level
     */
    static CARAPI_(Int32) GetJpegEncodingQualityParameter(
        /* [in] */ Int32 cameraId, 
        /* [in] */ Int32 quality);

    /*static {
        System.loadLibrary("media_jni");
        native_init();
    }*/

private:
    /*
     * Cache the Jpeg encoding quality parameters
     */
    static HashMap<Int32, ArrayOf<Int32>* > sCache;// = new HashMap<Integer, Int32[]>();

    static CARAPI_(ArrayOf<Int32>*) GetImageEncodingQualityLevels(
        /* [in] */ Int32 cameraId);

    // Methods implemented by JNI
    static void native_init();

    static Int32 native_get_num_image_encoding_quality_levels(
        /* [in] */ Int32 cameraId);

    static Int32 native_get_image_encoding_quality_level(
        /* [in] */ Int32 cameraId, 
        /* [in] */ Int32 index);
};

#endif
