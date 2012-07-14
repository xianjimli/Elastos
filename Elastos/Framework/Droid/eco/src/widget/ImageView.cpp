
#include "widget/ImageView.h"
#include "graphics/CMatrix.h"
#include "graphics/CBitmapDrawable.h"
#include "graphics/Drawable.h"
#include "graphics/CCanvas.h"
#include "graphics/CPorterDuffColorFilter.h"
#include <Logger.h>
#include <StringBuffer.h>
#include <elastos/Math.h>
#include <stdio.h>
#include <graphics/CPaint.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const ImageViewScaleType ImageView::sScaleTypeArray[] = {
    ImageViewScaleType_MATRIX,
    ImageViewScaleType_FIT_XY,
    ImageViewScaleType_FIT_START,
    ImageViewScaleType_FIT_CENTER,
    ImageViewScaleType_FIT_END,
    ImageViewScaleType_CENTER,
    ImageViewScaleType_CENTER_CROP,
    ImageViewScaleType_CENTER_INSIDE
};

ImageView::ImageView() :
    mResource(0),
    mHaveFrame(FALSE),
    mAdjustViewBounds(FALSE),
    mMaxWidth(Math::INT32_MAX_VALUE),
    mMaxHeight(Math::INT32_MAX_VALUE),
    mAlpha(255),
    mViewAlphaScale(256),
    mColorMod(FALSE),
    mState(NULL),
    mMergeState(FALSE),
    mLevel(0),
    mBaselineAligned(FALSE)
{
    assert(SUCCEEDED(CRectF::NewByFriend((CRectF**)&mTempSrc)));
    assert(SUCCEEDED(CRectF::NewByFriend((CRectF**)&mTempDst)));
}

ImageView::ImageView(
    /* [in] */ IContext* context)
{
    Init(context);
}

ImageView::ImageView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

ImageView::ImageView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Init(context, attrs, defStyle);
}

void ImageView::InitImageView()
{
    assert(SUCCEEDED(CMatrix::New((IMatrix**)&mMatrix)));
    mScaleType = ImageViewScaleType_FIT_CENTER;
}

Boolean ImageView::VerifyDrawable(
    /* [in] */ IDrawable* dr)
{
    return mDrawable.Get() == dr || View::VerifyDrawable(dr);
}

ECode ImageView::InvalidateDrawable(
    /* [in] */ IDrawable* dr)
{
    if (dr == mDrawable) {
        /* we invalidate the whole view in this case because it's very
         * hard to know where the drawable actually is. This is made
         * complicated because of the offsets and transformations that
         * can be applied. In theory we could get the drawable's bounds
         * and run them through the transformation and offsets, but this
         * is probably not worth the effort.
         */
        Invalidate();
        return NOERROR;
    } else {
        return View::InvalidateDrawable(dr);
    }
}

Boolean ImageView::OnSetAlpha(
    /* [in] */ Int32 alpha)
{
    if (GetBackground() == NULL) {
        Int32 scale = alpha + (alpha >> 7);
        if (mViewAlphaScale != scale) {
            mViewAlphaScale = scale;
            mColorMod = TRUE;
            ApplyColorMod();
        }
        return TRUE;
    }
    return FALSE;
}

/**
 * Set this to true if you want the ImageView to adjust its bounds
 * to preserve the aspect ratio of its drawable.
 * @param adjustViewBounds Whether to adjust the bounds of this view
 * to presrve the original aspect ratio of the drawable
 *
 * @attr ref android.R.styleable#ImageView_adjustViewBounds
 */
ECode ImageView::SetAdjustViewBounds(
    /* [in] */ Boolean adjustViewBounds)
{
    mAdjustViewBounds = adjustViewBounds;
    if (adjustViewBounds) {
        SetScaleType(ImageViewScaleType_FIT_CENTER);
    }
    return NOERROR;
}

