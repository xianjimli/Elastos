
#include "graphics/drawable/ClipDrawable.h"
#include "graphics/drawable/CClipDrawable.h"
#include "view/CGravity.h"


ClipDrawable::ClipState::ClipState(
    /* [in] */ ClipState* orig,
    /* [in] */ IClipDrawable* owner,
    /* [in] */ IResources* res)
{
    if (orig != NULL) {
        AutoPtr<IDrawableConstantState> state;
        orig->mDrawable->GetConstantState((IDrawableConstantState**)&state);
        if (res != NULL) {
            state->NewDrawableEx(res, (IDrawable**)&mDrawable);
        }
        else {
            state->NewDrawable((IDrawable**)&mDrawable);
        }
        mDrawable->SetCallback(IDrawableCallback::Probe(owner));
        mOrientation = orig->mOrientation;
        mGravity = orig->mGravity;
        mCheckedConstantState = mCanConstantState = TRUE;
    }
}

PInterface ClipDrawable::ClipState::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDrawableConstantState) {
        return (IDrawableConstantState*)this;
    }
    else if (riid == EIID_IClipState) {
        return (IClipState*)this;
    }

    return NULL;
}

UInt32 ClipDrawable::ClipState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ClipDrawable::ClipState::Release()
{
    return ElRefBase::Release();
}

ECode ClipDrawable::ClipState::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ClipState*)this) {
        *pIID = EIID_IClipState;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ClipDrawable::ClipState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CClipDrawable::New(this, NULL, (IClipDrawable**)drawable);
}

ECode ClipDrawable::ClipState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CClipDrawable::New(this, res, (IClipDrawable**)drawable);
}

ECode ClipDrawable::ClipState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    return mChangingConfigurations;
}

Boolean ClipDrawable::ClipState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}


ClipDrawable::ClipDrawable()
{
    CRect::NewByFriend((CRect**)&mTmpRect);
//    mClipState = new ClipState(NULL, this, NULL);
}

/**
 * @param orientation Bitwise-or of {@link #HORIZONTAL} and/or {@link #VERTICAL}
 */
ClipDrawable::ClipDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 orientation)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
    ASSERT_SUCCEEDED(Init(drawable, gravity, orientation));
}

static Int32 R_Styleable_ClipDrawable[] = {
    0x010100af, 0x01010199, 0x0101020a
};

//@Override
ECode ClipDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    Int32 type;

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_ClipDrawable,
                    sizeof(R_Styleable_ClipDrawable) / sizeof(Int32)),
            (ITypedArray**)&a);

    Int32 orientation;
    a->GetInt32(
            2/*com.android.internal.R.styleable.ClipDrawable_clipOrientation*/,
            ClipDrawable_HORIZONTAL, &orientation);
    Int32 g;
    a->GetInt32(0/*com.android.internal.R.styleable.ClipDrawable_gravity*/,
            Gravity_LEFT, &g);
    AutoPtr<IDrawable> dr;
    a->GetDrawable(1/*com.android.internal.R.styleable.ClipDrawable_drawable*/, (IDrawable**)&dr);

    a->Recycle();

    Int32 outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
            && (type != IXmlPullParser_END_TAG || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }
        dr = NULL;
        FAIL_RETURN(Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr));
    }

    if (dr == NULL) {
//        throw new IllegalArgumentException("No drawable specified for <clip>");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mClipState->mDrawable = dr;
    mClipState->mOrientation = orientation;
    mClipState->mGravity = g;

    dr->SetCallback(
            (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));

    return NOERROR;
}

// overrides from Drawable.Callback

ECode ClipDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    if (mCallback != NULL) {
        mCallback->InvalidateDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable));
    }

    return NOERROR;
}

ECode ClipDrawable::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    if (mCallback != NULL) {
        mCallback->ScheduleDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable), what, when);
    }

    return NOERROR;
}

ECode ClipDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    if (mCallback != NULL) {
        mCallback->UnscheduleDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable), what);
    }

    return NOERROR;
}

// overrides from Drawable

//@Override
Int32 ClipDrawable::GetChangingConfigurations()
{
    Int32 configs;
    mClipState->mDrawable->GetChangingConfigurations(&configs);
    return Drawable::GetChangingConfigurations()
            | mClipState->mChangingConfigurations
            | configs;
}

