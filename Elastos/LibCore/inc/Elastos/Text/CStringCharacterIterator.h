
#ifndef __CSTRINGCHARACTERITERATOR_H__
#define __CSTRINGCHARACTERITERATOR_H__

#include "_CStringCharacterIterator.h"
#include "StringCharacterIterator.h"
CarClass(CStringCharacterIterator), public StringCharacterIterator
{
public:
    CARAPI constructor(
        /* [in] */ const String& value);

    CARAPI constructor(
        /* [in] */ const String& value,
        /* [in] */ Int32 location);

    CARAPI constructor(
        /* [in] */ const String& value,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 location);

    CARAPI Clone(
        /* [out] */ IInterface ** ppShallowCopy);

    CARAPI Current(
        /* [out] */ Char32 * pCurrentCharacter);

    CARAPI First(
        /* [out] */ Char32 * pFirstCharacter);

    CARAPI GetBeginIndex(
        /* [out] */ Int32 * pBeginIndex);

    CARAPI GetEndIndex(
        /* [out] */ Int32 * pEndIndex);

    CARAPI GetIndex(
        /* [out] */ Int32 * pCurrentIndex);

    CARAPI Last(
        /* [out] */ Char32 * pLastCharacter);

    CARAPI Next(
        /* [out] */ Char32 * pNextCharacter);

    CARAPI Previous(
        /* [out] */ Char32 * pPreviousCharacter);

    CARAPI SetIndex(
        /* [in] */ Int32 location,
        /* [out] */ Char32 * pCharacter);

    CARAPI SetText(
        /* [in] */ const String& value);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSTRINGCHARACTERITERATOR_H__
