
#include "text/BoringLayout.h"
#include "text/CBoringLayout.h"
#include "text/TextUtils.h"
#include "text/Styled.h"
#include "text/CBoringLayoutMetrics.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

AutoPtr<CTextPaint> InitsTemp()
{
    AutoPtr<CTextPaint> paint;
    CTextPaint::NewByFriend((CTextPaint**)&paint);
    return paint;
}

const Char32 BoringLayout::FIRST_RIGHT_TO_LEFT = 0x0590;
const AutoPtr<CTextPaint> BoringLayout::sTemp = InitsTemp();
Mutex BoringLayout::sTempLock;

AutoPtr<IBoringLayout> BoringLayout::Make(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad)
{
    AutoPtr<IBoringLayout> layout;
    ASSERT_SUCCEEDED(CBoringLayout::New(source, paint, outerwidth, align,
            spacingmult, spacingadd, metrics, includepad, (IBoringLayout**)&layout));
    return layout;
}

AutoPtr<IBoringLayout> BoringLayout::Make(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    AutoPtr<IBoringLayout> layout;
    ASSERT_SUCCEEDED(CBoringLayout::New(source, paint, outerwidth, align,
            spacingmult, spacingadd, metrics, includepad, ellipsize,
            ellipsizedWidth, (IBoringLayout**)&layout));
    return layout;
}

/**
 * Returns a BoringLayout for the specified text, potentially reusing
 * this one if it is already suitable.  The caller must make sure that
 * no one is still using this Layout.
 */
ECode BoringLayout::ReplaceOrMake(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [out] */ IBoringLayout** layout)
{
    assert(layout != NULL);

    ReplaceWith(source, paint, outerwidth, align, spacingmult,
            spacingadd);

    mEllipsizedWidth = outerwidth;
    mEllipsizedStart = 0;
    mEllipsizedCount = 0;

    Init(source, paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, TRUE);
    *layout = (IBoringLayout*)this->Probe(EIID_IBoringLayout);
    (*layout)->AddRef();
    return NOERROR;
}

/**
 * Returns a BoringLayout for the specified text, potentially reusing
 * this one if it is already suitable.  The caller must make sure that
 * no one is still using this Layout.
 */
ECode BoringLayout::ReplaceOrMake(
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
    /* [out] */ IBoringLayout** layout)
{
    assert(layout != NULL);

    Boolean trust;

    if (ellipsize == -1 || ellipsize == TextUtilsTruncateAt_MARQUEE) {
        ReplaceWith(source, paint, outerwidth, align, spacingmult,
                spacingadd);

        mEllipsizedWidth = outerwidth;
        mEllipsizedStart = 0;
        mEllipsizedCount = 0;
        trust = TRUE;
    }
    else {
        ReplaceWith(TextUtils::Ellipsize(source, paint, ellipsizedWidth,
                ellipsize, TRUE, (IEllipsizeCallback*)this->Probe(EIID_IEllipsizeCallback)),
                paint, outerwidth, align, spacingmult, spacingadd);

        mEllipsizedWidth = ellipsizedWidth;
        trust = FALSE;
    }

    Init(GetText(), paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, trust);
    *layout = (IBoringLayout*)this->Probe(EIID_IBoringLayout);
    (*layout)->AddRef();
    return NOERROR;
}


BoringLayout::BoringLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad)
    : Layout(source, paint, outerwidth, align, spacingmult, spacingadd)
    , mEllipsizedWidth(outerwidth)
    , mEllipsizedStart(0)
    , mEllipsizedCount(0)
{
    Init(source, paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, TRUE);
}

BoringLayout::BoringLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
    /*
     * It is silly to have to call super() and then replaceWith(),
     * but we can't use "this" for the callback until the call to
     * super() finishes.
     */
    : Layout(source, paint, outerwidth, align, spacingmult, spacingadd)
{
    Boolean trust;

    if (ellipsize == -1 || ellipsize == TextUtilsTruncateAt_MARQUEE) {
        mEllipsizedWidth = outerwidth;
        mEllipsizedStart = 0;
        mEllipsizedCount = 0;
        trust = TRUE;
    }
    else {
        //todo: call virtual method Probe() in constructor is not correct,
        //      need to fix it.
//        ReplaceWith(TextUtils::Ellipsize(source, paint, ellipsizedWidth,
//                ellipsize, TRUE, (IEllipsizeCallback*)this->Probe(EIID_IEllipsizeCallback)),
//                paint, outerwidth, align, spacingmult, spacingadd);

        mEllipsizedWidth = ellipsizedWidth;
        trust = FALSE;
    }

    Init(GetText(), paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, trust);
}

