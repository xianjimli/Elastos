
#include "graphics/drawable/StateListDrawable.h"

const Boolean StateListDrawable::DEFAULT_DITHER;

StateListDrawable::StateListDrawable()
{
    Init();
}

StateListDrawable::~StateListDrawable()
{
    delete mStateListState;
}

PInterface StateListDrawable::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IStateListDrawable) {
        return (IStateListDrawable*)this;
    }

    return NULL;
}

/**
 * Add a new image/string ID to the set of images.
 *
 * @param stateSet - An array of resource Ids to associate with the image.
 *                 Switch to this image by calling setState().
 * @param drawable -The image to show.
 */
ECode StateListDrawable::AddState(
    /* [in] */ const ArrayOf<Int32>& stateSet,
    /* [in] */ IDrawable* drawable)
{
    if (drawable != NULL) {
        mStateListState->AddStateSet(stateSet, drawable);
        // in case the new state matches our current state...
        OnStateChange(GetState());
    }

    return NOERROR;
}

Boolean StateListDrawable::IsStateful()
{
    return TRUE;
}

Boolean StateListDrawable::OnStateChange(
    /* [in] */ ArrayOf<Int32>* stateSet)
{
    Int32 idx = mStateListState->IndexOfStateSet(stateSet);
    if (idx < 0) {
        idx = mStateListState->IndexOfStateSet((ArrayOf<Int32>*)StateSet::WILD_CARD);
    }
    if (SelectDrawable(idx)) {
        return TRUE;
    }
    return DrawableContainer::OnStateChange(stateSet);
}

ECode StateListDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{

    /*TypedArray a = r.obtainAttributes(attrs,
            com.android.internal.R.styleable.StateListDrawable);

    super.inflateWithAttributes(r, parser, a,
            com.android.internal.R.styleable.StateListDrawable_visible);

    mStateListState.setVariablePadding(a.getBoolean(
            com.android.internal.R.styleable.StateListDrawable_variablePadding, FALSE));
    mStateListState.setConstantSize(a.getBoolean(
            com.android.internal.R.styleable.StateListDrawable_constantSize, FALSE));

    setDither(a.getBoolean(com.android.internal.R.styleable.StateListDrawable_dither,
                           DEFAULT_DITHER));

    a.recycle();

    Int32 type;

    final Int32 innerDepth = parser.getDepth() + 1;
    Int32 depth;
    while ((type = parser.next()) != XmlPullParser.END_DOCUMENT
            && ((depth = parser.getDepth()) >= innerDepth
            || type != XmlPullParser.END_TAG)) {
        if (type != XmlPullParser.START_TAG) {
            continue;
        }

        if (depth > innerDepth || !parser.getName().equals("item")) {
            continue;
        }

        Int32 drawableRes = 0;

        Int32 i;
        Int32 j = 0;
        final Int32 numAttrs = attrs.getAttributeCount();
        Int32[] states = new Int32[numAttrs];
        for (i = 0; i < numAttrs; i++) {
            final Int32 stateResId = attrs.getAttributeNameResource(i);
            if (stateResId == 0) break;
            if (stateResId == com.android.internal.R.attr.drawable) {
                drawableRes = attrs.getAttributeResourceValue(i, 0);
            } else {
                states[j++] = attrs.getAttributeBooleanValue(i, FALSE)
                        ? stateResId
                        : -stateResId;
            }
        }
        states = StateSet.trimStateSet(states, j);

        Drawable dr;
        if (drawableRes != 0) {
            dr = r.getDrawable(drawableRes);
        } else {
            while ((type = parser.next()) == XmlPullParser.TEXT) {
            }
            if (type != XmlPullParser.START_TAG) {
                throw new XmlPullParserException(
                        parser.getPositionDescription()
                                + ": <item> tag requires a 'drawable' attribute or "
                                + "child tag defining a drawable");
            }
            dr = Drawable.createFromXmlInner(r, parser, attrs);
        }

        mStateListState.addStateSet(states, dr);
    }

    onStateChange(getState());*/

    return NOERROR;
}

StateListDrawable::StateListState* StateListDrawable::GetStateListState()
{
    return mStateListState;
}

/**
 * Gets the number of states contained in this drawable.
 *
 * @return The number of states contained in this drawable.
 * @hide pending API council
 * @see #getStateSet(Int32)
 * @see #getStateDrawable(Int32)
 */
Int32 StateListDrawable::GetStateCount()
{
    return mStateListState->GetChildCount();
}

/**
 * Gets the state set at an index.
 *
 * @param index The index of the state set.
 * @return The state set at the index.
 * @hide pending API council
 * @see #getStateCount()
 * @see #getStateDrawable(Int32)
 */
