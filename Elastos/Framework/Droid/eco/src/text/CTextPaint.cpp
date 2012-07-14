
#include "text/CTextPaint.h"

CTextPaint::CTextPaint()
    : mDrawableState(NULL)
    , mDensity(1.0f)
{}

CTextPaint::~CTextPaint()
{
    ArrayOf<Int32>::Free(mDrawableState);
}

PInterface CTextPaint::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Paint) {
        return reinterpret_cast<PInterface>((Paint*)this);
    }
    return _CTextPaint::Probe(riid);
}

ECode CTextPaint::Reset()
{
    return Paint::Reset();
}

ECode CTextPaint::Set(
    /* [in] */ IPaint* src)
{
    return Paint::Set(src);
}

ECode CTextPaint::SetCompatibilityScaling(
    /* [in] */ Float factor)
{
    return Paint::SetCompatibilityScaling(factor);
}

ECode CTextPaint::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = Paint::GetFlags();
    return NOERROR;
}

ECode CTextPaint::SetFlags(
    /* [in] */ Int32 flags)
{
    return Paint::SetFlags(flags);
}

ECode CTextPaint::IsAntiAlias(
    /* [out] */ Boolean* isAntiAlias)
{
    VALIDATE_NOT_NULL(isAntiAlias);
    *isAntiAlias = Paint::IsAntiAlias();
    return NOERROR;
}

ECode CTextPaint::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    return Paint::SetAntiAlias(aa);
}

ECode CTextPaint::IsDither(
    /* [out] */ Boolean* isDither)
{
    VALIDATE_NOT_NULL(isDither);
    *isDither = Paint::IsDither();
    return NOERROR;
}

ECode CTextPaint::SetDither(
    /* [in] */ Boolean dither)
{
    return Paint::SetDither(dither);
}

ECode CTextPaint::IsLinearText(
    /* [out] */ Boolean* isLinearText)
{
    VALIDATE_NOT_NULL(isLinearText);
    *isLinearText = Paint::IsLinearText();
    return NOERROR;
}

ECode CTextPaint::SetLinearText(
    /* [in] */ Boolean linearText)
{
    return Paint::SetLinearText(linearText);
}

ECode CTextPaint::IsSubpixelText(
    /* [out] */ Boolean* isSubpixelText)
{
    VALIDATE_NOT_NULL(isSubpixelText);
    *isSubpixelText = Paint::IsSubpixelText();
    return NOERROR;
}

ECode CTextPaint::SetSubpixelText(
    /* [in] */ Boolean subpixelText)
{
    return Paint::SetSubpixelText(subpixelText);
}

ECode CTextPaint::IsUnderlineText(
    /* [out] */ Boolean* isUnderlineText)
{
    VALIDATE_NOT_NULL(isUnderlineText);
    *isUnderlineText = Paint::IsUnderlineText();
    return NOERROR;
}

ECode CTextPaint::SetUnderlineText(
    /* [in] */ Boolean underlineText)
{
    return Paint::SetUnderlineText(underlineText);
}

ECode CTextPaint::IsStrikeThruText(
    /* [out] */ Boolean* isStrikeThruText)
{
    VALIDATE_NOT_NULL(isStrikeThruText);
    *isStrikeThruText = Paint::IsStrikeThruText();
    return NOERROR;
}

ECode CTextPaint::SetStrikeThruText(
    /* [in] */ Boolean strikeThruText)
{
    return Paint::SetStrikeThruText(strikeThruText);
}

ECode CTextPaint::IsFakeBoldText(
    /* [out] */ Boolean* isFakeBoldText)
{
    VALIDATE_NOT_NULL(isFakeBoldText);
    *isFakeBoldText = Paint::IsFakeBoldText();
    return NOERROR;
}

ECode CTextPaint::SetFakeBoldText(
    /* [in] */ Boolean fakeBoldText)
{
    return Paint::SetFakeBoldText(fakeBoldText);
}

ECode CTextPaint::IsFilterBitmap(
    /* [out] */ Boolean* isFilterBitmap)
{
    VALIDATE_NOT_NULL(isFilterBitmap);
    *isFilterBitmap = Paint::IsFilterBitmap();
    return NOERROR;
}

