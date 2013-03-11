
#include "graphics/drawable/NinePatchDrawable.h"
#include "graphics/drawable/CNinePatchDrawable.h"
#include "graphics/CBitmap.h"
#include "graphics/CRect.h"
#include "graphics/CPaint.h"
#include "graphics/CNinePatch.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/ElPixelFormat.h"
#include "utils/CDisplayMetrics.h"
#include "utils/CTypedValue.h"

////////////////////////////////////////////////////////////////////////////////
// NinePatchDrawable::NinePatchState
NinePatchDrawable::NinePatchState::NinePatchState(
    /* [in] */ INinePatch* ninePatch,
    /* [in] */ IRect* padding,
    /* [in] */ Boolean dither)
    : mNinePatch(ninePatch)
    , mPadding(padding)
    , mDither(dither)
    , mChangingConfigurations(0)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
{
}

NinePatchDrawable::NinePatchState::NinePatchState(
    /* [in] */ NinePatchState* state)
{
    ASSERT_SUCCEEDED(CNinePatch::New(state->mNinePatch, (INinePatch**)&mNinePatch));
    // Note we don't copy the padding because it is immutable.
    mPadding = state->mPadding;
    mDither = state->mDither;
    mChangingConfigurations = state->mChangingConfigurations;
    mTargetDensity = state->mTargetDensity;
}

PInterface NinePatchDrawable::NinePatchState::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDrawableConstantState) {
        return (IDrawableConstantState*)this;
    }

    return NULL;
}

UInt32 NinePatchDrawable::NinePatchState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NinePatchDrawable::NinePatchState::Release()
{
    return ElRefBase::Release();
}

ECode NinePatchDrawable::NinePatchState::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IDrawableConstantState*)this) {
        *pIID = EIID_IDrawableConstantState;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode NinePatchDrawable::NinePatchState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CNinePatchDrawable::New(
        reinterpret_cast<Handle32>(this), NULL, (INinePatchDrawable**)drawable);
}

ECode NinePatchDrawable::NinePatchState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CNinePatchDrawable::New(
        reinterpret_cast<Handle32>(this), res, (INinePatchDrawable**)drawable);
}

ECode NinePatchDrawable::NinePatchState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    return mChangingConfigurations;
}

////////////////////////////////////////////////////////////////////////////////
// NinePatchDrawable

const Boolean NinePatchDrawable::DEFAULT_DITHER;

NinePatchDrawable::NinePatchDrawable()
    : mMutated(FALSE)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte> & chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
    : mMutated(FALSE)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(bitmap, chunk, padding, srcName);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte> & chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
    : mMutated(FALSE)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(res, bitmap, chunk, padding, srcName);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ INinePatch* patch)
    : mMutated(FALSE)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(patch);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ IResources* res,
    /* [in] */ INinePatch* patch)
    : mMutated(FALSE)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(res, patch);
}

NinePatchDrawable::NinePatchDrawable(
    /* [in] */ NinePatchState* state,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    Init(state, res);
}

void NinePatchDrawable::SetNinePatchState(
    /* [in] */ NinePatchState* state,
    /* [in] */ IResources* res)
{
    mNinePatchState = state;
    mNinePatch = state->mNinePatch;
    mPadding = state->mPadding;
    mTargetDensity = state->mTargetDensity;
    if (res != NULL) {
        AutoPtr<IDisplayMetrics> dm;
        res->GetDisplayMetrics((IDisplayMetrics**)&dm);
        mTargetDensity = ((CDisplayMetrics*)dm.Get())->mDensityDpi;
    }

    if (DEFAULT_DITHER != state->mDither) {
        // avoid calling the setter unless we need to, since it does a
        // lazy allocation of a paint
        SetDither(state->mDither);
    }
    if (mNinePatch != NULL) {
        ComputeBitmapSize();
    }
}

/**
 * Set the density scale at which this drawable will be rendered. This
 * method assumes the drawable will be rendered at the same density as the
 * specified canvas.
 *
 * @param canvas The Canvas from which the density scale must be obtained.
 *
 * @see android.graphics.Bitmap#setDensity(int)
 * @see android.graphics.Bitmap#getDensity()
 */
ECode NinePatchDrawable::SetTargetDensity(
    /* [in] */ ICanvas* canvas)
{
    Int32 density;
    canvas->GetDensity(&density);
    return SetTargetDensity(density);
}

/**
 * Set the density scale at which this drawable will be rendered.
 *
 * @param metrics The DisplayMetrics indicating the density scale for this drawable.
 *
 * @see android.graphics.Bitmap#setDensity(int)
 * @see android.graphics.Bitmap#getDensity()
 */
ECode NinePatchDrawable::SetTargetDensity(
    /* [in] */ IDisplayMetrics* metrics)
{
    mTargetDensity = ((CDisplayMetrics*)metrics)->mDensityDpi;
    if (mNinePatch != NULL) {
        ComputeBitmapSize();
    }

    return NOERROR;
}

