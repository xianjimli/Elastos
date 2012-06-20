
#include "CChapter.h"
#include "CTextFactory.h"

CChapter::CChapter()
    : mParagraphCount(0)
    , mText(NULL)
{}

ECode CChapter::Append(
    /* [in] */ IParagraph* paragraph)
{
    assert(paragraph != NULL);

    mParagraphs.PushBack(paragraph);
    mParagraphCount++;

    //printf("<%s, %d>, m_paragraphCount=[%d]\n",  __FILE__, __LINE__, m_paragraphCount);
    return NOERROR;
}

ECode CChapter::BuildTexts(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IObjectContainer** texts)
{
    //printf("<%s, %d>, [%d], [%d]\n",  __FILE__, __LINE__, width, height);
    assert(width > 0 && height > 0);

    if (mTextFactory == NULL) {
        CTextFactory::AcquireSingleton((ITextFactory**)&mTextFactory);
    }

    //printf("<%s, %d>\n",  __FILE__, __LINE__);
    return mTextFactory->ProcessText(mText, width, height, texts);
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
            memcpy(mText->GetPayload() + offset, data, len);
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
    mOrder = order;
    return NOERROR;
}
