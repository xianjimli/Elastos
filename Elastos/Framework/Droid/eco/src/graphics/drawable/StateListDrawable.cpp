
#include "graphics/drawable/StateListDrawable.h"
#include "graphics/drawable/CStateListDrawable.h"

const Boolean StateListDrawable::DEFAULT_DITHER;

StateListDrawable::StateListDrawable()
{
}

StateListDrawable::StateListDrawable(
    /* [in] */ StateListState* state,
    /* [in] */ IResources* res)
{
    Init(state, res);
}

ECode StateListDrawable::Init(
    /* [in] */ StateListState* state,
    /* [in] */ IResources* res)
{
    mStateListState = new StateListState(state, NULL, res);
    SetConstantState(mStateListState);
    OnStateChange(GetState());
    return NOERROR;
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
        mStateListState->AddStateSet(&stateSet, drawable);
        // in case the new state matches our current state...
        OnStateChange(GetState());
    }

    return NOERROR;
}

Boolean StateListDrawable::IsStateful()
{
    return TRUE;
}
//#include <stdio.h>
Boolean StateListDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    Int32 idx = mStateListState->IndexOfStateSet(stateSet);
    //printf("StateListDrawable::OnStateChange idx = %d\n", idx);
    if (idx < 0) {
        idx = mStateListState->IndexOfStateSet(const_cast<ArrayOf<Int32>*>(StateSet::WILD_CARD));
    }
    if (SelectDrawable(idx)) {
        return TRUE;
    }
    return DrawableContainer::OnStateChange(stateSet);
}

static Int32 R_Styleable_StateListDrawable[] = {
    0x0101011c, 0x01010194, 0x01010195, 0x01010196
};

ECode StateListDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs,
            ArrayOf<Int32>(R_Styleable_StateListDrawable, sizeof(R_Styleable_StateListDrawable) / sizeof(Int32)),/*com.android.internal.R.styleable.StateListDrawable*/
            (ITypedArray**)&a));

    Drawable::InflateWithAttributes(r, parser, a,
            1/*com.android.internal.R.styleable.StateListDrawable_visible*/);

    Boolean value;
    a->GetBoolean(
            2,/*com.android.internal.R.styleable.StateListDrawable_variablePadding*/
            FALSE, &value);
    mStateListState->SetVariablePadding(value);
    a->GetBoolean(
            3,/*com.android.internal.R.styleable.StateListDrawable_constantSize*/
            FALSE, &value);
    mStateListState->SetConstantSize(value);
    a->GetBoolean(
            0,/*com.android.internal.R.styleable.StateListDrawable_dither*/
            DEFAULT_DITHER, &value);
    SetDither(value);
    a->Recycle();

    Int32 innerDepth;
    parser->GetDepth(&innerDepth);
    innerDepth += 1;
    Int32 type, depth;
    FAIL_RETURN(parser->Next(&type));
    FAIL_RETURN(parser->GetDepth(&depth));
    while(type != IXmlPullParser_END_DOCUMENT &&
            (depth >= innerDepth || type != IXmlPullParser_END_TAG)) {

        if (type != IXmlPullParser_START_TAG) {
            FAIL_RETURN(parser->Next(&type));
            FAIL_RETURN(parser->GetDepth(&depth));
            continue;
        }

        String name;
        parser->GetName(&name);

        if (depth > innerDepth || !name.Equals("item")) {
            FAIL_RETURN(parser->Next(&type));
            FAIL_RETURN(parser->GetDepth(&depth));
            continue;
        }

        Int32 drawableRes = 0;

        Int32 i;
        Int32 j = 0;
        Int32 numAttrs;
        attrs->GetAttributeCount(&numAttrs);
        ArrayOf<Int32>* states = ArrayOf<Int32>::Alloc(numAttrs);
        for (i = 0; i < numAttrs; i++) {
            Int32 stateResId;
            attrs->GetAttributeNameResource(i, &stateResId);
            if (stateResId == 0) break;
            if (stateResId == 0x01010199/*com.android.internal.R.attr.drawable*/) {
                attrs->GetAttributeResourceValueEx(i, 0, &drawableRes);
            }
            else {
                Boolean value;
                attrs->GetAttributeBooleanValueEx(i, FALSE, &value);
                if (value) {
                    (*states)[j++] = stateResId;
                }
                else {
                    (*states)[j++] = -stateResId;
                }
            }
        }
        ArrayOf<Int32>* temp = StateSet::TrimStateSet(states, j);
        ArrayOf<Int32>::Free(states);
        states = temp;

        AutoPtr<IDrawable> dr;
        if (drawableRes != 0) {
            r->GetDrawable(drawableRes, (IDrawable**)&dr);
            //printf("StateListDrawable::Inflate drawable = 0x%08x\n", dr.Get());
        }
        else {
            FAIL_RETURN(parser->Next(&type));
            while (type == IXmlPullParser_TEXT) {
                FAIL_RETURN(parser->Next(&type));
            }
            if (type != IXmlPullParser_START_TAG) {
//                throw new XmlPullParserException(
//                        parser.getPositionDescription()
//                                + ": <item> tag requires a 'drawable' attribute or "
//                                + "child tag defining a drawable");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
        }

        //TODO:
        //
        if (dr != NULL) {
            mStateListState->AddStateSet(states, dr);
        }

        FAIL_RETURN(parser->Next(&type));
        FAIL_RETURN(parser->GetDepth(&depth));
    }

    OnStateChange(GetState());
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
    /* [in] */ IStateListDrawable* owner,
    /* [in] */ IResources* res)
    : DrawableContainerState(orig, owner, res)
    , mStateSets(NULL)
{
    if (orig != NULL) {
        ArrayOf<ArrayOf<Int32>*>* origSS = orig->mStateSets;
        if (origSS != NULL) {
            Int32 N = origSS->GetLength();
            mStateSets = ArrayOf<ArrayOf<Int32>*>::Alloc(N);
            for (Int32 i = 0; i < N; i++) {
                (*mStateSets)[i] = (*origSS)[i]->Clone();
            }
        }
    }
    else {
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
    /* [in] */ const ArrayOf<Int32>* stateSet,
    /* [in] */ IDrawable* drawable)
{
    Int32 pos = AddChild(drawable);
    (*mStateSets)[pos] = const_cast<ArrayOf<Int32>*>(stateSet);
    return pos;
}

Int32 StateListDrawable::StateListState::IndexOfStateSet(
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        if (StateSet::StateSetMatches((*mStateSets)[i], stateSet)) {
            return i;
        }
    }
    return -1;
}

// IDrawableConstantState
//
ECode StateListDrawable::StateListState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CStateListDrawable::New((Handle32)this, NULL,
        (IStateListDrawable**)drawable);
}

ECode StateListDrawable::StateListState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CStateListDrawable::New((Handle32)this, res,
        (IStateListDrawable**)drawable);
}

void StateListDrawable::StateListState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    DrawableContainerState::GrowArray(oldSize, newSize);
    ArrayOf<ArrayOf<Int32>*>* newStateSets = ArrayOf<ArrayOf<Int32>*>::Alloc(newSize);
    for (Int32 i = 0; i < oldSize; i++) {
        (*newStateSets)[i] = (*mStateSets)[i];
    }
    ArrayOf<ArrayOf<Int32>*>::Free(mStateSets);
    mStateSets = newStateSets;
}

