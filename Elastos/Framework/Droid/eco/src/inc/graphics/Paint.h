
#ifndef __PAINT_H__
#define __PAINT_H__

#include "ext/frameworkext.h"
#include <skia/core/SkPaint.h>
#include <elastos/AutoPtr.h>

extern "C" const InterfaceID EIID_Paint;

class Paint
{
    friend class Canvas;

public:
    virtual ~Paint();

    /** Restores the paint to its default settings. */
    virtual CARAPI Reset();

    /**
     * Copy the fields from src into this paint. This is equivalent to calling
     * get() on all of the src fields, and calling the corresponding set()
     * methods on this.
     */
    virtual CARAPI Set(
        /* [in] */ IPaint* src);

    /** @hide */
    virtual CARAPI SetCompatibilityScaling(
        /* [in] */ Float factor);

     /**
     * Return the paint's flags. Use the Flag enum to test flag values.
     *
     * @return the paint's flags (see enums ending in _Flag for bit masks)
     */
    virtual CARAPI_(Int32) GetFlags();

    virtual CARAPI SetFlags(
        /* [in] */ Int32 flags);

    /**
     * Helper for getFlags(), returning true if ANTI_ALIAS_FLAG bit is set
     * AntiAliasing smooths out the edges of what is being drawn, but is has
     * no impact on the interior of the shape. See setDither() and
     * setFilterBitmap() to affect how colors are treated.
     *
     * @return true if the antialias bit is set in the paint's flags.
     */
    CARAPI_(Boolean) IsAntiAlias();

    /**
     * Helper for setFlags(), setting or clearing the ANTI_ALIAS_FLAG bit
     * AntiAliasing smooths out the edges of what is being drawn, but is has
     * no impact on the interior of the shape. See setDither() and
     * setFilterBitmap() to affect how colors are treated.
     *
     * @param aa true to set the antialias bit in the flags, false to clear it
     */
    virtual CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    /**
     * Helper for getFlags(), returning true if DITHER_FLAG bit is set
     * Dithering affects how colors that are higher precision than the device
     * are down-sampled. No dithering is generally faster, but higher precision
     * colors are just truncated down (e.g. 8888 -> 565). Dithering tries to
     * distribute the error inherent in this process, to reduce the visual
     * artifacts.
     *
     * @return true if the dithering bit is set in the paint's flags.
     */
    CARAPI_(Boolean) IsDither();

    /**
     * Helper for setFlags(), setting or clearing the DITHER_FLAG bit
     * Dithering affects how colors that are higher precision than the device
     * are down-sampled. No dithering is generally faster, but higher precision
     * colors are just truncated down (e.g. 8888 -> 565). Dithering tries to
     * distribute the error inherent in this process, to reduce the visual
     * artifacts.
     *
     * @param dither true to set the dithering bit in flags, false to clear it
     */
    virtual CARAPI SetDither(
        /* [in] */ Boolean dither);

    /**
     * Helper for getFlags(), returning true if LINEAR_TEXT_FLAG bit is set
     *
     * @return true if the lineartext bit is set in the paint's flags
     */
    CARAPI_(Boolean) IsLinearText();

    /**
     * Helper for setFlags(), setting or clearing the LINEAR_TEXT_FLAG bit
     *
     * @param linearText true to set the linearText bit in the paint's flags,
     *                   false to clear it.
     */
    virtual CARAPI SetLinearText(
        /* [in] */ Boolean linearText);

    /**
     * Helper for getFlags(), returning true if SUBPIXEL_TEXT_FLAG bit is set
     *
     * @return true if the subpixel bit is set in the paint's flags
     */
    CARAPI_(Boolean) IsSubpixelText();

    /**
     * Helper for setFlags(), setting or clearing the SUBPIXEL_TEXT_FLAG bit
     *
     * @param subpixelText true to set the subpixelText bit in the paint's
     *                     flags, false to clear it.
     */
    virtual CARAPI SetSubpixelText(
        /* [in] */ Boolean subpixelText);

