
#ifndef __CUNDERLINESPAN_H__
#define __CUNDERLINESPAN_H__

#include "_CUnderlineSpan.h"

CarClass(CUnderlineSpan)
{
public:
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* tp);

    CARAPI GetUnderlying(
        /* [out] */ ICharacterStyle** result);

    CARAPI GetSpanTypeId(
        /* [out] */ Int32* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* src);

private:
    // TODO: Add your private member variables here.
};

#endif // __CUNDERLINESPAN_H__
