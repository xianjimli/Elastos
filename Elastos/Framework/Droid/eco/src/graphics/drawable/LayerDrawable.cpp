
#include "graphics/drawable/LayerDrawable.h"
#include "graphics/drawable/CLayerDrawable.h"
#include "graphics/CRect.h"
#include "graphics/ElPixelFormat.h"
#include <stdio.h>

LayerDrawable::LayerState::LayerState(
    /* [in] */ LayerState* orig,
    /* [in] */ LayerDrawable* owner,
    /* [in] */ IResources* res)
    : mHaveOpacity(FALSE)
    , mHaveStateful(FALSE)
{
    if (orig != NULL) {
        Int32 N = orig->mNum;
        mNum = N;
        mChildren = ArrayOf<AutoPtr<ChildDrawable> >::Alloc(N);

        mChangingConfigurations = orig->mChangingConfigurations;
        mChildrenChangingConfigurations = orig->mChildrenChangingConfigurations;

        for (Int32 i = 0; i < N; i++) {
            AutoPtr<ChildDrawable> r = (*mChildren)[i] = new ChildDrawable();
            AutoPtr<ChildDrawable> r2 = (*orig->mChildren)[i];
            AutoPtr<IDrawableConstantState> state;
            r2->mDrawable->GetConstantState((IDrawableConstantState**)&state);
            if (res != NULL) {
                state->NewDrawableEx(res, (IDrawable**)&r->mDrawable);
            }
            else {
                state->NewDrawable((IDrawable**)&r->mDrawable);
            }
            r->mDrawable->SetCallback(
                owner != NULL ? (IDrawableCallback*)owner->Probe(EIID_IDrawableCallback) : NULL);
            r->mInsetL = r2->mInsetL;
            r->mInsetT = r2->mInsetT;
            r->mInsetR = r2->mInsetR;
            r->mInsetB = r2->mInsetB;
            r->mId = r2->mId;
        }

        mHaveOpacity = orig->mHaveOpacity;
        mOpacity = orig->mOpacity;
        mHaveStateful = orig->mHaveStateful;
        mStateful = orig->mStateful;
        mCheckedConstantState = mCanConstantState = TRUE;
    }
    else {
        mNum = 0;
        mChildren = NULL;
    }
}

PInterface LayerDrawable::LayerState::Probe(
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

UInt32 LayerDrawable::LayerState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LayerDrawable::LayerState::Release()
{
    return ElRefBase::Release();
}

ECode LayerDrawable::LayerState::GetInterfaceID(
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

ECode LayerDrawable::LayerState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CLayerDrawable::New(this, NULL, (ILayerDrawable**)drawable);
}

ECode LayerDrawable::LayerState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CLayerDrawable::New(this, res, (ILayerDrawable**)drawable);
}

ECode LayerDrawable::LayerState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    return mChangingConfigurations;
}

Int32 LayerDrawable::LayerState::GetOpacity()
{
    if (mHaveOpacity) {
        return mOpacity;
    }

    Int32 N = mNum;
    Int32 op = ElPixelFormat::TRANSPARENT;
    if (N > 0) {
        (*mChildren)[0]->mDrawable->GetOpacity(&op);
    }

    for (Int32 i = 1; i < N; i++) {
        Int32 op2;
        (*mChildren)[i]->mDrawable->GetOpacity(&op2);
        op = Drawable::ResolveOpacity(op, op2);
    }
    mOpacity = op;
    mHaveOpacity = TRUE;
    return op;
}

Boolean LayerDrawable::LayerState::IsStateful()
{
    if (mHaveStateful) {
        return mStateful;
    }

    Boolean stateful = FALSE;
    Int32 N = mNum;
    for (Int32 i = 0; i < N; i++) {
        Boolean isStateful;
        (*mChildren)[i]->mDrawable->IsStateful(&isStateful);
        if (isStateful) {
            stateful = TRUE;
            break;
        }
    }

    mStateful = stateful;
    mHaveStateful = TRUE;
    return stateful;
}

