
#include "graphics/drawable/AnimatedRotateDrawable.h"
#include "graphics/drawable/CAnimatedRotateDrawable.h"
#include "graphics/CRect.h"
#include "os/SystemClock.h"
#include "utils/CTypedValue.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;

AnimatedRotateDrawable::AnimatedRotateState::AnimatedRotateState(
    /* [in] */ AnimatedRotateState* source,
    /* [in] */ AnimatedRotateDrawable* owner,
    /* [in] */ IResources* res)
    : mCanConstantState(FALSE)
    , mCheckedConstantState(FALSE)
{
    if (source != NULL) {
        AutoPtr<IDrawableConstantState> state;
        source->mDrawable->GetConstantState((IDrawableConstantState**)&state);
        if (res != NULL) {
            state->NewDrawableEx(res, (IDrawable**)&mDrawable);
        }
        else {
            state->NewDrawable((IDrawable**)&mDrawable);
        }
        mDrawable->SetCallback(
            owner != NULL ? (IDrawableCallback*)owner->Probe(EIID_IDrawableCallback) : NULL);
        mPivotXRel = source->mPivotXRel;
        mPivotX = source->mPivotX;
        mPivotYRel = source->mPivotYRel;
        mPivotY = source->mPivotY;
        mFramesCount = source->mFramesCount;
        mFrameDuration = source->mFrameDuration;
        mCanConstantState = mCheckedConstantState = TRUE;
    }
}

PInterface AnimatedRotateDrawable::AnimatedRotateState::Probe(
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

UInt32 AnimatedRotateDrawable::AnimatedRotateState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AnimatedRotateDrawable::AnimatedRotateState::Release()
{
    return ElRefBase::Release();
}

ECode AnimatedRotateDrawable::AnimatedRotateState::GetInterfaceID(
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

ECode AnimatedRotateDrawable::AnimatedRotateState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CAnimatedRotateDrawable::New(
        this, NULL, (IAnimatedRotateDrawable**)drawable);
}

ECode AnimatedRotateDrawable::AnimatedRotateState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CAnimatedRotateDrawable::New(
        this, res, (IAnimatedRotateDrawable**)drawable);
}

ECode AnimatedRotateDrawable::AnimatedRotateState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    return mChangingConfigurations;
}

Boolean AnimatedRotateDrawable::AnimatedRotateState::CanConstantState()
{
    if (!mCheckedConstantState) {
        AutoPtr<IDrawableConstantState> state;
        mDrawable->GetConstantState((IDrawableConstantState**)&state);
        mCanConstantState = state != NULL;
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}

AnimatedRotateDrawable::AnimatedRotateDrawable(
    /* [in] */ AnimatedRotateState* rotateState,
    /* [in] */ IResources* res)
{
    Init(rotateState, res);
}

void AnimatedRotateDrawable::InitInternal()
{
    AnimatedRotateState* state = mState;
    mIncrement = 360.0f / (Float)state->mFramesCount;
    AutoPtr<IDrawable> drawable = state->mDrawable;
    if (drawable != NULL) {
        drawable->SetFilterBitmap(TRUE);
        if (IBitmapDrawable::Probe(drawable)) {
            IBitmapDrawable::Probe(drawable)->SetAntiAlias(TRUE);
        }
    }
}

//@Override
ECode AnimatedRotateDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 saveCount;
    canvas->Save(&saveCount);

    AnimatedRotateState* st = mState;
    IDrawable* drawable = st->mDrawable;
    AutoPtr<IRect> temp;
    drawable->GetBounds((IRect**)&temp);
    CRect* bounds = (CRect*)temp.Get();

    Int32 w = bounds->GetWidth();
    Int32 h = bounds->GetHeight();

    Float px = st->mPivotXRel ? (w * st->mPivotX) : st->mPivotX;
    Float py = st->mPivotYRel ? (h * st->mPivotY) : st->mPivotY;

    canvas->RotateEx(mCurrentDegrees, px, py);

    drawable->Draw(canvas);

    canvas->RestoreToCount(saveCount);

    return NOERROR;
}

ECode AnimatedRotateDrawable::Start()
{
    if (!mRunning) {
        mRunning = TRUE;
        NextFrame();
    }

    return NOERROR;
}

ECode AnimatedRotateDrawable::Stop()
{
    mRunning = FALSE;
    UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));

    return NOERROR;
}

Boolean AnimatedRotateDrawable::IsRunning()
{
    return mRunning;
}

void AnimatedRotateDrawable::NextFrame()
{
    UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    ScheduleSelf(
        (IRunnable*)this->Probe(EIID_IRunnable),
        SystemClock::GetUptimeMillis() + mState->mFrameDuration);
}

ECode AnimatedRotateDrawable::Run()
{
    // TODO: This should be computed in draw(Canvas), based on the amount
    // of time since the last frame drawn
    mCurrentDegrees += mIncrement;
    if (mCurrentDegrees > (360.0f - mIncrement)) {
        mCurrentDegrees = 0.0f;
    }
    InvalidateSelf();
    NextFrame();

    return NOERROR;
}

//@Override
Boolean AnimatedRotateDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean res;
    mState->mDrawable->SetVisible(visible, restart, &res);
    Boolean changed = Drawable::SetVisible(visible, restart);
    if (visible) {
        if (changed || restart) {
            mCurrentDegrees = 0.0f;
            NextFrame();
        }
    }
    else {
        UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    }
    return changed;
}

/**
 * Returns the drawable rotated by this RotateDrawable.
 */
AutoPtr<IDrawable> AnimatedRotateDrawable::GetDrawable()
{
    return mState->mDrawable;
}

