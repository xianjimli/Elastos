#include "NumericShaper.h"


ArrayOf<Int32> * NumericShaper::STRONG_TEXT_FLAGS;

PInterface NumericShaper::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

NumericShaper::NumericShaper(){}

NumericShaper::NumericShaper(
    /* [in] */ Int32 ranges,
    /* [in] */ Int32 defaultContext,
    /* [in] */ Boolean isContextual)
{
    mFRanges = ranges;
    GetIndexFromRange(defaultContext, &mFDefaultContextIndex);
    mFContextual = isContextual;

    if (!mFContextual){
        GetIndexFromRange(ranges, &mFSingleRangeIndex);
    }
}

NumericShaper::~NumericShaper()
{
    if (mScriptsRanges != NULL) {
        ArrayOf<Int32>::Free(mScriptsRanges);
    }
    if (mDigitsLowRanges != NULL) {
        ArrayOf<Int32>::Free(mDigitsLowRanges);
    }
    if (mContexts != NULL) {
        ArrayOf<String>::Free(mContexts);
    }
    if (STRONG_TEXT_FLAGS != NULL) {
        ArrayOf<Int32>::Free(STRONG_TEXT_FLAGS);
    }
}

ECode NumericShaper::GetIndexFromRange(
        /* [in] */ Int32 range,
        /* [out] */ Int32 * result)
{
    assert(result != NULL);
    if (range == 0) {
        //throw rangeException(range);
        return NOERROR;
    }

    Int32 index = 0;
    while (index < MAX_INDEX){
        if (range == (1 << index)){
            *result = index;
            return NOERROR;
        }
        index++;
    }

    //throw rangeException(range);
    return NOERROR;
}

ECode NumericShaper::GetRangeFromIndex(
    /* [in] */ Int32 index,
    /* [out] */ Int32 * range)
{
    assert(range != NULL);
    if (index < 0 || index >= MAX_INDEX){
        //throw rangeException(index);
        return NOERROR;
    }
    *range = (1 << index);
    return NOERROR;
}

