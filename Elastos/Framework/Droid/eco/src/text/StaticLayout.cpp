
#include "text/StaticLayout.h"
#include "graphics/CPaintFontMetricsInt.h"
#include "utils/ArrayUtils.h"
#include "text/TextUtils.h"
#include "text/Styled.h"
#include "text/CTextPaint.h"
#include "text/CDirections.h"
#include <elastos/Character.h>
#include <utils/String8.h>

using namespace Elastos::System;

const Char32 StaticLayout::FIRST_CJK;
const Int32 StaticLayout::COLUMNS_NORMAL;
const Int32 StaticLayout::COLUMNS_ELLIPSIZE;
const Int32 StaticLayout::START;
const Int32 StaticLayout::DIR;
const Int32 StaticLayout::TAB;
const Int32 StaticLayout::TOP;
const Int32 StaticLayout::DESCENT;
const Int32 StaticLayout::ELLIPSIS_START;
const Int32 StaticLayout::ELLIPSIS_COUNT;

const Int32 StaticLayout::START_MASK;
const Int32 StaticLayout::DIR_MASK;
const Int32 StaticLayout::DIR_SHIFT;
const Int32 StaticLayout::TAB_MASK;

const Char32 StaticLayout::FIRST_RIGHT_TO_LEFT;

StaticLayout::StaticLayout()
    : mLines(NULL)
    , mLineDirections(NULL)
    , mChdirs(NULL)
    , mChs(NULL)
    , mWidths(NULL)
{
    AutoPtr<CPaintFontMetricsInt> obj;
    CPaintFontMetricsInt::NewByFriend((CPaintFontMetricsInt**)&obj);
    mFontMetricsInt = (IPaintFontMetricsInt*)obj.Get();
}

