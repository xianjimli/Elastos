
#include "graphics/Paint.h"
#include "graphics/CPath.h"
#include "graphics/CPaintFontMetrics.h"
#include "graphics/CPaintFontMetricsInt.h"
#include "graphics/Shader.h"
#include "graphics/ColorFilter.h"
#include "graphics/Xfermode.h"
#include "graphics/PathEffect.h"
#include "graphics/MaskFilter.h"
#include "graphics/Typeface.h"
#include "graphics/Rasterizer.h"
#include "graphics/Graphics.h"
#include "text/TextUtils.h"
#include <SkBlurDrawLooper.h>
#include <elastos/Character.h>

using namespace Elastos::Core;

// {4407AE9E-A61A-4FE3-8F99-12BFFDD68E56}
extern "C" const InterfaceID EIID_Paint =
    { 0x4407ae9e, 0xa61a, 0x4fe3, { 0x8f, 0x99, 0x12, 0xbf, 0xfd, 0xd6, 0x8e, 0x56 } };

static void DefaultSettingsForElastos(SkPaint* paint)
{
    // looks best we decided
    paint->setHinting(SkPaint::kSlight_Hinting);
    // utf8 is required for Elastos
    paint->setTextEncoding(SkPaint::kUTF8_TextEncoding);
}

const PaintStyle Paint::sStyleArray[] = {
    PaintStyle_FILL, PaintStyle_STROKE, PaintStyle_FILL_AND_STROKE
};
const PaintCap Paint::sCapArray[] = {
    PaintCap_BUTT, PaintCap_ROUND, PaintCap_SQUARE
};
const PaintJoin Paint::sJoinArray[] = {
    PaintJoin_MITER, PaintJoin_ROUND, PaintJoin_BEVEL
};
const PaintAlign Paint::sAlignArray[] = {
    PaintAlign_LEFT, PaintAlign_CENTER, PaintAlign_RIGHT
};

const Int32 Paint::DEFAULT_PAINT_FLAGS = Paint_DEV_KERN_TEXT_FLAG;

Paint::~Paint()
{
    Finalizer(mNativePaint);
}

/** Restores the paint to its default settings. */
ECode Paint::Reset()
{
    NativeReset(mNativePaint);
    SetFlags(DEFAULT_PAINT_FLAGS);
    mHasCompatScaling = FALSE;
    mCompatScaling = mInvCompatScaling = 1;
    return NOERROR;
}

/**
 * Copy the fields from src into this paint. This is equivalent to calling
 * get() on all of the src fields, and calling the corresponding set()
 * methods on this.
 */
ECode Paint::Set(
    /* [in] */ IPaint* src)
{
    assert(src != NULL);
    Paint* src_ = (Paint*)src->Probe(EIID_Paint);
    if (this != src_) {
        assert(src_ != NULL);
        // copy over the native settings
        NativeSet(mNativePaint, src_->mNativePaint);
        // copy over our java settings
        mColorFilter    = src_->mColorFilter;
        mMaskFilter     = src_->mMaskFilter;
        mPathEffect     = src_->mPathEffect;
        mRasterizer     = src_->mRasterizer;
        mShader         = src_->mShader;
        mTypeface       = src_->mTypeface;
        mXfermode       = src_->mXfermode;
        mHasCompatScaling = src_->mHasCompatScaling;
        mCompatScaling    = src_->mCompatScaling;
        mInvCompatScaling = src_->mInvCompatScaling;
    }
    return NOERROR;
}

/** @hide */
ECode Paint::SetCompatibilityScaling(
    /* [in] */ Float factor)
{
    if (factor == 1.0) {
        mHasCompatScaling = FALSE;
        mCompatScaling = mInvCompatScaling = 1.0f;
    }
    else {
        mHasCompatScaling = TRUE;
        mCompatScaling = factor;
        mInvCompatScaling = 1.0f/factor;
    }
    return NOERROR;
}

 /**
 * Return the paint's flags. Use the Flag enum to test flag values.
 *
 * @return the paint's flags (see enums ending in _Flag for bit masks)
 */
Int32 Paint::GetFlags()
{
    return mNativePaint->getFlags();
}

/**
 * Return the paint's flags. Use the Flag enum to test flag values.
 *
 * @return the paint's flags (see enums ending in _Flag for bit masks)
 */
ECode Paint::SetFlags(
    /* [in] */ Int32 flags)
{
    mNativePaint->setFlags(flags);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if ANTI_ALIAS_FLAG bit is set
 * AntiAliasing smooths out the edges of what is being drawn, but is has
 * no impact on the interior of the shape. See setDither() and
 * setFilterBitmap() to affect how colors are treated.
 *
 * @return true if the antialias bit is set in the paint's flags.
 */
Boolean Paint::IsAntiAlias()
{
    return (GetFlags() & Paint_ANTI_ALIAS_FLAG) != 0;
}

/**
 * Helper for setFlags(), setting or clearing the ANTI_ALIAS_FLAG bit
 * AntiAliasing smooths out the edges of what is being drawn, but is has
 * no impact on the interior of the shape. See setDither() and
 * setFilterBitmap() to affect how colors are treated.
 *
 * @param aa true to set the antialias bit in the flags, false to clear it
 */
ECode Paint::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    mNativePaint->setAntiAlias(aa);
    return NOERROR;
}

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
Boolean Paint::IsDither()
{
    return (GetFlags() & Paint_DITHER_FLAG) != 0;
}

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
ECode Paint::SetDither(
    /* [in] */ Boolean dither)
{
    mNativePaint->setDither(dither);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if LINEAR_TEXT_FLAG bit is set
 *
 * @return true if the lineartext bit is set in the paint's flags
 */
Boolean Paint::IsLinearText()
{
    return (GetFlags() & Paint_LINEAR_TEXT_FLAG) != 0;
}

/**
 * Helper for setFlags(), setting or clearing the LINEAR_TEXT_FLAG bit
 *
 * @param linearText true to set the linearText bit in the paint's flags,
 *                   false to clear it.
 */
ECode Paint::SetLinearText(
    /* [in] */ Boolean linearText)
{
    mNativePaint->setLinearText(linearText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if SUBPIXEL_TEXT_FLAG bit is set
 *
 * @return true if the subpixel bit is set in the paint's flags
 */
Boolean Paint::IsSubpixelText()
{
    return (GetFlags() & Paint_SUBPIXEL_TEXT_FLAG) != 0;
}

/**
 * Helper for setFlags(), setting or clearing the SUBPIXEL_TEXT_FLAG bit
 *
 * @param subpixelText true to set the subpixelText bit in the paint's
 *                     flags, false to clear it.
 */
ECode Paint::SetSubpixelText(
    /* [in] */ Boolean subpixelText)
{
    mNativePaint->setSubpixelText(subpixelText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if UNDERLINE_TEXT_FLAG bit is set
 *
 * @return true if the underlineText bit is set in the paint's flags.
 */
Boolean Paint::IsUnderlineText()
{
    return (GetFlags() & Paint_UNDERLINE_TEXT_FLAG) != 0;
}

/**
 * Helper for setFlags(), setting or clearing the UNDERLINE_TEXT_FLAG bit
 *
 * @param underlineText true to set the underlineText bit in the paint's
 *                      flags, false to clear it.
 */
ECode Paint::SetUnderlineText(
    /* [in] */ Boolean underlineText)
{
    mNativePaint->setUnderlineText(underlineText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if STRIKE_THRU_TEXT_FLAG bit is set
 *
 * @return true if the strikeThruText bit is set in the paint's flags.
 */
Boolean Paint::IsStrikeThruText()
{
    return (GetFlags() & Paint_STRIKE_THRU_TEXT_FLAG) != 0;
}

/**
 * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
 *
 * @param strikeThruText true to set the strikeThruText bit in the paint's
 *                       flags, false to clear it.
 */
ECode Paint::SetStrikeThruText(
    /* [in] */ Boolean strikeThruText)
{
    mNativePaint->setStrikeThruText(strikeThruText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if FAKE_BOLD_TEXT_FLAG bit is set
 *
 * @return true if the fakeBoldText bit is set in the paint's flags.
 */
Boolean Paint::IsFakeBoldText()
{
    return (GetFlags() & Paint_FAKE_BOLD_TEXT_FLAG) != 0;
}

/**
 * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
 *
 * @param fakeBoldText true to set the fakeBoldText bit in the paint's
 *                     flags, false to clear it.
 */
ECode Paint::SetFakeBoldText(
    /* [in] */ Boolean fakeBoldText)
{
    mNativePaint->setFakeBoldText(fakeBoldText);
    return NOERROR;
}

/**
 * Whether or not the bitmap filter is activated.
 * Filtering affects the sampling of bitmaps when they are transformed.
 * Filtering does not affect how the colors in the bitmap are converted into
 * device pixels. That is dependent on dithering and xfermodes.
 *
 * @see #setFilterBitmap(boolean) setFilterBitmap()
 */
Boolean Paint::IsFilterBitmap()
{
    return (GetFlags() & Paint_FILTER_BITMAP_FLAG) != 0;
}

/**
 * Helper for setFlags(), setting or clearing the FILTER_BITMAP_FLAG bit.
 * Filtering affects the sampling of bitmaps when they are transformed.
 * Filtering does not affect how the colors in the bitmap are converted into
 * device pixels. That is dependent on dithering and xfermodes.
 *
 * @param filter true to set the FILTER_BITMAP_FLAG bit in the paint's
 *               flags, false to clear it.
 */
ECode Paint::SetFilterBitmap(
    /* [in] */ Boolean filterBitmap)
{
    mNativePaint->setFilterBitmap(filterBitmap);
    return NOERROR;
}

/**
 * Return the paint's style, used for controlling how primitives'
 * geometries are interpreted (except for drawBitmap, which always assumes
 * FILL_STYLE).
 *
 * @return the paint's style setting (Fill, Stroke, StrokeAndFill)
 */
PaintStyle Paint::GetStyle()
{
    return sStyleArray[NativeGetStyle(mNativePaint)];
}

/**
 * Set the paint's style, used for controlling how primitives'
 * geometries are interpreted (except for drawBitmap, which always assumes
 * Fill).
 *
 * @param style The new style to set in the paint
 */
ECode Paint::SetStyle(
    /* [in] */ PaintStyle style)
{
    NativeSetStyle(mNativePaint, (SkPaint::Style)style);
    return NOERROR;
}

/**
 * Return the paint's color. Note that the color is a 32bit value
 * containing alpha as well as r,g,b. This 32bit value is not premultiplied,
 * meaning that its alpha can be any value, regardless of the values of
 * r,g,b. See the Color class for more details.
 *
 * @return the paint's color (and alpha).
 */
Int32 Paint::GetColor()
{
    return mNativePaint->getColor();
}

/**
 * Set the paint's color. Note that the color is an int containing alpha
 * as well as r,g,b. This 32bit value is not premultiplied, meaning that
 * its alpha can be any value, regardless of the values of r,g,b.
 * See the Color class for more details.
 *
 * @param color The new color (including alpha) to set in the paint.
 */
ECode Paint::SetColor(
    /* [in] */ Int32 color)
{
    mNativePaint->setColor(color);
    return NOERROR;
}

/**
 * Helper to getColor() that just returns the color's alpha value. This is
 * the same as calling getColor() >>> 24. It always returns a value between
 * 0 (completely transparent) and 255 (completely opaque).
 *
 * @return the alpha component of the paint's color.
 */
Int32 Paint::GetAlpha()
{
    return mNativePaint->getAlpha();
}

/**
 * Helper to setColor(), that only assigns the color's alpha value,
 * leaving its r,g,b values unchanged. Results are undefined if the alpha
 * value is outside of the range [0..255]
 *
 * @param a set the alpha component [0..255] of the paint's color.
 */
ECode Paint::SetAlpha(
    /* [in] */ Int32 a)
{
    mNativePaint->setAlpha(a);
    return NOERROR;
}

/**
 * Helper to setColor(), that takes a,r,g,b and constructs the color int
 *
 * @param a The new alpha component (0..255) of the paint's color.
 * @param r The new red component (0..255) of the paint's color.
 * @param g The new green component (0..255) of the paint's color.
 * @param b The new blue component (0..255) of the paint's color.
 */
ECode Paint::SetARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return SetColor((a << 24) | (r << 16) | (g << 8) | b);
}

/**
 * Return the width for stroking.
 * <p />
 * A value of 0 strokes in hairline mode.
 * Hairlines always draws a single pixel independent of the canva's matrix.
 *
 * @return the paint's stroke width, used whenever the paint's style is
 *         Stroke or StrokeAndFill.
 */
Float Paint::GetStrokeWidth()
{
    return SkScalarToFloat(mNativePaint->getStrokeWidth());
}

/**
 * Set the width for stroking.
 * Pass 0 to stroke in hairline mode.
 * Hairlines always draws a single pixel independent of the canva's matrix.
 *
 * @param width set the paint's stroke width, used whenever the paint's
 *              style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeWidth(
    /* [in] */ Float width)
{
    mNativePaint->setStrokeWidth(SkFloatToScalar(width));
    return NOERROR;
}

/**
 * Return the paint's stroke miter value. Used to control the behavior
 * of miter joins when the joins angle is sharp.
 *
 * @return the paint's miter limit, used whenever the paint's style is
 *         Stroke or StrokeAndFill.
 */
Float Paint::GetStrokeMiter()
{
    return SkScalarToFloat(mNativePaint->getStrokeMiter());
}

/**
 * Set the paint's stroke miter value. This is used to control the behavior
 * of miter joins when the joins angle is sharp. This value must be >= 0.
 *
 * @param miter set the miter limit on the paint, used whenever the paint's
 *              style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeMiter(
    /* [in] */ Float miter)
{
    mNativePaint->setStrokeMiter(SkFloatToScalar(miter));
    return NOERROR;
}

/**
 * Return the paint's Cap, controlling how the start and end of stroked
 * lines and paths are treated.
 *
 * @return the line cap style for the paint, used whenever the paint's
 *         style is Stroke or StrokeAndFill.
 */
PaintCap Paint::GetStrokeCap()
{
    return sCapArray[NativeGetStrokeCap(mNativePaint)];
}

/**
 * Set the paint's Cap.
 *
 * @param cap set the paint's line cap style, used whenever the paint's
 *            style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeCap(
    /* [in] */ PaintCap cap)
{
    NativeSetStrokeCap(mNativePaint, (SkPaint::Cap)cap);
    return NOERROR;
}

/**
 * Return the paint's stroke join type.
 *
 * @return the paint's Join.
 */
PaintJoin Paint::GetStrokeJoin()
{
    return sJoinArray[NativeGetStrokeJoin(mNativePaint)];
}

/**
 * Set the paint's Join.
 *
 * @param join set the paint's Join, used whenever the paint's style is
 *             Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeJoin(
    /* [in] */ PaintJoin join)
{
    NativeSetStrokeJoin(mNativePaint, (SkPaint::Join)join);
    return NOERROR;
}

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
Boolean Paint::GetFillPath(
    /* [in] */ IPath* src,
    /* [in] */ IPath* dst)
{
    return NativeGetFillPath(mNativePaint, ((CPath*)src)->Ni(), ((CPath*)dst)->Ni());
}

/**
 * Get the paint's shader object.
 *
 * @return the paint's shader (or null)
 */
AutoPtr<IShader> Paint::GetShader()
{
    return mShader;
}

/**
 * Set or clear the shader object.
 * <p />
 * Pass null to clear any previous shader.
 * As a convenience, the parameter passed is also returned.
 *
 * @param shader May be null. the new shader to be installed in the paint
 * @return       shader
 */
ECode Paint::SetShader(
    /* [in] */ IShader* shader)
{
    SkShader* shaderNative = NULL;
    if (shader != NULL) {
        Shader* s = (Shader*)shader->Probe(EIID_Shader);
        assert(s != NULL);
        shaderNative = s->mNative;
    }
    NativeSetShader(mNativePaint, shaderNative);
    mShader = shader;
    return NOERROR;
}

 /**
 * Get the paint's colorfilter (maybe be null).
 *
 * @return the paint's colorfilter (maybe be null)
 */
AutoPtr<IColorFilter> Paint::GetColorFilter()
{
    return mColorFilter;
}

/**
 * Set or clear the paint's colorfilter, returning the parameter.
 *
 * @param filter May be null. The new filter to be installed in the paint
 * @return       filter
 */
ECode Paint::SetColorFilter(
    /* [in] */ IColorFilter* filter)
{
    SkColorFilter* filterNative = NULL;
    if (filter != NULL) {
        ColorFilter* cf = (ColorFilter*)filter->Probe(EIID_ColorFilter);
        assert(cf != NULL);
        filterNative = cf->mNativeInstance;
    }
    NativeSetColorFilter(mNativePaint, filterNative);
    mColorFilter = filter;
    return NOERROR;
}

/**
 * Get the paint's xfermode object.
 *
 * @return the paint's xfermode (or null)
 */
AutoPtr<IXfermode> Paint::GetXfermode()
{
    return mXfermode;
}

/**
 * Set or clear the xfermode object.
 * <p />
 * Pass null to clear any previous xfermode.
 * As a convenience, the parameter passed is also returned.
 *
 * @param xfermode May be null. The xfermode to be installed in the paint
 * @return         xfermode
 */
ECode Paint::SetXfermode(
    /* [in] */ IXfermode* xfermode)
{
    SkXfermode* xfermodeNative = NULL;
    if (xfermode != NULL) {
        Xfermode* x = (Xfermode*)xfermode->Probe(EIID_Xfermode);
        assert(x != NULL);
        xfermodeNative = x->mNativeInstance;
    }
    NativeSetXfermode(mNativePaint, xfermodeNative);
    mXfermode = xfermode;
    return NOERROR;
}

/**
 * Get the paint's patheffect object.
 *
 * @return the paint's patheffect (or null)
 */
AutoPtr<IPathEffect> Paint::GetPathEffect()
{
    return mPathEffect;
}

/**
 * Set or clear the patheffect object.
 * <p />
 * Pass null to clear any previous patheffect.
 * As a convenience, the parameter passed is also returned.
 *
 * @param effect May be null. The patheffect to be installed in the paint
 * @return       effect
 */
ECode Paint::SetPathEffect(
    /* [in] */ IPathEffect* effect)
{
    SkPathEffect* effectNative = NULL;
    if (effect != NULL) {
        PathEffect* pe = (PathEffect*)effect->Probe(EIID_PathEffect);
        assert(pe != NULL);
        effectNative = pe->mNativeInstance;
    }
    NativeSetPathEffect(mNativePaint, effectNative);
    mPathEffect = effect;
    return NOERROR;
}

/**
 * Get the paint's maskfilter object.
 *
 * @return the paint's maskfilter (or null)
 */
AutoPtr<IMaskFilter> Paint::GetMaskFilter()
{
    return mMaskFilter;
}

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
ECode Paint::SetMaskFilter(
    /* [in] */ IMaskFilter* maskfilter)
{
    SkMaskFilter* maskfilterNative = NULL;
    if (maskfilter != NULL) {
        MaskFilter* mf = (MaskFilter*)maskfilter->Probe(EIID_MaskFilter);
        assert(mf != NULL);
        maskfilterNative = mf->mNativeInstance;
    }
    NativeSetMaskFilter(mNativePaint, maskfilterNative);
    mMaskFilter = maskfilter;
    return NOERROR;
}

/**
 * Get the paint's typeface object.
 * <p />
 * The typeface object identifies which font to use when drawing or
 * measuring text.
 *
 * @return the paint's typeface (or null)
 */
AutoPtr<ITypeface> Paint::GetTypeface()
{
    return mTypeface;
}

/**
 * Set or clear the typeface object.
 * <p />
 * Pass null to clear any previous typeface.
 * As a convenience, the parameter passed is also returned.
 *
 * @param typeface May be null. The typeface to be installed in the paint
 * @return         typeface
 */
ECode Paint::SetTypeface(
    /* [in] */ ITypeface* typeface)
{
    SkTypeface* typefaceNative = NULL;
    if (typeface != NULL) {
        Typeface* t = (Typeface*)typeface->Probe(EIID_Typeface);
        typefaceNative = t->mNativeInstance;
    }
    NativeSetTypeface(mNativePaint, typefaceNative);
    mTypeface = typeface;
    return NOERROR;
}

/**
 * Get the paint's rasterizer (or null).
 * <p />
 * The raster controls/modifies how paths/text are turned into alpha masks.
 *
 * @return         the paint's rasterizer (or null)
 */
AutoPtr<IRasterizer> Paint::GetRasterizer()
{
    return mRasterizer;
}

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
ECode Paint::SetRasterizer(
    /* [in] */ IRasterizer* rasterizer)
{
    SkRasterizer* rasterizerNative = NULL;
    if (rasterizer != NULL) {
        Rasterizer* r = (Rasterizer*)rasterizer->Probe(EIID_Rasterizer);
        assert(r != NULL);
        rasterizerNative = r->mNativeInstance;
    }
    NativeSetRasterizer(mNativePaint, rasterizerNative);
    mRasterizer = rasterizer;
    return NOERROR;
}

/**
 * Temporary API to expose layer drawing. This draws a shadow layer below
 * the main layer, with the specified offset and color, and blur radius.
 * If radius is 0, then the shadow layer is removed.
 */
ECode Paint::SetShadowLayer(
    /* [in] */ Float radius,
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Int32 color)
{
    if (radius <= 0) {
        mNativePaint->setLooper(NULL);
    }
    else {
        mNativePaint->setLooper(new SkBlurDrawLooper(SkFloatToScalar(radius),
                                                     SkFloatToScalar(dx),
                                                     SkFloatToScalar(dy),
                                                     (SkColor)color))->unref();
    }
    return NOERROR;
}

/**
 * Temporary API to clear the shadow layer.
 */
ECode Paint::ClearShadowLayer()
{
    return SetShadowLayer(0, 0, 0, 0);
}

/**
 * Return the paint's Align value for drawing text. This controls how the
 * text is positioned relative to its origin. LEFT align means that all of
 * the text will be drawn to the right of its origin (i.e. the origin
 * specifieds the LEFT edge of the text) and so on.
 *
 * @return the paint's Align value for drawing text.
 */
PaintAlign Paint::GetTextAlign()
{
    return sAlignArray[NativeGetTextAlign(mNativePaint)];
}

/**
 * Set the paint's text alignment. This controls how the
 * text is positioned relative to its origin. LEFT align means that all of
 * the text will be drawn to the right of its origin (i.e. the origin
 * specifieds the LEFT edge of the text) and so on.
 *
 * @param align set the paint's Align value for drawing text.
 */
ECode Paint::SetTextAlign(
    /* [in] */ PaintAlign align)
{
    NativeSetTextAlign(mNativePaint, (SkPaint::Align)align);
    return NOERROR;
}

/**
 * Return the paint's text size.
 *
 * @return the paint's text size.
 */
Float Paint::GetTextSize()
{
    return SkScalarToFloat(mNativePaint->getTextSize());
}

/**
 * Set the paint's text size. This value must be > 0
 *
 * @param textSize set the paint's text size.
 */
ECode Paint::SetTextSize(
    /* [in] */ Float textSize)
{
    mNativePaint->setTextSize(SkFloatToScalar(textSize));
    return NOERROR;
}

/**
 * Return the paint's horizontal scale factor for text. The default value
 * is 1.0.
 *
 * @return the paint's scale factor in X for drawing/measuring text
 */
Float Paint::GetTextScaleX()
{
    return SkScalarToFloat(mNativePaint->getTextScaleX());
}

/**
 * Set the paint's horizontal scale factor for text. The default value
 * is 1.0. Values > 1.0 will stretch the text wider. Values < 1.0 will
 * stretch the text narrower.
 *
 * @param scaleX set the paint's scale in X for drawing/measuring text.
 */
ECode Paint::SetTextScaleX(
    /* [in] */ Float scaleX)
{
     mNativePaint->setTextScaleX(SkFloatToScalar(scaleX));
     return NOERROR;
}

/**
 * Return the paint's horizontal skew factor for text. The default value
 * is 0.
 *
 * @return         the paint's skew factor in X for drawing text.
 */
Float Paint::GetTextSkewX()
{
    return SkScalarToFloat(mNativePaint->getTextSkewX());
}

/**
 * Set the paint's horizontal skew factor for text. The default value
 * is 0. For approximating oblique text, use values around -0.25.
 *
 * @param skewX set the paint's skew factor in X for drawing text.
 */
ECode Paint::SetTextSkewX(
    /* [in] */ Float skewX)
{
    mNativePaint->setTextSkewX(SkFloatToScalar(skewX));
    return NOERROR;
}

/**
 * Return the distance above (negative) the baseline (ascent) based on the
 * current typeface and text size.
 *
 * @return the distance above (negative) the baseline (ascent) based on the
 *         current typeface and text size.
 */
Float Paint::Ascent()
{
    SkPaint::FontMetrics metrics;
    mNativePaint->getFontMetrics(&metrics);
    return SkScalarToFloat(metrics.fAscent);
}

/**
 * Return the distance below (positive) the baseline (descent) based on the
 * current typeface and text size.
 *
 * @return the distance below (positive) the baseline (descent) based on
 *         the current typeface and text size.
 */
Float Paint::Descent()
{
    SkPaint::FontMetrics metrics;
    mNativePaint->getFontMetrics(&metrics);
    return SkScalarToFloat(metrics.fDescent);
}

/**
 * Return the font's recommended interline spacing, given the Paint's
 * settings for typeface, textSize, etc. If metrics is not null, return the
 * fontmetric values in it.
 *
 * @param metrics If this object is not null, its fields are filled with
 *                the appropriate values given the paint's text attributes.
 * @return the font's recommended interline spacing.
 */
Float Paint::GetFontMetrics(
    /* [in] */ IPaintFontMetrics* metrics)
{
    SkPaint::FontMetrics metrics_;
    SkScalar spacing = mNativePaint->getFontMetrics(&metrics_);

    if (metrics) {
        metrics->SetTop(SkScalarToFloat(metrics_.fTop));
        metrics->SetAscent(SkScalarToFloat(metrics_.fAscent));
        metrics->SetDescent(SkScalarToFloat(metrics_.fDescent));
        metrics->SetBottom(SkScalarToFloat(metrics_.fBottom));
        metrics->SetLeading(SkScalarToFloat(metrics_.fLeading));
    }
    return SkScalarToFloat(spacing);
}

/**
 * Allocates a new FontMetrics object, and then calls getFontMetrics(fm)
 * with it, returning the object.
 */
AutoPtr<IPaintFontMetrics> Paint::GetFontMetrics()
{
    AutoPtr<IPaintFontMetrics> fm;
    CPaintFontMetrics::New((IPaintFontMetrics**)&fm);
    GetFontMetrics(fm);
    return fm;
}

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
Int32 Paint::GetFontMetricsInt(
    /* [in] */ IPaintFontMetricsInt* fmi)
{
    SkPaint::FontMetrics metrics;

    mNativePaint->getFontMetrics(&metrics);
    Int32 ascent = SkScalarRound(metrics.fAscent);
    Int32 descent = SkScalarRound(metrics.fDescent);
    Int32 leading = SkScalarRound(metrics.fLeading);

    if (fmi) {
        fmi->SetTop(SkScalarFloor(metrics.fTop));
        fmi->SetAscent(ascent);
        fmi->SetDescent(descent);
        fmi->SetBottom(SkScalarCeil(metrics.fBottom));
        fmi->SetLeading(leading);
    }
    return descent - ascent + leading;
}

AutoPtr<IPaintFontMetricsInt> Paint::GetFontMetricsInt()
{
    AutoPtr<IPaintFontMetricsInt> fm;
    CPaintFontMetricsInt::New((IPaintFontMetricsInt**)&fm);
    GetFontMetricsInt(fm);
    return fm;
}

/**
 * Return the recommend line spacing based on the current typeface and
 * text size.
 *
 * @return  recommend line spacing based on the current typeface and
 *          text size.
 */
Float Paint::GetFontSpacing()
{
    return GetFontMetrics(NULL);
}

/**
 * Return the width of the text.
 *
 * @param text  The text to measure
 * @param index The index of the first character to start measuring
 * @param count THe number of characters to measure, beginning with start
 * @return      The width of the text
 */
Float Paint::MeasureText(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count)
{
    if (!mHasCompatScaling) {
        return NativeMeasureText(text, index, count);
    }
    const Float oldSize = GetTextSize();
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text, index, count);
    SetTextSize(oldSize);
    return w * mInvCompatScaling;
}

 /**
 * Return the width of the text.
 *
 * @param text  The text to measure
 * @param start The index of the first character to start measuring
 * @param end   1 beyond the index of the last character to measure
 * @return      The width of the text
 */
Float Paint::MeasureText(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (!mHasCompatScaling) {
        return NativeMeasureText(text, start, end);
    }
    const Float oldSize = GetTextSize();
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text, start, end);
    SetTextSize(oldSize);
    return w * mInvCompatScaling;
}

/**
 * Return the width of the text.
 *
 * @param text  The text to measure
 * @return      The width of the text
 */
Float Paint::MeasureText(
    /* [in] */ const String& text)
{
    if (!mHasCompatScaling) {
        return NativeMeasureText(text);
    }
    const Float oldSize = GetTextSize();
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text);
    SetTextSize(oldSize);
    return w * mInvCompatScaling;
}

/**
 * Return the width of the text.
 *
 * @param text  The text to measure
 * @param start The index of the first character to start measuring
 * @param end   1 beyond the index of the last character to measure
 * @return      The width of the text
 */
Float Paint::MeasureText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
//    if (text instanceof String) {
//        return measureText((String)text, start, end);
//    }
//    if (text instanceof SpannedString ||
//        text instanceof SpannableString) {
//        return measureText(text.toString(), start, end);
//    }
//    if (text instanceof GraphicsOperations) {
//        return ((GraphicsOperations)text).measureText(start, end, this);
//    }

    ArrayOf<Char8>* buf;
    TextUtils::Obtain(4 * (end - start), &buf);
    TextUtils::GetChars(text, start, end, buf, 0);
    Float result = MeasureText(*buf, 0, end - start);
    TextUtils::Recycle(&buf);
    return result;
}

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
Int32 Paint::BreakText(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
    if (!mHasCompatScaling) {
        return NativeBreakText(text, index, count, maxWidth, measuredWidth);
    }
    const Float oldSize = GetTextSize();
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeBreakText(text, index, count, maxWidth * mCompatScaling,
            measuredWidth);
    SetTextSize(oldSize);
    if (measuredWidth != NULL) (*measuredWidth)[0] *= mInvCompatScaling;
    return res;
}

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
Int32 Paint::BreakText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
//    if (start == 0 && text instanceof String && end == text.length()) {
//        return breakText((String) text, measureForwards, maxWidth,
//                         measuredWidth);
//    }

    ArrayOf<Char8>* buf;
    TextUtils::Obtain(4 * (end - start), &buf);
    Int32 result;

    TextUtils::GetChars(text, start, end, buf, 0);

    if (measureForwards) {
        result = BreakText(*buf, 0, end - start, maxWidth, measuredWidth);
    }
    else {
        result = BreakText(*buf, 0, -(end - start), maxWidth, measuredWidth);
    }

    TextUtils::Recycle(&buf);
    return result;
}

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
Int32 Paint::BreakText(
    /* [in] */ const String& text,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
    if (!mHasCompatScaling) {
        return NativeBreakText(text, measureForwards, maxWidth, measuredWidth);
    }
    const Float oldSize = GetTextSize();
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeBreakText(text, measureForwards, maxWidth * mCompatScaling,
            measuredWidth);
    SetTextSize(oldSize);
    if (measuredWidth != NULL) (*measuredWidth)[0] *= mInvCompatScaling;
    return res;
}

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
Int32 Paint::GetTextWidths(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Float>* widths)
{
    if ((index | count) < 0 || count > widths->GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        assert(0);
    }

    Int32 offset;
    ECode ec = Character::GetOffsetByChars(text, 0,
            text.GetLength(), index, count, &offset);
    if (FAILED(ec)) {
//        throw new ArrayIndexOutOfBoundsException();
        assert(0);
    }

    if (!mHasCompatScaling) {
        return NativeGetTextWidths(mNativePaint, text, index, count, widths);
    }
    const Float oldSize = GetTextSize();
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeGetTextWidths(mNativePaint, text, index, count, widths);
    SetTextSize(oldSize);
    for (Int32 i = 0; i < res; i++) {
        (*widths)[i] *= mInvCompatScaling;
    }
    return res;
}

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
Int32 Paint::GetTextWidths(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Float>* widths)
{
//    if (text instanceof String) {
//        return getTextWidths((String) text, start, end, widths);
//    }
//    if (text instanceof SpannedString ||
//        text instanceof SpannableString) {
//        return getTextWidths(text.toString(), start, end, widths);
//    }
//    if (text instanceof GraphicsOperations) {
//        return ((GraphicsOperations) text).getTextWidths(start, end,
//                                                             widths, this);
//    }

    ArrayOf<Char8>* buf;
    TextUtils::Obtain(4 * (end - start), &buf);
    TextUtils::GetChars(text, start, end, buf, 0);
    Int32 result = GetTextWidths(*buf, 0, end - start, widths);
    TextUtils::Recycle(&buf);
    return result;
}

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
Int32 Paint::GetTextWidths(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Float>* widths)
{
    if ((start | end | (end - start) | (text.GetCharCount() - end)) < 0) {
//        throw new IndexOutOfBoundsException();
        assert(0);
    }
    if (end - start > widths->GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        assert(0);
    }

    if (!mHasCompatScaling) {
        return NativeGetTextWidths(mNativePaint, text, start, end, widths);
    }
    const Float oldSize = GetTextSize();
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeGetTextWidths(mNativePaint, text, start, end, widths);
    SetTextSize(oldSize);
    for (Int32 i = 0; i < res; i++) {
        (*widths)[i] *= mInvCompatScaling;
    }
    return res;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text   The text to measure
 * @param widths array to receive the advance widths of the characters.
 *               Must be at least a large as the text.
 * @return       the number of unichars in the specified text.
 */
Int32 Paint::GetTextWidths(
    /* [in] */ const String& text,
    /* [out] */ ArrayOf<Float>* widths)
{
    return GetTextWidths(text, 0, text.GetCharCount(), widths);
}

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
ECode Paint::GetTextPath(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    if ((index | count) < 0) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 offset;
    FAIL_RETURN(Character::GetOffsetByChars(text, 0,
            text.GetLength(), index, count, &offset));
    NativeGetTextPath(mNativePaint, text, index, count, x, y, ((CPath*)path)->Ni());
    return NOERROR;
}

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
ECode Paint::GetTextPath(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    if ((start | end | (end - start) | (text.GetCharCount() - end)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeGetTextPath(mNativePaint, text, start, end, x, y, ((CPath*)path)->Ni());
    return NOERROR;
}

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
ECode Paint::GetTextBounds(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IRect* bounds)
{
    if ((start | end | (end - start) | (text.GetCharCount() - end)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (bounds == NULL) {
//        throw new NullPointerException("need bounds Rect");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeGetStringBounds(mNativePaint, text, start, end, bounds);
    return NOERROR;
}

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
ECode Paint::GetTextBounds(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds)
{
    if ((index | count) < 0) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (bounds == NULL) {
//        throw new NullPointerException("need bounds Rect");
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 offset;
    FAIL_RETURN(Character::GetOffsetByChars(text, 0,
            text.GetLength(), index, count, &offset));
    NativeGetCharArrayBounds(mNativePaint, text, index, count, bounds);
    return NOERROR;
}

SkPaint* Paint::NativeInit()
{
    SkPaint* obj = new SkPaint();
    DefaultSettingsForElastos(obj);
    return obj;
}

SkPaint* Paint::NativeInitWithPaint(
    /* [in] */ SkPaint* nObj)
{
    SkPaint* obj = new SkPaint(*nObj);
    return obj;
}

void Paint::NativeReset(
    /* [in] */ SkPaint* nObj)
{
    nObj->reset();
    DefaultSettingsForElastos(nObj);
}

void Paint::NativeSet(
    /* [in] */ SkPaint* nDst,
    /* [in] */ SkPaint* nSrc)
{
    *nDst = *nSrc;
}

Int32 Paint::NativeGetStyle(
    /* [in] */ SkPaint* nObj)
{
    return nObj->getStyle();
}

void Paint::NativeSetStyle(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkPaint::Style style)
{
    nObj->setStyle(style);
}

Int32 Paint::NativeGetStrokeCap(
    /* [in] */ SkPaint* nObj)
{
    return nObj->getStrokeCap();
}

void Paint::NativeSetStrokeCap(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkPaint::Cap cap)
{
    nObj->setStrokeCap(cap);
}

Int32 Paint::NativeGetStrokeJoin(
    /* [in] */ SkPaint* nObj)
{
    return nObj->getStrokeJoin();
}

void Paint::NativeSetStrokeJoin(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkPaint::Join join)
{
    nObj->setStrokeJoin(join);
}

Boolean Paint::NativeGetFillPath(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkPath* src,
    /* [in] */ SkPath* dst)
{
    return nObj->getFillPath(*src, dst);
}

SkShader* Paint::NativeSetShader(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkShader* shader)
{
    return nObj->setShader(shader);
}

SkColorFilter* Paint::NativeSetColorFilter(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkColorFilter* filter)
{
    return nObj->setColorFilter(filter);
}

SkXfermode* Paint::NativeSetXfermode(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkXfermode* xfermode)
{
    return nObj->setXfermode(xfermode);
}

SkPathEffect* Paint::NativeSetPathEffect(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkPathEffect* effect)
{
    return nObj->setPathEffect(effect);
}

SkMaskFilter* Paint::NativeSetMaskFilter(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkMaskFilter* maskfilter)
{
    return nObj->setMaskFilter(maskfilter);
}

SkTypeface* Paint::NativeSetTypeface(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkTypeface* typeface)
{
    return nObj->setTypeface(typeface);
}

SkRasterizer* Paint::NativeSetRasterizer(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkRasterizer* rasterizer)
{
    return nObj->setRasterizer(rasterizer);
}

Int32 Paint::NativeGetTextAlign(
    /* [in] */ SkPaint* nObj)
{
    return nObj->getTextAlign();
}

void Paint::NativeSetTextAlign(
    /* [in] */ SkPaint* nObj,
    /* [in] */ SkPaint::Align align)
{
    nObj->setTextAlign(align);
}

Float Paint::NativeMeasureText(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count)
{
    //todo: (index + count) > text.GetCapcity() is not correct;
    if ((index | count) < 0) {
//        doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        assert(0);
        return 0;
    }

    Int32 offset;
    ECode ec = Character::GetOffsetByChars(text, 0,
            text.GetLength(), index, count, &offset);
    if (FAILED(ec)) {
//        doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        assert(0);
        return 0;
    }
    const char* textArray = (const char*)text.GetPayload();
    // we double count, since measureText wants a byteLength
    SkScalar width = mNativePaint->measureText(textArray + index, offset - index);
    return SkScalarToFloat(width);
}

Float Paint::NativeMeasureText(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 count = end - start;
    if ((start | count) < 0) {
//        doThrow(env, "java/lang/IndexOutOfBoundsException");
        assert(0);
        return 0;
    }

    Int32 startIndex, endIndex;
    ECode ec = Character::GetOffsetByChars(text, 0, start, &startIndex);
    if (FAILED(ec)) {
//        doThrow(env, "java/lang/IndexOutOfBoundsException");
        assert(0);
        return 0;
    }
    ec = Character::GetOffsetByChars(text, 0, end, &endIndex);
    if (FAILED(ec)) {
//        doThrow(env, "java/lang/IndexOutOfBoundsException");
        assert(0);
        return 0;
    }

    const char* textArray = (const char*)text;
    SkScalar width = mNativePaint->measureText(textArray + startIndex, endIndex - startIndex);
    return SkScalarToFloat(width);
}

Float Paint::NativeMeasureText(
    /* [in] */ const String& text)
{
    const char* textArray = (const char*)text;
    size_t textLength = (size_t)text.GetLength();

    Float width = SkScalarToFloat(mNativePaint->measureText(textArray, textLength));
    return width;
}

Int32 Paint::DoBreakText(
    /* [in] */ const SkPaint& paint,
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [in] */ SkPaint::TextBufferDirection tbd)
{
    SkASSERT(paint.getTextEncoding() == SkPaint::kUTF8_TextEncoding);

    Int32 offset;
    ECode ec = Character::GetOffsetByChars(text, 0,
            text.GetLength(), 0, count, &offset);
    if (FAILED(ec)) {
        assert(0);
    }

    SkScalar measured;
    size_t bytes = paint.breakText(text.GetPayload(), offset,
            SkFloatToScalar(maxWidth), &measured, tbd);
    SkASSERT((bytes & 1) == 0);

    if (measuredWidth && measuredWidth->GetLength() > 0) {
        assert(measuredWidth->GetLength() >= 1);
        (*measuredWidth)[0] = SkScalarToFloat(measured);
    }
    return bytes >> 1;
}

Int32 Paint::NativeBreakText(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
    SkPaint::TextBufferDirection tbd;
    if (count < 0) {
        tbd = SkPaint::kBackward_TextBufferDirection;
        count = -count;
    }
    else {
        tbd = SkPaint::kForward_TextBufferDirection;
    }

    if (index < 0) {
//        doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        return 0;
    }

    Int32 offset;
    ECode ec = Character::GetOffsetByChars(text, 0,
            text.GetLength(), index, count, &offset);
    if (FAILED(ec)) {
//        doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        return 0;
    }

    ArrayOf<Char8> buf(text.GetPayload() + index, text.GetLength() - index);
    count = DoBreakText(*mNativePaint, buf, count, maxWidth, measuredWidth, tbd);
    return count;
}

Int32 Paint::NativeBreakText(
    /* [in] */ const String& text,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
    SkPaint::TextBufferDirection tbd = measureForwards ?
            SkPaint::kForward_TextBufferDirection :
            SkPaint::kBackward_TextBufferDirection;


    Int32 count = text.GetCharCount();
    ArrayOf<Char8> buf(const_cast<char*>((const char*)text), text.GetLength());
    count = DoBreakText(*mNativePaint, buf, count, maxWidth, measuredWidth, tbd);
    return count;
}

Int32 Paint::DoTextWidths(
    /* [in] */ SkPaint* paint,
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* widths)
{
    assert(widths->GetLength() >= count);

    SkScalar* scalarArray = (SkScalar*)widths->GetPayload();

    Int32 offset;
    Character::GetOffsetByChars(text, 0, text.GetLength(), 0, count, &offset);
    count = paint->getTextWidths(text.GetPayload(), offset, scalarArray);
    for (Int32 i = 0; i < count; i++) {
        (*widths)[i] = SkScalarToFloat(scalarArray[i]);
    }
    return count;
}

Int32 Paint::NativeGetTextWidths(
    /* [in] */ SkPaint* nObj,
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Float>* widths)
{
    ArrayOf<Char8> buf(text.GetPayload() + index, text.GetLength() - index);
    count = DoTextWidths(nObj, buf, count, widths);
    return count;
}

Int32 Paint::NativeGetTextWidths(
    /* [in] */ SkPaint* nObj,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Float>* widths)
{
    Int32 offset;
    Character::GetOffsetByChars(text, 0, start, &offset);

    ArrayOf<Char8> buf(const_cast<char*>((const char*)text) + offset, text.GetLength() - offset);
    Int32 count = DoTextWidths(nObj, buf, end - start, widths);
    return count;
}

void Paint::NativeGetTextPath(
    /* [in] */ SkPaint* nObj,
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ SkPath* path)
{
    Int32 offset;
    Character::GetOffsetByChars(text, 0, text.GetLength(), index, count, &offset);
    const char* textArray = (const char*)text.GetPayload();
    nObj->getTextPath(textArray + index, offset - index,
            SkFloatToScalar(x), SkFloatToScalar(y), path);
}

void Paint::NativeGetTextPath(
    /* [in] */ SkPaint* nObj,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ SkPath* path)
{
    Int32 startOffset, endOffset;
    Character::GetOffsetByChars(text, 0, start, &startOffset);
    Character::GetOffsetByChars(text, 0, end, &endOffset);

    const char* textArray = (const char*)text;
    nObj->getTextPath(textArray + startOffset, endOffset - startOffset,
            SkFloatToScalar(x), SkFloatToScalar(y), path);
}

void Paint::DoTextBounds(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds,
    /* [in] */ const SkPaint& paint)
{
    SkRect  r;
    SkIRect ir;

    Int32 offset;
    Character::GetOffsetByChars(text, 0, text.GetLength(), 0, count, &offset);

    paint.measureText(text.GetPayload(), offset, &r);
    r.roundOut(&ir);
    Graphics::SkIRect2IRect(ir, bounds);
}

void Paint::NativeGetStringBounds(
    /* [in] */ SkPaint* nObj,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IRect* bounds)
{
    Int32 startOffset, endOffset;
    Character::GetOffsetByChars(text, 0, start, &startOffset);
    Character::GetOffsetByChars(text, 0, end, &endOffset);

    ArrayOf<Char8> buf(const_cast<char*>((const char*)text) + startOffset,
                       text.GetLength() - startOffset);
    DoTextBounds(buf, end - start, bounds, *nObj);
}

void Paint::NativeGetCharArrayBounds(
    /* [in] */ SkPaint* nObj,
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ int index,
    /* [in] */ int count,
    /* [in] */ IRect* bounds)
{
    ArrayOf<Char8> buf(text.GetPayload() + index, text.GetLength() - index);
    DoTextBounds(buf, count, bounds, *nObj);
}

void Paint::Finalizer(
    /* [in] */ SkPaint* nObj)
{
    delete nObj;
}

/**
 * Create a new paint with default settings.
 */
ECode Paint::Init()
{
    return Init(0);
}

/**
 * Create a new paint with the specified flags. Use setFlags() to change
 * these after the paint is created.
 *
 * @param flags initial flag bits, as if they were passed via setFlags().
 */
ECode Paint::Init(
    /* [in] */ Int32 flags)
{
    mNativePaint = NativeInit();
    SetFlags(flags | DEFAULT_PAINT_FLAGS);
    mCompatScaling = mInvCompatScaling = 1;
    return NOERROR;
}

ECode Paint::Init(
    /* [in] */ IPaint* paint)
{
    Paint* paint_ = (Paint*)paint->Probe(EIID_Paint);
    mNativePaint = NativeInitWithPaint(paint_->mNativePaint);
    mHasCompatScaling = paint_->mHasCompatScaling;
    mCompatScaling = paint_->mCompatScaling;
    mInvCompatScaling = paint_->mInvCompatScaling;
    return NOERROR;
}