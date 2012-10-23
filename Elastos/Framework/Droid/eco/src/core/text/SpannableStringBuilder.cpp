
#include "text/SpannableStringBuilder.h"
#include "text/CSpannableStringBuilder.h"
#include "text/SpannableStringInternal.h"
#include "text/TextUtils.h"
#include "text/Selection.h"
#include "utils/ArrayUtils.h"
#include <elastos/Character.h>
#include <elastos/Math.h>
#include <StringBuffer.h>

using namespace Elastos::Core;

const Int32 SpannableStringBuilder::MARK;
const Int32 SpannableStringBuilder::POINT;
const Int32 SpannableStringBuilder::PARAGRAPH;

const Int32 SpannableStringBuilder::START_MASK;
const Int32 SpannableStringBuilder::END_MASK;
const Int32 SpannableStringBuilder::START_SHIFT;

AutoPtr<ICharSequence> SpannableStringBuilder::EMPTY_CS;

AutoPtr<ICharSequence> CreateEmptyCs()
{
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String(""), (ICharSequence**)&cs);
    return cs;
}

SpannableStringBuilder::SpannableStringBuilder()
{
    if (EMPTY_CS == NULL) {
        EMPTY_CS = CreateEmptyCs();
    }
}

SpannableStringBuilder::~SpannableStringBuilder()
{
    ArrayOf<Char32>::Free(mText);
    FreeArray(mSpans);
    ArrayOf<Int32>::Free(mSpanStarts);
    ArrayOf<Int32>::Free(mSpanEnds);
    ArrayOf<Int32>::Free(mSpanFlags);
}

AutoPtr<ISpannableStringBuilder> SpannableStringBuilder::ValueOf(
    /* [in] */ ICharSequence* source)
{
    if (ISpannableStringBuilder::Probe(source)) {
        return ISpannableStringBuilder::Probe(source);
    }
    else {
        AutoPtr<ISpannableStringBuilder> ssb;
        CSpannableStringBuilder::New(source, (ISpannableStringBuilder**)&ssb);
        return ssb;
    }
}

/**
 * Return the char at the specified offset within the buffer.
 */
