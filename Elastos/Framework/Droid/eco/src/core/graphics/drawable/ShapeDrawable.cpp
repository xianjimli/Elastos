
#include "graphics/drawable/ShapeDrawable.h"
#include "graphics/drawable/CShapeDrawable.h"
#include "graphics/CPaint.h"
#include "graphics/CRect.h"
#include "graphics/ElPixelFormat.h"


ShapeDrawable::ShapeState::ShapeState(
    /* [in] */ ShapeState* orig)
    : mAlpha(255)
{
    if (orig != NULL) {
        mPaint = orig->mPaint;
        mShape = orig->mShape;
        mPadding = orig->mPadding;
        mIntrinsicWidth = orig->mIntrinsicWidth;
        mIntrinsicHeight = orig->mIntrinsicHeight;
        mAlpha = orig->mAlpha;
        mShaderFactory = orig->mShaderFactory;
    }
    else {
        CPaint::New(Paint_ANTI_ALIAS_FLAG, (IPaint**)&mPaint);
    }
}

PInterface ShapeDrawable::ShapeState::Probe(
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

UInt32 ShapeDrawable::ShapeState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ShapeDrawable::ShapeState::Release()
{
    return ElRefBase::Release();
}

ECode ShapeDrawable::ShapeState::GetInterfaceID(
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

//@Override
ECode ShapeDrawable::ShapeState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CShapeDrawable::New((Handle32)this, (IShapeDrawable**)drawable);
}

//@Override
ECode ShapeDrawable::ShapeState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CShapeDrawable::New((Handle32)this, (IShapeDrawable**)drawable);
}

//@Override
ECode ShapeDrawable::ShapeState::GetChangingConfigurations(
    /* [out] */ Int32* conf)
{
    VALIDATE_NOT_NULL(conf);
    *conf = mChangingConfigurations;
    return NOERROR;
}


/**
 * ShapeDrawable constructor.
 */
ShapeDrawable::ShapeDrawable()
{
//    this((ShapeState) null);
}

/**
 * Creates a ShapeDrawable with a specified Shape.
 *
 * @param s the Shape that this ShapeDrawable should be
 */
ShapeDrawable::ShapeDrawable(
    /* [in] */ IShape* s)
{
    mShapeState = new ShapeState(NULL);
    mShapeState->mShape = s;
}

ShapeDrawable::ShapeDrawable(
    /* [in] */ ShapeState* state)
{
    mShapeState = new ShapeState(state);
}

/**
 * Returns the Shape of this ShapeDrawable.
 */
AutoPtr<IShape> ShapeDrawable::GetShape()
{
    return mShapeState->mShape;
}

/**
 * Sets the Shape of this ShapeDrawable.
 */
ECode ShapeDrawable::SetShape(
    /* [in] */ IShape* s)
{
    mShapeState->mShape = s;
    UpdateShape();

    return NOERROR;
}

/**
 * Sets a ShaderFactory to which requests for a
 * {@link android.graphics.Shader} object will be made.
 *
 * @param fact an instance of your ShaderFactory implementation
 */
ECode ShapeDrawable::SetShaderFactory(
    /* [in] */ IShaderFactory* fact)
{
    mShapeState->mShaderFactory = fact;
    return NOERROR;
}

/**
 * Returns the ShaderFactory used by this ShapeDrawable for requesting a
 * {@link android.graphics.Shader}.
 */
AutoPtr<IShaderFactory> ShapeDrawable::GetShaderFactory()
{
    return mShapeState->mShaderFactory;
}

/**
 * Returns the Paint used to draw the shape.
 */
AutoPtr<IPaint> ShapeDrawable::GetPaint()
{
    return mShapeState->mPaint;
}

/**
 * Sets padding for the shape.
 *
 * @param left    padding for the left side (in pixels)
 * @param top     padding for the top (in pixels)
 * @param right   padding for the right side (in pixels)
 * @param bottom  padding for the bottom (in pixels)
 */
ECode ShapeDrawable::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if ((left | top | right | bottom) == 0) {
        mShapeState->mPadding = NULL;
    }
    else {
        if (mShapeState->mPadding == NULL) {
             CRect::New((IRect**)&mShapeState->mPadding);
        }
        mShapeState->mPadding->Set(left, top, right, bottom);
    }
    return NOERROR;
}

/**
 * Sets padding for this shape, defined by a Rect object.
 * Define the padding in the Rect object as: left, top, right, bottom.
 */
