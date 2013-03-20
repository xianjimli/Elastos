
#ifndef __CRULEBASEDBREAKITERATOR_H__
#define __CRULEBASEDBREAKITERATOR_H__

#include "_CRuleBasedBreakIterator.h"
#include "RuleBasedBreakIterator.h"
CarClass(CRuleBasedBreakIterator), public RuleBasedBreakIterator
{
public:

    CARAPI constructor();

    CARAPI IsBoundary(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean * pIsBoundary);

    CARAPI Preceding(
        /* [in] */ Int32 offset,
        /* [out] */ Int32 * pPrecedingValue);

    CARAPI SetText(
        /* [in] */ const String& newText);

    CARAPI Current(
        /* [out] */ Int32 * pCurrentValue);

    CARAPI First(
        /* [out] */ Int32 * pFirstValue);

    CARAPI Following(
        /* [in] */ Int32 offset,
        /* [out] */ Int32 * pFollowingValue);

    CARAPI GetText(
        /* [out] */ ICharacterIterator ** ppText);

    CARAPI Last(
        /* [out] */ Int32 * pLastValue);

    CARAPI Next(
        /* [out] */ Int32 * pNextValue);

    CARAPI NextEx(
        /* [in] */ Int32 n,
        /* [out] */ Int32 * pNextValue);

    CARAPI Previous(
        /* [out] */ Int32 * pPreviousValue);

    CARAPI SetTextEx(
        /* [in] */ ICharacterIterator * pNewText);


private:
    // TODO: Add your private member variables here.
};

#endif // __CRULEBASEDBREAKITERATOR_H__