/**
 * An optional argument to supply a maximum width for this view. Only valid if
 * {@link #setAdjustViewBounds} has been set to true. To set an image to be a maximum of 100 x
 * 100 while preserving the original aspect ratio, do the following: 1) set adjustViewBounds to
 * true 2) set maxWidth and maxHeight to 100 3) set the height and width layout params to
 * WRAP_CONTENT.
 *
 * <p>
 * Note that this view could be still smaller than 100 x 100 using this approach if the original
 * image is small. To set an image to a fixed size, specify that size in the layout params and
 * then use {@link #setScaleType} to determine how to fit the image within the bounds.
 * </p>
 *
 * @param maxWidth maximum width for this view
 *
 * @attr ref android.R.styleable#ImageView_maxWidth
 */
ECode ImageView::SetMaxWidth(
    /* [in] */ Int32 maxWidth)
{
    mMaxWidth = maxWidth;
    return NOERROR;
}

/**
 * An optional argument to supply a maximum height for this view. Only valid if
 * {@link #setAdjustViewBounds} has been set to true. To set an image to be a maximum of 100 x
 * 100 while preserving the original aspect ratio, do the following: 1) set adjustViewBounds to
 * true 2) set maxWidth and maxHeight to 100 3) set the height and width layout params to
 * WRAP_CONTENT.
 *
 * <p>
 * Note that this view could be still smaller than 100 x 100 using this approach if the original
 * image is small. To set an image to a fixed size, specify that size in the layout params and
 * then use {@link #setScaleType} to determine how to fit the image within the bounds.
 * </p>
 *
 * @param maxHeight maximum height for this view
 *
 * @attr ref android.R.styleable#ImageView_maxHeight
 */
ECode ImageView::SetMaxHeight(
    /* [in] */ Int32 maxHeight)
{
    mMaxHeight = maxHeight;
    return NOERROR;
}

/** Return the view's drawable, or null if no drawable has been
    assigned.
*/
IDrawable* ImageView::GetDrawable()
{
    return mDrawable;
}

/**
 * Sets a drawable as the content of this ImageView.
 *
 * <p class="note">This does Bitmap reading and decoding on the UI
 * thread, which can cause a latency hiccup.  If that's a concern,
 * consider using {@link #setImageDrawable} or
 * {@link #setImageBitmap} and
 * {@link android.graphics.BitmapFactory} instead.</p>
 *
 * @param resId the resource identifier of the the drawable
 *
 * @attr ref android.R.styleable#ImageView_src
 */
