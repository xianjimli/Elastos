
#ifndef __CSPANNABLESTRING_H__
#define __CSPANNABLESTRING_H__

#include "_CSpannableString.h"
#include "text/SpannableStringInternal.h"

CarClass(CSpannableString), public SpannableStringInternal
{
public:
    CARAPI constructor(
        /* [in] */ ICharSequence* source);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetLength(
        /* [out] */ Int32* number);

    CARAPI GetCharAt(
        /* [in] */ Int32 index,
        /* [out] */ Char32* c);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Char8>* dest,
        /* [in] */ Int32 destoff);

    CARAPI GetSpans(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const InterfaceID& type,
        /* [out, callee] */ ArrayOf<IInterface*>** objs);

    CARAPI GetSpanStart(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* start);

    CARAPI GetSpanEnd(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* end);

    CARAPI GetSpanFlags(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* flags);

    CARAPI NextSpanTransition(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ const InterfaceID& type,
        /* [out] */ Int32* offset);

    CARAPI SetSpan(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    CARAPI RemoveSpan(
        /* [in] */ IInterface* what);
};

#endif //__CSPANNABLESTRING_H__