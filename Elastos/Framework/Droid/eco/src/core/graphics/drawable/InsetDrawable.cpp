
#include "graphics/drawable/InsetDrawable.h"
#include "graphics/drawable/CInsetDrawable.h"
#include <Logger.h>


using namespace Elastos::Utility::Logging;

InsetDrawable::InsetState::InsetState(
    /* [in] */ InsetState* orig,
    /* [in] */ InsetDrawable* owner,
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
        mInsetLeft = orig->mInsetLeft;
        mInsetTop = orig->mInsetTop;
        mInsetRight = orig->mInsetRight;
        mInsetBottom = orig->mInsetBottom;
        mCheckedConstantState = mCanConstantState = TRUE;
    }
}

PInterface InsetDrawable::InsetState::Probe(
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

UInt32 InsetDrawable::InsetState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InsetDrawable::InsetState::Release()
{
    return ElRefBase::Release();
}

ECode InsetDrawable::InsetState::GetInterfaceID(
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

ECode InsetDrawable::InsetState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CInsetDrawable::New((Handle32)this, NULL,
            (IInsetDrawable**)drawable);
}

ECode InsetDrawable::InsetState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CInsetDrawable::New((Handle32)this, res,
            (IInsetDrawable**)drawable);
}

ECode InsetDrawable::InsetState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

Boolean InsetDrawable::InsetState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}


/*package*/ InsetDrawable::InsetDrawable()
{
    CRect::NewByFriend((CRect**)&mTmpRect);
//    this(null, null);
}

InsetDrawable::InsetDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 inset)
{
    CRect::NewByFriend((CRect**)&mTmpRect);

    ASSERT_SUCCEEDED(Init(
            drawable, inset, inset, inset, inset));
}

InsetDrawable::InsetDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 insetLeft,
    /* [in] */ Int32 insetTop,
    /* [in] */ Int32 insetRight,
    /* [in] */ Int32 insetBottom)
{
    CRect::NewByFriend((CRect**)&mTmpRect);

    ASSERT_SUCCEEDED(Init(
            drawable, insetLeft, insetTop, insetRight, insetBottom));
}

InsetDrawable::InsetDrawable(
    /* [in] */ InsetState* state,
    /* [in] */ IResources* res)
{
    CRect::NewByFriend((CRect**)&mTmpRect);
    mInsetState = new InsetState(state, this, res);
}

static Int32 R_Styleable_InsetDrawable[] = {
    0x01010194, 0x01010199, 0x010101b7, 0x010101b8,
    0x010101b9, 0x010101ba
};

ECode InsetDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 type;

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_InsetDrawable,
            sizeof(R_Styleable_InsetDrawable) / sizeof(Int32)),/*com.android.internal.R.styleable.InsetDrawable*/
            (ITypedArray**)&a);

    Drawable::InflateWithAttributes(r, parser, a,
            0/*com.android.internal.R.styleable.InsetDrawable_visible*/);

    Int32 drawableRes;
    a->GetResourceId(
            1/*com.android.internal.R.styleable.InsetDrawable_drawable*/,
            0, &drawableRes);

    Int32 inLeft, inTop, inRight, inBottom;
    a->GetDimensionPixelOffset(
            2/*com.android.internal.R.styleable.InsetDrawable_insetLeft*/,
            0, &inLeft);
    a->GetDimensionPixelOffset(
            4/*com.android.internal.R.styleable.InsetDrawable_insetTop*/,
            0, &inTop);
    a->GetDimensionPixelOffset(
            3/*com.android.internal.R.styleable.InsetDrawable_insetRight*/,
            0, &inRight);
    a->GetDimensionPixelOffset(
            5/*com.android.internal.R.styleable.InsetDrawable_insetBottom*/,
            0, &inBottom);

    a->Recycle();

    AutoPtr<IDrawable> dr;
    if (drawableRes != 0) {
        r->GetDrawable(drawableRes, (IDrawable**)&dr);
    }
    else {
        while (parser->Next(&type), type == IXmlPullParser_TEXT) {
        }
        if (type != IXmlPullParser_START_TAG) {
//            throw new XmlPullParserException(
//                    parser.getPositionDescription()
//                    + ": <inset> tag requires a 'drawable' attribute or "
//                    + "child tag defining a drawable");
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
    }

    if (dr == NULL) {
        Logger::W("drawable", "No drawable specified for <inset>");
    }

    mInsetState->mDrawable = dr;
    mInsetState->mInsetLeft = inLeft;
    mInsetState->mInsetRight = inRight;
    mInsetState->mInsetTop = inTop;
    mInsetState->mInsetBottom = inBottom;

    if (dr != NULL) {
        dr->SetCallback(
                (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }

    return NOERROR;
}

ECode InsetDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    if (mCallback != NULL) {
        mCallback->InvalidateDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable));
    }

    return NOERROR;
}

