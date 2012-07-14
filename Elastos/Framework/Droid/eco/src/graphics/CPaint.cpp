
#include "graphics/CPaint.h"

PInterface CPaint::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Paint) {
        return reinterpret_cast<PInterface>((Paint*)this);
    }
    return _CPaint::Probe(riid);
}

ECode CPaint::Reset()
{
    return Paint::Reset();
}

ECode CPaint::Set(
    /* [in] */ IPaint* src)
{
    return Paint::Set(src);
}

ECode CPaint::SetCompatibilityScaling(
    /* [in] */ Float factor)
{
    return Paint::SetCompatibilityScaling(factor);
}

ECode CPaint::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = Paint::GetFlags();
    return NOERROR;
}

/**
 * Return the paint's flags. Use the Flag enum to test flag values.
 *
 * @return the paint's flags (see enums ending in _Flag for bit masks)
 */
ECode CPaint::SetFlags(
    /* [in] */ Int32 flags)
{
    return Paint::SetFlags(flags);
}

ECode CPaint::IsAntiAlias(
    /* [out] */ Boolean* isAntiAlias)
{
    VALIDATE_NOT_NULL(isAntiAlias);
    *isAntiAlias = Paint::IsAntiAlias();
    return NOERROR;
}

ECode CPaint::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    return Paint::SetAntiAlias(aa);
}

ECode CPaint::IsDither(
    /* [out] */ Boolean* isDither)
{
    VALIDATE_NOT_NULL(isDither);
    *isDither = Paint::IsDither();
    return NOERROR;
}

ECode CPaint::SetDither(
    /* [in] */ Boolean dither)
{
    return Paint::SetDither(dither);
}

ECode CPaint::IsLinearText(
    /* [out] */ Boolean* isLinearText)
{
    VALIDATE_NOT_NULL(isLinearText);
    *isLinearText = Paint::IsLinearText();
    return NOERROR;
}

ECode CPaint::SetLinearText(
    /* [in] */ Boolean linearText)
{
    return Paint::SetLinearText(linearText);
}

ECode CPaint::IsSubpixelText(
    /* [out] */ Boolean* isSubpixelText)
{
    VALIDATE_NOT_NULL(isSubpixelText);
    *isSubpixelText = Paint::IsSubpixelText();
    return NOERROR;
}

ECode CPaint::SetSubpixelText(
    /* [in] */ Boolean subpixelText)
{
    return Paint::SetSubpixelText(subpixelText);
}

ECode CPaint::IsUnderlineText(
    /* [out] */ Boolean* isUnderlineText)
{
    VALIDATE_NOT_NULL(isUnderlineText);
    *isUnderlineText = Paint::IsUnderlineText();
    return NOERROR;
}

ECode CPaint::SetUnderlineText(
    /* [in] */ Boolean underlineText)
{
    return Paint::SetUnderlineText(underlineText);
}

ECode CPaint::IsStrikeThruText(
    /* [out] */ Boolean* isStrikeThruText)
{
    VALIDATE_NOT_NULL(isStrikeThruText);
    *isStrikeThruText = Paint::IsStrikeThruText();
    return NOERROR;
}

ECode CPaint::SetStrikeThruText(
    /* [in] */ Boolean strikeThruText)
{
    return Paint::SetStrikeThruText(strikeThruText);
}

ECode CPaint::IsFakeBoldText(
    /* [out] */ Boolean* isFakeBoldText)
{
    VALIDATE_NOT_NULL(isFakeBoldText);
    *isFakeBoldText = Paint::IsFakeBoldText();
    return NOERROR;
}

ECode CPaint::SetFakeBoldText(
    /* [in] */ Boolean fakeBoldText)
{
    return Paint::SetFakeBoldText(fakeBoldText);
}

ECode CPaint::IsFilterBitmap(
    /* [out] */ Boolean* isFilterBitmap)
{
    VALIDATE_NOT_NULL(isFilterBitmap);
    *isFilterBitmap = Paint::IsFilterBitmap();
    return NOERROR;
}

ECode CPaint::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    return Paint::SetFilterBitmap(filter);
}