/**
 * Set the density at which this drawable will be rendered.
 *
 * @param density The density scale for this drawable.
 *
 * @see android.graphics.Bitmap#setDensity(int)
 * @see android.graphics.Bitmap#getDensity()
 */
ECode NinePatchDrawable::SetTargetDensity(
    /* [in] */ Int32 density)
{
    mTargetDensity = density == 0 ? CDisplayMetrics::DENSITY_DEFAULT : density;
    if (mNinePatch != NULL) {
        ComputeBitmapSize();
    }

    return NOERROR;
}

ECode NinePatchDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);

//	    if (false) {
//	        float[] pts = new float[2];
//	        canvas.getMatrix().mapPoints(pts);
//	        Log.v("9patch", "Drawing 9-patch @ " + pts[0] + "," + pts[1] + ": " + getBounds());
//	    }

    AutoPtr<IRect> bounds = Drawable::GetBounds();
    return mNinePatch->DrawEx2(canvas, bounds.Get(), mPaint.Get());
}

//@Override
Int32 NinePatchDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
        | mNinePatchState->mChangingConfigurations;
}

//@Override
Boolean NinePatchDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    padding->SetEx(mPadding);
    return TRUE;
}

//@Override
ECode NinePatchDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return GetPaint()->SetAlpha(alpha);
}

//@Override
ECode NinePatchDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return GetPaint()->SetColorFilter(cf);
}

ECode NinePatchDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    return GetPaint()->SetDither(dither);
}

//@Override
ECode NinePatchDrawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    // at the moment, we see no quality improvement, but a big slowdown
    // with filtering, so ignore this call for now
    //
    //getPaint().setFilterBitmap(filter);
    return NOERROR;
}

static Int32 R_Styleable_NinePatchDrawable[] = {
    0x01010119, 0x0101011c
};

ECode NinePatchDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Drawable::Inflate(r, parser, attrs);

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_NinePatchDrawable,
            sizeof(R_Styleable_NinePatchDrawable) / sizeof(Int32))/*com.android.internal.R.styleable.NinePatchDrawable*/,
            (ITypedArray**)&a));

    Int32 id;
    a->GetResourceId(0/*com.android.internal.R.styleable.NinePatchDrawable_src*/,
            0, &id);
    if (id == 0) {
//        throw new XmlPullParserException(parser.getPositionDescription() +
//                ": <nine-patch> requires a valid src attribute");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    Boolean dither;
    a->GetBoolean(
            1/*com.android.internal.R.styleable.NinePatchDrawable_dither*/,
            DEFAULT_DITHER, &dither);
    AutoPtr<CBitmapFactoryOptions> options;
    CBitmapFactoryOptions::NewByFriend((CBitmapFactoryOptions**)&options);
    if (dither) {
        options->mInDither = FALSE;
    }
    options->mInScreenDensity = CDisplayMetrics::DENSITY_DEVICE;

    AutoPtr<IRect> padding;
    CRect::New((IRect**)&padding);
    AutoPtr<IBitmap> bitmap;

//    try {
    AutoPtr<ITypedValue> value;
    CTypedValue::New((ITypedValue**)&value);
    AutoPtr<IInputStream> is;
    r->OpenRawResourceEx(id, value, (IInputStream**)&is);

    AutoPtr<IBitmapFactory> bmFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bmFactory);
    bmFactory->DecodeResourceStream(r, value, is, padding, options, (IBitmap**)&bitmap);

    is->Close();
//    } catch (IOException e) {
//        // Ignore
//    }

    AutoFree< ArrayOf<Byte> > data;
    if (bitmap == NULL) {
//        throw new XmlPullParserException(parser.getPositionDescription() +
//                ": <nine-patch> requires a valid src attribute");
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    else {
        bitmap->GetNinePatchChunk((ArrayOf<Byte>**)&data);
        if (data == NULL) {
//            throw new XmlPullParserException(parser.getPositionDescription() +
//                    ": <nine-patch> requires a valid 9-patch source image");
            return E_XML_PULL_PARSER_EXCEPTION;
        }

    }

    AutoPtr<INinePatch> np;
    CNinePatch::New(bitmap, *data, String("XML 9-patch"), (INinePatch**)&np);
    SetNinePatchState(new NinePatchState(np, padding, dither), r);
    mNinePatchState->mTargetDensity = mTargetDensity;

    a->Recycle();
    return NOERROR;
}

AutoPtr<IPaint> NinePatchDrawable::GetPaint()
{
    if (mPaint == NULL) {
        ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mPaint));
        mPaint->SetDither(DEFAULT_DITHER);
    }

    return mPaint;
}

/**
 * Retrieves the width of the source .png file (before resizing).
 */
//@Override
Int32 NinePatchDrawable::GetIntrinsicWidth()
{
    return mBitmapWidth;
}

/**
 * Retrieves the height of the source .png file (before resizing).
 */
//@Override
Int32 NinePatchDrawable::GetIntrinsicHeight()
{
    return mBitmapHeight;
}

//@Override
Int32 NinePatchDrawable::GetMinimumWidth()
{
    return mBitmapWidth;
}

