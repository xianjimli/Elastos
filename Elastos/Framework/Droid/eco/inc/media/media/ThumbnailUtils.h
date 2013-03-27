
#ifndef __THUMBNAILUTILS_H__
#define __THUMBNAILUTILS_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * Thumbnail generation routines for media provider.
 */

class ThumbnailUtils 
{
public:
    /**
     * This method first examines if the thumbnail embedded in EXIF is bigger than our target
     * size. If not, then it'll create a thumbnail from original image. Due to efficiency
     * consideration, we want to let MediaThumbRequest avoid calling this method twice for
     * both kinds, so it only requests for MICRO_KIND and set saveImage to TRUE.
     *
     * This method always returns a "square thumbnail" for MICRO_KIND thumbnail.
     *
     * @param filePath the path of image file
     * @param kind could be MINI_KIND or MICRO_KIND
     * @return Bitmap
     *
     * @hide This method is only used by media framework and media provider internally.
     */
    static AutoPtr<IBitmap> CreateImageThumbnail(
        /* [in] */ String filePath, 
        /* [in] */ Int32 kind);

    /**
     * Create a video thumbnail for a video. May return NULL if the video is
     * corrupt or the format is not supported.
     *
     * @param filePath the path of video file
     * @param kind could be MINI_KIND or MICRO_KIND
     */
    static AutoPtr<IBitmap> CreateVideoThumbnail(
        /* [in] */ String filePath, 
        /* [in] */ Int32 kind);

    /**
     * Creates a centered bitmap of the desired size.
     *
     * @param source original bitmap source
     * @param width targeted width
     * @param height targeted height
     */
    static AutoPtr<IBitmap> ExtractThumbnail(
        /* [in] */ IBitmap* source, 
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height);

    /**
     * Creates a centered bitmap of the desired size.
     *
     * @param source original bitmap source
     * @param width targeted width
     * @param height targeted height
     * @param options options used during thumbnail extraction
     */
    static AutoPtr<IBitmap> ExtractThumbnail(
        /* [in] */ IBitmap* source, 
        /* [in] */ Int32 width, 
        /* [in] */ Int32 height, 
        /* [in] */ Int32 options);

private:
    /*
     * Compute the sample size as a function of minSideLength
     * and maxNumOfPixels.
     * minSideLength is used to specify that minimal width or height of a
     * bitmap.
     * maxNumOfPixels is used to specify the maximal size in pixels that is
     * tolerable in terms of memory usage.
     *
     * The function returns a sample size based on the constraints.
     * Both size and minSideLength can be passed in as IImage.UNCONSTRAINED,
     * which indicates no care of the corresponding constraint.
     * The functions prefers returning a sample size that
     * generates a smaller bitmap, unless minSideLength = IImage.UNCONSTRAINED.
     *
     * Also, the function rounds up the sample size to a power of 2 or multiple
     * of 8 because BitmapFactory only honors sample size this way.
     * For example, BitmapFactory downsamples an image by 2 even though the
     * request is 3. So we round up the sample size to avoid OOM.
     */
    static CARAPI_(Int32) ComputeSampleSize(
        /* [in] */ IBitmapFactoryOptions* options,
        /* [in] */ Int32 minSideLength, 
        /* [in] */ Int32 maxNumOfPixels);

    static CARAPI_(Int32) ComputeInitialSampleSize(
        /* [in] */ IBitmapFactoryOptions* options,
        /* [in] */ Int32 minSideLength, 
        /* [in] */ Int32 maxNumOfPixels);

    /**
     * Make a bitmap from a given Uri, minimal side length, and maximum number of pixels.
     * The image data will be read from specified pfd if it's not NULL, otherwise
     * a new input stream will be created using specified ContentResolver.
     *
     * Clients are allowed to pass their own BitmapFactory.Options used for bitmap decoding. A
     * new BitmapFactory.Options will be created if options is NULL.
     */
    static CARAPI_(AutoPtr<IBitmap>) MakeBitmap(
        /* [in] */ Int32 minSideLength, 
        /* [in] */ Int32 maxNumOfPixels,
        /* [in] */ IUri* uri, 
        /* [in] */ IContentResolver* cr, 
        /* [in] */ IParcelFileDescriptor* pfd,
        /* [in] */ IBitmapFactoryOptions* options);

    static CARAPI_(void) CloseSilently(
        /* [in] */ IParcelFileDescriptor* c);

    static CARAPI_(AutoPtr<IParcelFileDescriptor>) MakeInputStream(
        /* [in] */ IUri* uri, 
        /* [in] */ IContentResolver* cr);

    /**
     * Transform source Bitmap to targeted width and height.
     */
    static CARAPI_(AutoPtr<IBitmap>) Transform(
        /* [in] */ IMatrix* scaler,
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 targetWidth,
        /* [in] */ Int32 targetHeight,
        /* [in] */ Int32 options);

    /**
     * SizedThumbnailBitmap contains the bitmap, which is downsampled either from
     * the thumbnail in exif or the full image.
     * mThumbnailData, mThumbnailWidth and mThumbnailHeight are set together only if mThumbnail
     * is not NULL.
     *
     * The width/height of the sized bitmap may be different from mThumbnailWidth/mThumbnailHeight.
     */
    class SizedThumbnailBitmap 
    {
    public:
        ArrayOf<Byte>* mThumbnailData;
        AutoPtr<IBitmap> mBitmap;
        Int32 mThumbnailWidth;
        Int32 mThumbnailHeight;
    };

    /**
     * Creates a bitmap by either downsampling from the thumbnail in EXIF or the full image.
     * The functions returns a SizedThumbnailBitmap,
     * which contains a downsampled bitmap and the thumbnail data in EXIF if exists.
     */
    static CARAPI_(void) CreateThumbnailFromEXIF(
        /* [in] */ String filePath, 
        /* [in] */ Int32 targetSize,
        /* [in] */ Int32 maxPixels, 
        /* [in] */ SizedThumbnailBitmap* sizedThumbBitmap);

    static String TAG;// = "ThumbnailUtils";

    /* Maximum pixels size for created bitmap. */
    static const Int32 MAX_NUM_PIXELS_THUMBNAIL = 512 * 384;
    static const Int32 MAX_NUM_PIXELS_MICRO_THUMBNAIL = 128 * 128;
    static const Int32 UNCONSTRAINED = -1;

    /* Options used internally. */
    static const Int32 OPTIONS_NONE = 0x0;
    static const Int32 OPTIONS_SCALE_UP = 0x1;

public:
    /**
     * Constant used to indicate we should recycle the input in
     * {@link #extractThumbnail(Bitmap, Int32, Int32, Int32)} unless the output is the input.
     */
    static const Int32 OPTIONS_RECYCLE_INPUT = 0x2;

    /**
     * Constant used to indicate the dimension of mini thumbnail.
     * @hide Only used by media framework and media provider internally.
     */
    static const Int32 TARGET_SIZE_MINI_THUMBNAIL = 320;

    /**
     * Constant used to indicate the dimension of micro thumbnail.
     * @hide Only used by media framework and media provider internally.
     */
    static const Int32 TARGET_SIZE_MICRO_THUMBNAIL = 96;
};

#endif