StaticLayout::StaticLayout(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : Layout(source, paint, width, align, spacingmult, spacingadd)
{
    Int32 len;
    source->GetLength(&len);
    CommonInit(source, 0, len, paint, width, align, spacingmult,
            spacingadd, includepad, -1, 0);
}

StaticLayout::StaticLayout(
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
    /* [in] */ Int32 ellipsizedWidth)
{
    AutoPtr<ICharSequence> csq;
    if (ellipsize == -1) {
        csq = source;
    }
    else if (ISpanned::Probe(source) != NULL) {
        csq  = (ISpanned*)new SpannedEllipsizer(source);
    }
    else {
        csq = new Ellipsizer(source);
    }
    Layout::Init(csq, paint, outerwidth, align, spacingmult, spacingadd);

    CommonInit(source, bufstart, bufend, paint, outerwidth, align, spacingmult,
            spacingadd, includepad, ellipsize, ellipsizedWidth);
}

StaticLayout::StaticLayout(
    /* [in] */ Boolean ellipsize)
    : Layout(NULL, NULL, 0, NULL, 0, 0)
{
    mColumns = COLUMNS_ELLIPSIZE;
    mLines = ArrayOf<Int32>::Alloc(ArrayUtils::IdealInt32ArraySize(2 * mColumns));
    mLineDirections = ArrayOf<IDirections*>::Alloc(
            ArrayUtils::IdealInt32ArraySize(2 * mColumns));
}

StaticLayout::~StaticLayout()
{
    ArrayOf<Int32>::Free(mLines);

    if (mLineDirections != NULL) {
        for (Int32 i = 0; i < mLineDirections->GetLength(); i++) {
            if ((*mLineDirections)[i] != NULL) {
                (*mLineDirections)[i]->Release();
            }
        }
        ArrayOf<IDirections*>::Free(mLineDirections);
    }

    ArrayOf<Byte>::Free(mChdirs);
    BufferOf<Byte>::Free(mChs);
    ArrayOf<Float>::Free(mWidths);
}

void StaticLayout::Generate(
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
    /* [in] */ TextUtilsTruncateAt where)
{
    mLineCount = 0;

    Int32 v = 0;
    Boolean needMultiply = (spacingmult != 1 || spacingadd != 0);

    AutoPtr<IPaintFontMetricsInt> fm = mFontMetricsInt;
    ArrayOf<Int32>* choosehtv = NULL;

    Int32 end = TextUtils::IndexOf(source, '\n', bufstart, bufend);
    Int32 bufsiz = end >= 0 ? end - bufstart : bufend - bufstart;
    Boolean first = TRUE;

    if (mChdirs == NULL) {
        mChdirs = ArrayOf<Byte>::Alloc(ArrayUtils::IdealByteArraySize(bufsiz + 1));
        mChs = BufferOf<Byte>::Alloc(ArrayUtils::IdealChar8ArraySize(4 * (bufsiz + 1)));
        mWidths = ArrayOf<Float>::Alloc(ArrayUtils::IdealInt32ArraySize((bufsiz + 1) * 2));
    }

    ArrayOf<Byte>* chdirs = mChdirs;
    BufferOf<Byte>* chs = mChs;
    ArrayOf<Float>* widths = mWidths;

//    AlteredCharSequence alter = NULL;
    AutoPtr<ISpanned> spanned;

    if (ISpanned::Probe(source) != NULL) {
        spanned = ISpanned::Probe(source);
    }

    Int32 DEFAULT_DIR = DIR_LEFT_TO_RIGHT; // XXX

    for (Int32 start = bufstart; start <= bufend; start = end) {
        if (first) {
            first = FALSE;
        }
        else {
            end = TextUtils::IndexOf(source, '\n', start, bufend);
        }

        if (end < 0) {
            end = bufend;
        }
        else {
            end++;
        }

        Int32 firstWidthLineCount = 1;
        Int32 firstwidth = outerwidth;
        Int32 restwidth = outerwidth;

//        LineHeightSpan[] chooseht = NULL;
        ArrayOf<IInterface*>* chooseht = NULL;

        if (spanned != NULL) {
            ArrayOf<IInterface*>* sp;

            spanned->GetSpans(start, end, EIID_ILeadingMarginSpan, &sp);
            for (Int32 i = 0; i < sp->GetLength(); i++) {
                Int32 val;
                ILeadingMarginSpan* lms = ILeadingMarginSpan::Probe((*sp)[i]);
                lms->GetLeadingMargin(TRUE, &val);
                firstwidth -= val;
                lms->GetLeadingMargin(FALSE, &val);
                restwidth -= val;
                if (ILeadingMarginSpan2::Probe(lms) != NULL) {
                    ILeadingMarginSpan2::Probe(lms)->GetLeadingMarginLineCount(
                            &firstWidthLineCount);
                }
                lms->Release();
            }
            ArrayOf<IInterface*>::Free(sp);

            spanned->GetSpans(start, end, EIID_ILineHeightSpan, &chooseht);

            if (chooseht->GetLength() != 0) {
                if (choosehtv == NULL ||
                    choosehtv->GetLength() < chooseht->GetLength()) {
                        ArrayOf<Int32>::Free(choosehtv);
                        choosehtv = ArrayOf<Int32>::Alloc(
                                ArrayUtils::IdealInt32ArraySize(chooseht->GetLength()));
                }

                for (Int32 i = 0; i < chooseht->GetLength(); i++) {
                    Int32 o;
                    spanned->GetSpanStart((*chooseht)[i], &o);

                    if (o < start) {
                        // starts in this layout, before the
                        // current paragraph

                        (*choosehtv)[i] = GetLineTop(GetLineForOffset(o));
                    } else {
                        // starts in this paragraph

                        (*choosehtv)[i] = v;
                    }
                }
            }
        }

        if (end - start > chdirs->GetLength()) {
            chdirs = ArrayOf<Byte>::Alloc(ArrayUtils::IdealByteArraySize(end - start));
            ArrayOf<Byte>::Free(mChdirs);
            mChdirs = chdirs;
        }
        if (end - start > chs->GetCapacity()) {
            chs = BufferOf<Byte>::Alloc(ArrayUtils::IdealChar8ArraySize(4 * (end - start)));
            BufferOf<Byte>::Free(mChs);
            mChs = chs;
        }
        if ((end - start) * 2 > widths->GetLength()) {
            widths = ArrayOf<Float>::Alloc(ArrayUtils::IdealInt32ArraySize((end - start) * 2));
            ArrayOf<Float>::Free(mWidths);
            mWidths = widths;
        }

        TextUtils::GetChars(source, start, end, chs, 0);
        const Int32 N = end - start;

        Boolean easy = TRUE;
        Boolean altered = FALSE;
        Int32 dir = DEFAULT_DIR; // XXX

        for (Int32 i = 0; i < N; i++) {
            //todo: the implementation is not inefficient;
            Char32 ch = String((char*)chs->GetPayload()).GetChar(i);
            if (ch >= FIRST_RIGHT_TO_LEFT) {
                easy = FALSE;
                break;
            }
        }


        // Ensure that none of the underlying characters are treated
        // as viable breakpoints, and that the entire run gets the
        // same bidi direction.

        if (ISpanned::Probe(source) != NULL) {
            ISpanned* sp = ISpanned::Probe(source);
            ArrayOf<IInterface*>* spans;
            sp->GetSpans(start, end, EIID_IReplacementSpan, &spans);

            for (Int32 y = 0; y < spans->GetLength(); y++) {
                Int32 a, b;
                sp->GetSpanStart((*spans)[y], &a);
                sp->GetSpanEnd((*spans)[y], &b);

                for (Int32 x = a; x < b; x++) {
//                    chs[x - start] = '\uFFFC';
                }
                (*spans)[y]->Release();
            }
            ArrayOf<IInterface*>::Free(spans);
        }


        if (!easy) {
            // XXX put override flags, etc. into chdirs
            dir = Bidi(dir, chs, chdirs, N, FALSE);

            // Do mirroring for right-to-left segments

            for (Int32 i = 0; i < N; i++) {
                if ((*chdirs)[i] == Character::DIRECTIONALITY_RIGHT_TO_LEFT) {
                    Int32 j;

                    for (j = i; j < N; j++) {
                        if ((*chdirs)[j] !=
                            Character::DIRECTIONALITY_RIGHT_TO_LEFT) {
                            break;
                        }
                    }

//                    if (AndroidCharacter.mirror(chs, i, j - i)) {
//                        altered = TRUE;
//                    }

                    i = j - 1;
                }
            }
        }

        AutoPtr<ICharSequence> sub;

        if (altered) {
//            if (alter == NULL) {
//                alter = AlteredCharSequence.make(source, chs, start, end);
//            }
//            else {
//                alter.update(chs, start, end);
//            }
//
//            sub = alter;
        }
        else {
            sub = source;
        }

        Int32 width = firstwidth;

        Float w = 0;
        Int32 here = start;

        Int32 ok = start;
        Float okwidth = w;
        Int32 okascent = 0, okdescent = 0, oktop = 0, okbottom = 0;

        Int32 fit = start;
        Float fitwidth = w;
        Int32 fitascent = 0, fitdescent = 0, fittop = 0, fitbottom = 0;

        Boolean tab = FALSE;

        Int32 next;
        for (Int32 i = start; i < end; i = next) {
            if (spanned == NULL) {
                next = end;
            }
            else {
                spanned->NextSpanTransition(i, end,
                        EIID_IMetricAffectingSpan, &next);
            }

            if (spanned == NULL) {
                Int32 ret;
                paint->GetTextWidthsEx(sub, i, next, widths, &ret);
                for (Int32 j = 0; j < next - i; j++) {
                    (*widths)[end - start + (i - start) + j] = (*widths)[j];
                }

                paint->GetFontMetricsInt(fm, &ret);
            }
            else {
                ((CTextPaint*)mWorkPaint.Get())->mBaselineShift = 0;

                Styled::GetTextWidths(paint, mWorkPaint,
                        spanned, i, next, widths, fm);
                for (Int32 j = 0; j < next - i; j++) {
                    (*widths)[end - start + (i - start) + j] = (*widths)[j];
                }

                Int32 shift = ((CTextPaint*)mWorkPaint.Get())->mBaselineShift;
                if (shift < 0) {
                    Int32 val;
                    fm->GetAscent(&val);
                    fm->SetAscent(val + shift);
                    fm->GetTop(&val);
                    fm->SetTop(val + shift);
                } else {
                    Int32 val;
                    fm->GetDescent(&val);
                    fm->SetDescent(val + shift);
                    fm->GetBottom(&val);
                    fm->SetBottom(val + shift);
                }
            }

            Int32 fmtop, fmbottom, fmascent, fmdescent;
            fm->GetTop(&fmtop);
            fm->GetBottom(&fmbottom);
            fm->GetAscent(&fmascent);
            fm->GetDescent(&fmdescent);

//            if (FALSE) {
//                StringBuilder sb = new StringBuilder();
//                for (Int32 j = i; j < next; j++) {
//                    sb.append(widths[j - start + (end - start)]);
//                    sb.append(' ');
//                }
//
//                Log.e("text", sb.toString());
//            }

            String str((const char*)chs->GetPayload());
            for (Int32 j = i; j < next; j++) {
                Char32 c = str.GetChar(j - start);
                Float before = w;

                if (c == '\n') {
                    ;
                }
                else if (c == '\t') {
                    w = Layout::NextTab(sub, start, end, w, NULL);
                    tab = TRUE;
                }
                else if (c >= 0xD800 && c <= 0xDFFF && j + 1 < next) {
//                    Int32 emoji = Character.codePointAt(chs, j - start);
//
//                    if (emoji >= MIN_EMOJI && emoji <= MAX_EMOJI) {
//                        Bitmap bm = EMOJI_FACTORY.
//                            getBitmapFromAndroidPua(emoji);
//
//                        if (bm != NULL) {
//                            Paint whichPaint;
//
//                            if (spanned == NULL) {
//                                whichPaint = paint;
//                            }
//                            else {
//                                whichPaint = mWorkPaint;
//                            }
//
//                            Float wid = (Float) bm.getWidth() *
//                                -whichPaint.ascent() /
//                                bm.getHeight();
//
//                            w += wid;
//                            tab = TRUE;
//                            j++;
//                        }
//                        else {
//                            w += widths[j - start + (end - start)];
//                        }
//                    }
//                    else {
//                        w += widths[j - start + (end - start)];
//                    }
                }
                else {
                    w += (*widths)[j - start + (end - start)];
                }

                // Log.e("text", "was " + before + " now " + w + " after " + c + " within " + width);

                if (w <= width) {
                    fitwidth = w;
                    fit = j + 1;

                    if (fmtop < fittop) {
                        fittop = fmtop;
                    }
                    if (fmascent < fitascent) {
                        fitascent = fmascent;
                    }
                    if (fmdescent > fitdescent) {
                        fitdescent = fmdescent;
                    }
                    if (fmbottom > fitbottom) {
                        fitbottom = fmbottom;
                    }

                    /*
                    * From the Unicode Line Breaking Algorithm:
                    * (at least approximately)
                    *
                    * .,:; are class IS: breakpoints
                    *      except when adjacent to digits
                    * /    is class SY: a breakpoint
                    *      except when followed by a digit.
                    * -    is class HY: a breakpoint
                    *      except when followed by a digit.
                    *
                    * Ideographs are class ID: breakpoints when adjacent,
                    * except for NS (non-starters), which can be broken
                    * after but not before.
                    */

//                    if (c == ' ' || c == '\t' ||
//                        ((c == '.'  || c == ',' || c == ':' || c == ';') &&
//                        (j - 1 < here || !Character.isDigit(chs[j - 1 - start])) &&
//                        (j + 1 >= next || !Character.isDigit(chs[j + 1 - start]))) ||
//                        ((c == '/' || c == '-') &&
//                        (j + 1 >= next || !Character.isDigit(chs[j + 1 - start]))) ||
//                        (c >= FIRST_CJK && isIdeographic(c, TRUE) &&
//                        j + 1 < next && isIdeographic(chs[j + 1 - start], FALSE))) {
//                            okwidth = w;
//                            ok = j + 1;
//
//                            if (fittop < oktop) {
//                                oktop = fittop;
//                            }
//                            if (fitascent < okascent) {
//                                okascent = fitascent;
//                            }
//                            if (fitdescent > okdescent) {
//                                okdescent = fitdescent;
//                            }
//                            if (fitbottom > okbottom) {
//                                okbottom = fitbottom;
//                            }
//                    }
                }
                else if (breakOnlyAtSpaces) {
                    if (ok != here) {
                        // Log.e("text", "output ok " + here + " to " +ok);

                        while (ok < next && str.GetChar(ok - start) == ' ') {
                            ok++;
                        }

                        v = Out(source,
                            here, ok,
                            okascent, okdescent, oktop, okbottom,
                            v,
                            spacingmult, spacingadd, chooseht,
                            choosehtv, fm, tab,
                            needMultiply, start, chdirs, dir, easy,
                            ok == bufend, includepad, trackpad,
                            widths, start, end - start,
                            where, ellipsizedWidth, okwidth,
                            paint);

                        here = ok;
                    }
                    else {
                        // Act like it fit even though it didn't.

                        fitwidth = w;
                        fit = j + 1;

                        if (fmtop < fittop) {
                            fittop = fmtop;
                        }
                        if (fmascent < fitascent) {
                            fitascent = fmascent;
                        }
                        if (fmdescent > fitdescent) {
                            fitdescent = fmdescent;
                        }
                        if (fmbottom > fitbottom) {
                            fitbottom = fmbottom;
                        }
                    }
                }
                else {
                    if (ok != here) {
                        // Log.e("text", "output ok " + here + " to " +ok);

                        while (ok < next && str.GetChar(ok - start) == ' ') {
                            ok++;
                        }

                        v = Out(source,
                            here, ok,
                            okascent, okdescent, oktop, okbottom,
                            v,
                            spacingmult, spacingadd, chooseht,
                            choosehtv, fm, tab,
                            needMultiply, start, chdirs, dir, easy,
                            ok == bufend, includepad, trackpad,
                            widths, start, end - start,
                            where, ellipsizedWidth, okwidth,
                            paint);

                        here = ok;
                    }
                    else if (fit != here) {
                        // Log.e("text", "output fit " + here + " to " +fit);
                        v = Out(source,
                            here, fit,
                            fitascent, fitdescent,
                            fittop, fitbottom,
                            v,
                            spacingmult, spacingadd, chooseht,
                            choosehtv, fm, tab,
                            needMultiply, start, chdirs, dir, easy,
                            fit == bufend, includepad, trackpad,
                            widths, start, end - start,
                            where, ellipsizedWidth, fitwidth,
                            paint);

                        here = fit;
                    }
                    else {
                        // Log.e("text", "output one " + here + " to " +(here + 1));
                        MeasureText(paint, mWorkPaint,
                            source, here, here + 1, fm, tab,
                            NULL);

                        Int32 fmtop_, fmbottom_, fmascent_, fmdescent_;
                        fm->GetTop(&fmtop_);
                        fm->GetBottom(&fmbottom_);
                        fm->GetAscent(&fmascent_);
                        fm->GetDescent(&fmdescent_);
                        v = Out(source,
                            here, here + 1,
                            fmascent_, fmdescent_,
                            fmtop_, fmbottom_,
                            v,
                            spacingmult, spacingadd, chooseht,
                            choosehtv, fm, tab,
                            needMultiply, start, chdirs, dir, easy,
                            here + 1 == bufend, includepad,
                            trackpad,
                            widths, start, end - start,
                            where, ellipsizedWidth,
                            (*widths)[here - start], paint);

                        here = here + 1;
                    }

                    if (here < i) {
                        j = next = here; // must remeasure
                    }
                    else {
                        j = here - 1;    // continue looping
                    }

                    ok = fit = here;
                    w = 0;
                    fitascent = fitdescent = fittop = fitbottom = 0;
                    okascent = okdescent = oktop = okbottom = 0;

                    if (--firstWidthLineCount <= 0) {
                        width = restwidth;
                    }
                }
            }
        }

        if (end != here) {
            if ((fittop | fitbottom | fitdescent | fitascent) == 0) {
                Int32 ret;
                paint->GetFontMetricsInt(fm, &ret);

                fm->GetTop(&fittop);
                fm->GetBottom(&fitbottom);
                fm->GetAscent(&fitascent);
                fm->GetDescent(&fitdescent);
            }

            // Log.e("text", "output rest " + here + " to " + end);

            v = Out(source,
                here, end, fitascent, fitdescent,
                fittop, fitbottom,
                v,
                spacingmult, spacingadd, chooseht,
                choosehtv, fm, tab,
                needMultiply, start, chdirs, dir, easy,
                end == bufend, includepad, trackpad,
                widths, start, end - start,
                where, ellipsizedWidth, w, paint);
        }

        if (chooseht != NULL) {
            for (Int32 i = 0; i < chooseht->GetLength(); i++) {
                (*chooseht)[i]->Release();
            }
            ArrayOf<IInterface*>::Free(chooseht);
        }

        start = end;

        if (end == bufend) {
            break;
        }
    }

    Char32 ch;
    if (bufend == bufstart || (source->GetCharAt(bufend - 1, &ch), ch == '\n')) {
        // Log.e("text", "output last " + bufend);
        Int32 ret;
        paint->GetFontMetricsInt(fm, &ret);

        Int32 fmtop_, fmbottom_, fmascent_, fmdescent_;
        fm->GetTop(&fmtop_);
        fm->GetBottom(&fmbottom_);
        fm->GetAscent(&fmascent_);
        fm->GetDescent(&fmdescent_);
        v = Out(source,
            bufend, bufend, fmascent_, fmdescent_,
            fmtop_, fmbottom_,
            v,
            spacingmult, spacingadd, NULL,
            NULL, fm, FALSE,
            needMultiply, bufend, chdirs, DEFAULT_DIR, TRUE,
            TRUE, includepad, trackpad,
            widths, bufstart, 0,
            where, ellipsizedWidth, 0, paint);
    }
}

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
Int32 StaticLayout::Bidi(
    /* [in] */ Int32 dir,
    /* [in] */ BufferOf<Byte>* chs,
    /* [in] */ ArrayOf<Byte>* chInfo,
    /* [in] */ Int32 n,
    /* [in] */ Boolean hasInfo)
{
    //AndroidCharacter.getDirectionalities(chs, chInfo, n);

    /*
     * Determine primary paragraph direction if not specified
     */
    if (dir != DIR_REQUEST_LTR && dir != DIR_REQUEST_RTL) {
        // set up default
        dir = dir >= 0 ? DIR_LEFT_TO_RIGHT : DIR_RIGHT_TO_LEFT;
        for (Int32 j = 0; j < n; j++) {
            Int32 d = (*chInfo)[j];

            if (d == Character::DIRECTIONALITY_LEFT_TO_RIGHT) {
                dir = DIR_LEFT_TO_RIGHT;
                break;
            }
            if (d == Character::DIRECTIONALITY_RIGHT_TO_LEFT) {
                dir = DIR_RIGHT_TO_LEFT;
                break;
            }
        }
    }

    const Byte SOR = dir == DIR_LEFT_TO_RIGHT ?
            Character::DIRECTIONALITY_LEFT_TO_RIGHT :
            Character::DIRECTIONALITY_RIGHT_TO_LEFT;

    /*
     * XXX Explicit overrides should go here
     */

    /*
     * Weak type resolution
     */

    // dump(chdirs, n, "initial");

    // W1 non spacing marks
    for (Int32 j = 0; j < n; j++) {
        if ((*chInfo)[j] == Character::NON_SPACING_MARK) {
            if (j == 0) {
                (*chInfo)[j] = SOR;
            }
            else {
                (*chInfo)[j] = (*chInfo)[j - 1];
            }
        }
    }

    // dump(chdirs, n, "W1");

    // W2 european numbers
    Byte cur = SOR;
    for (Int32 j = 0; j < n; j++) {
        Byte d = (*chInfo)[j];

        if (d == Character::DIRECTIONALITY_LEFT_TO_RIGHT ||
            d == Character::DIRECTIONALITY_RIGHT_TO_LEFT ||
            d == Character::DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC) {
            cur = d;
        }
        else if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER) {
            if (cur == Character::DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC) {
                (*chInfo)[j] = Character::DIRECTIONALITY_ARABIC_NUMBER;
            }
        }
    }

    // dump(chdirs, n, "W2");

    // W3 arabic letters
    for (Int32 j = 0; j < n; j++) {
        if ((*chInfo)[j] == Character::DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC) {
            (*chInfo)[j] = Character::DIRECTIONALITY_RIGHT_TO_LEFT;
        }
    }

    // dump(chdirs, n, "W3");

    // W4 single separator between numbers
    for (Int32 j = 1; j < n - 1; j++) {
        Byte d = (*chInfo)[j];
        Byte prev = (*chInfo)[j - 1];
        Byte next = (*chInfo)[j + 1];

        if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER_SEPARATOR) {
            if (prev == Character::DIRECTIONALITY_EUROPEAN_NUMBER &&
                next == Character::DIRECTIONALITY_EUROPEAN_NUMBER) {
                (*chInfo)[j] = Character::DIRECTIONALITY_EUROPEAN_NUMBER;
            }
        }
        else if (d == Character::DIRECTIONALITY_COMMON_NUMBER_SEPARATOR) {
            if (prev == Character::DIRECTIONALITY_EUROPEAN_NUMBER &&
                next == Character::DIRECTIONALITY_EUROPEAN_NUMBER) {
                (*chInfo)[j] = Character::DIRECTIONALITY_EUROPEAN_NUMBER;
            }
            if (prev == Character::DIRECTIONALITY_ARABIC_NUMBER &&
                next == Character::DIRECTIONALITY_ARABIC_NUMBER) {
                (*chInfo)[j] = Character::DIRECTIONALITY_ARABIC_NUMBER;
            }
        }
    }

    // dump(chdirs, n, "W4");

    // W5 european number terminators
    Boolean adjacent = FALSE;
    for (Int32 j = 0; j < n; j++) {
        Byte d = (*chInfo)[j];

        if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER) {
            adjacent = TRUE;
        }
        else if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER_TERMINATOR && adjacent) {
            (*chInfo)[j] = Character::DIRECTIONALITY_EUROPEAN_NUMBER;
        }
        else {
            adjacent = FALSE;
        }
    }

    //dump(chdirs, n, "W5");

    // W5 european number terminators part 2,
    // W6 separators and terminators
    adjacent = FALSE;
    for (Int32 j = n - 1; j >= 0; j--) {
        Byte d = (*chInfo)[j];

        if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER) {
            adjacent = TRUE;
        }
        else if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER_TERMINATOR) {
            if (adjacent) {
                (*chInfo)[j] = Character::DIRECTIONALITY_EUROPEAN_NUMBER;
            }
            else {
                (*chInfo)[j] = Character::DIRECTIONALITY_OTHER_NEUTRALS;
            }
        }
        else {
            adjacent = FALSE;

            if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER_SEPARATOR ||
                d == Character::DIRECTIONALITY_COMMON_NUMBER_SEPARATOR ||
                d == Character::DIRECTIONALITY_PARAGRAPH_SEPARATOR ||
                d == Character::DIRECTIONALITY_SEGMENT_SEPARATOR) {
                (*chInfo)[j] = Character::DIRECTIONALITY_OTHER_NEUTRALS;
            }
        }
    }

    // dump(chdirs, n, "W6");

    // W7 strong direction of european numbers
    cur = SOR;
    for (Int32 j = 0; j < n; j++) {
        Byte d = (*chInfo)[j];

        if (d == SOR ||
            d == Character::DIRECTIONALITY_LEFT_TO_RIGHT ||
            d == Character::DIRECTIONALITY_RIGHT_TO_LEFT) {
            cur = d;
        }

        if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER) {
            (*chInfo)[j] = cur;
        }
    }

    // dump(chdirs, n, "W7");

    // N1, N2 neutrals
    cur = SOR;
    for (Int32 j = 0; j < n; j++) {
        Byte d = (*chInfo)[j];

        if (d == Character::DIRECTIONALITY_LEFT_TO_RIGHT ||
            d == Character::DIRECTIONALITY_RIGHT_TO_LEFT) {
            cur = d;
        }
        else if (d == Character::DIRECTIONALITY_EUROPEAN_NUMBER ||
            d == Character::DIRECTIONALITY_ARABIC_NUMBER) {
            cur = Character::DIRECTIONALITY_RIGHT_TO_LEFT;
        }
        else {
            Byte dd = SOR;
            Int32 k;

            for (k = j + 1; k < n; k++) {
                dd = (*chInfo)[k];

                if (dd == Character::DIRECTIONALITY_LEFT_TO_RIGHT ||
                    dd == Character::DIRECTIONALITY_RIGHT_TO_LEFT) {
                    break;
                }
                if (dd == Character::DIRECTIONALITY_EUROPEAN_NUMBER ||
                    dd == Character::DIRECTIONALITY_ARABIC_NUMBER) {
                    dd = Character::DIRECTIONALITY_RIGHT_TO_LEFT;
                    break;
                }
            }

            for (Int32 y = j; y < k; y++) {
                if (dd == cur) {
                    (*chInfo)[y] = cur;
                }
                else {
                    (*chInfo)[y] = SOR;
                }
            }

            j = k - 1;
        }
    }

    // dump(chdirs, n, "final");

    // extra: enforce that all tabs and surrogate characters go the
    // primary direction
    // TODO: actually do directions right for surrogates
    String str((const char*)chs->GetPayload());
    for (Int32 j = 0; j < n; j++) {
        Char32 c = str.GetChar(j);

        if (c == '\t' || (c >= 0xD800 && c <= 0xDFFF)) {
            (*chInfo)[j] = SOR;
        }
    }

    return dir;
}

