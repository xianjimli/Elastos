
#include "graphics/drawable/BitmapDrawable.h"
#include "graphics/CPaint.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CRect.h"
#include "graphics/CBitmapShader.h"
#include "graphics/ElPixelFormat.h"
#include "graphics/drawable/CBitmapDrawable.h"
#include "view/CGravity.h"
#include "utils/CDisplayMetrics.h"
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;


BitmapDrawable::BitmapState::BitmapState(
    /* [in] */ IBitmap* bitmap)
    : mBitmap(bitmap)
    , mGravity(Gravity_FILL)
    , mTileModeX(-1)
    , mTileModeY(-1)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
{
    ASSERT_SUCCEEDED(CPaint::New(DEFAULT_PAINT_FLAGS, (IPaint**)&mPaint));
}

BitmapDrawable::BitmapState::BitmapState(
    /* [in] */ IBitmapState* bitmap)
    : mGravity(Gravity_FILL)
    , mTileModeX(-1)
    , mTileModeY(-1)
    , mTargetDensity(DisplayMetrics_DENSITY_DEFAULT)
{
    BitmapState* state = (BitmapState*)bitmap;
    mBitmap = state->mBitmap;
    mChangingConfigurations = state->mChangingConfigurations;
    mGravity = state->mGravity;
    mTileModeX = state->mTileModeX;
    mTileModeY = state->mTileModeY;
    mTargetDensity = state->mTargetDensity;
    ASSERT_SUCCEEDED(CPaint::New(state->mPaint, (IPaint**)&mPaint));
}

PInterface BitmapDrawable::BitmapState::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDrawableConstantState) {
        return (IDrawableConstantState*)this;
    }
    else if (riid == EIID_IBitmapState) {
        return (IBitmapState*)this;
    }

    return NULL;
}

UInt32 BitmapDrawable::BitmapState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BitmapDrawable::BitmapState::Release()
{
    return ElRefBase::Release();
}

ECode BitmapDrawable::BitmapState::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IBitmapState*)this) {
        *pIID = EIID_IBitmapState;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode BitmapDrawable::BitmapState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CBitmapDrawable::New((IBitmapState*)this, NULL, (IBitmapDrawable**)drawable);
}

ECode BitmapDrawable::BitmapState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CBitmapDrawable::New((IBitmapState*)this, res, (IBitmapDrawable**)drawable);
}

ECode BitmapDrawable::BitmapState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    return mChangingConfigurations;
}

//////////////////////////////////////////////////////////////////////
const Int32 BitmapDrawable::DEFAULT_PAINT_FLAGS =
        Paint_FILTER_BITMAP_FLAG | Paint_DITHER_FLAG;

BitmapDrawable::BitmapDrawable()
    : mApplyGravity(FALSE)
    , mRebuildShader(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IResources* res)
    : mApplyGravity(FALSE)
    , mRebuildShader(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(res);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IBitmap* bitmap)
    : mApplyGravity(FALSE)
    , mRebuildShader(FALSE)
    , mMutated(FALSE)
    , mBitmapWidth(0)
    , mBitmapHeight(0)
{
    CRect::New((IRect**)&mDstRect);
    Init(bitmap);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap)
{
    CRect::New((IRect**)&mDstRect);
    Init(res, bitmap);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ const String& filepath)
{
    CRect::New((IRect**)&mDstRect);
    Init(filepath);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IResources* res,
    /* [in] */ const String& filepath)
{
    CRect::New((IRect**)&mDstRect);
    Init(res, filepath);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IInputStream* is)
{
    CRect::New((IRect**)&mDstRect);
    Init(is);
}

BitmapDrawable::BitmapDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IInputStream* is)
{
    CRect::New((IRect**)&mDstRect);
    Init(res, is);
}

AutoPtr<IPaint> BitmapDrawable::GetPaint()
{
    return mBitmapState->mPaint;
}

AutoPtr<IBitmap> BitmapDrawable::GetBitmap()
{
    return mBitmap;
}