Char32 SpannableStringBuilder::GetCharAt(
    /* [in] */ Int32 where)
{
    Int32 len = GetLength();
    if (where < 0) {
        //throw new IndexOutOfBoundsException("GetCharAt: " + where + " < 0");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    else if (where >= len) {
        //throw new IndexOutOfBoundsException("GetCharAt: " + where +
        //                                    " >= GetLength " + len);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (where >= mGapStart)
        return (*mText)[where + mGapLength];
    else
        return (*mText)[where];
}

/**
 * Return the number of chars in the buffer.
 */
Int32 SpannableStringBuilder::GetLength()
{
    return mText->GetLength() - mGapLength;
}

void SpannableStringBuilder::ResizeFor(
    /* [in] */ Int32 size)
{
    Int32 len = mText->GetLength();
    Int32 newlen = ArrayUtils::IdealChar8ArraySize(size + 1);
    ArrayOf<Char32>* newtext = ArrayOf<Char32>::Alloc(newlen);

    Int32 after = len - (mGapStart + mGapLength);

    memcpy(newtext->GetPayload(), mText->GetPayload(), sizeof(Char32) * mGapStart);
    memcpy(
        newtext->GetPayload() + newlen - after,
        mText->GetPayload() + len - after,
        sizeof(Char32) * after);

    for (Int32 i = 0; i < mSpanCount; i++) {
        if ((*mSpanStarts)[i] > mGapStart)
            (*mSpanStarts)[i] += newlen - len;
        if ((*mSpanEnds)[i] > mGapStart)
            (*mSpanEnds)[i] += newlen - len;
    }

    ArrayOf<Char32>::Free(mText);
    Int32 oldlen = len;
    mText = newtext;
    mGapLength += len - oldlen;

    if (mGapLength < 1)
        assert(0);
        //new Exception("mGapLength < 1").printStackTrace();
}

void SpannableStringBuilder::MoveGapTo(
    /* [in] */ Int32 where)
{
    if (where == mGapStart)
        return;

    Boolean atend = (where == GetLength());

    if (where < mGapStart) {
        Int32 overlap = mGapStart - where;

        memcpy(
            mText->GetPayload() + mGapStart + mGapLength - overlap,
            mText->GetPayload() + where, sizeof(Char32) * overlap);
    }
    else /* where > mGapStart */ {
        Int32 overlap = where - mGapStart;

        memcpy(
            mText->GetPayload() + mGapStart,
            mText->GetPayload() + where + mGapLength - overlap,
            sizeof(Char32) * overlap);
    }

    // XXX be more clever
    for (Int32 i = 0; i < mSpanCount; i++) {
        Int32 start = (*mSpanStarts)[i];
        Int32 end = (*mSpanEnds)[i];

        if (start > mGapStart)
            start -= mGapLength;
        if (start > where)
            start += mGapLength;
        else if (start == where) {
            Int32 flag = ((*mSpanFlags)[i] & START_MASK) >> START_SHIFT;

            if (flag == POINT || (atend && flag == PARAGRAPH))
                start += mGapLength;
        }

        if (end > mGapStart)
            end -= mGapLength;
        if (end > where)
            end += mGapLength;
        else if (end == where) {
            Int32 flag = ((*mSpanFlags)[i] & END_MASK);

            if (flag == POINT || (atend && flag == PARAGRAPH))
                end += mGapLength;
        }

        (*mSpanStarts)[i] = start;
        (*mSpanEnds)[i] = end;
    }

    mGapStart = where;
}

// Documentation from interface
AutoPtr<IEditable> SpannableStringBuilder::Insert(
    /* [in] */ Int32 where,
    /* [in] */ ICharSequence* tb,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return Replace(where, where, tb, start, end);
}

// Documentation from interface
AutoPtr<IEditable> SpannableStringBuilder::Insert(
    /* [in] */ Int32 where,
    /* [in] */ ICharSequence* tb)
{
    assert(tb);
    Int32 len;
    tb->GetLength(&len);
    return Replace(where, where, tb, 0, len);
}

// Documentation from interface
AutoPtr<IEditable> SpannableStringBuilder::Delete(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IEditable> ret = Replace(start, end, EMPTY_CS, 0, 0);

    if (mGapLength > 2 * GetLength())
        ResizeFor(GetLength());

    return ret; // == this
}

// Documentation from interface
ECode SpannableStringBuilder::Clear()
{
    Replace(0, GetLength(), EMPTY_CS, 0, 0);
    return NOERROR;
}

// Documentation from interface
ECode SpannableStringBuilder::ClearSpans()
{
    for (Int32 i = mSpanCount - 1; i >= 0; i--) {
        IInterface* what = (*mSpans)[i];
        Int32 ostart = (*mSpanStarts)[i];
        Int32 oend = (*mSpanEnds)[i];

        if (ostart > mGapStart)
            ostart -= mGapLength;
        if (oend > mGapStart)
            oend -= mGapLength;

        mSpanCount = i;
        (*mSpans)[i] = NULL;

        SendSpanRemoved(what, ostart, oend);
    }

    return NOERROR;
}

// Documentation from interface
AutoPtr<IEditable> SpannableStringBuilder::Append(
    /* [in] */ ICharSequence* text)
{
    Int32 length = GetLength();
    Int32 tLen;
    text->GetLength(&tLen);
    return Replace(length, length, text, 0, tLen);
}

// Documentation from interface
AutoPtr<IEditable> SpannableStringBuilder::Append(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 length = GetLength();
    return Replace(length, length, text, start, end);
}

// Documentation from interface
AutoPtr<IEditable> SpannableStringBuilder::Append(
    /* [in] */ Char32 text)
{
    ArrayOf_<Char8, 5> chs;
    Int32 num;
    Character::ToChars(text, chs, 0, &num);
    chs[num] = 0;
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String(chs.GetPayload()), (ICharSequence**)&cs);
    return Append(cs);
}

Int32 SpannableStringBuilder::Change(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* tb,
    /* [in] */ Int32 tbstart,
    /* [in] */ Int32 tbend)
{
    return Change(TRUE, start, end, tb, tbstart, tbend);
}