    /**
     * Helper for getFlags(), returning true if UNDERLINE_TEXT_FLAG bit is set
     *
     * @return true if the underlineText bit is set in the paint's flags.
     */
    CARAPI_(Boolean) IsUnderlineText();

    /**
     * Helper for setFlags(), setting or clearing the UNDERLINE_TEXT_FLAG bit
     *
     * @param underlineText true to set the underlineText bit in the paint's
     *                      flags, false to clear it.
     */
    virtual CARAPI SetUnderlineText(
        /* [in] */ Boolean underlineText);

    /**
     * Helper for getFlags(), returning true if STRIKE_THRU_TEXT_FLAG bit is set
     *
     * @return true if the strikeThruText bit is set in the paint's flags.
     */
    CARAPI_(Boolean) IsStrikeThruText();

    /**
     * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
     *
     * @param strikeThruText true to set the strikeThruText bit in the paint's
     *                       flags, false to clear it.
     */
    virtual CARAPI SetStrikeThruText(
        /* [in] */ Boolean strikeThruText);

    /**
     * Helper for getFlags(), returning true if FAKE_BOLD_TEXT_FLAG bit is set
     *
     * @return true if the fakeBoldText bit is set in the paint's flags.
     */
    CARAPI_(Boolean) IsFakeBoldText();

    /**
     * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
     *
     * @param fakeBoldText true to set the fakeBoldText bit in the paint's
     *                     flags, false to clear it.
     */
    virtual CARAPI SetFakeBoldText(
        /* [in] */ Boolean fakeBoldText);

    /**
     * Whether or not the bitmap filter is activated.
     * Filtering affects the sampling of bitmaps when they are transformed.
     * Filtering does not affect how the colors in the bitmap are converted into
     * device pixels. That is dependent on dithering and xfermodes.
     *
     * @see #setFilterBitmap(boolean) setFilterBitmap()
     */
    CARAPI_(Boolean) IsFilterBitmap();

    /**
     * Helper for setFlags(), setting or clearing the FILTER_BITMAP_FLAG bit.
     * Filtering affects the sampling of bitmaps when they are transformed.
     * Filtering does not affect how the colors in the bitmap are converted into
     * device pixels. That is dependent on dithering and xfermodes.
     *
     * @param filter true to set the FILTER_BITMAP_FLAG bit in the paint's
     *               flags, false to clear it.
     */
    virtual CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    /**
     * Return the paint's style, used for controlling how primitives'
     * geometries are interpreted (except for drawBitmap, which always assumes
     * FILL_STYLE).
     *
     * @return the paint's style setting (Fill, Stroke, StrokeAndFill)
     */
    virtual CARAPI_(PaintStyle) GetStyle();

    /**
     * Set the paint's style, used for controlling how primitives'
     * geometries are interpreted (except for drawBitmap, which always assumes
     * Fill).
     *
     * @param style The new style to set in the paint
     */
    virtual CARAPI SetStyle(
        /* [in] */ PaintStyle style);

    /**
     * Return the paint's color. Note that the color is a 32bit value
     * containing alpha as well as r,g,b. This 32bit value is not premultiplied,
     * meaning that its alpha can be any value, regardless of the values of
     * r,g,b. See the Color class for more details.
     *
     * @return the paint's color (and alpha).
     */
    virtual CARAPI_(Int32) GetColor();

    virtual CARAPI SetColor(
        /* [in] */ Int32 color);

    /**
     * Helper to getColor() that just returns the color's alpha value. This is
     * the same as calling getColor() >>> 24. It always returns a value between
     * 0 (completely transparent) and 255 (completely opaque).
     *
     * @return the alpha component of the paint's color.
     */
    virtual CARAPI_(Int32) GetAlpha();

