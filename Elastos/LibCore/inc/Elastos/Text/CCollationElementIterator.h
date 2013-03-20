
#ifndef __CCOLLATIONELEMENTITERATOR_H__
#define __CCOLLATIONELEMENTITERATOR_H__

#include "_CCollationElementIterator.h"
#include "CollationElementIterator.h"
CarClass(CCollationElementIterator), public CollationElementIterator
{
public:

    CARAPI constructor(
        /* [in] */ IICUCollationElementIterator * pIterator);

    CARAPI GetMaxExpansion(
        /* [in] */ Int32 order,
        /* [out] */ Int32 * pMaxExpansion);

    CARAPI GetOffset(
        /* [out] */ Int32 * pOffset);

    CARAPI Next(
        /* [out] */ Int32 * pNextValue);

    CARAPI Previous(
        /* [out] */ Int32 * pPreviousValue);

    CARAPI Reset();

    CARAPI SetOffset(
        /* [in] */ Int32 newOffset);

    CARAPI SetTextEx(
        /* [in] */ ICharacterIterator * pSource);

    CARAPI SetText(
        /* [in] */ const String& source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCOLLATIONELEMENTITERATOR_H__