/**
 * Returns TRUE if the specified character is one of those specified
 * as being Ideographic (class ID) by the Unicode Line Breaking Algorithm
 * (http://www.unicode.org/unicode/reports/tr14/), and is therefore OK
 * to break between a pair of.
 *
 * @param includeNonStarters also return TRUE for category NS
 *                           (non-starters), which can be broken
 *                           after but not before.
 */
Boolean StaticLayout::IsIdeographic(
    /* [in] */ Char32 c,
    /* [in] */ Boolean includeNonStarters)
{
    if (c >= 0x2E80 && c <= 0x2FFF) {
        return TRUE; // CJK, KANGXI RADICALS, DESCRIPTION SYMBOLS
    }
    if (c == 0x3000) {
        return TRUE; // IDEOGRAPHIC SPACE
    }
    if (c >= 0x3040 && c <= 0x309F) {
        if (!includeNonStarters) {
            switch (c) {
                case 0x3041: //  # HIRAGANA LETTER SMALL A
                case 0x3043: //  # HIRAGANA LETTER SMALL I
                case 0x3045: //  # HIRAGANA LETTER SMALL U
                case 0x3047: //  # HIRAGANA LETTER SMALL E
                case 0x3049: //  # HIRAGANA LETTER SMALL O
                case 0x3063: //  # HIRAGANA LETTER SMALL TU
                case 0x3083: //  # HIRAGANA LETTER SMALL YA
                case 0x3085: //  # HIRAGANA LETTER SMALL YU
                case 0x3087: //  # HIRAGANA LETTER SMALL YO
                case 0x308E: //  # HIRAGANA LETTER SMALL WA
                case 0x3095: //  # HIRAGANA LETTER SMALL KA
                case 0x3096: //  # HIRAGANA LETTER SMALL KE
                case 0x309B: //  # KATAKANA-HIRAGANA VOICED SOUND MARK
                case 0x309C: //  # KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK
                case 0x309D: //  # HIRAGANA ITERATION MARK
                case 0x309E: //  # HIRAGANA VOICED ITERATION MARK
                    return FALSE;
            }
        }
        return TRUE; // Hiragana (except small characters)
    }
    if (c >= 0x30A0 && c <= 0x30FF) {
        if (!includeNonStarters) {
            switch (c) {
                case 0x30A0: //  # KATAKANA-HIRAGANA DOUBLE HYPHEN
                case 0x30A1: //  # KATAKANA LETTER SMALL A
                case 0x30A3: //  # KATAKANA LETTER SMALL I
                case 0x30A5: //  # KATAKANA LETTER SMALL U
                case 0x30A7: //  # KATAKANA LETTER SMALL E
                case 0x30A9: //  # KATAKANA LETTER SMALL O
                case 0x30C3: //  # KATAKANA LETTER SMALL TU
                case 0x30E3: //  # KATAKANA LETTER SMALL YA
                case 0x30E5: //  # KATAKANA LETTER SMALL YU
                case 0x30E7: //  # KATAKANA LETTER SMALL YO
                case 0x30EE: //  # KATAKANA LETTER SMALL WA
                case 0x30F5: //  # KATAKANA LETTER SMALL KA
                case 0x30F6: //  # KATAKANA LETTER SMALL KE
                case 0x30FB: //  # KATAKANA MIDDLE DOT
                case 0x30FC: //  # KATAKANA-HIRAGANA PROLONGED SOUND MARK
                case 0x30FD: //  # KATAKANA ITERATION MARK
                case 0x30FE: //  # KATAKANA VOICED ITERATION MARK
                    return FALSE;
            }
        }
        return TRUE; // Katakana (except small characters)
    }
    if (c >= 0x3400 && c <= 0x4DB5) {
        return TRUE; // CJK UNIFIED IDEOGRAPHS EXTENSION A
    }
    if (c >= 0x4E00 && c <= 0x9FBB) {
        return TRUE; // CJK UNIFIED IDEOGRAPHS
    }
    if (c >= 0xF900 && c <= 0xFAD9) {
        return TRUE; // CJK COMPATIBILITY IDEOGRAPHS
    }
    if (c >= 0xA000 && c <= 0xA48F) {
        return TRUE; // YI SYLLABLES
    }
    if (c >= 0xA490 && c <= 0xA4CF) {
        return TRUE; // YI RADICALS
    }
    if (c >= 0xFE62 && c <= 0xFE66) {
        return TRUE; // SMALL PLUS SIGN to SMALL EQUALS SIGN
    }
    if (c >= 0xFF10 && c <= 0xFF19) {
        return TRUE; // WIDE DIGITS
    }

    return FALSE;
}

