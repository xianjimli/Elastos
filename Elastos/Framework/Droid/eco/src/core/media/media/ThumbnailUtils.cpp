
#include "media/ThumbnailUtils.h"
#include "media/MediaFile.h"
#include "media/CExifInterface.h"
#include "media/MediaMetadataRetriever.h"
#include "graphics/CMatrix.h"
#include "graphics/CCanvas.h"
#include "graphics/CRect.h"
#include "graphics/CBitmapFactoryOptions.h"
#include <elastos/Math.h>


using namespace Elastos::Core;

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
AutoPtr<IBitmap> ThumbnailUtils::CreateImageThumbnail(
    /* [in] */ String filePath,
    /* [in] */ Int32 kind)
{
    Boolean wantMini;// = (kind == Images.Thumbnails.MINI_KIND);
    Int32 targetSize = wantMini
            ? TARGET_SIZE_MINI_THUMBNAIL
            : TARGET_SIZE_MICRO_THUMBNAIL;
    Int32 maxPixels = wantMini
            ? MAX_NUM_PIXELS_THUMBNAIL
            : MAX_NUM_PIXELS_MICRO_THUMBNAIL;
    SizedThumbnailBitmap* sizedThumbnailBitmap = new SizedThumbnailBitmap();
    AutoPtr<IBitmap> bitmap = NULL;
    MediaFile::MediaFileType* fileType = MediaFile::GetFileType(filePath);
    if (fileType != NULL && fileType->fileType == MediaFile::FILE_TYPE_JPEG) {
        CreateThumbnailFromEXIF(filePath, targetSize, maxPixels, sizedThumbnailBitmap);
        bitmap = sizedThumbnailBitmap->mBitmap;
    }

    if (bitmap == NULL) {
        //try {
            /*FileDescriptor fd = new FileInputStream(filePath).getFD();
            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inSampleSize = 1;
            options.inJustDecodeBounds = TRUE;
            BitmapFactory.decodeFileDescriptor(fd, NULL, options);
            if (options.mCancel || options.outWidth == -1
                    || options.outHeight == -1) {
                return NULL;
            }
            options.inSampleSize = ComputeSampleSize(
                    options, targetSize, maxPixels);
            options.inJustDecodeBounds = FALSE;

            options.inDither = FALSE;
            options.inPreferredConfig = Bitmap.Config.ARGB_8888;
            bitmap = BitmapFactory.decodeFileDescriptor(fd, NULL, options);*/
        /*} catch (IOException ex) {
            Log.e(TAG, "", ex);
        }*/
    }

    //if (kind == Images.Thumbnails.MICRO_KIND) {
        // now we make it a "square thumbnail" for MICRO_KIND thumbnail
        bitmap = ExtractThumbnail(bitmap,
                TARGET_SIZE_MICRO_THUMBNAIL,
                TARGET_SIZE_MICRO_THUMBNAIL, OPTIONS_RECYCLE_INPUT);
    //}
    return bitmap;
}

/**
 * Create a video thumbnail for a video. May return NULL if the video is
 * corrupt or the format is not supported.
 *
 * @param filePath the path of video file
 * @param kind could be MINI_KIND or MICRO_KIND
 */
AutoPtr<IBitmap> ThumbnailUtils::CreateVideoThumbnail(
    /* [in] */ String filePath,
    /* [in] */ Int32 kind)
{
    AutoPtr<IBitmap> bitmap = NULL;
    MediaMetadataRetriever* retriever = new MediaMetadataRetriever();
    //try {
        retriever->SetDataSource(filePath);
        bitmap = retriever->GetFrameAtTime(-1);
    //} catch (IllegalArgumentException ex) {
    //    // Assume this is a corrupt video file
    //} catch (RuntimeException ex) {
    //    // Assume this is a corrupt video file.
    //} finally {
    //    try {
    //        retriever.release();
    //    } catch (RuntimeException ex) {
    //        // Ignore failures while cleaning up.
    //    }
    //}
    //if (kind == Images.Thumbnails.MICRO_KIND && bitmap != NULL) {
        bitmap = ExtractThumbnail(bitmap,
                TARGET_SIZE_MICRO_THUMBNAIL,
                TARGET_SIZE_MICRO_THUMBNAIL,
                OPTIONS_RECYCLE_INPUT);
    //}
    return bitmap;
}

/**
 * Creates a centered bitmap of the desired size.
 *
 * @param source original bitmap source
 * @param width targeted width
 * @param height targeted height
 */
AutoPtr<IBitmap> ThumbnailUtils::ExtractThumbnail(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ExtractThumbnail(source, width, height, OPTIONS_NONE);
}

/**
 * Creates a centered bitmap of the desired size.
 *
 * @param source original bitmap source
 * @param width targeted width
 * @param height targeted height
 * @param options options used during thumbnail extraction
 */
AutoPtr<IBitmap> ThumbnailUtils::ExtractThumbnail(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 options)
{
    if (source == NULL) {
        return NULL;
    }

    Float scale;

    Int32 w, h;
    source->GetWidth(&w);
    source->GetHeight(&h);

    if (w < h) {
        scale = width / (Float) w;
    } else {
        scale = height / (Float) h;
    }

    AutoPtr<IMatrix> matrix;
    CMatrix::New((IMatrix**)&matrix);

    matrix->SetScaleEx(scale, scale);
    AutoPtr<IBitmap> thumbnail = Transform(matrix, source, width, height,
            OPTIONS_SCALE_UP | options);
    return thumbnail;
}

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
Int32 ThumbnailUtils::ComputeSampleSize(
    /* [in] */ IBitmapFactoryOptions* options,
    /* [in] */ Int32 minSideLength,
    /* [in] */ Int32 maxNumOfPixels)
{
    Int32 initialSize = ComputeInitialSampleSize(options, minSideLength,
            maxNumOfPixels);

    Int32 roundedSize;
    if (initialSize <= 8 ) {
        roundedSize = 1;
        while (roundedSize < initialSize) {
            roundedSize <<= 1;
        }
    } else {
        roundedSize = (initialSize + 7) / 8 * 8;
    }

    return roundedSize;
}

Int32 ThumbnailUtils::ComputeInitialSampleSize(
    /* [in] */ IBitmapFactoryOptions* options,
    /* [in] */ Int32 minSideLength,
    /* [in] */ Int32 maxNumOfPixels)
{
    Double w;// = ((CBitmapFactoryOptions*)options)->mOutWidth;
    Double h;// = ((CBitmapFactoryOptions*)options)->mOutHeight;

    Int32 lowerBound = (maxNumOfPixels == UNCONSTRAINED) ? 1 :
        (Int32) Math::Ceil(Math::Sqrt(w * h / maxNumOfPixels));
    Int32 upperBound = (minSideLength == UNCONSTRAINED) ? 128 :
        (Int32) Math::Min(Math::Floor(w / minSideLength),
        Math::Floor(h / minSideLength));

    if (upperBound < lowerBound) {
        // return the larger one when there is no overlapping zone.
        return lowerBound;
    }

    if ((maxNumOfPixels == UNCONSTRAINED) &&
            (minSideLength == UNCONSTRAINED)) {
        return 1;
    } else if (minSideLength == UNCONSTRAINED) {
        return lowerBound;
    } else {
        return upperBound;
    }
}

/**
 * Make a bitmap from a given Uri, minimal side length, and maximum number of pixels.
 * The image data will be read from specified pfd if it's not NULL, otherwise
 * a new input stream will be created using specified ContentResolver.
 *
 * Clients are allowed to pass their own BitmapFactory.Options used for bitmap decoding. A
 * new BitmapFactory.Options will be created if options is NULL.
 */
AutoPtr<IBitmap> ThumbnailUtils::MakeBitmap(
    /* [in] */ Int32 minSideLength,
    /* [in] */ Int32 maxNumOfPixels,
    /* [in] */ IUri* uri,
    /* [in] */ IContentResolver* cr,
    /* [in] */ IParcelFileDescriptor* pfd,
    /* [in] */ IBitmapFactoryOptions* options)
{
        AutoPtr<IBitmap> b = NULL;
    //try {
        if (pfd == NULL) pfd = MakeInputStream(uri, cr);
        if (pfd == NULL) return NULL;
        if (options == NULL) CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&options);

        AutoPtr<IFileDescriptor> fd;
        pfd->GetFileDescriptor((IFileDescriptor**)&fd);
        /*((CBitmapFactoryOptions*)options)->mInSampleSize = 1;
        ((CBitmapFactoryOptions*)options)->mInJustDecodeBounds = TRUE;*/

        AutoPtr<IBitmapFactory> bf;
        //CBitmapFactory::New((IBitmapFactory**)&bf);
        //bf->DecodeFileDescriptor(fd, NULL, options);
        /*if (((CBitmapFactoryOptions*)options)->mCancel || ((CBitmapFactoryOptions*)options)->mOutWidth == -1
                || ((CBitmapFactoryOptions*)options)->mOutHeight == -1) {
            return NULL;
        }
        ((CBitmapFactoryOptions*)options)->mInSampleSize = ComputeSampleSize(
                options, minSideLength, maxNumOfPixels);
        ((CBitmapFactoryOptions*)options)->mInJustDecodeBounds = FALSE;

        ((CBitmapFactoryOptions*)options)->inDither = FALSE;
        ((CBitmapFactoryOptions*)options)->inPreferredConfig = BitmapConfig_ARGB_8888;*/
        //b = bf->DecodeFileDescriptor(fd, NULL, options);
    /*} catch (OutOfMemoryError ex) {
        Log.e(TAG, "Got oom exception ", ex);
        return NULL;
    } finally {*/
        CloseSilently(pfd);
    //}
    return b;
}

