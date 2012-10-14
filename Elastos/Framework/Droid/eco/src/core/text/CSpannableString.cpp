
#include "text/CSpannableString.h"

ECode CSpannableString::constructor(
    /* [in] */ ICharSequence* source)
{
    VALIDATE_NOT_NULL(source);

    Int32 len;
    source->GetLength(&len);
    return SpannableStringInternal::Init(source, 0, len);
}

ECode CSpannableString::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(source);

    return SpannableStringInternal::Init(source, start, end);
}

PInterface CSpannableString::Probe(
    /* [in] */ REIID riid)
{
    return _CSpannableString::Probe(riid);
}

ECode CSpannableString::GetLength(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    *number = SpannableStringInternal::GetLength();
    return NOERROR;
}

ECode CSpannableString::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);

    *c = SpannableStringInternal::GetCharAt(index);
    return NOERROR;
}

ECode CSpannableString::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    return CSpannableString::New((ISpannable*)this, start, end, (ISpannable**)csq);
}

ECode CSpannableString::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return SpannableStringInternal::ToString(str);
}

ECode CSpannableString::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char8>* dest,
    /* [in] */ Int32 destoff)
{
    VALIDATE_NOT_NULL(dest);

    return SpannableStringInternal::GetChars(start, end, dest, destoff);
}

ECode CSpannableString::GetSpans(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type,
    /* [out, callee] */ ArrayOf<IInterface*>** objs)
{
    VALIDATE_NOT_NULL(objs);

    return SpannableStringInternal::GetSpans(start, end, type, objs);
}

ECode CSpannableString::GetSpanStart(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start);

    *start = SpannableStringInternal::GetSpanStart(tag);
    return NOERROR;
}

ECode CSpannableString::GetSpanEnd(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* end)
{
    VALIDATE_NOT_NULL(end);

    *end = SpannableStringInternal::GetSpanEnd(tag);
    return NOERROR;
}

ECode CSpannableString::GetSpanFlags(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);

    *flags = SpannableStringInternal::GetSpanFlags(tag);
    return NOERROR;
}

ECode CSpannableString::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& type,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    *offset = SpannableStringInternal::NextSpanTransition(start, limit, type);
    return NOERROR;
}

ECode CSpannableString::SetSpan(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    return SpannableStringInternal::SetSpan(what, start, end, flags);
}

ECode CSpannableString::RemoveSpan(
    /* [in] */ IInterface* what)
{
    return SpannableStringInternal::RemoveSpan(what);
}
