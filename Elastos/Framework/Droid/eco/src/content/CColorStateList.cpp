
#include "content/CColorStateList.h"


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

ECode CColorStateList::WithAlpha(
    /* [in] */ Int32 alpha,
    /* [out] */ IColorStateList** colors)
{
    return E_NOT_IMPLEMENTED;
}

ECode CColorStateList::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    return E_NOT_IMPLEMENTED;
}

ECode CColorStateList::GetColorForState(
    /* [in] */ ArrayOf<Int32>* stateSet,
    /* [in] */ Int32 defaultColor,
    /* [out] */ Int32* color)
{
    return E_NOT_IMPLEMENTED;
}

ECode CColorStateList::GetDefaultColor(
    /* [out] */ Int32 * pColor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CColorStateList::constructor()
{
    return NOERROR;
}

ECode CColorStateList::constructor(
    /* [in] */ const ArrayOf<Handle32> & states,
    /* [in] */ const ArrayOf<Int32> & colors)
{
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

