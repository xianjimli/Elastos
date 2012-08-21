
#include "graphics/drawable/GradientDrawable.h"
#include "graphics/ElPixelFormat.h"
#include "graphics/CPaint.h"
#include "graphics/CPath.h"
#include "graphics/CRectF.h"
#include "graphics/CDashPathEffect.h"
#include "graphics/CLinearGradient.h"
#include "graphics/CRadialGradient.h"
#include "graphics/CSweepGradient.h"
#include "graphics/drawable/CGradientDrawable.h"
#include "utils/CTypedValue.h"
#include <elastos/Math.h>
#include <Logger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

GradientDrawable::GradientState::GradientState()
    : mShape(GradientDrawable_RECTANGLE)
    , mGradient(GradientDrawable_LINEAR_GRADIENT)
    , mOrientation(GradientDrawableOrientation_TOP_BOTTOM)
    , mColors(NULL)
    , mTempColors(NULL)
    , mTempPositions(NULL)
    , mPositions(NULL)
    , mHasSolidColor(FALSE)
    , mStrokeWidth(-1)
    , mRadiusArray(NULL)
    , mWidth(-1)
    , mHeight(-1)
    , mCenterX(0.5f)
    , mCenterY(0.5f)
    , mGradientRadius(0.5f)
    , mUseLevel(FALSE)
    , mUseLevelForShape(FALSE)
{
}

GradientDrawable::GradientState::GradientState(
    /* [in] */ GradientDrawableOrientation orientation,
    /* [in] */ ArrayOf<Int32>* colors)
    : mShape(GradientDrawable_RECTANGLE)
    , mGradient(GradientDrawable_LINEAR_GRADIENT)
    , mOrientation(orientation)
    , mColors(colors)
    , mTempColors(NULL)
    , mTempPositions(NULL)
    , mPositions(NULL)
    , mHasSolidColor(FALSE)
    , mStrokeWidth(-1)
    , mRadiusArray(NULL)
    , mWidth(-1)
    , mHeight(-1)
    , mCenterX(0.5f)
    , mCenterY(0.5f)
    , mGradientRadius(0.5f)
    , mUseLevel(FALSE)
    , mUseLevelForShape(FALSE)
{
}

GradientDrawable::GradientState::GradientState(
    /* [in] */ GradientState* state)
    : mChangingConfigurations(state->mChangingConfigurations)
    , mShape(state->mShape)
    , mGradient(state->mGradient)
    , mOrientation(state->mOrientation)
    , mColors(NULL)
    , mTempColors(NULL)
    , mTempPositions(NULL)
    , mPositions(NULL)
    , mHasSolidColor(state->mHasSolidColor)
    , mStrokeWidth(state->mStrokeWidth)
    , mStrokeColor(state->mStrokeColor)
    , mStrokeDashWidth(state->mStrokeDashWidth)
    , mStrokeDashGap(state->mStrokeDashGap)
    , mRadius(state->mRadius)
    , mRadiusArray(NULL)
    , mWidth(state->mWidth)
    , mHeight(state->mHeight)
    , mInnerRadiusRatio(state->mInnerRadiusRatio)
    , mThicknessRatio(state->mThicknessRatio)
    , mInnerRadius(state->mInnerRadius)
    , mThickness(state->mThickness)
    , mCenterX(state->mCenterX)
    , mCenterY(state->mCenterY)
    , mGradientRadius(state->mGradientRadius)
    , mUseLevel(state->mUseLevel)
    , mUseLevelForShape(state->mUseLevelForShape)
{
    if (state->mColors != NULL) {
        mColors = state->mColors->Clone();
    }
    if (state->mPositions != NULL) {
        mPositions = state->mPositions->Clone();
    }
    if (state->mRadiusArray != NULL) {
        mRadiusArray = state->mRadiusArray->Clone();
    }
    if (state->mPadding != NULL) {
        CRect::NewByFriend((IRect*)state->mPadding.Get(), (CRect**)&mPadding);
    }
}

GradientDrawable::GradientState::~GradientState()
{
    ArrayOf<Int32>::Free(mColors);
    ArrayOf<Float>::Free(mPositions);
    ArrayOf<Float>::Free(mRadiusArray);
}

PInterface GradientDrawable::GradientState::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDrawableConstantState) {
        return (IDrawableConstantState*)this;
    }
    else if (riid == EIID_IGradientState) {
        return (IGradientState*)this;
    }

    return NULL;
}

UInt32 GradientDrawable::GradientState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 GradientDrawable::GradientState::Release()
{
    return ElRefBase::Release();
}

ECode GradientDrawable::GradientState::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IGradientState*)this) {
        *pIID = EIID_IGradientState;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode GradientDrawable::GradientState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CGradientDrawable::New(this, (IGradientDrawable**)drawable);
}

ECode GradientDrawable::GradientState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CGradientDrawable::New(this, (IGradientDrawable**)drawable);
}

ECode GradientDrawable::GradientState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    return mChangingConfigurations;
}