ECode CPaint::GetStyle(
    /* [out] */ PaintStyle* style)
{
    VALIDATE_NOT_NULL(style);
    *style = Paint::GetStyle();
    return NOERROR;
}

ECode CPaint::SetStyle(
    /* [in] */ PaintStyle style)
{
    return Paint::SetFilterBitmap(style);
}

ECode CPaint::GetColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = Paint::GetColor();
    return NOERROR;
}

/**
 * Set the paint's color. Note that the color is an int containing alpha
 * as well as r,g,b. This 32bit value is not premultiplied, meaning that
 * its alpha can be any value, regardless of the values of r,g,b.
 * See the Color class for more details.
 *
 * @param color The new color (including alpha) to set in the paint.
 */
ECode CPaint::SetColor(
    /* [in] */ Int32 color)
{
    return Paint::SetColor(color);
}

ECode CPaint::GetAlpha(
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = Paint::GetAlpha();
    return NOERROR;
}

ECode CPaint::SetAlpha(
    /* [in] */ Int32 a)
{
    return Paint::SetAlpha(a);
}

ECode CPaint::SetARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Paint::SetARGB(a, r, g, b);
}

ECode CPaint::GetStrokeWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::GetStrokeWidth();
    return NOERROR;
}

ECode CPaint::SetStrokeWidth(
    /* [in] */ Float width)
{
    return Paint::SetStrokeWidth(width);
}

ECode CPaint::GetStrokeMiter(
    /* [out] */ Float* strokeMiter)
{
    VALIDATE_NOT_NULL(strokeMiter);
    *strokeMiter = Paint::GetStrokeMiter();
    return NOERROR;
}

ECode CPaint::SetStrokeMiter(
    /* [in] */ Float miter)
{
    return Paint::SetStrokeMiter(miter);
}

ECode CPaint::GetStrokeCap(
    /* [out] */ PaintCap* cap)
{
    VALIDATE_NOT_NULL(cap);
    *cap = Paint::GetStrokeCap();
    return NOERROR;
}

ECode CPaint::SetStrokeCap(
    /* [in] */ PaintCap cap)
{
    return Paint::SetStrokeCap(cap);
}

ECode CPaint::GetStrokeJoin(
    /* [out] */ PaintJoin* join)
{
    VALIDATE_NOT_NULL(join);
    *join = Paint::GetStrokeJoin();
    return NOERROR;
}

ECode CPaint::SetStrokeJoin(
    /* [in] */ PaintJoin join)
{
    return Paint::SetStrokeJoin(join);
}

ECode CPaint::GetFillPath(
    /* [in] */ IPath * src,
    /* [in] */ IPath * dst,
    /* [out] */ Boolean * isShouldFilled)
{
    VALIDATE_NOT_NULL(isShouldFilled);
    *isShouldFilled = Paint::GetFillPath(src, dst);
    return NOERROR;
}

ECode CPaint::GetShader(
    /* [out] */ IShader** shader)
{
    VALIDATE_NOT_NULL(shader);
    AutoPtr<IShader> s = Paint::GetShader();
    *shader = s.Get();
    if (*shader) (*shader)->AddRef();
    return NOERROR;
}

ECode CPaint::SetShader(
    /* [in] */ IShader* shader)
{
    return Paint::SetShader(shader);
}

ECode CPaint::GetColorFilter(
    /* [out] */ IColorFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IColorFilter> f = Paint::GetColorFilter();
    *filter = f.Get();
    if (*filter) (*filter)->AddRef();
    return NOERROR;
}

ECode CPaint::SetColorFilter(
    /* [in] */ IColorFilter* filter)
{
    return Paint::SetColorFilter(filter);
}

ECode CPaint::GetXfermode(
    /* [out] */ IXfermode** xfermode)
{
    VALIDATE_NOT_NULL(xfermode);
    AutoPtr<IXfermode> x = Paint::GetXfermode();
    *xfermode = x.Get();
    if (*xfermode) (*xfermode)->AddRef();
    return NOERROR;
}

ECode CPaint::SetXfermode(
    /* [in] */ IXfermode* xfermode)
{
    return Paint::SetXfermode(xfermode);
}

ECode CPaint::GetPathEffect(
    /* [out] */ IPathEffect** effect)
{
    VALIDATE_NOT_NULL(effect);
    AutoPtr<IPathEffect> e = Paint::GetPathEffect();
    *effect = e.Get();
    if (*effect) (*effect)->AddRef();
    return NOERROR;
}

ECode CPaint::SetPathEffect(
    /* [in] */ IPathEffect* effect)
{
    return Paint::SetPathEffect(effect);
}

ECode CPaint::GetMaskFilter(
    /* [out] */ IMaskFilter** maskfilter)
{
    VALIDATE_NOT_NULL(maskfilter);
    AutoPtr<IMaskFilter> m = Paint::GetMaskFilter();
    *maskfilter = m.Get();
    if (*maskfilter) (*maskfilter)->AddRef();
    return NOERROR;
}

ECode CPaint::SetMaskFilter(
    /* [in] */ IMaskFilter* maskfilter)
{
    return Paint::SetMaskFilter(maskfilter);
}

ECode CPaint::GetTypeface(
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    AutoPtr<ITypeface> t = Paint::GetTypeface();
    *typeface = t.Get();
    if (*typeface) (*typeface)->AddRef();
    return NOERROR;
}

ECode CPaint::SetTypeface(
    /* [in] */ ITypeface* typeface)
{
    return Paint::SetTypeface(typeface);
}

ECode CPaint::GetRasterizer(
    /* [out] */ IRasterizer** rasterizer)
{
    VALIDATE_NOT_NULL(rasterizer);
    AutoPtr<IRasterizer> r = Paint::GetRasterizer();
    *rasterizer = r.Get();
    if (*rasterizer) (*rasterizer)->AddRef();
    return NOERROR;
}

ECode CPaint::SetRasterizer(
    /* [in] */ IRasterizer* rasterizer)
{
    return Paint::SetRasterizer(rasterizer);
}

ECode CPaint::SetShadowLayer(
    /* [in] */ Float radius,
    /* [in] */ Float rdx,
    /* [in] */ Float rdy,
    /* [in] */ Int32 color)
{
    return Paint::SetShadowLayer(radius, rdx, rdy, color);
}

ECode CPaint::ClearShadowLayer()
{
    return Paint::ClearShadowLayer();
}

ECode CPaint::GetTextAlign(
    /* [out] */ PaintAlign* align)
{
    VALIDATE_NOT_NULL(align);
    *align = Paint::GetTextAlign();
    return NOERROR;
}

ECode CPaint::SetTextAlign(
    /* [in] */ PaintAlign align)
{
    return Paint::SetTextAlign(align);
}

ECode CPaint::GetTextSize(
    /* [out] */ Float* size)
{
    VALIDATE_NOT_NULL(size);
    *size = Paint::GetTextSize();
    return NOERROR;
}

ECode CPaint::SetTextSize(
    /* [in] */ Float textSize)
{
    return Paint::SetTextSize(textSize);
}

ECode CPaint::GetTextScaleX(
    /* [out] */ Float* scaleX)
{
    VALIDATE_NOT_NULL(scaleX);
    *scaleX = Paint::GetTextScaleX();
    return NOERROR;
}

ECode CPaint::SetTextScaleX(
    /* [in] */ Float scaleX)
{
    return Paint::SetTextScaleX(scaleX);
}

ECode CPaint::GetTextSkewX(
    /* [out] */ Float* skewX)
{
    VALIDATE_NOT_NULL(skewX);
    *skewX = Paint::GetTextSkewX();
    return NOERROR;
}

ECode CPaint::SetTextSkewX(
    /* [in] */ Float skewX)
{
    return Paint::SetTextSkewX(skewX);
}

ECode CPaint::Ascent(
    /* [out] */ Float* distance)
{
    VALIDATE_NOT_NULL(distance);
    *distance = Paint::Ascent();
    return NOERROR;
}

ECode CPaint::Descent(
    /* [out] */ Float* distance)
{
    VALIDATE_NOT_NULL(distance);
    *distance = Paint::Descent();
    return NOERROR;
}