Int32 SpannableStringBuilder::Change(
    /* [in] */ Boolean notify,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* tb,
    /* [in] */ Int32 tbstart,
    /* [in] */ Int32 tbend)
{
    CheckRange(String("replace"), start, end);
    Int32 ret = tbend - tbstart;
    ArrayOf<AutoPtr<ITextWatcher> >* recipients = NULL;

    if (notify)
        recipients = SendTextWillChange(
            start, end - start, tbend - tbstart);

    for (Int32 i = mSpanCount - 1; i >= 0; i--) {
        if (((*mSpanFlags)[i] & Spanned_SPAN_PARAGRAPH) == Spanned_SPAN_PARAGRAPH) {
            Int32 st = (*mSpanStarts)[i];
            if (st > mGapStart)
                st -= mGapLength;

            Int32 en = (*mSpanEnds)[i];
            if (en > mGapStart)
                en -= mGapLength;

            Int32 ost = st;
            Int32 oen = en;
            Int32 clen = GetLength();

            if (st > start && st <= end) {
                for (st = end; st < clen; st++)
                    if (st > end && GetCharAt(st - 1) == '\n')
                        break;
            }

            if (en > start && en <= end) {
                for (en = end; en < clen; en++)
                    if (en > end && GetCharAt(en - 1) == '\n')
                        break;
            }

            if (st != ost || en != oen)
                SetSpan((*mSpans)[i], st, en, (*mSpanFlags)[i]);
        }
    }

    MoveGapTo(end);

    if (tbend - tbstart >= mGapLength + (end - start))
        ResizeFor(mText->GetLength() - mGapLength +
                  tbend - tbstart - (end - start));

    mGapStart += tbend - tbstart - (end - start);
    mGapLength -= tbend - tbstart - (end - start);

    if (mGapLength < 1)
        assert(0);
        //new Exception("mGapLength < 1").printStackTrace();

    for (Int32 i = tbstart; i < tbend; ++i) {
        Char32 ch;
        tb->GetCharAt(i, &ch);
        (*mText)[i - tbstart + start] = ch;
    }

    if (ISpanned::Probe(tb)) {
        AutoPtr<ISpanned> sp = ISpanned::Probe(tb);
        ArrayOf<IInterface*>* spans;
        sp->GetSpans(tbstart, tbend, EIID_IInterface, &spans);

        for (Int32 i = 0; i < spans->GetLength(); i++) {
            Int32 st;
            sp->GetSpanStart((*spans)[i], &st);
            Int32 en;
            sp->GetSpanEnd((*spans)[i], &en);

            if (st < tbstart)
                st = tbstart;
            if (en > tbend)
                en = tbend;

            if (GetSpanStart((*spans)[i]) < 0) {
                Int32 flags;
                sp->GetSpanFlags((*spans)[i], &flags);
                SetSpan(
                    FALSE, (*spans)[i], st - tbstart + start,
                    en - tbstart + start, flags);
            }
        }
        FreeArray(spans);
    }

    // no need for span fixup on pure insertion
    if (tbend > tbstart && end - start == 0) {
        if (notify) {
            SendTextChange(*recipients, start, end - start, tbend - tbstart);
            SendTextHasChanged(*recipients);
        }

        return ret;
    }

    Boolean atend = (mGapStart + mGapLength == mText->GetLength());

    for (Int32 i = mSpanCount - 1; i >= 0; i--) {
        if ((*mSpanStarts)[i] >= start &&
            (*mSpanStarts)[i] < mGapStart + mGapLength) {
            Int32 flag = ((*mSpanFlags)[i] & START_MASK) >> START_SHIFT;

            if (flag == POINT || (flag == PARAGRAPH && atend))
                (*mSpanStarts)[i] = mGapStart + mGapLength;
            else
                (*mSpanStarts)[i] = start;
        }

        if ((*mSpanEnds)[i] >= start &&
            (*mSpanEnds)[i] < mGapStart + mGapLength) {
            Int32 flag = ((*mSpanFlags)[i] & END_MASK);

            if (flag == POINT || (flag == PARAGRAPH && atend))
                (*mSpanEnds)[i] = mGapStart + mGapLength;
            else
                (*mSpanEnds)[i] = start;
        }

        // remove 0-GetLength SPAN_EXCLUSIVE_EXCLUSIVE
        // XXX send notification on removal

        if ((*mSpanEnds)[i] < (*mSpanStarts)[i]) {
            if ((*mSpans)[i])
                (*mSpans)[i]->Release();
            memcpy(mSpans->GetPayload() + i, mSpans->GetPayload() + i + 1,
                sizeof(IInterface*) * mSpanCount - (i + 1));
            memcpy(mSpanStarts->GetPayload() + i, mSpanStarts->GetPayload() + i + 1,
                sizeof(Int32) * mSpanCount - (i + 1));
            memcpy(mSpanEnds->GetPayload() + i, mSpanEnds->GetPayload() + i + 1,
                sizeof(Int32) * mSpanCount - (i + 1));
            memcpy(mSpanFlags->GetPayload() + i, mSpanFlags->GetPayload() + i + 1,
                sizeof(Int32) * mSpanCount - (i + 1));

            mSpanCount--;
        }
    }

    if (notify) {
        SendTextChange(*recipients, start, end - start, tbend - tbstart);
        SendTextHasChanged(*recipients);
    }

    return ret;
}

// Documentation from interface
AutoPtr<IEditable> SpannableStringBuilder::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* tb)
{
    Int32 len;
    tb->GetLength(&len);
    return Replace(start, end, tb, 0, len);
}