/*
private static void dump(Byte[] data, Int32 count, String label) {
if (FALSE) {
System.out.print(label);

for (Int32 i = 0; i < count; i++)
System.out.print(" " + data[i]);

System.out.println();
}
}
*/

Int32 StaticLayout::GetFit(
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float wid)
{
    Int32 high = end + 1, low = start - 1, guess;

    while (high - low > 1) {
        guess = (high + low) / 2;

        if (MeasureText(paint, workPaint,
            text, start, guess, NULL, TRUE, NULL) > wid) {
            high = guess;
        }
        else {
            low = guess;
        }
    }

    if (low < start) {
        return start;
    }
    else {
        return low;
    }
}

Int32 StaticLayout::Out(
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
    /* [in] */ ITextPaint* paint)
{
    Int32 j = mLineCount;
    Int32 off = j * mColumns;
    Int32 want = off + mColumns + TOP;
    ArrayOf<Int32>* lines = mLines;

    // Log.e("text", "line " + start + " to " + end + (last ? "===" : ""));

    if (want >= lines->GetLength()) {
        Int32 nlen = ArrayUtils::IdealInt32ArraySize(want + 1);
        ArrayOf<Int32>* grow = ArrayOf<Int32>::Alloc(nlen);
        Int32 len = lines->GetLength();
        for (Int32 i = 0; i < len; i++) {
            (*grow)[i] = (*lines)[i];
        }
        ArrayOf<Int32>::Free(mLines);
        mLines = grow;
        lines = grow;

        ArrayOf<IDirections*>* grow2 = ArrayOf<IDirections*>::Alloc(nlen);
        len = mLineDirections->GetLength();
        for (Int32 i = 0; i < len; i++) {
            (*grow2)[i] = (*mLineDirections)[i];
        }
        ArrayOf<IDirections*>::Free(mLineDirections);
        mLineDirections = grow2;
    }

    if (chooseht != NULL) {
        fm->SetAscent(above);
        fm->SetDescent(below);
        fm->SetTop(top);
        fm->SetBottom(bottom);

        for (Int32 i = 0; i < chooseht->GetLength(); i++) {
            if (ILineHeightSpanWithDensity::Probe((*chooseht)[i]) != NULL) {
                ILineHeightSpanWithDensity::Probe((*chooseht)[i])->ChooseHeightEx(
                        text, start, end, (*choosehtv)[i], v, fm, paint);

            } else {
                ILineHeightSpan::Probe((*chooseht)[i])->ChooseHeight(
                        text, start, end, (*choosehtv)[i], v, fm);
            }
        }

        fm->GetAscent(&above);
        fm->GetDescent(&below);
        fm->GetTop(&top);
        fm->GetBottom(&bottom);
    }

    if (j == 0) {
        if (trackpad) {
            mTopPadding = top - above;
        }

        if (includepad) {
            above = top;
        }
    }
    if (last) {
        if (trackpad) {
            mBottomPadding = bottom - below;
        }

        if (includepad) {
            below = bottom;
        }
    }

    Int32 extra;

    if (needMultiply) {
        Double ex = (below - above) * (spacingmult - 1) + spacingadd;
        if (ex >= 0) {
            extra = (Int32)(ex + 0.5);
        }
        else {
            extra = -(Int32)(-ex + 0.5);
        }
    } else {
        extra = 0;
    }

    (*lines)[off + START] = start;
    (*lines)[off + TOP] = v;
    (*lines)[off + DESCENT] = below + extra;

    v += (below - above) + extra;
    (*lines)[off + mColumns + START] = end;
    (*lines)[off + mColumns + TOP] = v;

    if (tab) {
        (*lines)[off + TAB] |= TAB_MASK;
    }

    {
        (*lines)[off + DIR] |= dir << DIR_SHIFT;

        Int32 cur = Character::DIRECTIONALITY_LEFT_TO_RIGHT;
        Int32 count = 0;

        if (!easy) {
            for (Int32 k = start; k < end; k++) {
                if ((*chdirs)[k - pstart] != cur) {
                    count++;
                    cur = (*chdirs)[k - pstart];
                }
            }
        }

        AutoPtr<IDirections> linedirs;

        if (count == 0) {
            linedirs = DIRS_ALL_LEFT_TO_RIGHT;
        }
        else {
            ArrayOf<Int16>* ld = ArrayOf<Int16>::Alloc(count + 1);

            cur = Character::DIRECTIONALITY_LEFT_TO_RIGHT;
            count = 0;
            Int32 here = start;

            for (Int32 k = start; k < end; k++) {
                if ((*chdirs)[k - pstart] != cur) {
                    // XXX check to make sure we don't
                    //     overflow short
                    (*ld)[count++] = (Int16)(k - here);
                    cur = (*chdirs)[k - pstart];
                    here = k;
                }
            }

            (*ld)[count] = (Int16)(end - here);

            if (count == 1 && (*ld)[0] == 0) {
                linedirs = DIRS_ALL_RIGHT_TO_LEFT;
            }
            else {
                CDirections::New(*ld, (IDirections**)&linedirs);
            }

            ArrayOf<Int16>::Free(ld);
        }

        if ((*mLineDirections)[j] != NULL) (*mLineDirections)[j]->Release();
        (*mLineDirections)[j] = linedirs;
        linedirs->AddRef();

        // If ellipsize is in marquee mode, do not apply ellipsis on the first line
        if (ellipsize != -1 && (ellipsize != TextUtilsTruncateAt_MARQUEE || j != 0)) {
            CalculateEllipsis(start, end, widths, widstart, widoff,
                    ellipsiswidth, ellipsize, j, textwidth, paint);
        }
    }

    mLineCount++;
    return v;
}

