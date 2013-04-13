#include "Bidi.h"
#include "CBidi.h"
#include "NativeBidi.h"


PInterface Bidi::Probe(
    /* [in]  */ REIID riid)
{
    return NULL;
}

Bidi::Bidi(){}

Bidi::~Bidi(){
    if (mOffsetLevel != NULL) {
        ArrayOf<Byte>::Free(mOffsetLevel);
    }
    if (mRuns != NULL) {
        ArrayOf<IBidiRun * >::Free(mRuns);
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
    ArrayOf<Char32>* text = ArrayOf<Char32>::Alloc(length + 1) ; // One more char for AttributedCharacterIterator.DONE
    Char32 c;
    if (length != 0) {
        paragraph->First(&((*text)[0]));
    } else {
        paragraph->First(&c);
    }

    // First check the RUN_DIRECTION attribute.
    Int32 flags = Bidi_DIRECTION_DEFAULT_LEFT_TO_RIGHT;
    AutoPtr<IInterface> direction;
    paragraph->GetAttribute(
        (IAttributedCharacterIteratorAttribute*)(TextAttribute::RUN_DIRECTION),
        (IInterface**)&direction);
    if (direction != NULL /*&& direction instanceof Boolean*/) {
        /*
        if (direction == TextAttribute::RUN_DIRECTION_LTR) {
            flags = Bidi_DIRECTION_LEFT_TO_RIGHT;
        } else {
            flags = Bidi_DIRECTION_RIGHT_TO_LEFT;
        }*/
    }

    // Retrieve the text and gather BIDI_EMBEDDINGS
    ArrayOf<Byte>* embeddings = NULL;
    Int32 textLimit = 1, i = 1;
    while (i < length) {
        AutoPtr<IInterface> embedding;
        paragraph->GetAttribute(
            (IAttributedCharacterIteratorAttribute*)(TextAttribute::BIDI_EMBEDDING),
            (IInterface**)&embedding);
        if (embedding != NULL /*&& embedding instanceof Integer*/) {
            Int32 embLevel /*= (Int32)embedding*/;

            if (embeddings == NULL) {
                embeddings = ArrayOf<Byte>::Alloc(length);
            }

            for (; i < textLimit; i++) {
                paragraph->Next(&((*text)[i]));
                (*embeddings)[i - 1] = (Byte)embLevel;
            }
        } else {
            for (; i < textLimit; i++) {
                paragraph->Next(&((*text)[i]));
            }
        }
        paragraph->GetRunLimitEx(
            (IAttributedCharacterIteratorAttribute*)(TextAttribute::BIDI_EMBEDDING),
            &textLimit);
        textLimit = textLimit - begin + 1;
    }

    // Apply NumericShaper to the text
    AutoPtr<IInterface> numericShaper;
    paragraph->GetAttribute(
        (IAttributedCharacterIteratorAttribute*)(TextAttribute::NUMERIC_SHAPING),
        (IInterface**)&numericShaper);
    if (numericShaper != NULL && (numericShaper->Probe(EIID_INumericShaper)) != NULL ) {
        AutoPtr<INumericShaper> ns = reinterpret_cast<INumericShaper*>(numericShaper->Probe(EIID_INumericShaper));
        ns->ShapeEx(text, 0, length);
    }

    Int64 bidi = 0L;
    //try {
    bidi = CreateUBiDi(text, 0, embeddings, 0, length, flags);
    ReadBidiInfo(bidi);
    //} finally {
    NativeBidi::Ubidi_close(bidi);
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

    Int64 bidi = 0L;
    //try {
    bidi = CreateUBiDi(text, textStart, embeddings, embStart, paragraphLength, flags);
    ReadBidiInfo(bidi);
    //} finally {
    NativeBidi::Ubidi_close(bidi);
    //}
    return NOERROR;
}

ECode Bidi::Init(
        /* [in] */ String paragraph,
        /* [in] */ Int32 flags)
{
    ArrayOf<Char32>* text = ArrayOf<Char32>::Alloc(paragraph.GetLength());
    for (Int32 i = 0; i < (Int32)(paragraph.GetLength()); ++i) {
        (*text)[i] = paragraph.GetChar(i);
    }
    return Init((paragraph.IsNull() ? NULL : text), 0, NULL, 0,
            (paragraph.IsNull() ? 0 : paragraph.GetLength()), flags);
}

Int64 Bidi::CreateUBiDi(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte>* embeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags)
{
    ArrayOf<Char32>* realText = NULL;

    ArrayOf<Byte>* realEmbeddings = NULL;

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
            AutoPtr<IBidi> temp;
            CBidi::New(text, textStart, NULL, 0, paragraphLength, flags, (IBidi**)&temp);
            realEmbeddings = ArrayOf<Byte>::Alloc(paragraphLength);
            for (Int32 i = 0; i < paragraphLength; ++i) {
                ArrayOf<Byte>* offsetLevel;
                temp->GetOffsetLevel(&offsetLevel);
                (*realEmbeddings)[i] = (*offsetLevel)[i];
            }
            for (Int32 i = 0; i < paragraphLength; i++) {
                Byte e = (*embeddings)[i];
                if (e < 0) {
                    (*realEmbeddings)[i] = (Byte) (NativeBidi::UBIDI_LEVEL_OVERRIDE - e);
                    (*realEmbeddings)[i] = (Byte) (0x80 - e);
                } else if (e > 0) {
                    (*realEmbeddings)[i] = e;
                } else {
                    (*realEmbeddings)[i] |= (Byte) NativeBidi::UBIDI_LEVEL_OVERRIDE;
                    (*realEmbeddings)[i] |= (Byte) 0x80;
                }
            }
        }
    }

    if (flags > 1 || flags < -2) {
        flags = 0;
    }

    Int64 bidi = 0L;
    Boolean needsDeletion = TRUE;
    //try {
    bidi = NativeBidi::Ubidi_open();
    NativeBidi::Ubidi_setPara(bidi, realText, paragraphLength, flags, realEmbeddings);
    needsDeletion = FALSE;
    //} finally {
    if (needsDeletion) {
        NativeBidi::Ubidi_close(bidi);
    }
    //}
    return bidi;
}

