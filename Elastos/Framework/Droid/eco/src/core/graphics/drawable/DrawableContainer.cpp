
#include "graphics/drawable/DrawableContainer.h"

const Boolean DrawableContainer::DEFAULT_DITHER;

DrawableContainer::DrawableContainer() : mAlpha(0xFF), mCurIndex(-1)
{
}

ECode DrawableContainer::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mCurrDrawable.Get() != NULL) {
        mCurrDrawable->Draw(canvas);
    }

    return NOERROR;
}

Int32 DrawableContainer::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
            | mDrawableContainerState->mChangingConfigurations
            | mDrawableContainerState->mChildrenChangingConfigurations;
}

Boolean DrawableContainer::GetPadding(
    /* [in] */ IRect* padding)
{
    AutoPtr<IRect> r = mDrawableContainerState->GetConstantPadding();
    if (r.Get() != NULL) {
        padding->SetEx(r.Get());
        return TRUE;
    }
    if (mCurrDrawable.Get() != NULL) {
        Boolean result;
        mCurrDrawable->GetPadding(padding, &result);
        return result;
    } else {
        return Drawable::GetPadding(padding);
    }
}

ECode DrawableContainer::SetAlpha(
    /* [in] */ Int32 alpha)
{
    if (mAlpha != alpha) {
        mAlpha = alpha;
        if (mCurrDrawable.Get() != NULL) {
            mCurrDrawable->SetAlpha(alpha);
        }
    }

    return NOERROR;
}

ECode DrawableContainer::SetDither(
    /* [in] */ Boolean dither)
{
    if (mDrawableContainerState->mDither != dither) {
        mDrawableContainerState->mDither = dither;
        if (mCurrDrawable.Get() != NULL) {
            mCurrDrawable->SetDither(mDrawableContainerState->mDither);
        }
    }

    return NOERROR;
}

ECode DrawableContainer::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    if (mColorFilter.Get() != cf) {
        mColorFilter = cf;
        if (mCurrDrawable.Get() != NULL) {
            mCurrDrawable->SetColorFilter(cf);
        }
    }

    return NOERROR;
}

void DrawableContainer::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    if (mCurrDrawable.Get() != NULL) {
        mCurrDrawable->SetBoundsEx(bounds);
    }
}


Boolean DrawableContainer::IsStateful()
{
    return mDrawableContainerState->IsStateful();
}


Boolean DrawableContainer::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    if (mCurrDrawable.Get() != NULL) {
        Boolean isStateful;
        mCurrDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &isStateful);
        return isStateful;
    }
    return FALSE;
}


Boolean DrawableContainer::OnLevelChange(
    /* [in] */ Int32 level)
{
    if (mCurrDrawable.Get() != NULL) {
        Boolean changed;
        mCurrDrawable->SetLevel(level, &changed);
        return changed;
    }
    return FALSE;
}


Int32 DrawableContainer::GetIntrinsicWidth()
{
    if (mDrawableContainerState->IsConstantSize()) {
        return mDrawableContainerState->GetConstantWidth();
    }

    Int32 width = -1;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetIntrinsicWidth(&width);
    }

    return width;
}


Int32 DrawableContainer::GetIntrinsicHeight()
{
    if (mDrawableContainerState->IsConstantSize()) {
        return mDrawableContainerState->GetConstantHeight();
    }

    Int32 height = -1;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetIntrinsicHeight(&height);
    }

    return height;
}


Int32 DrawableContainer::GetMinimumWidth()
{
    if (mDrawableContainerState->IsConstantSize()) {
        return mDrawableContainerState->GetConstantMinimumWidth();
    }

    Int32 width = 0;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetMinimumWidth(&width);
    }

    return width;
}


Int32 DrawableContainer::GetMinimumHeight()
{
    if (mDrawableContainerState->IsConstantSize()) {
        return mDrawableContainerState->GetConstantMinimumHeight();
    }

    Int32 height = 0;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetMinimumHeight(&height);
    }

    return height;
}

ECode DrawableContainer::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    if (who == mCurrDrawable && mCallback != NULL) {
        mCallback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }

    return NOERROR;
}

ECode DrawableContainer::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    if (who == mCurrDrawable && mCallback != NULL) {
        mCallback->ScheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable), what, when);
    }

    return NOERROR;
}