void StaticLayout::CalculateEllipsis(
    /* [in] */ Int32 linestart,
    /* [in] */ Int32 lineend,
    /* [in] */ ArrayOf<Float>* widths,
    /* [in] */ Int32 widstart,
    /* [in] */ Int32 widoff,
    /* [in] */ Float avail,
    /* [in] */ TextUtilsTruncateAt where,
    /* [in] */ Int32 line,
    /* [in] */ Float textwidth,
    /* [in] */ ITextPaint* paint)
{
    Int32 len = lineend - linestart;

    if (textwidth <= avail) {
        // Everything fits!
        (*mLines)[mColumns * line + ELLIPSIS_START] = 0;
        (*mLines)[mColumns * line + ELLIPSIS_COUNT] = 0;
        return;
    }

    Float ellipsiswid;
    Char16 cc[2] = {0x2026, 0};
    paint->MeasureTextEx2(String(android::String8(cc).string()), &ellipsiswid);
    Int32 ellipsisStart, ellipsisCount;

    if (where == TextUtilsTruncateAt_START) {
        Float sum = 0;
        Int32 i;

        for (i = len; i >= 0; i--) {
            Float w = (*widths)[i - 1 + linestart - widstart + widoff];

            if (w + sum + ellipsiswid > avail) {
                break;
            }

            sum += w;
        }

        ellipsisStart = 0;
        ellipsisCount = i;
    }
    else if (where == TextUtilsTruncateAt_END || where == TextUtilsTruncateAt_MARQUEE) {
        Float sum = 0;
        Int32 i;

        for (i = 0; i < len; i++) {
            Float w = (*widths)[i + linestart - widstart + widoff];

            if (w + sum + ellipsiswid > avail) {
                break;
            }

            sum += w;
        }

        ellipsisStart = i;
        ellipsisCount = len - i;
    }
    else /* where = TextUtils.TruncateAt.MIDDLE */ {
        Float lsum = 0, rsum = 0;
        Int32 left = 0, right = len;

        Float ravail = (avail - ellipsiswid) / 2;
        for (right = len; right >= 0; right--) {
            Float w = (*widths)[right - 1 + linestart - widstart + widoff];

            if (w + rsum > ravail) {
                break;
            }

            rsum += w;
        }

        Float lavail = avail - ellipsiswid - rsum;
        for (left = 0; left < right; left++) {
            Float w = (*widths)[left + linestart - widstart + widoff];

            if (w + lsum > lavail) {
                break;
            }

            lsum += w;
        }

        ellipsisStart = left;
        ellipsisCount = right - left;
    }

    (*mLines)[mColumns * line + ELLIPSIS_START] = ellipsisStart;
    (*mLines)[mColumns * line + ELLIPSIS_COUNT] = ellipsisCount;
}