ECode GradientDrawable::GradientState::SetShape(
    /* [in] */ Int32 shape)
{
    mShape = shape;
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetGradientType(
    /* [in] */ Int32 gradient)
{
    mGradient = gradient;
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetGradientCenter(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mCenterX = x;
    mCenterY = y;
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetSolidColor(
    /* [in] */ Int32 argb)
{
    mHasSolidColor = true;
    mSolidColor = argb;
    mColors = NULL;
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetStroke(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color)
{
    mStrokeWidth = width;
    mStrokeColor = color;
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetStrokeEx(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color,
    /* [in] */ Float dashWidth,
    /* [in] */ Float dashGap)
{
    mStrokeWidth = width;
    mStrokeColor = color;
    mStrokeDashWidth = dashWidth;
    mStrokeDashGap = dashGap;
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetCornerRadius(
    /* [in] */ Float radius)
{
    if (radius < 0) {
        radius = 0;
    }
    mRadius = radius;
    ArrayOf<Float>::Free(mRadiusArray);
    mRadiusArray = NULL;
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetCornerRadii(
    /* [in] */ ArrayOf<Float>* radii)
{
    if (radii == NULL) {
        mRadius = 0;
        mRadiusArray = NULL;
    }
    else {
        mRadiusArray = radii->Clone();
    }
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWidth = width;
    mHeight = height;
    return NOERROR;
}

ECode GradientDrawable::GradientState::SetGradientRadius(
    /* [in] */ Float gradientRadius)
{
    mGradientRadius = gradientRadius;
    return NOERROR;
}

GradientDrawable::GradientDrawable()
    : mAlpha(0xFF)
    , mDither(FALSE)
    , mRectIsDirty(FALSE)
    , mMutated(FALSE)
    , mPathIsDirty(FALSE)
{
    CPaint::New(Paint_ANTI_ALIAS_FLAG, (IPaint**)&mFillPaint);
    CPath::New((IPath**)&mPath);
    CRectF::New((IRectF**)&mRect);

//    this(new GradientState(Orientation.TOP_BOTTOM, null));
}

/**
 * Create a new gradient drawable given an orientation and an array
 * of colors for the gradient.
 */
GradientDrawable::GradientDrawable(
    /* [in] */ GradientDrawableOrientation orientation,
    /* [in] */ ArrayOf<Int32>* colors)
    : mAlpha(0xFF)
    , mDither(FALSE)
    , mRectIsDirty(FALSE)
    , mMutated(FALSE)
    , mPathIsDirty(FALSE)
{
    CPaint::New(Paint_ANTI_ALIAS_FLAG, (IPaint**)&mFillPaint);
    CPath::New((IPath**)&mPath);
    CRectF::NewByFriend((CRectF**)&mRect);

    Init(new GradientState(orientation, colors));
}

//@Override
Boolean GradientDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    if (mPadding != NULL) {
        padding->SetEx((IRect*)mPadding.Get());
        return TRUE;
    }
    else {
        return Drawable::GetPadding(padding);
    }
}

/**
 * Specify radii for each of the 4 corners. For each corner, the array
 * contains 2 values, [X_radius, Y_radius]. The corners are ordered
 * top-left, top-right, bottom-right, bottom-left
 */
ECode GradientDrawable::SetCornerRadii(
    /* [in] */ ArrayOf<Float>* radii)
{
    return mGradientState->SetCornerRadii(radii);
}

/**
 * Specify radius for the corners of the gradient. If this is > 0, then the
 * drawable is drawn in a round-rectangle, rather than a rectangle.
 */
ECode GradientDrawable::SetCornerRadius(
    /* [in] */ Float radius)
{
    return mGradientState->SetCornerRadius(radius);
}

/**
 * Set the stroke width and color for the drawable. If width is zero,
 * then no stroke is drawn.
 */
ECode GradientDrawable::SetStroke(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color)
{
    return SetStroke(width, color, 0, 0);
}

ECode GradientDrawable::SetStroke(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color,
    /* [in] */ Float dashWidth,
    /* [in] */ Float dashGap)
{
    mGradientState->SetStrokeEx(width, color, dashWidth, dashGap);

    if (mStrokePaint == NULL)  {
        CPaint::New(Paint_ANTI_ALIAS_FLAG, (IPaint**)&mStrokePaint);
        mStrokePaint->SetStyle(PaintStyle_STROKE);
    }
    mStrokePaint->SetStrokeWidth(width);
    mStrokePaint->SetColor(color);

    AutoPtr<IPathEffect> e;
    if (dashWidth > 0) {
        ArrayOf_<Float, 2> intervals;
        intervals[0] = dashWidth;
        intervals[1] = dashGap;
        CDashPathEffect::New(intervals, 0, (IDashPathEffect**)&e);
    }
    mStrokePaint->SetPathEffect(e);
    return NOERROR;
}

ECode GradientDrawable::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return mGradientState->SetSize(width, height);
}

ECode GradientDrawable::SetShape(
    /* [in] */ Int32 shape)
{
    mRingPath = NULL;
    return mGradientState->SetShape(shape);
}

ECode GradientDrawable::SetGradientType(
    /* [in] */ Int32 gradient)
{
    FAIL_RETURN(mGradientState->SetGradientType(gradient));
    mRectIsDirty = TRUE;
    return NOERROR;
}

ECode GradientDrawable::SetGradientCenter(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return mGradientState->SetGradientCenter(x, y);
}

ECode GradientDrawable::SetGradientRadius(
    /* [in] */ Float gradientRadius)
{
    return mGradientState->SetGradientRadius(gradientRadius);
}

ECode GradientDrawable::SetUseLevel(
    /* [in] */ Boolean useLevel)
{
    mGradientState->mUseLevel = useLevel;
    return NOERROR;
}

Int32 GradientDrawable::ModulateAlpha(
    /* [in] */ Int32 alpha)
{
    Int32 scale = mAlpha + (mAlpha >> 7);
    return alpha * scale >> 8;
}

//@Override
ECode GradientDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (!EnsureValidRect()) {
        // nothing to draw
        return NOERROR;
    }

    // remember the alpha values, in case we temporarily overwrite them
    // when we modulate them with mAlpha
    Int32 prevFillAlpha;
    mFillPaint->GetAlpha(&prevFillAlpha);
    Int32 prevStrokeAlpha = 0;
    if (mStrokePaint != NULL) mStrokePaint->GetAlpha(&prevStrokeAlpha);
    // compute the modulate alpha values
    Int32 currFillAlpha = ModulateAlpha(prevFillAlpha);
    Int32 currStrokeAlpha = ModulateAlpha(prevStrokeAlpha);

    Float width;
    Boolean haveStroke = currStrokeAlpha > 0 && (mStrokePaint->GetStrokeWidth(&width), width > 0);
    Boolean haveFill = currFillAlpha > 0;
    GradientState* st = mGradientState;
    /*  we need a layer iff we're drawing both a fill and stroke, and the
        stroke is non-opaque, and our shapetype actually supports
        fill+stroke. Otherwise we can just draw the stroke (if any) on top
        of the fill (if any) without worrying about blending artifacts.
    */
    Boolean useLayer = haveStroke && haveFill && st->mShape != GradientDrawable_LINE &&
            currStrokeAlpha < 255;

    /*  Drawing with a layer is slower than direct drawing, but it
        allows us to apply paint effects like alpha and colorfilter to
        the result of multiple separate draws. In our case, if the user
        asks for a non-opaque alpha value (via setAlpha), and we're
        stroking, then we need to apply the alpha AFTER we've drawn
        both the fill and the stroke.
    */
    if (useLayer) {
        if (mLayerPaint == NULL) {
            CPaint::New((IPaint**)&mLayerPaint);
        }
        mLayerPaint->SetDither(mDither);
        mLayerPaint->SetAlpha(mAlpha);
        mLayerPaint->SetColorFilter(mColorFilter);

        Float rad;
        mStrokePaint->GetStrokeWidth(&rad);
        Int32 count;
        canvas->SaveLayerEx(mRect->mLeft - rad, mRect->mTop - rad,
                mRect->mRight + rad, mRect->mBottom + rad,
                mLayerPaint, Canvas_HAS_ALPHA_LAYER_SAVE_FLAG, &count);

        // don't perform the filter in our individual paints
        // since the layer will do it for us
        mFillPaint->SetColorFilter(NULL);
        mStrokePaint->SetColorFilter(NULL);
    }
    else {
        /*  if we're not using a layer, apply the dither/filter to our
            individual paints
        */
        mFillPaint->SetAlpha(currFillAlpha);
        mFillPaint->SetDither(mDither);
        mFillPaint->SetColorFilter(mColorFilter);
        if (haveStroke) {
            mStrokePaint->SetAlpha(currStrokeAlpha);
            mStrokePaint->SetDither(mDither);
            mStrokePaint->SetColorFilter(mColorFilter);
        }
    }

    switch (st->mShape) {
        case GradientDrawable_RECTANGLE:
            if (st->mRadiusArray != NULL) {
                mPath->Reset();
                mPath->AddRoundRectEx((IRectF*)mRect.Get(), *st->mRadiusArray,
                        PathDirection_CW);
                canvas->DrawPath(mPath, mFillPaint);
                if (haveStroke) {
                    canvas->DrawPath(mPath, mStrokePaint);
                }
            }
            else {
                // since the caller is only giving us 1 value, we will force
                // it to be square if the rect is too small in one dimension
                // to show it. If we did nothing, Skia would clamp the rad
                // independently along each axis, giving us a thin ellips
                // if the rect were very wide but not very tall
                Float rad = st->mRadius;
                Float width = mRect->GetWidth();
                Float height = mRect->GetHeight();
                Float r = Math::Min(width, height) * 0.5f;
                if (rad > r) {
                    rad = r;
                }
                canvas->DrawRoundRect(mRect, rad, rad, mFillPaint);
                if (haveStroke) {
                    canvas->DrawRoundRect(mRect, rad, rad, mStrokePaint);
                }
            }
            break;
        case GradientDrawable_OVAL:
            canvas->DrawOval(mRect, mFillPaint);
            if (haveStroke) {
                canvas->DrawOval(mRect, mStrokePaint);
            }
            break;
        case GradientDrawable_LINE: {
            CRectF* r = mRect;
            Float y;
            r->CenterY(&y);
            canvas->DrawLine(r->mLeft, y, r->mRight, y, mStrokePaint);
            break;
        }
        case GradientDrawable_RING:
            AutoPtr<IPath> path = BuildRing(st);
            canvas->DrawPath(path, mFillPaint);
            if (haveStroke) {
                canvas->DrawPath(path, mStrokePaint);
            }
            break;
    }

    if (useLayer) {
        canvas->Restore();
    }
    else {
        mFillPaint->SetAlpha(prevFillAlpha);
        if (haveStroke) {
            mStrokePaint->SetAlpha(prevStrokeAlpha);
        }
    }
    return NOERROR;
}

AutoPtr<IPath> GradientDrawable::BuildRing(
    /* [in] */ GradientState* st)
{
    if (mRingPath != NULL && (!st->mUseLevelForShape || !mPathIsDirty)) return mRingPath;
    mPathIsDirty = FALSE;

    Float sweep = st->mUseLevelForShape ? (360.0f * GetLevel() / 10000.0f) : 360.0f;

    AutoPtr<IRectF> bounds;
    CRectF::New(mRect, (IRectF**)&bounds);

    Float w, h, x, y;
    bounds->Width(&w);
    bounds->Height(&h);
    x = w /2.0f;
    y = h /2.0f;

    Float thickness = st->mThickness != -1 ?
            st->mThickness : w / st->mThicknessRatio;
    // inner radius
    Float radius = st->mInnerRadius != -1 ?
            st->mInnerRadius : w / st->mInnerRadiusRatio;

    AutoPtr<IRectF> innerBounds;
    CRectF::New(bounds, (IRectF**)&innerBounds);
    innerBounds->Inset(x - radius, y - radius);

    bounds = NULL;
    CRectF::New(innerBounds, (IRectF**)&bounds);
    bounds->Inset(-thickness, -thickness);

    if (mRingPath == NULL) {
        CPath::New((IPath**)&mRingPath);
    }
    else {
        mRingPath->Reset();
    }

    AutoPtr<IPath> ringPath = mRingPath;
    // arcTo treats the sweep angle mod 360, so check for that, since we
    // think 360 means draw the entire oval
    if (sweep < 360 && sweep > -360) {
        ringPath->SetFillType(PathFillType_EVEN_ODD);
        // inner top
        ringPath->MoveTo(x + radius, y);
        // outer top
        ringPath->LineTo(x + radius + thickness, y);
        // outer arc
        ringPath->ArcTo(bounds, 0.0f, sweep, FALSE);
        // inner arc
        ringPath->ArcTo(innerBounds, sweep, -sweep, FALSE);
        ringPath->Close();
    }
    else {
        // add the entire ovals
        ringPath->AddOval(bounds, PathDirection_CW);
        ringPath->AddOval(innerBounds, PathDirection_CCW);
    }

    return ringPath;
}

ECode GradientDrawable::SetColor(
    /* [in] */ Int32 argb)
{
    mGradientState->SetSolidColor(argb);
    mFillPaint->SetColor(argb);
    return NOERROR;
}

//@Override
Int32 GradientDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
            | mGradientState->mChangingConfigurations;
}

//@Override
ECode GradientDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    mAlpha = alpha;
    return NOERROR;
}

//@Override
ECode GradientDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    mDither = dither;
    return NOERROR;
}

//@Override
ECode GradientDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    mColorFilter = cf;
    return NOERROR;
}

