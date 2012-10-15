
#ifndef __SPANNABLESTRINGBUILDER_H__
#define __SPANNABLESTRINGBUILDER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

class SpannableStringBuilder
{
public:
    SpannableStringBuilder();

    virtual ~SpannableStringBuilder();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    static CARAPI_(AutoPtr<ISpannableStringBuilder>) ValueOf(
        /* [in] */ ICharSequence* source);

    CARAPI_(Char32) GetCharAt(
        /* [in] */ Int32 where);

    CARAPI_(Int32) GetLength();

    CARAPI_(AutoPtr<IEditable>) Insert(
        /* [in] */ Int32 where,
        /* [in] */ ICharSequence* tb,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(AutoPtr<IEditable>) Insert(
        /* [in] */ Int32 where,
        /* [in] */ ICharSequence* tb);

    CARAPI_(AutoPtr<IEditable>) Delete(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Clear();

    CARAPI ClearSpans();

    CARAPI_(AutoPtr<IEditable>) Append(
        /* [in] */ ICharSequence* text);

    CARAPI_(AutoPtr<IEditable>) Append(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(AutoPtr<IEditable>) Append(
        /* [in] */ Char32 text);

    CARAPI_(AutoPtr<IEditable>) Replace(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ICharSequence* tb);

    CARAPI_(AutoPtr<IEditable>) Replace(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ICharSequence* tb,
        /* [in] */ Int32 tbstart,
        /* [in] */ Int32 tbend);

    CARAPI SetSpan(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    CARAPI RemoveSpan(
        /* [in] */ IInterface* what);

    CARAPI_(Int32) GetSpanStart(
        /* [in] */ IInterface* what);

    CARAPI_(Int32) GetSpanEnd(
        /* [in] */ IInterface* what);

    CARAPI_(Int32) GetSpanFlags(
        /* [in] */ IInterface* what);

    CARAPI GetSpans(
        /* [in] */ Int32 queryStart,
        /* [in] */ Int32 queryEnd,
        /* [in] */ const InterfaceID& kind,
        /* [out, callee] */ ArrayOf<IInterface*>** objs);

    CARAPI_(Int32) NextSpanTransition(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ const InterfaceID& kind);

    CARAPI_(AutoPtr<ICharSequence>) SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Char8>* dest,
        /* [in] */ Int32 destoff);

    CARAPI_(String) ToString();

    CARAPI DrawText(
        /* [in] */ ICanvas* c,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* p);

    CARAPI_(Float) MeasureText(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaint* p);

    CARAPI_(Int32) GetTextWidths(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [in] */ IPaint* p);

    CARAPI SetFilters(
        /* [in] */ IObjectContainer* filters);

    CARAPI GetFilters(
        /* [out] */ IObjectContainer** filters);

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ ICharSequence* text);

    CARAPI Init(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

private:
    CARAPI_(void) ResizeFor(
        /* [in] */ Int32 size);

    CARAPI_(void) MoveGapTo(
        /* [in] */ Int32 where);

    CARAPI_(Int32) Change(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ICharSequence* tb,
        /* [in] */ Int32 tbstart,
        /* [in] */ Int32 tbend);

    CARAPI_(Int32) Change(
        /* [in] */ Boolean notify,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ICharSequence* tb,
        /* [in] */ Int32 tbstart,
        /* [in] */ Int32 tbend);

    CARAPI SetSpan(
        /* [in] */ Boolean send,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    CARAPI_(ArrayOf<AutoPtr<ITextWatcher> >*) SendTextWillChange(
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    CARAPI_(void) SendTextChange(
        /* [in] */ const ArrayOf<AutoPtr<ITextWatcher> >& recip,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    CARAPI_(void) SendTextHasChanged(
        /* [in] */ const ArrayOf<AutoPtr<ITextWatcher> >& recip);

    CARAPI_(void) SendSpanAdded(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) SendSpanRemoved(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) SendSpanChanged(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 s,
        /* [in] */ Int32 e,
        /* [in] */ Int32 st,
        /* [in] */ Int32 en);

    static CARAPI_(String) Region(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckRange(
        /* [in] */ const String& operation,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Boolean) Isprint(
        /* [in] */ Char32 c);

    CARAPI_(void) FreeArray(
        /* [in] */ ArrayOf<IInterface*>* arr);

    CARAPI_(void) Char32ArrayToChar8Array(
        /* [in]*/ const ArrayOf<Char32>& src,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in]*/ ArrayOf<Char8>& dst,
        /* [in]*/ Int32 dstOffset);

private:
    //static ArrayOf<AutoPtr<IInputFilter> >* NO_FILTERS = new InputFilter[0];
    //ArrayOf<AutoPtr<IInputFilter> >* mFilters = NO_FILTERS;

    ArrayOf<Char32>* mText;
    Int32 mGapStart;
    Int32 mGapLength;

    ArrayOf<IInterface*>* mSpans;
    ArrayOf<Int32>* mSpanStarts;
    ArrayOf<Int32>* mSpanEnds;
    ArrayOf<Int32>* mSpanFlags;
    Int32 mSpanCount;

    static const Int32 MARK = 1;
    static const Int32 POINT = 2;
    static const Int32 PARAGRAPH = 3;

    static const Int32 START_MASK = 0xF0;
    static const Int32 END_MASK = 0x0F;
    static const Int32 START_SHIFT = 4;
    static AutoPtr<ICharSequence> EMPTY_CS;
};
#endif//__SPANNABLESTRINGBUILDER_H__