// Override the baseclass so we can directly access our members,
// rather than relying on member functions.
// The logic mirrors that of Layout.getLineForVertical
// FIXME: It may be faster to do a linear search for layouts without many lines.
Int32 StaticLayout::GetLineForVertical(
    /* [in] */ Int32 vertical)
{
    Int32 high = mLineCount;
    Int32 low = -1;
    Int32 guess;
    while (high - low > 1) {
        guess = (high + low) >> 1;
        if ((*mLines)[mColumns * guess + TOP] > vertical){
            high = guess;
        }
        else {
            low = guess;
        }
    }
    if (low < 0) {
        return 0;
    }
    else {
        return low;
    }
}

Int32 StaticLayout::GetLineCount()
{
    return mLineCount;
}

Int32 StaticLayout::GetLineTop(
    /* [in] */ Int32 line)
{
    return (*mLines)[mColumns * line + TOP];
}

Int32 StaticLayout::GetLineDescent(
    /* [in] */ Int32 line)
{
    return (*mLines)[mColumns * line + DESCENT];
}

Int32 StaticLayout::GetLineStart(
    /* [in] */ Int32 line)
{
    return (*mLines)[mColumns * line + START] & START_MASK;
}

Int32 StaticLayout::GetParagraphDirection(
    /* [in] */ Int32 line)
{
    return (*mLines)[mColumns * line + DIR] >> DIR_SHIFT;
}

