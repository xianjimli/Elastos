
#ifndef __CBACKGROUNDCOLORSPAN_H__
#define __CBACKGROUNDCOLORSPAN_H__

#include "_CBackgroundColorSpan.h"

CarClass(CBackgroundColorSpan)
{
public:
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* tp);

    CARAPI GetUnderlying(
        /* [out] */ ICharacterStyle** result);

    CARAPI GetSpanTypeId(
        /* [out] */ Int32* result);

    CARAPI GetBackgroundColor(
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

#endif // __CBACKGROUNDCOLORSPAN_H__
