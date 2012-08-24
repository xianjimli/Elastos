
#ifndef __CCHAPTER_H__
#define __CCHAPTER_H__

#include "FBReaderDef.h"
#include "_CChapter.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
//#include "CTextFactory.h"

CarClass(CChapter)
{
public:
    CChapter();

    CARAPI Append(
        /* [in] */ IParagraph* paragraph);

    CARAPI BuildTexts(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 lineHeight,
	    /* [in] */ ITextPaint* textPaint,
        /* [out] */ IObjectContainer** texts);

    CARAPI Flush();

    CARAPI GetOrder(
        /* [out] */ Int32* order);

    CARAPI SetOrder(
        /*[in]*/ Int32 order);

    CARAPI GetStartPageIndex(
        /* [out] */ Int32* pageIndex);

    CARAPI SetStartPageIndex(
        /*[in]*/ Int32 pageIndex);

private:
    List< AutoPtr<IParagraph> > mParagraphs;
    Int32 mParagraphCount;
    ArrayOf<Byte>* mText;

    AutoPtr<ITextFactory> mTextFactory;

    Int32 mOrder;
	Int32 mStartPageIndex;
};

#endif //__CCHAPTER_H__