ECode ImageView::SetImageResource(
    /* [in] */ Int32 resId)
{
    if (mUri != NULL || mResource != resId) {
        UpdateDrawable(NULL);
        mResource = resId;
        mUri = NULL;
        ResolveUri();
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

/**
 * Sets the content of this ImageView to the specified Uri.
 *
 * <p class="note">This does Bitmap reading and decoding on the UI
 * thread, which can cause a latency hiccup.  If that's a concern,
 * consider using {@link #setImageDrawable} or
 * {@link #setImageBitmap} and
 * {@link android.graphics.BitmapFactory} instead.</p>
 *
 * @param uri The Uri of an image
 */
ECode ImageView::SetImageURI(
    /* [in] */ IUri* uri)
{
    Boolean isEqual;
    if (mResource != 0 || (mUri.Get() != uri &&
            (uri == NULL || mUri == NULL ||
            (uri->Equals(mUri, &isEqual), !isEqual)))) {
        UpdateDrawable(NULL);
        mResource = 0;
        mUri = uri;
        ResolveUri();
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}


/**
 * Sets a drawable as the content of this ImageView.
 *
 * @param drawable The drawable to set
 */
ECode ImageView::SetImageDrawable(
    /* [in] */ IDrawable* drawable)
{
    if (mDrawable.Get() != drawable) {
        mResource = 0;
        mUri = NULL;
        UpdateDrawable(drawable);
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

/**
 * Sets a Bitmap as the content of this ImageView.
 *
 * @param bm The bitmap to set
 */
ECode ImageView::SetImageBitmap(
    /* [in] */ IBitmap* bm)
{
    // if this is used frequently, may handle bitmaps explicitly
    // to reduce the intermediate drawable object
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoPtr<IBitmapDrawable> bd;
    CBitmapDrawable::New(res.Get(), bm, (IBitmapDrawable**)&bd);
    SetImageDrawable((IDrawable*)bd.Get());
    return NOERROR;
}

ECode ImageView::SetImageState(
    /* [in] */ const ArrayOf<Int32>& state,
    /* [in] */ Boolean merge)
{
    if (mState != NULL) ArrayOf<Int32>::Free(mState);

    mState = state.Clone();
    mMergeState = merge;
    if (mDrawable != NULL) {
        RefreshDrawableState();
        ResizeFromDrawable();
    }
    return NOERROR;
}

ECode ImageView::SetSelected(
    /* [in] */ Boolean selected)
{
    View::SetSelected(selected);
    ResizeFromDrawable();
    return NOERROR;
}

/**
 * Sets the image level, when it is constructed from a
 * {@link android.graphics.drawable.LevelListDrawable}.
 *
 * @param level The new level for the image.
 */
ECode ImageView::SetImageLevel(
    /* [in] */ Int32 level)
{
    mLevel = level;
    if (mDrawable != NULL) {
        Boolean changed;
        mDrawable->SetLevel(level, &changed);
        ResizeFromDrawable();
    }
    return NOERROR;
}

/**
 * Controls how the image should be resized or moved to match the size
 * of this ImageView.
 *
 * @param scaleType The desired scaling mode.
 *
 * @attr ref android.R.styleable#ImageView_scaleType
 */
ECode ImageView::SetScaleType(
    /* [in] */ ImageViewScaleType scaleType)
{
//    if (scaleType == NULL) {
//        throw new NullPointerException();
//    }

    if (mScaleType != scaleType) {
        mScaleType = scaleType;

        SetWillNotCacheDrawing(mScaleType == ImageViewScaleType_CENTER);

        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

/**
 * Return the current scale type in use by this ImageView.
 *
 * @see ImageView.ImageViewScaleType
 *
 * @attr ref android.R.styleable#ImageView_scaleType
 */
ImageViewScaleType ImageView::GetScaleType()
{
    return mScaleType;
}

/** Return the view's optional matrix. This is applied to the
    view's drawable when it is drawn. If there is not matrix,
    this method will return null.
    Do not change this matrix in place. If you want a different matrix
    applied to the drawable, be sure to call setImageMatrix().
*/
AutoPtr<IMatrix> ImageView::GetImageMatrix()
{
    return mMatrix;
}

ECode ImageView::SetImageMatrix(
    /* [in] */ IMatrix* matrix)
{
    Boolean isIdentity;
    Boolean isEqual;

    // collaps null and identity to just null
    if (matrix != NULL) {
        matrix->IsIdentity(&isIdentity);
        if (isIdentity) {
            matrix = NULL;
        }
    }

    mMatrix->IsIdentity(&isIdentity);
    mMatrix->Equals(matrix, &isEqual);
    // don't invalidate unless we're actually changing our matrix
    if ((matrix == NULL && !isIdentity) ||
            (matrix != NULL && !isEqual)) {
        mMatrix->Set(matrix);
        ConfigureBounds();
        Invalidate();
    }
    return NOERROR;
}

void ImageView::ResolveUri()
{
    if (mDrawable != NULL) {
        return;
    }

    AutoPtr<IResources> rsrc = GetResources();
    if (rsrc == NULL) {
        return;
    }

    AutoPtr<IDrawable> d;

    if (mResource != 0) {
        if (FAILED(rsrc->GetDrawable(mResource, (IDrawable**)&d))) {
            Logger::W("ImageView",
                    StringBuffer("Unable to find resource: ") + mResource);
            //Don't try again.
            mUri = NULL;
        }
    }
    else if (mUri != NULL) {
        String scheme;
        mUri->GetScheme(&scheme);
        if (!String(ContentResolver_SCHEME_ANDROID_RESOURCE).Compare(scheme)) {
//            try {
                // Load drawable through Resources, to get the source density information
                AutoPtr<IContentResolver> resolver;
                AutoPtr<IOpenResourceIdResult> r;
                mContext->GetContentResolver((IContentResolver**)&resolver);
                resolver->GetResourceId(mUri.Get(), (IOpenResourceIdResult**)&r);
                AutoPtr<IResources> res;
                r->GetResources((IResources**)&res);
                Int32 resId;
                r->GetResourceId(&resId);
                res->GetDrawable(resId, (IDrawable**)&d);
//            } catch (Exception e) {
//                Log.w("ImageView", "Unable to open content: " + mUri, e);
//            }
        }
        else if (!String(ContentResolver_SCHEME_CONTENT).Compare(scheme)
                || !String(ContentResolver_SCHEME_FILE).Compare(scheme)) {
//            try {
                AutoPtr<IContentResolver> resolver;
                mContext->GetContentResolver((IContentResolver**)&resolver);
                AutoPtr<IInputStream> istream;
                resolver->OpenInputStream(mUri.Get(), (IInputStream**)&istream);
                Drawable::CreateFromStream(istream.Get(), String(NULL), (IDrawable**)&d);
//            } catch (Exception e) {
//                Log.w("ImageView", "Unable to open content: " + mUri, e);
//            }
        }
        else {
            String uriDes;
            mUri->GetDescription(&uriDes);
            Drawable::CreateFromPath(uriDes, (IDrawable**)&d);
        }

        if (d == NULL) {
            String uriDes;
            mUri->GetDescription(&uriDes);
            Logger::W("ImageView",
                    StringBuffer("resolveUri failed on bad bitmap uri: ") + uriDes);
            // Don't try again.
            mUri = NULL;
        }
    }
    else {
        return;
    }

    UpdateDrawable(d);
}

ECode ImageView::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    if (mState == NULL) {
        return View::OnCreateDrawableState(extraSpace, drawableState);
    }
    else if (!mMergeState) {
        *drawableState = mState->Clone();
        return NOERROR;
    }
    else {
        View::OnCreateDrawableState(extraSpace + mState->GetLength(), drawableState);
        *drawableState = MergeDrawableStates(*drawableState, mState);
        return NOERROR;
    }
}

void ImageView::UpdateDrawable(
    /* [in] */ IDrawable* d)
{
    if (mDrawable != NULL) {
        mDrawable->SetCallback(NULL);
        UnscheduleDrawable(mDrawable);
    }
    mDrawable = d;
    if (d != NULL) {
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        Boolean stateful;
        d->IsStateful(&stateful);
        if (stateful) {
            d->SetState(GetDrawableState(), &stateful);
        }
        Boolean changed;
        d->SetLevel(mLevel, &changed);
        d->GetIntrinsicWidth(&mDrawableWidth);
        d->GetIntrinsicHeight(&mDrawableHeight);
        ApplyColorMod();
        ConfigureBounds();
    }
}

void ImageView::ResizeFromDrawable()
{
    AutoPtr<IDrawable> d = mDrawable;
    if (d != NULL) {
        Int32 w;
        d->GetIntrinsicWidth(&w);
        if (w < 0) w = mDrawableWidth;
        Int32 h;
        d->GetIntrinsicHeight(&h);
        if (h < 0) h = mDrawableHeight;
        if (w != mDrawableWidth || h != mDrawableHeight) {
            mDrawableWidth = w;
            mDrawableHeight = h;
            RequestLayout();
        }
    }
}

const MatrixScaleToFit ImageView::sS2FArray[4] = {
    MatrixScaleToFit_FILL,
    MatrixScaleToFit_START,
    MatrixScaleToFit_CENTER,
    MatrixScaleToFit_END
};

MatrixScaleToFit ImageView::ScaleTypeToScaleToFit(
    /* [in] */ ImageViewScaleType st)
{
    // ScaleToFit enum to their corresponding Matrix.ScaleToFit values
    return sS2FArray[st - 1];
}

void ImageView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    ResolveUri();
    Int32 w;
    Int32 h;

    // Desired aspect ratio of the view's contents (not including padding)
    Float desiredAspect = 0.0f;

    // We are allowed to change the view's width
    Boolean resizeWidth = FALSE;

    // We are allowed to change the view's height
    Boolean resizeHeight = FALSE;

    if (mDrawable == NULL) {
        // If no drawable, its intrinsic size is 0.
        mDrawableWidth = -1;
        mDrawableHeight = -1;
        w = h = 0;
    }
    else {
        w = mDrawableWidth;
        h = mDrawableHeight;
        if (w <= 0) w = 1;
        if (h <= 0) h = 1;

        // We are supposed to adjust view bounds to match the aspect
        // ratio of our drawable. See if that is possible.
        if (mAdjustViewBounds) {

            Int32 widthSpecMode = MeasureSpec::GetMode(widthMeasureSpec);
            Int32 heightSpecMode = MeasureSpec::GetMode(heightMeasureSpec);

            resizeWidth = widthSpecMode != MeasureSpec::EXACTLY;
            resizeHeight = heightSpecMode != MeasureSpec::EXACTLY;

            desiredAspect = (Float)w/(Float)h;
        }
    }

    Int32 pleft = mPaddingLeft;
    Int32 pright = mPaddingRight;
    Int32 ptop = mPaddingTop;
    Int32 pbottom = mPaddingBottom;

    Int32 widthSize;
    Int32 heightSize;

    if (resizeWidth || resizeHeight) {
        /* If we get here, it means we want to resize to match the
            drawables aspect ratio, and we have the freedom to change at
            least one dimension.
        */

        // Get the max possible width given our constraints
        widthSize = ResolveAdjustedSize(w + pleft + pright,
                                             mMaxWidth, widthMeasureSpec);

        // Get the max possible height given our constraints
        heightSize = ResolveAdjustedSize(h + ptop + pbottom,
                                            mMaxHeight, heightMeasureSpec);

        if (desiredAspect != 0.0f) {
            // See what our actual aspect ratio is
            Float actualAspect = (Float)(widthSize - pleft - pright) /
                                    (heightSize - ptop - pbottom);

            if (Math::Abs(actualAspect - desiredAspect) > 0.0000001) {

                Boolean done = FALSE;

                // Try adjusting width to be proportional to height
                if (resizeWidth) {
                    Int32 newWidth = (Int32)(desiredAspect *
                                        (heightSize - ptop - pbottom))
                                        + pleft + pright;
                    if (newWidth <= widthSize) {
                        widthSize = newWidth;
                        done = TRUE;
                    }
                }

                // Try adjusting height to be proportional to width
                if (!done && resizeHeight) {
                    Int32 newHeight = (Int32)((widthSize - pleft - pright)
                                        / desiredAspect) + ptop + pbottom;
                    if (newHeight <= heightSize) {
                        heightSize = newHeight;
                    }
                }
            }
        }
    }
    else {
        /* We are either don't want to preserve the drawables aspect ratio,
           or we are not allowed to change view dimensions. Just measure in
           the normal way.
        */
        w += pleft + pright;
        h += ptop + pbottom;

        w = Math::Max(w, GetSuggestedMinimumWidth());
        h = Math::Max(h, GetSuggestedMinimumHeight());

        widthSize = ResolveSize(w, widthMeasureSpec);
        heightSize = ResolveSize(h, heightMeasureSpec);
    }

    SetMeasuredDimension(widthSize, heightSize);
}

Int32 ImageView::ResolveAdjustedSize(
    /* [in] */ Int32 desiredSize,
    /* [in] */ Int32 maxSize,
    /* [in] */ Int32 measureSpec)
{
    Int32 result = desiredSize;
    Int32 specMode = MeasureSpec::GetMode(measureSpec);
    Int32 specSize =  MeasureSpec::GetSize(measureSpec);
    switch (specMode) {
        case MeasureSpec::UNSPECIFIED:
            /* Parent says we can be as big as we want. Just don't be larger
               than max size imposed on ourselves.
            */
            result = Math::Min(desiredSize, maxSize);
            break;
        case MeasureSpec::AT_MOST:
            // Parent says we can be as big as we want, up to specSize.
            // Don't be larger than specSize, and don't be larger than
            // the max size imposed on ourselves.
            result = Math::Min(Math::Min(desiredSize, specSize), maxSize);
            break;
        case MeasureSpec::EXACTLY:
            // No choice. Do what we are told.
            result = specSize;
            break;
    }
    return result;
}

Boolean ImageView::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Boolean changed = View::SetFrame(left, top, right, bottom);
    mHaveFrame = TRUE;
    ConfigureBounds();
    return changed;
}

void ImageView::ConfigureBounds()
{
    if (mDrawable == NULL || !mHaveFrame) {
        return;
    }

    Int32 dwidth = mDrawableWidth;
    Int32 dheight = mDrawableHeight;

    Int32 vwidth = GetWidth() - mPaddingLeft - mPaddingRight;
    Int32 vheight = GetHeight() - mPaddingTop - mPaddingBottom;

    Boolean fits = (dwidth < 0 || vwidth == dwidth) &&
                   (dheight < 0 || vheight == dheight);

    if (dwidth <= 0 || dheight <= 0 || ImageViewScaleType_FIT_XY == mScaleType) {
        /* If the drawable has no intrinsic size, or we're told to
            scaletofit, then we just fill our entire view.
        */
        mDrawable->SetBounds(0, 0, vwidth, vheight);
        mDrawMatrix = NULL;
    }
    else {
        Boolean result;

        // We need to do the scaling ourself, so have the drawable
        // use its native size.
        mDrawable->SetBounds(0, 0, dwidth, dheight);

        if (ImageViewScaleType_MATRIX == mScaleType) {
            // Use the specified matrix as-is.
            Boolean isIdentity;
            mMatrix->IsIdentity(&isIdentity);
            if (isIdentity) {
                mDrawMatrix = NULL;
            }
            else {
                mDrawMatrix = mMatrix;
            }
        }
        else if (fits) {
            // The bitmap fits exactly, no transform needed.
            mDrawMatrix = NULL;
        }
        else if (ImageViewScaleType_CENTER == mScaleType) {
            // Center bitmap in view, no scaling.
            mDrawMatrix = mMatrix;
            mDrawMatrix->SetTranslate((Int32)((vwidth - dwidth) * 0.5f + 0.5f),
                                     (Int32)((vheight - dheight) * 0.5f + 0.5f));
        }
        else if (ImageViewScaleType_CENTER_CROP == mScaleType) {
            mDrawMatrix = mMatrix;

            Float scale;
            Float dx = 0, dy = 0;

            if (dwidth * vheight > vwidth * dheight) {
                scale = (Float)vheight / (Float)dheight;
                dx = (vwidth - dwidth * scale) * 0.5f;
            }
            else {
                scale = (Float)vwidth / (Float)dwidth;
                dy = (vheight - dheight * scale) * 0.5f;
            }

            mDrawMatrix->SetScaleEx(scale, scale);
            mDrawMatrix->PostTranslate(dx + 0.5f, dy + 0.5f, &result);
        }
        else if (ImageViewScaleType_CENTER_INSIDE == mScaleType) {
            mDrawMatrix = mMatrix;
            Float scale;
            Float dx;
            Float dy;

            if (dwidth <= vwidth && dheight <= vheight) {
                scale = 1.0f;
            }
            else {
                scale = Math::Min((Float)vwidth / (Float)dwidth,
                        (Float)vheight / (Float)dheight);
            }

            dx = (Int32)((vwidth - dwidth * scale) * 0.5f + 0.5f);
            dy = (Int32)((vheight - dheight * scale) * 0.5f + 0.5f);

            mDrawMatrix->SetScaleEx(scale, scale);
            mDrawMatrix->PostTranslate(dx, dy, &result);
        }
        else {
            // Generate the required transform.
            mTempSrc->Set(0, 0, dwidth, dheight);
            mTempDst->Set(0, 0, vwidth, vheight);

            mDrawMatrix = mMatrix;
            mDrawMatrix->SetRectToRect(mTempSrc, mTempDst,
                    ScaleTypeToScaleToFit(mScaleType), &result);
        }
    }
}

ECode ImageView::DrawableStateChanged()
{
    FAIL_RETURN(View::DrawableStateChanged());
    AutoPtr<IDrawable> d = mDrawable;
    if (d != NULL) {
        Boolean stateful;
        d->IsStateful(&stateful);
        if (stateful) {
            d->SetState(GetDrawableState(), &stateful);
        }
    }
    return NOERROR;
}

void ImageView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    View::OnDraw(canvas);

    if (mDrawable == NULL) {
        return; // couldn't resolve the URI
    }

    if (mDrawableWidth == 0 || mDrawableHeight == 0) {
        return;     // nothing to draw (empty bounds)
    }

    if (mDrawMatrix == NULL && mPaddingTop == 0 && mPaddingLeft == 0) {
        mDrawable->Draw(canvas);
    }
    else {
        Int32 saveCount1, saveCount2;
        canvas->GetSaveCount(&saveCount1);
        canvas->Save(&saveCount2);

        if (mCropToPadding) {
            Boolean IsNonEmpty;
            Int32 scrollX = mScrollX;
            Int32 scrollY = mScrollY;
            canvas->ClipRectEx6(scrollX + mPaddingLeft, scrollY + mPaddingTop,
                    scrollX + mRight - mLeft - mPaddingRight,
                    scrollY + mBottom - mTop - mPaddingBottom,
                    &IsNonEmpty);
        }

        canvas->Translate(mPaddingLeft, mPaddingTop);

        if (mDrawMatrix != NULL) {
            canvas->Concat(mDrawMatrix);
        }
        mDrawable->Draw(canvas);
        canvas->RestoreToCount(saveCount1);
    }
}

ECode ImageView::GetBaseline(
    /* [in] */ Int32* baseLine)
{
    *baseLine = mBaselineAligned ? GetMeasuredHeight() : -1;
    return NOERROR;
}

/**
 * Set a tinting option for the image.
 *
 * @param color Color tint to apply.
 * @param mode How to apply the color.  The standard mode is
 * {@link PorterDuff.Mode#SRC_ATOP}
 *
 * @attr ref android.R.styleable#ImageView_tint
 */
ECode ImageView::SetColorFilter(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    AutoPtr<IColorFilter> cf;
    CPorterDuffColorFilter::New(color, mode, (IColorFilter**)&cf);
    return SetColorFilter(cf.Get());
}

/**
 * Set a tinting option for the image. Assumes
 * {@link PorterDuff.Mode#SRC_ATOP} blending mode.
 *
 * @param color Color tint to apply.
 * @attr ref android.R.styleable#ImageView_tint
 */
ECode ImageView::SetColorFilter(
    /* [in] */ Int32 color)
{
    return SetColorFilter(color, PorterDuffMode_SRC_ATOP);
}

ECode ImageView::ClearColorFilter()
{
    return SetColorFilter(NULL);
}

/**
 * Apply an arbitrary colorfilter to the image.
 *
 * @param cf the colorfilter to apply (may be NULL)
 */
ECode ImageView::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    if (mColorFilter.Get() != cf) {
        mColorFilter = cf;
        mColorMod = TRUE;
        ApplyColorMod();
        Invalidate();
    }
    return NOERROR;
}

ECode ImageView::SetAlpha(
    /* [in] */ Int32 alpha)
{
    alpha &= 0xFF;          // keep it legal
    if (mAlpha != alpha) {
        mAlpha = alpha;
        mColorMod = TRUE;
        ApplyColorMod();
        Invalidate();
    }
    return NOERROR;
}

void ImageView::ApplyColorMod()
{
    // Only mutate and apply when modifications have occurred. This should
    // not reset the mColorMod flag, since these filters need to be
    // re-applied if the Drawable is changed.
    if (mDrawable != NULL && mColorMod) {
        AutoPtr<IDrawable> drawable;
        mDrawable->Mutate((IDrawable**)&drawable);
        mDrawable = drawable;
        mDrawable->SetColorFilter(mColorFilter);
        mDrawable->SetAlpha(mAlpha * mViewAlphaScale >> 8);
    }
}

ECode ImageView::Init(
    /* [in] */ IContext* context)
{
    View::Init(context);
    InitImageView();
    return NOERROR;
}

ECode ImageView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0);
}