void ThumbnailUtils::CloseSilently(
    /* [in] */ IParcelFileDescriptor* c)
{
  if (c == NULL) return;
  //try {
      c->Close();
  //} catch (Throwable t) {
  //    // do nothing
  //}
}

AutoPtr<IParcelFileDescriptor> ThumbnailUtils::MakeInputStream(
    /* [in] */ IUri* uri,
    /* [in] */ IContentResolver* cr)
{
    //try {
        //return cr.openFileDescriptor(uri, "r");
    //} catch (IOException ex) {
        return NULL;
    //}
}

/**
 * Transform source Bitmap to targeted width and height.
 */
AutoPtr<IBitmap> ThumbnailUtils::Transform(
    /* [in] */ IMatrix* scaler,
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 targetWidth,
    /* [in] */ Int32 targetHeight,
    /* [in] */ Int32 options)
{
    Boolean scaleUp = (options & OPTIONS_SCALE_UP) != 0;
    Boolean recycle = (options & OPTIONS_RECYCLE_INPUT) != 0;

    Int32 w, h;
    source->GetWidth(&w);
    source->GetHeight(&h);

    Int32 deltaX = w - targetWidth;
    Int32 deltaY = h - targetHeight;
    if (!scaleUp && (deltaX < 0 || deltaY < 0)) {
        /*
        * In this case the bitmap is smaller, at least in one dimension,
        * than the target.  Transform it by placing as much of the image
        * as possible into the target and leaving the top/bottom or
        * left/right (or both) black.
        */
        AutoPtr<IBitmap> b2;
        //CBitmap::New(targetWidth, targetHeight, BitmapConfig_ARGB_8888, (IBitmap**)&b2);
        AutoPtr<ICanvas> c;
        CCanvas::New(b2, (ICanvas**)&c);

        Int32 deltaXHalf = Math::Max(0, deltaX / 2);
        Int32 deltaYHalf = Math::Max(0, deltaY / 2);

        AutoPtr<IRect> src;
        CRect::New(
        deltaXHalf,
        deltaYHalf,
        deltaXHalf + Math::Min(targetWidth, w),
        deltaYHalf + Math::Min(targetHeight, h), (IRect**)&src);

        Int32 srcW, srcH;
        src->GetWidth(&srcW);
        src->GetHeight(&srcH);
        Int32 dstX = (targetWidth  - srcW)  / 2;
        Int32 dstY = (targetHeight - srcH) / 2;
        AutoPtr<IRect> dst;
        CRect::New(
                dstX,
                dstY,
                targetWidth - dstX,
                targetHeight - dstY, (IRect**)&dst);
        //c->DrawBitmap(source, src, dst, NULL);
        if (recycle) {
            source->Recycle();
        }
        return b2;
    }
    Float bitmapWidthF = w;
    Float bitmapHeightF = h;

    Float bitmapAspect = bitmapWidthF / bitmapHeightF;
    Float viewAspect   = (Float) targetWidth / targetHeight;

    if (bitmapAspect > viewAspect) {
        Float scale = targetHeight / bitmapHeightF;
        if (scale < .9 || scale > 1) {
            //scaler->SetScale(scale, scale);
        } else {
            scaler = NULL;
        }
    } else {
        Float scale = targetWidth / bitmapWidthF;
        if (scale < .9 || scale > 1) {
            //scaler->SetScale(scale, scale);
        } else {
            scaler = NULL;
        }
    }

    AutoPtr<IBitmap> b1;
    if (scaler != NULL) {
        source->GetHeight(&h);
        // this is used for minithumb and crop, so we want to filter here.
        /*CBitmap::New(source, 0, 0,
        w, h, scaler, TRUE, (IBitmap**)&b1);*/
    } else {
        b1 = source;
    }

    if (recycle && b1.Get() != source) {
        source->Recycle();
    }

    b1->GetWidth(&w);
    b1->GetHeight(&h);

    Int32 dx1 = Math::Max(0, w - targetWidth);
    Int32 dy1 = Math::Max(0, h - targetHeight);

    AutoPtr<IBitmap> b2;
    /*CBitmap::New(
            b1,
            dx1 / 2,
            dy1 / 2,
            targetWidth,
            targetHeight, (IBitmap**)&b2);*/

    if (b2 != b1) {
        if (recycle || b1.Get() != source) {
            b1->Recycle();
        }
    }

    return b2;
}