ArrayOf<Int32>* StateListDrawable::GetStateSet(
    /* [in] */ Int32 index)
{
    return (*mStateListState->mStateSets)[index];
}

/**
 * Gets the drawable at an index.
 *
 * @param index The index of the drawable.
 * @return The drawable at the index.
 * @hide pending API council
 * @see #getStateCount()
 * @see #getStateSet(Int32)
 */
AutoPtr<IDrawable> StateListDrawable::GetStateDrawable(
    /* [in] */ Int32 index)
{
    return (*mStateListState->GetChildren())[index];
}

/**
 * Gets the index of the drawable with the provided state set.
 *
 * @param stateSet the state set to look up
 * @return the index of the provided state set, or -1 if not found
 * @hide pending API council
 * @see #getStateDrawable(Int32)
 * @see #getStateSet(Int32)
 */
Int32 StateListDrawable::GetStateDrawableIndex(
    /* [in] */ const ArrayOf<Int32>& stateSet)
{
    return mStateListState->IndexOfStateSet((ArrayOf<Int32>*)&stateSet);
}

AutoPtr<IDrawable> StateListDrawable::Mutate()
{
    if (!mMutated && DrawableContainer::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        ArrayOf<ArrayOf<Int32>*>* sets = mStateListState->mStateSets;
        Int32 count = sets->GetLength();
        mStateListState->mStateSets = ArrayOf<ArrayOf<Int32>*>::Alloc(count);
        for (Int32 i = 0; i < count; i++) {
            ArrayOf<Int32>* set = (*sets)[i];
            if (set != NULL) {
                (*mStateListState->mStateSets)[i] = set->Clone();
            }
        }
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

StateListDrawable::StateListState::StateListState(
    /* [in] */ StateListState* orig,
    /* [in] */ StateListDrawable* owner,
    /* [in] */ IResources* res) : DrawableContainerState(orig, owner, res)
{
    if (orig != NULL) {
        mStateSets = orig->mStateSets;
    } else {
        mStateSets = ArrayOf<ArrayOf<Int32>*>::Alloc(GetChildren()->GetLength());
    }
}

StateListDrawable::StateListState::~StateListState()
{
    for (Int32 i = 0; i < mStateSets->GetLength(); i++) {
        ArrayOf<Int32>::Free((*mStateSets)[i]);
    }

    ArrayOf<ArrayOf<Int32>*>::Free(mStateSets);
}

Int32 StateListDrawable::StateListState::AddStateSet(
    /* [in] */ const ArrayOf<Int32>& stateSet,
    /* [in] */ IDrawable* drawable)
{
    Int32 pos = AddChild(drawable);
    (*mStateSets)[pos] = (ArrayOf<Int32>*)&stateSet;
    return pos;
}

Int32 StateListDrawable::StateListState::IndexOfStateSet(
    /* [in] */ ArrayOf<Int32>* stateSet)
{
    ArrayOf<ArrayOf<Int32>*>* stateSets = mStateSets;
    Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        if (StateSet::StateSetMatches((*stateSets)[i], stateSet)) {
            return i;
        }
    }
    return -1;
}

ECode StateListDrawable::StateListState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    //return CStateListDrawable::New((IStateListState*)this->Probe(EIID_IStateListState), NULL, drawable);
    StateListDrawable* as = new StateListDrawable(this, NULL);
    *drawable = (IDrawable*)as->Probe(EIID_IDrawable);

    return NOERROR;
}

ECode StateListDrawable::StateListState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    //return CStateListDrawable::New((IStateListState*)this->Probe(EIID_IStateListState), res, drawable);
    StateListDrawable* as = new StateListDrawable(this, res);
    *drawable = (IDrawable*)as->Probe(EIID_IDrawable);

    return NOERROR;
}

ECode StateListDrawable::StateListState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    DrawableContainerState::GrowArray(oldSize, newSize);
    ArrayOf<ArrayOf<Int32>*>* newStateSets = ArrayOf<ArrayOf<Int32>*>::Alloc(newSize);
    memcpy(newStateSets->GetPayload(), mStateSets->GetPayload(), oldSize * sizeof(ArrayOf<Int32>*));
    mStateSets = newStateSets;

    return NOERROR;
}

StateListDrawable::StateListDrawable(
    /* [in] */ StateListState* state,
    /* [in] */ IResources* res)
{
    Init(state, res);
}

void StateListDrawable::Init(
    /* [in] */ StateListState* state,
    /* [in] */ IResources* res)
{
    StateListState* as = new StateListState(state, this, res);
    mStateListState = as;
    SetConstantState(as);
    OnStateChange(GetState());
}