void BitmapDrawable::ComputeBitmapSize()
{
    mBitmap->GetScaledWidthEx2(mTargetDensity, &mBitmapWidth);
    mBitmap->GetScaledHeightEx2(mTargetDensity, &mBitmapHeight);
}

void BitmapDrawable::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
    mBitmap = bitmap;
    if (bitmap != NULL) {
        ComputeBitmapSize();
    }
    else {
        mBitmapWidth = mBitmapHeight = -1;
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
ECode BitmapDrawable::SetTargetDensity(
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
ECode BitmapDrawable::SetTargetDensity(
    /* [in] */ IDisplayMetrics* metrics)
{
    mTargetDensity = ((CDisplayMetrics*)metrics)->mDensityDpi;
    if (mBitmap != NULL) {
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
ECode BitmapDrawable::SetTargetDensity(
    /* [in] */ Int32 density)
{
    mTargetDensity = density == 0 ? CDisplayMetrics::DENSITY_DEFAULT : density;
    if (mBitmap != NULL) {
        ComputeBitmapSize();
    }

    return NOERROR;
}

/** Get the gravity used to position/stretch the bitmap within its bounds.
 * See android.view.Gravity
 * @return the gravity applied to the bitmap
 */
Int32 BitmapDrawable::GetGravity()
{
    return mBitmapState->mGravity;
}

/** Set the gravity used to position/stretch the bitmap within its bounds.
    See android.view.Gravity
 * @param gravity the gravity
 */
ECode BitmapDrawable::SetGravity(
    /* [in] */ Int32 gravity)
{
    mBitmapState->mGravity = gravity;
    mApplyGravity = TRUE;

    return NOERROR;
}

ECode BitmapDrawable::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    return mBitmapState->mPaint->SetAntiAlias(aa);
}

//@Override
ECode BitmapDrawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    return mBitmapState->mPaint->SetFilterBitmap(filter);
}

//@Override
ECode BitmapDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    return mBitmapState->mPaint->SetDither(dither);
}

ShaderTileMode BitmapDrawable::GetTileModeX()
{
    return mBitmapState->mTileModeX;
}

ShaderTileMode BitmapDrawable::GetTileModeY()
{
    return mBitmapState->mTileModeY;
}

ECode BitmapDrawable::SetTileModeX(
    /* [in] */ ShaderTileMode mode)
{
    return SetTileModeXY(mode, mBitmapState->mTileModeY);
}

//final
ECode BitmapDrawable::SetTileModeY(
    /* [in] */ ShaderTileMode mode)
{
    return SetTileModeXY(mBitmapState->mTileModeX, mode);
}

ECode BitmapDrawable::SetTileModeXY(
    /* [in] */ ShaderTileMode xmode,
    /* [in] */ ShaderTileMode ymode)
{
    BitmapState* state = mBitmapState;
    AutoPtr<IShader> shader;
    state->mPaint->GetShader((IShader**)&shader);
    if (shader == NULL || state->mTileModeX != xmode
        || state->mTileModeY != ymode) {
        state->mTileModeX = xmode;
        state->mTileModeY = ymode;
        mRebuildShader = TRUE;
    }

    return NOERROR;
}

//@Override
Int32 BitmapDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
        | mBitmapState->mChangingConfigurations;
}

void BitmapDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    Drawable::OnBoundsChange(bounds);
    mApplyGravity = TRUE;
}