ECode CTextPaint::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    return Paint::SetFilterBitmap(filter);
}

ECode CTextPaint::GetStyle(
    /* [out] */ PaintStyle* style)
{
    VALIDATE_NOT_NULL(style);
    *style = Paint::GetStyle();
    return NOERROR;
}

ECode CTextPaint::SetStyle(
    /* [in] */ PaintStyle style)
{
    return Paint::SetFilterBitmap(style);
}

ECode CTextPaint::GetColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = Paint::GetColor();
    return NOERROR;
}

ECode CTextPaint::SetColor(
    /* [in] */ Int32 color)
{
    return Paint::SetColor(color);
}

ECode CTextPaint::GetAlpha(
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = Paint::GetAlpha();
    return NOERROR;
}

ECode CTextPaint::SetAlpha(
    /* [in] */ Int32 a)
{
    return Paint::SetAlpha(a);
}

ECode CTextPaint::SetARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return Paint::SetARGB(a, r, g, b);
}

ECode CTextPaint::GetStrokeWidth(
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::GetStrokeWidth();
    return NOERROR;
}

ECode CTextPaint::SetStrokeWidth(
    /* [in] */ Float width)
{
    return Paint::SetStrokeWidth(width);
}

ECode CTextPaint::GetStrokeMiter(
    /* [out] */ Float* strokeMiter)
{
    VALIDATE_NOT_NULL(strokeMiter);
    *strokeMiter = Paint::GetStrokeMiter();
    return NOERROR;
}

ECode CTextPaint::SetStrokeMiter(
    /* [in] */ Float miter)
{
    return Paint::SetStrokeMiter(miter);
}

ECode CTextPaint::GetStrokeCap(
    /* [out] */ PaintCap* cap)
{
    VALIDATE_NOT_NULL(cap);
    *cap = Paint::GetStrokeCap();
    return NOERROR;
}

ECode CTextPaint::SetStrokeCap(
    /* [in] */ PaintCap cap)
{
    return Paint::SetStrokeCap(cap);
}

ECode CTextPaint::GetStrokeJoin(
    /* [out] */ PaintJoin* join)
{
    VALIDATE_NOT_NULL(join);
    *join = Paint::GetStrokeJoin();
    return NOERROR;
}

ECode CTextPaint::SetStrokeJoin(
    /* [in] */ PaintJoin join)
{
    return Paint::SetStrokeJoin(join);
}

ECode CTextPaint::GetFillPath(
    /* [in] */ IPath * src,
    /* [in] */ IPath * dst,
    /* [out] */ Boolean * isShouldFilled)
{
    VALIDATE_NOT_NULL(isShouldFilled);
    *isShouldFilled = Paint::GetFillPath(src, dst);
    return NOERROR;
}

ECode CTextPaint::GetShader(
    /* [out] */ IShader** shader)
{
    VALIDATE_NOT_NULL(shader);
    AutoPtr<IShader> s = Paint::GetShader();
    *shader = s.Get();
    if (*shader) (*shader)->AddRef();
    return NOERROR;
}

ECode CTextPaint::SetShader(
    /* [in] */ IShader* shader)
{
    return Paint::SetShader(shader);
}

ECode CTextPaint::GetColorFilter(
    /* [out] */ IColorFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IColorFilter> f = Paint::GetColorFilter();
    *filter = f.Get();
    if (*filter) (*filter)->AddRef();
    return NOERROR;
}

ECode CTextPaint::SetColorFilter(
    /* [in] */ IColorFilter* filter)
{
    return Paint::SetColorFilter(filter);
}

ECode CTextPaint::GetXfermode(
    /* [out] */ IXfermode** xfermode)
{
    VALIDATE_NOT_NULL(xfermode);
    AutoPtr<IXfermode> x = Paint::GetXfermode();
    *xfermode = x.Get();
    if (*xfermode) (*xfermode)->AddRef();
    return NOERROR;
}

ECode CTextPaint::SetXfermode(
    /* [in] */ IXfermode* xfermode)
{
    return Paint::SetXfermode(xfermode);
}

