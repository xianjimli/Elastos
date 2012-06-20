
#ifndef __CCHAPTER_H__
#define __CCHAPTER_H__

#include "FBReaderDef.h"
#include "_CChapter.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>

CarClass(CChapter)
{
public:
    CChapter();

    CARAPI Append(
        /* [in] */ IParagraph* paragraph);

    CARAPI BuildTexts(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IObjectContainer** texts);

    CARAPI Flush();

    CARAPI GetOrder(
        /* [out] */ Int32* order);

    CARAPI SetOrder(
        /*[in]*/ Int32 order);

private:
    List< AutoPtr<IParagraph> > mParagraphs;
    Int32 mParagraphCount;
    ArrayOf<Byte>* mText;

    AutoPtr<ITextFactory> mTextFactory;

    Int32 mOrder;
};

#endif //__CCHAPTER_H__