
#include "graphics/drawable/ScaleDrawable.h"
#include "graphics/drawable/CScaleDrawable.h"
#include "view/CGravity.h"


ScaleDrawable::ScaleState::ScaleState(
    /* [in] */ ScaleState* orig,
    /* [in] */ ScaleDrawable* owner,
    /* [in] */ IResources* res)
    : mCheckedConstantState(FALSE)
    , mCanConstantState(FALSE)
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
        mDrawable->SetCallback((IDrawableCallback*)owner->Probe(EIID_IDrawableCallback));
        mScaleWidth = orig->mScaleWidth;
        mScaleHeight = orig->mScaleHeight;
        mGravity = orig->mGravity;
        mCheckedConstantState = mCanConstantState = TRUE;
    }
}

PInterface ScaleDrawable::ScaleState::Probe(
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

UInt32 ScaleDrawable::ScaleState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ScaleDrawable::ScaleState::Release()
{
    return ElRefBase::Release();
}

ECode ScaleDrawable::ScaleState::GetInterfaceID(
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

ECode ScaleDrawable::ScaleState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CScaleDrawable::New((Handle32)this, NULL,
            (IScaleDrawable**)drawable);
}

ECode ScaleDrawable::ScaleState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CScaleDrawable::New((Handle32)this, res,
            (IScaleDrawable**)drawable);
}

ECode ScaleDrawable::ScaleState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

Boolean ScaleDrawable::ScaleState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}


ScaleDrawable::ScaleDrawable()
{
    CRect::NewByFriend((CRect**)&mTmpRect);
//    this(null, null);
}

ScaleDrawable::ScaleDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Float scaleWidth,
    /* [in] */ Float scaleHeight)
{
    CRect::NewByFriend((CRect**)&mTmpRect);

    ASSERT_SUCCEEDED(Init(drawable, gravity, scaleWidth, scaleHeight));
}

ScaleDrawable::ScaleDrawable(
    /* [in] */ ScaleState* state,
    /* [in] */ IResources* res)
{
    mScaleState = new ScaleState(state, this, res);
}

/**
 * Returns the drawable scaled by this ScaleDrawable.
 */
AutoPtr<IDrawable> ScaleDrawable::GetDrawable()
{
    return mScaleState->mDrawable;
}

Float ScaleDrawable::GetPercent(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 name)
{
    String s;
    a->GetString(name, &s);
    if (!s.IsNull()) {
        if (s.EndWith("%")) {
            String f = s.Substring(0, s.GetLength() - 1);
            return ((Float)f.ToDouble()) / 100.0f;
        }
    }
    return -1.0f;
}

static Int32 R_Styleable_ScaleDrawable[] = {
    0x01010199, 0x010101fc, 0x010101fd, 0x010101fe
};

//@Override
ECode ScaleDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    Int32 type;

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_ScaleDrawable,
            sizeof(R_Styleable_ScaleDrawable) / sizeof(Int32)),/*com.android.internal.R.styleable.ScaleDrawable*/
            (ITypedArray**)&a);

    Float sw = GetPercent(a, 1/*com.android.internal.R.styleable.ScaleDrawable_scaleWidth*/);
    Float sh = GetPercent(a, 2/*com.android.internal.R.styleable.ScaleDrawable_scaleHeight*/);
    Int32 g;
    a->GetInt32(
            3/*com.android.internal.R.styleable.ScaleDrawable_scaleGravity*/,
            Gravity_LEFT, &g);
    AutoPtr<IDrawable> dr;
    a->GetDrawable(0/*com.android.internal.R.styleable.ScaleDrawable_drawable*/, (IDrawable**)&dr);

    a->Recycle();

    Int32 outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
            && (type != IXmlPullParser_END_TAG || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }
        dr = NULL;
        Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
    }

    if (dr == NULL) {
//        throw new IllegalArgumentException("No drawable specified for <scale>");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mScaleState->mDrawable = dr;
    mScaleState->mScaleWidth = sw;
    mScaleState->mScaleHeight = sh;
    mScaleState->mGravity = g;
    if (dr != NULL) {
        dr->SetCallback(
                (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }

    return NOERROR;
}

ECode ScaleDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    if (mCallback != NULL) {
        mCallback->InvalidateDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable));
    }

    return NOERROR;
}

ECode ScaleDrawable::ScheduleDrawable(
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

ECode ScaleDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    if (mCallback != NULL) {
        mCallback->UnscheduleDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable), what);
    }

    return NOERROR;
}

