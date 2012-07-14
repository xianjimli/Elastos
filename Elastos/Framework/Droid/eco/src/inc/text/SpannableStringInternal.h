
#ifndef __SPANNABLESTRINGINTERNAL_H__
#define __SPANNABLESTRINGINTERNAL_H__

#include "ext/frameworkext.h"

class SpannableStringInternal
{
public:
    SpannableStringInternal(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual ~SpannableStringInternal();

    virtual CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid) = 0;

    CARAPI_(Int32) GetLength();

    CARAPI_(Char32) GetCharAt(
        /* [in] */ Int32 i);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Char8>* dest,
        /* [in] */ Int32 off);

    /* package */
    CARAPI SetSpan(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    /* package */
    CARAPI RemoveSpan(
        /* [in] */ IInterface* what);

    virtual CARAPI_(Int32) GetSpanStart(
        /* [in] */ IInterface* what);

    virtual CARAPI_(Int32) GetSpanEnd(
        /* [in] */ IInterface* what);

    virtual CARAPI_(Int32) GetSpanFlags(
        /* [in] */ IInterface* what);

    virtual CARAPI GetSpans(
        /* [in] */ Int32 queryStart,
        /* [in] */ Int32 queryEnd,
        /* [in] */ const InterfaceID& kind,
        /* [out, callee] */ ArrayOf<IInterface*>** objs);

    virtual CARAPI_(Int32) NextSpanTransition(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ const InterfaceID& kind);

protected:
    SpannableStringInternal();

    CARAPI Init(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

private:
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

    static CARAPI_(void) Region(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ String* str);

    CARAPI CheckRange(
        /* [in] */ const char* operation,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

public:
    /* package */
    static const ArrayOf<IInterface*>* EMPTY;

private:
    String mText;
    ArrayOf<IInterface*>* mSpans;
    ArrayOf<Int32>* mSpanData;
    Int32 mSpanCount;

    static const Int32 START = 0;
    static const Int32 END = 1;
    static const Int32 FLAGS = 2;
    static const Int32 COLUMNS = 3;
};

#endif //__SPANNABLESTRINGINTERNAL_H__