Boolean LayerDrawable::LayerState::CanConstantState()
{
    Mutex::Autolock lock(m_syncLock);

    if (!mCheckedConstantState && mChildren != NULL) {
        mCanConstantState = TRUE;
        Int32 N = mNum;
        for (Int32 i=0; i<N; i++) {
            AutoPtr<IDrawableConstantState> state;
            (*mChildren)[i]->mDrawable->GetConstantState(
                (IDrawableConstantState**)&state);
            if (state == NULL) {
                mCanConstantState = FALSE;
                break;
            }
        }
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}

LayerDrawable::LayerDrawable()
    : mPaddingL(NULL)
    , mPaddingT(NULL)
    , mPaddingR(NULL)
    , mPaddingB(NULL)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
}

/**
 * Create a new layer drawable with the specified list of layers and the specified
 * constant state.
 *
 * @param layers The list of layers to add to this drawable.
 * @param state The constant drawable state.
 */
LayerDrawable::LayerDrawable(
    /* [in] */ IObjectContainer* layers,
    /* [in] */ LayerState* state)
    : mPaddingL(NULL)
    , mPaddingT(NULL)
    , mPaddingR(NULL)
    , mPaddingB(NULL)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
    Init(layers, state);
}

LayerDrawable::LayerDrawable(
    /* [in] */ LayerState* state,
    /* [in] */ IResources* res)
    : mPaddingL(NULL)
    , mPaddingT(NULL)
    , mPaddingR(NULL)
    , mPaddingB(NULL)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
    Init(state, res);
}

AutoPtr<LayerDrawable::LayerState> LayerDrawable::CreateConstantState(
    /* [in] */ LayerState* state,
    /* [in] */ IResources* res)
{
    return new LayerState(state, this, res);
}

static Int32 R_Styleable_LayerDrawableItem[] = {
    0x010100d0, 0x01010199, 0x010101ad, 0x010101ae,
    0x010101af, 0x010101b0
};

//@Override
ECode LayerDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Drawable::Inflate(r, parser, attrs);

    Int32 type;

    Int32 innerDepth;
    parser->GetDepth(&innerDepth);
    innerDepth++;
    Int32 depth;
    while ((parser->Next(&type), type) != IXmlPullParser_END_DOCUMENT
        && ((parser->GetDepth(&depth), depth) >= innerDepth
        || type != IXmlPullParser_END_TAG)) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (depth > innerDepth || !name.Equals("item")) {
            continue;
        }

        AutoPtr<ITypedArray> a;
        r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_LayerDrawableItem,
            sizeof(R_Styleable_LayerDrawableItem) / sizeof(Int32)),/*com.android.internal.R.styleable.LayerDrawableItem*/
            (ITypedArray**)&a);

        Int32 left;
        a->GetDimensionPixelOffset(
            2/*com.android.internal.R.styleable.LayerDrawableItem_left*/, 0, &left);
        Int32 top;
        a->GetDimensionPixelOffset(
            3/*com.android.internal.R.styleable.LayerDrawableItem_top*/, 0, &top);
        Int32 right;
        a->GetDimensionPixelOffset(
            4/*com.android.internal.R.styleable.LayerDrawableItem_right*/, 0, &right);
        Int32 bottom;
        a->GetDimensionPixelOffset(
            5/*com.android.internal.R.styleable.LayerDrawableItem_bottom*/, 0, &bottom);
        Int32 drawableRes;
        a->GetResourceId(
            1/*com.android.internal.R.styleable.LayerDrawableItem_drawable*/, 0, &drawableRes);
        Int32 id;
        a->GetResourceId(
            0/*com.android.internal.R.styleable.LayerDrawableItem_id*/, View_NO_ID, &id);

        a->Recycle();

        AutoPtr<IDrawable> dr;
        if (drawableRes != 0) {
            r->GetDrawable(drawableRes, (IDrawable**)&dr);
        }
        else {
            while ((parser->Next(&type), type) == IXmlPullParser_TEXT) {
            }
            if (type != IXmlPullParser_START_TAG) {
                //throw new XmlPullParserException(parser->getPositionDescription()
                //        + ": <item> tag requires a 'drawable' attribute or "
                //        + "child tag defining a drawable");
            }
            Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
        }

        if (dr != NULL) {
            AddLayer(dr, id, left, top, right, bottom);
        }
    }

    EnsurePadding();
    OnStateChange(GetState());

    return NOERROR;
}


/**
 * Add a new layer to this drawable. The new layer is identified by an id.
 *
 * @param layer The drawable to add as a layer->
 * @param id The id of the new layer->
 * @param left The left padding of the new layer->
 * @param top The top padding of the new layer->
 * @param right The right padding of the new layer->
 * @param bottom The bottom padding of the new layer->
 */
