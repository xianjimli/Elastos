#include "RuleBasedBreakIterator.h"
PInterface RuleBasedBreakIterator::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

RuleBasedBreakIterator::RuleBasedBreakIterator()
{
    //BreakIterator(iterator);
}

RuleBasedBreakIterator::~RuleBasedBreakIterator()
{}

RuleBasedBreakIterator::RuleBasedBreakIterator(
    /* [in] */ NativeBreakIterator* iterator)
{
}

ECode RuleBasedBreakIterator::Current(
        /* [out] */ Int32* currentValue)
{
    VALIDATE_NOT_NULL(currentValue);
    *currentValue = wrapped->Current();
    return NOERROR;
}

ECode RuleBasedBreakIterator::First(
        /* [out] */ Int32* firstValue)
{
    VALIDATE_NOT_NULL(firstValue);
    *firstValue = wrapped->First();
    return NOERROR;
}

ECode RuleBasedBreakIterator::Following(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* followingValue)
{
    VALIDATE_NOT_NULL(followingValue);
    ValidateOffset(offset);
    *followingValue = wrapped->Following(offset);
    return NOERROR;
}

ECode RuleBasedBreakIterator::ValidateOffset(
        /* [in] */ Int32 offset)
{
    AutoPtr<ICharacterIterator> it = wrapped->GetText();
    Int32 beginIndex, endIndex;
    it->GetBeginIndex(&beginIndex);
    it->GetEndIndex(&endIndex);
    if (offset < beginIndex || offset >= endIndex) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode RuleBasedBreakIterator::GetText(
        /* [out] */ ICharacterIterator** text)
{
    *text = wrapped->GetText();
    return NOERROR;
}

ECode RuleBasedBreakIterator::Last(
        /* [out] */ Int32* lastValue)
{
    VALIDATE_NOT_NULL(lastValue);
    *lastValue = wrapped->Last();
    return NOERROR;
}

ECode RuleBasedBreakIterator::Next(
        /* [out] */ Int32* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    *nextValue = wrapped->Next();
    return NOERROR;
}

ECode RuleBasedBreakIterator::NextEx(
        /* [in] */ Int32 n,
        /* [out] */ Int32* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    *nextValue = wrapped->Next(n);
    return NOERROR;
}

ECode RuleBasedBreakIterator::Previous(
        /* [out] */ Int32* previousValue)
{
    VALIDATE_NOT_NULL(previousValue);
    *previousValue = wrapped->Previous();
    return NOERROR;
}

ECode RuleBasedBreakIterator::SetTextEx(
        /* [in] */ ICharacterIterator* newText)
{
    // call a method to check if null pointer
    Char32 current;
    newText->Current(&current);
    wrapped->SetText(newText);
    return NOERROR;
}

ECode RuleBasedBreakIterator::IsBoundary(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* isBoundary)
{
    VALIDATE_NOT_NULL(isBoundary);
    ValidateOffset(offset);
    *isBoundary = wrapped->IsBoundary(offset);
    return NOERROR;
}

/*
 * (non-Javadoc)
 *
 * @see java.text.BreakIterator#preceding(int)
 */
//@Override
ECode RuleBasedBreakIterator::Preceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* precedingValue) 
{
    VALIDATE_NOT_NULL(precedingValue);
    ValidateOffset(offset);
    *precedingValue = wrapped->Preceding(offset);
    return NOERROR;
}