//@Override
Int32 AnimatedRotateDrawable::GetChangingConfigurations()
{
    Int32 drawableConfig;
    return Drawable::GetChangingConfigurations()
        | mState->mChangingConfigurations
        | (mState->mDrawable->GetChangingConfigurations(&drawableConfig), drawableConfig);
}

ECode AnimatedRotateDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return mState->mDrawable->SetAlpha(alpha);
}

ECode AnimatedRotateDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return mState->mDrawable->SetColorFilter(cf);
}

Int32 AnimatedRotateDrawable::GetOpacity()
{
    Int32 opacity;
    mState->mDrawable->GetOpacity(&opacity);

    return opacity;
}

ECode AnimatedRotateDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    if (mCallback != NULL) {
        mCallback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }
    return NOERROR;
}

ECode AnimatedRotateDrawable::ScheduleDrawable(
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

ECode AnimatedRotateDrawable::UnscheduleDrawable(
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
Boolean AnimatedRotateDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    Boolean res;
    mState->mDrawable->GetPadding(padding, &res);

    return res;
}

//@Override
Boolean AnimatedRotateDrawable::IsStateful()
{
    Boolean res;
    mState->mDrawable->IsStateful(&res);

    return res;
}

//@Override
void AnimatedRotateDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* temp = (CRect*)bounds;
    mState->mDrawable->SetBounds(
        temp->mLeft, temp->mTop, temp->mRight, temp->mBottom);
}

//@Override
Int32 AnimatedRotateDrawable::GetIntrinsicWidth()
{
    Int32 width;
    mState->mDrawable->GetIntrinsicWidth(&width);

    return width;
}

//@Override
Int32 AnimatedRotateDrawable::GetIntrinsicHeight()
{
    Int32 height;
    mState->mDrawable->GetIntrinsicHeight(&height);

    return height;
}

//@Override
AutoPtr<IDrawableConstantState> AnimatedRotateDrawable::GetConstantState()
{
    if (mState->CanConstantState()) {
        mState->mChangingConfigurations = Drawable::GetChangingConfigurations();
        return mState;
    }
    return NULL;
}

static Int32 R_Styleable_AnimatedRotateDrawable[] = {
    0x01010194, 0x01010199, 0x010101b5, 0x010101b6,
    0x010102d7, 0x010102d8
};

//@Override
ECode AnimatedRotateDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs,
        ArrayOf<Int32>(R_Styleable_AnimatedRotateDrawable,
        sizeof(R_Styleable_AnimatedRotateDrawable) / sizeof(Int32))/*R.styleable.AnimatedRotateDrawable*/,
        (ITypedArray**)&a));

    FAIL_RETURN(Drawable::InflateWithAttributes(
        r, parser, a, 0/*R.styleable.AnimatedRotateDrawable_visible*/));

    AutoPtr<ITypedValue> tv;
    a->PeekValue(2/*R.styleable.AnimatedRotateDrawable_pivotX*/, (ITypedValue**)&tv);
    Boolean pivotXRel = ((CTypedValue*)tv.Get())->mType == TypedValue_TYPE_FRACTION;
    Float pivotX;
    pivotXRel ? tv->GetFraction(1.0f, 1.0f, &pivotX) : tv->GetFloat(&pivotX);

    a->PeekValue(3/*R.styleable.AnimatedRotateDrawable_pivotY*/, (ITypedValue**)&tv);
    Boolean pivotYRel = ((CTypedValue*)tv.Get())->mType == TypedValue_TYPE_FRACTION;
    Float pivotY;
    pivotYRel ? tv->GetFraction(1.0f, 1.0f, &pivotY) : tv->GetFloat(&pivotY);

    Int32 framesCount;
    a->GetInt32(5/*R.styleable.AnimatedRotateDrawable_framesCount*/, 12, &framesCount);
    Int32 frameDuration;
    a->GetInt32(4/*R.styleable.AnimatedRotateDrawable_frameDuration*/, 150, &frameDuration);

    Int32 res;
    a->GetResourceId(1/*R.styleable.AnimatedRotateDrawable_drawable*/, 0, &res);
    AutoPtr<IDrawable> drawable;
    if (res > 0) {
        r->GetDrawable(res, (IDrawable**)&drawable);
    }

    a->Recycle();

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type, depth;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
        && (type != IXmlPullParser_END_TAG
        || (parser->GetDepth(&depth), depth) > outerDepth)) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&drawable);
        if (drawable == NULL) {
            String name;
            parser->GetName(&name);
            Logger::W("Drawable", String("Bad element under <animated-rotate>: ")
                    + name);
        }
    }

    if (drawable == NULL) {
        Logger::W("drawable", "No drawable specified for <animated-rotate>");
    }

    mState->mDrawable = drawable;
    mState->mPivotXRel = pivotXRel;
    mState->mPivotX = pivotX;
    mState->mPivotYRel = pivotYRel;
    mState->mPivotY = pivotY;
    mState->mFramesCount = framesCount;
    mState->mFrameDuration = frameDuration;

    InitInternal();

    if (drawable != NULL) {
        drawable->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }

    return NOERROR;
}

//@Override
AutoPtr<IDrawable> AnimatedRotateDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get()
        == (IDrawable*)this->Probe(EIID_IDrawable)) {
        AutoPtr<IDrawable> temp;
        mState->mDrawable->Mutate((IDrawable**)&temp);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode AnimatedRotateDrawable::Init(
    /* [in] */ AnimatedRotateState* rotateState,
    /* [in] */ IResources* res)
{
    mState = new AnimatedRotateState(rotateState, this, res);
    InitInternal();

    return NOERROR;
}