void LayerDrawable::AddLayer(
    /* [in] */ IDrawable* layer,
    /* [in] */ Int32 id,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    LayerState* st = mLayerState;
    Int32 N = st->mChildren != NULL ? st->mChildren->GetLength() : 0;
    Int32 i = st->mNum;
    if (i >= N) {
        ArrayOf<AutoPtr<ChildDrawable> >* nu = ArrayOf<AutoPtr<ChildDrawable> >::Alloc(N + 10);
        if (i > 0) {
            for (Int32 j = 0; j < i; j++) {
                (*nu)[j] = (*st->mChildren)[i];
            }
        }
        ArrayOf<AutoPtr<ChildDrawable> >::Free(st->mChildren);
        st->mChildren = nu;
    }

    Int32 config;
    layer->GetChangingConfigurations(&config);
    mLayerState->mChildrenChangingConfigurations |= config;

    AutoPtr<ChildDrawable> childDrawable = new ChildDrawable();
    (*st->mChildren)[i] = childDrawable;
    childDrawable->mId = id;
    childDrawable->mDrawable = layer;
    childDrawable->mInsetL = left;
    childDrawable->mInsetT = top;
    childDrawable->mInsetR = right;
    childDrawable->mInsetB = bottom;
    st->mNum++;

    layer->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
}

/**
 * Look for a layer with the given id, and returns its {@link Drawable}.
 *
 * @param id The layer ID to search for->
 * @return The {@link Drawable} of the layer that has the given id in the hierarchy or NULL.
 */
AutoPtr<IDrawable> LayerDrawable::FindDrawableByLayerId(
    /* [in] */ Int32 id)
{
    ArrayOf<AutoPtr<ChildDrawable> >* layers = mLayerState->mChildren;

    for (Int32 i = mLayerState->mNum - 1; i >= 0; i--) {
        if ((*layers)[i]->mId == id) {
            return (*layers)[i]->mDrawable;
        }
    }

    return NULL;
}

/**
 * Sets the ID of a layer->
 *
 * @param index The index of the layer which will received the ID.
 * @param id The ID to assign to the layer->
 */
ECode LayerDrawable::SetId(
    /* [in] */ Int32 index,
    /* [in] */ Int32 id)
{
    (*mLayerState->mChildren)[index]->mId = id;

    return NOERROR;
}

/**
 * Returns the number of layers contained within this.
 * @return The number of layers.
 */
Int32 LayerDrawable::GetNumberOfLayers()
{
    return mLayerState->mNum;
}

/**
 * Returns the drawable at the specified layer index.
 *
 * @param index The layer index of the drawable to retrieve.
 *
 * @return The {@link android.graphics.drawable.Drawable} at the specified layer index.
 */
AutoPtr<IDrawable> LayerDrawable::GetDrawable(
    /* [in] */ Int32 index)
{
    return (*mLayerState->mChildren)[index]->mDrawable;
}

/**
 * Returns the id of the specified layer.
 *
 * @param index The index of the layer.
 *
 * @return The id of the layer or {@link android.view.View#NO_ID} if the layer has no id.
 */
Int32 LayerDrawable::GetId(
    /* [in] */ Int32 index)
{
    return (*mLayerState->mChildren)[index]->mId;
}

/**
 * Sets (or replaces) the {@link Drawable} for the layer with the given id.
 *
 * @param id The layer ID to search for->
 * @param drawable The replacement {@link Drawable}.
 * @return Whether the {@link Drawable} was replaced (could return FALSE if
 *         the id was not found).
 */
Boolean LayerDrawable::SetDrawableByLayerId(
    /* [in] */ Int32 id,
    /* [in] */ IDrawable* drawable)
{
    ArrayOf<AutoPtr<ChildDrawable> >* layers = mLayerState->mChildren;

    for (Int32 i = mLayerState->mNum - 1; i >= 0; i--) {
        if ((*layers)[i]->mId == id) {
            (*layers)[i]->mDrawable = drawable;
            return TRUE;
        }
    }

    return FALSE;
}

/** Specify modifiers to the bounds for the drawable[index]->
    left += l
    top += t;
    right -= r;
    bottom -= b;
*/
ECode LayerDrawable::SetLayerInset(
    /* [in] */ Int32 index,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    ChildDrawable* childDrawable = (*mLayerState->mChildren)[index];
    childDrawable->mInsetL = l;
    childDrawable->mInsetT = t;
    childDrawable->mInsetR = r;
    childDrawable->mInsetB = b;

    return NOERROR;
}

