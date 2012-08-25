
#ifndef __CSTATICLAYOUT_H__
#define __CSTATICLAYOUT_H__

#include "_CStaticLayout.h"
#include "text/StaticLayout.h"
#include "text/LayoutMacro.h"

CarClass(CStaticLayout), public StaticLayout
{
public:
    ILAYOUT_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 bufstart,
        /* [in] */ Int32 bufend,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 outerwidth,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    CARAPI constructor(
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
        /* [in] */ Int32 ellipsizedWidth);

    CARAPI constructor(
        /* [in] */ Boolean ellipsize);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSTATICLAYOUT_H__
