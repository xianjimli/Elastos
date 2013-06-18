
#include "RuleBasedBreakIterator.h"

RuleBasedBreakIterator::~RuleBasedBreakIterator()
{}

RuleBasedBreakIterator::RuleBasedBreakIterator(
    /* [in] */ NativeBreakIterator* iterator)
{
    //BreakIterator(iterator);
}

PInterface RuleBasedBreakIterator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IBreakIterator*)this;
    }
    else if (riid == EIID_IBreakIterator) {
        return (IBreakIterator*)this;
    }
    return NULL;
}

UInt32 RuleBasedBreakIterator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 RuleBasedBreakIterator::Release()
{
    return ElRefBase::Release();
}

ECode RuleBasedBreakIterator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IBreakIterator*)this) {
        *pIID = EIID_IBreakIterator;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode RuleBasedBreakIterator::Current(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mWrapped->Current();
    return NOERROR;
}

ECode RuleBasedBreakIterator::First(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mWrapped->First();
    return NOERROR;
}

ECode RuleBasedBreakIterator::Following(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    FAIL_RETURN(ValidateOffset(offset));
    *position = mWrapped->Following(offset);
    return NOERROR;
}

ECode RuleBasedBreakIterator::ValidateOffset(
    /* [in] */ Int32 offset)
{
    AutoPtr<ICharacterIterator> it = mWrapped->GetText();
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
    VALIDATE_NOT_NULL(text);
    *text = mWrapped->GetText();
    return NOERROR;
}

ECode RuleBasedBreakIterator::Last(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mWrapped->Last();
    return NOERROR;
}

ECode RuleBasedBreakIterator::Next(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mWrapped->Next();
    return NOERROR;
}

ECode RuleBasedBreakIterator::NextEx(
    /* [in] */ Int32 n,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mWrapped->Next(n);
    return NOERROR;
}

ECode RuleBasedBreakIterator::Previous(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mWrapped->Previous();
    return NOERROR;
}

ECode RuleBasedBreakIterator::SetText(
    /* [in] */ const String& newText)
{
    return BreakIterator::SetText(newText);
}

ECode RuleBasedBreakIterator::SetTextEx(
    /* [in] */ ICharacterIterator* newText)
{
    // call a method to check if null pointer
    Char32 current;
    newText->Current(&current);
    mWrapped->SetText(newText);
    return NOERROR;
}

ECode RuleBasedBreakIterator::IsBoundary(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* isBoundary)
{
    VALIDATE_NOT_NULL(isBoundary);
    FAIL_RETURN(ValidateOffset(offset));
    *isBoundary = mWrapped->IsBoundary(offset);
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
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    FAIL_RETURN(ValidateOffset(offset));
    *position = mWrapped->Preceding(offset);
    return NOERROR;
}