/* package */
void BoringLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ Boolean trustWidth)
{
    Int32 spacing;

    IObject* obj = IObject::Probe(source);
    assert(obj != NULL);
    ClassID clsid;
    obj->GetClassID(&clsid);
    if (clsid == ECLSID_CStringWrapper && align == LayoutAlignment_ALIGN_NORMAL) {
        source->ToString(&mDirect);
    }
    else {
        mDirect = String(NULL);
    }

    mPaint = paint;

    Int32 mtbottom, mttop, mtdescent, mtascent, mtwidth;
    metrics->GetBottom(&mtbottom);
    metrics->GetTop(&mttop);
    metrics->GetDescent(&mtdescent);
    metrics->GetAscent(&mtascent);
    metrics->GetWidth(&mtwidth);
    if (includepad) {
        spacing = mtbottom - mttop;
    }
    else {
        spacing = mtdescent - mtascent;
    }

    if (spacingmult != 1 || spacingadd != 0) {
        spacing = (Int32)(spacing * spacingmult + spacingadd + 0.5f);
    }

    mBottom = spacing;

    if (includepad) {
        mDesc = spacing + mttop;
    }
    else {
        mDesc = spacing + mtascent;
    }

    if (trustWidth) {
        mMax = mtwidth;
    }
    else {
        /*
         * If we have ellipsized, we have to actually calculate the
         * width because the width that was passed in was for the
         * full text, not the ellipsized form.
         */
        Mutex::Autolock lock(&sTempLock);

        Int32 len;
        source->GetLength(&len);
        mMax = (Int32)(Math::Ceil(Styled::MeasureText(paint, sTemp,
                source, 0, len, NULL)));
    }

    if (includepad) {
        mTopPadding = mttop - mtascent;
        mBottomPadding = mtbottom - mtdescent;
    }
}

/**
 * Returns null if not boring; the width, ascent, and descent if boring.
 */
AutoPtr<IBoringLayoutMetrics> BoringLayout::IsBoring(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint)
{
    return IsBoring(text, paint, NULL);
}

/**
 * Returns null if not boring; the width, ascent, and descent in the
 * provided Metrics object (or a new one if the provided one was null)
 * if boring.
 */
AutoPtr<IBoringLayoutMetrics> BoringLayout::IsBoring(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ IBoringLayoutMetrics* metrics)
{
    ArrayOf<Char8>* temp;
    TextUtils::Obtain(4 * 500, &temp);
    Int32 len;
    text->GetLength(&len);
    Boolean boring = TRUE;

    for (Int32 i = 0; i < len; i += 500) {
        Int32 j = i + 500;

        if (j > len) {
            j = len;
        }

        TextUtils::GetChars(text, i, j, temp, 0);

        Int32 N = j - i;

        String str((char*)temp->GetPayload());
        for (Int32 a = 0; a < N; a++) {
            Char32 c = str.GetChar(a);

            if (c == '\n' || c == '\t' || c >= FIRST_RIGHT_TO_LEFT) {
                boring = FALSE;
                goto outer;
            }
        }
    }

outer:
    TextUtils::Recycle(&temp);

    if (boring && ISpanned::Probe(text) != NULL) {
        ISpanned* sp = ISpanned::Probe(text);
        ArrayOf<IInterface*>* styles;
        sp->GetSpans(0, len, EIID_IParagraphStyle, &styles);
        if (styles->GetLength() > 0) {
            boring = FALSE;
        }
        //free
        for (Int32 i = 0; i < styles->GetLength(); i++) {
            (*styles)[i]->Release();
        }
        ArrayOf<IInterface*>::Free(styles);
    }

    if (boring) {
        AutoPtr<IBoringLayoutMetrics> fm = metrics;
        if (fm == NULL) {
            CBoringLayoutMetrics::New((IBoringLayoutMetrics**)&fm);
        }

        Int32 wid;

        {
            Mutex::Autolock lock(&sTempLock);

            wid = (Int32)(Math::Ceil(Styled::MeasureText(paint, sTemp,
                    text, 0, len, fm)));
        }
        fm->SetWidth(wid);
        return fm;
    }
    else {
        return NULL;
    }
}

