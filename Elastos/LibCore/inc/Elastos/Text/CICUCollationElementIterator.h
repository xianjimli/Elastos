
#ifndef __CICUCOLLATIONELEMENTITERATOR_H__
#define __CICUCOLLATIONELEMENTITERATOR_H__

#include "_CICUCollationElementIterator.h"
#include "ICUCollationElementIterator.h"

CarClass(CICUCollationElementIterator), public ICUCollationElementIterator
{
public:

    CARAPI constructor(
        /* [in] */ Int32 collelemiteratoraddress);

    CARAPI Reset();

    CARAPI Next(
        /* [out] */ Int32 * pNextValue);

    CARAPI Previous(
        /* [out] */ Int32 * pPreviousValue);

    CARAPI GetMaxExpansion(
        /* [in] */ Int32 order,
        /* [out] */ Int32 * pMaxExpansion);

    CARAPI SetText(
        /* [in] */ const String& source);

    CARAPI SetTextEx(
        /* [in] */ ICharacterIterator * pSource);

    CARAPI GetOffset(
        /* [out] */ Int32 * pOffset);

    CARAPI SetOffset(
        /* [in] */ Int32 offset);


private:
    // TODO: Add your private member variables here.
};

#endif // __CICUCOLLATIONELEMENTITERATOR_H__