ECode CPaint::GetFontMetrics(
    /* [in] */ IPaintFontMetrics* metrics,
    /* [out] */ Float* dpacing)
{
    VALIDATE_NOT_NULL(dpacing);
    *dpacing = Paint::GetFontMetrics(metrics);
    return NOERROR;
}

ECode CPaint::GetFontMetricsEx(
    /* [out] */ IPaintFontMetrics** metrics)
{
    VALIDATE_NOT_NULL(metrics);
    AutoPtr<IPaintFontMetrics> m = Paint::GetFontMetrics();
    *metrics = m.Get();
    if (*metrics) (*metrics)->AddRef();
    return NOERROR;
}

ECode CPaint::GetFontMetricsInt(
    /* [in] */ IPaintFontMetricsInt* fmi,
    /* [out] */ Int32* spacing)
{
    VALIDATE_NOT_NULL(spacing);
    *spacing = Paint::GetFontMetricsInt(fmi);
    return NOERROR;
}

ECode CPaint::GetFontMetricsIntEx(
    /* [out] */ IPaintFontMetricsInt** fmi)
{
    VALIDATE_NOT_NULL(fmi);
    AutoPtr<IPaintFontMetricsInt> f = Paint::GetFontMetricsInt();
    *fmi = f.Get();
    if (*fmi) (*fmi)->AddRef();
    return NOERROR;
}

ECode CPaint::GetFontSpacing(
    /* [out] */ Float* spacing)
{
    VALIDATE_NOT_NULL(spacing);
    *spacing = Paint::GetFontSpacing();
    return NOERROR;
}

ECode CPaint::MeasureText(
    /* [in] */ const ArrayOf<Char8> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::MeasureText(text, index, count);
    return NOERROR;
}

ECode CPaint::MeasureTextEx(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::MeasureText(text, start, end);
    return NOERROR;
}

ECode CPaint::MeasureTextEx2(
    /* [in] */ const String& text,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::MeasureText(text);
    return NOERROR;
}

ECode CPaint::MeasureTextEx3(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::MeasureText(text, start, end);
    return NOERROR;
}

ECode CPaint::BreakText(
    /* [in] */ const ArrayOf<Char8> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = Paint::BreakText(text, index, count, maxWidth, measuredWidth);
    return NOERROR;
}

ECode CPaint::BreakTextEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = Paint::BreakText(text, start, end, measureForwards, maxWidth, measuredWidth);
    return NOERROR;
}

ECode CPaint::BreakTextEx2(
    /* [in] */ const String& text,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = Paint::BreakText(text, measureForwards, maxWidth, measuredWidth);
    return NOERROR;
}

ECode CPaint::GetTextWidths(
    /* [in] */ const ArrayOf<Char8> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32 * width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::GetTextWidths(text, index, count, widths);
    return NOERROR;
}

ECode CPaint::GetTextWidthsEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::GetTextWidths(text, start, end, widths);
    return NOERROR;
}

ECode CPaint::GetTextWidthsEx2(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::GetTextWidths(text, start, end, widths);
    return NOERROR;
}

ECode CPaint::GetTextWidthsEx3(
    /* [in] */ const String& text,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::GetTextWidths(text, widths);
    return NOERROR;
}

ECode CPaint::GetTextPath(
    /* [in] */ const ArrayOf<Char8> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    return Paint::GetTextPath(text, index, count, x, y, path);
}

ECode CPaint::GetTextPathEx(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    return Paint::GetTextPath(text, start, end, x, y, path);
}

ECode CPaint::GetTextBounds(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IRect* bounds)
{
    return Paint::GetTextBounds(text, start, end, bounds);
}

ECode CPaint::GetTextBoundsEx(
    /* [in] */ const ArrayOf<Char8> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds)
{
    return Paint::GetTextBounds(text, index, count, bounds);
}

ECode CPaint::constructor()
{
    return Paint::Init();
}

ECode CPaint::constructor(
    /* [in] */ Int32 flags)
{
    return Paint::Init(flags);
}

ECode CPaint::constructor(
    /* [in] */ IPaint* paint)
{
    return Paint::Init(paint);
}