// Documentation from interface
AutoPtr<IEditable> SpannableStringBuilder::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* tb,
    /* [in] */ Int32 tbstart,
    /* [in] */ Int32 tbend)
{
    //Int32 filtercount = mFilters.GetLength;
    //for (Int32 i = 0; i < filtercount; i++) {
    //    CharSequence repl = mFilters[i].filter(tb, tbstart, tbend,
    //                                           this, start, end);

    //    if (repl != NULL) {
    //        tb = repl;
    //        tbstart = 0;
    //        tbend = repl.GetLength();
    //    }
    //}

    if (end == start && tbstart == tbend) {
        return (IEditable*)this->Probe(EIID_IEditable);
    }

    if (end == start || tbstart == tbend) {
        Change(start, end, tb, tbstart, tbend);
    }
    else {
        Int32 selstart = Selection::GetSelectionStart(
            (ICharSequence*)this->Probe(EIID_ICharSequence));
        Int32 selend = Selection::GetSelectionEnd(
            (ICharSequence*)this->Probe(EIID_ICharSequence));

        // XXX just make the span fixups in Change() do the right thing
        // instead of this madness!

        CheckRange(String("replace"), start, end);
        MoveGapTo(end);
        ArrayOf<AutoPtr<ITextWatcher> >* recipients = NULL;

        recipients = SendTextWillChange(start, end - start,
                                        tbend - tbstart);

        Int32 origlen = end - start;

        if (mGapLength < 2)
            ResizeFor(GetLength() + 1);

        for (Int32 i = mSpanCount - 1; i >= 0; i--) {
            if ((*mSpanStarts)[i] == mGapStart)
                (*mSpanStarts)[i]++;

            if ((*mSpanEnds)[i] == mGapStart)
                (*mSpanEnds)[i]++;
        }

        (*mText)[mGapStart] = ' ';
        mGapStart++;
        mGapLength--;

        if (mGapLength < 1)
            assert(0);
            //new Exception("mGapLength < 1").printStackTrace();

        Int32 oldlen = (end + 1) - start;

        Int32 inserted = Change(
            FALSE, start + 1, start + 1, tb, tbstart, tbend);
        Change(FALSE, start, start + 1, EMPTY_CS, 0, 0);
        Change(FALSE, start + inserted, start + inserted + oldlen - 1,
               EMPTY_CS, 0, 0);

        /*
         * Special case to keep the cursor in the same position
         * if it was somewhere in the middle of the replaced region.
         * If it was at the start or the end or crossing the whole
         * replacement, it should already be where it belongs.
         * TODO: Is there some more general mechanism that could
         * accomplish this?
         */
        if (selstart > start && selstart < end) {
            Int64 off = selstart - start;

            off = off * inserted / (end - start);
            selstart = (Int32) off + start;

            SetSpan(FALSE, Selection::SELECTION_START, selstart, selstart,
                    Spanned_SPAN_POINT_POINT);
        }
        if (selend > start && selend < end) {
            Int64 off = selend - start;

            off = off * inserted / (end - start);
            selend = (Int32) off + start;

            SetSpan(FALSE, Selection::SELECTION_END, selend, selend,
                    Spanned_SPAN_POINT_POINT);
        }

        SendTextChange(*recipients, start, origlen, inserted);
        SendTextHasChanged(*recipients);
    }
    return (IEditable*)this->Probe(EIID_IEditable);
}

/**
 * Mark the specified range of text with the specified object.
 * The flags determine how the span will behave when text is
 * inserted at the start or end of the span's range.
 */
ECode SpannableStringBuilder::SetSpan(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    SetSpan(TRUE, what, start, end, flags);

    return NOERROR;
}

ECode SpannableStringBuilder::SetSpan(
    /* [in] */ Boolean send,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    assert(what);
    Int32 nstart = start;
    Int32 nend = end;

    CheckRange(String("SetSpan"), start, end);

    if ((flags & START_MASK) == (PARAGRAPH << START_SHIFT)) {
        if (start != 0 && start != GetLength()) {
            Char32 c = GetCharAt(start - 1);

            if (c != '\n')
                //throw new RuntimeException(
                //        "PARAGRAPH span must start at paragraph boundary");
                return E_RUNTIME_EXCEPTION;
        }
    }

    if ((flags & END_MASK) == PARAGRAPH) {
        if (end != 0 && end != GetLength()) {
            Char32 c = GetCharAt(end - 1);

            if (c != '\n')
                //throw new RuntimeException(
                //        "PARAGRAPH span must end at paragraph boundary");
                return E_RUNTIME_EXCEPTION;
        }
    }

    if (start > mGapStart)
        start += mGapLength;
    else if (start == mGapStart) {
        Int32 flag = (flags & START_MASK) >> START_SHIFT;

        if (flag == POINT || (flag == PARAGRAPH && start == GetLength()))
            start += mGapLength;
    }

    if (end > mGapStart)
        end += mGapLength;
    else if (end == mGapStart) {
        Int32 flag = (flags & END_MASK);

        if (flag == POINT || (flag == PARAGRAPH && end == GetLength()))
            end += mGapLength;
    }

    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;

    for (Int32 i = 0; i < count; i++) {
        if (spans[i] == what) {
            Int32 ostart = (*mSpanStarts)[i];
            Int32 oend = (*mSpanEnds)[i];

            if (ostart > mGapStart)
                ostart -= mGapLength;
            if (oend > mGapStart)
                oend -= mGapLength;

            (*mSpanStarts)[i] = start;
            (*mSpanEnds)[i] = end;
            (*mSpanFlags)[i] = flags;

            if (send)
                SendSpanChanged(what, ostart, oend, nstart, nend);

            return NOERROR;
        }
    }

    if (mSpanCount + 1 >= mSpans->GetLength()) {
        Int32 newsize = ArrayUtils::IdealInt32ArraySize(mSpanCount + 1);
        ArrayOf<IInterface*>* newspans = ArrayOf<IInterface*>::Alloc(newsize);
        ArrayOf<Int32>* newspanstarts = ArrayOf<Int32>::Alloc(newsize);
        ArrayOf<Int32>* newspanends = ArrayOf<Int32>::Alloc(newsize);
        ArrayOf<Int32>* newspanflags = ArrayOf<Int32>::Alloc(newsize);

        memcpy(newspans->GetPayload(), mSpans->GetPayload(),
            sizeof(IInterface*) * mSpanCount);
        memcpy(newspanstarts->GetPayload(), mSpanStarts->GetPayload(),
            sizeof(Int32) * mSpanCount);
        memcpy(newspanends->GetPayload(), mSpanEnds->GetPayload(),
            sizeof(Int32) * mSpanCount);
        memcpy(newspanflags->GetPayload(), mSpanFlags->GetPayload(),
            sizeof(Int32) * mSpanCount);

        ArrayOf<IInterface*>::Free(mSpans);
        ArrayOf<Int32>::Free(mSpanStarts);
        ArrayOf<Int32>::Free(mSpanEnds);
        ArrayOf<Int32>::Free(mSpanFlags);
        mSpans = newspans;
        mSpanStarts = newspanstarts;
        mSpanEnds = newspanends;
        mSpanFlags = newspanflags;
    }

    (*mSpans)[mSpanCount] = what;
    if (what)
        what->AddRef();
    (*mSpanStarts)[mSpanCount] = start;
    (*mSpanEnds)[mSpanCount] = end;
    (*mSpanFlags)[mSpanCount] = flags;
    mSpanCount++;

    if (send)
        SendSpanAdded(what, nstart, nend);

    return NOERROR;
}

