
#ifndef __STYLED_H__
#define __STYLED_H__

#include "ext/frameworkext.h"

/**
 * This class provides static methods for drawing and measuring styled text,
 * like {@link android.text.Spanned} object with
 * {@link android.text.style.ReplacementSpan}.
 *
 * @hide
 */
class Styled
{
public:
    /**
     * Returns the advance widths for a uniform left-to-right run of text with
     * no style changes in the middle of the run. If any style is replacement
     * text, the first character will get the width of the replacement and the
     * remaining characters will get a width of 0.
     *
     * @param paint the paint, will not be modified
     * @param workPaint a paint to modify; on return will reflect the original
     *        paint plus the effect of all spans on the run
     * @param text the text
     * @param start the start of the run
     * @param end the limit of the run
     * @param widths array to receive the advance widths of the characters. Must
     *        be at least a large as (end - start).
     * @param fmi FontMetrics information; can be null
     * @return the actual number of widths returned
     */
    static CARAPI_(Int32) GetTextWidths(
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ ISpanned* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Float>* widths,
        /* [in] */ IPaintFontMetricsInt* fmi);

    /**
     * Draws a unidirectional run of text on a single line, and optionally
     * returns the signed advance.  Unlike drawDirectionalRun, the paragraph
     * direction and run direction can be different.
     */
    /* package */
    static CARAPI_(Float) DrawText(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 dir,
        /* [in] */ Boolean runIsRtl,
        /* [in] */ Float x,
        /* [in] */ Int32 top,
        /* [in] */ Int32 y,
        /* [in] */ Int32 bottom,
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ Boolean needWidth);

    /**
     * Draws a run of text on a single line, with its
     * origin at (x,y), in the specified Paint. The origin is interpreted based
     * on the Align setting in the Paint.
     *
     * This method considers style information in the text (e.g. even when text
     * is an instance of {@link android.text.Spanned}, this method correctly
     * draws the text). See also
     * {@link android.graphics.Canvas#drawText(CharSequence, int, int, float,
     * float, Paint)} and
     * {@link android.graphics.Canvas#drawRect(float, float, float, float,
     * Paint)}.
     *
     * @param canvas The target canvas
     * @param text The text to be drawn
     * @param start The index of the first character in text to draw
     * @param end (end - 1) is the index of the last character in text to draw
     * @param direction The direction of the text. This must be
     *        {@link android.text.Layout#DIR_LEFT_TO_RIGHT} or
     *        {@link android.text.Layout#DIR_RIGHT_TO_LEFT}.
     * @param x The x-coordinate of origin for where to draw the text
     * @param top The top side of the rectangle to be drawn
     * @param y The y-coordinate of origin for where to draw the text
     * @param bottom The bottom side of the rectangle to be drawn
     * @param paint The main {@link TextPaint} object.
     * @param workPaint The {@link TextPaint} object used for temporal
     *        workspace.
     * @param needWidth If true, this method returns the width of drawn text
     * @return Width of the drawn text if needWidth is true
     */
    static CARAPI_(Float) DrawText(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 direction,
        /* [in] */ Float x,
        /* [in] */ Int32 top,
        /* [in] */ Int32 y,
        /* [in] */ Int32 bottom,
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ Boolean needWidth);

    /**
     * Returns the width of a run of left-to-right text on a single line,
     * considering style information in the text (e.g. even when text is an
     * instance of {@link android.text.Spanned}, this method correctly measures
     * the width of the text).
     *
     * @param paint the main {@link TextPaint} object; will not be modified
     * @param workPaint the {@link TextPaint} object available for modification;
     *        will not necessarily be used
     * @param text the text to measure
     * @param start the index of the first character to start measuring
     * @param end 1 beyond the index of the last character to measure
     * @param fmi FontMetrics information; can be null
     * @return The width of the text
     */
    static CARAPI_(Float) MeasureText(
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaintFontMetricsInt* fmi);

private:
    /**
     * Draws and/or measures a uniform run of text on a single line. No span of
     * interest should start or end in the middle of this run (if not
     * drawing, character spans that don't affect metrics can be ignored).
     * Neither should the run direction change in the middle of the run.
     *
     * <p>The x position is the leading edge of the text. In a right-to-left
     * paragraph, this will be to the right of the text to be drawn. Paint
     * should not have an Align value other than LEFT or positioning will get
     * confused.
     *
     * <p>On return, workPaint will reflect the original paint plus any
     * modifications made by character styles on the run.
     *
     * <p>The returned width is signed and will be < 0 if the paragraph
     * direction is right-to-left.
     */
    static CARAPI_(Float) DrawUniformRun(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ISpanned* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 dir,
        /* [in] */ Boolean runIsRtl,
        /* [in] */ Float x,
        /* [in] */ Int32 top,
        /* [in] */ Int32 y,
        /* [in] */ Int32 bottom,
        /* [in] */ IPaintFontMetricsInt* fmi,
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ Boolean needWidth);

    /**
     * Renders and/or measures a directional run of text on a single line.
     * Unlike {@link #drawUniformRun}, this can render runs that cross style
     * boundaries.  Returns the signed advance width, if requested.
     *
     * <p>The x position is the leading edge of the text. In a right-to-left
     * paragraph, this will be to the right of the text to be drawn. Paint
     * should not have an Align value other than LEFT or positioning will get
     * confused.
     *
     * <p>This optimizes for unstyled text and so workPaint might not be
     * modified by this call.
     *
     * <p>The returned advance width will be < 0 if the paragraph
     * direction is right-to-left.
     */
    static CARAPI_(Float) DrawDirectionalRun(
        /* [in] */ ICanvas* canvas,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 dir,
        /* [in] */ Boolean runIsRtl,
        /* [in] */ Float x,
        /* [in] */ Int32 top,
        /* [in] */ Int32 y,
        /* [in] */ Int32 bottom,
        /* [in] */ IPaintFontMetricsInt* fmi,
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ Boolean needWidth);
};

#endif //__STYLED_H__
