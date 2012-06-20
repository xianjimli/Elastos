
#include "text/SpannableStringInternal.h"
#include "utils/ArrayUtils.h"
#include "utils/AutoString.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::System;

const ArrayOf<IInterface*>* SpannableStringInternal::EMPTY = ArrayOf<IInterface*>::Alloc(0);
const Int32 SpannableStringInternal::START;
const Int32 SpannableStringInternal::END;
const Int32 SpannableStringInternal::FLAGS;
const Int32 SpannableStringInternal::COLUMNS;

SpannableStringInternal::SpannableStringInternal()
    : mSpans(NULL)
    , mSpanData(NULL)
{}

SpannableStringInternal::SpannableStringInternal(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
    : mSpans(NULL)
    , mSpanData(NULL)
{
    ASSERT_SUCCEEDED(Init(source, start, end));
}

SpannableStringInternal::~SpannableStringInternal()
{
    String::Free(mText);
    if (mSpans != NULL) {
        for (Int32 i = 0; i < mSpans->GetLength(); i++) {
            (*mSpans)[i]->Release();
        }
        ArrayOf<IInterface*>::Free(mSpans);
    }
    if (mSpanData != NULL) ArrayOf<Int32>::Free(mSpanData);
}

Int32 SpannableStringInternal::GetLength()
{
    return mText.GetCharCount();
}

Char32 SpannableStringInternal::GetCharAt(
    /* [in] */ Int32 i)
{
    return mText.GetChar(i);
}

ECode SpannableStringInternal::ToString(
    /* [out] */ String* str)
{
    assert(str != NULL);
    *str = String::Duplicate(mText);
    return NOERROR;
}

ECode SpannableStringInternal::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ BufferOf<Byte>* dest,
    /* [in] */ Int32 off)
{
    StringBuf buf((char*)dest->GetPayload() + off, dest->GetCapacity() - off);
    mText.Substring(start, end, buf);
    return NOERROR;
}

/* package */
ECode SpannableStringInternal::SetSpan(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    Int32 nstart = start;
    Int32 nend = end;

    FAIL_RETURN(CheckRange("SetSpan", start, end));

    if ((flags & Spanned_SPAN_PARAGRAPH) == Spanned_SPAN_PARAGRAPH) {
        if (start != 0 && start != GetLength()) {
            Char32 c = GetCharAt(start - 1);

            if (c != '\n') {
//                throw new RuntimeException(
//                        "PARAGRAPH span must start at paragraph boundary" +
//                        " (" + start + " follows " + c + ")");
                return E_RUNTIME_EXCEPTION;
            }
        }

        if (end != 0 && end != GetLength()) {
            Char32 c = GetCharAt(end - 1);

            if (c != '\n') {
//                throw new RuntimeException(
//                        "PARAGRAPH span must end at paragraph boundary" +
//                        " (" + end + " follows " + c + ")");
                return E_RUNTIME_EXCEPTION;
            }
        }
    }

    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& data = *mSpanData;

    for (Int32 i = 0; i < count; i++) {
        if (spans[i]->Probe(EIID_IInterface)
                == what->Probe(EIID_IInterface)) {
            Int32 ostart = data[i * COLUMNS + START];
            Int32 oend = data[i * COLUMNS + END];

            data[i * COLUMNS + START] = start;
            data[i * COLUMNS + END] = end;
            data[i * COLUMNS + FLAGS] = flags;

            SendSpanChanged(what, ostart, oend, nstart, nend);
            return NOERROR;
        }
    }

    if (mSpanCount + 1 >= mSpans->GetLength()) {
        Int32 newsize = ArrayUtils::IdealInt32ArraySize(mSpanCount + 1);
        ArrayOf<IInterface*>* newtags = ArrayOf<IInterface*>::Alloc(newsize);
        ArrayOf<Int32>* newdata = ArrayOf<Int32>::Alloc(newsize * 3);

        for (Int32 i = 0; i < mSpanCount; i++) {
            (*newtags)[i] = (*mSpans)[i];
        }
        for (Int32 i = 0; i < mSpanCount * 3; i++) {
            (*newdata)[i] = (*mSpanData)[i];
        }

        ArrayOf<IInterface*>::Free(mSpans);
        ArrayOf<Int32>::Free(mSpanData);
        mSpans = newtags;
        mSpanData = newdata;
    }

    (*mSpans)[mSpanCount] = what;
    if (what != NULL) what->AddRef();
    (*mSpanData)[mSpanCount * COLUMNS + START] = start;
    (*mSpanData)[mSpanCount * COLUMNS + END] = end;
    (*mSpanData)[mSpanCount * COLUMNS + FLAGS] = flags;
    mSpanCount++;

    if (this->Probe(EIID_ISpannable) != NULL) {
        SendSpanAdded(what, nstart, nend);
    }
    return NOERROR;
}

