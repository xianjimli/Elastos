
#include "graphics/drawable/RotateDrawable.h"
#include "graphics/drawable/CRotateDrawable.h"
#include "graphics/CRect.h"
#include "utils/CTypedValue.h"
#include <Logger.h>
#include <elastos/Math.h>


using namespace Elastos::Utility::Logging;
using namespace Elastos::Core;

RotateDrawable::RotateState::RotateState(
    /* [in] */ RotateState* source,
    /* [in] */ RotateDrawable* owner,
    /* [in] */ IResources* res)
{
    if (source != NULL) {

        if (res != NULL) {
            AutoPtr<IDrawableConstantState> state;
            source->mDrawable->GetConstantState((IDrawableConstantState**)&state);
            state->NewDrawableEx(res, (IDrawable**)&mDrawable);
        }
        else {
            AutoPtr<IDrawableConstantState> state;
            source->mDrawable->GetConstantState((IDrawableConstantState**)&state);
            state->NewDrawable((IDrawable**)&mDrawable);
        }
        mDrawable->SetCallback((IDrawableCallback*)owner->Probe(EIID_IDrawableCallback));
        mPivotXRel = source->mPivotXRel;
        mPivotX = source->mPivotX;
        mPivotYRel = source->mPivotYRel;
        mPivotY = source->mPivotY;
        mFromDegrees = mCurrentDegrees = source->mFromDegrees;
        mToDegrees = source->mToDegrees;
        mCanConstantState = mCheckedConstantState = TRUE;
    }
}

PInterface RotateDrawable::RotateState::Probe(
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

UInt32 RotateDrawable::RotateState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 RotateDrawable::RotateState::Release()
{
    return ElRefBase::Release();
}

ECode RotateDrawable::RotateState::GetInterfaceID(
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

ECode RotateDrawable::RotateState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CRotateDrawable::New((Handle32)this, NULL, (IRotateDrawable**)drawable);
}

ECode RotateDrawable::RotateState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CRotateDrawable::New((Handle32)this, res, (IRotateDrawable**)drawable);
}

ECode RotateDrawable::RotateState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

Boolean RotateDrawable::RotateState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}


const Float RotateDrawable::MAX_LEVEL = 10000.0f;

/**
 * <p>Create a new rotating drawable with an empty state.</p>
 */
RotateDrawable::RotateDrawable()
    : mMutated(FALSE)
{
//    this(null, null);
}

/**
 * <p>Create a new rotating drawable with the specified state. A copy of
 * this state is used as the internal state for the newly created
 * drawable.</p>
 *
 * @param rotateState the state for this drawable
 */
RotateDrawable::RotateDrawable(
    /* [in] */ RotateState* rotateState,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
{
    mState = new RotateState(rotateState, this, res);
}

ECode RotateDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 saveCount;
    canvas->Save(&saveCount);

    AutoPtr<IRect> bounds;
    mState->mDrawable->GetBounds((IRect**)&bounds);

    CRect* _bounds = (CRect*)bounds.Get();
    Int32 w = _bounds->mRight - _bounds->mLeft;
    Int32 h = _bounds->mBottom - _bounds->mTop;

    const RotateState* st = mState;

    Float px = st->mPivotXRel ? (w * st->mPivotX) : st->mPivotX;
    Float py = st->mPivotYRel ? (h * st->mPivotY) : st->mPivotY;

    canvas->RotateEx(st->mCurrentDegrees, px, py);

    st->mDrawable->Draw(canvas);

    canvas->RestoreToCount(saveCount);

    return NOERROR;
}

/**
 * Returns the drawable rotated by this RotateDrawable.
 */
AutoPtr<IDrawable> RotateDrawable::GetDrawable()
{
    return mState->mDrawable;
}

//@Override
Int32 RotateDrawable::GetChangingConfigurations()
{
    Int32 conf;
    mState->mDrawable->GetChangingConfigurations(&conf);
    return Drawable::GetChangingConfigurations()
            | mState->mChangingConfigurations
            | conf;
}

ECode RotateDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mState->mDrawable->SetAlpha(alpha);
}

ECode RotateDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mState->mDrawable->SetColorFilter(cf);
}

Int32 RotateDrawable::GetOpacity()
{
    Int32 opacity;
    mState->mDrawable->GetOpacity(&opacity);
    return opacity;
}

ECode RotateDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    if (mCallback != NULL) {
        mCallback->InvalidateDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable));
    }

    return NOERROR;
}

ECode RotateDrawable::ScheduleDrawable(
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

ECode RotateDrawable::UnscheduleDrawable(
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
Boolean RotateDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    Boolean isPadding;
    mState->mDrawable->GetPadding(padding, &isPadding);
    return isPadding;
}

//@Override
Boolean RotateDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean isDifferent;
    mState->mDrawable->SetVisible(visible, restart, &isDifferent);
    return Drawable::SetVisible(visible, restart);
}

//@Override
Boolean RotateDrawable::IsStateful()
{
    Boolean isStateful;
    mState->mDrawable->IsStateful(&isStateful);
    return isStateful;
}