ECode ImageView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    View::Init(context, attrs, defStyle);
    InitImageView();
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));

    return NOERROR;
}

static Int32 R_Styleable_ImageView[] = {
    0x01010119, 0x0101011d, 0x0101011e, 0x0101011f,
    0x01010120, 0x01010121, 0x01010122, 0x01010123
};

ECode ImageView::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx3(
            attrs,
            ArrayOf<Int32>(R_Styleable_ImageView,
            sizeof(R_Styleable_ImageView) / sizeof(Int32)),/*com.android.internal.R.styleable.ImageView*/
            defStyle, 0, (ITypedArray**)&a));

    AutoPtr<IDrawable> d;
    a->GetDrawable(
            0/*com.android.internal.R.styleable.ImageView_src*/,
            (IDrawable**)&d);
    if (d != NULL) {
        SetImageDrawable(d);
    }

    a->GetBoolean(
            6/*com.android.internal.R.styleable.ImageView_baselineAlignBottom*/,
            FALSE, &mBaselineAligned);

    Boolean bv;
    a->GetBoolean(
            2/*com.android.internal.R.styleable.ImageView_adjustViewBounds*/,
            FALSE, &bv);
    SetAdjustViewBounds(bv);

    Int32 iv;
    a->GetDimensionPixelSize(
            3/*com.android.internal.R.styleable.ImageView_maxWidth*/,
            Math::INT32_MAX_VALUE, &iv);
    SetMaxWidth(iv);

    a->GetDimensionPixelSize(
            4/*com.android.internal.R.styleable.ImageView_maxHeight*/,
            Math::INT32_MAX_VALUE, &iv);
    SetMaxHeight(iv);

    Int32 index;
    a->GetInt32(
            1/*com.android.internal.R.styleable.ImageView_scaleType*/,
            -1, &index);
    if (index >= 0) {
        SetScaleType(sScaleTypeArray[index]);
    }

    Int32 tint;
    a->GetInt32(
            5/*com.android.internal.R.styleable.ImageView_tint*/,
            0, &tint);
    if (tint != 0) {
        SetColorFilter(tint);
    }

    a->GetBoolean(
            7/*com.android.internal.R.styleable.ImageView_cropToPadding*/,
            FALSE, &mCropToPadding);

    a->Recycle();

    //need inflate syntax/reader for matrix
    return NOERROR;
}
