#include "Bidi.h"

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
    ArrayOf<Char32>* text = ArrayOf<Char32>::Alloc(length + 1); // One more char for AttributedCharacterIterator.DONE

    if (length != 0) {
        paragraph->First(&((*text)[0]));
    } else {
        Char32 c;
        paragraph->First(&c);
    }

    // First check the RUN_DIRECTION attribute.
    Int32 flags = Bidi_DIRECTION_DEFAULT_LEFT_TO_RIGHT;
    Object direction = paragraph.getAttribute(TextAttribute.RUN_DIRECTION);
    if (direction != null && direction instanceof Boolean) {
        if (direction.equals(TextAttribute.RUN_DIRECTION_LTR)) {
            flags = Bidi_DIRECTION_LEFT_TO_RIGHT;
        } else {
            flags = Bidi_DIRECTION_RIGHT_TO_LEFT;
        }
    }

    // Retrieve the text and gather BIDI_EMBEDDINGS
    byte[] embeddings = null;
    for (int textLimit = 1, i = 1; i < length; textLimit = paragraph
            .getRunLimit(TextAttribute.BIDI_EMBEDDING)
            - begin + 1) {
        Object embedding = paragraph.getAttribute(TextAttribute.BIDI_EMBEDDING);
        if (embedding != null && embedding instanceof Integer) {
            int embLevel = ((Integer) embedding).intValue();

            if (embeddings == null) {
                embeddings = new byte[length];
            }

            for (; i < textLimit; i++) {
                text[i] = paragraph.next();
                embeddings[i - 1] = (byte) embLevel;
            }
        } else {
            for (; i < textLimit; i++) {
                text[i] = paragraph.next();
            }
        }
    }

    // Apply NumericShaper to the text
    Object numericShaper = paragraph.getAttribute(TextAttribute.NUMERIC_SHAPING);
    if (numericShaper != null && numericShaper instanceof NumericShaper) {
        ((NumericShaper) numericShaper).shape(text, 0, length);
    }

    long bidi = 0;
    try {
        bidi = createUBiDi(text, 0, embeddings, 0, length, flags);
        readBidiInfo(bidi);
    } finally {
        NativeBidi.ubidi_close(bidi);
    }
}

ECode Bidi::Init(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte>* embeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags)
{
    if (text == null || text.length - textStart < paragraphLength) {
        throw new IllegalArgumentException();
    }

    if (embeddings != null) {
        if (embeddings.length - embStart < paragraphLength) {
            throw new IllegalArgumentException();
        }
    }

    if (textStart < 0) {
        throw new IllegalArgumentException("Negative textStart value " + textStart);
    }
    if (embStart < 0) {
        throw new IllegalArgumentException("Negative embStart value " + embStart);
    }
    if (paragraphLength < 0) {
        throw new IllegalArgumentException("Negative paragraph length " + paragraphLength);
    }

    long bidi = 0;
    try {
        bidi = createUBiDi(text, textStart, embeddings, embStart, paragraphLength, flags);
        readBidiInfo(bidi);
    } finally {
        NativeBidi.ubidi_close(bidi);
    }
}

ECode Bidi::Init(
        /* [in] */ String paragraph,
        /* [in] */ Int32 flags)
{
    this((paragraph == null ? null : paragraph.toCharArray()), 0, null, 0,
            (paragraph == null ? 0 : paragraph.length()), flags);
}

Int64 Bidi::CreateUBiDi(
        /* [in] */ ArrayOf<Char32>* texts,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte>* embeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags)
{
    char[] realText = null;

    byte[] realEmbeddings = null;

    if (text == null || text.length - textStart < paragraphLength) {
        throw new IllegalArgumentException();
    }
    realText = new char[paragraphLength];
    System.arraycopy(text, textStart, realText, 0, paragraphLength);

    if (embeddings != null) {
        if (embeddings.length - embStart < paragraphLength) {
            throw new IllegalArgumentException();
        }
        if (paragraphLength > 0) {
            Bidi temp = new Bidi(text, textStart, null, 0, paragraphLength, flags);
            realEmbeddings = new byte[paragraphLength];
            System.arraycopy(temp.offsetLevel, 0, realEmbeddings, 0, paragraphLength);
            for (int i = 0; i < paragraphLength; i++) {
                byte e = embeddings[i];
                if (e < 0) {
                    realEmbeddings[i] = (byte) (NativeBidi.UBIDI_LEVEL_OVERRIDE - e);
                } else if (e > 0) {
                    realEmbeddings[i] = e;
                } else {
                    realEmbeddings[i] |= (byte) NativeBidi.UBIDI_LEVEL_OVERRIDE;
                }
            }
        }
    }

    if (flags > 1 || flags < -2) {
        flags = 0;
    }

    long bidi = 0;
    boolean needsDeletion = true;
    try {
        bidi = NativeBidi.ubidi_open();
        NativeBidi.ubidi_setPara(bidi, realText, paragraphLength, flags, realEmbeddings);
        needsDeletion = false;
    } finally {
        if (needsDeletion) {
            NativeBidi.ubidi_close(bidi);
        }
    }
    return bidi;
}

Bidi::Bidi(
        /* [in] */ Int64 pBidi)
{
    readBidiInfo(pBidi);
}