Bidi::Bidi(
        /* [in] */ Int64 pBidi)
{
    ReadBidiInfo(pBidi);
}

void Bidi::ReadBidiInfo(
        /* [in] */ Int64 pBidi)
{
    mLength = NativeBidi::Ubidi_getLength(pBidi);

    mOffsetLevel = (mLength == 0) ? NULL : NativeBidi::Ubidi_getLevels(pBidi);

    mBaseLevel = NativeBidi::Ubidi_getParaLevel(pBidi);

    Int32 runCount = NativeBidi::Ubidi_countRuns(pBidi);
    if (runCount == 0) {
        mUnidirectional = TRUE;
        mRuns = NULL;
    } else if (runCount < 0) {
        mRuns = NULL;
    } else {
        mRuns = NativeBidi::Ubidi_getRuns(pBidi);

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

    ArrayOf<Char32>* text = ArrayOf<Char32>::Alloc(mLength);
    for (Int32 i = 0; i < text->GetLength(); i++) {
        (*text)[i] = 'a';
    }
    ArrayOf<Byte>* embeddings = ArrayOf<Byte>::Alloc(mLength);
    for (Int32 i = 0; i < embeddings->GetLength(); i++) {
        (*embeddings)[i] = (Byte)(-(*mOffsetLevel)[i]);
    }
    Boolean isLefttoRight;
    BaseIsLeftToRight(&isLefttoRight);
    Int32 dir = isLefttoRight ? Bidi_DIRECTION_LEFT_TO_RIGHT : Bidi_DIRECTION_RIGHT_TO_LEFT;
    Int64 parent = 0;
    //try {
    parent = CreateUBiDi(text, 0, embeddings, 0, mLength, dir);
    if (lineStart == lineLimit) {
        return CreateEmptyLineBidi(parent, lineBidi);
    }
    Bidi * bidi = new Bidi(NativeBidi::Ubidi_setLine(parent, lineStart, lineLimit));
    *lineBidi = reinterpret_cast<IBidi*>(bidi->Probe(EIID_IBidi));
    //} finally {
    NativeBidi::Ubidi_close(parent);
    //}
    return NOERROR;
}

ECode Bidi::CreateEmptyLineBidi(
        /* [in] */ Int64 parent,
        /* [out] */ IBidi** lineBidi)
{
    // ICU4C doesn't allow this case, but the RI does.
    Bidi * result = new Bidi(parent);
    result->mLength = 0;
    result->mOffsetLevel = NULL;
    result->mRuns = NULL;
    result->mUnidirectional = TRUE;
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

ECode Bidi::SetLength(
        /* [in] */ Int32 length)
{
    mLength = length;
    return NOERROR;
}

ECode Bidi::GetOffsetLevel(
    /* [out] */ ArrayOf<Byte>** offsetLevel)
{
    *offsetLevel = mOffsetLevel->Clone();
    return NOERROR;
}

ECode Bidi::SetOffsetLevel(
    /* [in] */ ArrayOf<Byte>* offsetLevel)
{
    mOffsetLevel = offsetLevel->Clone();
    return NOERROR;
}

ECode Bidi::SetUnidirectional(
    /* [in] */ Boolean unidirectional)
{
    mUnidirectional = unidirectional;
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
    assert(runLimit != NULL);
    Int32 limit;
    (*mRuns)[run]->GetLimit(&limit);
    *runLimit = mUnidirectional ? mLength : limit;
    return NOERROR;

}

ECode Bidi::GetRunStart(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runStart)
{
    assert(runStart != NULL);
    Int32 start;
    (*mRuns)[run]->GetStart(&start);
    *runStart = mUnidirectional ? 0 : start;
    return NOERROR;
}

ECode Bidi::IsLeftToRight(
        /* [out] */ Boolean* isLefttoRight)
{
    assert(isLefttoRight != NULL);
    *isLefttoRight = (mDirection == NativeBidi::UBiDiDirection_UBIDI_LTR);
    return NOERROR;
}

ECode Bidi::IsMixed(
        /* [out] */ Boolean* isMixed)
{
    assert(isMixed != NULL);
    *isMixed = (mDirection == NativeBidi::UBiDiDirection_UBIDI_MIXED);
    return NOERROR;
}

ECode Bidi::IsRightToLeft(
        /* [out] */ Boolean* isRightToLeft)
{
    assert(isRightToLeft != NULL);
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

    ArrayOf<Byte>* realLevels = ArrayOf<Byte>::Alloc(count);
    for (Int32 i = 0; i < count; i++) {
        (*realLevels)[i] = (*levels)[levelStart + i];
    }

    ArrayOf<Int32>* indices = ArrayOf<Int32>::Alloc(count)
                            /* = NativeBidi.ubidi_reorderVisual(realLevels, count)*/;

    List<IInterface*>* result/* = new ArrayList<Object>(count)*/;
    for (Int32 i = 0; i < count; i++) {
        result->PushBack((*objects)[objectStart + (*indices)[i]]);
    }
    List<IInterface*>::Iterator it = result->Begin();
    for (Int32 i = 0; i < count; i++, ++it) {
        (*objects)[objectStart + i] = *it;
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