ECode BitmapDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IBitmap> bitmap = mBitmap;
    if (bitmap != NULL) {
        BitmapState* state = mBitmapState;
        if (mRebuildShader) {
            ShaderTileMode tmx = state->mTileModeX;
            ShaderTileMode tmy = state->mTileModeY;

            if (tmx == -1 && tmy == -1) {
                state->mPaint->SetShader(NULL);
            }
            else {
                AutoPtr<IShader> s;
                CBitmapShader::New(bitmap,
                        tmx == -1 ? ShaderTileMode_CLAMP : tmx,
                        tmy == -1 ? ShaderTileMode_CLAMP : tmy, (IShader**)&s);
                state->mPaint->SetShader(s);
            }
            mRebuildShader = FALSE;
            CopyBounds(mDstRect);
        }

        AutoPtr<IShader> shader;
        state->mPaint->GetShader((IShader**)&shader);
        if (shader == NULL) {
            if (mApplyGravity) {
                AutoPtr<IGravity> gravity;
                CGravity::AcquireSingleton((IGravity**)&gravity);
                gravity->Apply(state->mGravity, mBitmapWidth, mBitmapHeight,
                        GetBounds(), mDstRect);
                mApplyGravity = FALSE;
            }
            canvas->DrawBitmapEx2(bitmap.Get(), NULL, mDstRect, state->mPaint);
        }
        else {
            if (mApplyGravity) {
                mDstRect->SetEx(GetBounds());
                mApplyGravity = FALSE;
            }
            canvas->DrawRectEx(mDstRect, state->mPaint);
        }
    }
    return NOERROR;
}

ECode BitmapDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mBitmapState->mPaint->SetAlpha(alpha);
}

ECode BitmapDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mBitmapState->mPaint->SetColorFilter(cf);
}

/**
 * A mutable BitmapDrawable still shares its Bitmap with any other Drawable
 * that comes from the same resource.
 *
 * @return This drawable.
 */
//@Override
AutoPtr<IDrawable> BitmapDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get()
        == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mBitmapState = new BitmapState(mBitmapState.Get());
        mMutated = TRUE;
    }

    return (IDrawable*)this->Probe(EIID_IDrawable);
}

static Int32 R_Styleable_BitmapDrawable[] = {
    0x010100af, 0x01010119, 0x0101011a, 0x0101011b,
    0x0101011c, 0x01010201
};

//@Override
ECode BitmapDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(
        attrs, ArrayOf<Int32>(R_Styleable_BitmapDrawable, sizeof(R_Styleable_BitmapDrawable) / sizeof(Int32)),/*com.android.internal.R.styleable.BitmapDrawable*/
        (ITypedArray**)&a));

    Int32 id;
    a->GetResourceId(
        1/*com.android.internal.R.styleable.BitmapDrawable_src*/, 0, &id);
    if (id == 0) {
        String str;
        parser->GetPositionDescription(&str);
        Logger::E("BitmapDrawable",
            StringBuffer(str) + ": <bitmap> requires a valid src attribute");

        return E_XML_PULL_PARSER_EXCEPTION;
    }

    AutoPtr<IBitmapFactory> bmFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bmFactory);
    AutoPtr<IBitmap> bitmap;
    bmFactory->DecodeResourceEx(r, id, (IBitmap**)&bitmap);

    if (bitmap == NULL) {
        String str;
        parser->GetPositionDescription(&str);
        Logger::E("BitmapDrawable",
            StringBuffer(str) + ": <bitmap> requires a valid src attribute");

        return E_XML_PULL_PARSER_EXCEPTION;
    }

    mBitmapState->mBitmap = bitmap;
    SetBitmap(bitmap);

    AutoPtr<IDisplayMetrics> metrics;
    r->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    SetTargetDensity(metrics);

    IPaint* paint = mBitmapState->mPaint;
    Boolean newValue, oldValue;
    paint->IsAntiAlias(&oldValue);
    a->GetBoolean(2, /*com.android.internal.R.styleable.BitmapDrawable_antialias*/
        oldValue, &newValue);
    paint->SetAntiAlias(newValue);

    paint->IsFilterBitmap(&oldValue);
    a->GetBoolean(3, /*com.android.internal.R.styleable.BitmapDrawable_filter*/
        oldValue, &newValue);
    paint->SetFilterBitmap(newValue);

    paint->IsDither(&oldValue);
    a->GetBoolean(4, /*com.android.internal.R.styleable.BitmapDrawable_dither*/
        oldValue, &newValue);
    paint->SetDither(newValue);

    Int32 gravity;
    a->GetInt32(0,/*com.android.internal.R.styleable.BitmapDrawable_gravity*/
        Gravity_FILL, &gravity);
    SetGravity(gravity);

    Int32 tileMode;
    a->GetInt32(5,/*com.android.internal.R.styleable.BitmapDrawable_tileMode*/
        -1, &tileMode);
    if (tileMode != -1) {
        switch (tileMode) {
        case 0:
            SetTileModeXY(ShaderTileMode_CLAMP, ShaderTileMode_CLAMP);
            break;
        case 1:
            SetTileModeXY(ShaderTileMode_REPEAT, ShaderTileMode_REPEAT);
            break;
        case 2:
            SetTileModeXY(ShaderTileMode_MIRROR, ShaderTileMode_MIRROR);
            break;
        }
    }

    return a->Recycle();
}