ECode DrawableContainer::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    if (who == mCurrDrawable && mCallback != NULL) {
        mCallback->UnscheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable), what);
    }

    return NOERROR;
}


Boolean DrawableContainer::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean changed = Drawable::SetVisible(visible, restart);
    if (mCurrDrawable.Get() != NULL) {
        Boolean isDifferent;
        mCurrDrawable->SetVisible(visible, restart, &isDifferent);
    }
    return changed;
}


Int32 DrawableContainer::GetOpacity()
{
    Boolean visible;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->IsVisible(&visible);
    }

    return mCurrDrawable == NULL || !visible ? ElPixelFormat::TRANSPARENT :
            mDrawableContainerState->GetOpacity();
}

Boolean DrawableContainer::SelectDrawable(
    /* [in] */ Int32 idx)
{
    if (idx == mCurIndex) {
        return FALSE;
    }

    Boolean isDifferent;
    if (idx >= 0 && idx < mDrawableContainerState->mNumChildren) {
        IDrawable* d = (*mDrawableContainerState->mDrawables)[idx];
        if (mCurrDrawable != NULL) {
            mCurrDrawable->SetVisible(FALSE, FALSE, &isDifferent);
        }
        mCurrDrawable = d;
        mCurIndex = idx;
        if (d != NULL) {
            d->SetVisible(IsVisible(), TRUE, &isDifferent);
            d->SetAlpha(mAlpha);
            d->SetDither(mDrawableContainerState->mDither);
            d->SetColorFilter(mColorFilter);
            d->SetState(GetState(), &isDifferent);
            d->SetLevel(GetLevel(), &isDifferent);
            d->SetBoundsEx(GetBounds());
        }
    }
    else {
        if (mCurrDrawable != NULL) {
            mCurrDrawable->SetVisible(FALSE, FALSE, &isDifferent);
        }
        mCurrDrawable = NULL;
        mCurIndex = -1;
    }
    InvalidateSelf();
    return TRUE;
}

AutoPtr<IDrawable> DrawableContainer::GetCurrent()
{
    return mCurrDrawable;
}


AutoPtr<IDrawableConstantState> DrawableContainer::GetConstantState()
{
    if (mDrawableContainerState->CanConstantState()) {
        mDrawableContainerState->mChangingConfigurations = Drawable::GetChangingConfigurations();
        AutoPtr<IDrawableConstantState> ret =
                (IDrawableConstantState*)mDrawableContainerState->Probe(EIID_IDrawableConstantState);
        return ret;
    }
    return NULL;
}

AutoPtr<IDrawable> DrawableContainer::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        Int32 N = mDrawableContainerState->GetChildCount();
        ArrayOf<IDrawable*>* drawables = mDrawableContainerState->GetChildren();
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IDrawable> drawable;
            if ((*drawables)[i] != NULL) (*drawables)[i]->Mutate((IDrawable**)&drawable);
        }
        mMutated = TRUE;
    }
    return AutoPtr<IDrawable>((IDrawable*)this->Probe(EIID_IDrawable));
}