ECode CTextPaint::GetPathEffect(
    /* [out] */ IPathEffect** effect)
{
    VALIDATE_NOT_NULL(effect);
    AutoPtr<IPathEffect> e = Paint::GetPathEffect();
    *effect = e.Get();
    if (*effect) (*effect)->AddRef();
    return NOERROR;
}

ECode CTextPaint::SetPathEffect(
    /* [in] */ IPathEffect* effect)
{
    return Paint::SetPathEffect(effect);
}

ECode CTextPaint::GetMaskFilter(
    /* [out] */ IMaskFilter** maskfilter)
{
    VALIDATE_NOT_NULL(maskfilter);
    AutoPtr<IMaskFilter> m = Paint::GetMaskFilter();
    *maskfilter = m.Get();
    if (*maskfilter) (*maskfilter)->AddRef();
    return NOERROR;
}

ECode CTextPaint::SetMaskFilter(
    /* [in] */ IMaskFilter* maskfilter)
{
    return Paint::SetMaskFilter(maskfilter);
}

ECode CTextPaint::GetTypeface(
    /* [out] */ ITypeface** typeface)
{
    VALIDATE_NOT_NULL(typeface);
    AutoPtr<ITypeface> t = Paint::GetTypeface();
    *typeface = t.Get();
    if (*typeface) (*typeface)->AddRef();
    return NOERROR;
}

ECode CTextPaint::SetTypeface(
    /* [in] */ ITypeface* typeface)
{
    return Paint::SetTypeface(typeface);
}

ECode CTextPaint::GetRasterizer(
    /* [out] */ IRasterizer** rasterizer)
{
    VALIDATE_NOT_NULL(rasterizer);
    AutoPtr<IRasterizer> r = Paint::GetRasterizer();
    *rasterizer = r.Get();
    if (*rasterizer) (*rasterizer)->AddRef();
    return NOERROR;
}

ECode CTextPaint::SetRasterizer(
    /* [in] */ IRasterizer* rasterizer)
{
    return Paint::SetRasterizer(rasterizer);
}

ECode CTextPaint::SetShadowLayer(
    /* [in] */ Float radius,
    /* [in] */ Float rdx,
    /* [in] */ Float rdy,
    /* [in] */ Int32 color)
{
    return Paint::SetShadowLayer(radius, rdx, rdy, color);
}

ECode CTextPaint::ClearShadowLayer()
{
    return Paint::ClearShadowLayer();
}

ECode CTextPaint::GetTextAlign(
    /* [out] */ PaintAlign* align)
{
    VALIDATE_NOT_NULL(align);
    *align = Paint::GetTextAlign();
    return NOERROR;
}

ECode CTextPaint::SetTextAlign(
    /* [in] */ PaintAlign align)
{
    return Paint::SetTextAlign(align);
}

ECode CTextPaint::GetTextSize(
    /* [out] */ Float* size)
{
    VALIDATE_NOT_NULL(size);
    *size = Paint::GetTextSize();
    return NOERROR;
}

ECode CTextPaint::SetTextSize(
    /* [in] */ Float textSize)
{
    return Paint::SetTextSize(textSize);
}

ECode CTextPaint::GetTextScaleX(
    /* [out] */ Float* scaleX)
{
    VALIDATE_NOT_NULL(scaleX);
    *scaleX = Paint::GetTextScaleX();
    return NOERROR;
}

ECode CTextPaint::SetTextScaleX(
    /* [in] */ Float scaleX)
{
    return Paint::SetTextScaleX(scaleX);
}

ECode CTextPaint::GetTextSkewX(
    /* [out] */ Float* skewX)
{
    VALIDATE_NOT_NULL(skewX);
    *skewX = Paint::GetTextSkewX();
    return NOERROR;
}

ECode CTextPaint::SetTextSkewX(
    /* [in] */ Float skewX)
{
    return Paint::SetTextSkewX(skewX);
}

ECode CTextPaint::Ascent(
    /* [out] */ Float* distance)
{
    VALIDATE_NOT_NULL(distance);
    *distance = Paint::Ascent();
    return NOERROR;
}

ECode CTextPaint::Descent(
    /* [out] */ Float* distance)
{
    VALIDATE_NOT_NULL(distance);
    *distance = Paint::Descent();
    return NOERROR;
}

