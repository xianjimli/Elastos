#ifndef __BIDI_H__
#define __BIDI_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include "TextAttribute.h"
//#include "CBidi.h"
#include <elastos/List.h>
using namespace Elastos;

class Bidi {
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    Bidi();

    virtual ~Bidi();
    /**
     * Creates a {@code Bidi} object from the {@code
     * AttributedCharacterIterator} of a paragraph text. The RUN_DIRECTION
     * attribute determines the base direction of the bidirectional text. If it
     * is not specified explicitly, the algorithm uses
     * DIRECTION_DEFAULT_LEFT_TO_RIGHT by default. The BIDI_EMBEDDING attribute
     * specifies the level of embedding for each character. Values between -1
     * and -62 denote overrides at the level's absolute value, values from 1 to
     * 62 indicate embeddings, and the 0 value indicates the level is calculated
     * by the algorithm automatically. For the character with no BIDI_EMBEDDING
     * attribute or with a improper attribute value, such as a {@code null}
     * value, the algorithm treats its embedding level as 0. The NUMERIC_SHAPING
     * attribute specifies the instance of NumericShaper used to convert
     * European digits to other decimal digits before performing the bidi
     * algorithm.
     *
     * @param paragraph
     *            the String containing the paragraph text to perform the
     *            algorithm.
     * @throws IllegalArgumentException if {@code paragraph == null}
     * @see java.awt.font.TextAttribute#BIDI_EMBEDDING
     * @see java.awt.font.TextAttribute#NUMERIC_SHAPING
     * @see java.awt.font.TextAttribute#RUN_DIRECTION
     */
    CARAPI Init(
        /* [in] */ IAttributedCharacterIterator* paragraph);

    /**
     * Creates a {@code Bidi} object.
     *
     * @param text
     *            the char array of the paragraph text that is processed.
     * @param textStart
     *            the index in {@code text} of the start of the paragraph.
     * @param embeddings
     *            the embedding level array of the paragraph text, specifying
     *            the embedding level information for each character. Values
     *            between -1 and -61 denote overrides at the level's absolute
     *            value, values from 1 to 61 indicate embeddings, and the 0
     *            value indicates the level is calculated by the algorithm
     *            automatically.
     * @param embStart
     *            the index in {@code embeddings} of the start of the paragraph.
     * @param paragraphLength
     *            the length of the text to perform the algorithm.
     * @param flags
     *            indicates the base direction of the bidirectional text. It is
     *            expected that this will be one of the direction constant
     *            values defined in this class. An unknown value is treated as
     *            DIRECTION_DEFAULT_LEFT_TO_RIGHT.
     * @throws IllegalArgumentException
     *             if {@code textStart}, {@code embStart}, or {@code
     *             paragraphLength} is negative; if
     *             {@code text.length < textStart + paragraphLength} or
     *             {@code embeddings.length < embStart + paragraphLength}.
     * @see #DIRECTION_LEFT_TO_RIGHT
     * @see #DIRECTION_RIGHT_TO_LEFT
     * @see #DIRECTION_DEFAULT_RIGHT_TO_LEFT
     * @see #DIRECTION_DEFAULT_LEFT_TO_RIGHT
     */
    CARAPI Init(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte>* embeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags);

    /**
     * Creates a {@code Bidi} object.
     *
     * @param paragraph
     *            the string containing the paragraph text to perform the
     *            algorithm on.
     * @param flags
     *            indicates the base direction of the bidirectional text. It is
     *            expected that this will be one of the direction constant
     *            values defined in this class. An unknown value is treated as
     *            DIRECTION_DEFAULT_LEFT_TO_RIGHT.
     * @see #DIRECTION_LEFT_TO_RIGHT
     * @see #DIRECTION_RIGHT_TO_LEFT
     * @see #DIRECTION_DEFAULT_RIGHT_TO_LEFT
     * @see #DIRECTION_DEFAULT_LEFT_TO_RIGHT
     */
    CARAPI Init(
        /* [in] */ String paragraph,
        /* [in] */ Int32 flags);

    /**
     * Returns whether the base level is from left to right.
     *
     * @return true if the base level is from left to right.
     */
    CARAPI BaseIsLeftToRight(
        /* [out] */ Boolean* isLeftToRight);

    /**
     * Creates a new {@code Bidi} object containing the information of one line
     * from this object.
     *
     * @param lineStart
     *            the start offset of the line.
     * @param lineLimit
     *            the limit of the line.
     * @return the new line Bidi object. In this new object, the indices will
     *         range from 0 to (limit - start - 1).
     * @throws IllegalArgumentException
     *             if {@code lineStart < 0}, {@code lineLimit < 0}, {@code
     *             lineStart > lineLimit} or if {@code lineStart} is greater
     *             than the length of this object's paragraph text.
     */
    CARAPI CreateLineBidi(
        /* [in] */ Int32 lineStart,
        /* [in] */ Int32 lineLimit,
        /* [out] */ IBidi** lineBidi);

    /**
     * Returns the base level.
     *
     * @return the base level.
     */
    CARAPI GetBaseLevel(
        /* [out] */ Int32* baseLevel);

    /**
     * Returns the length of the text in the {@code Bidi} object.
     *
     * @return the length.
     */
    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI SetLength(
        /* [in] */ Int32 length);

