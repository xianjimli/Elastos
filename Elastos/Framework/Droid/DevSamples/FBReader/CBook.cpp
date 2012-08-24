
#include "CBook.h"
#include "CChapter.h"
#include "CParagraph.h"
#include "CText.h"
#include <Logger.h>
#include "ZLibrary.h"
#include "Book.h"

using namespace Elastos::Utility::Logging;

//static const std::string FILE_PATH = "/data/data/com.elastos.runtime/elastos/FBReader/Books/doyle-lost-world.epub";
//static const std::string FILE_PATH = "/data/data/com.elastos.runtime/elastos/FBReader/Books/MiniHelp.en.fb2";
static const std::string FILE_PATH = "/data/data/com.elastos.runtime/elastos/FBReader/Books/daodejing.epub";
//static const std::string FILE_PATH = "/data/data/com.elastos.runtime/elastos/FBReader/Books/MiniHelp.zh.fb2";

const String CBook::TAG("CBook");

ECode CBook::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 lineHeight,
    /* [in] */ ITextPaint* textPaint)
{
    assert(width > 0 && height > 0);
printf("----CBook....  1 .\n");
    mWidth = width;
    mHeight = height;
	mLineHeight = lineHeight;
	mTextNodeCount = 0;
	mTextPaint = textPaint;
	mContentItemCount = 0;

    int argc = 1;
    char **argv = NULL;
    if (!ZLibrary::init(argc, argv)) {
		printf("----CBook....  1---1 .\n");
        //Logger::E(TAG, "ZLibrary::init() fail!");
        return E_RUNTIME_EXCEPTION;
    }
	printf("----CBook....  2 .\n");

    mBook = Book::loadFromFile(FILE_PATH);
	printf("----CBook....  3 .\n");

    if (mBook.isNull()) {
        Logger::E(TAG, "fail Book::loadFromFile!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
	printf("----CBook....  4 .\n");

    mBookModel = new BookModel(mBook);
    if (mBookModel.isNull()) {
        Logger::E(TAG, "fail new BookModel!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
	printf("----CBook....  5 .\n");

//    const std::string& title = mBook->title();
//    Logger::I(TAG, title.c_str());
//
//    const std::string& filePath = mBook->filePath();
//    Logger::I(TAG, filePath.c_str());

    //First: Generate chapater node list.
    BuildChapters();
	printf("----CBook....  6 .\n");

    //Second: Generate a textNode list which one node can fill a page.
    BuildTexts();
	printf("----CBook....  7 .\n");

    BuildContents();
	printf("----CBook....  8 .\n");

    return NOERROR;
}

ECode CBook::SetProperty(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 lineHeight,
    /* [in] */ ITextPaint* textPaint)
{
	if (textPaint == NULL) return E_INVALID_ARGUMENT;
    mWidth = width;
    mHeight = height;
	mLineHeight = lineHeight;
	mTextNodeCount = 0;
	mContentItemCount = 0;
	mTextPaint = textPaint;

    //Second: Generate a textNode list which one node can fill a page.
    BuildTexts();

    return NOERROR;
}

ECode CBook::GetTitle(
    /* [out] */ String* title)
{
    VALIDATE_NOT_NULL(title);
    //*title = String::Duplicate(String(mBook->title().c_str()));
    *title = mBook->title().c_str();
    return NOERROR;
}

ECode CBook::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    //*path = String::Duplicate(String(mBook->filePath().c_str()));
    *path = mBook->filePath().c_str();
    return NOERROR;
}

//Make chapater node list.
ECode CBook::BuildChapters()
{
    printf("<%s, %d>\n",  __FILE__, __LINE__);
    shared_ptr<ZLTextModel> textModel = mBookModel->bookTextModel();
    shared_ptr<ZLTextModel> contentsModel = mBookModel->contentsModel();

    Int32 paraNum1 = textModel->paragraphsNumber();
    mChapterCount = contentsModel->paragraphsNumber();

    printf("<%s, %d>, [%d, %d]\n", __FILE__, __LINE__, paraNum1, mChapterCount);
    if (mChapterCount == 0 && paraNum1 > 0) {
        mChapterCount = 1;
    }

    const std::map<Int32, Int32>& chapterMap = mBook->getChapterMap();

    Int32 offset = 0;
    Int32 order = 0;
    for (std::map<Int32, Int32>::const_iterator it = chapterMap.begin();
            it != chapterMap.end(); ++it) {
        Int32 paraIndex = it->second;
//        printf("<%s, %d>, %d, paraIndex=[%d]\n",  __FILE__, __LINE__, offset, paraIndex);
        if (paraIndex == 0) {
            continue;
        }

        AutoPtr<IChapter> newChapter;
        CChapter::New((IChapter**)&newChapter);
        newChapter->SetOrder(order++);

        for (Int32 i = offset; i < paraIndex; i++) {
            const ZLTextParagraph* textParagraph = (*textModel)[i];

            AutoPtr<IParagraph> newParagraph;
            CParagraph::New(textParagraph->textDataLength(), (IParagraph**)&newParagraph);

            ZLTextParagraph::Iterator it(*textParagraph);
            while(!it.isEnd()) {
                switch (it.entryKind()) {
                    case ZLTextParagraphEntry::STYLE_ENTRY:
                        //printf("<%s, %d>, [STYLE_ENTRY]\n",  __FILE__, __LINE__);
                        break;
                    case ZLTextParagraphEntry::FIXED_HSPACE_ENTRY:
                        //printf("<%s, %d>, [FIXED_HSPACE_ENTRY]\n",  __FILE__, __LINE__);
                        break;
                    case ZLTextParagraphEntry::CONTROL_ENTRY:
                    case ZLTextParagraphEntry::HYPERLINK_CONTROL_ENTRY:
                    {
                        //printf("<%s, %d>, [CONTROL_ENTRY]----[%d]\n",  __FILE__, __LINE__, it.entryKind());
                        break;
                    }
                    case ZLTextParagraphEntry::IMAGE_ENTRY:
                    {
                        //printf("<%s, %d>, [IMAGE_ENTRY]\n",  __FILE__, __LINE__);
                        break;
                    }
                    case ZLTextParagraphEntry::TEXT_ENTRY:
                    {
                        //printf("<%s, %d>, [TEXT_ENTRY]\n",  __FILE__, __LINE__);
                        const ZLTextEntry& textEntry = (const ZLTextEntry&)*it.entry();
						//printf("<%s, %d>, [TEXT_ENTRY]-----text===[%s].\n",  __FILE__, __LINE__, textEntry.data());
                        newParagraph->Append(String(textEntry.data()), textEntry.dataLength());
                        break;
                    }
                    case ZLTextParagraphEntry::RESET_BIDI_ENTRY:
                        //printf("<%s, %d>, [RESET_BIDI_ENTRY]\n",  __FILE__, __LINE__);
                        break;
                }
                it.next();
            }
            newChapter->Append(newParagraph);
        }

        newChapter->Flush();
        mChapters.PushBack(newChapter);
        offset = paraIndex;
    }

    AutoPtr<IChapter> newChapter;
    CChapter::New((IChapter**)&newChapter);
    newChapter->SetOrder(order);

    if (offset < paraNum1) {
        printf("<%s, %d>, %d, paraNum1=[%d]\n",  __FILE__, __LINE__, offset, paraNum1);
        for (Int32 i = offset; i < paraNum1; i++) {
            const ZLTextParagraph* textParagraph = (*textModel)[i];

            AutoPtr<IParagraph> newParagraph;
            CParagraph::New(textParagraph->textDataLength(), (IParagraph**)&newParagraph);

            ZLTextParagraph::Iterator it(*textParagraph);
            while(!it.isEnd()) {
                switch (it.entryKind()) {
                    case ZLTextParagraphEntry::STYLE_ENTRY:
                        //printf("<%s, %d>, [STYLE_ENTRY]\n",  __FILE__, __LINE__);
                        break;
                    case ZLTextParagraphEntry::FIXED_HSPACE_ENTRY:
                        //printf("<%s, %d>, [FIXED_HSPACE_ENTRY]\n",  __FILE__, __LINE__);
                        break;
                    case ZLTextParagraphEntry::CONTROL_ENTRY:
                    case ZLTextParagraphEntry::HYPERLINK_CONTROL_ENTRY:
                        //printf("<%s, %d>, [CONTROL_ENTRY]----[%d]\n",  __FILE__, __LINE__, it.entryKind());
                        break;
                    case ZLTextParagraphEntry::IMAGE_ENTRY:
                    {
                        //printf("<%s, %d>, [IMAGE_ENTRY]\n",  __FILE__, __LINE__);
                        break;
                    }
                    case ZLTextParagraphEntry::TEXT_ENTRY:
                    {
                        //printf("<%s, %d>, [TEXT_ENTRY]\n",  __FILE__, __LINE__);
                        const ZLTextEntry& textEntry = (const ZLTextEntry&)*it.entry();
						//printf("<%s, %d>, [TEXT_ENTRY]-----text===[%s].\n",  __FILE__, __LINE__, textEntry.data());

                        newParagraph->Append(String(textEntry.data()), textEntry.dataLength());

                        break;
                    }
                    case ZLTextParagraphEntry::RESET_BIDI_ENTRY:
                        //printf("<%s, %d>, [RESET_BIDI_ENTRY]\n",  __FILE__, __LINE__);
                        break;
                }
                it.next();
            }
            newChapter->Append(newParagraph);
        }
    }

    newChapter->Flush();
    mChapters.PushBack(newChapter);
    return NOERROR;
}

//Generate a textNode list which one node can fill a page.
ECode CBook::BuildTexts()
{
    printf("mChapterCount==[%d], mWidth == [%d], mHeight == [%d]\n", mChapterCount, mWidth, mHeight);
    assert(mChapterCount > 0 && mWidth > 0 && mHeight > 0);
    List< AutoPtr<IChapter> >::Iterator it = mChapters.Begin();

	Int32 tmpChapterCount = 0;
    for (; it != mChapters.End(); ++it) {
        IChapter* chapter = *it;
        AutoPtr<IObjectContainer> texts;
		FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&texts));
        FAIL_RETURN(chapter->BuildTexts(mWidth, mHeight, mLineHeight, mTextPaint, (IObjectContainer**)&texts));
		FAIL_RETURN(chapter->SetOrder(tmpChapterCount));
		FAIL_RETURN(chapter->SetStartPageIndex(mTextNodeCount));
        AutoPtr<IObjectEnumerator> textsIt;
        texts->GetObjectEnumerator((IObjectEnumerator**)&textsIt);
        Boolean hasNext;
        while(textsIt->MoveNext(&hasNext), hasNext) {
            AutoPtr<IText> text;
            textsIt->Current((IInterface**)&text);
            mTexts.PushBack(text);
            mTextNodeCount++;
        }

		tmpChapterCount ++;
    }

	printf("==== File: %s, Line: %d ==== mTextNodeCount  = [%d]\n", __FILE__, __LINE__, mTextNodeCount);

    return NOERROR;
}

ECode CBook::BuildContents()
{
	typedef std::vector<shared_ptr<Content> > ContentList;
	ContentList myContents = mBook->contents();
	for (ContentList::iterator iterator = myContents.begin(); iterator != myContents.end(); ++ iterator) {
		if (!iterator->isNull()) {
			printf("==== File: %s, Line: %d ==== contentText  = [%s]\n", __FILE__, __LINE__, ((*iterator)->getText()).c_str());
			mContents.PushBack(String(((*iterator)->getText()).c_str()));

			mContentItemCount ++;
		}
	}

    return NOERROR;
}

ECode CBook::GetTextNodeCount(
	/* [out] */ Int32* count)
{
	assert(count != NULL);
	*count = mTextNodeCount;

	return NOERROR;
}

ECode CBook::GetTextNodeByIndex(
	/* [in] */ Int32 index,
	/* [out] */ String* nodeText)
{
	VALIDATE_NOT_NULL(nodeText);
	*nodeText = "";

	printf("==== File: %s, Line: %d  , index= [%d], mTextNodeCount = [%d]\n", __FILE__, __LINE__, index, mTextNodeCount);

	if (mTextNodeCount == 0 || index < 0 || index >= mTextNodeCount) {
		return E_INVALID_ARGUMENT;
	}

	List< AutoPtr<IText> >::Iterator it;

	Int32 pos = 0;
	for (it = mTexts.Begin(); it != mTexts.End(); it++) {
		AutoPtr<IText> pa = *it;
		if (pos == index) {
			(*it)->GetText(nodeText);
			break;
		}

		pos ++;
	}

	return NOERROR;
}

ECode CBook::GetTextNodeByChapterIndex(
	/* [in] */ Int32 chapterIndex,
	/* [out] */ Int32* pageIndex,
	/* [out] */ String* nodeText)
{
	VALIDATE_NOT_NULL(nodeText);
	VALIDATE_NOT_NULL(pageIndex);
	*nodeText = "";
	*pageIndex = 0;

	if (mTextNodeCount == 0 || chapterIndex < 0 || chapterIndex >= mChapterCount) {
		return E_INVALID_ARGUMENT;
	}

	Int32 chapterPos = 0;
	List< AutoPtr<IChapter> >::Iterator it = mChapters.Begin();
	for (; it != mChapters.End(); ++it) {
		if (chapterPos == chapterIndex) {
			IChapter* chapter = *it;
			Int32 startPageIndex = 0;

			FAIL_RETURN(chapter->GetStartPageIndex(&startPageIndex));

			*pageIndex = startPageIndex;
			return GetTextNodeByIndex(startPageIndex, nodeText);
		}

		chapterPos ++;
	}

	return E_INVALID_ARGUMENT;
}

ECode CBook::GetContentItemByIndex(
	/* [in] */ Int32 index,
	/* [out] */ String* itemText)
{
	VALIDATE_NOT_NULL(itemText);

	if (index < 0 || index >= mContentItemCount) {
		return E_INVALID_ARGUMENT;
	}

	List<String>::Iterator it = mContents.Begin();
	Int32 tmpIndex = 0;
	for (; it != mContents.End(); ++it) {
		if (tmpIndex == index) {
			*itemText = (*it);
			return NOERROR;
		}

		tmpIndex ++;
	}

	return E_INVALID_ARGUMENT;
}

ECode CBook::GetContentItemCount(
	/* [out] */ Int32* count)
{
	VALIDATE_NOT_NULL(count);
	*count = mContentItemCount;

	return NOERROR;
}