/**
 * Remove the specified markup object from the buffer.
 */
ECode SpannableStringBuilder::RemoveSpan(
    /* [in] */ IInterface* what)
{
    for (Int32 i = mSpanCount - 1; i >= 0; i--) {
        if ((*mSpans)[i] == what) {
            Int32 ostart = (*mSpanStarts)[i];
            Int32 oend = (*mSpanEnds)[i];

            if (ostart > mGapStart)
                ostart -= mGapLength;
            if (oend > mGapStart)
                oend -= mGapLength;

            Int32 count = mSpanCount - (i + 1);

            if ((*mSpans)[i])
                (*mSpans)[i]->Release();
            memcpy(mSpans->GetPayload() + i, mSpans->GetPayload() + i + 1,
                sizeof(IInterface*) * count);
            memcpy(mSpanStarts->GetPayload() + i, mSpanStarts->GetPayload() + i + 1,
                sizeof(Int32) * count);
            memcpy(mSpanEnds->GetPayload() + i, mSpanEnds->GetPayload() + i + 1,
                sizeof(Int32) * count);
            memcpy(mSpanFlags->GetPayload() + i, mSpanFlags->GetPayload() + i + 1,
                sizeof(Int32) * count);

            mSpanCount--;
            (*mSpans)[mSpanCount] = NULL;

            SendSpanRemoved(what, ostart, oend);
            return NOERROR;
        }
    }

    return NOERROR;
}

/**
 * Return the buffer offset of the beginning of the specified
 * markup object, or -1 if it is not attached to this buffer.
 */
Int32 SpannableStringBuilder::GetSpanStart(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;

    for (Int32 i = count - 1; i >= 0; i--) {
        if (spans[i] == what) {
            Int32 where = (*mSpanStarts)[i];

            if (where > mGapStart)
                where -= mGapLength;

            return where;
        }
    }

    return -1;
}

/**
 * Return the buffer offset of the end of the specified
 * markup object, or -1 if it is not attached to this buffer.
 */
Int32 SpannableStringBuilder::GetSpanEnd(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;

    for (Int32 i = count - 1; i >= 0; i--) {
        if (spans[i] == what) {
            Int32 where = (*mSpanEnds)[i];

            if (where > mGapStart)
                where -= mGapLength;

            return where;
        }
    }

    return -1;
}

/**
 * Return the flags of the end of the specified
 * markup object, or 0 if it is not attached to this buffer.
 */
Int32 SpannableStringBuilder::GetSpanFlags(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;

    for (Int32 i = count - 1; i >= 0; i--) {
        if (spans[i] == what) {
            return (*mSpanFlags)[i];
        }
    }

    return 0;
}

/**
 * Return an array of the spans of the specified type that overlap
 * the specified range of the buffer.  The kind may be Object.class to get
 * a list of all the spans regardless of type.
 */
