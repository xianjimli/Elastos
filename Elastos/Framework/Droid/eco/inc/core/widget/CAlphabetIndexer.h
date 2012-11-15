
#ifndef __CALPHABETINDEXER_H__
#define __CALPHABETINDEXER_H__

#include "_CAlphabetIndexer.h"
#include "widget/AlphabetIndexer.h"

CarClass(CAlphabetIndexer), public AlphabetIndexer
{
public:
    CARAPI OnChanged();

    CARAPI OnInvalidated();

    CARAPI SetCursor(
        /* [in] */ ICursor * pCursor);

    CARAPI GetSections(
        /* [out] */ IObjectContainer ** ppSections);

    CARAPI GetPositionForSection(
        /* [in] */ Int32 section,
        /* [out] */ Int32 * pPosition);

    CARAPI GetSectionForPosition(
        /* [in] */ Int32 position,
        /* [out] */ Int32 * pSection);

    CARAPI constructor(
        /* [in] */ ICursor * pCursor,
        /* [in] */ Int32 sortedColumnIndex,
        /* [in] */ ICharSequence * pAlphabet);

private:
    // TODO: Add your private member variables here.
};

#endif // __CALPHABETINDEXER_H__