Int32 BoringLayout::GetHeight()
{
    return mBottom;
}

Int32 BoringLayout::GetLineCount()
{
    return 1;
}

Int32 BoringLayout::GetLineTop(
    /* [in] */ Int32 line)
{
    if (line == 0) {
        return 0;
    }
    else {
        return mBottom;
    }
}

Int32 BoringLayout::GetLineDescent(
    /* [in] */ Int32 line)
{
    return mDesc;
}

Int32 BoringLayout::GetLineStart(
    /* [in] */ Int32 line)
{
    if (line == 0) {
        return 0;
    }
    else {
        Int32 len;
        GetText()->GetLength(&len);
        return len;
    }
}

Int32 BoringLayout::GetParagraphDirection(
    /* [in] */ Int32 line)
{
    return DIR_LEFT_TO_RIGHT;
}

Boolean BoringLayout::GetLineContainsTab(
    /* [in] */ Int32 line)
{
    return FALSE;
}

Float BoringLayout::GetLineMax(
    /* [in] */ Int32 line)
{
    return mMax;
}

AutoPtr<IDirections> BoringLayout::GetLineDirections(
    /* [in] */ Int32 line)
{
    return DIRS_ALL_LEFT_TO_RIGHT;
}

Int32 BoringLayout::GetTopPadding()
{
    return mTopPadding;
}

Int32 BoringLayout::GetBottomPadding()
{
    return mBottomPadding;
}

Int32 BoringLayout::GetEllipsisCount(
    /* [in] */ Int32 line)
{
    return mEllipsizedCount;
}

Int32 BoringLayout::GetEllipsisStart(
    /* [in] */ Int32 line)
{
    return mEllipsizedStart;
}

Int32 BoringLayout::GetEllipsizedWidth()
{
    return mEllipsizedWidth;
}

ECode BoringLayout::Draw(
    /* [in] */ ICanvas* c)
{
    return Layout::Draw(c);
}

// Override draw so it will be faster.
ECode BoringLayout::Draw(
    /* [in] */ ICanvas* c,
    /* [in] */ IPath* highlight,
    /* [in] */ IPaint* highlightPaint,
    /* [in] */ Int32 cursorOffset)
{
    if (!mDirect.IsNull() && highlight == NULL) {
        return c->DrawTextInString(mDirect, 0, mBottom - mDesc, mPaint);
    }
    else {
        return Layout::Draw(c, highlight, highlightPaint, cursorOffset);
    }
}

/**
 * Callback for the ellipsizer to report what region it ellipsized.
 */
ECode BoringLayout::Ellipsized(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    mEllipsizedStart = start;
    mEllipsizedCount = end - start;
    return NOERROR;
}

BoringLayout::BoringLayout()
{}

ECode BoringLayout::_Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad)
{
    Layout::Init(source, paint, outerwidth, align, spacingmult, spacingadd);
    mEllipsizedWidth = outerwidth;
    mEllipsizedStart = 0;
    mEllipsizedCount = 0;

    Init(source, paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, TRUE);
    return NOERROR;
}

ECode BoringLayout::_Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 outerwidth,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ IBoringLayoutMetrics* metrics,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    /*
     * It is silly to have to call super() and then replaceWith(),
     * but we can't use "this" for the callback until the call to
     * super() finishes.
     */
    Layout::Init(source, paint, outerwidth, align, spacingmult, spacingadd);

    Boolean trust;

    if (ellipsize == -1 || ellipsize == TextUtilsTruncateAt_MARQUEE) {
        mEllipsizedWidth = outerwidth;
        mEllipsizedStart = 0;
        mEllipsizedCount = 0;
        trust = TRUE;
    }
    else {
        //todo: call virtual method Probe() in constructor is not correct,
        //      need to fix it.
        ReplaceWith(TextUtils::Ellipsize(source, paint, ellipsizedWidth,
                ellipsize, TRUE, (IEllipsizeCallback*)this->Probe(EIID_IEllipsizeCallback)),
                paint, outerwidth, align, spacingmult, spacingadd);

        mEllipsizedWidth = ellipsizedWidth;
        trust = FALSE;
    }

    Init(GetText(), paint, outerwidth, align, spacingmult, spacingadd,
         metrics, includepad, trust);
    return NOERROR;
}
