
#ifndef __CPAINT_H__
#define __CPAINT_H__

#include "_CPaint.h"
#include "graphics/Paint.h"


CarClass(CPaint), public Paint
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Reset();

    CARAPI Set(
        /* [in] */ IPaint* src);

    CARAPI SetCompatibilityScaling(
        /* [in] */ Float factor);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI IsAntiAlias(
        /* [out] */ Boolean* isAntiAlias);

    CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    CARAPI IsDither(
        /* [out] */ Boolean* isDither);

    CARAPI SetDither(
        /* [in] */ Boolean dither);

    CARAPI IsLinearText(
        /* [out] */ Boolean* isLinearText);

    CARAPI SetLinearText(
        /* [in] */ Boolean linearText);

    CARAPI IsSubpixelText(
        /* [out] */ Boolean* isSubpixelText);

    CARAPI SetSubpixelText(
        /* [in] */ Boolean subpixelText);

    CARAPI IsUnderlineText(
        /* [out] */ Boolean* isUnderlineText);

    CARAPI SetUnderlineText(
        /* [in] */ Boolean underlineText);

    CARAPI IsStrikeThruText(
        /* [out] */ Boolean* isStrikeThruText);

    CARAPI SetStrikeThruText(
        /* [in] */ Boolean strikeThruText);

    CARAPI IsFakeBoldText(
        /* [out] */ Boolean* isFakeBoldText);

    CARAPI SetFakeBoldText(
        /* [in] */ Boolean fakeBoldText);

    CARAPI IsFilterBitmap(
        /* [out] */ Boolean* isFilterBitmap);

    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    CARAPI GetStyle(
        /* [out] */ PaintStyle* style);

    CARAPI SetStyle(
        /* [in] */ PaintStyle style);

    CARAPI GetColor(
        /* [out] */ Int32* color);

    CARAPI SetColor(
        /* [in] */ Int32 color);

    CARAPI GetAlpha(
        /* [out] */ Int32* alpha);

    CARAPI SetAlpha(
        /* [in] */ Int32 a);

    CARAPI SetARGB(
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    CARAPI GetStrokeWidth(
        /* [out] */ Float* width);

    CARAPI SetStrokeWidth(
        /* [in] */ Float width);

    CARAPI GetStrokeMiter(
        /* [out] */ Float* strokeMiter);

    CARAPI SetStrokeMiter(
        /* [in] */ Float miter);

    CARAPI GetStrokeCap(
        /* [out] */ PaintCap* cap);

    CARAPI SetStrokeCap(
        /* [in] */ PaintCap cap);

    CARAPI GetStrokeJoin(
        /* [out] */ PaintJoin* join);

    CARAPI SetStrokeJoin(
        /* [in] */ PaintJoin join);

    CARAPI GetFillPath(
        /* [in] */ IPath* src,
        /* [in] */ IPath* dst,
        /* [out] */ Boolean* isShouldFilled);

    CARAPI GetShader(
        /* [out] */ IShader** shader);

    CARAPI SetShader(
        /* [in] */ IShader* shader);

    CARAPI GetColorFilter(
        /* [out] */ IColorFilter** filter);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* filter);

    CARAPI GetXfermode(
        /* [out] */ IXfermode** xfermode);

    CARAPI SetXfermode(
        /* [in] */ IXfermode* xfermode);

    CARAPI GetPathEffect(
        /* [out] */ IPathEffect** effect);

    CARAPI SetPathEffect(
        /* [in] */ IPathEffect* effect);

    CARAPI GetMaskFilter(
        /* [out] */ IMaskFilter** maskfilter);

    CARAPI SetMaskFilter(
        /* [in] */ IMaskFilter* maskfilter);

    CARAPI GetTypeface(
        /* [out] */ ITypeface** typeface);

    CARAPI SetTypeface(
        /* [in] */ ITypeface* typeface);

    CARAPI GetRasterizer(
        /* [out] */ IRasterizer** rasterizer);

    CARAPI SetRasterizer(
        /* [in] */ IRasterizer* rasterizer);

    CARAPI SetShadowLayer(
        /* [in] */ Float radius,
        /* [in] */ Float rdx,
        /* [in] */ Float rdy,
        /* [in] */ Int32 color);

    CARAPI ClearShadowLayer();

    CARAPI GetTextAlign(
        /* [out] */ PaintAlign* align);

    CARAPI SetTextAlign(
        /* [in] */ PaintAlign align);

    CARAPI GetTextSize(
        /* [out] */ Float* size);

    CARAPI SetTextSize(
        /* [in] */ Float textSize);

    CARAPI GetTextScaleX(
        /* [out] */ Float* scaleX);

    CARAPI SetTextScaleX(
        /* [in] */ Float scaleX);

    CARAPI GetTextSkewX(
        /* [out] */ Float* skewX);

    CARAPI SetTextSkewX(
        /* [in] */ Float skewX);

    CARAPI Ascent(
        /* [out] */ Float* distance);

    CARAPI Descent(
        /* [out] */ Float* distance);

    CARAPI GetFontMetrics(
        /* [in] */ IPaintFontMetrics* metrics,
        /* [out] */ Float* spacing);

    CARAPI GetFontMetricsEx(
        /* [out] */ IPaintFontMetrics** metrics);

    CARAPI GetFontMetricsInt(
        /* [in] */ IPaintFontMetricsInt* fmi,
        /* [out] */ Int32* spacing);

    CARAPI GetFontMetricsIntEx(
        /* [out] */ IPaintFontMetricsInt** fmi);

    CARAPI GetFontSpacing(
        /* [out] */ Float* spacing);

    CARAPI MeasureText(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [out] */ Float* width);

    CARAPI MeasureTextEx(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Float* width);

    CARAPI MeasureTextEx2(
        /* [in] */ const String& text,
        /* [out] */ Float* width);

    CARAPI MeasureTextEx3(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Float* width);

    CARAPI BreakText(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32* number);

    CARAPI BreakTextEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32* number);

    CARAPI BreakTextEx2(
        /* [in] */ const String& text,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32* number);

    CARAPI GetTextWidths(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* width);

    CARAPI GetTextWidthsEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* width);

    CARAPI GetTextWidthsEx2(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* width);

    CARAPI GetTextWidthsEx3(
        /* [in] */ const String& text,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32* width);

    CARAPI GetTextPath(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath* path);

    CARAPI GetTextPathEx(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath* path);

    CARAPI GetTextBounds(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IRect* bounds);

    CARAPI GetTextBoundsEx(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IRect* bounds);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ IPaint* paint);

private:
};

#endif // __CPAINT_H__