ECode SpannableStringBuilder::GetSpans(
    /* [in] */ Int32 queryStart,
    /* [in] */ Int32 queryEnd,
    /* [in] */ const InterfaceID& kind,
    /* [out, callee] */ ArrayOf<IInterface*>** objs)
{
    Int32 spanCount = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& starts = *mSpanStarts;
    ArrayOf<Int32>& ends = *mSpanEnds;
    ArrayOf<Int32>& flags = *mSpanFlags;
    Int32 gapstart = mGapStart;
    Int32 gaplen = mGapLength;

    Int32 count = 0;
    ArrayOf<IInterface*>* ret = NULL;
    AutoPtr<IInterface> ret1;

    for (Int32 i = 0; i < spanCount; i++) {
        Int32 spanStart = starts[i];
        Int32 spanEnd = ends[i];

        if (spanStart > gapstart) {
            spanStart -= gaplen;
        }
        if (spanEnd > gapstart) {
            spanEnd -= gaplen;
        }

        if (spanStart > queryEnd) {
            continue;
        }
        if (spanEnd < queryStart) {
            continue;
        }

        if (spanStart != spanEnd && queryStart != queryEnd) {
            if (spanStart == queryEnd)
                continue;
            if (spanEnd == queryStart)
                continue;
        }

        if (spans[i]->Probe(kind) == NULL) {
            continue;
        }

        if (count == 0) {
            ret1 = spans[i];
            count++;
        }
        else {
            if (count == 1) {
                FreeArray(ret);
                ret = ArrayOf<IInterface*>::Alloc(spanCount - i + 1);
                (*ret)[0] = ret1;
                if (ret1 != NULL) ret1->AddRef();
            }

            Int32 prio = flags[i] & Spanned_SPAN_PRIORITY;
            if (prio != 0) {
                Int32 j;

                for (j = 0; j < count; j++) {
                    Int32 p = GetSpanFlags((*ret)[j]) & Spanned_SPAN_PRIORITY;

                    if (prio > p) {
                        break;
                    }
                }

                for (Int32 k = 0; k < count - j; k++) {
                    (*ret)[count - k] = (*ret)[count - k - 1];
                }
                (*ret)[j] = spans[i];
                if (spans[i] != NULL) spans[i]->AddRef();
                count++;
            }
            else {
                (*ret)[count++] = spans[i];
                if (spans[i] != NULL) spans[i]->AddRef();
            }
        }
    }

    if (count == 0) {
        //        return (T[]) ArrayUtils.emptyArray(kind);
        *objs = ArrayOf<IInterface*>::Alloc(0);
        return NOERROR;
    }
    if (count == 1) {
        FreeArray(ret);
        ret = ArrayOf<IInterface*>::Alloc(1);
        (*ret)[0] = ret1;
        if (ret1 != NULL) ret1->AddRef();
        *objs = ret;
        return NOERROR;
    }
    if (count == ret->GetLength()) {
        *objs = ret;
        return NOERROR;
    }

    ArrayOf<IInterface*>* nret = ArrayOf<IInterface*>::Alloc(count);
    for (Int32 i = 0; i < count; i++) {
        (*nret)[i] = (*ret)[i];
        if ((*nret)[i] != NULL) (*nret)[i]->AddRef();
    }
    FreeArray(ret);
    *objs = nret;
    return NOERROR;
}

/**
 * Return the next offset after <code>start</code> but less than or
 * equal to <code>limit</code> where a span of the specified type
 * begins or ends.
 */
Int32 SpannableStringBuilder::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& kind)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& starts = *mSpanStarts;
    ArrayOf<Int32>& ends = *mSpanEnds;
    Int32 gapstart = mGapStart;
    Int32 gaplen = mGapLength;

    //if (kind == NULL) {
    //    kind = Object.class;
    //}

    for (Int32 i = 0; i < count; i++) {
        Int32 st = starts[i];
        Int32 en = ends[i];

        if (st > gapstart)
            st -= gaplen;
        if (en > gapstart)
            en -= gaplen;

        if (st > start && st < limit && spans[i]->Probe(kind) != NULL)
            limit = st;
        if (en > start && en < limit && spans[i]->Probe(kind) != NULL)
            limit = en;
    }

    return limit;
}

/**
 * Return a new CharSequence containing a copy of the specified
 * range of this buffer, including the overlapping spans.
 */
AutoPtr<ICharSequence> SpannableStringBuilder::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return NULL;
    //return new SpannableStringBuilder(this, start, end);
}

/**
 * Copy the specified range of chars from this buffer into the
 * specified array, beginning at the specified offset.
 */
ECode SpannableStringBuilder::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Char8>* dest,
    /* [in] */ Int32 destoff)
{
    CheckRange(String("getChars"), start, end);

    if (end <= mGapStart) {
        Char32ArrayToChar8Array(*mText, start, end, *dest, destoff);
    }
    else if (start >= mGapStart) {
        Char32ArrayToChar8Array(
            *mText, start + mGapLength, end + mGapLength, *dest, destoff);
    }
    else {
        Char32ArrayToChar8Array(*mText, start, mGapStart, *dest, destoff);
        Char32ArrayToChar8Array(
            *mText, mGapStart + mGapLength, end + mGapLength,
            *dest, destoff + (mGapStart - start));
    }

    return NOERROR;
}

/**
 * Return a String containing a copy of the chars in this buffer.
 */
String SpannableStringBuilder::ToString()
{
    Int32 len = GetLength();
    ArrayOf<Char8>* buf = ArrayOf<Char8>::Alloc(len * 4);

    GetChars(0, len, buf, 0);
    String str(buf->GetPayload());
    ArrayOf<Char8>::Free(buf);

    return str;
}