void Bidi::ReadBidiInfo(
        /* [in] */ Int64 pBidi) 
{
    length = NativeBidi.ubidi_getLength(pBidi);

    offsetLevel = (length == 0) ? null : NativeBidi.ubidi_getLevels(pBidi);

    baseLevel = NativeBidi.ubidi_getParaLevel(pBidi);

    int runCount = NativeBidi.ubidi_countRuns(pBidi);
    if (runCount == 0) {
        unidirectional = true;
        runs = null;
    } else if (runCount < 0) {
        runs = null;
    } else {
        runs = NativeBidi.ubidi_getRuns(pBidi);

        // Simplified case for one run which has the base level
        if (runCount == 1 && runs[0].getLevel() == baseLevel) {
            unidirectional = true;
            runs = null;
        }
    }

    direction = NativeBidi.ubidi_getDirection(pBidi);
}

ECode Bidi::BaseIsLeftToRight(
        /* [out] */ Boolean* isLeftToRight)
{
    return baseLevel % 2 == 0 ? true : false;
}

ECode Bidi::CreateLineBidi(
        /* [in] */ Int32 lineStart,
        /* [in] */ Int32 lineLimit,
        /* [out] */ IBidi** lineBidi)
{
    if (lineStart < 0 || lineLimit < 0 || lineLimit > length || lineStart > lineLimit) {
        throw new IllegalArgumentException("Invalid ranges (start=" + lineStart + ", " +
                "limit=" + lineLimit + ", length=" + length + ")");
    }

    char[] text = new char[this.length];
    Arrays.fill(text, 'a');
    byte[] embeddings = new byte[this.length];
    for (int i = 0; i < embeddings.length; i++) {
        embeddings[i] = (byte) -this.offsetLevel[i];
    }

    int dir = this.baseIsLeftToRight()
            ? Bidi.DIRECTION_LEFT_TO_RIGHT
            : Bidi.DIRECTION_RIGHT_TO_LEFT;
    long parent = 0;
    try {
        parent = createUBiDi(text, 0, embeddings, 0, this.length, dir);
        if (lineStart == lineLimit) {
            return createEmptyLineBidi(parent);
        }
        return new Bidi(NativeBidi.ubidi_setLine(parent, lineStart, lineLimit));
    } finally {
        NativeBidi.ubidi_close(parent);
    }
}

ECode Bidi::CreateEmptyLineBidi(
        /* [in] */ Int64 parent,
        /* [out] */ IBidi* lineBidi)
{
    // ICU4C doesn't allow this case, but the RI does.
    Bidi result = new Bidi(parent);
    result.length = 0;
    result.offsetLevel = null;
    result.runs = null;
    result.unidirectional = true;
    return result;
}

ECode Bidi::GetBaseLevel(
        /* [out] */ Int32* baseLevel)
{
    return baseLevel;
}

ECode Bidi::GetLength(
        /* [out] */ Int32* length)
{
    return length;
}

ECode Bidi::GetLevelAt(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* level)
{
    try {
        return offsetLevel[offset] & ~NativeBidi.UBIDI_LEVEL_OVERRIDE;
    } catch (RuntimeException e) {
        return baseLevel;
    }
}

ECode Bidi::GetRunCount(
        /* [out] */ Int32* runCount)
{
    return unidirectional ? 1 : runs.length;
}

ECode Bidi::GetRunLevel(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runLevel)
{
    return unidirectional ? baseLevel : runs[run].getLevel();
}

ECode Bidi::GetRunLimit(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runLimit)
{
    return unidirectional ? length : runs[run].getLimit();
}

ECode Bidi::GetRunStart(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runStart)
{
    return unidirectional ? 0 : runs[run].getStart();
}

ECode Bidi::IsLeftToRight(
        /* [out] */ Boolean* isLefttoRight)
{
    return direction == NativeBidi.UBiDiDirection_UBIDI_LTR;
}

ECode Bidi::IsMixed(
        /* [out] */ Boolean* isMixed)
{
    return direction == NativeBidi.UBiDiDirection_UBIDI_MIXED;
}

ECode Bidi::IsRightToLeft(
        /* [out] */ Boolean* isRightToLeft)
{
    return direction == NativeBidi.UBiDiDirection_UBIDI_RTL;
}

void Bidi::ReorderVisually(
        /* [in] */ ArrayOf<Byte>* levels,
        /* [in] */ Int32 levelStart,
        /* [in] */ ArrayOf<IInterface*>* objects,
        /* [in] */ Int32 objectStart,
        /* [in] */ Int32 count)
{
    if (count < 0 || levelStart < 0 || objectStart < 0
            || count > levels.length - levelStart
            || count > objects.length - objectStart) {
        throw new IllegalArgumentException("Invalid ranges (levels=" + levels.length +
                ", levelStart=" + levelStart + ", objects=" + objects.length +
                ", objectStart=" + objectStart + ", count=" + count + ")");
    }

    byte[] realLevels = new byte[count];
    System.arraycopy(levels, levelStart, realLevels, 0, count);

    int[] indices = NativeBidi.ubidi_reorderVisual(realLevels, count);

    ArrayList<Object> result = new ArrayList<Object>(count);
    for (int i = 0; i < count; i++) {
        result.add(objects[objectStart + indices[i]]);
    }

    System.arraycopy(result.toArray(), 0, objects, objectStart, count);
}

Boolean Bidi::RequiresBidi(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit)
{
    if (limit < 0 || start < 0 || start > limit || limit > text.length) {
        throw new IllegalArgumentException();
    }

    Bidi bidi = new Bidi(text, start, null, 0, limit - start, 0);
    return !bidi.isLeftToRight();
}