
#include "text/Layout.h"
#include "text/CSpannableString.h"
#include "text/CDirections.h"
#include "text/CTextPaint.h"
#include "text/TextUtils.h"
#include "text/Styled.h"
#include "text/method/TextKeyListener.h"
#include "graphics/CRect.h"
#include <elastos/Math.h>
#include <elastos/Character.h>

using namespace Elastos::Core;

Layout::Ellipsizer::Ellipsizer(
    /* [in] */ ICharSequence* s)
    : mText(s)
    , mLayout(NULL)
{
    mText = s;
}

PInterface Layout::Ellipsizer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ICharSequence*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)this;
    }
    else if (riid == EIID_IGetChars) {
        return (IGetChars*)this;
    }

    return NULL;
}

UInt32 Layout::Ellipsizer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Layout::Ellipsizer::Release()
{
    return ElRefBase::Release();
}

ECode Layout::Ellipsizer::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return E_NOT_IMPLEMENTED;
}

ECode Layout::Ellipsizer::GetCharAt(
    /* [in] */ Int32 off,
    /* [out] */ Char32* ch)
{
    VALIDATE_NOT_NULL(ch);

    ArrayOf<Char8>* buf;
    TextUtils::Obtain(5, &buf);
    GetChars(off, off + 1, buf, 0);
    *ch = String((const char*)buf->GetPayload()).GetChar(0);

    TextUtils::Recycle(&buf);
    return NOERROR;
}

ECode Layout::Ellipsizer::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char8>* dest,
    /* [in] */ Int32 destoff)
{
    Int32 line1 = mLayout->GetLineForOffset(start);
    Int32 line2 = mLayout->GetLineForOffset(end);

    TextUtils::GetChars(mText, start, end, dest, destoff);

    for (Int32 i = line1; i <= line2; i++) {
        mLayout->Ellipsize(start, end, i, dest, destoff);
    }
    return NOERROR;
}

ECode Layout::Ellipsizer::GetLength(
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len);

    return mText->GetLength(len);
}

ECode Layout::Ellipsizer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** subcsq)
{
    VALIDATE_NOT_NULL(subcsq);

    ArrayOf<Char8>* buf;
    //todo: 5 * (end - start) is not the accurate size
    TextUtils::Obtain(4 * (end - start) + 1, &buf);
    GetChars(start, end, buf, 0);
    FAIL_RETURN(CStringWrapper::New(
            String((const char*)buf->GetPayload()), subcsq));

    TextUtils::Recycle(&buf);
    return NOERROR;
}

ECode Layout::Ellipsizer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    Int32 len;
    GetLength(&len);
    ArrayOf<Char8>* buf;
    //todo: 5 * len is not the accurate size
    TextUtils::Obtain(4 * len + 1, &buf);
    GetChars(0, len, buf, 0);
    *str = (const char*)buf->GetPayload();

    TextUtils::Recycle(&buf);
    return NOERROR;
}

Layout::SpannedEllipsizer::SpannedEllipsizer(
    /* [in] */ ICharSequence* display)
    : Ellipsizer(display)
    , mSpanned(ISpanned::Probe(display))
{}

PInterface Layout::SpannedEllipsizer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ISpanned) {
        return (ISpanned*)this;
    }

    return Ellipsizer::Probe(riid);
}

UInt32 Layout::SpannedEllipsizer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Layout::SpannedEllipsizer::Release()
{
    return ElRefBase::Release();
}

ECode Layout::SpannedEllipsizer::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return E_NOT_IMPLEMENTED;
}

ECode Layout::SpannedEllipsizer::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char8>* dest,
    /* [in] */ Int32 destoff)
{
    return Ellipsizer::GetChars(start, end, dest, destoff);
}

ECode Layout::SpannedEllipsizer::GetLength(
    /* [out] */ Int32* len)
{
    return Ellipsizer::GetLength(len);
}

ECode Layout::SpannedEllipsizer::GetCharAt(
    /* [in] */ Int32 off,
    /* [out] */ Char32* ch)
{
    return Ellipsizer::GetCharAt(off, ch);
}

ECode Layout::SpannedEllipsizer::ToString(
    /* [out] */ String* str)
{
    return Ellipsizer::ToString(str);
}

ECode Layout::SpannedEllipsizer::GetSpans(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type,
    /* [out, callee] */ ArrayOf<IInterface*>** objs)
{
    return mSpanned->GetSpans(start, end, type, objs);
}

ECode Layout::SpannedEllipsizer::GetSpanStart(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* start)
{
    return mSpanned->GetSpanStart(tag, start);
}

ECode Layout::SpannedEllipsizer::GetSpanEnd(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* end)
{
    return mSpanned->GetSpanEnd(tag, end);
}

ECode Layout::SpannedEllipsizer::GetSpanFlags(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* flags)
{
    return mSpanned->GetSpanFlags(tag, flags);
}

ECode Layout::SpannedEllipsizer::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& type,
    /* [out] */ Int32* offset)
{
    return mSpanned->NextSpanTransition(start, limit, type, offset);
}

ECode Layout::SpannedEllipsizer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** subcsq)
{
    ArrayOf<Char8>* buf;
    //todo: 5 * len is not the accurate size
    TextUtils::Obtain(4 * (end - start) + 1, &buf);
    GetChars(start, end, buf, 0);

    AutoPtr<ICharSequence> tmp;
    CStringWrapper::New(String((char*)buf->GetPayload()),
            (ICharSequence**)&tmp);
    AutoPtr<ISpannable> ret;
    FAIL_RETURN(CSpannableString::New(tmp, (ISpannable**)&ret));
    InterfaceID id = EIID_IInterface;
    TextUtils::CopySpansFrom(mSpanned, start, end, &id, ret, 0);

    TextUtils::Recycle(&buf);
    *subcsq = (ICharSequence*)ret.Get();
    return NOERROR;
}


ArrayOf<IParagraphStyle*>* InitNO_PARA_SPANS()
{
    ArrayOf<IParagraphStyle*>* tmp = ArrayOf<IParagraphStyle*>::Alloc(1);
    (*tmp)[0] = NULL;
    return tmp;
}

AutoPtr<CRect> InitsTempRect()
{
    AutoPtr<CRect> tmp;
    CRect::NewByFriend((CRect**)&tmp);
    return tmp;
}

AutoPtr<IDirections> InitDIRS_ALL_LEFT_TO_RIGHT()
{
    AutoPtr<IDirections> tmp;
    ArrayOf_<Int16, 1> val;
    val[0] = 32767;
    AutoPtr<CDirections> obj;
    CDirections::NewByFriend(val, (CDirections**)&obj);
    tmp = (IDirections*)obj.Get();
    return tmp;
}

AutoPtr<IDirections> InitDIRS_ALL_RIGHT_TO_LEFT()
{
    AutoPtr<IDirections> tmp;
    ArrayOf_<Int16, 2> val;
    val[0] = 0;
    val[1] = 32767;
    AutoPtr<CDirections> obj;
    CDirections::NewByFriend(val, (CDirections**)&obj);
    tmp = (IDirections*)obj.Get();
    return tmp;
}

EmojiFactory* InitEMOJI()
{
    EmojiFactory* ef = EmojiFactory::NewAvailableInstance();
    if (ef != NULL) {
        Layout::MIN_EMOJI = ef->GetMinimumAndroidPua();
        Layout::MAX_EMOJI = ef->GetMaximumAndroidPua();
    }
    else {
        Layout::MIN_EMOJI = -1;
        Layout::MAX_EMOJI = -1;
    }
    return ef;
}

const Boolean Layout::DEBUG;
ArrayOf<IParagraphStyle*>* Layout::NO_PARA_SPANS = InitNO_PARA_SPANS();
EmojiFactory* Layout::EMOJI_FACTORY = InitEMOJI();
UInt32 Layout::MIN_EMOJI;
UInt32 Layout::MAX_EMOJI;
AutoPtr<CRect> Layout::sTempRect = InitsTempRect();
Mutex Layout::sTempRectLock;
const Int32 Layout::DIR_LEFT_TO_RIGHT;
const Int32 Layout::DIR_RIGHT_TO_LEFT;
const Int32 Layout::DIR_REQUEST_LTR;
const Int32 Layout::DIR_REQUEST_RTL;
const Int32 Layout::DIR_REQUEST_DEFAULT_LTR;
const Int32 Layout::DIR_REQUEST_DEFAULT_RTL;
AutoPtr<IDirections> Layout::DIRS_ALL_LEFT_TO_RIGHT = InitDIRS_ALL_LEFT_TO_RIGHT();
AutoPtr<IDirections> Layout::DIRS_ALL_RIGHT_TO_LEFT = InitDIRS_ALL_RIGHT_TO_LEFT();
const Int32 Layout::TAB_INCREMENT;

