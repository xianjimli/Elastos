
#include "text/CSpannableStringBuilder.h"

ECode CSpannableStringBuilder::GetLength(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = SpannableStringBuilder::GetLength();

    return NOERROR;
}

ECode CSpannableStringBuilder::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);
    *c = SpannableStringBuilder::GetCharAt(index);

    return NOERROR;
}

ECode CSpannableStringBuilder::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    AutoPtr<ICharSequence> temp = SpannableStringBuilder::SubSequence(start, end);
    *csq = temp;
    if (*csq) {
        (*csq)->AddRef();
    }
    return NOERROR;
}

ECode CSpannableStringBuilder::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = SpannableStringBuilder::ToString();

    return NOERROR;
}

ECode CSpannableStringBuilder::GetSpans(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type,
    /* [out, callee] */ ArrayOf<IInterface *>** objs)
{
    VALIDATE_NOT_NULL(objs);
    return SpannableStringBuilder::GetSpans(start, end, type, objs);
}

ECode CSpannableStringBuilder::GetSpanStart(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start);
    *start = SpannableStringBuilder::GetSpanStart(tag);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetSpanEnd(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* end)
{
    VALIDATE_NOT_NULL(end);
    *end = SpannableStringBuilder::GetSpanEnd(tag);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetSpanFlags(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = SpannableStringBuilder::GetSpanFlags(tag);

    return NOERROR;
}

ECode CSpannableStringBuilder::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& type,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = SpannableStringBuilder::NextSpanTransition(start, limit, type);

    return NOERROR;
}

ECode CSpannableStringBuilder::SetSpan(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    return SpannableStringBuilder::SetSpan(what, start, end, flags);
}

ECode CSpannableStringBuilder::RemoveSpan(
    /* [in] */ IInterface* what)
{
    return SpannableStringBuilder::RemoveSpan(what);
}

ECode CSpannableStringBuilder::Replace(
    /* [in] */ Int32 st,
    /* [in] */ Int32 en,
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    AutoPtr<IEditable> temp = SpannableStringBuilder::Replace(
        st, en, source, start, end);
    *editable = temp;
    if (*editable) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode CSpannableStringBuilder::ReplaceEx(
    /* [in] */ Int32 st,
    /* [in] */ Int32 en,
    /* [in] */ ICharSequence* text,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    AutoPtr<IEditable> temp = SpannableStringBuilder::Replace(st, en, text);
    *editable = temp;
    if (*editable) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode CSpannableStringBuilder::Insert(
    /* [in] */ Int32 where,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    AutoPtr<IEditable> temp = SpannableStringBuilder::Insert(where, text, start, end);
    *editable = temp;
    if (*editable) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode CSpannableStringBuilder::InsertEx(
    /* [in] */ Int32 where,
    /* [in] */ ICharSequence* text,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    AutoPtr<IEditable> temp = SpannableStringBuilder::Insert(where, text);
    *editable = temp;
    if (*editable) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode CSpannableStringBuilder::Delete(
    /* [in] */ Int32 st,
    /* [in] */ Int32 en,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    AutoPtr<IEditable> temp = SpannableStringBuilder::Delete(st, en);
    *editable = temp;
    if (*editable) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode CSpannableStringBuilder::Append(
    /* [in] */ ICharSequence* text,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    AutoPtr<IEditable> temp = SpannableStringBuilder::Append(text);
    *editable = temp;
    if (*editable) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode CSpannableStringBuilder::AppendEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    AutoPtr<IEditable> temp = SpannableStringBuilder::Append(text, start, end);
    *editable = temp;
    if (*editable) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode CSpannableStringBuilder::AppendEx2(
    /* [in] */ Char32 text,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    AutoPtr<IEditable> temp = SpannableStringBuilder::Append(text);
    *editable = temp;
    if (*editable) {
        (*editable)->AddRef();
    }

    return NOERROR;
}

ECode CSpannableStringBuilder::Clear()
{
    return SpannableStringBuilder::Clear();
}

ECode CSpannableStringBuilder::ClearSpans()
{
    return SpannableStringBuilder::ClearSpans();
}

ECode CSpannableStringBuilder::SetFilters(
    /* [in] */ IObjectContainer* filters)
{
    return SpannableStringBuilder::SetFilters(filters);
}

ECode CSpannableStringBuilder::GetFilters(
    /* [out] */ IObjectContainer** filters)
{
    VALIDATE_NOT_NULL(filters);
    return SpannableStringBuilder::GetFilters(filters);
}

ECode CSpannableStringBuilder::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char8>* dest,
    /* [in] */ Int32 destoff)
{
    VALIDATE_NOT_NULL(dest);
    return SpannableStringBuilder::GetChars(start, end, dest, destoff);
}

ECode CSpannableStringBuilder::DrawText(
    /* [in] */ ICanvas* c,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* p)
{
    return SpannableStringBuilder::DrawText(c, start, end, x, y, p);
}

ECode CSpannableStringBuilder::MeasureText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaint* p,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    *width = SpannableStringBuilder::MeasureText(start, end, p);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetTextWidths(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Float>* widths,
    /* [in] */ IPaint* p,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(widths);
    VALIDATE_NOT_NULL(count);
    *count = SpannableStringBuilder::GetTextWidths(start, end, widths, p);

    return NOERROR;
}

ECode CSpannableStringBuilder::constructor()
{
    return SpannableStringBuilder::Init();
}

ECode CSpannableStringBuilder::constructor(
    /* [in] */ ICharSequence* source)
{
    return SpannableStringBuilder::Init(source);
}

ECode CSpannableStringBuilder::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return SpannableStringBuilder::Init(source, start, end);
}

PInterface CSpannableStringBuilder::Probe(
    /* [in] */ REIID riid)
{
    return _CSpannableStringBuilder::Probe(riid);
}
