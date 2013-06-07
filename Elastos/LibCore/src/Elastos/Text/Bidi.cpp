#include "Bidi.h"
#include "CBidi.h"
#include "NativeBidi.h"
#include <elastos/AutoFree.h>

Bidi::Bidi()
    : mBaseLevel(0)
    , mLength(0)
    , mDirection(0)
    , mUnidirectional(FALSE)
{}

Bidi::~Bidi()
{
    if (mOffsetLevel != NULL) {
        ArrayOf<Byte>::Free(mOffsetLevel);
    }
    if (mRuns != NULL) {
        for (Int32 i = 0; i < mRuns->GetLength(); ++i) {
            (*mRuns)[i] = NULL;
        }
        ArrayOf<AutoPtr<IBidiRun> >::Free(mRuns);
    }
}

ECode Bidi::Init(
    /* [in] */ IAttributedCharacterIterator* paragraph)
{
    if (paragraph == NULL) {
        //throw new IllegalArgumentException("paragraph is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 begin;
    paragraph->GetBeginIndex(&begin);
    Int32 end;
    paragraph->GetEndIndex(&end);
    Int32 length = end - begin;
    AutoFree< ArrayOf<Char32> > text = ArrayOf<Char32>::Alloc(length + 1) ; // One more char for AttributedCharacterIterator.DONE
    if (length != 0) {
        paragraph->First(&(*text)[0]);
    } else {
        Char32 c;
        paragraph->First(&c);
    }

    // First check the RUN_DIRECTION attribute.
    Int32 flags = IBidi_DIRECTION_DEFAULT_LEFT_TO_RIGHT;
    AutoPtr<IInterface> direction;
    paragraph->GetAttribute(
        (IAttributedCharacterIteratorAttribute*)TextAttribute::RUN_DIRECTION->Probe(EIID_IAttributedCharacterIteratorAttribute),
        (IInterface**)&direction);
    if (direction != NULL && direction->Probe(EIID_IBoolean) != NULL) {
        AutoPtr<IBoolean> b = (IBoolean*)direction->Probe(EIID_IBoolean);
        Boolean value;
        b->GetValue(&value);
        if (value == TextAttribute::RUN_DIRECTION_LTR) {
            flags = IBidi_DIRECTION_LEFT_TO_RIGHT;
        }
        else {
            flags = IBidi_DIRECTION_RIGHT_TO_LEFT;
        }
    }

    // Retrieve the text and gather BIDI_EMBEDDINGS
    AutoFree< ArrayOf<Byte> > embeddings = NULL;
    Int32 textLimit = 1, i = 1;
    while (i < length) {
        AutoPtr<IInterface> embedding;
        paragraph->GetAttribute(
            (IAttributedCharacterIteratorAttribute*)TextAttribute::BIDI_EMBEDDING->Probe(EIID_IAttributedCharacterIteratorAttribute),
            (IInterface**)&embedding);
        if (embedding != NULL && embedding->Probe(EIID_IInteger32) != NULL) {
            AutoPtr<IInteger32> integer = (IInteger32*)embedding->Probe(EIID_IInteger32);
            Int32 embLevel;
            integer->GetValue(&embLevel);

            if (embeddings == NULL) {
                embeddings = ArrayOf<Byte>::Alloc(length);
            }

            for (; i < textLimit; i++) {
                paragraph->Next(&(*text)[i]);
                (*embeddings)[i - 1] = (Byte)embLevel;
            }
        }
        else {
            for (; i < textLimit; i++) {
                paragraph->Next(&(*text)[i]);
            }
        }
        paragraph->GetRunLimitEx(
            (IAttributedCharacterIteratorAttribute*)TextAttribute::BIDI_EMBEDDING->Probe(EIID_IAttributedCharacterIteratorAttribute),
            &textLimit);
        textLimit = textLimit - begin + 1;
    }

    // Apply NumericShaper to the text
    AutoPtr<IInterface> numericShaper;
    paragraph->GetAttribute(
        (IAttributedCharacterIteratorAttribute*)TextAttribute::NUMERIC_SHAPING->Probe(EIID_IAttributedCharacterIteratorAttribute),
        (IInterface**)&numericShaper);
    if (numericShaper != NULL && numericShaper->Probe(EIID_INumericShaper) != NULL ) {
        AutoPtr<INumericShaper> ns = (INumericShaper*)numericShaper->Probe(EIID_INumericShaper);
        ns->ShapeEx(text, 0, length);
    }

    Int64 ubidi = 0ll;
    //try {
    ECode ec = CreateUBiDi(text, 0, embeddings, 0, length, flags, &ubidi);
    if (FAILED(ec)) {
        NativeBidi::Ubidi_close(ubidi);
        return ec;
    }
    ReadBidiInfo(ubidi);
    //} finally {
    NativeBidi::Ubidi_close(ubidi);
    //}
    return NOERROR;
}

ECode Bidi::Init(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 textStart,
    /* [in] */ ArrayOf<Byte>* embeddings,
    /* [in] */ Int32 embStart,
    /* [in] */ Int32 paragraphLength,
    /* [in] */ Int32 flags)
{
    if (text == NULL || text->GetLength() - textStart < paragraphLength) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (embeddings != NULL) {
        if (embeddings->GetLength() - embStart < paragraphLength) {
            //throw new IllegalArgumentException();
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    if (textStart < 0) {
        //throw new IllegalArgumentException("Negative textStart value " + textStart);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (embStart < 0) {
        //throw new IllegalArgumentException("Negative embStart value " + embStart);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (paragraphLength < 0) {
        //throw new IllegalArgumentException("Negative paragraph length " + paragraphLength);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 ubidi = 0ll;
    //try {
    ECode ec = CreateUBiDi(text, textStart, embeddings, embStart,
            paragraphLength, flags, &ubidi);
    if (FAILED(ec)) {
        NativeBidi::Ubidi_close(ubidi);
        return ec;
    }
    ReadBidiInfo(ubidi);
    //} finally {
    NativeBidi::Ubidi_close(ubidi);
    //}
    return NOERROR;
}

ECode Bidi::Init(
    /* [in] */ String paragraph,
    /* [in] */ Int32 flags)
{
    Int32 count = paragraph.GetCharCount();
    AutoFree< ArrayOf<Char32> > text = ArrayOf<Char32>::Alloc(count);
    for (Int32 i = 0; i < count; ++i) {
        (*text)[i] = paragraph.GetChar(i);
    }
    FAIL_RETURN(Init((paragraph.IsNull() ? NULL : text), 0, NULL, 0,
            (paragraph.IsNull() ? 0 : paragraph.GetLength()), flags));

    return NOERROR;
}

ECode Bidi::Init(
    /* [in] */ Int64 pBidi)
{
    ReadBidiInfo(pBidi);
    return NOERROR;
}

ECode Bidi::CreateUBiDi(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 textStart,
    /* [in] */ ArrayOf<Byte>* embeddings,
    /* [in] */ Int32 embStart,
    /* [in] */ Int32 paragraphLength,
    /* [in] */ Int32 flags,
    /* [out] */ Int64* uBiDi)
{
    VALIDATE_NOT_NULL(uBiDi);

    AutoFree< ArrayOf<Char32> > realText = NULL;

    AutoFree< ArrayOf<Byte> > realEmbeddings = NULL;

    if (text == NULL || text->GetLength() - textStart < paragraphLength) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    realText = ArrayOf<Char32>::Alloc(paragraphLength);
    for (Int32 i = 0; i < paragraphLength; ++i) {
        (*realText)[i] = (*text)[textStart + i];
    }

    if (embeddings != NULL) {
        if (embeddings->GetLength() - embStart < paragraphLength) {
            //throw new IllegalArgumentException();
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (paragraphLength > 0) {
            AutoPtr<CBidi> temp;
            CBidi::NewByFriend(text, textStart, NULL, 0, paragraphLength, flags,
                    (CBidi**)&temp);
            realEmbeddings = ArrayOf<Byte>::Alloc(paragraphLength);
            for (Int32 i = 0; i < paragraphLength; ++i) {
                (*realEmbeddings)[i] = (*(temp->mOffsetLevel))[i];
            }

            for (Int32 i = 0; i < paragraphLength; i++) {
                Byte e = (*embeddings)[i];
                if (e < 0) {
                    (*realEmbeddings)[i] = (Byte)NativeBidi::UBIDI_LEVEL_OVERRIDE - e;
                }
                else if (e > 0) {
                    (*realEmbeddings)[i] = e;
                } else {
                    (*realEmbeddings)[i] |= (Byte)NativeBidi::UBIDI_LEVEL_OVERRIDE;
                }
            }
        }
    }

    if (flags > 1 || flags < -2) {
        flags = 0;
    }

    Int64 bidi = 0ll;
    bidi = NativeBidi::Ubidi_open();
    ECode ec = NativeBidi::Ubidi_setPara(bidi, realText, paragraphLength,
            flags, realEmbeddings);
    if (FAILED(ec)) {
        NativeBidi::Ubidi_close(bidi);
        return ec;
    }

    *uBiDi = bidi;
    return NOERROR;
}

void Bidi::ReadBidiInfo(
        /* [in] */ Int64 pBidi)
{
    mLength = NativeBidi::Ubidi_getLength(pBidi);

    if (mLength == 0) {
        mOffsetLevel = NULL;
    }
    else {
        NativeBidi::Ubidi_getLevels(pBidi, &mOffsetLevel);
    }

    mBaseLevel = NativeBidi::Ubidi_getParaLevel(pBidi);

    Int32 runCount;
    NativeBidi::Ubidi_countRuns(pBidi, &runCount);
    if (runCount == 0) {
        mUnidirectional = TRUE;
        mRuns = NULL;
    } else if (runCount < 0) {
        mRuns = NULL;
    } else {
        NativeBidi::Ubidi_getRuns(pBidi, &mRuns);

        // Simplified case for one run which has the base level
        Int32 level;
        (*mRuns)[0]->GetLevel(&level);
        if (runCount == 1 &&  level == mBaseLevel) {
            mUnidirectional = TRUE;
            mRuns = NULL;
        }
    }

    mDirection = NativeBidi::Ubidi_getDirection(pBidi);
}

ECode Bidi::BaseIsLeftToRight(
        /* [out] */ Boolean* isLeftToRight)
{
    VALIDATE_NOT_NULL(isLeftToRight);
    *isLeftToRight = (mBaseLevel % 2 == 0);
    return NOERROR;
}

ECode Bidi::CreateLineBidi(
    /* [in] */ Int32 lineStart,
    /* [in] */ Int32 lineLimit,
    /* [out] */ IBidi** lineBidi)
{
    if (lineStart < 0 || lineLimit < 0 || lineLimit > mLength || lineStart > lineLimit) {
        //throw new IllegalArgumentException("Invalid ranges (start=" + lineStart + ", " +
        //        "limit=" + lineLimit + ", length=" + length + ")");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoFree< ArrayOf<Char32> > text = ArrayOf<Char32>::Alloc(mLength);
    for (Int32 i = 0; i < mLength; i++) {
        (*text)[i] = 'a';
    }
    AutoFree< ArrayOf<Byte> > embeddings = ArrayOf<Byte>::Alloc(mLength);
    for (Int32 i = 0; i < mLength; i++) {
        (*embeddings)[i] = (Byte)-(*mOffsetLevel)[i];
    }

    Boolean isLefttoRight;
    BaseIsLeftToRight(&isLefttoRight);
    Int32 dir = isLefttoRight ? IBidi_DIRECTION_LEFT_TO_RIGHT : IBidi_DIRECTION_RIGHT_TO_LEFT;
    Int64 parent = 0ll;

    ECode ec = CreateUBiDi(text, 0, embeddings, 0, mLength, dir, &parent);
    if (FAILED(ec)) {
        NativeBidi::Ubidi_close(parent);
        return ec;
    }
    if (lineStart == lineLimit) {
        return CreateEmptyLineBidi(parent, lineBidi);
    }

    AutoPtr<IBidi> temp;
    Int64 line;
    NativeBidi::Ubidi_setLine(parent, lineStart, lineLimit, &line);
    ASSERT_SUCCEEDED(CBidi::New(line, (IBidi**)&temp));
    *lineBidi = temp;
    (*lineBidi)->AddRef();
    return NOERROR;
}

ECode Bidi::CreateEmptyLineBidi(
    /* [in] */ Int64 parent,
    /* [out] */ IBidi** lineBidi)
{
    VALIDATE_NOT_NULL(lineBidi);
    // ICU4C doesn't allow this case, but the RI does.
    AutoPtr<CBidi> result;
    ASSERT_SUCCEEDED(CBidi::NewByFriend(parent, (CBidi**)&result));
    result->mLength = 0;
    result->mOffsetLevel = NULL;
    result->mRuns = NULL;
    result->mUnidirectional = TRUE;

    *lineBidi = (IBidi*)result->Probe(EIID_IBidi);
    return NOERROR;
}

ECode Bidi::GetBaseLevel(
    /* [out] */ Int32* baseLevel)
{
    VALIDATE_NOT_NULL(baseLevel);
    *baseLevel = mBaseLevel;
    return NOERROR;
}

ECode Bidi::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    *length = mLength;
    return NOERROR;
}

ECode Bidi::GetLevelAt(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* level)
{
    VALIDATE_NOT_NULL(level);
    //try {
    *level = (*mOffsetLevel)[offset] & ~NativeBidi::UBIDI_LEVEL_OVERRIDE;
    //} catch (RuntimeException e) {
    //    return baseLevel;
    //}
    return NOERROR;
}

ECode Bidi::GetRunCount(
    /* [out] */ Int32* runCount)
{
    VALIDATE_NOT_NULL(runCount);
    *runCount = mUnidirectional ? 1 : mRuns->GetLength();
    return NOERROR;
}

ECode Bidi::GetRunLevel(
    /* [in] */ Int32 run,
    /* [out] */ Int32* runLevel)
{
    VALIDATE_NOT_NULL(runLevel);
    Int32 level;
    (*mRuns)[run]->GetLevel(&level);
    *runLevel = mUnidirectional ? mBaseLevel : level;
    return NOERROR;
}

ECode Bidi::GetRunLimit(
    /* [in] */ Int32 run,
    /* [out] */ Int32* runLimit)
{
    VALIDATE_NOT_NULL(runLimit);
    Int32 limit;
    (*mRuns)[run]->GetLimit(&limit);
    *runLimit = mUnidirectional ? mLength : limit;
    return NOERROR;

}

ECode Bidi::GetRunStart(
    /* [in] */ Int32 run,
    /* [out] */ Int32* runStart)
{
    VALIDATE_NOT_NULL(runStart);
    Int32 start;
    (*mRuns)[run]->GetStart(&start);
    *runStart = mUnidirectional ? 0 : start;
    return NOERROR;
}

ECode Bidi::IsLeftToRight(
    /* [out] */ Boolean* isLefttoRight)
{
    VALIDATE_NOT_NULL(isLefttoRight);
    *isLefttoRight = (mDirection == NativeBidi::UBiDiDirection_UBIDI_LTR);
    return NOERROR;
}

ECode Bidi::IsMixed(
    /* [out] */ Boolean* isMixed)
{
    VALIDATE_NOT_NULL(isMixed);
    *isMixed = (mDirection == NativeBidi::UBiDiDirection_UBIDI_MIXED);
    return NOERROR;
}

ECode Bidi::IsRightToLeft(
    /* [out] */ Boolean* isRightToLeft)
{
    VALIDATE_NOT_NULL(isRightToLeft);
    *isRightToLeft = (mDirection == NativeBidi::UBiDiDirection_UBIDI_RTL);
    return NOERROR;
}

ECode Bidi::ReorderVisually(
    /* [in] */ ArrayOf<Byte>* levels,
    /* [in] */ Int32 levelStart,
    /* [in] */ ArrayOf<IInterface*>* objects,
    /* [in] */ Int32 objectStart,
    /* [in] */ Int32 count)
{
    if (count < 0 || levelStart < 0 || objectStart < 0
            || count > levels->GetLength() - levelStart
            || count > objects->GetLength() - objectStart) {
        //throw new IllegalArgumentException("Invalid ranges (levels=" + levels.length +
        //        ", levelStart=" + levelStart + ", objects=" + objects.length +
        //        ", objectStart=" + objectStart + ", count=" + count + ")");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoFree< ArrayOf<Byte> > realLevels = ArrayOf<Byte>::Alloc(count);
    for (Int32 i = 0; i < count; i++) {
        (*realLevels)[i] = (*levels)[levelStart + i];
    }

    AutoFree< ArrayOf<Int32> > indices = NativeBidi::Ubidi_reorderVisual(realLevels, count);

    AutoFree< ArrayOf<AutoPtr<IInterface> > > result =
            ArrayOf<AutoPtr<IInterface> >::Alloc(count);
    Int32 i = 0;
    for (; i < count; i++) {
        (*result)[i] = (*objects)[objectStart + (*indices)[i]];
    }

    for (i = 0; i < count; i++) {
        (*objects)[objectStart + i] = (*result)[i];
    }

    return NOERROR;
}

ECode Bidi::RequiresBidi(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [out] */ Boolean * result)
{
    if (limit < 0 || start < 0 || start > limit || limit > text->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IBidi> bidi;
    CBidi::New(text, start, NULL, 0, limit - start, 0, (IBidi**)&bidi);
    Boolean isLefttoRight;
    bidi->IsLeftToRight(&isLefttoRight);
    *result = !isLefttoRight;
    return NOERROR;
}