    CARAPI GetOffsetLevel(
        /* [out] */ ArrayOf<Byte>** offsetLevel);

    CARAPI SetOffsetLevel(
        /* [in] */ ArrayOf<Byte>* offsetLevel);

    CARAPI SetUnidirectional(
        /* [in] */ Boolean unidirectional);

    /**
     * Returns the level of a specified character.
     *
     * @param offset
     *            the offset of the character.
     * @return the level.
     */
    CARAPI GetLevelAt(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* level);

    /**
     * Returns the number of runs in the bidirectional text.
     *
     * @return the number of runs, at least 1.
     */
    CARAPI GetRunCount(
        /* [out] */ Int32* runCount);

    /**
     * Returns the level of the specified run.
     *
     * @param run
     *            the index of the run.
     * @return the level of the run.
     */
    CARAPI GetRunLevel(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runLevel);

    /**
     * Returns the limit offset of the specified run.
     *
     * @param run
     *            the index of the run.
     * @return the limit offset of the run.
     */
    CARAPI GetRunLimit(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runLimit);

    /**
     * Returns the start offset of the specified run.
     *
     * @param run
     *            the index of the run.
     * @return the start offset of the run.
     */
    CARAPI GetRunStart(
        /* [in] */ Int32 run,
        /* [out] */ Int32* runStart);

    /**
     * Indicates whether the text is from left to right, that is, both the base
     * direction and the text direction is from left to right.
     *
     * @return {@code true} if the text is from left to right; {@code false}
     *         otherwise.
     */
    CARAPI IsLeftToRight(
        /* [out] */ Boolean* isLefttoRight);

    /**
     * Indicates whether the text direction is mixed.
     *
     * @return {@code true} if the text direction is mixed; {@code false}
     *         otherwise.
     */
    CARAPI IsMixed(
        /* [out] */ Boolean* isMixed);

    /**
     * Indicates whether the text is from right to left, that is, both the base
     * direction and the text direction is from right to left.
     *
     * @return {@code true} if the text is from right to left; {@code false}
     *         otherwise.
     */
    CARAPI IsRightToLeft(
        /* [out] */ Boolean* isRightToLeft);

    /**
     * Reorders a range of objects according to their specified levels. This is
     * a convenience function that does not use a {@code Bidi} object. The range
     * of objects at {@code index} from {@code objectStart} to {@code
     * objectStart + count} will be reordered according to the range of levels
     * at {@code index} from {@code levelStart} to {@code levelStart + count}.
     *
     * @param levels
     *            the level array, which is already determined.
     * @param levelStart
     *            the start offset of the range of the levels.
     * @param objects
     *            the object array to reorder.
     * @param objectStart
     *            the start offset of the range of objects.
     * @param count
     *            the count of the range of objects to reorder.
     * @throws IllegalArgumentException
     *             if {@code count}, {@code levelStart} or {@code objectStart}
     *             is negative; if {@code count > levels.length - levelStart} or
     *             if {@code count > objects.length - objectStart}.
     */
    static CARAPI ReorderVisually(
        /* [in] */ ArrayOf<Byte>* levels,
        /* [in] */ Int32 levelStart,
        /* [in] */ ArrayOf<IInterface*>* objects,
        /* [in] */ Int32 objectStart,
        /* [in] */ Int32 count);

    /**
     * Indicates whether a range of characters of a text requires a {@code Bidi}
     * object to display properly.
     *
     * @param text
     *            the char array of the text.
     * @param start
     *            the start offset of the range of characters.
     * @param limit
     *            the limit offset of the range of characters.
     * @return {@code true} if the range of characters requires a {@code Bidi}
     *         object; {@code false} otherwise.
     * @throws IllegalArgumentException
     *             if {@code start} or {@code limit} is negative; {@code start >
     *             limit} or {@code limit} is greater than the length of this
     *             object's paragraph text.
     */
    static CARAPI RequiresBidi(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [out] */ Boolean * result);

    /**
     * Returns the internal message of the {@code Bidi} object, used in
     * debugging.
     *
     * @return a string containing the internal message.
     */
    //@Override
    //public String toString() {
    //    return getClass().getName()
    //            + "[direction: " + direction + " baseLevel: " + baseLevel
    //            + " length: " + length + " runs: " + Arrays.toString(runs) + "]";
    //}

private:
    // create the native UBiDi struct, need to be closed with ubidi_close().
    static CARAPI_(Int64) CreateUBiDi(
        /* [in] */ ArrayOf<Char32>* texts,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte>* embeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags);

    /* private constructor used by createLineBidi() */
    Bidi(
        /* [in] */ Int64 pBidi);

    // read info from the native UBiDi struct
    CARAPI_(void) ReadBidiInfo(
        /* [in] */ Int64 pBidi);

    CARAPI CreateEmptyLineBidi(
        /* [in] */ Int64 parent,
        /* [out] */ IBidi** lineBidi);

private:
    Int32 mBaseLevel;

    Int32 mLength;

    ArrayOf<Byte> * mOffsetLevel;

    ArrayOf<IBidiRun * > * mRuns;

    Int32 mDirection;

    Boolean mUnidirectional;
};
#endif //__BIDI_H__