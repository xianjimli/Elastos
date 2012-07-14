
#ifndef __CBOOK_H__
#define __CBOOK_H__

#include "FBReaderDef.h"
#include "_CBook.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <shared_ptr.h>
#include <Book.h>
#include <BookModel.h>

typedef unsigned int size_t;
typedef unsigned char	utf8_t;		/**< Type for UTF-8 data points */
typedef unsigned short	utf16_t;	/**< Type for UTF-16 data points */
typedef unsigned int	utf32_t;	/**< Type for UTF-32 data points */

class LineBreaker;

CarClass(CBook)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 lineHeight,
	    /* [in] */ ITextPaint* textPaint);

    CARAPI GetTitle(
        /* [out] */ String* title);

    CARAPI GetPath(
        /* [out] */ String* path);

	CARAPI SetProperty(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 lineHeight,
	    /* [in] */ ITextPaint* textPaint);

	CARAPI GetTextNodeCount(
		/* [out] */ Int32* count);

	CARAPI GetTextNodeByIndex(
		/* [in] */ Int32 index,
		/* [out] */ String* nodeText);

	CARAPI GetTextNodeByChapterIndex(
		/* [in] */ Int32 chapterIndex,
		/* [out] */ Int32* pageIndex,
		/* [out] */ String* nodeText);

	CARAPI GetContentItemByIndex(
		/* [in] */ Int32 index,
		/* [out] */ String* itemText);

	CARAPI GetContentItemCount(
		/* [out] */ Int32* count);

private:
    CARAPI BuildChapters();
    CARAPI BuildTexts();
    CARAPI BuildContents();

private:
    static const String TAG;

    shared_ptr<Book> mBook;
    shared_ptr<BookModel> mBookModel;
    List< AutoPtr<IChapter> > mChapters;
    List< AutoPtr<IText> > mTexts;
    List<String> mContents;
	Int32 mContentItemCount;

    Int32 mChapterCount;
    Int32 mTextNodeCount;
    Int32 mWidth;
    Int32 mHeight;
	Int32 mLineHeight;

	AutoPtr<ITextPaint> 	mTextPaint;
};

#endif //__CBOOK_H__