/* package */
ECode SpannableStringInternal::RemoveSpan(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& data = *mSpanData;

    for (Int32 i = count - 1; i >= 0; i--) {
        if (spans[i]->Probe(EIID_IInterface)
                == what->Probe(EIID_IInterface)) {
            Int32 ostart = data[i * COLUMNS + START];
            Int32 oend = data[i * COLUMNS + END];

            Int32 c = count - (i + 1);

            spans[i]->Release();
            for (Int32 j = 0; j < c; j++) {
                spans[i + j] = spans[i + 1 + j];
            }
            for (Int32 j = 0; j < c * COLUMNS; j++) {
                data[i * COLUMNS + j] = data[(i + 1) * COLUMNS + j];
            }

            mSpanCount--;

            SendSpanRemoved(what, ostart, oend);
            return NOERROR;
        }
    }
    return NOERROR;
}

Int32 SpannableStringInternal::GetSpanStart(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& data = *mSpanData;

    for (Int32 i = count - 1; i >= 0; i--) {
        if (spans[i]->Probe(EIID_IInterface)
                == what->Probe(EIID_IInterface)) {
            return data[i * COLUMNS + START];
        }
    }

    return -1;
}

Int32 SpannableStringInternal::GetSpanEnd(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& data = *mSpanData;

    for (Int32 i = count - 1; i >= 0; i--) {
        if (spans[i]->Probe(EIID_IInterface)
                == what->Probe(EIID_IInterface)) {
            return data[i * COLUMNS + END];
        }
    }

    return -1;
}

Int32 SpannableStringInternal::GetSpanFlags(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& data = *mSpanData;

    for (Int32 i = count - 1; i >= 0; i--) {
        if (spans[i]->Probe(EIID_IInterface)
                == what->Probe(EIID_IInterface)) {
            return data[i * COLUMNS + FLAGS];
        }
    }

    return 0;
}

void FreeArray(
    /* [in] */ ArrayOf<IInterface*>* arr)
{
    if (arr == NULL) return;

    for (Int32 i = 0; i < arr->GetLength(); i++) {
        if ((*arr)[i] != NULL) {
            (*arr)[i]->Release();
        }
    }
}

