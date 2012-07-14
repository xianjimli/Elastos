
#ifndef __CTEXTPAINT_H__
#define __CTEXTPAINT_H__

#include "_CTextPaint.h"
#include "graphics/Paint.h"


/**
 * TextPaint is an extension of Paint that leaves room for some extra
 * data used during text measuring and drawing.
 */
CarClass(CTextPaint), public Paint
{
public:
    CTextPaint();

    ~CTextPaint();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Reset();

    CARAPI Set(
        /* [in] */ IPaint * pSrc);

    CARAPI SetCompatibilityScaling(
        /* [in] */ Float factor);

    CARAPI GetFlags(
        /* [out] */ Int32 * pFlags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI IsAntiAlias(
        /* [out] */ Boolean * pIsAntiAlias);

    CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    CARAPI IsDither(
        /* [out] */ Boolean * pIsDither);

    CARAPI SetDither(
        /* [in] */ Boolean dither);

    CARAPI IsLinearText(
        /* [out] */ Boolean * pIsLinearText);

    CARAPI SetLinearText(
        /* [in] */ Boolean linearText);

    CARAPI IsSubpixelText(
        /* [out] */ Boolean * pIsSubpixelText);

    CARAPI SetSubpixelText(
        /* [in] */ Boolean subpixelText);

    CARAPI IsUnderlineText(
        /* [out] */ Boolean * pIsUnderlineText);

    CARAPI SetUnderlineText(
        /* [in] */ Boolean underlineText);

    CARAPI IsStrikeThruText(
        /* [out] */ Boolean * pIsStrikeThruText);

    CARAPI SetStrikeThruText(
        /* [in] */ Boolean strikeThruText);

    CARAPI IsFakeBoldText(
        /* [out] */ Boolean * pIsFakeBoldText);

    CARAPI SetFakeBoldText(
        /* [in] */ Boolean fakeBoldText);

    CARAPI IsFilterBitmap(
        /* [out] */ Boolean * pIsFilterBitmap);

    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    CARAPI GetStyle(
        /* [out] */ PaintStyle * pStyle);

    CARAPI SetStyle(
        /* [in] */ PaintStyle style);

    CARAPI GetColor(
        /* [out] */ Int32 * pColor);

    CARAPI SetColor(
        /* [in] */ Int32 color);

    CARAPI GetAlpha(
        /* [out] */ Int32 * pAlpha);

    CARAPI SetAlpha(
        /* [in] */ Int32 a);

    CARAPI SetARGB(
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    CARAPI GetStrokeWidth(
        /* [out] */ Float * pWidth);

    CARAPI SetStrokeWidth(
        /* [in] */ Float width);

    CARAPI GetStrokeMiter(
        /* [out] */ Float * pStrokeMiter);

    CARAPI SetStrokeMiter(
        /* [in] */ Float miter);

    CARAPI GetStrokeCap(
        /* [out] */ PaintCap * pCap);

    CARAPI SetStrokeCap(
        /* [in] */ PaintCap cap);

    CARAPI GetStrokeJoin(
        /* [out] */ PaintJoin * pJoin);

    CARAPI SetStrokeJoin(
        /* [in] */ PaintJoin join);

    CARAPI GetFillPath(
        /* [in] */ IPath * pSrc,
        /* [in] */ IPath * pDst,
        /* [out] */ Boolean * pIsShouldFilled);

    CARAPI GetShader(
        /* [out] */ IShader ** ppShader);

    CARAPI SetShader(
        /* [in] */ IShader * pShader);

    CARAPI GetColorFilter(
        /* [out] */ IColorFilter ** ppFilter);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter * pFilter);

    CARAPI GetXfermode(
        /* [out] */ IXfermode ** ppXfermode);

    CARAPI SetXfermode(
        /* [in] */ IXfermode * pXfermode);

    CARAPI GetPathEffect(
        /* [out] */ IPathEffect ** ppEffect);

    CARAPI SetPathEffect(
        /* [in] */ IPathEffect * pEffect);

    CARAPI GetMaskFilter(
        /* [out] */ IMaskFilter ** ppMaskfilter);

    CARAPI SetMaskFilter(
        /* [in] */ IMaskFilter * pMaskfilter);

    CARAPI GetTypeface(
        /* [out] */ ITypeface ** ppTypeface);

    CARAPI SetTypeface(
        /* [in] */ ITypeface * pTypeface);

    CARAPI GetRasterizer(
        /* [out] */ IRasterizer ** ppRasterizer);

    CARAPI SetRasterizer(
        /* [in] */ IRasterizer * pRasterizer);

    CARAPI SetShadowLayer(
        /* [in] */ Float radius,
        /* [in] */ Float rdx,
        /* [in] */ Float rdy,
        /* [in] */ Int32 color);

    CARAPI ClearShadowLayer();

    CARAPI GetTextAlign(
        /* [out] */ PaintAlign * pAlign);

    CARAPI SetTextAlign(
        /* [in] */ PaintAlign align);

    CARAPI GetTextSize(
        /* [out] */ Float * pSize);

    CARAPI SetTextSize(
        /* [in] */ Float textSize);

    CARAPI GetTextScaleX(
        /* [out] */ Float * pScaleX);

    CARAPI SetTextScaleX(
        /* [in] */ Float scaleX);

    CARAPI GetTextSkewX(
        /* [out] */ Float * pSkewX);

    CARAPI SetTextSkewX(
        /* [in] */ Float skewX);

    CARAPI Ascent(
        /* [out] */ Float * pDistance);

    CARAPI Descent(
        /* [out] */ Float * pDistance);

    CARAPI GetFontMetrics(
        /* [in] */ IPaintFontMetrics * pMetrics,
        /* [out] */ Float * pSpacing);

    CARAPI GetFontMetricsEx(
        /* [out] */ IPaintFontMetrics ** ppMetrics);

    CARAPI GetFontMetricsInt(
        /* [in] */ IPaintFontMetricsInt * pFmi,
        /* [out] */ Int32 * pSpacing);

    CARAPI GetFontMetricsIntEx(
        /* [out] */ IPaintFontMetricsInt ** ppFmi);

    CARAPI GetFontSpacing(
        /* [out] */ Float * pSpacing);

    CARAPI MeasureText(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [out] */ Float * pWidth);

    CARAPI MeasureTextEx(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Float * pWidth);

    CARAPI MeasureTextEx2(
        /* [in] */ const String& text,
        /* [out] */ Float * pWidth);

    CARAPI MeasureTextEx3(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Float * pWidth);

    CARAPI BreakText(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32 * pNumber);

    CARAPI BreakTextEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32 * pNumber);

    CARAPI BreakTextEx2(
        /* [in] */ const String& text,
        /* [in] */ Boolean measureForwards,
        /* [in] */ Float maxWidth,
        /* [in] */ ArrayOf<Float>* measuredWidth,
        /* [out] */ Int32 * pNumber);

    CARAPI GetTextWidths(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32 * pWidth);

    CARAPI GetTextWidthsEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32 * pWidth);

    CARAPI GetTextWidthsEx2(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32 * pWidth);

    CARAPI GetTextWidthsEx3(
        /* [in] */ const String& text,
        /* [in] */ ArrayOf<Float>* widths,
        /* [out] */ Int32 * pWidth);

    CARAPI GetTextPath(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath * pPath);

    CARAPI GetTextPathEx(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPath * pPath);

    CARAPI GetTextBounds(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IRect * pBounds);

    CARAPI GetTextBoundsEx(
        /* [in] */ const ArrayOf<Char8> & text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IRect * pBounds);

    CARAPI SetEx(
        /* [in] */ ITextPaint * pTp);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ IPaint * pPaint);

public:
    Int32 mBgColor;
    Int32 mBaselineShift;
    Int32 mLinkColor;
    ArrayOf<Int32>* mDrawableState;
    Float mDensity;
};

#endif // __CTEXTPAINT_H__