ECode NumericShaper::RangeException(
    /* [in] */ Int32 value)
{
    //throw new IllegalArgumentException("Illegal range argument value: " + value);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

INumericShaper* NumericShaper::GetContextualShaper(
        /* [in] */ Int32 ranges,
        /* [in] */ Int32 defaultContext)
{
    ranges &= ALL_RANGES;
    defaultContext &= ALL_RANGES;
    NumericShaper * ns = new NumericShaper(ranges, defaultContext, TRUE);
    INumericShaper * ins = reinterpret_cast<INumericShaper*>(ns->Probe(EIID_INumericShaper));
    return ins;
}

INumericShaper* NumericShaper::GetContextualShaper(
    /* [in] */ Int32 ranges)
{
    ranges &= ALL_RANGES;
    NumericShaper * ns = new NumericShaper(ranges, EUROPEAN, TRUE);
    INumericShaper * ins = reinterpret_cast<INumericShaper*>(ns->Probe(EIID_INumericShaper));    
    return ins;
}

ECode NumericShaper::GetRanges(
    /* [out] */ Int32 * ranges)
{
    assert(ranges != NULL);
    *ranges = mFRanges;
    return NOERROR;
}

INumericShaper* NumericShaper::GetShaper(
        /* [in] */ Int32 singleRange)
{
    singleRange &= ALL_RANGES;
    NumericShaper * ns = new NumericShaper(singleRange, EUROPEAN, FALSE);
    INumericShaper * ins = reinterpret_cast<INumericShaper*>(ns->Probe(EIID_INumericShaper));    
    return ins;
}

ECode NumericShaper::IsContextual(
        /* [out] */ Boolean * isContextual)
{
    assert(isContextual != NULL);
    *isContextual = mFContextual;
    return NOERROR;
}

ECode NumericShaper::Shape(
        /* [in] */ ArrayOf<Char32> * text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 context)
{
    Boolean isContextual;
    IsContextual(&isContextual);
    if (isContextual) {
        Int32 i;
        GetIndexFromRange(context, &i);
        ContextualShape(text, start, count, i);
    } else {
        NonContextualShape(text, start, count);
    }
    return NOERROR;
}

ECode NumericShaper::ShapeEx(
        /* [in] */ ArrayOf<Char32> * text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count)
{
    Boolean isContextual;
    IsContextual(&isContextual);
    if (isContextual) {
        ContextualShape(text, start, count, mFDefaultContextIndex);
    } else {
        NonContextualShape(text, start, count);
    }
    return NOERROR;
}

ECode NumericShaper::ContextualShape(
    /* [in] */ ArrayOf<Char32> * text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex)
{
    Char32 maxDigit = (Char32)0x0039;
    Char32 minDigit = (Char32)0x0030;

    Int32 currIndex;
    if (((1 << contextIndex) & mFRanges) == 0 ){
        currIndex = INDEX_EUROPEAN;
    } else {
        currIndex = contextIndex;
    }

    for (Int32 ind = start; ind < start + count; ind++){
        if (minDigit <= (*text)[ind] && (*text)[ind] <= maxDigit){
            if (currIndex != INDEX_ETHIOPIC || (*text)[ind] != '0'){
                (*text)[ind] = (Char32)((*mDigitsLowRanges)[currIndex] + (*text)[ind]);
            }
        } else {
            Boolean isCharStrong;
            IsCharStrong((*text)[ind], &isCharStrong);
            if (isCharStrong) {
                Int32 index;
                GetCharIndex((*text)[ind], &index);
                if (currIndex != index){
                    if (((1 << index) & mFRanges) != 0){
                        currIndex = index;
                    } else {
                        currIndex = INDEX_EUROPEAN;
                    }
                }
            }
        }
    }
    return NOERROR;
}

ECode NumericShaper::NonContextualShape(
    /* [in] */ ArrayOf<Char32> * text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    Char32 maxDigit = (Char32)0x0039;
    Char32 minDigit = (Char32)((mFRanges == ETHIOPIC) ? 0x0031 : 0x0030);
    for (Int32 ind = start; ind < start + count; ind++){
        if (minDigit <= (*text)[ind] && (*text)[ind] <= maxDigit){
                (*text)[ind] = (Char32)((*mDigitsLowRanges)[mFSingleRangeIndex] + (*text)[ind]);
        }
    }
    return NOERROR;
}

ECode NumericShaper::GetCharIndex(
    /* [in] */ Char32 ch,
    /* [out] */ Int32 * count)
{
    assert(count != NULL);
    Int32 index = INDEX_EUROPEAN;
    for (Int32 i=0; i < MAX_INDEX; i++){
        Int32 j = i * 2;
        if ((*mScriptsRanges)[j] <= ch && ch <= (*mScriptsRanges)[j+1]){
            *count = i;
            return NOERROR;
        }
    }
    *count = index;
    return NOERROR;
}

ECode NumericShaper::IsCharStrong(
    /* [in] */ Int32 chr,
    /* [out] */ Boolean * isCharStrong)
{
    assert(isCharStrong != NULL);
    *isCharStrong = (((*STRONG_TEXT_FLAGS)[chr >> 5] & (1 << (chr % 32))) != 0);
    return NULL;
}

ECode NumericShaper::UpdateRangesFields()
{
    mFRanges = (mMask & ~(1 << 31));
    mFContextual = ((mMask &(1 << 31)) != 0);
    if (mFContextual){
        mFRanges = (mMask & ~(1 << 31));
        mFDefaultContextIndex = mKey;
    } else {
        mFRanges = mMask;
        mFSingleRangeIndex = mKey;
    }
    return NOERROR;
}

ECode NumericShaper::UpdateKeyMaskFields()
{
    mMask = mFRanges;
    if (mFContextual){
        mMask |= (1 << 31);
        mKey = mFDefaultContextIndex;
    } else{
        mKey = mFSingleRangeIndex;
    }
    return NOERROR;
}