//@Override
Int32 NinePatchDrawable::GetMinimumHeight()
{
    return mBitmapHeight;
}

//@Override
Int32 NinePatchDrawable::GetOpacity()
{
    Boolean hasAlpha;
    Int32 alpha;

    mNinePatch->HasAlpha(&hasAlpha);
    mPaint->GetAlpha(&alpha);

    return  (hasAlpha || (mPaint != NULL && alpha < 255) ?
               ElPixelFormat::TRANSLUCENT : ElPixelFormat::OPAQUE);
}

//@Override
AutoPtr<IRegion> NinePatchDrawable::GetTransparentRegion()
{
    AutoPtr<IRegion> region;
    mNinePatch->GetTransparentRegion(GetBounds(), (IRegion**)&region);

    return region;
}

//@Override
AutoPtr<IDrawableConstantState> NinePatchDrawable::GetConstantState()
{
    mNinePatchState->mChangingConfigurations = Drawable::GetChangingConfigurations();
    AutoPtr<IDrawableConstantState> ret = (IDrawableConstantState*)mNinePatchState.Get();
    return ret;
}

//@Override
AutoPtr<IDrawable> NinePatchDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get()
        == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mNinePatchState = new NinePatchState(mNinePatchState);
        mNinePatch = mNinePatchState->mNinePatch;
        mMutated = TRUE;
    }

    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode NinePatchDrawable::Init(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte> & chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
{
    AutoPtr<INinePatch> patch;
    FAIL_RETURN(CNinePatch::New(bitmap, chunk, srcName, (INinePatch**)&patch));

    AutoPtr<NinePatchState> patchState = new NinePatchState(patch.Get(), padding);
    if (patchState == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    return Init(patchState, NULL);
}

ECode NinePatchDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte> & chunk,
    /* [in] */ IRect* padding,
    /* [in] */ const String& srcName)
{
    VALIDATE_NOT_NULL(bitmap);

    AutoPtr<INinePatch> patch;
    FAIL_RETURN(CNinePatch::New(bitmap, chunk, srcName, (INinePatch**)&patch));
    AutoPtr<NinePatchState> patchState = new NinePatchState(patch.Get(), padding);
    if (patchState == NULL) return E_OUT_OF_MEMORY_ERROR;
    FAIL_RETURN(Init(patchState, res));
    mNinePatchState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode NinePatchDrawable::Init(
    /* [in] */ INinePatch* patch)
{
    AutoPtr<IRect> rect;
    FAIL_RETURN(CRect::New((IRect**)&rect));

    AutoPtr<NinePatchState> patchState = new NinePatchState(patch, rect);
    if (patchState == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    return Init(patchState, NULL);
}

ECode NinePatchDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ INinePatch* patch)
{
    AutoPtr<IRect> rect;
    FAIL_RETURN(CRect::New((IRect**)&rect));

    AutoPtr<NinePatchState> patchState = new NinePatchState(patch, rect);
    if (patchState == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    FAIL_RETURN(Init(patchState, res));

    mNinePatchState->mTargetDensity = mTargetDensity;

    return NOERROR;
}

ECode NinePatchDrawable::Init(
    /* [in] */ NinePatchState* state,
    /* [in] */ IResources* res)
{
    SetNinePatchState(state, res);
    return NOERROR;
}

ECode NinePatchDrawable::ComputeBitmapSize()
{
    Int32 sdensity = 0;
    mNinePatch->GetDensity(&sdensity);
    const Int32 tdensity = mTargetDensity;
    if (sdensity == tdensity) {
        mNinePatch->GetWidth(&mBitmapWidth);
        mNinePatch->GetHeight(&mBitmapHeight);
    }
    else {
        Int32 width = 0, height = 0;
        mNinePatch->GetWidth(&width);
        mNinePatch->GetHeight(&height);
        mBitmapWidth = CBitmap::ScaleFromDensity(width, sdensity, tdensity);
        mBitmapHeight = CBitmap::ScaleFromDensity(height, sdensity, tdensity);
        if (mNinePatchState->mPadding != NULL && mPadding != NULL) {
            AutoPtr<IRect> dest = mPadding;
            AutoPtr<IRect> src = mNinePatchState->mPadding;
            if (dest == src) {
                FAIL_RETURN(CRect::New(src, (IRect**)&mPadding));
                dest = mPadding;
            }
            CRect* csrc = (CRect*)src.Get();
            CRect* cdest = (CRect*)dest.Get();
            cdest->mLeft = CBitmap::ScaleFromDensity(csrc->mLeft, sdensity, tdensity);
            cdest->mTop = CBitmap::ScaleFromDensity(csrc->mTop, sdensity, tdensity);
            cdest->mRight = CBitmap::ScaleFromDensity(csrc->mRight, sdensity, tdensity);
            cdest->mBottom = CBitmap::ScaleFromDensity(csrc->mBottom, sdensity, tdensity);
        }
    }

    return NOERROR;
}

