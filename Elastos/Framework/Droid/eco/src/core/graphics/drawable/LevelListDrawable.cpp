
#include "graphics/drawable/LevelListDrawable.h"
#include "graphics/drawable/CLevelListDrawable.h"


LevelListDrawable::LevelListState::LevelListState(
    /* [in] */ LevelListState* orig,
    /* [in] */ LevelListDrawable* owner,
    /* [in] */ IResources* res)
    : DrawableContainerState(orig, owner, res)
    , mLows(NULL)
    , mHighs(NULL)
{
    if (orig != NULL) {
        mLows = orig->mLows;
        mHighs = orig->mHighs;
    }
    else {
        mLows = ArrayOf<Int32>::Alloc(GetChildren()->GetLength());
        mHighs = ArrayOf<Int32>::Alloc(GetChildren()->GetLength());
    }
}

LevelListDrawable::LevelListState::~LevelListState()
{
    if (static_cast<LevelListDrawable*>(mOwner)->mMutated) {
        ArrayOf<Int32>::Free(mLows);
        ArrayOf<Int32>::Free(mHighs);
    }
}

void LevelListDrawable::LevelListState::AddLevel(
    /* [in] */ Int32 low,
    /* [in] */ Int32 high,
    /* [in] */ IDrawable* drawable)
{
    Int32 pos = AddChild(drawable);
    (*mLows)[pos] = low;
    (*mHighs)[pos] = high;
}

Int32 LevelListDrawable::LevelListState::IndexOfLevel(
    /* [in] */ Int32 level)
{
    const ArrayOf<Int32>* lows = mLows;
    const ArrayOf<Int32>* highs = mHighs;
    const Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        if (level >= (*lows)[i] && level <= (*highs)[i]) {
            return i;
        }
    }
    return -1;
}

//@Override
ECode LevelListDrawable::LevelListState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CLevelListDrawable::New(
            (Handle32)this, NULL, (ILevelListDrawable**)drawable);
}

//@Override
ECode LevelListDrawable::LevelListState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CLevelListDrawable::New(
            (Handle32)this, res, (ILevelListDrawable**)drawable);
}

//@Override
void LevelListDrawable::LevelListState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    DrawableContainerState::GrowArray(oldSize, newSize);
    ArrayOf<Int32>* newInts = ArrayOf<Int32>::Alloc(newSize);
    memcpy(newInts->GetPayload(), mLows->GetPayload(), oldSize * sizeof(Int32));
    ArrayOf<Int32>::Free(mLows);
    mLows = newInts;
    newInts = ArrayOf<Int32>::Alloc(newSize);
    memcpy(newInts->GetPayload(), mHighs->GetPayload(), oldSize * sizeof(Int32));
    ArrayOf<Int32>::Free(mHighs);
    mHighs = newInts;
}



LevelListDrawable::LevelListDrawable()
    : mMutated(FALSE)
{
//    this(null, null);
}

LevelListDrawable::LevelListDrawable(
    /* [in] */ LevelListState* state,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
{
    mLevelListState = new LevelListState(state, this, res);
    SetConstantState(mLevelListState);
    OnLevelChange(GetLevel());
}

ECode LevelListDrawable::AddLevel(
    /* [in] */ Int32 low,
    /* [in] */ Int32 high,
    /* [in] */ IDrawable* drawable)
{
    if (drawable != NULL) {
        mLevelListState->AddLevel(low, high, drawable);
        // in case the new state matches our current state...
        OnLevelChange(GetLevel());
    }
    return NOERROR;
}

//@Override
Boolean LevelListDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Int32 idx = mLevelListState->IndexOfLevel(level);
    if (SelectDrawable(idx)) {
        return TRUE;
    }
    return DrawableContainer::OnLevelChange(level);
}

static Int32 R_Styleable_LevelListDrawableItem[] = {
    0x01010199, 0x010101b1, 0x010101b2
};

//@Override
ECode LevelListDrawable::Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(DrawableContainer::Inflate(r, parser, attrs));

    Int32 type;

    Int32 low = 0;

    Int32 innerDepth, depth;
    parser->GetDepth(&depth);
    innerDepth = depth + 1;
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
            && ((parser->GetDepth(&depth), depth >= innerDepth)
            || type != IXmlPullParser_END_TAG)) {
        if (type != IXmlPullParser_START_TAG) {
            continue;
        }

        String name;
        if (depth > innerDepth || (parser->GetName(&name), !name.Equals("item"))) {
            continue;
        }

        AutoPtr<ITypedArray> a;
        r->ObtainAttributes(attrs,
                ArrayOf<Int32>(R_Styleable_LevelListDrawableItem,
                sizeof(R_Styleable_LevelListDrawableItem) / sizeof(Int32)),/*com.android.internal.R.styleable.LevelListDrawableItem*/
                (ITypedArray**)&a);

        a->GetInt32(
                1/*com.android.internal.R.styleable.LevelListDrawableItem_minLevel*/,
                0, &low);
        Int32 high;
        a->GetInt32(
                2/*com.android.internal.R.styleable.LevelListDrawableItem_maxLevel*/,
                0, &high);
        Int32 drawableRes;
        a->GetResourceId(
                0/*com.android.internal.R.styleable.LevelListDrawableItem_drawable*/,
                0, &drawableRes);

        a->Recycle();

        if (high < 0) {
//            throw new XmlPullParserException(parser.getPositionDescription()
//                    + ": <item> tag requires a 'maxLevel' attribute");
            return E_XML_PULL_PARSER_EXCEPTION;
        }

        AutoPtr<IDrawable> dr;
        if (drawableRes != 0) {
            r->GetDrawable(drawableRes, (IDrawable**)&dr);
        }
        else {
            while (parser->Next(&type), type == IXmlPullParser_TEXT) {
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

        mLevelListState->AddLevel(low, high, dr);
    }

    OnLevelChange(GetLevel());

    return NOERROR;
}

//@Override
AutoPtr<IDrawable> LevelListDrawable::Mutate()
{
    if (!mMutated &&
            DrawableContainer::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mLevelListState->mLows = mLevelListState->mLows->Clone();
        mLevelListState->mHighs = mLevelListState->mHighs->Clone();
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode LevelListDrawable::Init()
{
    return Init(NULL, NULL);
}

ECode LevelListDrawable::Init(
    /* [in] */ LevelListState* state,
    /* [in] */ IResources* res)
{
    mLevelListState = new LevelListState(state, this, res);
    SetConstantState(mLevelListState);
    OnLevelChange(GetLevel());
    return NOERROR;
}