    /**
     * Helper to setColor(), that only assigns the color's alpha value,
     * leaving its r,g,b values unchanged. Results are undefined if the alpha
     * value is outside of the range [0..255]
     *
     * @param a set the alpha component [0..255] of the paint's color.
     */
    virtual CARAPI SetAlpha(
        /* [in] */ Int32 a);

    /**
     * Helper to setColor(), that takes a,r,g,b and constructs the color int
     *
     * @param a The new alpha component (0..255) of the paint's color.
     * @param r The new red component (0..255) of the paint's color.
     * @param g The new green component (0..255) of the paint's color.
     * @param b The new blue component (0..255) of the paint's color.
     */
    virtual CARAPI SetARGB(
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    /**
     * Return the width for stroking.
     * <p />
     * A value of 0 strokes in hairline mode.
     * Hairlines always draws a single pixel independent of the canva's matrix.
     *
     * @return the paint's stroke width, used whenever the paint's style is
     *         Stroke or StrokeAndFill.
     */
    virtual CARAPI_(Float) GetStrokeWidth();

    /**
     * Set the width for stroking.
     * Pass 0 to stroke in hairline mode.
     * Hairlines always draws a single pixel independent of the canva's matrix.
     *
     * @param width set the paint's stroke width, used whenever the paint's
     *              style is Stroke or StrokeAndFill.
     */
    virtual CARAPI SetStrokeWidth(
        /* [in] */ Float width);

    /**
     * Return the paint's stroke miter value. Used to control the behavior
     * of miter joins when the joins angle is sharp.
     *
     * @return the paint's miter limit, used whenever the paint's style is
     *         Stroke or StrokeAndFill.
     */
    virtual CARAPI_(Float) GetStrokeMiter();

    /**
     * Set the paint's stroke miter value. This is used to control the behavior
     * of miter joins when the joins angle is sharp. This value must be >= 0.
     *
     * @param miter set the miter limit on the paint, used whenever the paint's
     *              style is Stroke or StrokeAndFill.
     */
    virtual CARAPI SetStrokeMiter(
        /* [in] */ Float miter);

    /**
     * Return the paint's Cap, controlling how the start and end of stroked
     * lines and paths are treated.
     *
     * @return the line cap style for the paint, used whenever the paint's
     *         style is Stroke or StrokeAndFill.
     */
    virtual CARAPI_(PaintCap) GetStrokeCap();

    /**
     * Set the paint's Cap.
     *
     * @param cap set the paint's line cap style, used whenever the paint's
     *            style is Stroke or StrokeAndFill.
     */
    virtual CARAPI SetStrokeCap(
        /* [in] */ PaintCap cap);

    /**
     * Return the paint's stroke join type.
     *
     * @return the paint's Join.
     */
    virtual CARAPI_(PaintJoin) GetStrokeJoin();

    /**
     * Set the paint's Join.
     *
     * @param join set the paint's Join, used whenever the paint's style is
     *             Stroke or StrokeAndFill.
     */
    virtual CARAPI SetStrokeJoin(
        /* [in] */ PaintJoin join);

    /**
     * Applies any/all effects (patheffect, stroking) to src, returning the
     * result in dst. The result is that drawing src with this paint will be
     * the same as drawing dst with a default paint (at least from the
     * geometric perspective).
     *
     * @param src input path
     * @param dst output path (may be the same as src)
     * @return    true if the path should be filled, or false if it should be
     *                 drawn with a hairline (width == 0)
     */
    virtual CARAPI_(Boolean) GetFillPath(
        /* [in] */ IPath* src,
        /* [in] */ IPath* dst);

    /**
     * Get the paint's shader object.
     *
     * @return the paint's shader (or null)
     */
    virtual CARAPI_(AutoPtr<IShader>) GetShader();

    /**
     * Set or clear the shader object.
     * <p />
     * Pass null to clear any previous shader.
     * As a convenience, the parameter passed is also returned.
     *
     * @param shader May be null. the new shader to be installed in the paint
     * @return       shader
     */
    virtual CARAPI SetShader(
        /* [in] */ IShader* shader);

     /**
     * Get the paint's colorfilter (maybe be null).
     *
     * @return the paint's colorfilter (maybe be null)
     */
    virtual CARAPI_(AutoPtr<IColorFilter>) GetColorFilter();

    /**
     * Set or clear the paint's colorfilter, returning the parameter.
     *
     * @param filter May be null. The new filter to be installed in the paint
     * @return       filter
     */
    virtual CARAPI SetColorFilter(
        /* [in] */ IColorFilter* filter);

    /**
     * Get the paint's xfermode object.
     *
     * @return the paint's xfermode (or null)
     */
    virtual CARAPI_(AutoPtr<IXfermode>) GetXfermode();

    /**
     * Set or clear the xfermode object.
     * <p />
     * Pass null to clear any previous xfermode.
     * As a convenience, the parameter passed is also returned.
     *
     * @param xfermode May be null. The xfermode to be installed in the paint
     * @return         xfermode
     */
    virtual CARAPI SetXfermode(
        /* [in] */ IXfermode* xfermode);

    /**
     * Get the paint's patheffect object.
     *
     * @return the paint's patheffect (or null)
     */
    virtual CARAPI_(AutoPtr<IPathEffect>) GetPathEffect();

    /**
     * Set or clear the patheffect object.
     * <p />
     * Pass null to clear any previous patheffect.
     * As a convenience, the parameter passed is also returned.
     *
     * @param effect May be null. The patheffect to be installed in the paint
     * @return       effect
     */
    virtual CARAPI SetPathEffect(
        /* [in] */ IPathEffect* effect);

    /**
     * Get the paint's maskfilter object.
     *
     * @return the paint's maskfilter (or null)
     */
    virtual CARAPI_(AutoPtr<IMaskFilter>) GetMaskFilter();

    /**
     * Set or clear the maskfilter object.
     * <p />
     * Pass null to clear any previous maskfilter.
     * As a convenience, the parameter passed is also returned.
     *
     * @param maskfilter May be null. The maskfilter to be installed in the
     *                   paint
     * @return           maskfilter
     */
    virtual CARAPI SetMaskFilter(
        /* [in] */ IMaskFilter* maskfilter);

    /**
     * Get the paint's typeface object.
     * <p />
     * The typeface object identifies which font to use when drawing or
     * measuring text.
     *
     * @return the paint's typeface (or null)
     */
    virtual CARAPI_(AutoPtr<ITypeface>) GetTypeface();

    /**
     * Set or clear the typeface object.
     * <p />
     * Pass null to clear any previous typeface.
     * As a convenience, the parameter passed is also returned.
     *
     * @param typeface May be null. The typeface to be installed in the paint
     * @return         typeface
     */
    virtual CARAPI SetTypeface(
        /* [in] */ ITypeface* typeface);

    /**
     * Get the paint's rasterizer (or null).
     * <p />
     * The raster controls/modifies how paths/text are turned into alpha masks.
     *
     * @return         the paint's rasterizer (or null)
     */
    virtual CARAPI_(AutoPtr<IRasterizer>) GetRasterizer();

    /**
     * Set or clear the rasterizer object.
     * <p />
     * Pass null to clear any previous rasterizer.
     * As a convenience, the parameter passed is also returned.
     *
     * @param rasterizer May be null. The new rasterizer to be installed in
     *                   the paint.
     * @return           rasterizer
     */
    virtual CARAPI SetRasterizer(
        /* [in] */ IRasterizer* rasterizer);

    /**
     * Temporary API to expose layer drawing. This draws a shadow layer below
     * the main layer, with the specified offset and color, and blur radius.
     * If radius is 0, then the shadow layer is removed.
     */
    virtual CARAPI SetShadowLayer(
        /* [in] */ Float radius,
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [in] */ Int32 color);

    /**
     * Temporary API to clear the shadow layer.
     */
    virtual CARAPI ClearShadowLayer();

    /**
     * Return the paint's Align value for drawing text. This controls how the
     * text is positioned relative to its origin. LEFT align means that all of
     * the text will be drawn to the right of its origin (i.e. the origin
     * specifieds the LEFT edge of the text) and so on.
     *
     * @return the paint's Align value for drawing text.
     */
    virtual CARAPI_(PaintAlign) GetTextAlign();

    /**
     * Set the paint's text alignment. This controls how the
     * text is positioned relative to its origin. LEFT align means that all of
     * the text will be drawn to the right of its origin (i.e. the origin
     * specifieds the LEFT edge of the text) and so on.
     *
     * @param align set the paint's Align value for drawing text.
     */
    virtual CARAPI SetTextAlign(
        /* [in] */ PaintAlign align);

    /**
     * Return the paint's text size.
     *
     * @return the paint's text size.
     */
    virtual CARAPI_(Float) GetTextSize();

    /**
     * Set the paint's text size. This value must be > 0
     *
     * @param textSize set the paint's text size.
     */
    virtual CARAPI SetTextSize(
        /* [in] */ Float textSize);

    /**
     * Return the paint's horizontal scale factor for text. The default value
     * is 1.0.
     *
     * @return the paint's scale factor in X for drawing/measuring text
     */
    virtual CARAPI_(Float) GetTextScaleX();

    /**
     * Set the paint's horizontal scale factor for text. The default value
     * is 1.0. Values > 1.0 will stretch the text wider. Values < 1.0 will
     * stretch the text narrower.
     *
     * @param scaleX set the paint's scale in X for drawing/measuring text.
     */
    virtual CARAPI SetTextScaleX(
        /* [in] */ Float scaleX);

    /**
     * Return the paint's horizontal skew factor for text. The default value
     * is 0.
     *
     * @return         the paint's skew factor in X for drawing text.
     */
    virtual CARAPI_(Float) GetTextSkewX();

    /**
     * Set the paint's horizontal skew factor for text. The default value
     * is 0. For approximating oblique text, use values around -0.25.
     *
     * @param skewX set the paint's skew factor in X for drawing text.
     */
    virtual CARAPI SetTextSkewX(
        /* [in] */ Float skewX);

    /**
     * Return the distance above (negative) the baseline (ascent) based on the
     * current typeface and text size.
     *
     * @return the distance above (negative) the baseline (ascent) based on the
     *         current typeface and text size.
     */
    virtual CARAPI_(Float) Ascent();

    /**
     * Return the distance below (positive) the baseline (descent) based on the
     * current typeface and text size.
     *
     * @return the distance below (positive) the baseline (descent) based on
     *         the current typeface and text size.
     */
    virtual CARAPI_(Float) Descent();

    /**
     * Return the font's recommended interline spacing, given the Paint's
     * settings for typeface, textSize, etc. If metrics is not null, return the
     * fontmetric values in it.
     *
     * @param metrics If this object is not null, its fields are filled with
     *                the appropriate values given the paint's text attributes.
     * @return the font's recommended interline spacing.
     */
    virtual CARAPI_(Float) GetFontMetrics(
        /* [in] */ IPaintFontMetrics* metrics);

    /**
     * Allocates a new FontMetrics object, and then calls getFontMetrics(fm)
     * with it, returning the object.
     */
    virtual CARAPI_(AutoPtr<IPaintFontMetrics>) GetFontMetrics();

    /**
     * Return the font's interline spacing, given the Paint's settings for
     * typeface, textSize, etc. If metrics is not null, return the fontmetric
     * values in it. Note: all values have been converted to integers from
     * floats, in such a way has to make the answers useful for both spacing
     * and clipping. If you want more control over the rounding, call
     * getFontMetrics().
     *
     * @return the font's interline spacing.
     */
    virtual CARAPI_(Int32) GetFontMetricsInt(
        /* [in] */ IPaintFontMetricsInt* fmi);

    virtual CARAPI_(AutoPtr<IPaintFontMetricsInt>) GetFontMetricsInt();

    /**
     * Return the recommend line spacing based on the current typeface and
     * text size.
     *
     * @return  recommend line spacing based on the current typeface and
     *          text size.
     */
    virtual CARAPI_(Float) GetFontSpacing();

    /**
     * Return the width of the text.
     *
     * @param text  The text to measure
     * @param index The index of the first character to start measuring
     * @param count THe number of characters to measure, beginning with start
     * @return      The width of the text
     */
    virtual CARAPI_(Float) MeasureText(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count);

     /**
     * Return the width of the text.
     *
     * @param text  The text to measure
     * @param start The index of the first character to start measuring
     * @param end   1 beyond the index of the last character to measure
     * @return      The width of the text
     */
    virtual CARAPI_(Float) MeasureText(
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Return the width of the text.
     *
     * @param text  The text to measure
     * @return      The width of the text
     */
    virtual CARAPI_(Float) MeasureText(
        /* [in] */ String text);

    /**
     * Return the width of the text.
     *
     * @param text  The text to measure
     * @param start The index of the first character to start measuring
     * @param end   1 beyond the index of the last character to measure
     * @return      The width of the text
     */
    virtual CARAPI_(Float) MeasureText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Measure the text, stopping early if the measured width exceeds maxWidth.
     * Return the number of chars that were measured, and if measuredWidth is
     * not null, return in it the actual width measured.
     *
     * @param text  The text to measure
     * @param index The offset into text to begin measuring at
     * @param count The number of maximum number of entries to measure. If count
     *              is negative, then the characters before index are measured
     *              in reverse order. This allows for measuring the end of
     *              string.
     * @param maxWidth The maximum width to accumulate.
     * @param measuredWidth Optional. If not null, returns the actual width
     *                     measured.
     * @return The number of chars that were measured. Will always be <=
     *         abs(count).
     */
    virtual CARAPI_(Int32) BreakText(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth);

    /**
     * Measure the text, stopping early if the measured width exceeds maxWidth.
     * Return the number of chars that were measured, and if measuredWidth is
     * not null, return in it the actual width measured.
     *
     * @param text  The text to measure
     * @param start The offset into text to begin measuring at
     * @param end   The end of the text slice to measure.
     * @param measureForwards If true, measure forwards, starting at start.
     *                        Otherwise, measure backwards, starting with end.
     * @param maxWidth The maximum width to accumulate.
     * @param measuredWidth Optional. If not null, returns the actual width
     *                     measured.
     * @return The number of chars that were measured. Will always be <=
     *         abs(end - start).
     */
    virtual CARAPI_(Int32) BreakText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth);

    /**
     * Measure the text, stopping early if the measured width exceeds maxWidth.
     * Return the number of chars that were measured, and if measuredWidth is
     * not null, return in it the actual width measured.
     *
     * @param text  The text to measure
     * @param measureForwards If true, measure forwards, starting with the
     *                        first character in the string. Otherwise,
     *                        measure backwards, starting with the
     *                        last character in the string.
     * @param maxWidth The maximum width to accumulate.
     * @param measuredWidth Optional. If not null, returns the actual width
     *                     measured.
     * @return The number of chars that were measured. Will always be <=
     *         abs(count).
     */
    virtual CARAPI_(Int32) BreakText(
        /* [in] */ String text,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth);

    /**
     * Return the advance widths for the characters in the string.
     *
     * @param text     The text to measure
     * @param index    The index of the first char to to measure
     * @param count    The number of chars starting with index to measure
     * @param widths   array to receive the advance widths of the characters.
     *                 Must be at least a large as count.
     * @return         the actual number of widths returned.
     */
    virtual CARAPI_(Int32) GetTextWidths(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [out] */ ArrayOf<Float>* widths);

    /**
     * Return the advance widths for the characters in the string.
     *
     * @param text     The text to measure
     * @param start    The index of the first char to to measure
     * @param end      The end of the text slice to measure
     * @param widths   array to receive the advance widths of the characters.
     *                 Must be at least a large as (end - start).
     * @return         the actual number of widths returned.
     */
    virtual CARAPI_(Int32) GetTextWidths(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Float>* widths);

    /**
     * Return the advance widths for the characters in the string.
     *
     * @param text   The text to measure
     * @param start  The index of the first char to to measure
     * @param end    The end of the text slice to measure
     * @param widths array to receive the advance widths of the characters.
     *               Must be at least a large as the text.
     * @return       the number of unichars in the specified text.
     */
    virtual CARAPI_(Int32) GetTextWidths(
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Float>* widths);

    /**
     * Return the advance widths for the characters in the string.
     *
     * @param text   The text to measure
     * @param widths array to receive the advance widths of the characters.
     *               Must be at least a large as the text.
     * @return       the number of unichars in the specified text.
     */
    virtual CARAPI_(Int32) GetTextWidths(
        /* [in] */ String text,
        /* [out] */ ArrayOf<Float>* widths);

    /**
     * Return the path (outline) for the specified text.
     * Note: just like Canvas.drawText, this will respect the Align setting in
     * the paint.
     *
     * @param text     The text to retrieve the path from
     * @param index    The index of the first character in text
     * @param count    The number of characterss starting with index
     * @param x        The x coordinate of the text's origin
     * @param y        The y coordinate of the text's origin
     * @param path     The path to receive the data describing the text. Must
     *                 be allocated by the caller.
     */
    virtual CARAPI GetTextPath(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath* path);

    /**
     * Return the path (outline) for the specified text.
     * Note: just like Canvas.drawText, this will respect the Align setting
     * in the paint.
     *
     * @param text  The text to retrieve the path from
     * @param start The first character in the text
     * @param end   1 past the last charcter in the text
     * @param x     The x coordinate of the text's origin
     * @param y     The y coordinate of the text's origin
     * @param path  The path to receive the data describing the text. Must
     *              be allocated by the caller.
     */
    virtual CARAPI GetTextPath(
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath* path);

    /**
     * Return in bounds (allocated by the caller) the smallest rectangle that
     * encloses all of the characters, with an implied origin at (0,0).
     *
     * @param text  String to measure and return its bounds
     * @param start Index of the first char in the string to measure
     * @param end   1 past the last char in the string measure
     * @param bounds Returns the unioned bounds of all the text. Must be
     *               allocated by the caller.
     */
    virtual CARAPI GetTextBounds(
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IRect* bounds);

    /**
     * Return in bounds (allocated by the caller) the smallest rectangle that
     * encloses all of the characters, with an implied origin at (0,0).
     *
     * @param text  Array of chars to measure and return their unioned bounds
     * @param index Index of the first char in the array to measure
     * @param count The number of chars, beginning at index, to measure
     * @param bounds Returns the unioned bounds of all the text. Must be
     *               allocated by the caller.
     */
    virtual CARAPI GetTextBounds(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IRect* bounds);

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ Int32 flags);

