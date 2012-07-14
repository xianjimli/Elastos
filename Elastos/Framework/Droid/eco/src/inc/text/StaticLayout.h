
#ifndef __STATICLAYOUT_H__
#define __STATICLAYOUT_H__

#include "text/Layout.h"
#include <elastos/Vector.h>

/**
 * StaticLayout is a Layout for text that will not be edited after it
 * is laid out.  Use {@link DynamicLayout} for text that may change.
 * <p>This is used by widgets to control text layout. You should not need
 * to use this class directly unless you are implementing your own widget
 * or custom display object, or would be tempted to call
 * {@link android.graphics.Canvas#drawText(java.lang.CharSequence, Int32, Int32, Float, Float, android.graphics.Paint)
 *  Canvas.drawText()} directly.</p>
 */
class StaticLayout : public Layout
{
public:
    StaticLayout(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    StaticLayout(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize = -1,
        /* [in] */ Int32 ellipsizedWidth = 0);

    StaticLayout(
        /* [in] */ Boolean ellipsize);

    virtual ~StaticLayout();

    CARAPI_(void) Generate(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ Boolean trackpad,
        /* [in] */ Boolean breakOnlyAtSpaces,
        /* [in] */ Float ellipsizedWidth,
        /* [in] */ TextUtilsTruncateAt where);

    /**
     * Runs the unicode bidi algorithm on the first n chars in chs, returning
     * the char dirs in chInfo and the base line direction of the first
     * paragraph.
     *
     * XXX change result from dirs to levels
     *
     * @param dir the direction flag, either DIR_REQUEST_LTR,
     * DIR_REQUEST_RTL, DIR_REQUEST_DEFAULT_LTR, or DIR_REQUEST_DEFAULT_RTL.
     * @param chs the text to examine
     * @param chInfo on input, if hasInfo is TRUE, override and other flags
     * representing out-of-band embedding information. On output, the generated
     * dirs of the text.
     * @param n the length of the text/information in chs and chInfo
     * @param hasInfo TRUE if chInfo has input information, otherwise the
     * input data in chInfo is ignored.
     * @return the resolved direction level of the first paragraph, either
     * DIR_LEFT_TO_RIGHT or DIR_RIGHT_TO_LEFT.
     */
     static CARAPI_(Int32) Bidi(
         /* [in] */ Int32 dir,
         /* [in] */ ArrayOf<Char8>* chs,
         /* [in] */ ArrayOf<Byte>* chInfo,
         /* [in] */ Int32 n,
         /* [in] */ Boolean hasInfo);

    // Override the baseclass so we can directly access our members,
    // rather than relying on member functions.
    // The logic mirrors that of Layout.getLineForVertical
    // FIXME: It may be faster to do a linear search for layouts without many lines.
    CARAPI_(Int32) GetLineForVertical(
        /* [in] */ Int32 vertical);

    CARAPI_(Int32) GetLineCount();

    CARAPI_(Int32) GetLineTop(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetLineDescent(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetLineStart(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetParagraphDirection(
        /* [in] */ Int32 line);

    CARAPI_(Boolean) GetLineContainsTab(
        /* [in] */ Int32 line);

    CARAPI_(AutoPtr<IDirections>) GetLineDirections(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetTopPadding();

    CARAPI_(Int32) GetBottomPadding();

    CARAPI_(Int32) GetEllipsisCount(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetEllipsisStart(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetEllipsizedWidth();

protected:
    StaticLayout();

    CARAPI Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    CARAPI Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize = -1,
        /* [in] */ Int32 ellipsizedWidth = 0);

    CARAPI Init(
        /* [in] */ Boolean ellipsize);

private:
    static CARAPI_(Boolean) IsIdeographic(
        /* [in] */ Char32 c,
        /* [in] */ Boolean includeNonStarters);

    static CARAPI_(Int32) GetFit(
        /* [in] */ ITextPaint* paint,
        /* [in] */ ITextPaint* workPaint,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float wid);

    CARAPI_(Int32) Out(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 above,
        /* [in] */ Int32 below,
        /* [in] */ Int32 top,
        /* [in] */ Int32 bottom,
        /* [in] */ Int32 v,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ ArrayOf<IInterface*>* chooseht,
        /* [in] */ ArrayOf<Int32>* choosehtv,
        /* [in] */ IPaintFontMetricsInt* fm,
        /* [in] */ Boolean tab,
        /* [in] */ Boolean needMultiply,
        /* [in] */ Int32 pstart,
        /* [in] */ ArrayOf<Byte>* chdirs,
        /* [in] */ Int32 dir,
        /* [in] */ Boolean easy,
        /* [in] */ Boolean last,
        /* [in] */ Boolean includepad,
        /* [in] */ Boolean trackpad,
        /* [in] */ ArrayOf<Float>* widths,
        /* [in] */ Int32 widstart,
        /* [in] */ Int32 widoff,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Float ellipsiswidth,
        /* [in] */ Float textwidth,
        /* [in] */ ITextPaint* paint);

    CARAPI_(void) CalculateEllipsis(
        /* [in] */ Int32 linestart,
        /* [in] */ Int32 lineend,
        /* [in] */ ArrayOf<Float>* widths,
        /* [in] */ Int32 widstart,
        /* [in] */ Int32 widoff,
        /* [in] */ Float avail,
        /* [in] */ TextUtilsTruncateAt where,
        /* [in] */ Int32 line,
        /* [in] */ Float textwidth,
        /* [in] */ ITextPaint* paint);

    CARAPI_(void) CommonInit(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

private:
    static const Char32 FIRST_CJK = 0x2E80;

    Int32 mLineCount;
    Int32 mTopPadding, mBottomPadding;
    Int32 mColumns;
    Int32 mEllipsizedWidth;

    static const Int32 COLUMNS_NORMAL = 3;
    static const Int32 COLUMNS_ELLIPSIZE = 5;
    static const Int32 START = 0;
    static const Int32 DIR = START;
    static const Int32 TAB = START;
    static const Int32 TOP = 1;
    static const Int32 DESCENT = 2;
    static const Int32 ELLIPSIS_START = 3;
    static const Int32 ELLIPSIS_COUNT = 4;

    ArrayOf<Int32>* mLines;
    ArrayOf<IDirections*>* mLineDirections;

    static const Int32 START_MASK = 0x1FFFFFFF;
    static const Int32 DIR_MASK   = 0xC0000000;
    static const Int32 DIR_SHIFT  = 30;
    static const Int32 TAB_MASK   = 0x20000000;

    static const Char32 FIRST_RIGHT_TO_LEFT = 0x0590;

    /*
     * These are reused across calls to generate()
     */
    ArrayOf<Byte>* mChdirs;
    ArrayOf<Char8>* mChs;
    ArrayOf<Float>* mWidths;
    AutoPtr<IPaintFontMetricsInt> mFontMetricsInt;
};

#endif // __STATICLAYOUT_H__