// overrides from Drawable.Callback

ECode LayerDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    if (mCallback != NULL) {
        mCallback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }

    return NOERROR;
}

ECode LayerDrawable::ScheduleDrawable(
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

ECode LayerDrawable::UnscheduleDrawable(
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
ECode LayerDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i=0; i<N; i++) {
        (*array)[i]->mDrawable->Draw(canvas);
    }

    return NOERROR;
}

//@Override
Int32 LayerDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
            | mLayerState->mChangingConfigurations
            | mLayerState->mChildrenChangingConfigurations;
}

//@Override
Boolean LayerDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    CRect* rect = (CRect*)padding;
    // Arbitrarily get the padding from the first image.
    // Technically we should maybe do something more intelligent,
    // like take the max padding of all the images.
    rect->mLeft = 0;
    rect->mTop = 0;
    rect->mRight = 0;
    rect->mBottom = 0;

    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i=0; i<N; i++) {
        ReapplyPadding(i, (*array)[i]);
        rect->mLeft += (*mPaddingL)[i];
        rect->mTop += (*mPaddingT)[i];
        rect->mRight += (*mPaddingT)[i];
        rect->mBottom += (*mPaddingB)[i];
    }
    return TRUE;
}

//@Override
Boolean LayerDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean changed = Drawable::SetVisible(visible, restart);
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i=0; i<N; i++) {
        Boolean res;
        (*array)[i]->mDrawable->SetVisible(visible, restart, &res);
    }
    return changed;
}

//@Override
ECode LayerDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i=0; i<N; i++) {
        (*array)[i]->mDrawable->SetDither(dither);
    }

    return NOERROR;
}

//@Override
ECode LayerDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i=0; i<N; i++) {
        (*array)[i]->mDrawable->SetAlpha(alpha);
    }

    return NOERROR;
}

//@Override
ECode LayerDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i=0; i<N; i++) {
        (*array)[i]->mDrawable->SetColorFilter(cf);
    }

    return NOERROR;
}

//@Override
Int32 LayerDrawable::GetOpacity()
{
    return mLayerState->GetOpacity();
}

//@Override
Boolean LayerDrawable::IsStateful()
{
    return mLayerState->IsStateful();
}

//@Override
Boolean LayerDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Boolean paddingChanged = FALSE;
    Boolean changed = FALSE;
    for (Int32 i=0; i<N; i++) {
        ChildDrawable* r = (*array)[i];
        Boolean res;
        r->mDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &res);
        if (res) {
            changed = TRUE;
        }
        if (ReapplyPadding(i, r)) {
            paddingChanged = TRUE;
        }
    }
    if (paddingChanged) {
        OnBoundsChange(GetBounds());
    }
    return changed;
}

//@Override
Boolean LayerDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Boolean paddingChanged = FALSE;
    Boolean changed = FALSE;
    for (Int32 i=0; i<N; i++) {
        ChildDrawable* r = (*array)[i];
        Boolean res;
        r->mDrawable->SetLevel(level, &res);
        if (res) {
            changed = TRUE;
        }
        if (ReapplyPadding(i, r)) {
            paddingChanged = TRUE;
        }
    }
    if (paddingChanged) {
        OnBoundsChange(GetBounds());
    }
    return changed;
}

//@Override
void LayerDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* rect = (CRect*)bounds;
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Int32 padL=0, padT=0, padR=0, padB=0;
    for (Int32 i=0; i<N; i++) {
        ChildDrawable* r = (*array)[i];
        r->mDrawable->SetBounds(
            rect->mLeft + r->mInsetL + padL,
            rect->mTop + r->mInsetT + padT,
            rect->mRight - r->mInsetR - padR,
            rect->mBottom - r->mInsetB - padB);
        padL += (*mPaddingL)[i];
        padR += (*mPaddingT)[i];
        padT += (*mPaddingT)[i];
        padB += (*mPaddingB)[i];
    }
}

//@Override
Int32 LayerDrawable::GetIntrinsicWidth()
{
    Int32 width = -1;
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Int32 padL=0, padR=0;
    for (Int32 i=0; i<N; i++) {
        ChildDrawable* r = (*array)[i];
        Int32 w;
        r->mDrawable->GetIntrinsicWidth(&w);
        w += r->mInsetL + r->mInsetR + padL + padR;
        if (w > width) {
            width = w;
        }
        padL += (*mPaddingL)[i];
        padR += (*mPaddingT)[i];
    }
    return width;
}

