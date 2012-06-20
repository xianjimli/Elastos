
#ifndef __IMAGEVIEW_H__
#define __IMAGEVIEW_H__

#include "view/View.h"
#include "graphics/CRectF.h"

using namespace Elastos;

class ImageView : public View
{
public:
    ImageView();

    ImageView(
        /* [in] */ IContext* context);

    ImageView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ImageView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* drawable);

    virtual CARAPI SetAdjustViewBounds(
        /* [in] */ Boolean adjustViewBounds);

    virtual CARAPI SetMaxWidth(
        /* [in] */ Int32 maxWidth);

    virtual CARAPI SetMaxHeight(
        /* [in] */ Int32 maxHeight);

    virtual CARAPI_(IDrawable*) GetDrawable();

    virtual CARAPI SetImageResource(
        /* [in] */ Int32 resId);

    virtual CARAPI SetImageURI(
        /* [in] */ IUri* uri);

    virtual CARAPI SetImageDrawable(
        /* [in] */ IDrawable* drawable);

    virtual CARAPI SetImageBitmap(
        /* [in] */ IBitmap* bm);

    virtual CARAPI SetImageState(
        /* [in] */ const ArrayOf<Int32>& state,
        /* [in] */ Boolean merge);

    CARAPI SetSelected(
        /* [in] */ Boolean selected);

    virtual CARAPI SetImageLevel(
        /* [in] */ Int32 level);

    virtual CARAPI SetScaleType(
        /* [in] */ ImageViewScaleType scaleType);

    virtual CARAPI_(ImageViewScaleType) GetScaleType();

    virtual CARAPI_(AutoPtr<IMatrix>) GetImageMatrix();

    virtual CARAPI SetImageMatrix(
        /* [in] */ IMatrix* matrix);

    CARAPI GetBaseline(
        /* [in] */ Int32* baseLine);

    CARAPI SetColorFilter(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode);

    CARAPI SetColorFilter(
        /* [in] */ Int32 color);

    CARAPI ClearColorFilter();

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

protected:
    CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* dr);

    CARAPI_(Boolean) OnSetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI DrawableStateChanged();

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    static CARAPI_(MatrixScaleToFit) ScaleTypeToScaleToFit(
        /* [in] */ ImageViewScaleType st);

    CARAPI_(void) InitImageView();

    CARAPI_(void) ResolveUri();

    CARAPI_(void) UpdateDrawable(
        /* [in] */ IDrawable* d);

    CARAPI_(void) ResizeFromDrawable();

    CARAPI_(Int32) ResolveAdjustedSize(
        /* [in] */ Int32 desiredSize,
        /* [in] */ Int32 maxSize,
        /* [in] */ Int32 measureSpec);

    CARAPI_(void) ConfigureBounds();

    CARAPI_(void) ApplyColorMod();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    static const ImageViewScaleType sScaleTypeArray[];
    static const MatrixScaleToFit sS2FArray[4];

    // settable by the client
    AutoPtr<IUri> mUri;
    Int32 mResource;
    AutoPtr<IMatrix> mMatrix;
    ImageViewScaleType mScaleType;
    Boolean mHaveFrame;
    Boolean mAdjustViewBounds;
    Int32 mMaxWidth;
    Int32 mMaxHeight;


    // these are applied to the drawable
    AutoPtr<IColorFilter> mColorFilter;
    Int32 mAlpha;
    Int32 mViewAlphaScale;
    Boolean mColorMod;

    AutoPtr<IDrawable> mDrawable;
    ArrayOf<Int32>* mState;
    Boolean mMergeState;
    Int32 mLevel;
    Int32 mDrawableWidth;
    Int32 mDrawableHeight;
    AutoPtr<IMatrix> mDrawMatrix;

    // Avoid allocations...
    AutoPtr<CRectF> mTempSrc;
    AutoPtr<CRectF> mTempDst;

    Boolean mCropToPadding;

    Boolean mBaselineAligned;
};
#endif //__IMAGEVIEW_H__
