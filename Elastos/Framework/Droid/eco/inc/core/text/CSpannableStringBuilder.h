
#ifndef __CSPANNABLESTRINGBUILDER_H__
#define __CSPANNABLESTRINGBUILDER_H__

#include "_CSpannableStringBuilder.h"
#include "text/SpannableStringBuilder.h"

CarClass(CSpannableStringBuilder), public SpannableStringBuilder
{
public:
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

    CARAPI Replace(
        /* [in] */ Int32 st,
        /* [in] */ Int32 en,
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IEditable** editable);

    CARAPI ReplaceEx(
        /* [in] */ Int32 st,
        /* [in] */ Int32 en,
        /* [in] */ ICharSequence* text,
        /* [out] */ IEditable** editable);

    CARAPI Insert(
        /* [in] */ Int32 where,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IEditable** editable);

    CARAPI InsertEx(
        /* [in] */ Int32 where,
        /* [in] */ ICharSequence* text,
        /* [out] */ IEditable** editable);

    CARAPI Delete(
        /* [in] */ Int32 st,
        /* [in] */ Int32 en,
        /* [out] */ IEditable** editable);

    CARAPI Append(
        /* [in] */ ICharSequence* text,
        /* [out] */ IEditable** editable);

    CARAPI AppendEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IEditable** editable);

    CARAPI AppendEx2(
        /* [in] */ Char32 text,
        /* [out] */ IEditable** editable);

    CARAPI Clear();

    CARAPI ClearSpans();

    CARAPI SetFilters(
        /* [in] */ IObjectContainer* filters);

    CARAPI GetFilters(
        /* [out] */ IObjectContainer** filters);

    CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Char8>* dest,
        /* [in] */ Int32 destoff);

    CARAPI DrawText(
        /* [in] */ ICanvas* c,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* p);

    CARAPI MeasureText(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaint* p,
        /* [out] */ Float* width);

    CARAPI GetTextWidths(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Float>* widths,
        /* [in] */ IPaint* p,
        /* [out] */ Int32* count);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICharSequence* source);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSPANNABLESTRINGBUILDER_H__
