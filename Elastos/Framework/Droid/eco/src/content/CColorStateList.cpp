
#include "content/CColorStateList.h"
#include "utils/StateSet.h"
#include "utils/Xml.h"
#include "utils/ArrayUtils.h"
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

ArrayOf<Handle32>* InitEMPTY()
{
    ArrayOf<Int32>* state = ArrayOf<Int32>::Alloc(1);
    (*state)[0] = 0;
    ArrayOf<Handle32>* states = ArrayOf<Handle32>::Alloc(1);
    (*states)[0] = (Handle32)state;
    return states;
}

ArrayOf<Handle32>* CColorStateList::EMPTY = InitEMPTY();
Map<Int32, AutoPtr<IColorStateList> > CColorStateList::sCache;
Mutex CColorStateList::sCacheLock;

CColorStateList::CColorStateList()
    : mStateSpecs(NULL)
    , mColors(NULL)
    , mDefaultColor(0xffff0000)
{}

CColorStateList::~CColorStateList()
{
    if (mStateSpecs != NULL) {
        for (Int32 i = 0; i < mStateSpecs->GetLength(); i++) {
            ArrayOf<Int32>* state = (ArrayOf<Int32>*)(*mStateSpecs)[i];
            ArrayOf<Int32>::Free(state);
        }
        ArrayOf<Handle32>::Free(mStateSpecs);
    }
    ArrayOf<Int32>::Free(mColors);
}

ECode CColorStateList::ValueOf(
    /* [in] */ Int32 color,
    /* [out] */ IColorStateList** _csl)
{
    VALIDATE_NOT_NULL(_csl);

    //TODO: should we collect these eventually?
    Mutex::Autolock lock(sCacheLock);

    AutoPtr<IColorStateList> csl;
    Map<Int32, AutoPtr<IColorStateList> >::Iterator it = sCache.Find(color);
    if (it != sCache.End()) csl = it->mSecond;

    if (csl == NULL) {
        ArrayOf<Int32>* colors = ArrayOf<Int32>::Alloc(1);
        (*colors)[0] = color;
        CColorStateList::New(*EMPTY, *colors, (IColorStateList**)&csl);
        ArrayOf<Int32>::Free(colors);
        sCache[color] = csl;
    }

    *_csl = csl.Get();
    (*_csl)->AddRef();
    return NOERROR;
}

/**
 * Create a ColorStateList from an XML document, given a set of {@link Resources}.
 */
ECode CColorStateList::CreateFromXml(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IColorStateList** csl)
{
    VALIDATE_NOT_NULL(csl);

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    Int32 type;
    FAIL_RETURN(parser->Next(&type));
    while (type != IXmlPullParser_START_TAG
        && type != IXmlPullParser_END_DOCUMENT) {
        FAIL_RETURN(parser->Next(&type));
    }

    if (type != IXmlPullParser_START_TAG) {
        Logger::E("CColorStateList", "No start tag found");

        return E_XML_PULL_PARSER_EXCEPTION;
    }

    return CreateFromXmlInner(r, parser, attrs, csl);
}

/* Create from inside an XML document.  Called on a parser positioned at
 * a tag in an XML document, tries to create a ColorStateList from that tag.
 * Returns null if the tag is not a valid ColorStateList.
 */
ECode CColorStateList::CreateFromXmlInner(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IColorStateList** csl)
{
    String name;
    FAIL_RETURN(parser->GetName((String*)&name));

    if (name.Equals("selector")) {
        FAIL_RETURN(CColorStateList::New(csl));
    }
    else {
        String pos;
        parser->GetPositionDescription(&pos);
        Logger::E(
            "CColorStateList",
            StringBuffer(pos) + ": invalid drawable tag " + name);

        return E_XML_PULL_PARSER_EXCEPTION;
    }

    return ((CColorStateList*)(*csl))->Inflate(r, parser, attrs);
}

ECode CColorStateList::WithAlpha(
    /* [in] */ Int32 alpha,
    /* [out] */ IColorStateList** colorState)
{
    VALIDATE_NOT_NULL(colorState);

    ArrayOf<Int32>* colors = ArrayOf<Int32>::Alloc(mColors->GetLength());

    Int32 len = colors->GetLength();
    for (Int32 i = 0; i < len; i++) {
        (*colors)[i] = ((*mColors)[i] & 0xFFFFFF) | (alpha << 24);
    }

    return CColorStateList::New(*mStateSpecs, *colors, colorState);
}

/**
 * Fill in this object based on the contents of an XML "selector" element.
 */
ECode CColorStateList::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 type;

    Int32 innerDepth;
    FAIL_RETURN(parser->GetDepth(&innerDepth));
    innerDepth++;

    Int32 depth;

    Int32 listAllocated = 20;
    Int32 listSize = 0;
    ArrayOf<Int32>* colorList = ArrayOf<Int32>::Alloc(listAllocated);
    ArrayOf<Handle32>* stateSpecList = ArrayOf<Handle32>::Alloc(listAllocated);

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

        Int32 colorRes = 0;
        Int32 color = 0xffff0000;
        Boolean haveColor = FALSE;

        Int32 i;
        Int32 j = 0;
        Int32 numAttrs;
        attrs->GetAttributeCount(&numAttrs);
        ArrayOf<Int32>* stateSpec = ArrayOf<Int32>::Alloc(numAttrs);
        for (i = 0; i < numAttrs; i++) {
            Int32 stateResId;
            attrs->GetAttributeNameResource(i, &stateResId);

            if (stateResId == 0)
                break;

            if (stateResId == 0x010101a5/*com.android.internal.R.attr.color*/) {
                attrs->GetAttributeResourceValueEx(i, 0, &colorRes);

                if (colorRes == 0) {
                    attrs->GetAttributeIntValueEx(i, color, &color);
                    haveColor = TRUE;
                }
            }
            else {
                Boolean value;
                attrs->GetAttributeBooleanValueEx(i, FALSE, &value);
                (*stateSpec)[j++] = value ? stateResId : -stateResId;
            }
        }
        ArrayOf<Int32>* temp = StateSet::TrimStateSet(stateSpec, j);
        ArrayOf<Int32>::Free(stateSpec);
        stateSpec = temp;

        if (colorRes != 0) {
            r->GetColor(colorRes, &color);
        }
        else if (!haveColor) {
            String pos;
            parser->GetPositionDescription(&pos);
            Logger::E(
                "CColorStateList", pos +
                ": <item> tag requires a 'android:color' attribute.");

            return E_XML_PULL_PARSER_EXCEPTION;
        }

        if (listSize == 0 || stateSpec->GetLength() == 0) {
            mDefaultColor = color;
        }

        if (listSize + 1 >= listAllocated) {
            listAllocated = ArrayUtils::IdealInt32ArraySize(listSize + 1);

            ArrayOf<Int32>* ncolor = ArrayOf<Int32>::Alloc(listAllocated);
            memcpy(ncolor->GetPayload(), colorList->GetPayload(), listSize * sizeof(Int32));

            ArrayOf<Handle32>* nstate = ArrayOf<Handle32>::Alloc(listAllocated);
            memcpy(nstate->GetPayload(), stateSpecList->GetPayload(), listSize * sizeof(Handle32));

            ArrayOf<Int32>::Free(colorList);
            ArrayOf<Handle32>::Free(stateSpecList);
            colorList = ncolor;
            stateSpecList = nstate;
        }

        (*colorList)[listSize] = color;
        (*stateSpecList)[listSize] = (Handle32)stateSpec;
        listSize++;
    }

    mColors = StateSet::TrimStateSet(colorList, listSize);
    mStateSpecs = ArrayOf<Handle32>::Alloc(listSize);
    memcpy(mStateSpecs->GetPayload(),
        stateSpecList->GetPayload(),listSize * sizeof(Handle32));

    ArrayOf<Int32>::Free(colorList);
    ArrayOf<Handle32>::Free(stateSpecList);

    return NOERROR;
}
ECode CColorStateList::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    VALIDATE_NOT_NULL(isStateful);
    *isStateful = mStateSpecs->GetLength() > 1;

    return NOERROR;
}

ECode CColorStateList::GetColorForState(
    /* [in] */ ArrayOf<Int32>* stateSet,
    /* [in] */ Int32 defaultColor,
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    Int32 setLength = mStateSpecs->GetLength();
    for (Int32 i = 0; i < setLength; i++) {
        ArrayOf<Int32>* stateSpec = (ArrayOf<Int32>*)(*mStateSpecs)[i];
        if (StateSet::StateSetMatches(stateSpec, stateSet)) {
            *color = (*mColors)[i];
            return NOERROR;
        }
    }
    *color = defaultColor;

    return NOERROR;
}

ECode CColorStateList::GetDefaultColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = mDefaultColor;

    return NOERROR;
}

ECode CColorStateList::constructor()
{
    return NOERROR;
}

ECode CColorStateList::constructor(
    /* [in] */ const ArrayOf<Handle32> & states,
    /* [in] */ const ArrayOf<Int32> & colors)
{
    assert(states.GetLength() == colors.GetLength());
    mStateSpecs = states.Clone();
    for (Int32 i = 0; i < states.GetLength(); i++) {
        (*mStateSpecs)[i] = (Handle32)((ArrayOf<Int32>*)states[i])->Clone();
    }
    mColors = colors.Clone();

    if (states.GetLength() > 0) {
        mDefaultColor = colors[0];

        for (Int32 i = 0; i < states.GetLength(); i++) {
            if (((ArrayOf<Int32>*)states[i])->GetLength() == 0) {
                mDefaultColor = colors[i];
            }
        }
    }
    return NOERROR;
}

