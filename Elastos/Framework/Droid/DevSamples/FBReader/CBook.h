
#ifndef __CBOOK_H__
#define __CBOOK_H__

#include "FBReaderDef.h"
#include "_CBook.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <shared_ptr.h>
#include <Book.h>
#include <BookModel.h>

CarClass(CBook)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI GetTitle(
        /* [out] */ String* title);

    CARAPI GetPath(
        /* [out] */ String* path);

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
    List< AutoPtr<IText> > mContents;

    Int32 mChapterCount;
    Int32 mTextNodeCount;
    Int32 mWidth;
    Int32 mHeight;
};

#endif //__CBOOK_H__