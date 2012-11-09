
#include "CChapter.h"
#include "CTextFactory.h"
#include <stdio.h>

CChapter::CChapter()
    : mParagraphCount(0)
    , mText(NULL)
    , mStartPageIndex(0)
{}

ECode CChapter::Append(
    /* [in] */ IParagraph* paragraph)
{
    assert(paragraph != NULL);

    mParagraphs.PushBack(paragraph);
    mParagraphCount++;

    return NOERROR;
}

ECode CChapter::BuildTexts(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 lineHeight,
    /* [in] */ ITextPaint* textPaint,
    /* [out] */ IObjectContainer** texts)
{
    assert(width > 0 && height > 0);

    if (mTextFactory == NULL) {
        CTextFactory::AcquireSingleton((ITextFactory**)&mTextFactory);
    }

    return mTextFactory->ProcessText(mText, width, height, lineHeight, textPaint, texts);
}

ECode CChapter::Flush()
{
    Int32 size = 0;
    List< AutoPtr<IParagraph> >::Iterator it;
    for (it = mParagraphs.Begin(); it != mParagraphs.End(); ++it) {
        Int32 len;
        (*it)->GetLength(&len);
        size += len;
    }

    assert(mText == NULL);
    mText = ArrayOf<Byte>::Alloc(size + 1);
    (*mText)[size] = '\0';

    Int32 offset = 0;
    for (it = mParagraphs.Begin(); it != mParagraphs.End(); ++it) {
        Int32 len = 0;
        ArrayOf<Byte>* data;
        (*it)->GetData((Handle32*)&data);
        if (data != NULL) {
            len = data->GetLength();
            memcpy(mText->GetPayload() + offset, data->GetPayload(), len);
        }
        offset += len;
    }

    return NOERROR;
}

ECode CChapter::GetOrder(
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order);
    *order = mOrder;
    return NOERROR;
}

ECode CChapter::SetOrder(
    /*[in]*/ Int32 order)
{
	//printf("==== File: %s, Line: %d ==== order  = [%d]\n", __FILE__, __LINE__, order);

    mOrder = order;
    return NOERROR;
}

ECode CChapter::GetStartPageIndex(
    /* [out] */ Int32* pageIndex)
{
    VALIDATE_NOT_NULL(pageIndex);
    *pageIndex = mStartPageIndex;
    return NOERROR;
}

ECode CChapter::SetStartPageIndex(
    /*[in]*/ Int32 pageIndex)
{
	printf("==== File: %s, Line: %d ==== pageIndex  = [%d]\n", __FILE__, __LINE__, pageIndex);

    mStartPageIndex = pageIndex;
    return NOERROR;
}