    CARAPI Init(
        /* [in] */ IPaint * pPaint);

private:
    static CARAPI_(SkPaint*) NativeInit();

    static CARAPI_(SkPaint*) NativeInitWithPaint(
        /* [in] */ SkPaint* nObj);

    static CARAPI_(void) NativeReset(
        /* [in] */ SkPaint* nObj);

    static CARAPI_(void) NativeSet(
        /* [in] */ SkPaint* nDst,
        /* [in] */ SkPaint* nSrc);

    static CARAPI_(Int32) NativeGetStyle(
        /* [in] */ SkPaint* nObj);

    static CARAPI_(void) NativeSetStyle(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkPaint::Style style);

    static CARAPI_(Int32) NativeGetStrokeCap(
        /* [in] */ SkPaint* nObj);

    static CARAPI_(void) NativeSetStrokeCap(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkPaint::Cap cap);

    static CARAPI_(Int32) NativeGetStrokeJoin(
        /* [in] */ SkPaint* nObj);

    static CARAPI_(void) NativeSetStrokeJoin(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkPaint::Join join);

    static CARAPI_(Boolean) NativeGetFillPath(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkPath* src,
        /* [in] */ SkPath* dst);

    static CARAPI_(SkShader*) NativeSetShader(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkShader* shader);

    static CARAPI_(SkColorFilter*) NativeSetColorFilter(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkColorFilter* filter);

    static CARAPI_(SkXfermode*) NativeSetXfermode(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkXfermode* xfermode);

    static CARAPI_(SkPathEffect*) NativeSetPathEffect(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkPathEffect* effect);

    static CARAPI_(SkMaskFilter*) NativeSetMaskFilter(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkMaskFilter* maskfilter);

    static CARAPI_(SkTypeface*) NativeSetTypeface(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkTypeface* typeface);

    static CARAPI_(SkRasterizer*) NativeSetRasterizer(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkRasterizer* rasterizer);

    static CARAPI_(Int32) NativeGetTextAlign(
        /* [in] */ SkPaint* nObj);

    static CARAPI_(void) NativeSetTextAlign(
        /* [in] */ SkPaint* nObj,
        /* [in] */ SkPaint::Align align);

    static CARAPI_(Int32) DoTextWidths(
        /* [in] */ SkPaint* paint,
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<Float>* widths);

    static CARAPI_(Int32) NativeGetTextWidths(
        /* [in] */ SkPaint* nObj,
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [out] */ ArrayOf<Float>* widths);

    static CARAPI_(Int32) NativeGetTextWidths(
        /* [in] */ SkPaint* nObj,
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Float>* widths);

    static CARAPI_(void) NativeGetTextPath(
        /* [in] */ SkPaint* nObj,
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ SkPath* path);

    static CARAPI_(void) NativeGetTextPath(
        /* [in] */ SkPaint* nObj,
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ SkPath* path);

    static CARAPI_(void) DoTextBounds(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 count,
        /* [in] */ IRect* bounds,
        /* [in] */ const SkPaint& paint);

    static CARAPI_(void) NativeGetStringBounds(
        /* [in] */ SkPaint* nObj,
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IRect* bounds);

    static CARAPI_(void) NativeGetCharArrayBounds(
        /* [in] */ SkPaint* nObj,
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IRect* bounds);

    static CARAPI_(void) Finalizer(
        /* [in] */ SkPaint* nativePaint);

    CARAPI_(Float) NativeMeasureText(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count);

    CARAPI_(Float) NativeMeasureText(
        /* [in] */ String text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Float) NativeMeasureText(
        /* [in] */ String text);

    static CARAPI_(Int32) DoBreakText(
        /* [in] */ const SkPaint& paint,
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 count,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [in] */ SkPaint::TextBufferDirection tbd);

    CARAPI_(Int32) NativeBreakText(
        /* [in] */ const BufferOf<Byte>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth);

    CARAPI_(Int32) NativeBreakText(
        /* [in] */ String text,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth);

public:
    SkPaint* mNativePaint;

private:
    static const PaintStyle sStyleArray[];
    static const PaintCap sCapArray[];
    static const PaintJoin sJoinArray[];
    static const PaintAlign sAlignArray[];

    // we use this when we first create a paint
    static const Int32 DEFAULT_PAINT_FLAGS;

    AutoPtr<IColorFilter> mColorFilter;
    AutoPtr<IMaskFilter>  mMaskFilter;
    AutoPtr<IPathEffect>  mPathEffect;
    AutoPtr<IRasterizer>  mRasterizer;
    AutoPtr<IShader>      mShader;
    AutoPtr<ITypeface>    mTypeface;
    AutoPtr<IXfermode>    mXfermode;

    Boolean     mHasCompatScaling;
    Float       mCompatScaling;
    Float       mInvCompatScaling;
};

#endif //__PAINT_H__