/**
 * Return how wide a layout must be in order to display the
 * specified text with one line per paragraph.
 */
Float Layout::GetDesiredWidth(
    /* [in] */ ICharSequence* source,
    /* [in] */ ITextPaint* paint)
{
    assert(source != NULL);
    Int32 len;
    source->GetLength(&len);
    return GetDesiredWidth(source, 0, len, paint);
}

/**
 * Return how wide a layout must be in order to display the
 * specified text slice with one line per paragraph.
 */
Float Layout::GetDesiredWidth(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ITextPaint* paint)
{
    Float need = 0;
    AutoPtr<ITextPaint> workPaint;
    CTextPaint::New((ITextPaint**)&workPaint);

    Int32 next;
    for (Int32 i = start; i <= end; i = next) {
        next = TextUtils::IndexOf(source, '\n', i, end);

        if (next < 0) {
            next = end;
        }

        // note, omits trailing paragraph Char8
        Float w = MeasureText(paint, workPaint.Get(),
                                source, i, next, NULL, TRUE, NULL);

        if (w > need) {
            need = w;
        }

        next++;
    }

    return need;
}

Layout::Layout()
{}

/**
 * Subclasses of Layout use this constructor to set the display text,
 * width, and other standard properties.
 * @param text the text to render
 * @param paint the default paint for the layout.  Styles can override
 * various attributes of the paint.
 * @param width the wrapping width for the text.
 * @param align whether to left, right, or center the text.  Styles can
 * override the alignment.
 * @param spacingMult factor by which to scale the font size to get the
 * default line spacing
 * @param spacingAdd amount to add to the default line spacing
 */
Layout::Layout(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingMult,
    /* [in] */ Float spacingAdd)
    : mText(text)
    , mPaint(paint)
    , mWidth(width)
    , mAlignment(align)
    , mSpacingMult(spacingMult)
    , mSpacingAdd(spacingAdd)
{
//    if (width < 0)
//        throw new IllegalArgumentException("Layout: " + width + " < 0");
    assert(width >= 0);

    CTextPaint::New((ITextPaint**)&mWorkPaint);
    mSpannedText = text != NULL && text->Probe(EIID_ISpanned) != NULL;
}

/**
 * Replace constructor properties of this Layout with new ones.  Be careful.
 */