//@Override
Int32 LayerDrawable::GetIntrinsicHeight()
{
    Int32 height = -1;
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Int32 padT=0, padB=0;
    for (Int32 i=0; i<N; i++) {
        ChildDrawable* r = (*array)[i];
        Int32 h;
        r->mDrawable->GetIntrinsicHeight(&h);
        h += r->mInsetT + r->mInsetB + + padT + padB;
        if (h > height) {
            height = h;
        }
        padT += (*mPaddingT)[i];
        padB += (*mPaddingB)[i];
    }
    return height;
}

Boolean LayerDrawable::ReapplyPadding(
    /* [in] */ Int32 i,
    /* [in] */ ChildDrawable* r)
{
    CRect* rect = (CRect*)mTmpRect.Get();
    Boolean res;
    r->mDrawable->GetPadding(rect, &res);
    if (rect->mLeft != (*mPaddingL)[i] || rect->mTop != (*mPaddingT)[i] ||
        rect->mRight != (*mPaddingT)[i] || rect->mBottom != (*mPaddingB)[i]) {
        (*mPaddingL)[i] = rect->mLeft;
        (*mPaddingT)[i] = rect->mTop;
        (*mPaddingT)[i] = rect->mRight;
        (*mPaddingB)[i] = rect->mBottom;
        return TRUE;
    }
    return FALSE;
}

void LayerDrawable::EnsurePadding()
{
    Int32 N = mLayerState->mNum;
    if (mPaddingL != NULL) {
        if (mPaddingL->GetLength() >= N) {
            return;
        }
        else {
            ArrayOf<Int32>::Free(mPaddingL);
            ArrayOf<Int32>::Free(mPaddingT);
            ArrayOf<Int32>::Free(mPaddingR);
            ArrayOf<Int32>::Free(mPaddingB);
        }
    }
    mPaddingL = ArrayOf<Int32>::Alloc(N);
    mPaddingT = ArrayOf<Int32>::Alloc(N);
    mPaddingR = ArrayOf<Int32>::Alloc(N);
    mPaddingB = ArrayOf<Int32>::Alloc(N);
}

//@Override
AutoPtr<IDrawableConstantState> LayerDrawable::GetConstantState()
{
    if (mLayerState->CanConstantState()) {
        mLayerState->mChangingConfigurations =
            Drawable::GetChangingConfigurations();
        return mLayerState;
    }
    return NULL;
}

//@Override
AutoPtr<IDrawable> LayerDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get()
        == (IDrawable*)this->Probe(EIID_IDrawable)) {
        ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
        Int32 N = mLayerState->mNum;
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IDrawable> drawable;
            (*array)[i]->mDrawable->Mutate((IDrawable**)&drawable);
        }
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode LayerDrawable::Init(
    /* [in] */ IObjectContainer* layers,
    /* [in] */ LayerState* state)
{
    Init(state, NULL);
    Int32 length;
    layers->GetObjectCount(&length);
    ArrayOf<AutoPtr<ChildDrawable> >* r =ArrayOf<AutoPtr<ChildDrawable> >::Alloc(length);

    AutoPtr<IObjectEnumerator> it;
    layers->GetObjectEnumerator((IObjectEnumerator**)&it);
    Boolean hasNext;
    for (Int32 i = 0; i < length; i++) {
        (*r)[i] = new ChildDrawable();
        it->MoveNext(&hasNext);
        assert(hasNext);
        AutoPtr<IDrawable> layer;
        it->Current((IInterface**)&layer);
        (*r)[i]->mDrawable = layer;
        layer->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        Int32 config;
        layer->GetChangingConfigurations(&config);
        mLayerState->mChildrenChangingConfigurations |= config;
    }
    mLayerState->mNum = length;
    mLayerState->mChildren = r;

    EnsurePadding();

    return NOERROR;
}

ECode LayerDrawable::Init(
    /* [in] */ LayerState* state,
    /* [in] */ IResources* res)
{
    AutoPtr<LayerState> as = CreateConstantState(state, res);
    mLayerState = as;
    if (as->mNum > 0) {
        EnsurePadding();
    }

    return NOERROR;
}