ECode InsetDrawable::ScheduleDrawable(
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

ECode InsetDrawable::UnscheduleDrawable(
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
ECode InsetDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    return mInsetState->mDrawable->Draw(canvas);
}

//@Override
Int32 InsetDrawable::GetChangingConfigurations()
{
    Int32 conf;
    mInsetState->mDrawable->GetChangingConfigurations(&conf);
    return Drawable::GetChangingConfigurations()
            | mInsetState->mChangingConfigurations
            | conf;
}

//@Override
Boolean InsetDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    Boolean pad;
    mInsetState->mDrawable->GetPadding(padding, &pad);

    ((CRect*)padding)->mLeft += mInsetState->mInsetLeft;
    ((CRect*)padding)->mRight += mInsetState->mInsetRight;
    ((CRect*)padding)->mTop += mInsetState->mInsetTop;
    ((CRect*)padding)->mBottom += mInsetState->mInsetBottom;

    if (pad || (mInsetState->mInsetLeft | mInsetState->mInsetRight |
                mInsetState->mInsetTop | mInsetState->mInsetBottom) != 0) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

//@Override
Boolean InsetDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean isDifferent;
    mInsetState->mDrawable->SetVisible(visible, restart, &isDifferent);
    return Drawable::SetVisible(visible, restart);
}

ECode InsetDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mInsetState->mDrawable->SetAlpha(alpha);
}

ECode InsetDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mInsetState->mDrawable->SetColorFilter(cf);
}

Int32 InsetDrawable::GetOpacity()
{
    Int32 opacity;
    mInsetState->mDrawable->GetOpacity(&opacity);
    return opacity;
}

//@Override
Boolean InsetDrawable::IsStateful()
{
    Boolean isStateful;
    mInsetState->mDrawable->IsStateful(&isStateful);
    return isStateful;
}

//@Override
Boolean InsetDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    Boolean changed;
    mInsetState->mDrawable->SetState(
        const_cast<ArrayOf<Int32>*>(state), &changed);
    OnBoundsChange(GetBounds());
    return changed;
}

//@Override
void InsetDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* r = mTmpRect;
    r->SetEx(bounds);

    r->mLeft += mInsetState->mInsetLeft;
    r->mTop += mInsetState->mInsetTop;
    r->mRight -= mInsetState->mInsetRight;
    r->mBottom -= mInsetState->mInsetBottom;

    mInsetState->mDrawable->SetBounds(r->mLeft, r->mTop, r->mRight, r->mBottom);
}

//@Override
Int32 InsetDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mInsetState->mDrawable->GetIntrinsicWidth(&width);
    return width;
}

//@Override
Int32 InsetDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mInsetState->mDrawable->GetIntrinsicHeight(&height);
    return height;
}

//@Override
AutoPtr<IDrawableConstantState> InsetDrawable::GetConstantState()
{
    if (mInsetState->CanConstantState()) {
        mInsetState->mChangingConfigurations = Drawable::GetChangingConfigurations();
        return mInsetState;
    }
    return NULL;
}

//@Override
AutoPtr<IDrawable> InsetDrawable::Mutate()
{
    if (!mMutated &&
            Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        AutoPtr<IDrawable> drawable;
        mInsetState->mDrawable->Mutate((IDrawable**)&drawable);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode InsetDrawable::Init()
{
    return Init(NULL, NULL);
}

ECode InsetDrawable::Init(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 inset)
{
    return Init(drawable, inset, inset, inset, inset);
}

ECode InsetDrawable::Init(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 insetLeft,
    /* [in] */ Int32 insetTop,
    /* [in] */ Int32 insetRight,
    /* [in] */ Int32 insetBottom)
{
    FAIL_RETURN(Init(NULL, NULL));

    mInsetState->mDrawable = drawable;
    mInsetState->mInsetLeft = insetLeft;
    mInsetState->mInsetTop = insetTop;
    mInsetState->mInsetRight = insetRight;
    mInsetState->mInsetBottom = insetBottom;

    if (drawable != NULL) {
        drawable->SetCallback(
                (IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }

    return NOERROR;
}

ECode InsetDrawable::Init(
    /* [in] */ InsetState* state,
    /* [in] */ IResources* res)
{
    mInsetState = new InsetState(state, this, res);
    return NOERROR;
}