//@Override
Boolean RotateDrawable::OnStateChange(
    /* [in] */ ArrayOf<Int32>* state)
{
    Boolean changed;
    mState->mDrawable->SetState(state, &changed);
    OnBoundsChange(GetBounds());
    return changed;
}

//@Override
Boolean RotateDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Boolean changed;
    mState->mDrawable->SetLevel(level, &changed);
    OnBoundsChange(GetBounds());

    mState->mCurrentDegrees = mState->mFromDegrees +
            (mState->mToDegrees - mState->mFromDegrees) *
                    ((Float)level / MAX_LEVEL);

    InvalidateSelf();
    return TRUE;
}

//@Override
void RotateDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* _bounds = (CRect*)bounds;
    mState->mDrawable->SetBounds(
            _bounds->mLeft, _bounds->mTop,
            _bounds->mRight, _bounds->mBottom);
}

//@Override
Int32 RotateDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mState->mDrawable->GetIntrinsicWidth(&width);
    return width;
}

//@Override
Int32 RotateDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mState->mDrawable->GetIntrinsicHeight(&height);
    return height;
}

//@Override
AutoPtr<IDrawableConstantState> RotateDrawable::GetConstantState()
{
    if (mState->CanConstantState()) {
        mState->mChangingConfigurations = Drawable::GetChangingConfigurations();
        return mState;
    }
    return NULL;
}

static Int32 R_Styleable_RotateDrawable[] = {
    0x01010194, 0x01010199, 0x010101b3, 0x010101b4,
    0x010101b5, 0x010101b6
};

//@Override
ECode RotateDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_RotateDrawable,
            sizeof(R_Styleable_RotateDrawable) / sizeof(Int32)),/*com.android.internal.R.styleable.RotateDrawable*/
            (ITypedArray**)&a);

    Drawable::InflateWithAttributes(r, parser, a,
            0/*com.android.internal.R.styleable.RotateDrawable_visible*/);

    AutoPtr<ITypedValue> tv;
    a->PeekValue(4/*com.android.internal.R.styleable.RotateDrawable_pivotX*/, (ITypedValue**)&tv);
    Boolean pivotXRel;
    Float pivotX;
    if (tv == NULL) {
        pivotXRel = TRUE;
        pivotX = 0.5f;
    }
    else {
        pivotXRel = ((CTypedValue*)tv.Get())->mType == TypedValue_TYPE_FRACTION;
        pivotXRel ? tv->GetFraction(1.0f, 1.0f, &pivotX) : tv->GetFloat(&pivotX);
    }

    tv = NULL;
    a->PeekValue(5/*com.android.internal.R.styleable.RotateDrawable_pivotY*/, (ITypedValue**)&tv);
    Boolean pivotYRel;
    Float pivotY;
    if (tv == NULL) {
        pivotYRel = TRUE;
        pivotY = 0.5f;
    }
    else {
        pivotYRel = ((CTypedValue*)tv.Get())->mType == TypedValue_TYPE_FRACTION;
        pivotYRel ? tv->GetFraction(1.0f, 1.0f, &pivotY) : tv->GetFloat(&pivotY);
    }

    Float fromDegrees, toDegrees;
    a->GetFloat(
            2/*com.android.internal.R.styleable.RotateDrawable_fromDegrees*/,
            0.0f, &fromDegrees);
    a->GetFloat(
            3/*com.android.internal.R.styleable.RotateDrawable_toDegrees*/,
            360.0f, &toDegrees);

    toDegrees = Math::Max(fromDegrees, toDegrees);

    Int32 res;
    a->GetResourceId(
            1/*com.android.internal.R.styleable.RotateDrawable_drawable*/,
            0, &res);
    AutoPtr<IDrawable> drawable;
    if (res > 0) {
        r->GetDrawable(res, (IDrawable**)&drawable);
    }

    a->Recycle();

    Int32 outerDepth, type, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT) &&
           (type != IXmlPullParser_END_TAG || (parser->GetDepth(&depth), depth > outerDepth))) {

        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        drawable = NULL;
        Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&drawable);
        if (drawable == NULL) {
            String name;
            parser->GetName(&name);
            Logger::W("drawable", String("Bad element under <rotate>: ")
                    + name);
        }
    }

    if (drawable == NULL) {
        Logger::W("drawable", "No drawable specified for <rotate>");
    }

    mState->mDrawable = drawable;
    mState->mPivotXRel = pivotXRel;
    mState->mPivotX = pivotX;
    mState->mPivotYRel = pivotYRel;
    mState->mPivotY = pivotY;
    mState->mFromDegrees = mState->mCurrentDegrees = fromDegrees;
    mState->mToDegrees = toDegrees;

    if (drawable != NULL) {
        drawable->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }

    return NOERROR;
}

//@Override
AutoPtr<IDrawable> RotateDrawable::Mutate()
{
    if (!mMutated &&
            Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        AutoPtr<IDrawable> drawable;
        mState->mDrawable->Mutate((IDrawable**)&drawable);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode RotateDrawable::Init()
{
    return Init(NULL, NULL);
}

ECode RotateDrawable::Init(
    /* [in] */ RotateState* rotateState,
    /* [in] */ IResources* res)
{
    mState = new RotateState(rotateState, this, res);
    return NOERROR;
}
