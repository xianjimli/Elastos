
#include "CRuleBasedBreakIterator.h"
ECode CRuleBasedBreakIterator::constructor()
{
    // RuleBasedBreakIterator::RuleBasedBreakIterator();
    return NOERROR;
}

ECode CRuleBasedBreakIterator::IsBoundary(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean * pIsBoundary)
{
    return RuleBasedBreakIterator::IsBoundary(offset, pIsBoundary);
}

ECode CRuleBasedBreakIterator::Preceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32 * pPrecedingValue)
{
    return RuleBasedBreakIterator::Preceding(offset, pPrecedingValue);
}

ECode CRuleBasedBreakIterator::SetText(
    /* [in] */ const String& newText)
{
    return RuleBasedBreakIterator::SetText(newText);
}

ECode CRuleBasedBreakIterator::Current(
    /* [out] */ Int32 * pCurrentValue)
{
    return RuleBasedBreakIterator::Current(pCurrentValue);
}

ECode CRuleBasedBreakIterator::First(
    /* [out] */ Int32 * pFirstValue)
{
    return RuleBasedBreakIterator::First(pFirstValue);
}

ECode CRuleBasedBreakIterator::Following(
    /* [in] */ Int32 offset,
    /* [out] */ Int32 * pFollowingValue)
{
    return RuleBasedBreakIterator::Following(offset, pFollowingValue);
}

ECode CRuleBasedBreakIterator::GetText(
    /* [out] */ ICharacterIterator ** ppText)
{
    return RuleBasedBreakIterator::GetText(ppText);
}

ECode CRuleBasedBreakIterator::Last(
    /* [out] */ Int32 * pLastValue)
{
    return RuleBasedBreakIterator::Last(pLastValue);
}

ECode CRuleBasedBreakIterator::Next(
    /* [out] */ Int32 * pNextValue)
{
    return RuleBasedBreakIterator::Next(pNextValue);
}

ECode CRuleBasedBreakIterator::NextEx(
    /* [in] */ Int32 n,
    /* [out] */ Int32 * pNextValue)
{
    return RuleBasedBreakIterator::NextEx(n, pNextValue);
}

ECode CRuleBasedBreakIterator::Previous(
    /* [out] */ Int32 * pPreviousValue)
{
    return RuleBasedBreakIterator::Previous(pPreviousValue);
}

ECode CRuleBasedBreakIterator::SetTextEx(
    /* [in] */ ICharacterIterator * pNewText)
{
    return RuleBasedBreakIterator::SetTextEx(pNewText);
}



