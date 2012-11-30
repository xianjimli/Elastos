#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include "ext/frameworkext.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include "emoji/EmojiFactory.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

/**
 * A base class that manages text layout in visual elements on
 * the screen.
 * <p>For text that will be edited, use a {@link DynamicLayout},
 * which will be updated as the text changes.
 * For text that will not change, use a {@link StaticLayout}.
 */
class Layout
{
public:
    /* package */
    class Ellipsizer
        : public ElRefBase
        , public IGetChars
    {
    public:
        Ellipsizer(
            /* [in] */ ICharSequence* s);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetCharAt(
            /* [in] */ Int32 off,
            /* [out] */ Char32* ch);

        CARAPI GetChars(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ArrayOf<Char8>* dest,
            /* [in] */ Int32 destoff);

        CARAPI GetLength(
            /* [out] */ Int32* len);

        CARAPI SubSequence(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ICharSequence** subcsq);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        /* package */ AutoPtr<ICharSequence> mText;
        /* package */ Layout* mLayout;
        /* package */ Int32 mWidth;
        /* package */ TextUtilsTruncateAt mMethod;
    };

    /* package */
    class SpannedEllipsizer
        : public Ellipsizer
        , public ISpanned
    {
    public:
        SpannedEllipsizer(
            /* [in] */ ICharSequence* display);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetChars(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ArrayOf<Char8>* dest,
            /* [in] */ Int32 destoff);

        CARAPI GetLength(
            /* [out] */ Int32* len);

        CARAPI GetCharAt(
            /* [in] */ Int32 off,
            /* [out] */ Char32* ch);

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI GetSpans(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ const InterfaceID& type,
            /* [out, callee] */ ArrayOf<IInterface*>** objs);

        CARAPI GetSpanStart(
            /* [in] */ IInterface* tag,
            /* [out] */ Int32* start);

        CARAPI GetSpanEnd(
            /* [in] */ IInterface* tag,
            /* [out] */ Int32* end);

        CARAPI GetSpanFlags(
            /* [in] */ IInterface* tag,
            /* [out] */ Int32* flags);

        CARAPI NextSpanTransition(
            /* [in] */ Int32 start,
            /* [in] */ Int32 limit,
            /* [in] */ const InterfaceID& type,
            /* [out] */ Int32* offset);

        CARAPI SubSequence(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ ICharSequence** subcsq);

    private:
        AutoPtr<ISpanned> mSpanned;
    };

public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Return how wide a layout must be in order to display the
     * specified text with one line per paragraph.
     */
    static CARAPI_(Float) GetDesiredWidth(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint);

    /**
     * Return how wide a layout must be in order to display the
     * specified text slice with one line per paragraph.
     */
    static CARAPI_(Float) GetDesiredWidth(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ITextPaint* paint);

    /**
     * Replace constructor properties of this Layout with new ones.  Be careful.
     */
    virtual CARAPI ReplaceWith(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd);

    /**
     * Draw this Layout on the specified ICanvas*.
     */
    virtual CARAPI Draw(
        /* [in] */ ICanvas* c);

    /**
     * Draw this Layout on the specified canvas, with the highlight path drawn
     * between the background and the text.
     *
     * @param c the canvas
     * @param highlight the path of the highlight or cursor; can be null
     * @param highlightPaint the paint for the highlight
     * @param cursorOffsetVertical the amount to temporarily translate the
     *        canvas while rendering the highlight
     */
    virtual CARAPI Draw(
        /* [in] */ ICanvas* c,
        /* [in] */ IPath* highlight,
        /* [in] */ IPaint* highlightPaint,
        /* [in] */ Int32 cursorOffsetVertical);

    /**
     * Return the text that is displayed by this Layout.
     */
    CARAPI_(AutoPtr<ICharSequence>) GetText();

    /**
     * Return the base Paint properties for this layout.
     * Do NOT change the paint, which may result in funny
     * drawing for this layout.
     */
    CARAPI_(AutoPtr<ITextPaint>) GetPaint();

    /**
     * Return the width of this layout.
     */
    CARAPI_(Int32) GetWidth();

    /**
     * Return the width to which this Layout is ellipsizing, or
     * {@link #getWidth} if it is not doing anything special.
     */
    virtual CARAPI_(Int32) GetEllipsizedWidth();

    /**
     * Increase the width of this layout to the specified width.
     * Be careful to use this only when you know it is appropriate&mdash;
     * it does not cause the text to reflow to use the full new width.
     */
    CARAPI IncreaseWidthTo(Int32 wid);

    /**
    * Return the total height of this layout.
    */
    virtual CARAPI_(Int32) GetHeight();