ArrayOf<AutoPtr<ITextWatcher> >* SpannableStringBuilder::SendTextWillChange(
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    ArrayOf<IInterface*>* array;
    GetSpans(start, start + before, EIID_ITextWatcher, &array);

    Int32 n = array->GetLength();
    ArrayOf<AutoPtr<ITextWatcher> >* recip =
        ArrayOf<AutoPtr<ITextWatcher> >::Alloc(n);
    for (Int32 i = 0; i < n; i++) {
        (*recip)[i] = ITextWatcher::Probe((*array)[i]);
        (*recip)[i]->BeforeTextChanged(
            (ICharSequence*)this->Probe(EIID_ICharSequence), start, before, after);
    }
    FreeArray(array);

    return recip;
}

void SpannableStringBuilder::SendTextChange(
    /* [in] */ const ArrayOf<AutoPtr<ITextWatcher> >& recip,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    Int32 n = recip.GetLength();

    for (Int32 i = 0; i < n; i++) {
        recip[i]->OnTextChanged(
            (ICharSequence*)this->Probe(EIID_ICharSequence), start, before, after);
    }
}

void SpannableStringBuilder::SendTextHasChanged(
    /* [in] */ const ArrayOf<AutoPtr<ITextWatcher> >& recip)
{
    Int32 n = recip.GetLength();

    for (Int32 i = 0; i < n; i++) {
        recip[i]->AfterTextChanged(
            (IEditable*)this->Probe(EIID_IEditable));
    }
}

void SpannableStringBuilder::SendSpanAdded(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    ArrayOf<IInterface*>* recip;
    GetSpans(start, end, EIID_ISpanWatcher, &recip);

    Int32 n = recip->GetLength();
    for (Int32 i = 0; i < n; i++) {
        ISpanWatcher::Probe((*recip)[i])->OnSpanAdded(
            (ISpannable*)this->Probe(EIID_ISpannable), what, start, end);
    }
    FreeArray(recip);
}

void SpannableStringBuilder::SendSpanRemoved(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    ArrayOf<IInterface*>* recip;
    GetSpans(start, end, EIID_ISpanWatcher, &recip);

    Int32 n = recip->GetLength();
    for (Int32 i = 0; i < n; i++) {
        ISpanWatcher::Probe((*recip)[i])->OnSpanRemoved(
            (ISpannable*)this->Probe(EIID_ISpannable), what, start, end);
    }
    FreeArray(recip);
}

void SpannableStringBuilder::SendSpanChanged(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 s,
    /* [in] */ Int32 e,
    /* [in] */ Int32 st,
    /* [in] */ Int32 en)
{
    ArrayOf<IInterface*>* recip;
    GetSpans(Math::Min(s, st), Math::Max(e, en), EIID_ISpanWatcher, &recip);

    Int32 n = recip->GetLength();
    for (Int32 i = 0; i < n; i++) {
        ISpanWatcher::Probe((*recip)[i])->OnSpanChanged(
            (ISpannable*)this->Probe(EIID_ISpannable), what, s, e, st, en);
    }
    FreeArray(recip);
}

String SpannableStringBuilder::Region(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return String(StringBuffer("(") + start + " ... " + end + ")");
}

ECode SpannableStringBuilder::CheckRange(
    /* [in] */ const String& operation,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (end < start) {
        //throw new IndexOutOfBoundsException(operation + " " +
        //                                    region(start, end) +
        //                                    " has end before start");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 len = GetLength();

    if (start > len || end > len) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        //throw new IndexOutOfBoundsException(operation + " " +
        //                                    region(start, end) +
        //                                    " ends beyond GetLength " + len);
    }

    if (start < 0 || end < 0) {
        //throw new IndexOutOfBoundsException(operation + " " +
        //                                    region(start, end) +
        //                                    " starts before 0");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return NOERROR;
}

Boolean SpannableStringBuilder::Isprint(
    /* [in] */ Char32 c)
{ // XXX
    if (c >= ' ' && c <= '~')
        return TRUE;
    else
        return FALSE;
}

/**
 * Don't call this yourself -- exists for Canvas to use internally.
 * {@hide}
 */
ECode SpannableStringBuilder::DrawText(
    /* [in] */ ICanvas* c,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* p)
{
    CheckRange(String("drawText"), start, end);

    ArrayOf<Char8>* buf;
    TextUtils::Obtain(end - start, &buf);
    if (end <= mGapStart) {
        Char32ArrayToChar8Array(*mText, start, end, *buf, 0);
        c->DrawTextInBuffer(*buf, 0, end - start, x, y, p);
    }
    else if (start >= mGapStart) {
        Char32ArrayToChar8Array(*mText, start + mGapLength, end + mGapLength, *buf, 0);
        c->DrawTextInBuffer(*buf, 0, end - start, x, y, p);
    }
    else {
        GetChars(start, end, buf, 0);
        c->DrawTextInBuffer(*buf, 0, end - start, x, y, p);

    }
    TextUtils::Recycle(&buf);

    return NOERROR;
}

/**
 * Don't call this yourself -- exists for Paint to use internally.
 * {@hide}
 */
Float SpannableStringBuilder::MeasureText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaint* p)
{
    CheckRange(String("measureText"), start, end);

    Float ret;

    ArrayOf<Char8>* buf;
    TextUtils::Obtain(end - start, &buf);
    if (end <= mGapStart) {
        Char32ArrayToChar8Array(*mText, start, end, *buf, 0);
        p->MeasureText(*buf, 0, end - start, &ret);
    }
    else if (start >= mGapStart) {
        Char32ArrayToChar8Array(*mText, start + mGapLength, end + mGapLength, *buf, 0);
        p->MeasureText(*buf, 0, end - start, &ret);
    }
    else {
        GetChars(start, end, buf, 0);
        p->MeasureText(*buf, 0, end - start, &ret);

    }
    TextUtils::Recycle(&buf);

    return ret;
}