//@Override
Int32 GradientDrawable::GetOpacity()
{
    // XXX need to figure out the actual opacity...
    return ElPixelFormat::TRANSLUCENT;
}

//@Override
void GradientDrawable::OnBoundsChange(
    /* [in] */ IRect* r)
{
    Drawable::OnBoundsChange(r);
    mRingPath = NULL;
    mPathIsDirty = TRUE;
    mRectIsDirty = TRUE;
}

//@Override
Boolean GradientDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Drawable::OnLevelChange(level);
    mRectIsDirty = TRUE;
    mPathIsDirty = TRUE;
    InvalidateSelf();
    return TRUE;
}

/**
 * This checks mRectIsDirty, and if it is true, recomputes both our drawing
 * rectangle (mRect) and the gradient itself, since it depends on our
 * rectangle too.
 * @return true if the resulting rectangle is not empty, false otherwise
 */
Boolean GradientDrawable::EnsureValidRect()
{
    if (mRectIsDirty) {
        mRectIsDirty = FALSE;

        AutoPtr<CRect> bounds = (CRect*)GetBounds().Get();
        Float inset = 0;

        if (mStrokePaint != NULL) {
            mStrokePaint->GetStrokeWidth(&inset);
            inset = inset * 0.5f;
        }

        GradientState* st = mGradientState;

        mRect->Set(bounds->mLeft + inset, bounds->mTop + inset,
                   bounds->mRight - inset, bounds->mBottom - inset);

        ArrayOf<Int32>* colors = st->mColors;
        if (colors != NULL) {
            CRectF* r = mRect;
            Float x0, x1, y0, y1;

            if (st->mGradient == GradientDrawable_LINEAR_GRADIENT) {
                Float level = st->mUseLevel ? (Float)GetLevel() / 10000.0f : 1.0f;
                switch (st->mOrientation) {
                case GradientDrawableOrientation_TOP_BOTTOM:
                    x0 = r->mLeft;          y0 = r->mTop;
                    x1 = x0;                y1 = level * r->mBottom;
                    break;
                case GradientDrawableOrientation_TR_BL:
                    x0 = r->mRight;         y0 = r->mTop;
                    x1 = level * r->mLeft;  y1 = level * r->mBottom;
                    break;
                case GradientDrawableOrientation_RIGHT_LEFT:
                    x0 = r->mRight;         y0 = r->mTop;
                    x1 = level * r->mLeft;  y1 = y0;
                    break;
                case GradientDrawableOrientation_BR_TL:
                    x0 = r->mRight;         y0 = r->mBottom;
                    x1 = level * r->mLeft;  y1 = level * r->mTop;
                    break;
                case GradientDrawableOrientation_BOTTOM_TOP:
                    x0 = r->mLeft;          y0 = r->mBottom;
                    x1 = x0;                y1 = level * r->mTop;
                    break;
                case GradientDrawableOrientation_BL_TR:
                    x0 = r->mLeft;          y0 = r->mBottom;
                    x1 = level * r->mRight; y1 = level * r->mTop;
                    break;
                case GradientDrawableOrientation_LEFT_RIGHT:
                    x0 = r->mLeft;          y0 = r->mTop;
                    x1 = level * r->mRight; y1 = y0;
                    break;
                default:/* TL_BR */
                    x0 = r->mLeft;          y0 = r->mTop;
                    x1 = level * r->mRight; y1 = level * r->mBottom;
                    break;
                }

                AutoPtr<IShader> shader;
                CLinearGradient::New(x0, y0, x1, y1, *colors,
                        st->mPositions, ShaderTileMode_CLAMP, (IShader**)&shader);
                mFillPaint->SetShader(shader);
            }
            else if (st->mGradient == GradientDrawable_RADIAL_GRADIENT) {
                x0 = r->mLeft + (r->mRight - r->mLeft) * st->mCenterX;
                y0 = r->mTop + (r->mBottom - r->mTop) * st->mCenterY;

                Float level = st->mUseLevel ? (Float)GetLevel() / 10000.0f : 1.0f;

                AutoPtr<IShader> shader;
                CRadialGradient::New(x0, y0,
                        level * st->mGradientRadius, *colors, NULL,
                        ShaderTileMode_CLAMP, (IShader**)&shader);
                mFillPaint->SetShader(shader);
            }
            else if (st->mGradient == GradientDrawable_SWEEP_GRADIENT) {
                x0 = r->mLeft + (r->mRight - r->mLeft) * st->mCenterX;
                y0 = r->mTop + (r->mBottom - r->mTop) * st->mCenterY;

                ArrayOf<Int32>* tempColors = colors;
                ArrayOf<Float>* tempPositions = NULL;

                if (st->mUseLevel) {
                    tempColors = st->mTempColors;
                    Int32 length = colors->GetLength();
                    if (tempColors == NULL || tempColors->GetLength() != length + 1) {
                        tempColors = st->mTempColors = ArrayOf<Int32>::Alloc(length + 1);
                    }
                    memcpy(tempColors->GetPayload(), colors->GetPayload(), length * sizeof(Int32));
                    (*tempColors)[length] = (*colors)[length - 1];

                    tempPositions = st->mTempPositions;
                    Float fraction = 1.0f / (Float)(length - 1);
                    if (tempPositions == NULL || tempPositions->GetLength() != length + 1) {
                        tempPositions = st->mTempPositions = ArrayOf<Float>::Alloc(length + 1);
                    }

                    Float level = (Float)GetLevel() / 10000.0f;
                    for (Int32 i = 0; i < length; i++) {
                        (*tempPositions)[i] = i * fraction * level;
                    }
                    (*tempPositions)[length] = 1.0f;

                }
                AutoPtr<IShader> shader;
                CSweepGradient::New(x0, y0, *tempColors, tempPositions, (IShader**)&shader);
                mFillPaint->SetShader(shader);
            }
        }
    }

    Boolean empty;
    mRect->IsEmpty(&empty);
    return !empty;
}