void DrawableContainer::SetConstantState(
    /* [in] */ DrawableContainerState* state)
{
    mDrawableContainerState = state;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
DrawableContainer::DrawableContainerState::DrawableContainerState(
    /* [in] */ DrawableContainerState* orig,
    /* [in] */ DrawableContainer* owner,
    /* [in] */ IResources* res)
    : mDrawables(NULL)
    , mVariablePadding(FALSE)
    , mConstantSize(FALSE)
    , mComputedConstantSize(FALSE)
    , mHaveOpacity(FALSE)
    , mHaveStateful(FALSE)
    , mPaddingChecked(FALSE)
    , mDither(DrawableContainer::DEFAULT_DITHER)
{
    mOwner = owner;

    if (orig != NULL) {
        mChangingConfigurations = orig->mChangingConfigurations;
        mChildrenChangingConfigurations = orig->mChildrenChangingConfigurations;

        ArrayOf<IDrawable*>* origDr = orig->mDrawables;

        mDrawables = ArrayOf<IDrawable*>::Alloc(origDr->GetLength());
        mNumChildren = orig->mNumChildren;

        Int32 N = mNumChildren;
        for (Int32 i = 0; i < N; i++) {
            if (res != NULL) {
                AutoPtr<IDrawableConstantState> state;
                (*origDr)[i]->GetConstantState((IDrawableConstantState**)&state);
                state->NewDrawableEx(res, &(*mDrawables)[i]);
            }
            else {
                AutoPtr<IDrawableConstantState> state;
                (*origDr)[i]->GetConstantState((IDrawableConstantState**)&state);
                state->NewDrawable(&(*mDrawables)[i]);
            }
            (*mDrawables)[i]->SetCallback(
                    owner ? (IDrawableCallback*)owner->Probe(EIID_IDrawableCallback) : NULL);
        }

        mCheckedConstantState = mCanConstantState = TRUE;
        mVariablePadding = orig->mVariablePadding;
        if (orig->mConstantPadding.Get() != NULL) {
            CRect::New(orig->mConstantPadding.Get(), (IRect**)&mConstantPadding);
        }
        mConstantSize = orig->mConstantSize;
        mComputedConstantSize = orig->mComputedConstantSize;
        mConstantWidth = orig->mConstantWidth;
        mConstantHeight = orig->mConstantHeight;

        mHaveOpacity = orig->mHaveOpacity;
        mOpacity = orig->mOpacity;
        mHaveStateful = orig->mHaveStateful;
        mStateful = orig->mStateful;

        mDither = orig->mDither;
    }
    else {
        mDrawables = ArrayOf<IDrawable*>::Alloc(10);
        mNumChildren = 0;
        mCheckedConstantState = mCanConstantState = FALSE;
    }
}

DrawableContainer::DrawableContainerState::~DrawableContainerState()
{
    for (Int32 i = 0; i < mDrawables->GetLength(); i++) {
        if ((*mDrawables)[i]) {
            (*mDrawables)[i]->Release();
        }
    }

    ArrayOf<IDrawable*>::Free(mDrawables);
}

PInterface DrawableContainer::DrawableContainerState::Probe(
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

UInt32 DrawableContainer::DrawableContainerState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DrawableContainer::DrawableContainerState::Release()
{
    return ElRefBase::Release();
}

ECode DrawableContainer::DrawableContainerState::GetInterfaceID(
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

ECode DrawableContainer::DrawableContainerState::GetChangingConfigurations(
    /* [out] */ Int32* configurations)
{
    VALIDATE_NOT_NULL(configurations);
    *configurations = mChangingConfigurations;

    return NOERROR;
}

Int32 DrawableContainer::DrawableContainerState::AddChild(
    /* [in] */ IDrawable* dr)
{
    Int32 pos = mNumChildren;

    if (pos >= mDrawables->GetLength()) {
        GrowArray(pos, pos + 10);
    }

    Boolean isDifferent;
    dr->SetVisible(FALSE, TRUE, &isDifferent);
    IDrawableCallback* cb = NULL;
    if (mOwner != NULL) cb = (IDrawableCallback*)mOwner->Probe(EIID_IDrawableCallback);
    dr->SetCallback(cb);

    if ((*mDrawables)[pos] != NULL) {
        (*mDrawables)[pos]->Release();
    }
    (*mDrawables)[pos] = dr;
    if (dr != NULL) {
        dr->AddRef();
    }
    mNumChildren++;
    Int32 config;
    dr->GetChangingConfigurations(&config);
    mChildrenChangingConfigurations |= config;
    mHaveOpacity = FALSE;
    mHaveStateful = FALSE;

    mConstantPadding = NULL;
    mPaddingChecked = FALSE;
    mComputedConstantSize = FALSE;

    return pos;
}

Int32 DrawableContainer::DrawableContainerState::GetChildCount()
{
    return mNumChildren;
}

ArrayOf<IDrawable*>* DrawableContainer::DrawableContainerState::GetChildren()
{
    return mDrawables;
}

void DrawableContainer::DrawableContainerState::SetVariablePadding(
    /* [in] */ Boolean variable)
{
    mVariablePadding = variable;
}

AutoPtr<IRect> DrawableContainer::DrawableContainerState::GetConstantPadding()
{
    if (mVariablePadding) {
        return NULL;
    }
    if (mConstantPadding != NULL || mPaddingChecked) {
        return mConstantPadding;
    }

    AutoPtr<IRect> r;
    AutoPtr<IRect> t;
    ASSERT_SUCCEEDED(CRect::New((IRect**)&t));

    Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        Boolean isPadding;
        (*mDrawables)[i]->GetPadding(t, &isPadding);
        if (isPadding) {
            if (r == NULL) ASSERT_SUCCEEDED(CRect::New(0, 0, 0, 0, (IRect**)&r));
            CRect* t_ = (CRect*)t.Get();
            CRect* r_ = (CRect*)r.Get();
            if (t_->mLeft > r_->mLeft) r_->mLeft = t_->mLeft;
            if (t_->mTop > r_->mTop) r_->mTop = t_->mTop;
            if (t_->mRight > r_->mRight) r_->mRight = t_->mRight;
            if (t_->mBottom > r_->mBottom) r_->mBottom = t_->mBottom;
        }
    }
    mPaddingChecked = TRUE;
    mConstantPadding = r;
    return mConstantPadding;
}

void DrawableContainer::DrawableContainerState::SetConstantSize(
    /* [in] */ Boolean constant)
{
    mConstantSize = constant;
}

Boolean DrawableContainer::DrawableContainerState::IsConstantSize()
{
    return mConstantSize;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantWidth()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantWidth;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantHeight()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantHeight;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantMinimumWidth()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantMinimumWidth;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantMinimumHeight()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantMinimumHeight;
}

void DrawableContainer::DrawableContainerState::ComputeConstantSize()
{
    mComputedConstantSize = TRUE;

    Int32 N = GetChildCount();
    mConstantWidth = mConstantHeight = 0;
    mConstantMinimumWidth = mConstantMinimumHeight = 0;
    for (Int32 i = 0; i < N; i++) {
        IDrawable* dr = (*mDrawables)[i];
        Int32 s;
        dr->GetIntrinsicWidth(&s);
        if (s > mConstantWidth) mConstantWidth = s;
        dr->GetIntrinsicHeight(&s);
        if (s > mConstantHeight) mConstantHeight = s;
        dr->GetMinimumWidth(&s);
        if (s > mConstantMinimumWidth) mConstantMinimumWidth = s;
        dr->GetMinimumHeight(&s);
        if (s > mConstantMinimumHeight) mConstantMinimumHeight = s;
    }
}

Int32 DrawableContainer::DrawableContainerState::GetOpacity()
{
    if (mHaveOpacity) {
        return mOpacity;
    }

    Int32 N = GetChildCount();
    Int32 op;
    if (N > 0) {
        Int32 opacity;
        (*mDrawables)[0]->GetOpacity(&opacity);
        op = opacity;
    }
    else op = ElPixelFormat::TRANSPARENT;
    for (Int32 i = 1; i < N; i++) {
        Int32 opacity;
        (*mDrawables)[i]->GetOpacity(&opacity);
        op = Drawable::ResolveOpacity(op, opacity);
    }
    mOpacity = op;
    mHaveOpacity = TRUE;
    return op;
}

Boolean DrawableContainer::DrawableContainerState::IsStateful()
{
    if (mHaveStateful) {
        return mStateful;
    }

    Boolean stateful = FALSE;
    Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        Boolean state;
        (*mDrawables)[i]->IsStateful(&state);
        if (state) {
            stateful = TRUE;
            break;
        }
    }

    mStateful = stateful;
    mHaveStateful = TRUE;
    return stateful;
}

void DrawableContainer::DrawableContainerState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    ArrayOf<IDrawable*>* newDrawables = ArrayOf<IDrawable*>::Alloc(newSize);
    for (Int32 i = 0; i < oldSize; i++) {
        (*newDrawables)[i] = (*mDrawables)[i];
    }

    ArrayOf<IDrawable*>::Free(mDrawables);
    mDrawables = newDrawables;
}

Boolean DrawableContainer::DrawableContainerState::CanConstantState()
{
    Mutex::Autolock lock(&mLock);

    if (!mCheckedConstantState) {
        mCanConstantState = TRUE;
        Int32 N = mNumChildren;
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IDrawableConstantState> state;
            (*mDrawables)[i]->GetConstantState((IDrawableConstantState**)&state);
            if (state == NULL) {
                mCanConstantState = FALSE;
                break;
            }
        }
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}