/**
 * Creates a bitmap by either downsampling from the thumbnail in EXIF or the full image.
 * The functions returns a SizedThumbnailBitmap,
 * which contains a downsampled bitmap and the thumbnail data in EXIF if exists.
 */
void ThumbnailUtils::CreateThumbnailFromEXIF(
    /* [in] */ String filePath,
    /* [in] */ Int32 targetSize,
    /* [in] */ Int32 maxPixels,
    /* [in] */ SizedThumbnailBitmap* sizedThumbBitmap)
{
    if (filePath == NULL) return;

    AutoPtr<IExifInterface> exif = NULL;
    ArrayOf<Byte>* thumbData = NULL;
    //try {
    CExifInterface::New(filePath, (IExifInterface**)&exif);
        if (exif != NULL) {
            exif->GetThumbnail(&thumbData);
        }
    /*} catch (IOException ex) {
        Log.w(TAG, ex);
    }*/

    AutoPtr<IBitmapFactoryOptions> fullOptions;
    CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&fullOptions);
    AutoPtr<IBitmapFactoryOptions> exifOptions;
    CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&exifOptions);
    Int32 exifThumbWidth = 0;
    Int32 fullThumbWidth = 0;

    AutoPtr<IBitmapFactory> bf;
    //CBitmapFactory::New((IBitmapFactory**)&bf);

    // Compute exifThumbWidth.
    if (thumbData != NULL) {
        //((CBitmapFactoryOptions*)exifOptions)->inJustDecodeBounds = TRUE;


        //bf->DecodeByteArray(thumbData, 0, thumbData->GetLength(), exifOptions);
        /*((CBitmapFactoryOptions*)exifOptions)->inSampleSize = ComputeSampleSize(exifOptions, targetSize, maxPixels);
        exifThumbWidth = ((CBitmapFactoryOptions*)exifOptions)->outWidth / ((CBitmapFactoryOptions*)exifOptions)->inSampleSize;*/
    }

    // Compute fullThumbWidth.
    //((CBitmapFactoryOptions*)fullOptions)->inJustDecodeBounds = TRUE;
    //bf->DecodeFile(filePath, fullOptions);
    //fullOptions->inSampleSize = ComputeSampleSize(fullOptions, targetSize, maxPixels);
    //fullThumbWidth = ((CBitmapFactoryOptions*)fullOptions)->outWidth / ((CBitmapFactoryOptions*)fullOptions)->inSampleSize;

    // Choose the larger thumbnail as the returning sizedThumbBitmap.
    if (thumbData != NULL && exifThumbWidth >= fullThumbWidth) {
        Int32 width;// = ((CBitmapFactoryOptions*)exifOptions)->outWidth;
        Int32 height;// = ((CBitmapFactoryOptions*)exifOptions)->outHeight;
        //((CBitmapFactoryOptions*)exifOptions)->inJustDecodeBounds = FALSE;
        /*bf->DecodeByteArray(thumbData, 0,
                thumbData->GetLength(), exifOptions, &sizedThumbBitmap->mBitmap);*/
        if (sizedThumbBitmap->mBitmap != NULL) {
            sizedThumbBitmap->mThumbnailData = thumbData;
            sizedThumbBitmap->mThumbnailWidth = width;
            sizedThumbBitmap->mThumbnailHeight = height;
        }
    } else {
        //((CBitmapFactoryOptions*)fullOptions)->inJustDecodeBounds = FALSE;
        //bf->DecodeFile(filePath, fullOptions, &sizedThumbBitmap->mBitmap);
    }
}