/**
 * Don't call this yourself -- exists for Paint to use internally.
 * {@hide}
 */
Int32 SpannableStringBuilder::GetTextWidths(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [in] */ IPaint* p)
{
    CheckRange(String("getTextWidths"), start, end);

    Int32 ret;

    ArrayOf<Char8>* buf;
    TextUtils::Obtain(end - start, &buf);
    if (end <= mGapStart) {
        Char32ArrayToChar8Array(*mText, start, end, *buf, 0);
        p->GetTextWidths(*buf, 0, end - start, widths, &ret);
    }
    else if (start >= mGapStart) {
        Char32ArrayToChar8Array(*mText, start + mGapLength, end + mGapLength, *buf, 0);
        p->GetTextWidths(*buf, 0, end - start, widths, &ret);
    }
    else {
        GetChars(start, end, buf, 0);
        p->GetTextWidths(*buf, 0, end - start, widths, &ret);

    }
    TextUtils::Recycle(&buf);

    return ret;
}

// Documentation from interface
ECode SpannableStringBuilder::SetFilters(
    /* [in] */ IObjectContainer* filters)
{
    //if (filters == NULL) {
    //    return E_ILLEGAL_ARGUMENT_EXCEPTION;
    //}

    //mFilters = filters;
    return E_NOT_IMPLEMENTED;
}

// Documentation from interface
ECode SpannableStringBuilder::GetFilters(
    /* [out] */ IObjectContainer** filters)
{
    return E_NOT_IMPLEMENTED;
}

ECode SpannableStringBuilder::Init()
{
    return Init(EMPTY_CS);
}

ECode SpannableStringBuilder::Init(
    /* [in] */ ICharSequence* text)
{
    Int32 len;
    text->GetLength(&len);
    return Init(text, 0, len);
}

ECode SpannableStringBuilder::Init(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    assert(text);
    Int32 srclen = end - start;

    Int32 len = ArrayUtils::IdealInt32ArraySize(srclen + 1);
    mText = ArrayOf<Char32>::Alloc(len);
    mGapStart = srclen;
    mGapLength = len - srclen;

    for (Int32 i = start; i < end; i++) {
        Char32 ch;
        text->GetCharAt(i, &ch);
        (*mText)[i - start] = ch;
    }

    mSpanCount = 0;
    Int32 alloc = ArrayUtils::IdealInt32ArraySize(0);
    mSpans = ArrayOf<IInterface*>::Alloc(alloc);
    mSpanStarts = ArrayOf<Int32>::Alloc(alloc);
    mSpanEnds = ArrayOf<Int32>::Alloc(alloc);
    mSpanFlags = ArrayOf<Int32>::Alloc(alloc);

    if (ISpanned::Probe(text)) {
        AutoPtr<ISpanned> sp = ISpanned::Probe(text);
        ArrayOf<IInterface*>* spansTemp = NULL;
        sp->GetSpans(start, end, EIID_IInterface, &spansTemp);
        ArrayOf<IInterface*>& spans = *spansTemp;

        for (Int32 i = 0; i < spans.GetLength(); i++) {
            if (INoCopySpan::Probe(spans[i])) {
                continue;
            }

            Int32 st;
            sp->GetSpanStart(spans[i], &st);
            st -= start;
            Int32 en;
            sp->GetSpanEnd(spans[i], &en);
            en -= start;
            Int32 fl;
            sp->GetSpanFlags(spans[i], &fl);

            if (st < 0)
                st = 0;
            if (st > end - start)
                st = end - start;

            if (en < 0)
                en = 0;
            if (en > end - start)
                en = end - start;

            SetSpan(spans[i], st, en, fl);
        }
        FreeArray(spansTemp);
    }

    return NOERROR;
}

void SpannableStringBuilder::Char32ArrayToChar8Array(
    /* [in]*/ const ArrayOf<Char32>& src,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in]*/ ArrayOf<Char8>& dst,
    /* [in]*/ Int32 dstOffset)
{
    for (Int32 i = start; i < end; ++i) {
        Int32 bytes;
        ASSERT_SUCCEEDED(Character::ToChars(src[i], dst, dstOffset, &bytes));
        dstOffset += bytes;
    }
}