ECode CTextPaint::GetFontMetrics(
    /* [in] */ IPaintFontMetrics* metrics,
    /* [out] */ Float* dpacing)
{
    VALIDATE_NOT_NULL(dpacing);
    *dpacing = Paint::GetFontMetrics(metrics);
    return NOERROR;
}

ECode CTextPaint::GetFontMetricsEx(
    /* [out] */ IPaintFontMetrics** metrics)
{
    VALIDATE_NOT_NULL(metrics);
    AutoPtr<IPaintFontMetrics> m = Paint::GetFontMetrics();
    *metrics = m.Get();
    if (*metrics) (*metrics)->AddRef();
    return NOERROR;
}

ECode CTextPaint::GetFontMetricsInt(
    /* [in] */ IPaintFontMetricsInt* fmi,
    /* [out] */ Int32* spacing)
{
    VALIDATE_NOT_NULL(spacing);
    *spacing = Paint::GetFontMetricsInt(fmi);
    return NOERROR;
}

ECode CTextPaint::GetFontMetricsIntEx(
    /* [out] */ IPaintFontMetricsInt** fmi)
{
    VALIDATE_NOT_NULL(fmi);
    AutoPtr<IPaintFontMetricsInt> f = Paint::GetFontMetricsInt();
    *fmi = f.Get();
    if (*fmi) (*fmi)->AddRef();
    return NOERROR;
}

ECode CTextPaint::GetFontSpacing(
    /* [out] */ Float* spacing)
{
    VALIDATE_NOT_NULL(spacing);
    *spacing = Paint::GetFontSpacing();
    return NOERROR;
}

ECode CTextPaint::MeasureText(
    /* [in] */ const ArrayOf<Char8> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::MeasureText(text, index, count);
    return NOERROR;
}

ECode CTextPaint::MeasureTextEx(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::MeasureText(text, start, end);
    return NOERROR;
}

ECode CTextPaint::MeasureTextEx2(
    /* [in] */ const String& text,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::MeasureText(text);
    return NOERROR;
}

ECode CTextPaint::MeasureTextEx3(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::MeasureText(text, start, end);
    return NOERROR;
}

ECode CTextPaint::BreakText(
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

ECode CTextPaint::BreakTextEx(
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

ECode CTextPaint::BreakTextEx2(
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

ECode CTextPaint::GetTextWidths(
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

ECode CTextPaint::GetTextWidthsEx(
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

ECode CTextPaint::GetTextWidthsEx2(
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

ECode CTextPaint::GetTextWidthsEx3(
    /* [in] */ const String& text,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = Paint::GetTextWidths(text, widths);
    return NOERROR;
}

ECode CTextPaint::GetTextPath(
    /* [in] */ const ArrayOf<Char8> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    return Paint::GetTextPath(text, index, count, x, y, path);
}

ECode CTextPaint::GetTextPathEx(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    return Paint::GetTextPath(text, start, end, x, y, path);
}

ECode CTextPaint::GetTextBounds(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IRect* bounds)
{
    return Paint::GetTextBounds(text, start, end, bounds);
}

ECode CTextPaint::GetTextBoundsEx(
    /* [in] */ const ArrayOf<Char8> & text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds)
{
    return Paint::GetTextBounds(text, index, count, bounds);
}

ECode CTextPaint::SetEx(
    /* [in] */ ITextPaint* tp)
{
    FAIL_RETURN(Paint::Set((IPaint*)tp));

    mBgColor = ((CTextPaint*)tp)->mBgColor;
    mBaselineShift = ((CTextPaint*)tp)->mBaselineShift;
    mLinkColor = ((CTextPaint*)tp)->mLinkColor;
    if (mDrawableState != NULL) {
        ArrayOf<Int32>::Free(mDrawableState);
    }
    mDrawableState = ((CTextPaint*)tp)->mDrawableState->Clone();
    mDensity = ((CTextPaint*)tp)->mDensity;

    return NOERROR;
}

ECode CTextPaint::constructor()
{
    return Paint::Init();
}

ECode CTextPaint::constructor(
    /* [in] */ Int32 flags)
{
    return Paint::Init(flags);
}

ECode CTextPaint::constructor(
    /* [in] */ IPaint* paint)
{
    return Paint::Init(paint);
}

