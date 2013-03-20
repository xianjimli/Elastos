
#ifndef __CBIDI_H__
#define __CBIDI_H__

#include "_CBidi.h"
#include "Bidi.h"
CarClass(CBidi), public Bidi
{
public:
    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator * pParagraph);

    CARAPI constructor(
        /* [in] */ ArrayOf<Char32> * pTexts,
        /* [in] */ Int32 textStart,
        /* [in] */ ArrayOf<Byte> * pEmbeddings,
        /* [in] */ Int32 embStart,
        /* [in] */ Int32 paragraphLength,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ const String& paragraph,
        /* [in] */ Int32 flags);

    CARAPI BaseIsLeftToRight(
        /* [out] */ Boolean * pBaseLevel);

    CARAPI CreateLineBidi(
        /* [in] */ Int32 lineStart,
        /* [in] */ Int32 lineLimit,
        /* [out] */ IBidi ** ppObject);

    CARAPI GetBaseLevel(
        /* [out] */ Int32 * pBaseLevel);

    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI GetLevelAt(
        /* [in] */ Int32 offset,
        /* [out] */ Int32 * pLevel);

    CARAPI GetRunCount(
        /* [out] */ Int32 * pRunCount);

    CARAPI GetRunLevel(
        /* [in] */ Int32 run,
        /* [out] */ Int32 * pRunLevel);

    CARAPI GetRunLimit(
        /* [in] */ Int32 run,
        /* [out] */ Int32 * pRunLimit);

    CARAPI GetRunStart(
        /* [in] */ Int32 run,
        /* [out] */ Int32 * pRunStart);

    CARAPI IsLeftToRight(
        /* [out] */ Boolean * pIsLefttoRight);

    CARAPI IsMixed(
        /* [out] */ Boolean * pIsMixed);

    CARAPI IsRightToLeft(
        /* [out] */ Boolean * pIsRightToLeft);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBIDI_H__