//@Override
Boolean ClipDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    // XXX need to adjust padding!
    Boolean isPadding;
    mClipState->mDrawable->GetPadding(padding, &isPadding);
    return isPadding;
}

//@Override
Boolean ClipDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean different;
    mClipState->mDrawable->SetVisible(visible, restart, &different);
    return Drawable::SetVisible(visible, restart);
}

//@Override
ECode ClipDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mClipState->mDrawable->SetAlpha(alpha);
}

//@Override
ECode ClipDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mClipState->mDrawable->SetColorFilter(cf);
}

//@Override
Int32 ClipDrawable::GetOpacity()
{
    Int32 opacity;
    mClipState->mDrawable->GetOpacity(&opacity);
    return opacity;
}

//@Override
Boolean ClipDrawable::IsStateful()
{
    Boolean stateful;
    mClipState->mDrawable->IsStateful(&stateful);
    return stateful;
}

//@Override
Boolean ClipDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>& state)
{
    Boolean changed;
    mClipState->mDrawable->SetState(const_cast<ArrayOf<Int32>*>(&state), &changed);
    return changed;
}

//@Override
Boolean ClipDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Boolean changed;
    mClipState->mDrawable->SetLevel(level, &changed);
    InvalidateSelf();
    return TRUE;
}

//@Override
void ClipDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    mClipState->mDrawable->SetBoundsEx(bounds);
}

//@Override
ECode ClipDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 l;
    mClipState->mDrawable->GetLevel(&l);
    if (l == 0) {
        return NOERROR;
    }

    IRect* r = (IRect*)mTmpRect.Get();
    AutoPtr<IRect> bounds = GetBounds();
    Int32 level = GetLevel();
    Int32 w;
    bounds->GetWidth(&w);
    Int32 iw = 0; //mClipState.mDrawable.getIntrinsicWidth();
    if ((mClipState->mOrientation & ClipDrawable_HORIZONTAL) != 0) {
        w -= (w - iw) * (10000 - level) / 10000;
    }
    Int32 h;
    bounds->GetHeight(&h);
    Int32 ih = 0; //mClipState.mDrawable.getIntrinsicHeight();
    if ((mClipState->mOrientation & ClipDrawable_VERTICAL) != 0) {
        h -= (h - ih) * (10000 - level) / 10000;
    }
    AutoPtr<IGravity> gravity;
    CGravity::AcquireSingleton((IGravity**)&gravity);
    gravity->Apply(mClipState->mGravity, w, h, bounds, r);

    if (w > 0 && h > 0) {
        Int32 count;
        canvas->Save(&count);
        Boolean nonEmpty;
        canvas->ClipRectEx3(r, &nonEmpty);
        mClipState->mDrawable->Draw(canvas);
        canvas->Restore();
    }

    return NOERROR;
}

//@Override
Int32 ClipDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mClipState->mDrawable->GetIntrinsicWidth(&width);
    return width;
}

//@Override
Int32 ClipDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mClipState->mDrawable->GetIntrinsicHeight(&height);
    return height;
}

//@Override
AutoPtr<IDrawableConstantState> ClipDrawable::GetConstantState()
{
    if (mClipState->CanConstantState()) {
        mClipState->mChangingConfigurations = Drawable::GetChangingConfigurations();
        return mClipState;
    }
    return NULL;
}

ClipDrawable::ClipDrawable(
    /* [in] */ ClipState* state,
    /* [in] */ IResources* res)
{
    ASSERT_SUCCEEDED(Init(state, res));
}

ECode ClipDrawable::Init(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 orientation)
{
    FAIL_RETURN(Init(NULL, NULL));

    mClipState->mDrawable = drawable;
    mClipState->mGravity = gravity;
    mClipState->mOrientation = orientation;

    if (drawable != NULL) {
        drawable->SetCallback(
                (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    return NOERROR;
}

ECode ClipDrawable::Init(
    /* [in] */ ClipState* state,
    /* [in] */ IResources* res)
{
    mClipState = new ClipState(
            state, (IClipDrawable*)this->Probe(EIID_IClipDrawable), res);
    return NOERROR;
}