ECode ShapeDrawable::SetPadding(
    /* [in] */ IRect* padding)
{
    if (padding == NULL) {
        mShapeState->mPadding = NULL;
    }
    else {
        if (mShapeState->mPadding == NULL) {
            CRect::New((IRect**)&mShapeState->mPadding);
        }
        mShapeState->mPadding->SetEx(padding);
    }
    return NOERROR;
}

/**
 * Sets the intrinsic (default) width for this shape.
 *
 * @param width the intrinsic width (in pixels)
 */
ECode ShapeDrawable::SetIntrinsicWidth(
    /* [in] */ Int32 width)
{
    mShapeState->mIntrinsicWidth = width;
    return NOERROR;
}

/**
 * Sets the intrinsic (default) height for this shape.
 *
 * @param height the intrinsic height (in pixels)
 */
ECode ShapeDrawable::SetIntrinsicHeight(
    /* [in] */ Int32 height)
{
    mShapeState->mIntrinsicHeight = height;
    return NOERROR;
}

//@Override
Int32 ShapeDrawable::GetIntrinsicWidth()
{
    return mShapeState->mIntrinsicWidth;
}

//@Override
Int32 ShapeDrawable::GetIntrinsicHeight()
{
    return mShapeState->mIntrinsicHeight;
}

//@Override
Boolean ShapeDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    if (mShapeState->mPadding != NULL) {
        padding->SetEx(mShapeState->mPadding);
        return TRUE;
    }
    else {
        return Drawable::GetPadding(padding);
    }
}

Int32 ShapeDrawable::ModulateAlpha(
    /* [in] */ Int32 paintAlpha,
    /* [in] */ Int32 alpha)
{
    UInt32 u = alpha;
    Int32 scale = alpha + (u >> 7);  // convert to 0..256
    u = paintAlpha * scale;
    return (Int32)(u >> 8);
}

/**
 * Called from the drawable's draw() method after the canvas has been set
 * to draw the shape at (0,0). Subclasses can override for special effects
 * such as multiple layers, stroking, etc.
 */
void ShapeDrawable::OnDraw(
    /* [in] */ IShape* shape,
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    shape->Draw(canvas, paint);
}

//@Override
ECode ShapeDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IRect> r = GetBounds();
    IPaint* paint = mShapeState->mPaint;

    Int32 prevAlpha;
    paint->GetAlpha(&prevAlpha);
    paint->SetAlpha(ModulateAlpha(prevAlpha, mShapeState->mAlpha));

    if (mShapeState->mShape != NULL) {
        // need the save both for the translate, and for the (unknown) Shape
        Int32 count;
        canvas->Save(&count);
        canvas->Translate(
                static_cast<CRect*>(r.Get())->mLeft,
                static_cast<CRect*>(r.Get())->mTop);
        OnDraw(mShapeState->mShape, canvas, paint);
        canvas->RestoreToCount(count);
    }
    else {
        canvas->DrawRectEx(r, paint);
    }

    // restore
    paint->SetAlpha(prevAlpha);

    return NOERROR;
}

//@Override
Int32 ShapeDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
            | mShapeState->mChangingConfigurations;
}

/**
 * Set the alpha level for this drawable [0..255]. Note that this drawable
 * also has a color in its paint, which has an alpha as well. These two
 * values are automatically combined during drawing. Thus if the color's
 * alpha is 75% (i.e. 192) and the drawable's alpha is 50% (i.e. 128), then
 * the combined alpha that will be used during drawing will be 37.5%
 * (i.e. 96).
 */
//@Override
ECode ShapeDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    mShapeState->mAlpha = alpha;
    return NOERROR;
}

//@Override
ECode ShapeDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mShapeState->mPaint->SetColorFilter(cf);
}

//@Override
Int32 ShapeDrawable::GetOpacity()
{
    if (mShapeState->mShape == NULL) {
        IPaint* p = mShapeState->mPaint;
        AutoPtr<IXfermode> mode;
        if (p->GetXfermode((IXfermode**)&mode), mode == NULL) {
            Int32 alpha;
            p->GetAlpha(&alpha);
            if (alpha == 0) {
                return ElPixelFormat::TRANSPARENT;
            }
            if (alpha == 255) {
                return ElPixelFormat::OPAQUE;
            }
        }
    }
    // not sure, so be safe
    return ElPixelFormat::TRANSLUCENT;
}

//@Override
ECode ShapeDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    return mShapeState->mPaint->SetDither(dither);
}

//@Override
void ShapeDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    Drawable::OnBoundsChange(bounds);
    UpdateShape();
}

static Int32 R_Styleable_ShapeDrawablePadding[] = {
    0x010101ad, 0x010101ae, 0x010101af, 0x010101b0
};