/* package */ ECode Layout::ReplaceWith(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd)
{
    if (width < 0) {
//        throw new IllegalArgumentException("Layout: " + width + " < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mText = text;
    mPaint = paint;
    mWidth = width;
    mAlignment = align;
    mSpacingMult = spacingmult;
    mSpacingAdd = spacingadd;
    mSpannedText = text->Probe(EIID_ISpanned) != NULL;
    return NOERROR;
}

/**
 * Draw this Layout on the specified Canvas.
 */
ECode Layout::Draw(
    /* [in] */ ICanvas* c)
{
    return Draw(c, NULL, NULL, 0);
}

/**
 * Draw this Layout on the specified canvas, with the highlight path drawn
 * between the background and the text.
 *
 * @param c the canvas
 * @param highlight the path of the highlight or cursor; can be NULL
 * @param highlightPaint the paint for the highlight
 * @param cursorOffsetVertical the amount to temporarily translate the
 *        canvas while rendering the highlight
 */
ECode Layout::Draw(
    /* [in] */ ICanvas* c,
    /* [in] */ IPath* highlight,
    /* [in] */ IPaint* highlightPaint,
    /* [in] */ Int32 cursorOffsetVertical)
{
    Int32 dtop, dbottom;

    {
        Mutex::Autolock lock(sTempRectLock);
        Boolean isNonEmpty;
        c->GetClipBounds((IRect*)sTempRect.Get(), &isNonEmpty);
        if (!isNonEmpty) {
            return NOERROR;
        }

        dtop = sTempRect->mTop;
        dbottom = sTempRect->mBottom;
    }

    Int32 top = 0;
    Int32 bottom = GetLineTop(GetLineCount());

    if (dtop > top) {
        top = dtop;
    }
    if (dbottom < bottom) {
        bottom = dbottom;
    }

    Int32 first = GetLineForVertical(top);
    Int32 last = GetLineForVertical(bottom);

    Int32 previousLineBottom = GetLineTop(first);
    Int32 previousLineEnd = GetLineStart(first);

    AutoPtr<ITextPaint> paint = mPaint;
    AutoPtr<ICharSequence> buf = mText;
    Int32 width = mWidth;
    Boolean spannedText = mSpannedText;

    ArrayOf<IInterface*>* spans = (ArrayOf<IInterface*>*)NO_PARA_SPANS;
    Int32 spanend = 0;
    Int32 textLength = 0;

    // First, draw LineBackgroundSpans.
    // LineBackgroundSpans know nothing about the alignment or direction of
    // the layout or line.  XXX: Should they?
    if (spannedText) {
        buf->GetLength(&textLength);
        for (Int32 i = first; i <= last; i++) {
            Int32 start = previousLineEnd;
            Int32 end = GetLineStart(i + 1);
            previousLineEnd = end;

            Int32 ltop = previousLineBottom;
            Int32 lbottom = GetLineTop(i + 1);
            previousLineBottom = lbottom;
            Int32 lbaseline = lbottom - GetLineDescent(i);
            if (start >= spanend) {
                AutoPtr<ISpanned> sp = (ISpanned*)buf->Probe(EIID_ISpanned);
                assert(sp != NULL);
                sp->NextSpanTransition(start, textLength,
                                       EIID_ILineBackgroundSpan,
                                       &spanend);

                if (spans != (ArrayOf<IInterface*>*)NO_PARA_SPANS) {
                    FreeArray(spans);
                }
                sp->GetSpans(start, spanend,
                             EIID_ILineBackgroundSpan,
                             &spans);
            }

            for (Int32 n = 0; n < spans->GetLength(); n++) {
                ILineBackgroundSpan* back = ILineBackgroundSpan::Probe((*spans)[n]);

                back->DrawBackground(c, paint, 0, width,
                                     ltop, lbaseline, lbottom,
                                     buf, start, end, i);
            }
        }
        // reset to their original values
        spanend = 0;
        previousLineBottom = GetLineTop(first);
        previousLineEnd = GetLineStart(first);
        if (spans != (ArrayOf<IInterface*>*)NO_PARA_SPANS) {
            FreeArray(spans);
            spans = (ArrayOf<IInterface*>*)NO_PARA_SPANS;
        }
    }
    // There can be a highlight even without spans if we are drawing
    // a non-spanned transformation of a spanned editing buffer.
    if (highlight != NULL) {
        if (cursorOffsetVertical != 0) {
            c->Translate(0, cursorOffsetVertical);
        }

        c->DrawPath(highlight, highlightPaint);

        if (cursorOffsetVertical != 0) {
            c->Translate(0, -cursorOffsetVertical);
        }
    }
    LayoutAlignment align = mAlignment;

    // Next draw the lines, one at a time.
    // the baseline is the top of the following line minus the current
    // line's descent.
    for (Int32 i = first; i <= last; i++) {
        Int32 start = previousLineEnd;

        previousLineEnd = GetLineStart(i + 1);
        Int32 end = GetLineVisibleEnd(i, start, previousLineEnd);

        Int32 ltop = previousLineBottom;
        Int32 lbottom = GetLineTop(i + 1);
        previousLineBottom = lbottom;
        Int32 lbaseline = lbottom - GetLineDescent(i);

        Boolean isFirstParaLine = FALSE;
        if (spannedText) {
            Char32 c;
            if (start == 0 || (buf->GetCharAt(start - 1, &c), c == '\n')) {
                isFirstParaLine = TRUE;
            }
            // New batch of paragraph styles, compute the alignment.
            // Last alignment style wins.
            if (start >= spanend) {
                AutoPtr<ISpanned> sp = (ISpanned*)buf->Probe(EIID_ISpanned);
                assert(sp != NULL);
                sp->NextSpanTransition(start, textLength,
                                       EIID_IParagraphStyle,
                                       &spanend);
                sp->GetSpans(start, spanend,
                             EIID_IParagraphStyle,
                             &spans);

                align = mAlignment;
                for (Int32 n = spans->GetLength() - 1; n >= 0; n--) {
                    if (IAlignmentSpan::Probe((*spans)[n]) != NULL) {
                        IAlignmentSpan::Probe((*spans)[n])->GetAlignment(&align);
                        break;
                    }
                }
            }
        }

        Int32 dir = GetParagraphDirection(i);
        Int32 left = 0;
        Int32 right = mWidth;
        // Draw all leading margin spans.  Adjust left or right according
        // to the paragraph direction of the line.
        if (spannedText) {
            const Int32 length = spans->GetLength();
            for (Int32 n = 0; n < length; n++) {
                if (ILeadingMarginSpan::Probe((*spans)[n]) != NULL) {
                    ILeadingMarginSpan* margin = ILeadingMarginSpan::Probe((*spans)[n]);

                    Int32 offset;
                    if (dir == DIR_RIGHT_TO_LEFT) {
                        margin->DrawLeadingMargin(c, paint, right, dir, ltop,
                                                  lbaseline, lbottom, buf,
                                                  start, end, isFirstParaLine,
                                                  (ILayout*)this->Probe(EIID_ILayout));

                        margin->GetLeadingMargin(isFirstParaLine, &offset);
                        right -= offset;
                    } else {
                        margin->DrawLeadingMargin(c, paint, left, dir, ltop,
                                                  lbaseline, lbottom, buf,
                                                  start, end, isFirstParaLine,
                                                  (ILayout*)this->Probe(EIID_ILayout));

                        Boolean useMargin = isFirstParaLine;
                        if (ILeadingMarginSpan2::Probe(margin) != NULL) {
                            Int32 count;
                            ILeadingMarginSpan2::Probe(margin)->GetLeadingMarginLineCount(&count);
                            useMargin = count > i;
                        }
                        margin->GetLeadingMargin(useMargin, &offset);
                        left += offset;
                    }
                }
            }
        }
        // Adjust the point at which to start rendering depending on the
        // alignment of the paragraph.
        Int32 x;
        if (align == LayoutAlignment_ALIGN_NORMAL) {
            if (dir == DIR_LEFT_TO_RIGHT) {
                x = left;
            } else {
                x = right;
            }
        } else {
            Int32 max = (Int32)GetLineMax(i, spans, FALSE);
            if (align == LayoutAlignment_ALIGN_OPPOSITE) {
                if (dir == DIR_RIGHT_TO_LEFT) {
                    x = left + max;
                } else {
                    x = right - max;
                }
            } else {
                // LayoutAlignment.ALIGN_CENTER
                max = max & ~1;
                Int32 half = (right - left - max) >> 1;
                if (dir == DIR_RIGHT_TO_LEFT) {
                    x = right - half;
                } else {
                    x = left + half;
                }
            }
        }

        AutoPtr<IDirections> directions = GetLineDirections(i);
        Boolean hasTab = GetLineContainsTab(i);
        if (directions == DIRS_ALL_LEFT_TO_RIGHT &&
                !spannedText && !hasTab) {
            if (DEBUG) {
                //Assert::AssertTrue(dir == DIR_LEFT_TO_RIGHT);
                //Assert::AssertNotNull(c);
            }
            // XXX: assumes there's nothing additional to be done
            c->DrawTextInCharSequence(buf, start, end, x, lbaseline, paint.Get());
        }
        else {
            DrawText(c, buf, start, end, dir, directions,
                     x, ltop, lbaseline, lbottom, paint, mWorkPaint,
                     hasTab, spans);
        }
        if (spans != (ArrayOf<IInterface*>*)NO_PARA_SPANS) {
            FreeArray(spans);
        }
    }

    return NOERROR;
}

/**
 * Return the text that is displayed by this Layout.
 */
AutoPtr<ICharSequence> Layout::GetText()
{
    return mText;
}

/**
 * Return the base Paint properties for this layout.
 * Do NOT change the paint, which may result in funny
 * drawing for this layout.
 */
AutoPtr<ITextPaint> Layout::GetPaint()
{
    return mPaint;
}

/**
 * Return the width of this layout.
 */
Int32 Layout::GetWidth()
{
    return mWidth;
}

/**
 * Return the width to which this Layout is ellipsizing, or
 * {@link #getWidth} if it is not doing anything special.
 */
Int32 Layout::GetEllipsizedWidth()
{
    return mWidth;
}

/**
 * Increase the width of this layout to the specified width.
 * Be careful to use this only when you know it is appropriate&mdash;
 * it does not cause the text to reflow to use the full new width.
 */
ECode Layout::IncreaseWidthTo(
    /* [in] */ Int32 wid)
{
    if (wid < mWidth) {
        //throw new RuntimeException("attempted to reduce Layout width");
        return E_RUNTIME_EXCEPTION;
    }

    mWidth = wid;
    return NOERROR;
}

/**
 * Return the total height of this layout.
 */
Int32 Layout::GetHeight()
{
    return GetLineTop(GetLineCount());
}

/**
 * Return the base alignment of this layout.
 */
LayoutAlignment Layout::GetAlignment()
{
    return mAlignment;
}

/**
 * Return what the text height is multiplied by to get the line height.
 */
Float Layout::GetSpacingMultiplier()
{
    return mSpacingMult;
}

/**
 * Return the number of units of leading that are added to each line.
 */
Float Layout::GetSpacingAdd()
{
    return mSpacingAdd;
}

/**
 * Return the baseline for the specified line (0&hellip;getLineCount() - 1)
 * If bounds is not NULL, return the top, left, right, bottom extents
 * of the specified line in it.
 * @param line which line to examine (0..getLineCount() - 1)
 * @param bounds Optional. If not NULL, it returns the extent of the line
 * @return the Y-coordinate of the baseline
 */
Int32 Layout::GetLineBounds(
    /* [in] */ Int32 line,
    /* [in] */ IRect* bounds)
{
    if (bounds != NULL) {
        ((CRect*)bounds)->mLeft = 0;     // ???
        ((CRect*)bounds)->mTop = GetLineTop(line);
        ((CRect*)bounds)->mRight = mWidth;   // ???
        ((CRect*)bounds)->mBottom = GetLineTop(line + 1);
    }
    return GetLineBaseline(line);
}

/**
 * Get the primary horizontal position for the specified text offset.
 * This is the location where a new character would be inserted in
 * the paragraph's primary direction.
 */
Float Layout::GetPrimaryHorizontal(
    /* [in] */ Int32 offset)
{
    return GetHorizontal(offset, FALSE, TRUE);
}

/**
 * Get the secondary horizontal position for the specified text offset.
 * This is the location where a new character would be inserted in
 * the direction other than the paragraph's primary direction.
 */
Float Layout::GetSecondaryHorizontal(
    /* [in] */ Int32 offset)
{
    return GetHorizontal(offset, TRUE, TRUE);
}

Float Layout::GetHorizontal(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean trailing,
    /* [in] */ Boolean alt)
{
    Int32 line = GetLineForOffset(offset);

    return GetHorizontal(offset, trailing, alt, line);
}

Float Layout::GetHorizontal(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean trailing,
    /* [in] */ Boolean alt,
    /* [in] */ Int32 line)
{
    Int32 start = GetLineStart(line);
    Int32 end = GetLineVisibleEnd(line);
    Int32 dir = GetParagraphDirection(line);
    Boolean tab = GetLineContainsTab(line);
    AutoPtr<IDirections> directions = GetLineDirections(line);

    ArrayOf<IInterface*>* tabs = NULL;
    if (tab && ISpanned::Probe(mText.Get()) != NULL) {
        ISpanned::Probe(mText.Get())->GetSpans(
                start, end, EIID_ITabStopSpan, &tabs);
    }

    Float wid = MeasureText(mPaint, mWorkPaint, mText, start, offset, end,
                            dir, directions, trailing, alt, tab, tabs);

    if (offset > end) {
        if (dir == DIR_RIGHT_TO_LEFT) {
            wid -= MeasureText(mPaint, mWorkPaint,
                               mText, end, offset, NULL, tab, tabs);
        }
        else {
            wid += MeasureText(mPaint, mWorkPaint,
                               mText, end, offset, NULL, tab, tabs);
        }
    }

    if (tabs != NULL) {
        FreeArray(tabs);
        tabs = NULL;
    }

    LayoutAlignment align = GetParagraphAlignment(line);
    Int32 left = GetParagraphLeft(line);
    Int32 right = GetParagraphRight(line);

    if (align == LayoutAlignment_ALIGN_NORMAL) {
        if (dir == DIR_RIGHT_TO_LEFT) {
            return right + wid;
        }
        else {
            return left + wid;
        }
    }

    Float max = GetLineMax(line);

    if (align == LayoutAlignment_ALIGN_OPPOSITE) {
        if (dir == DIR_RIGHT_TO_LEFT) {
            return left + max + wid;
        }
        else {
            return right - max + wid;
        }
    }
    else { /* align == LayoutAlignment.ALIGN_CENTER */
        Int32 imax = ((Int32) max) & ~1;

        if (dir == DIR_RIGHT_TO_LEFT) {
            return right - (((right - left) - imax) / 2) + wid;
        }
        else {
            return left + ((right - left) - imax) / 2 + wid;
        }
    }
}

/**
 * Get the leftmost position that should be exposed for horizontal
 * scrolling on the specified line.
 */
Float Layout::GetLineLeft(
    /* [in] */ Int32 line)
{
    Int32 dir = GetParagraphDirection(line);
    LayoutAlignment align = GetParagraphAlignment(line);

    if (align == LayoutAlignment_ALIGN_NORMAL) {
        if (dir == DIR_RIGHT_TO_LEFT) {
            return GetParagraphRight(line) - GetLineMax(line);
        }
        else {
            return 0;
        }
    } else if (align == LayoutAlignment_ALIGN_OPPOSITE) {
        if (dir == DIR_RIGHT_TO_LEFT) {
            return 0;
        }
        else {
            return mWidth - GetLineMax(line);
        }
    } else { /* align == LayoutAlignment.ALIGN_CENTER */
        Int32 left = GetParagraphLeft(line);
        Int32 right = GetParagraphRight(line);
        Int32 max = ((Int32) GetLineMax(line)) & ~1;

        return left + ((right - left) - max) / 2;
    }
}


/**
 * Get the rightmost position that should be exposed for horizontal
 * scrolling on the specified line.
 */
Float Layout::GetLineRight(
    /* [in] */ Int32 line)
{
    Int32 dir = GetParagraphDirection(line);
    LayoutAlignment align = GetParagraphAlignment(line);

    if (align == LayoutAlignment_ALIGN_NORMAL) {
        if (dir == DIR_RIGHT_TO_LEFT) {
            return mWidth;
        }
        else {
            return GetParagraphLeft(line) + GetLineMax(line);
        }
    } else if (align == LayoutAlignment_ALIGN_OPPOSITE) {
        if (dir == DIR_RIGHT_TO_LEFT) {
            return GetLineMax(line);
        }
        else {
            return mWidth;
        }
    }
    else { /* align == LayoutAlignment.ALIGN_CENTER */
        Int32 left = GetParagraphLeft(line);
        Int32 right = GetParagraphRight(line);
        Int32 max = ((Int32) GetLineMax(line)) & ~1;

        return right - ((right - left) - max) / 2;
    }
}

/**
 * Gets the horizontal extent of the specified line, excluding
 * trailing whitespace.
 */
Float Layout::GetLineMax(
    /* [in] */ Int32 line)
{
    return GetLineMax(line, NULL, FALSE);
}

/**
 * Gets the horizontal extent of the specified line, including
 * trailing whitespace.
 */
Float Layout::GetLineWidth(
    /* [in] */ Int32 line)
{
    return GetLineMax(line, NULL, TRUE);
}

Float Layout::GetLineMax(
    /* [in] */ Int32 line,
    /* [in] */ ArrayOf<IInterface*>* tabs,
    /* [in] */ Boolean full)
{
    Int32 start = GetLineStart(line);
    Int32 end;

    if (full) {
        end = GetLineEnd(line);
    }
    else {
        end = GetLineVisibleEnd(line);
    }
    Boolean tab = GetLineContainsTab(line);

    Boolean free = FALSE;
    if (tabs == NULL && tab && ISpanned::Probe(mText.Get()) != NULL) {
        free = TRUE;
        ISpanned::Probe(mText.Get())->GetSpans(start, end, EIID_ITabStopSpan, &tabs);
    }

    Float val = MeasureText(mPaint, mWorkPaint,
                            mText, start, end, NULL, tab, tabs);

    if (free) {
        FreeArray(tabs);
    }

    return val;
}

/**
 * Get the line number corresponding to the specified vertical position.
 * If you ask for a position above 0, you get 0; if you ask for a position
 * below the bottom of the text, you get the last line.
 */
// FIXME: It may be faster to do a linear search for layouts without many lines.
Int32 Layout::GetLineForVertical(
    /* [in] */ Int32 vertical)
{
    Int32 high = GetLineCount(), low = -1, guess;

    while (high - low > 1) {
        guess = (high + low) / 2;

        if (GetLineTop(guess) > vertical) {
            high = guess;
        }
        else {
            low = guess;
        }
    }

    return low < 0? 0 : low;
}

/**
 * Get the line number on which the specified text offset appears.
 * If you ask for a position before 0, you get 0; if you ask for a position
 * beyond the end of the text, you get the last line.
 */
Int32 Layout::GetLineForOffset(
    /* [in] */ Int32 offset)
{
    Int32 high = GetLineCount(), low = -1, guess;

    while (high - low > 1) {
        guess = (high + low) / 2;

        if (GetLineStart(guess) > offset) {
            high = guess;
        }
        else {
            low = guess;
        }
    }

    return low < 0? 0 : low;
}

/**
 * Get the character offset on the specfied line whose position is
 * closest to the specified horizontal position.
 */
Int32 Layout::GetOffsetForHorizontal(
    /* [in] */ Int32 line,
    /* [in] */ Float horiz)
{
    Int32 max = GetLineEnd(line) - 1;
    Int32 min = GetLineStart(line);
    AutoPtr<IDirections> dirs = GetLineDirections(line);

    if (line == GetLineCount() - 1) {
        max++;
    }

    Int32 best = min;
    Float bestdist = Math::Abs(GetPrimaryHorizontal(best) - horiz);

    ArrayOf<Int16>* directions;
    dirs->GetDirections(&directions);
    Int32 here = min;
    for (Int32 i = 0; i < directions->GetLength(); i++) {
        Int32 there = here + (*directions)[i];
        Int32 swap = ((i & 1) == 0) ? 1 : -1;

        if (there > max) {
            there = max;
        }

        Int32 high = there - 1 + 1, low = here + 1 - 1, guess;

        while (high - low > 1) {
            guess = (high + low) / 2;
            Int32 adguess = GetOffsetAtStartOf(guess);

            if (GetPrimaryHorizontal(adguess) * swap >= horiz * swap) {
                high = guess;
            }
            else {
                low = guess;
            }
        }

        if (low < here + 1) {
            low = here + 1;
        }

        if (low < there) {
            low = GetOffsetAtStartOf(low);

            Float dist = Math::Abs(GetPrimaryHorizontal(low) - horiz);

            Int32 aft = TextUtils::GetOffsetAfter(mText, low);
            if (aft < there) {
                Float other = Math::Abs(GetPrimaryHorizontal(aft) - horiz);

                if (other < dist) {
                    dist = other;
                    low = aft;
                }
            }

            if (dist < bestdist) {
                bestdist = dist;
                best = low;
            }
        }

        Float dist = Math::Abs(GetPrimaryHorizontal(here) - horiz);

        if (dist < bestdist) {
            bestdist = dist;
            best = here;
        }

        here = there;
    }
    ArrayOf<Int16>::Free(directions);

    Float dist = Math::Abs(GetPrimaryHorizontal(max) - horiz);

    if (dist < bestdist) {
        bestdist = dist;
        best = max;
    }

    return best;
}

/**
 * Return the text offset after the last character on the specified line.
 */
Int32 Layout::GetLineEnd(
    /* [in] */ Int32 line)
{
    return GetLineStart(line + 1);
}

/**
 * Return the text offset after the last visible character (so whitespace
 * is not counted) on the specified line.
 */
Int32 Layout::GetLineVisibleEnd(
    /* [in] */ Int32 line)
{
    return GetLineVisibleEnd(line, GetLineStart(line), GetLineStart(line + 1));
}

Int32 Layout::GetLineVisibleEnd(
    /* [in] */ Int32 line,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (DEBUG) {
        //Assert.assertTrue(getLineStart(line) == start && getLineStart(line+1) == end);
    }

    if (line == GetLineCount() - 1) {
        return end;
    }

    Char32 ch;
    for (; end > start; end--) {
        mText->GetCharAt(end - 1, &ch);

        if (ch == '\n') {
            return end - 1;
        }

        if (ch != ' ' && ch != '\t') {
            break;
        }

    }

    return end;
}

/**
 * Return the vertical position of the bottom of the specified line.
 */
Int32 Layout::GetLineBottom(
    /* [in] */ Int32 line)
{
    return GetLineTop(line + 1);
}

/**
 * Return the vertical position of the baseline of the specified line.
 */
Int32 Layout::GetLineBaseline(
    /* [in] */ Int32 line)
{
    // getLineTop(line+1) == getLineTop(line)
    return GetLineTop(line + 1) - GetLineDescent(line);
}

/**
 * Get the ascent of the text on the specified line.
 * The return value is negative to match the Paint.ascent() convention.
 */
Int32 Layout::GetLineAscent(
    /* [in] */ Int32 line)
{
    // getLineTop(line+1) - getLineDescent(line) == getLineBaseLine(line)
    return GetLineTop(line) - (GetLineTop(line + 1) - GetLineDescent(line));
}

/**
 * Return the text offset that would be reached by moving left
 * (possibly onto another line) from the specified offset.
 */
Int32 Layout::GetOffsetToLeftOf(
    /* [in] */ Int32 offset)
{
    Int32 line = GetLineForOffset(offset);
    Int32 start = GetLineStart(line);
    Int32 end = GetLineEnd(line);
    AutoPtr<IDirections> dirs = GetLineDirections(line);

    if (line != GetLineCount() - 1) {
        end--;
    }

    Float horiz = GetPrimaryHorizontal(offset);

    Int32 best = offset;
    Float besth = Math::INT32_MIN_VALUE;
    Int32 candidate;

    candidate = TextUtils::GetOffsetBefore(mText, offset);
    if (candidate >= start && candidate <= end) {
        Float h = GetPrimaryHorizontal(candidate);

        if (h < horiz && h > besth) {
            best = candidate;
            besth = h;
        }
    }

    candidate = TextUtils::GetOffsetAfter(mText, offset);
    if (candidate >= start && candidate <= end) {
        Float h = GetPrimaryHorizontal(candidate);

        if (h < horiz && h > besth) {
            best = candidate;
            besth = h;
        }
    }

    ArrayOf<Int16>* directions;
    dirs->GetDirections(&directions);
    Int32 here = start;
    for (Int32 i = 0; i < directions->GetLength(); i++) {
        Int32 there = here + (*directions)[i];
        if (there > end) {
            there = end;
        }

        Float h = GetPrimaryHorizontal(here);

        if (h < horiz && h > besth) {
            best = here;
            besth = h;
        }

        candidate = TextUtils::GetOffsetAfter(mText, here);
        if (candidate >= start && candidate <= end) {
            h = GetPrimaryHorizontal(candidate);

            if (h < horiz && h > besth) {
                best = candidate;
                besth = h;
            }
        }

        candidate = TextUtils::GetOffsetBefore(mText, there);
        if (candidate >= start && candidate <= end) {
            h = GetPrimaryHorizontal(candidate);

            if (h < horiz && h > besth) {
                best = candidate;
                besth = h;
            }
        }

        here = there;
    }
    ArrayOf<Int16>::Free(directions);

    Float h = GetPrimaryHorizontal(end);

    if (h < horiz && h > besth) {
        best = end;
        besth = h;
    }

    if (best != offset) {
        return best;
    }

    Int32 dir = GetParagraphDirection(line);

    if (dir > 0) {
        if (line == 0) {
            return best;
        }
        else {
            return GetOffsetForHorizontal(line - 1, 10000);
        }
    }
    else {
        if (line == GetLineCount() - 1) {
            return best;
        }
        else {
            return GetOffsetForHorizontal(line + 1, 10000);
        }
    }
}

/**
 * Return the text offset that would be reached by moving right
 * (possibly onto another line) from the specified offset.
 */
Int32 Layout::GetOffsetToRightOf(
    /* [in] */ Int32 offset)
{
    Int32 line = GetLineForOffset(offset);
    Int32 start = GetLineStart(line);
    Int32 end = GetLineEnd(line);
    AutoPtr<IDirections> dirs = GetLineDirections(line);

    if (line != GetLineCount() - 1) {
        end--;
    }

    Float horiz = GetPrimaryHorizontal(offset);

    Int32 best = offset;
    Float besth = Math::INT32_MAX_VALUE;
    Int32 candidate;

    candidate = TextUtils::GetOffsetBefore(mText, offset);
    if (candidate >= start && candidate <= end) {
        Float h = GetPrimaryHorizontal(candidate);

        if (h > horiz && h < besth) {
            best = candidate;
            besth = h;
        }
    }

    candidate = TextUtils::GetOffsetAfter(mText, offset);
    if (candidate >= start && candidate <= end) {
        Float h = GetPrimaryHorizontal(candidate);

        if (h > horiz && h < besth) {
            best = candidate;
            besth = h;
        }
    }

    ArrayOf<Int16>* directions;
    dirs->GetDirections(&directions);
    Int32 here = start;
    for (Int32 i = 0; i < directions->GetLength(); i++) {
        Int32 there = here + (*directions)[i];
        if (there > end) {
            there = end;
        }

        Float h = GetPrimaryHorizontal(here);

        if (h > horiz && h < besth) {
            best = here;
            besth = h;
        }

        candidate = TextUtils::GetOffsetAfter(mText, here);
        if (candidate >= start && candidate <= end) {
            h = GetPrimaryHorizontal(candidate);

            if (h > horiz && h < besth) {
                best = candidate;
                besth = h;
            }
        }

        candidate = TextUtils::GetOffsetBefore(mText, there);
        if (candidate >= start && candidate <= end) {
            h = GetPrimaryHorizontal(candidate);

            if (h > horiz && h < besth) {
                best = candidate;
                besth = h;
            }
        }

        here = there;
    }
    ArrayOf<Int16>::Free(directions);

    Float h = GetPrimaryHorizontal(end);

    if (h > horiz && h < besth) {
        best = end;
        besth = h;
    }

    if (best != offset) {
        return best;
    }

    Int32 dir = GetParagraphDirection(line);

    if (dir > 0) {
        if (line == GetLineCount() - 1) {
            return best;
        }
        else {
            return GetOffsetForHorizontal(line + 1, -10000);
        }
    } else {
        if (line == 0) {
            return best;
        }
        else {
            return GetOffsetForHorizontal(line - 1, -10000);
        }
    }
}

Int32 Layout::GetOffsetAtStartOf(
    /* [in] */ Int32 offset)
{
    if (offset == 0) {
        return 0;
    }

    ICharSequence* text = mText;
    Char32 c;
    text->GetCharAt(offset, &c);

    if (c >= 0xDC00/*'\uDC00'*/ && c <= 0xDFFF/*'\uDFFF'*/) {
        Char32 c1;
        text->GetCharAt(offset - 1, &c1);

        if (c1 >= 0xD800/*'\uD800'*/ && c1 <= 0xDBFF/*'\uDBFF'*/) {
            offset -= 1;
        }
    }

    if (mSpannedText) {
//        ReplacementSpan[] spans = ((Spanned) text).getSpans(offset, offset,
//                                                    ReplacementSpan.class);
//
//        for (Int32 i = 0; i < spans.length; i++) {
//            Int32 start = ((Spanned) text).getSpanStart(spans[i]);
//            Int32 end = ((Spanned) text).getSpanEnd(spans[i]);
//
//            if (start < offset && end > offset)
//                offset = start;
//        }
    }

    return offset;
}

/**
 * Fills in the specified Path with a representation of a cursor
 * at the specified offset.  This will often be a vertical line
 * but can be multiple discontinous lines in text with multiple
 * directionalities.
 */
ECode Layout::GetCursorPath(
    /* [in] */ Int32 point,
    /* [in] */ IPath* dest,
    /* [in] */ ICharSequence* editingBuffer)
{
    dest->Reset();

    Int32 line = GetLineForOffset(point);
    Int32 top = GetLineTop(line);
    Int32 bottom = GetLineTop(line+1);

    Float h1 = GetPrimaryHorizontal(point) - 0.5f;
    Float h2 = GetSecondaryHorizontal(point) - 0.5f;

    Int32 caps = TextKeyListener::GetMetaState(editingBuffer,
            KeyEvent_META_SHIFT_ON) |
            TextKeyListener::GetMetaState(editingBuffer,
            MetaKeyKeyListener::META_SELECTING);
    Int32 fn = TextKeyListener::GetMetaState(editingBuffer,
            KeyEvent_META_ALT_ON);
    Int32 dist = 0;

    if (caps != 0 || fn != 0) {
        dist = (bottom - top) >> 2;

        if (fn != 0) {
            top += dist;
        }
        if (caps != 0) {
            bottom -= dist;
        }
    }

    if (h1 < 0.5f) {
        h1 = 0.5f;
    }
    if (h2 < 0.5f) {
        h2 = 0.5f;
    }

    if (h1 == h2) {
        dest->MoveTo(h1, top);
        dest->LineTo(h1, bottom);
    }
    else {
        dest->MoveTo(h1, top);
        dest->LineTo(h1, (top + bottom) >> 1);

        dest->MoveTo(h2, (top + bottom) >> 1);
        dest->LineTo(h2, bottom);
    }

    if (caps == 2) {
        dest->MoveTo(h2, bottom);
        dest->LineTo(h2 - dist, bottom + dist);
        dest->LineTo(h2, bottom);
        dest->LineTo(h2 + dist, bottom + dist);
    }
    else if (caps == 1) {
        dest->MoveTo(h2, bottom);
        dest->LineTo(h2 - dist, bottom + dist);

        dest->MoveTo(h2 - dist, bottom + dist - 0.5f);
        dest->LineTo(h2 + dist, bottom + dist - 0.5f);

        dest->MoveTo(h2 + dist, bottom + dist);
        dest->LineTo(h2, bottom);
    }

    if (fn == 2) {
        dest->MoveTo(h1, top);
        dest->LineTo(h1 - dist, top - dist);
        dest->LineTo(h1, top);
        dest->LineTo(h1 + dist, top - dist);
    }
    else if (fn == 1) {
        dest->MoveTo(h1, top);
        dest->LineTo(h1 - dist, top - dist);

        dest->MoveTo(h1 - dist, top - dist + 0.5f);
        dest->LineTo(h1 + dist, top - dist + 0.5f);

        dest->MoveTo(h1 + dist, top - dist);
        dest->LineTo(h1, top);
    }

    return NOERROR;
}

void Layout::AddSelection(
    /* [in] */ Int32 line,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom,
    /* [in] */ IPath* dest)
{
    Int32 linestart = GetLineStart(line);
    Int32 lineend = GetLineEnd(line);
    AutoPtr<IDirections> dirs = GetLineDirections(line);

    Char32 c;
    if (lineend > linestart && (mText->GetCharAt(lineend - 1, &c), c == '\n')) {
        lineend--;
    }

    ArrayOf<Int16>* directions;
    dirs->GetDirections(&directions);
    Int32 here = linestart;
    for (Int32 i = 0; i < directions->GetLength(); i++) {
        Int32 there = here + (*directions)[i];
        if (there > lineend)
            there = lineend;

        if (start <= there && end >= here) {
            Int32 st = Math::Max(start, here);
            Int32 en = Math::Min(end, there);

            if (st != en) {
                Float h1 = GetHorizontal(st, FALSE, FALSE, line);
                Float h2 = GetHorizontal(en, TRUE, FALSE, line);

                dest->AddRectEx(h1, top, h2, bottom, PathDirection_CW);
            }
        }

        here = there;
    }
    ArrayOf<Int16>::Free(directions);
}

/**
 * Fills in the specified Path with a representation of a highlight
 * between the specified offsets.  This will often be a rectangle
 * or a potentially discontinuous set of rectangles.  If the start
 * and end are the same, the returned path is empty.
 */
ECode Layout::GetSelectionPath(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPath* dest)
{
    dest->Reset();

    if (start == end) {
        return NOERROR;
    }

    if (end < start) {
        Int32 temp = end;
        end = start;
        start = temp;
    }

    Int32 startline = GetLineForOffset(start);
    Int32 endline = GetLineForOffset(end);

    Int32 top = GetLineTop(startline);
    Int32 bottom = GetLineBottom(endline);

    if (startline == endline) {
        AddSelection(startline, start, end, top, bottom, dest);
    }
    else {
        Float width = mWidth;

        AddSelection(startline, start, GetLineEnd(startline),
                        top, GetLineBottom(startline), dest);

        if (GetParagraphDirection(startline) == DIR_RIGHT_TO_LEFT) {
            dest->AddRectEx(GetLineLeft(startline), top,
                            0, GetLineBottom(startline), PathDirection_CW);
        }
        else {
            dest->AddRectEx(GetLineRight(startline), top,
                            width, GetLineBottom(startline), PathDirection_CW);
        }

        for (Int32 i = startline + 1; i < endline; i++) {
            top = GetLineTop(i);
            bottom = GetLineBottom(i);
            dest->AddRectEx(0, top, width, bottom, PathDirection_CW);
        }

        top = GetLineTop(endline);
        bottom = GetLineBottom(endline);

        AddSelection(endline, GetLineStart(endline), end,
                        top, bottom, dest);

        if (GetParagraphDirection(endline) == DIR_RIGHT_TO_LEFT) {
            dest->AddRectEx(width, top, GetLineRight(endline),
                            bottom, PathDirection_CW);
        }
        else {
            dest->AddRectEx(0, top, GetLineLeft(endline),
                            bottom, PathDirection_CW);
        }
    }

    return NOERROR;
}

/**
 * Get the alignment of the specified paragraph, taking into account
 * markup attached to it.
 */
LayoutAlignment Layout::GetParagraphAlignment(
    /* [in] */ Int32 line)
{
    LayoutAlignment align = mAlignment;

    if (mSpannedText) {
        ISpanned* sp = ISpanned::Probe(mText.Get());
        ArrayOf<IInterface*>* spans;
        sp->GetSpans(GetLineStart(line), GetLineEnd(line),
                     EIID_IAlignmentSpan, &spans);

        Int32 spanLength = spans->GetLength();
        if (spanLength > 0) {
            IAlignmentSpan::Probe((*spans)[spanLength - 1])->GetAlignment(&align);
        }
        FreeArray(spans);
    }

    return align;
}

/**
 * Get the left edge of the specified paragraph, inset by left margins.
 */
Int32 Layout::GetParagraphLeft(Int32 line)
{
    Int32 dir = GetParagraphDirection(line);

    Int32 left = 0;

    Boolean par = FALSE;
    Int32 off = GetLineStart(line);
    Char32 c;
    if (off == 0 || (mText->GetCharAt(off - 1, &c), c == '\n')) {
        par = TRUE;
    }

    if (dir == DIR_LEFT_TO_RIGHT) {
        if (mSpannedText) {
            ISpanned* sp = ISpanned::Probe(mText.Get());
            ArrayOf<IInterface*>* spans;
            sp->GetSpans(GetLineStart(line), GetLineEnd(line),
                         EIID_ILeadingMarginSpan, &spans);

            for (Int32 i = 0; i < spans->GetLength(); i++) {
                Boolean margin = par;
                ILeadingMarginSpan* span = ILeadingMarginSpan::Probe((*spans)[i]);
                if (ILeadingMarginSpan2::Probe(span) != NULL) {
                    Int32 count;
                    ILeadingMarginSpan2::Probe(span)->GetLeadingMarginLineCount(&count);
                    margin = count >= line;
                }
                Int32 offset;
                span->GetLeadingMargin(margin, &offset);
                left += offset;
                (*spans)[i]->Release();
            }
            ArrayOf<IInterface*>::Free(spans);
        }
    }

    return left;
}

/**
 * Get the right edge of the specified paragraph, inset by right margins.
 */
Int32 Layout::GetParagraphRight(
    /* [in] */ Int32 line)
{
    Int32 dir = GetParagraphDirection(line);

    Int32 right = mWidth;

    Boolean par = FALSE;
    Int32 off = GetLineStart(line);
    Char32 c;
    if (off == 0 || (mText->GetCharAt(off - 1, &c), c == '\n')) {
        par = TRUE;
    }

    if (dir == DIR_RIGHT_TO_LEFT) {
        if (mSpannedText) {
            ISpanned* sp = ISpanned::Probe(mText.Get());
            ArrayOf<IInterface*>* spans;
            sp->GetSpans(GetLineStart(line), GetLineEnd(line),
                         EIID_ILeadingMarginSpan, &spans);

            for (Int32 i = 0; i < spans->GetLength(); i++) {
                Int32 offset;
                ILeadingMarginSpan::Probe((*spans)[i])->GetLeadingMargin(par, &offset);
                right -= offset;
                (*spans)[i]->Release();
            }
            ArrayOf<IInterface*>::Free(spans);
        }
    }

    return right;
}

void Layout::DrawText(
    /* [in] */ ICanvas* canvas,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 dir,
    /* [in] */ IDirections* directions,
    /* [in] */ Float x,
    /* [in] */ Int32 top,
    /* [in] */ Int32 y,
    /* [in] */ Int32 bottom,
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ Boolean hasTabs,
    /* [in] */ ArrayOf<IInterface*>* parspans)
{
    ArrayOf<Char8>* buf;
    if (!hasTabs) {
        if (directions == DIRS_ALL_LEFT_TO_RIGHT) {
            if (DEBUG) {
//                Assert.assertTrue(DIR_LEFT_TO_RIGHT == dir);
            }
            Styled::DrawText(canvas, text, start, end, dir, FALSE,
                             x, top, y, bottom, paint, workPaint, FALSE);
            return;
        }
        buf = NULL;
    } else {
        TextUtils::Obtain(4 * (end - start) + 1, &buf);
        TextUtils::GetChars(text, start, end, buf, 0);
    }

    Float h = 0;

    ArrayOf<Int16>* dirs;
    directions->GetDirections(&dirs);
    Int32 here = 0;
    for (Int32 i = 0; i < dirs->GetLength(); i++) {
        Int32 there = here + (*dirs)[i];
        if (there > end - start) {
            there = end - start;
        }

        String str(buf != NULL ? (const char*)buf->GetPayload() : "");
        Int32 segstart = here;
        for (Int32 j = hasTabs ? here : there; j <= there; j++) {
            Char32 ch = str.GetChar(j);
            if (j == there || ch == '\t') {
                h += Styled::DrawText(canvas, text,
                                      start + segstart, start + j,
                                      dir, (i & 1) != 0, x + h,
                                      top, y, bottom, paint, workPaint,
                                      start + j != end);

                if (j != there && ch == '\t') {
                    h = dir * NextTab(text, start, end, h * dir, parspans);
                }

                segstart = j + 1;
            }
            else if (hasTabs && ch >= 0xD800 && ch <= 0xDFFF && j + 1 < there) {
                Char32 emoji;
                ASSERT_SUCCEEDED(Character::GetCharAt(*buf, j, &emoji));

                if (emoji >= MIN_EMOJI && emoji <= MAX_EMOJI) {
                    AutoPtr<IBitmap> bm = EMOJI_FACTORY->GetBitmapFromAndroidPua(emoji);

                    if (bm != NULL) {
                        h += Styled::DrawText(canvas, text,
                                              start + segstart, start + j,
                                              dir, (i & 1) != 0, x + h,
                                              top, y, bottom, paint, workPaint,
                                              start + j != end);

                        if (mEmojiRect == NULL) {
                            CRectF::NewByFriend((CRectF**)&mEmojiRect);
                        }

                        workPaint->Set(paint);
                        Styled::MeasureText(paint, workPaint, text,
                                            start + j, start + j + 1,
                                            NULL);

                        Float bitmapHeight, textHeight, scale, width;
                        Int32 val;
                        bm->GetHeight(&val);
                        bitmapHeight = val;
                        workPaint->Ascent(&textHeight);
                        textHeight = -textHeight;
                        scale = textHeight / bitmapHeight;
                        bm->GetWidth(&val);
                        width = val * scale;

                        mEmojiRect->Set(x + h, y - textHeight,
                                        x + h + width, y);

                        canvas->DrawBitmapEx(bm, NULL, (IRectF*)mEmojiRect.Get(), paint);
                        h += width;

                        j++;
                        segstart = j + 1;
                    }
                }
            }
        }

        here = there;
    }
    ArrayOf<Int16>::Free(dirs);

    if (hasTabs) {
        TextUtils::Recycle(&buf);
    }
}

Float Layout::MeasureText(
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 end,
    /* [in] */ Int32 dir,
    /* [in] */ IDirections* directions,
    /* [in] */ Boolean trailing,
    /* [in] */ Boolean alt,
    /* [in] */ Boolean hasTabs,
    /* [in] */ ArrayOf<IInterface*>* tabs)
{
    ArrayOf<Char8>* buf = NULL;

    if (hasTabs) {
        TextUtils::Obtain(4 * (end - start) + 1, &buf);
        TextUtils::GetChars(text, start, end, buf, 0);
    }

    Float h = 0;

    if (alt) {
        if (dir == DIR_RIGHT_TO_LEFT) {
            trailing = !trailing;
        }
    }

    ArrayOf<Int16>* dirs;
    directions->GetDirections(&dirs);
    Int32 here = 0;
    for (Int32 i = 0; i < dirs->GetLength(); i++) {
        if (alt) {
            trailing = !trailing;
        }

        Int32 there = here + (*dirs)[i];
        if (there > end - start) {
            there = end - start;
        }

        String str(buf != NULL ? (const char*)buf->GetPayload() : NULL);
        Int32 segstart = here;
        for (Int32 j = hasTabs ? here : there; j <= there; j++) {
            Char32 codept = 0;
            AutoPtr<IBitmap> bm;

            if (hasTabs && j < there) {
                codept = str.GetChar(j);
            }

            if (codept >= 0xD800 && codept <= 0xDFFF && j + 1 < there) {
                ASSERT_SUCCEEDED(Character::GetCharAt(*buf, j, &codept));

                if (codept >= MIN_EMOJI && codept <= MAX_EMOJI) {
                    bm = EMOJI_FACTORY->GetBitmapFromAndroidPua(codept);
                }
            }

            if (j == there || codept == '\t' || bm != NULL) {
                Float segw;

                if (offset < start + j ||
                    (trailing && offset <= start + j)) {
                    if (dir == DIR_LEFT_TO_RIGHT && (i & 1) == 0) {
                        h += Styled::MeasureText(paint, workPaint, text,
                                                 start + segstart, offset,
                                                 NULL);
                        return h;
                    }

                    if (dir == DIR_RIGHT_TO_LEFT && (i & 1) != 0) {
                        h -= Styled::MeasureText(paint, workPaint, text,
                                                 start + segstart, offset,
                                                 NULL);
                        return h;
                    }
                }

                segw = Styled::MeasureText(paint, workPaint, text,
                                           start + segstart, start + j,
                                           NULL);

                if (offset < start + j ||
                    (trailing && offset <= start + j)) {
                    if (dir == DIR_LEFT_TO_RIGHT) {
                        h += segw - Styled::MeasureText(paint, workPaint,
                                                        text,
                                                        start + segstart,
                                                        offset, NULL);
                        return h;
                    }

                    if (dir == DIR_RIGHT_TO_LEFT) {
                        h -= segw - Styled::MeasureText(paint, workPaint,
                                                        text,
                                                        start + segstart,
                                                        offset, NULL);
                        return h;
                    }
                }

                if (dir == DIR_RIGHT_TO_LEFT) {
                    h -= segw;
                }
                else {
                    h += segw;
                }

                if (j != there && codept == '\t') {
                    if (offset == start + j) {
                        return h;
                    }

                    h = dir * NextTab(text, start, end, h * dir, tabs);
                }

                if (bm != NULL) {
                    workPaint->Set(paint);
                    Styled::MeasureText(paint, workPaint, text,
                                        j, j + 2, NULL);

                    Int32 width, height;
                    Float ascent;
                    bm->GetWidth(&width);
                    workPaint->Ascent(&ascent);
                    bm->GetHeight(&height);
                    Float wid = (Float)width * -ascent / height;

                    if (dir == DIR_RIGHT_TO_LEFT) {
                        h -= wid;
                    }
                    else {
                        h += wid;
                    }

                    j++;
                }

                segstart = j + 1;
            }
        }

        here = there;
    }
    ArrayOf<Int16>::Free(dirs);

    if (hasTabs) {
        TextUtils::Recycle(&buf);
    }

    return h;
}

/**
 * Measure width of a run of text on a single line that is known to all be
 * in the same direction as the paragraph base direction. Returns the width,
 * and the line metrics in fm if fm is not NULL.
 *
 * @param paint the paint for the text; will not be modified
 * @param workPaint paint available for modification
 * @param text text
 * @param start start of the line
 * @param end limit of the line
 * @param fm object to return integer metrics in, can be NULL
 * @param hasTabs TRUE if it is known that the line has tabs
 * @param tabs tab position information
 * @return the width of the text from start to end
 */
Float Layout::MeasureText(
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaintFontMetricsInt* fm,
    /* [in] */ Boolean hasTabs,
    /* [in] */ ArrayOf<IInterface*>* tabs)
{
    ArrayOf<Char8>* buf = NULL;

    if (hasTabs) {
        TextUtils::Obtain(4 * (end - start) + 1, &buf);
        TextUtils::GetChars(text, start, end, buf, 0);
    }

    Int32 len = end - start;

    Int32 lastPos = 0;
    Float width = 0;
    Int32 ascent = 0, descent = 0, top = 0, bottom = 0;

    if (fm != NULL) {
        fm->SetAscent(0);
        fm->SetDescent(0);
    }

    String str(buf != NULL ? (const char*)buf->GetPayload() : NULL);
    for (Int32 pos = hasTabs ? 0 : len; pos <= len; pos++) {
        Char32 codept = 0;
        AutoPtr<IBitmap> bm;

        if (hasTabs && pos < len) {
            codept = str.GetChar(pos);
        }

        if (codept >= 0xD800 && codept <= 0xDFFF && pos < len) {
            ASSERT_SUCCEEDED(Character::GetCharAt(*buf, pos, &codept));

            if (codept >= MIN_EMOJI && codept <= MAX_EMOJI) {
                bm = EMOJI_FACTORY->GetBitmapFromAndroidPua(codept);
            }
        }

        if (pos == len || codept == '\t' || bm != NULL) {
            CTextPaint* workPaint_ = (CTextPaint*)workPaint;
            workPaint_->mBaselineShift = 0;

            width += Styled::MeasureText(paint, workPaint, text,
                                         start + lastPos, start + pos,
                                         fm);

            if (fm != NULL) {
                if (workPaint_->mBaselineShift < 0) {
                    Int32 ascent, top;
                    fm->GetAscent(&ascent);
                    fm->SetAscent(ascent + workPaint_->mBaselineShift);
                    fm->GetTop(&top);
                    fm->SetTop(top + workPaint_->mBaselineShift);
                }
                else {
                    Int32 descent, bottom;
                    fm->GetDescent(&descent);
                    fm->SetDescent(descent + workPaint_->mBaselineShift);
                    fm->GetBottom(&bottom);
                    fm->SetBottom(bottom + workPaint_->mBaselineShift);
                }
            }

            if (pos != len) {
                if (bm == NULL) {
                    // no emoji, must have hit a tab
                    width = NextTab(text, start, end, width, tabs);
                }
                else {
                    // This sets up workPaint with the font on the emoji
                    // text, so that we can extract the ascent and scale.

                    // We can't use the result of the previous call to
                    // measureText because the emoji might have its own style.
                    // We have to initialize workPaint here because if the
                    // text is unstyled measureText might not use workPaint
                    // at all.
                    workPaint->Set(paint);
                    Styled::MeasureText(paint, workPaint, text,
                                        start + pos, start + pos + 1, NULL);

                    Int32 w, h;
                    Float a;
                    bm->GetWidth(&w);
                    workPaint->Ascent(&a);
                    bm->GetHeight(&h);
                    width += (Float)w * -a / h;

                    // Since we had an emoji, we bump past the second half
                    // of the surrogate pair.
                    pos++;
                }
            }

            if (fm != NULL) {
                Int32 a, d, t, b;
                fm->GetAscent(&a);
                fm->GetDescent(&d);
                fm->GetTop(&t);
                fm->GetBottom(&b);

                if (a < ascent) {
                    ascent = a;
                }
                if (d > descent) {
                    descent = d;
                }

                if (t < top) {
                    top = t;
                }
                if (b > bottom) {
                    bottom = b;
                }

                // No need to take bitmap height into account here,
                // since it is scaled to match the text height.
            }

            lastPos = pos + 1;
        }
    }

    if (fm != NULL) {
        fm->SetAscent(ascent);
        fm->SetDescent(descent);
        fm->SetTop(top);
        fm->SetBottom(bottom);
    }

    if (hasTabs) {
        TextUtils::Recycle(&buf);
    }

    return width;
}

/**
 * Returns the position of the next tab stop after h on the line.
 *
 * @param text the text
 * @param start start of the line
 * @param end limit of the line
 * @param h the current horizontal offset
 * @param tabs the tabs, can be NULL.  If it is NULL, any tabs in effect
 * on the line will be used.  If there are no tabs, a default offset
 * will be used to compute the tab stop.
 * @return the offset of the next tab stop.
 */
Float Layout::NextTab(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float h,
    /* [in] */ ArrayOf<IInterface*>* tabs)
{
    Float nh = Math::FLOAT_MAX_VALUE;
    Boolean alltabs = FALSE;

    if (ISpanned::Probe(text) != NULL) {
        if (tabs == NULL) {
            ISpanned::Probe(text)->GetSpans(start, end, EIID_ITabStopSpan, &tabs);
            alltabs = TRUE;
        }

        for (Int32 i = 0; i < tabs->GetLength(); i++) {
            if (!alltabs) {
                if (ITabStopSpan::Probe((*tabs)[i]) == NULL) {
                    continue;
                }
            }

            Int32 where;
            ITabStopSpan::Probe((*tabs)[i])->GetTabStop(&where);

            if (where < nh && where > h) {
                nh = where;
            }

            if (alltabs) (*tabs)[i]->Release();
        }

        if (alltabs) ArrayOf<IInterface*>::Free(tabs);

        if (nh != Math::FLOAT_MAX_VALUE) {
            return nh;
        }
    }

    return ((Int32)((h + TAB_INCREMENT) / TAB_INCREMENT)) * TAB_INCREMENT;
}

Boolean Layout::IsSpanned()
{
    return mSpannedText;
}

void Layout::Ellipsize(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 line,
    /* [out] */ ArrayOf<Char8>* dest,
    /* [in] */ Int32 destoff)
{
    Int32 ellipsisCount = GetEllipsisCount(line);

    if (ellipsisCount == 0) {
        return;
    }

    Int32 ellipsisStart = GetEllipsisStart(line);
    Int32 linestart = GetLineStart(line);

    for (Int32 i = ellipsisStart; i < ellipsisStart + ellipsisCount; i++) {
        Char32 c;

        if (i == ellipsisStart) {
            c = 0x2026/*'\u2026'*/; // ellipsis
        }
        else {
            c = 0xFEFF/*'\uFEFF'*/; // 0-width space
        }

        Int32 a = i + linestart;

        if (a >= start && a < end) {
//            dest[destoff + a - start] = c;
        }
    }
}

ECode Layout::Init(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingMult,
    /* [in] */ Float spacingAdd)
{
    mText = text;
    mPaint = paint;
    mWidth = width;
    mAlignment = align;
    mSpacingMult = spacingMult;
    mSpacingAdd = spacingAdd;

//    if (width < 0)
//        throw new IllegalArgumentException("Layout: " + width + " < 0");
    assert(width >= 0);

    AutoPtr<CTextPaint> tmp;
    CTextPaint::NewByFriend((CTextPaint**)&tmp);
    mWorkPaint = (ITextPaint*)tmp.Get();
    mSpannedText = text != NULL && text->Probe(EIID_ISpanned) != NULL;

    return NOERROR;
}
