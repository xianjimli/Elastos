
#ifndef __BORINGLAYOUT_H__
#define __BORINGLAYOUT_H__

#include "ext/frameworkext.h"
#include "text/Layout.h"
#include "text/CTextPaint.h"
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class BoringLayout : public Layout
{
public:
    BoringLayout(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad);

    BoringLayout(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

    static CARAPI_(AutoPtr<IBoringLayout>) Make(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad);

    static CARAPI_(AutoPtr<IBoringLayout>) Make(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

    /**
     * Returns a BoringLayout for the specified text, potentially reusing
     * this one if it is already suitable.  The caller must make sure that
     * no one is still using this Layout.
     */
    CARAPI ReplaceOrMake(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [out] */ IBoringLayout** layout);

    /**
     * Returns a BoringLayout for the specified text, potentially reusing
     * this one if it is already suitable.  The caller must make sure that
     * no one is still using this Layout.
     */
    CARAPI ReplaceOrMake(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth,
        /* [out] */ IBoringLayout** layout);

    /* package */
    CARAPI_(void) Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ Boolean trustWidth);

    /**
     * Returns null if not boring; the width, ascent, and descent if boring.
     */
    static CARAPI_(AutoPtr<IBoringLayoutMetrics>) IsBoring(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint);

    /**
     * Returns null if not boring; the width, ascent, and descent in the
     * provided Metrics object (or a new one if the provided one was null)
     * if boring.
     */
    static CARAPI_(AutoPtr<IBoringLayoutMetrics>) IsBoring(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* paint,
        /* [in] */ IBoringLayoutMetrics* metrics);

    //@Override
    CARAPI_(Int32) GetHeight();

    //@Override
    CARAPI_(Int32) GetLineCount();

    //@Override
    CARAPI_(Int32) GetLineTop(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetLineDescent(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetLineStart(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetParagraphDirection(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Boolean) GetLineContainsTab(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Float) GetLineMax(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(AutoPtr<IDirections>) GetLineDirections(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetTopPadding();

    CARAPI_(Int32) GetBottomPadding();

    //@Override
    CARAPI_(Int32) GetEllipsisCount(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetEllipsisStart(
        /* [in] */ Int32 line);

    //@Override
    CARAPI_(Int32) GetEllipsizedWidth();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* c);

    // Override draw so it will be faster.
    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* c,
        /* [in] */ IPath* highlight,
        /* [in] */ IPaint* highlightPaint,
        /* [in] */ Int32 cursorOffset);

    /**
     * Callback for the ellipsizer to report what region it ellipsized.
     */
    CARAPI Ellipsized(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

protected:
    BoringLayout();

    CARAPI _Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad);

    CARAPI _Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ IBoringLayoutMetrics* metrics,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

public:
    /* package */
    Int32 mBottom, mDesc;   // for Direct

private:
    static const Char32 FIRST_RIGHT_TO_LEFT;

    String mDirect;
    AutoPtr<IPaint> mPaint;

    Int32 mTopPadding, mBottomPadding;
    Float mMax;
    Int32 mEllipsizedWidth, mEllipsizedStart, mEllipsizedCount;

    static const AutoPtr<CTextPaint> sTemp;
    static Mutex sTempLock;
};

#endif //__BORINGLAYOUT_H__
