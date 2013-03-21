
#include "CBidi.h"

ECode CBidi::constructor(
    /* [in] */ IAttributedCharacterIterator * pParagraph)
{
    return Bidi::Init(pParagraph);
}

ECode CBidi::constructor(
    /* [in] */ ArrayOf<Char32> * pTexts,
    /* [in] */ Int32 textStart,
    /* [in] */ ArrayOf<Byte> * pEmbeddings,
    /* [in] */ Int32 embStart,
    /* [in] */ Int32 paragraphLength,
    /* [in] */ Int32 flags)
{
    return Bidi::Init(pTexts, textStart, pEmbeddings, embStart, paragraphLength, flags);
}

ECode CBidi::constructor(
    /* [in] */ const String& paragraph,
    /* [in] */ Int32 flags)
{
    return Bidi::Init(paragraph, flags);
}

ECode CBidi::BaseIsLeftToRight(
    /* [out] */ Boolean * pBaseLevel)
{
    return Bidi::BaseIsLeftToRight(pBaseLevel);
}

ECode CBidi::CreateLineBidi(
    /* [in] */ Int32 lineStart,
    /* [in] */ Int32 lineLimit,
    /* [out] */ IBidi ** ppObject)
{
    return Bidi::CreateLineBidi(lineStart, lineLimit, ppObject);
}

ECode CBidi::GetBaseLevel(
    /* [out] */ Int32 * pBaseLevel)
{
    return Bidi::GetBaseLevel(pBaseLevel);
}

ECode CBidi::GetLength(
    /* [out] */ Int32 * pLength)
{
    return Bidi::GetLength(pLength);
}

ECode CBidi::SetLength(
        /* [in] */ Int32 length)
{
    return Bidi::SetLength(length);    
}

ECode CBidi::GetOffsetLevel(
    /* [out] */ ArrayOf<Byte>** ppOffsetLevel)
{
    return Bidi::GetOffsetLevel(ppOffsetLevel);    
}

ECode CBidi::SetOffsetLevel(
    /* [in] */ ArrayOf<Byte>* pOffsetLevel)
{
    return Bidi::SetOffsetLevel(pOffsetLevel);    
}

ECode CBidi::SetUnidirectional(
    /* [in] */ Boolean unidirectional)
{
    return Bidi::SetUnidirectional(unidirectional);    
}

ECode CBidi::GetLevelAt(
    /* [in] */ Int32 offset,
    /* [out] */ Int32 * pLevel)
{
    return Bidi::GetLevelAt(offset, pLevel);
}

ECode CBidi::GetRunCount(
    /* [out] */ Int32 * pRunCount)
{
    return Bidi::GetRunCount(pRunCount);
}

ECode CBidi::GetRunLevel(
    /* [in] */ Int32 run,
    /* [out] */ Int32 * pRunLevel)
{
    return Bidi::GetRunLevel(run, pRunLevel);
}

ECode CBidi::GetRunLimit(
    /* [in] */ Int32 run,
    /* [out] */ Int32 * pRunLimit)
{
    return Bidi::GetRunLimit(run, pRunLimit);
}

ECode CBidi::GetRunStart(
    /* [in] */ Int32 run,
    /* [out] */ Int32 * pRunStart)
{
    return Bidi::GetRunStart(run, pRunStart);
}

ECode CBidi::IsLeftToRight(
    /* [out] */ Boolean * pIsLefttoRight)
{
    return Bidi::IsLeftToRight(pIsLefttoRight);
}

ECode CBidi::IsMixed(
    /* [out] */ Boolean * pIsMixed)
{
    return Bidi::IsMixed(pIsMixed);
}

ECode CBidi::IsRightToLeft(
    /* [out] */ Boolean * pIsRightToLeft)
{
    return Bidi::IsRightToLeft(pIsRightToLeft);
}