    /**
    * Return the base alignment of this layout.
    */
    CARAPI_(LayoutAlignment) GetAlignment();

    /**
     * Return what the text height is multiplied by to get the line height.
     */
    CARAPI_(Float) GetSpacingMultiplier();

    /**
     * Return the number of units of leading that are added to each line.
     */
    CARAPI_(Float) GetSpacingAdd();

    /**
     * Return the number of lines of text in this layout.
     */
    virtual CARAPI_(Int32) GetLineCount() = 0;

    /**
     * Return the baseline for the specified line (0&hellip;getLineCount() - 1)
     * If bounds is not null, return the top, left, right, bottom extents
     * of the specified line in it.
     * @param line which line to examine (0..getLineCount() - 1)
     * @param bounds Optional. If not null, it returns the extent of the line
     * @return the Y-coordinate of the baseline
     */
    virtual CARAPI_(Int32) GetLineBounds(
        /* [in] */ Int32 line,
        /* [in] */ IRect* bounds);

    /**
     * Return the vertical position of the top of the specified line
     * (0&hellip;getLineCount()).
     * If the specified line is equal to the line count, returns the
     * bottom of the last line.
     */
    virtual CARAPI_(Int32) GetLineTop(
        /* [in] */ Int32 line) = 0;

    /**
     * Return the descent of the specified line(0&hellip;getLineCount() - 1).
     */
    virtual CARAPI_(Int32) GetLineDescent(
        /* [in] */ Int32 line) = 0;