//@Override
ECode ScaleDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 level;
    mScaleState->mDrawable->GetLevel(&level);
    if (level != 0) {
        mScaleState->mDrawable->Draw(canvas);
    }

    return NOERROR;
}

//@Override
Int32 ScaleDrawable::GetChangingConfigurations()
{
    Int32 conf;
    mScaleState->mDrawable->GetChangingConfigurations(&conf);
    return Drawable::GetChangingConfigurations()
            | mScaleState->mChangingConfigurations
            | conf;
}

//@Override
Boolean ScaleDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    // XXX need to adjust padding!
    Boolean pad;
    mScaleState->mDrawable->GetPadding(padding, &pad);
    return pad;
}

//@Override
Boolean ScaleDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean isDifferent;
    mScaleState->mDrawable->SetVisible(visible, restart, &isDifferent);
    return Drawable::SetVisible(visible, restart);
}

ECode ScaleDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mScaleState->mDrawable->SetAlpha(alpha);
}

ECode ScaleDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mScaleState->mDrawable->SetColorFilter(cf);
}

Int32 ScaleDrawable::GetOpacity()
{
    Int32 opacity;
    mScaleState->mDrawable->GetOpacity(&opacity);
    return opacity;
}

//@Override
Boolean ScaleDrawable::IsStateful()
{
    Boolean isStateful;
    mScaleState->mDrawable->IsStateful(&isStateful);
    return isStateful;
}

//@Override
Boolean ScaleDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    Boolean changed;
    mScaleState->mDrawable->SetState(
        const_cast<ArrayOf<Int32>*>(state), &changed);
    OnBoundsChange(GetBounds());
    return changed;
}

//@Override
Boolean ScaleDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Boolean changed;
    mScaleState->mDrawable->SetLevel(level, &changed);
    OnBoundsChange(GetBounds());
    InvalidateSelf();
    return TRUE;
}

//@Override
void ScaleDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* r = mTmpRect;
    Int32 level = GetLevel();
    Int32 w;
    bounds->GetWidth(&w);
    const Int32 iw = 0; //mScaleState.mDrawable.getIntrinsicWidth();
    if (mScaleState->mScaleWidth > 0) {
        w -= (Int32)((w - iw) * (10000 - level) * mScaleState->mScaleWidth / 10000);
    }
    Int32 h;
    bounds->GetHeight(&h);
    const Int32 ih = 0; //mScaleState.mDrawable.getIntrinsicHeight();
    if (mScaleState->mScaleHeight > 0) {
        h -= (Int32)((h - ih) * (10000 - level) * mScaleState->mScaleHeight / 10000);
    }
    AutoPtr<IGravity> gravity;
    CGravity::AcquireSingleton((IGravity**)&gravity);
    gravity->Apply(mScaleState->mGravity, w, h, bounds, r);

    if (w > 0 && h > 0) {
        mScaleState->mDrawable->SetBounds(r->mLeft, r->mTop, r->mRight, r->mBottom);
    }
}

//@Override
Int32 ScaleDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mScaleState->mDrawable->GetIntrinsicWidth(&width);
    return width;
}

//@Override
Int32 ScaleDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mScaleState->mDrawable->GetIntrinsicHeight(&height);
    return height;
}

//@Override
AutoPtr<IDrawableConstantState> ScaleDrawable::GetConstantState()
{
    if (mScaleState->CanConstantState()) {
        mScaleState->mChangingConfigurations = Drawable::GetChangingConfigurations();
        return mScaleState;
    }
    return NULL;
}

//@Override
AutoPtr<IDrawable> ScaleDrawable::Mutate()
{
    if (!mMutated &&
            Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        AutoPtr<IDrawable> drawable;
        mScaleState->mDrawable->Mutate((IDrawable**)&drawable);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode ScaleDrawable::Init()
{
    return Init(NULL, NULL);
}

ECode ScaleDrawable::Init(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Float scaleWidth,
    /* [in] */ Float scaleHeight)
{
    FAIL_RETURN(Init(NULL, NULL));

    mScaleState->mDrawable = drawable;
    mScaleState->mGravity = gravity;
    mScaleState->mScaleWidth = scaleWidth;
    mScaleState->mScaleHeight = scaleHeight;

    if (drawable != NULL) {
        drawable->SetCallback(
                (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }

    return NOERROR;
}

ECode ScaleDrawable::Init(
    /* [in] */ ScaleState* state,
    /* [in] */ IResources* res)
{
    mScaleState = new ScaleState(state, this, res);
    return NOERROR;
}