Boolean StaticLayout::GetLineContainsTab(
    /* [in] */ Int32 line)
{
    return ((*mLines)[mColumns * line + TAB] & TAB_MASK) != 0;
}

AutoPtr<IDirections> StaticLayout::GetLineDirections(
    /* [in] */ Int32 line)
{
    return (*mLineDirections)[line];
}

Int32 StaticLayout::GetTopPadding()
{
    return mTopPadding;
}

Int32 StaticLayout::GetBottomPadding()
{
    return mBottomPadding;
}

Int32 StaticLayout::GetEllipsisCount(
    /* [in] */ Int32 line)
{
    if (mColumns < COLUMNS_ELLIPSIZE) {
        return 0;
    }

    return (*mLines)[mColumns * line + ELLIPSIS_COUNT];
}

Int32 StaticLayout::GetEllipsisStart(
    /* [in] */ Int32 line)
{
    if (mColumns < COLUMNS_ELLIPSIZE) {
        return 0;
    }

    return (*mLines)[mColumns * line + ELLIPSIS_START];
}

Int32 StaticLayout::GetEllipsizedWidth()
{
    return mEllipsizedWidth;
}

void StaticLayout::CommonInit(
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
    /* [in] */ Int32 ellipsizedWidth)
{
    /*
    * This is annoying, but we can't refer to the layout until
    * superclass construction is finished, and the superclass
    * constructor wants the reference to the display text.
    *
    * This will break if the superclass constructor ever actually
    * cares about the content instead of just holding the reference.
    */
    if (ellipsize != -1) {
        Ellipsizer* e = (Ellipsizer*)GetText().Get();

        e->mLayout = this;
        e->mWidth = ellipsizedWidth;
        e->mMethod = ellipsize;
        mEllipsizedWidth = ellipsizedWidth;

        mColumns = COLUMNS_ELLIPSIZE;
    }
    else {
        mColumns = COLUMNS_NORMAL;
        mEllipsizedWidth = outerwidth;
    }

    mLines = ArrayOf<Int32>::Alloc(ArrayUtils::IdealInt32ArraySize(2 * mColumns));
    mLineDirections = ArrayOf<IDirections*>::Alloc(
            ArrayUtils::IdealInt32ArraySize(2 * mColumns));

    Generate(source, bufstart, bufend, paint, outerwidth, align,
            spacingmult, spacingadd, includepad, includepad,
            ellipsize != -1, ellipsizedWidth, ellipsize);

    mChdirs = NULL;
    mChs = NULL;
    mWidths = NULL;
    mFontMetricsInt = NULL;
}

