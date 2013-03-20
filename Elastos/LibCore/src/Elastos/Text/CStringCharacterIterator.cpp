
#include "CStringCharacterIterator.h"
ECode CStringCharacterIterator::constructor(
    /* [in] */ const String& value)
{
    return StringCharacterIterator::Init(value);
}

ECode CStringCharacterIterator::constructor(
    /* [in] */ const String& value,
    /* [in] */ Int32 location)
{
    return StringCharacterIterator::Init(value, location);
}

ECode CStringCharacterIterator::constructor(
    /* [in] */ const String& value,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 location)
{
    return StringCharacterIterator::Init(value, start, end, location);
}

ECode CStringCharacterIterator::Clone(
    /* [out] */ IInterface ** ppShallowCopy)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringCharacterIterator::Current(
    /* [out] */ Char32 * pCurrentCharacter)
{
    return StringCharacterIterator::Current(pCurrentCharacter);
}

ECode CStringCharacterIterator::First(
    /* [out] */ Char32 * pFirstCharacter)
{
    return StringCharacterIterator::First(pFirstCharacter);
}

ECode CStringCharacterIterator::GetBeginIndex(
    /* [out] */ Int32 * pBeginIndex)
{
    return StringCharacterIterator::GetBeginIndex(pBeginIndex);
}

ECode CStringCharacterIterator::GetEndIndex(
    /* [out] */ Int32 * pEndIndex)
{
    return StringCharacterIterator::GetEndIndex(pEndIndex);
}

ECode CStringCharacterIterator::GetIndex(
    /* [out] */ Int32 * pCurrentIndex)
{
    return StringCharacterIterator::GetIndex(pCurrentIndex);
}

ECode CStringCharacterIterator::Last(
    /* [out] */ Char32 * pLastCharacter)
{
    return StringCharacterIterator::Last(pLastCharacter);
}

ECode CStringCharacterIterator::Next(
    /* [out] */ Char32 * pNextCharacter)
{
    return StringCharacterIterator::Next(pNextCharacter);
}

ECode CStringCharacterIterator::Previous(
    /* [out] */ Char32 * pPreviousCharacter)
{
    return StringCharacterIterator::Previous(pPreviousCharacter);
}

ECode CStringCharacterIterator::SetIndex(
    /* [in] */ Int32 location,
    /* [out] */ Char32 * pCharacter)
{
    return StringCharacterIterator::SetIndex(location, pCharacter);
}

ECode CStringCharacterIterator::SetText(
    /* [in] */ const String& value)
{
    return StringCharacterIterator::SetText(value);
}