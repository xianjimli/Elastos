
#ifndef __CFOREGROUNDCOLORSPAN_H__
#define __CFOREGROUNDCOLORSPAN_H__

#include "_CForegroundColorSpan.h"

CarClass(CForegroundColorSpan)
{
public:
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* tp);

    CARAPI GetUnderlying(
        /* [out] */ ICharacterStyle** result);

    CARAPI GetSpanTypeId(
        /* [out] */ Int32* result);

    CARAPI GetForegroundColor(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ Int32 color);

    CARAPI constructor(
        /* [in] */ IParcel* src);

private:
    // TODO: Add your private member variables here.
};

#endif // __CFOREGROUNDCOLORSPAN_H__