Int32 BitmapDrawable::GetIntrinsicWidth()
{
    return mBitmapWidth;
}

Int32 BitmapDrawable::GetIntrinsicHeight()
{
    return mBitmapHeight;
}

Int32 BitmapDrawable::GetOpacity()
{
    if (mBitmapState->mGravity != Gravity_FILL) {
        return ElPixelFormat::TRANSLUCENT;
    }

    Boolean bmHasAlpha = TRUE;
    if (mBitmap != NULL) {
        mBitmap->HasAlpha(&bmHasAlpha);
    }

    Int32 alpha;
    mBitmapState->mPaint->GetAlpha(&alpha);

    return (bmHasAlpha || alpha < 255) ?
        ElPixelFormat::TRANSLUCENT : ElPixelFormat::OPAQUE;
}

//@Override
AutoPtr<IDrawableConstantState> BitmapDrawable::GetConstantState()
{
    mBitmapState->mChangingConfigurations = Drawable::GetChangingConfigurations();

    return mBitmapState;
}

ECode BitmapDrawable::Init(
    /* [in] */ BitmapState* state,
    /* [in] */ IResources* res)
{
    mBitmapState = state;
    if (res != NULL) {
        AutoPtr<IDisplayMetrics> metrics;
        res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
        mTargetDensity = ((CDisplayMetrics*)metrics.Get())->mDensityDpi;
    }
    else if (state != NULL) {
        mTargetDensity = state->mTargetDensity;
    }
    else {
        mTargetDensity = DisplayMetrics_DENSITY_DEFAULT;
    }

    SetBitmap(state != NULL ? state->mBitmap : NULL);

    return NOERROR;
}

ECode BitmapDrawable::Init()
{
    mBitmapState = new BitmapState((IBitmap*)NULL);

    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IResources* res)
{
    mBitmapState = new BitmapState((IBitmap*)NULL);
    mBitmapState->mTargetDensity = mTargetDensity;

    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IBitmap* bitmap)
{
    return Init(new BitmapState(bitmap), NULL);
}

ECode BitmapDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ IBitmap* bitmap)
{
    Init(new BitmapState(bitmap), res);
    mBitmapState->mTargetDensity = mTargetDensity;
    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ const String& filepath)
{
    AutoPtr<IBitmapFactory> factory;
    AutoPtr<IBitmap> bitmap;

    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));

    factory->DecodeFileEx(filepath, (IBitmap**)&bitmap);

    Init(new BitmapState(bitmap), NULL);

    if (mBitmap == NULL) {
        Logger::W("BitmapDrawable",
            StringBuffer("BitmapDrawable cannot decode ") + filepath);
    }

    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ const String& filepath)
{
    FAIL_RETURN(Init(filepath));
    mBitmapState->mTargetDensity = mTargetDensity;

    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IInputStream* is)
{
    AutoPtr<IBitmapFactory> factory;
    AutoPtr<IBitmap> bitmap;

    FAIL_RETURN(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));

    factory->DecodeStreamEx(is, (IBitmap**)&bitmap);

    Init(new BitmapState(bitmap), NULL);

    if (mBitmap == NULL) {
        Logger::W("BitmapDrawable",
            StringBuffer("BitmapDrawable cannot decode ")/* + is*/);
    }

    return NOERROR;
}

ECode BitmapDrawable::Init(
    /* [in] */ IResources* res,
    /* [in] */ IInputStream* is)
{
    FAIL_RETURN(Init(is));
    mBitmapState->mTargetDensity = mTargetDensity;

    return NOERROR;
}