/**
 * Subclasses override this to parse custom subelements.
 * If you handle it, return true, else return <em>super.inflateTag(...)</em>.
 */
Boolean ShapeDrawable::InflateTag(
    /* [in] */ const String& name,
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{

    if (name.Equals("padding")) {
        AutoPtr<ITypedArray> a;
        r->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_ShapeDrawablePadding,
                sizeof(R_Styleable_ShapeDrawablePadding) / sizeof(Int32)),/*com.android.internal.R.styleable.ShapeDrawablePadding*/
                (ITypedArray**)&a);
        Int32 left, top, right, bottom;
        a->GetDimensionPixelOffset(
                0/*com.android.internal.R.styleable.ShapeDrawablePadding_left*/,
                0, &left);
        a->GetDimensionPixelOffset(
                1/*com.android.internal.R.styleable.ShapeDrawablePadding_top*/,
                0, &top);
        a->GetDimensionPixelOffset(
                2/*com.android.internal.R.styleable.ShapeDrawablePadding_right*/,
                0, &right);
        a->GetDimensionPixelOffset(
                3/*com.android.internal.R.styleable.ShapeDrawablePadding_bottom*/,
                0, &bottom);
        SetPadding(left, top, right, bottom);
        a->Recycle();
        return TRUE;
    }

    return FALSE;
}

static Int32 R_Styleable_ShapeDrawable[] = {
    0x01010155, 0x01010159, 0x010101a5
};

//@Override
ECode ShapeDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_ShapeDrawable,
                sizeof(R_Styleable_ShapeDrawable) / sizeof(Int32)),/*com.android.internal.R.styleable.ShapeDrawable*/
                (ITypedArray**)&a);

    Int32 color;
    mShapeState->mPaint->GetColor(&color);
    a->GetColor(
        2/*com.android.internal.R.styleable.ShapeDrawable_color*/,
        color, &color);
    mShapeState->mPaint->SetColor(color);

    Float width, height;
    a->GetDimension(
            1/*com.android.internal.R.styleable.ShapeDrawable_width*/,
            0.0f, &width);
    a->GetDimension(
            0/*com.android.internal.R.styleable.ShapeDrawable_height*/,
            0.0f, &height);

    SetIntrinsicWidth((Int32)width);
    SetIntrinsicHeight((Int32)height);

    a->Recycle();

    Int32 type, outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG ||
              (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        String name;
        parser->GetName(&name);
        // call our subclass
        if (!InflateTag(name, r, parser, attrs)) {
//            android.util.Log.w("drawable", "Unknown element: " + name +
//                    " for ShapeDrawable " + this);
        }
    }

    return NOERROR;
}

void ShapeDrawable::UpdateShape()
{
    if (mShapeState->mShape != NULL) {
        AutoPtr<IRect> r = GetBounds();
        Int32 w, h;
        r->GetWidth(&w);
        r->GetHeight(&h);

        mShapeState->mShape->Resize(w, h);
        if (mShapeState->mShaderFactory != NULL) {
            AutoPtr<IShader> shader;
            mShapeState->mShaderFactory->Resize(w, h, (IShader**)&shader);
            mShapeState->mPaint->SetShader(shader);
        }
    }
}

//@Override
AutoPtr<IDrawableConstantState> ShapeDrawable::GetConstantState()
{
    mShapeState->mChangingConfigurations = Drawable::GetChangingConfigurations();
    return (IDrawableConstantState*)mShapeState;
}

//@Override
AutoPtr<IDrawable> ShapeDrawable::Mutate()
{
    if (!mMutated &&
            Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        AutoPtr<IPaint> paint;
        CPaint::New(mShapeState->mPaint, (IPaint**)&paint);
        mShapeState->mPaint = paint;

        AutoPtr<IRect> padding;
        CRect::New(mShapeState->mPadding, (IRect**)&padding);
        mShapeState->mPadding = padding;
//        try {
        AutoPtr<IShape> shape;
        mShapeState->mShape->Clone((IShape**)&shape);
        mShapeState->mShape = shape;
//        } catch (CloneNotSupportedException e) {
//            return null;
//        }
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode ShapeDrawable::Init()
{
    return Init((ShapeState*)NULL);
}

ECode ShapeDrawable::Init(
    /* [in] */ IShape* s)
{
    Init((ShapeState*)NULL);
    mShapeState->mShape = s;
    return NOERROR;
}

ECode ShapeDrawable::Init(
    /* [in] */ ShapeState* state)
{
    mShapeState = new ShapeState(state);
    return NOERROR;
}