static Int32 R_Styleable_GradientDrawable[] = {
    0x01010194, 0x0101019a, 0x0101019b, 0x0101019c,
    0x0101019f, 0x0101025f, 0x01010260
};

static Int32 R_Styleable_GradientDrawableSize[] = {
    0x01010155, 0x01010159
};

static Int32 R_Styleable_GradientDrawableGradient[] = {
    0x0101019d, 0x0101019e, 0x0101019f, 0x010101a0,
    0x010101a1, 0x010101a2, 0x010101a3, 0x010101a4,
    0x0101020b
};

static Int32 R_Styleable_GradientDrawableSolid[] = {
    0x010101a5
};

static Int32 R_Styleable_GradientDrawableStroke[] = {
    0x01010159, 0x010101a5, 0x010101a6, 0x010101a7
};

static Int32 R_Styleable_DrawableCorners[] = {
    0x010101a8, 0x010101a9, 0x010101aa, 0x010101ab,
    0x010101ac
};

static Int32 R_Styleable_GradientDrawablePadding[] = {
    0x010101ad, 0x010101ae, 0x010101af, 0x010101b0
};

//@Override
ECode GradientDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    GradientState* st = mGradientState;

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_GradientDrawable,
                    sizeof(R_Styleable_GradientDrawable) / sizeof(Int32)),
            (ITypedArray**)&a);

    Drawable::InflateWithAttributes(r, parser, a,
            0/*com.android.internal.R.styleable.GradientDrawable_visible*/);

    Int32 shapeType;
    a->GetInt32(
            1/*com.android.internal.R.styleable.GradientDrawable_shape*/, GradientDrawable_RECTANGLE, &shapeType);

    if (shapeType == GradientDrawable_RING) {
        a->GetDimensionPixelSize(
                5/*com.android.internal.R.styleable.GradientDrawable_innerRadius*/, -1, &st->mInnerRadius);
        if (st->mInnerRadius == -1) {
            a->GetFloat(
                    2/*com.android.internal.R.styleable.GradientDrawable_innerRadiusRatio*/, 3.0f,
                    &st->mInnerRadiusRatio);
        }
        a->GetDimensionPixelSize(
                6/*com.android.internal.R.styleable.GradientDrawable_thickness*/, -1,
                &st->mThickness);
        if (st->mThickness == -1) {
            a->GetFloat(
                    3/*com.android.internal.R.styleable.GradientDrawable_thicknessRatio*/, 9.0f,
                    &st->mThicknessRatio);
        }
        a->GetBoolean(
                4/*com.android.internal.R.styleable.GradientDrawable_useLevel*/, TRUE,
                &st->mUseLevelForShape);
    }

    a->Recycle();

    SetShape(shapeType);

    Int32 type;

    Int32 innerDepth, depth;
    parser->GetDepth(&innerDepth);
    innerDepth += 1;
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && ((parser->GetDepth(&depth), depth >= innerDepth)
                   || type != IXmlPullParser_END_TAG)) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        if (depth > innerDepth) {
            continue;
        }

        String name;
        parser->GetName(&name);

        if (name.Equals("size")) {
            a = NULL;
            r->ObtainAttributes(attrs,
                    ArrayOf<Int32>(R_Styleable_GradientDrawableSize,
                            sizeof(R_Styleable_GradientDrawableSize) / sizeof(Int32)),
                    (ITypedArray**)&a);
            Int32 width, height;
            a->GetDimensionPixelSize(
                    1/*com.android.internal.R.styleable.GradientDrawableSize_width*/, -1, &width);
            a->GetDimensionPixelSize(
                    0/*com.android.internal.R.styleable.GradientDrawableSize_height*/, -1, &height);
            a->Recycle();
            SetSize(width, height);
        }
        else if (name.Equals("gradient")) {
            a = NULL;
            r->ObtainAttributes(attrs,
                    ArrayOf<Int32>(R_Styleable_GradientDrawableGradient,
                            sizeof(R_Styleable_GradientDrawableGradient) / sizeof(Int32)),
                    (ITypedArray**)&a);
            Int32 startColor;
            a->GetColor(
                    0/*com.android.internal.R.styleable.GradientDrawableGradient_startColor*/, 0,
                    &startColor);
            Boolean hasCenterColor;
            a->HasValue(
                    8/*com.android.internal.R.styleable.GradientDrawableGradient_centerColor*/,
                    &hasCenterColor);
            Int32 centerColor;
            a->GetColor(
                    8/*com.android.internal.R.styleable.GradientDrawableGradient_centerColor*/, 0,
                    &centerColor);
            Int32 endColor;
            a->GetColor(
                    1/*com.android.internal.R.styleable.GradientDrawableGradient_endColor*/, 0,
                    &endColor);
            Int32 gradientType;
            a->GetInt32(
                    4/*com.android.internal.R.styleable.GradientDrawableGradient_type*/,
                    GradientDrawable_LINEAR_GRADIENT, &gradientType);

            st->mCenterX = GetFloatOrFraction(
                    a,
                    5/*com.android.internal.R.styleable.GradientDrawableGradient_centerX*/,
                    0.5f);

            st->mCenterY = GetFloatOrFraction(
                    a,
                    6/*com.android.internal.R.styleable.GradientDrawableGradient_centerY*/,
                    0.5f);

            a->GetBoolean(
                    2/*com.android.internal.R.styleable.GradientDrawableGradient_useLevel*/, FALSE,
                    &st->mUseLevel);
            st->mGradient = gradientType;

            if (gradientType == GradientDrawable_LINEAR_GRADIENT) {
                Float anglef;
                a->GetFloat(
                        3/*com.android.internal.R.styleable.GradientDrawableGradient_angle*/, 0,
                        &anglef);
                Int32 angle = (Int32)anglef % 360;
                if (angle % 45 != 0) {
//                    throw new XmlPullParserException(a.getPositionDescription()
//                            + "<gradient> tag requires 'angle' attribute to "
//                            + "be a multiple of 45");
                    return E_XML_PULL_PARSER_EXCEPTION;
                }

                switch (angle) {
                case 0:
                    st->mOrientation = GradientDrawableOrientation_LEFT_RIGHT;
                    break;
                case 45:
                    st->mOrientation = GradientDrawableOrientation_BL_TR;
                    break;
                case 90:
                    st->mOrientation = GradientDrawableOrientation_BOTTOM_TOP;
                    break;
                case 135:
                    st->mOrientation = GradientDrawableOrientation_BR_TL;
                    break;
                case 180:
                    st->mOrientation = GradientDrawableOrientation_RIGHT_LEFT;
                    break;
                case 225:
                    st->mOrientation = GradientDrawableOrientation_TR_BL;
                    break;
                case 270:
                    st->mOrientation = GradientDrawableOrientation_TOP_BOTTOM;
                    break;
                case 315:
                    st->mOrientation = GradientDrawableOrientation_TL_BR;
                    break;
                }
            } else {
                AutoPtr<ITypedValue> tv;
                a->PeekValue(
                        7/*com.android.internal.R.styleable.GradientDrawableGradient_gradientRadius*/,
                        (ITypedValue**)&tv);
                if (tv != NULL) {
                    Boolean radiusRel = ((CTypedValue*)tv.Get())->mType == TypedValue_TYPE_FRACTION;
                    radiusRel ? tv->GetFraction(1.0f, 1.0f, &st->mGradientRadius)
                              : tv->GetFloat(&st->mGradientRadius);
                }
                else if (gradientType == GradientDrawable_RADIAL_GRADIENT) {
//                    throw new XmlPullParserException(
//                            a.getPositionDescription()
//                            + "<gradient> tag requires 'gradientRadius' "
//                            + "attribute with radial type");
                    return E_XML_PULL_PARSER_EXCEPTION;
                }
            }

            a->Recycle();

            if (hasCenterColor) {
                assert(st->mColors == NULL);
                st->mColors = ArrayOf<Int32>::Alloc(3);
                (*st->mColors)[0] = startColor;
                (*st->mColors)[1] = centerColor;
                (*st->mColors)[2] = endColor;

                assert(st->mPositions == NULL);
                st->mPositions = ArrayOf<Float>::Alloc(3);
                (*st->mPositions)[0] = 0.0f;
                // Since 0.5f is default value, try to take the one that isn't 0.5f
                (*st->mPositions)[1] = st->mCenterX != 0.5f ? st->mCenterX : st->mCenterY;
                (*st->mPositions)[2] = 1.0f;
            }
            else {
                assert(st->mColors == NULL);
                st->mColors = ArrayOf<Int32>::Alloc(2);
                (*st->mColors)[0] = startColor;
                (*st->mColors)[1] = endColor;
            }

        }
        else if (name.Equals("solid")) {
            a = NULL;
            r->ObtainAttributes(attrs,
                    ArrayOf<Int32>(R_Styleable_GradientDrawableSolid,
                            sizeof(R_Styleable_GradientDrawableSolid) / sizeof(Int32)),
                    (ITypedArray**)&a);
            Int32 argb;
            a->GetColor(
                    0/*com.android.internal.R.styleable.GradientDrawableSolid_color*/, 0,
                    &argb);
            a->Recycle();
            SetColor(argb);
        }
        else if (name.Equals("stroke")) {
            a = NULL;
            r->ObtainAttributes(attrs,
                    ArrayOf<Int32>(R_Styleable_GradientDrawableStroke,
                            sizeof(R_Styleable_GradientDrawableStroke) / sizeof(Int32)),
                    (ITypedArray**)&a);
            Int32 width;
            a->GetDimensionPixelSize(
                    0/*com.android.internal.R.styleable.GradientDrawableStroke_width*/, 0,
                    &width);
            Int32 color;
            a->GetColor(
                    1/*com.android.internal.R.styleable*.GradientDrawableStroke_color*/, 0,
                    &color);
            Float dashWidth;
            a->GetDimension(
                    2/*com.android.internal.R.styleable.GradientDrawableStroke_dashWidth*/, 0,
                    &dashWidth);
            if (dashWidth != 0.0f) {
                Float dashGap;
                a->GetDimension(
                        3/*com.android.internal.R.styleable.GradientDrawableStroke_dashGap*/, 0,
                        &dashGap);
                SetStroke(width, color, dashWidth, dashGap);
            }
            else {
                SetStroke(width, color);
            }
            a->Recycle();
        }
        else if (name.Equals("corners")) {
            a = NULL;
            r->ObtainAttributes(attrs,
                    ArrayOf<Int32>(R_Styleable_DrawableCorners,
                            sizeof(R_Styleable_DrawableCorners) / sizeof(Int32)),
                    (ITypedArray**)&a);
            Int32 radius;
            a->GetDimensionPixelSize(
                    0/*com.android.internal.R.styleable.DrawableCorners_radius*/, 0,
                    &radius);
            SetCornerRadius(radius);
            Int32 topLeftRadius;
            a->GetDimensionPixelSize(
                    1/*com.android.internal.R.styleable.DrawableCorners_topLeftRadius*/, radius,
                    &topLeftRadius);
            Int32 topRightRadius;
            a->GetDimensionPixelSize(
                    2/*com.android.internal.R.styleable.DrawableCorners_topRightRadius*/, radius,
                    &topRightRadius);
            Int32 bottomLeftRadius;
            a->GetDimensionPixelSize(
                    3/*com.android.internal.R.styleable.DrawableCorners_bottomLeftRadius*/, radius,
                    &bottomLeftRadius);
            Int32 bottomRightRadius;
            a->GetDimensionPixelSize(
                    4/*com.android.internal.R.styleable.DrawableCorners_bottomRightRadius*/, radius,
                    &bottomRightRadius);
            if (topLeftRadius != radius || topRightRadius != radius ||
                    bottomLeftRadius != radius || bottomRightRadius != radius) {
                ArrayOf_<Float, 8> radii;
                radii[0] = topLeftRadius;
                radii[1] = topLeftRadius;
                radii[2] = topRightRadius;
                radii[3] = topRightRadius;
                radii[4] = bottomLeftRadius;
                radii[5] = bottomLeftRadius;
                radii[6] = bottomRightRadius;
                radii[7] = bottomRightRadius;
                SetCornerRadii(&radii);
            }
            a->Recycle();
        }
        else if (name.Equals("padding")) {
            a = NULL;
            r->ObtainAttributes(attrs,
                    ArrayOf<Int32>(R_Styleable_GradientDrawablePadding,
                            sizeof(R_Styleable_GradientDrawablePadding) / sizeof(Int32)),
                    (ITypedArray**)&a);
            Int32 left, top, right, bottom;
            a->GetDimensionPixelOffset(
                    0/*com.android.internal.R.styleable.GradientDrawablePadding_left*/, 0,
                    &left),
            a->GetDimensionPixelOffset(
                    1/*com.android.internal.R.styleable.GradientDrawablePadding_top*/, 0,
                    &top),
            a->GetDimensionPixelOffset(
                    2/*com.android.internal.R.styleable.GradientDrawablePadding_right*/, 0,
                    &right),
            a->GetDimensionPixelOffset(
                    3/*com.android.internal.R.styleable.GradientDrawablePadding_bottom*/, 0,
                    &bottom);
            CRect::NewByFriend(left, top, right, bottom, (CRect**)&mPadding);
            a->Recycle();
            mGradientState->mPadding = mPadding;
        }
        else {
            Logger::W("drawable", String("Bad element under <shape>: ") + name);
        }
    }

    return NOERROR;
}