ECode StaticLayout::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    FAIL_RETURN(Layout::Init(
        source, paint, width, align, spacingmult, spacingadd));

    Int32 len;
    source->GetLength(&len);
    CommonInit(source, 0, len, paint, width, align, spacingmult,
            spacingadd, includepad, -1, 0);

    return NOERROR;
}

ECode StaticLayout::Init(
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
    /* [in] */ Int32 ellipsizedWidth)
{
    AutoPtr<ICharSequence> csq;
    if (ellipsize == -1) {
        csq = source;
    }
    else if (ISpanned::Probe(source) != NULL) {
        csq  = (ISpanned*)new SpannedEllipsizer(source);
    }
    else {
        csq = new Ellipsizer(source);
    }

    FAIL_RETURN(Layout::Init(
        csq, paint, outerwidth, align, spacingmult, spacingadd));

    CommonInit(source, bufstart, bufend, paint, outerwidth, align, spacingmult,
            spacingadd, includepad, ellipsize, ellipsizedWidth);

    return NOERROR;
}

ECode StaticLayout::Init(
    /* [in] */ Boolean ellipsize)
{
    FAIL_RETURN(Layout::Init(NULL, NULL, 0, NULL, 0, 0));

    mColumns = COLUMNS_ELLIPSIZE;
    mLines = ArrayOf<Int32>::Alloc(ArrayUtils::IdealInt32ArraySize(2 * mColumns));
    mLineDirections = ArrayOf<IDirections*>::Alloc(
            ArrayUtils::IdealInt32ArraySize(2 * mColumns));

    return NOERROR;
}
