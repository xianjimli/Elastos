#include "StringCharacterIterator.h"

ECode StringCharacterIterator::Init(
        /* [in] */ String value) {
    string = String(value);
    start = offset = 0;
    end = string.GetLength();
    return NOERROR;
}

ECode StringCharacterIterator::Init(
        /* [in] */ String value,
        /* [in] */ Int32 location)
{
    string = String(value);
    start = 0;
    end = string.GetLength();
    if (location < 0 || location > end) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    offset = location;
    return NOERROR;
}

ECode StringCharacterIterator::Init(
        /* [in] */ String value,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 location)
{
    string = value;
    if (start < 0 || end > (Int32)(string.GetLength()) || start > end
            || location < start || location > end) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    this->start = start;
    this->end = end;
    offset = location;
    return NOERROR;
}

ECode StringCharacterIterator::Current(
        /* [out] */ Char32* currentCharacter)
{
    VALIDATE_NOT_NULL(currentCharacter);
    if (offset == end) {
        //return DONE;
        //*index = '0xffff';
        return NOERROR;
    }
    *currentCharacter = string.GetChar(offset);
    return NOERROR;
}

ECode StringCharacterIterator::First(
        /* [out] */ Char32* firstCharacter)
{
    VALIDATE_NOT_NULL(firstCharacter);
    if (start == end) {
        //return DONE;
        //*index = '0xffff';
        return NOERROR;
    }
    offset = start;
    *firstCharacter = string.GetChar(offset);
    return NOERROR;
}

ECode StringCharacterIterator::GetBeginIndex(
        /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = start;
    return NOERROR;
}

ECode StringCharacterIterator::GetEndIndex(
        /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = end;
    return NOERROR;
}

ECode StringCharacterIterator::GetIndex(
        /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = offset;
    return NOERROR;
}

ECode StringCharacterIterator::Last(
        /* [out] */ Char32* endCharacter)
{
    VALIDATE_NOT_NULL(endCharacter);
    if (start == end) {
        //return DONE;
        //*index = '0xffff';
        return NOERROR;
    }
    offset = end - 1;
    *endCharacter = string.GetChar(offset);
    return NOERROR;
}

ECode StringCharacterIterator::Next(
        /* [out] */ Char32* nextCharacter)
{
    VALIDATE_NOT_NULL(nextCharacter);
    if (offset >= (end - 1)) {
        offset = end;
        //return DONE;
        //*index = '0xffff';
        return NOERROR;
    }
    *nextCharacter = string.GetChar(++offset);
    return NOERROR;
}

ECode StringCharacterIterator::Previous(
        /* [out] */ Char32* previousCharacter)
{
    VALIDATE_NOT_NULL(previousCharacter);
    if (offset == start) {
        //return DONE;
        //*index = '0xffff';
        return NOERROR;
    }
    *previousCharacter = string.GetChar(--offset);
    return NOERROR;
}

ECode StringCharacterIterator::SetIndex(
        /* [in] */ Int32 location,
        /* [out] */ Char32* index)
{
    VALIDATE_NOT_NULL(index);
    if (location < start || location > end) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    offset = location;
    if (offset == end) {
        //return DONE;
        //*index = '0xffff';
        return NOERROR;
    }
    *index = string.GetChar(offset);
    return NOERROR;
}

ECode StringCharacterIterator::SetText(
        /* [in] */ String value)
{
    string = value;
    start = offset = 0;
    end = value.GetLength();
    return NOERROR;
}