Float GradientDrawable::GetFloatOrFraction(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [in] */ Float defaultValue)
{
    AutoPtr<ITypedValue> tv;
    a->PeekValue(index, (ITypedValue**)&tv);
    Float v = defaultValue;
    if (tv != NULL) {
        Boolean vIsFraction = ((CTypedValue*)tv.Get())->mType == TypedValue_TYPE_FRACTION;
        vIsFraction ? tv->GetFraction(1.0f, 1.0f, &v) : tv->GetFloat(&v);
    }
    return v;
}

//@Override
Int32 GradientDrawable::GetIntrinsicWidth()
{
    return mGradientState->mWidth;
}

//@Override
Int32 GradientDrawable::GetIntrinsicHeight()
{
    return mGradientState->mHeight;
}

//@Override
AutoPtr<IDrawableConstantState> GradientDrawable::GetConstantState()
{
    mGradientState->mChangingConfigurations = Drawable::GetChangingConfigurations();
    return (IDrawableConstantState*)mGradientState.Get();
}

//@Override
AutoPtr<IDrawable> GradientDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mGradientState = new GradientState(mGradientState);
        InitializeWithState(mGradientState);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

GradientDrawable::GradientDrawable(
    /* [in] */ GradientState* state)
{
    mGradientState = state;
    InitializeWithState(state);
    mRectIsDirty = TRUE;
}

