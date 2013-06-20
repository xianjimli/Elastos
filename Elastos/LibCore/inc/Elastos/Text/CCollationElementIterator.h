
#ifndef __CCOLLATIONELEMENTITERATOR_H__
#define __CCOLLATIONELEMENTITERATOR_H__

#include "_CCollationElementIterator.h"
#include <elastos/AutoPtr.h>

CarClass(CCollationElementIterator)
{
public:
    CARAPI constructor(
        /* [in] */ IICUCollationElementIterator* iterator);

    CARAPI GetMaxExpansion(
        /* [in] */ Int32 order,
        /* [out] */ Int32* maxExpansion);

    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    CARAPI Next(
        /* [out] */ Int32* nextValue);

    CARAPI Previous(
        /* [out] */ Int32* previousValue);

    CARAPI Reset();

    CARAPI SetOffset(
        /* [in] */ Int32 newOffset);

    CARAPI SetTextEx(
        /* [in] */ ICharacterIterator* source);

    CARAPI SetText(
        /* [in] */ const String& source);

private:
    AutoPtr<IICUCollationElementIterator> mIcuIterator;
};

#endif // __CCOLLATIONELEMENTITERATOR_H__
