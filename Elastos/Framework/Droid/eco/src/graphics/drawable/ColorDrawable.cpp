
#include "graphics/drawable/ColorDrawable.h"
#include "graphics/CColorDrawable.h"
#include "graphics/ElPixelFormat.h"

static Int32 R_Styleable_ColorDrawable[] = {
    0x010101a5
};

ColorDrawable::ColorState::ColorState(
    /* [in] */ IColorState* state)
{
    if (state != NULL) {
        mBaseColor = ((ColorState*)state)->mBaseColor;
        mUseColor = ((ColorState*)state)->mUseColor;
    }
}

PInterface ColorDrawable::ColorState::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDrawableConstantState) {
        return (IDrawableConstantState*)this;
    }
    else if (riid == EIID_IColorState) {
        return (IColorState*)this;
    }

    return NULL;
}

UInt32 ColorDrawable::ColorState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ColorDrawable::ColorState::Release()
{
    return ElRefBase::Release();
}

ECode ColorDrawable::ColorState::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IColorState*)this) {
        *pIID = EIID_IColorState;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ColorDrawable::ColorState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CColorDrawable::New(this, (IColorDrawable**)drawable);
}

ECode ColorDrawable::ColorState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CColorDrawable::New(this, (IColorDrawable**)drawable);
}

ECode ColorDrawable::ColorState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    return mChangingConfigurations;
}

/**
 * Creates a new black ColorDrawable.
 */
ColorDrawable::ColorDrawable()
{
}

/**
 * Creates a new ColorDrawable with the specified color.
 *
 * @param color The color to draw.
 */
ColorDrawable::ColorDrawable(
    /* [in] */ Int32 color)
{
    InitEx(NULL);
    mState->mBaseColor = mState->mUseColor = color;
}

ColorDrawable::ColorDrawable(
    /* [in] */ IColorState* state)
{
    InitEx(state);
}

//@Override
Int32 ColorDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations() | mState->mChangingConfigurations;
}

//@Override
ECode ColorDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    return canvas->DrawColor(mState->mUseColor);
}

/**
 * Returns the alpha value of this drawable's color.
 *
 * @return A value between 0 and 255.
 */
Int32 ColorDrawable::GetAlpha()
{
    return (mState->mUseColor >> 24) & 0xFF;
}

/**
 * Sets the color's alpha value.
 *
 * @param alpha The alpha value to set, between 0 and 255.
 */
ECode ColorDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    alpha += alpha >> 7;   // make it 0..256
    Int32 baseAlpha = (mState->mBaseColor >> 24) & 0xFF;
    Int32 useAlpha = baseAlpha * alpha >> 8;
    mState->mUseColor = (mState->mBaseColor & 0x00FFFFFF) | (useAlpha << 24);

    return NOERROR;
}

/**
 * Setting a color filter on a ColorDrawable has no effect.
 *
 * @param colorFilter Ignore.
 */
ECode ColorDrawable::SetColorFilter(
    /* [in] */ IColorFilter* colorFilter)
{
    return NOERROR;
}

//@Override
Int32 ColorDrawable::GetOpacity()
{
    switch ((mState->mUseColor >> 24) & 0xFF) {
        case 255:
            return ElPixelFormat::OPAQUE;
        case 0:
            return ElPixelFormat::TRANSPARENT;
    }

    return ElPixelFormat::TRANSLUCENT;
}

//@Override
ECode ColorDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(
        attrs, ArrayOf<Int32>(R_Styleable_ColorDrawable, 1)/*com.android.internal.R.styleable.ColorDrawable*/,
        (ITypedArray**)&a));

    Int32 color;
    FAIL_RETURN(a->GetColor(
        0/*com.android.internal.R.styleable.ColorDrawable_color*/,
        mState->mBaseColor, &color));
    mState->mBaseColor = mState->mUseColor = color;

    return a->Recycle();
}

//@Override
AutoPtr<IDrawableConstantState> ColorDrawable::GetConstantState()
{
    mState->mChangingConfigurations = Drawable::GetChangingConfigurations();
    return IDrawableConstantState::Probe(mState.Get());
}

ECode ColorDrawable::Init(
    /* [in] */ Int32 color)
{
    InitEx(NULL);
    mState->mBaseColor = mState->mUseColor = color;

    return NOERROR;
}

ECode ColorDrawable::InitEx(
    /* [in] */ IColorState* state)
{
    mState = new ColorState(state);

    return NOERROR;
}