ECode SpannableStringInternal::GetSpans(
    /* [in] */ Int32 queryStart,
    /* [in] */ Int32 queryEnd,
    /* [in] */ const InterfaceID& kind,
    /* [out, callee] */ ArrayOf<IInterface*>** objs)
{
    assert(objs != NULL);

    Int32 count = 0;

    Int32 spanCount = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& data = *mSpanData;
    ArrayOf<IInterface*>* ret = NULL;
    AutoPtr<IInterface> ret1;

    for (Int32 i = 0; i < spanCount; i++) {
        Int32 spanStart = data[i * COLUMNS + START];
        Int32 spanEnd = data[i * COLUMNS + END];

        if (spanStart > queryEnd) {
            continue;
        }
        if (spanEnd < queryStart) {
            continue;
        }

        if (spanStart != spanEnd && queryStart != queryEnd) {
            if (spanStart == queryEnd) {
                continue;
            }
            if (spanEnd == queryStart) {
                continue;
            }
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

            Int32 prio = data[i * COLUMNS + FLAGS] & Spanned_SPAN_PRIORITY;
            if (prio != 0) {
                Int32 j;

                for (j = 0; j < count; j++) {
                    Int32 p = GetSpanFlags((*ret)[j]) & Spanned_SPAN_PRIORITY;

                    if (prio > p) {
                        break;
                    }
                }

                for (Int32 k = 0; k < count - j; k++) {
                    (*ret)[j + 1] = (*ret)[j];
                }
                (*ret)[j] = spans[i];
                if (spans[i] != NULL) spans[i]->AddRef();
                count++;
            } else {
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

Int32 SpannableStringInternal::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& kind)
{
    Int32 count = mSpanCount;
    ArrayOf<IInterface*>& spans = *mSpans;
    ArrayOf<Int32>& data = *mSpanData;

//    if (kind == null) {
//        kind = Object.class;
//    }

    for (Int32 i = 0; i < count; i++) {
        Int32 st = data[i * COLUMNS + START];
        Int32 en = data[i * COLUMNS + END];

        if (st > start && st < limit && spans[i]->Probe(kind) != NULL) {
            limit = st;
        }
        if (en > start && en < limit && spans[i]->Probe(kind) != NULL) {
            limit = en;
        }
    }

    return limit;
}

void SpannableStringInternal::SendSpanAdded(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    ArrayOf<IInterface*>* recip;
    GetSpans(start, end, EIID_ISpanWatcher, &recip);

    Int32 N = recip->GetLength();
    for (int i = 0; i < N; i++) {
        ISpanWatcher* span = ISpanWatcher::Probe((*recip)[i]);
        span->OnSpanAdded((ISpannable*)this->Probe(EIID_ISpannable), what, start, end);
        span->Release();
    }
    ArrayOf<IInterface*>::Free(recip);
}

void SpannableStringInternal::SendSpanRemoved(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    ArrayOf<IInterface*>* recip;
    GetSpans(start, end, EIID_ISpanWatcher, &recip);

    Int32 N = recip->GetLength();
    for (int i = 0; i < N; i++) {
        ISpanWatcher* span = ISpanWatcher::Probe((*recip)[i]);
        span->OnSpanRemoved((ISpannable*)this->Probe(EIID_ISpannable), what, start, end);
        span->Release();
    }
    ArrayOf<IInterface*>::Free(recip);
}

void SpannableStringInternal::SendSpanChanged(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 s,
    /* [in] */ Int32 e,
    /* [in] */ Int32 st,
    /* [in] */ Int32 en)
{
    ArrayOf<IInterface*>* recip;
    GetSpans(Math::Min(s, st), Math::Max(e, en), EIID_ISpanWatcher, &recip);

    Int32 N = recip->GetLength();
    for (int i = 0; i < N; i++) {
        ISpanWatcher* span = ISpanWatcher::Probe((*recip)[i]);
        span->OnSpanChanged((ISpannable*)this->Probe(EIID_ISpannable), what, s, e, st, en);
        span->Release();
    }
    ArrayOf<IInterface*>::Free(recip);
}

void SpannableStringInternal::Region(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    assert(str != NULL);
    StringBuffer sb("(");
    sb += start;
    sb += " ... ";
    sb += end;
    sb += ")";
    *str = String::Duplicate(sb);
}

ECode SpannableStringInternal::CheckRange(
    /* [in] */ String operation,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (end < start) {
//        throw new IndexOutOfBoundsException(operation + " " +
//                                            region(start, end) +
//                                            " has end before start");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 len = GetLength();

    if (start > len || end > len) {
//        throw new IndexOutOfBoundsException(operation + " " +
//                                            region(start, end) +
//                                            " ends beyond length " + len);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (start < 0 || end < 0) {
//        throw new IndexOutOfBoundsException(operation + " " +
//                                            region(start, end) +
//                                            " starts before 0");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return NOERROR;
}

ECode SpannableStringInternal::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    assert(source != NULL);

    Int32 len;
    source->GetLength(&len);
    if (start == 0 && end == len) {
        source->ToString(&mText);
    }
    else {
        AutoString tmp;
        source->ToString(&tmp);
        StringBuf* buf = StringBuf::Alloc(tmp.GetLength());
        tmp.Substring(start, end, *buf);
        mText = String::Duplicate(buf->GetPayload());
        StringBuf::Free(buf);
    }

    Int32 initial = ArrayUtils::IdealInt32ArraySize(0);
    mSpans = ArrayOf<IInterface*>::Alloc(initial);
    mSpanData = ArrayOf<Int32>::Alloc(initial * 3);

    if (ISpanned::Probe(source) != NULL) {
        AutoPtr<ISpanned> sp = ISpanned::Probe(source);
        ArrayOf<IInterface*>* spans;
        sp->GetSpans(start, end, EIID_IInterface, &spans);

        for (Int32 i = 0; i < spans->GetLength(); i++) {
            Int32 st, en, fl;
            sp->GetSpanStart((*spans)[i], &st);
            sp->GetSpanEnd((*spans)[i], &en);
            sp->GetSpanFlags((*spans)[i], &fl);

            if (st < start) {
                st = start;
            }
            if (en > end) {
                en = end;
            }

            SetSpan((*spans)[i], st - start, en - start, fl);
        }
    }
    return NOERROR;
}