    /**
     * Return the text offset of the beginning of the specified line (
     * 0&hellip;getLineCount()). If the specified line is equal to the line
     * count, returns the length of the text.
     */
    virtual CARAPI_(Int32) GetLineStart(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns the primary directionality of the paragraph containing the
     * specified line, either 1 for left-to-right lines, or -1 for right-to-left
     * lines (see {@link #DIR_LEFT_TO_RIGHT}, {@link #DIR_RIGHT_TO_LEFT}).
     */
    virtual CARAPI_(Int32) GetParagraphDirection(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns whether the specified line contains one or more
     * characters that need to be handled specially, like tabs
     * or emoji.
     */
    virtual CARAPI_(Boolean) GetLineContainsTab(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns the directional run information for the specified line.
     * The array alternates counts of characters in left-to-right
     * and right-to-left segments of the line.
     *
     * <p>NOTE: this is inadequate to support bidirectional text, and will change.
     */
    virtual CARAPI_(AutoPtr<IDirections>) GetLineDirections(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns the (negative) number of extra pixels of ascent padding in the
     * top line of the Layout.
     */

    virtual CARAPI_(Int32) GetTopPadding() = 0;

    /**
     * Returns the number of extra pixels of descent padding in the
     * bottom line of the Layout.
     */
    virtual CARAPI_(Int32) GetBottomPadding() = 0;

    /**
     * Get the primary horizontal position for the specified text offset.
     * This is the location where a new character would be inserted in
     * the paragraph's primary direction.
     */
    virtual CARAPI_(Float) GetPrimaryHorizontal(
        /* [in] */ Int32 offset);

    /**
     * Get the secondary horizontal position for the specified text offset.
     * This is the location where a new character would be inserted in
     * the direction other than the paragraph's primary direction.
     */
    virtual CARAPI_(Float) GetSecondaryHorizontal(
        /* [in] */ Int32 offset);

    /**
     * Get the leftmost position that should be exposed for horizontal
     * scrolling on the specified line.
     */
    virtual CARAPI_(Float) GetLineLeft(
        /* [in] */ Int32 line);

    /**
     * Get the rightmost position that should be exposed for horizontal
     * scrolling on the specified line.
     */
    virtual CARAPI_(Float) GetLineRight(
        /* [in] */ Int32 line);

    /**
     * Gets the horizontal extent of the specified line, excluding
     * trailing whitespace.
     */
    virtual CARAPI_(Float) GetLineMax(
        /* [in] */ Int32 line);

    /**
     * Gets the horizontal extent of the specified line, including
     * trailing whitespace.
     */
    virtual CARAPI_(Float) GetLineWidth(
        /* [in] */ Int32 line);

    /**
     * Get the line number corresponding to the specified vertical position.
     * If you ask for a position above 0, you get 0; if you ask for a position
     * below the bottom of the text, you get the last line.
     */
    // FIXME: It may be faster to do a linear search for layouts without many lines.
    virtual CARAPI_(Int32) GetLineForVertical(
        /* [in] */ Int32 vertical);

    /**
     * Get the line number on which the specified text offset appears.
     * If you ask for a position before 0, you get 0; if you ask for a position
     * beyond the end of the text, you get the last line.
     */
    virtual CARAPI_(Int32) GetLineForOffset(
        /* [in] */ Int32 offset);

    /**
     * Get the character offset on the specfied line whose position is
     * closest to the specified horizontal position.
     */
    virtual CARAPI_(Int32) GetOffsetForHorizontal(
        /* [in] */ Int32 line,
        /* [in] */ Float horiz);

    /**
     * Return the text offset after the last character on the specified line.
     */
    CARAPI_(Int32) GetLineEnd(
        /* [in] */ Int32 line);

    /**
     * Return the text offset after the last visible character (so whitespace
     * is not counted) on the specified line.
     */
    virtual CARAPI_(Int32) GetLineVisibleEnd(
        /* [in] */ Int32 line);

    /**
     * Return the vertical position of the bottom of the specified line.
     */
    CARAPI_(Int32) GetLineBottom(
        /* [in] */ Int32 line);

    /**
     * Return the vertical position of the baseline of the specified line.
     */
    CARAPI_(Int32) GetLineBaseline(
        /* [in] */ Int32 line);

    /**
     * Get the ascent of the text on the specified line.
     * The return value is negative to match the Paint.ascent() convention.
     */
    CARAPI_(Int32) GetLineAscent(
        /* [in] */ Int32 line);

    /**
     * Return the text offset that would be reached by moving left
     * (possibly onto another line) from the specified offset.
     */
    virtual CARAPI_(Int32) GetOffsetToLeftOf(
        /* [in] */ Int32 offset);

    /**
     * Return the text offset that would be reached by moving right
     * (possibly onto another line) from the specified offset.
     */
    virtual CARAPI_(Int32) GetOffsetToRightOf(
        /* [in] */ Int32 offset);

    /**
     * Fills in the specified Path with a representation of a cursor
     * at the specified offset.  This will often be a vertical line
     * but can be multiple discontinous lines in text with multiple
     * directionalities.
     */
    virtual CARAPI GetCursorPath(
        /* [in] */ Int32 point,
        /* [in] */ IPath* dest,
        /* [in] */ ICharSequence* editingBuffer);

    /**
     * Fills in the specified Path with a representation of a highlight
     * between the specified offsets.  This will often be a rectangle
     * or a potentially discontinuous set of rectangles.  If the start
     * and end are the same, the returned path is empty.
     */
    virtual CARAPI GetSelectionPath(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPath* dest);

    /**
     * Get the alignment of the specified paragraph, taking into account
     * markup attached to it.
     */
    CARAPI_(LayoutAlignment) GetParagraphAlignment(
        /* [in] */ Int32 line);

    /**
     * Get the left edge of the specified paragraph, inset by left margins.
     */
    CARAPI_(Int32) GetParagraphLeft(
        /* [in] */ Int32 line);

    /**
     * Get the right edge of the specified paragraph, inset by right margins.
     */
    CARAPI_(Int32) GetParagraphRight(
        /* [in] */ Int32 line);

    /**
     * Return the offset of the first character to be ellipsized away,
     * relative to the start of the line.  (So 0 if the beginning of the
     * line is ellipsized, not getLineStart().)
     */
    virtual CARAPI_(Int32) GetEllipsisStart(
        /* [in] */ Int32 line) = 0;

    /**
     * Returns the number of characters to be ellipsized away, or 0 if
     * no ellipsis is to take place.
     */
    virtual CARAPI_(Int32) GetEllipsisCount(
        /* [in] */ Int32 line) = 0;

    /**
     * Measure width of a run of text on a single line that is known to all be
     * in the same direction as the paragraph base direction. Returns the width,
     * and the line metrics in fm if fm is not NULL.
     *
     * @param paint the paint for the text; will not be modified
     * @param workPaint paint available for modification
     * @param text text
     * @param start start of the line
     * @param end limit of the line
     * @param fm object to return integer metrics in, can be NULL
     * @param hasTabs TRUE if it is known that the line has tabs
     * @param tabs tab position information
     * @return the width of the text from start to end
     */
    static CARAPI_(Float) MeasureText(
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaintFontMetricsInt* fm,
        /* [in] */ Boolean hasTabs,
        /* [in] */ ArrayOf<IInterface*>* tabs);

    /**
     * Returns the position of the next tab stop after h on the line.
     *
     * @param text the text
     * @param start start of the line
     * @param end limit of the line
     * @param h the current horizontal offset
     * @param tabs the tabs, can be NULL.  If it is NULL, any tabs in effect
     * on the line will be used.  If there are no tabs, a default offset
     * will be used to compute the tab stop.
     * @return the offset of the next tab stop.
     */
    static CARAPI_(Float) NextTab(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float h,
        /* [in] */ ArrayOf<IInterface*>* tabs);

protected:
    Layout();

    /**
     * Subclasses of Layout use this constructor to set the display text,
     * width, and other standard properties.
     * @param text the text to render
     * @param paint the default paint for the layout.  Styles can override
     * various attributes of the paint.
     * @param width the wrapping width for the text.
     * @param align whether to left, right, or center the text.  Styles can
     * override the alignment.
     * @param spacingMult factor by which to scale the font size to get the
     * default line spacing
     * @param spacingAdd amount to add to the default line spacing
     */
    Layout(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingMult,
        /* [in] */ Float spacingAdd);

    CARAPI_(Boolean) IsSpanned();

    CARAPI Init(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingMult,
        /* [in] */ Float spacingAdd);

private:
    CARAPI_(Float) GetHorizontal(
        /* [in] */ Int32 offset,
        /* [in] */ Boolean trailing,
        /* [in] */ Boolean alt);

    CARAPI_(Float) GetHorizontal(
        /* [in] */ Int32 offset,
        /* [in] */ Boolean trailing,
        /* [in] */ Boolean alt,
        /* [in] */ Int32 line);

    CARAPI_(Float) GetLineMax(
        /* [in] */ Int32 line,
        /* [in] */ ArrayOf<IInterface*>* tabs,
        /* [in] */ Boolean full);

    CARAPI_(Int32) GetLineVisibleEnd(
        /* [in] */ Int32 line,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Int32) GetOffsetAtStartOf(
        /* [in] */ Int32 offset);

    CARAPI_(void) AddSelection(
        /* [in] */ Int32 line,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 top,
        /* [in] */ Int32 bottom,
        /* [in] */ IPath* dest);

    CARAPI_(void) DrawText(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 dir,
        /* [in] */ IDirections* directions,
        /* [in] */ Float x,
        /* [in] */ Int32 top,
        /* [in] */ Int32 y,
        /* [in] */ Int32 bottom,
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ Boolean hasTabs,
        /* [in] */ ArrayOf<IInterface*>* parspans);

    static CARAPI_(Float) MeasureText(
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 end,
        /* [in] */ Int32 dir,
        /* [in] */ IDirections* directions,
        /* [in] */ Boolean trailing,
        /* [in] */ Boolean alt,
        /* [in] */ Boolean hasTabs,
        /* [in] */ ArrayOf<IInterface*>* tabs);

    CARAPI_(void) Ellipsize(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 line,
        /* [out] */ ArrayOf<Char8>* dest,
        /* [in] */ Int32 destoff);

public:
    /* package */ static EmojiFactory* EMOJI_FACTORY;
    /* package */ static UInt32 MIN_EMOJI, MAX_EMOJI;

    static const Int32 DIR_LEFT_TO_RIGHT = 1;
    static const Int32 DIR_RIGHT_TO_LEFT = -1;

    /* package */ static const Int32 DIR_REQUEST_LTR = 1;
    /* package */ static const Int32 DIR_REQUEST_RTL = -1;
    /* package */ static const Int32 DIR_REQUEST_DEFAULT_LTR = 2;
    /* package */ static const Int32 DIR_REQUEST_DEFAULT_RTL = -2;

    /* package */ static AutoPtr<IDirections> DIRS_ALL_LEFT_TO_RIGHT;
    /* package */ static AutoPtr<IDirections> DIRS_ALL_RIGHT_TO_LEFT;

public:
    /* package */ AutoPtr<ITextPaint> mWorkPaint;

private:
    static const Boolean DEBUG = FALSE;
    //static const IParagraphStyle*[] NO_PARA_SPANS =
    //    ArrayUtils.emptyArray(ParagraphStyle.class);
    static ArrayOf<IParagraphStyle*>* NO_PARA_SPANS;

    AutoPtr<CRectF> mEmojiRect;

    AutoPtr<ICharSequence> mText;
    AutoPtr<ITextPaint> mPaint;

    Int32 mWidth;
    LayoutAlignment mAlignment;// = LayoutAlignment.ALIGN_NORMAL;
    Float mSpacingMult;
    Float mSpacingAdd;
    static AutoPtr<CRect> sTempRect;
    static Mutex sTempRectLock;
    Boolean mSpannedText;

    static const Int32 TAB_INCREMENT = 20;
};

#endif //__LAYOUT_H__