void GradientDrawable::InitializeWithState(
    /* [in] */ GradientState* state)
{
    if (state->mHasSolidColor) {
        mFillPaint->SetColor(state->mSolidColor);
    }
    mPadding = state->mPadding;
    if (state->mStrokeWidth >= 0) {
        CPaint::New(Paint_ANTI_ALIAS_FLAG, (IPaint**)&mStrokePaint);
        mStrokePaint->SetStyle(PaintStyle_STROKE);
        mStrokePaint->SetStrokeWidth(state->mStrokeWidth);
        mStrokePaint->SetColor(state->mStrokeColor);

        if (state->mStrokeDashWidth != 0.0f) {
            AutoPtr<IPathEffect> e;
            ArrayOf_<Float, 2> intervals;
            intervals[0] = state->mStrokeDashWidth;
            intervals[1] = state->mStrokeDashGap;
            CDashPathEffect::New(intervals, 0, (IDashPathEffect**)&e);
            mStrokePaint->SetPathEffect(e);
        }
    }
}

ECode GradientDrawable::Init()
{
    return Init(new GradientState(GradientDrawableOrientation_TOP_BOTTOM, NULL));
}

ECode GradientDrawable::Init(
    /* [in] */ GradientDrawableOrientation orientation,
    /* [in] */ ArrayOf<Int32>* colors)
{
    return Init(new GradientState(orientation, colors));
}

ECode GradientDrawable::Init(
    /* [in] */ GradientState* state)
{
    mGradientState = state;
    InitializeWithState(state);
    mRectIsDirty = TRUE;
    return NOERROR;
}
