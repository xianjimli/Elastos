
#include "CBook.h"
#include "CChapter.h"
#include "CParagraph.h"
#include "CText.h"
#include <Logger.h>
#include "ZLibrary.h"
#include "Book.h"

using namespace Elastos::Utility::Logging;

static const std::string FILE_PATH = "/data/data/com.elastos.runtime/elastos/FBReader/Books/doyle-lost-world.epub";

const String CBook::TAG = "CBook";

ECode CBook::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    assert(width > 0 && height > 0);

    mWidth = width;
    mHeight = height;

    int argc = 1;
    char **argv = NULL;
    if (!ZLibrary::init(argc, argv)) {
        Logger::E(TAG, "ZLibrary::init() fail!");
        return E_RUNTIME_EXCEPTION;
    }

    mBook = Book::loadFromFile(FILE_PATH);

    if (mBook.isNull()) {
        Logger::E(TAG, "fail Book::loadFromFile!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mBookModel = new BookModel(mBook);
    if (mBookModel.isNull()) {
        Logger::E(TAG, "fail new BookModel!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

//    const std::string& title = mBook->title();
//    Logger::I(TAG, title.c_str());
//
//    const std::string& filePath = mBook->filePath();
//    Logger::I(TAG, filePath.c_str());

    //First: Generate chapater node list.
    BuildChapters();

    //Second: Generate a textNode list which one node can fill a page.
    BuildTexts();

    BuildContents();

    return NOERROR;
}

ECode CBook::GetTitle(
    /* [out] */ String* title)
{
    VALIDATE_NOT_NULL(title);
    *title = String::Duplicate(String(mBook->title().c_str()));
    return NOERROR;
}

ECode CBook::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = String::Duplicate(String(mBook->filePath().c_str()));
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
//        printf("<%s, %d>, %d, paraNum1=[%d]\n",  __FILE__, __LINE__, offset, paraNum1);
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
    assert(mChapterCount > 0 && mWidth > 0 && mHeight > 0);
    List< AutoPtr<IChapter> >::Iterator it = mChapters.Begin();
    for (; it != mChapters.End(); ++it) {
        IChapter* chapter = *it;
        AutoPtr<IObjectContainer> texts;
        FAIL_RETURN(chapter->BuildTexts(mWidth, mHeight, (IObjectContainer**)&texts));
        AutoPtr<IObjectEnumerator> textsIt;
        texts->GetObjectEnumerator((IObjectEnumerator**)&textsIt);
        Boolean hasNext;
        while(textsIt->MoveNext(&hasNext), hasNext) {
            AutoPtr<IText> text;
            textsIt->Current((IInterface**)&text);
            mTexts.PushBack(text);
            mTextNodeCount++;
        }
    }
    return NOERROR;
}

ECode CBook::BuildContents()
{
    shared_ptr<ZLTextModel> contentsModel = mBookModel->contentsModel();
    Int32 contentCount = contentsModel->paragraphsNumber();

    /*
    * -1 : the cover of book;
    *  0 : the content of book;
    *  x : the chapter number of book.
    */
    Int32 chapterIndex = -1;
    for(Int32 i = 0; i < contentCount; i ++) {
        ZLTextParagraph::Iterator it(*(*contentsModel)[i]);

        // TODO: the content item is single paragraph.
        switch (it.entryKind()) {
            case ZLTextParagraphEntry::TEXT_ENTRY:
            {
                const ZLTextEntry& textEntry = (const ZLTextEntry&)*it.entry();
                //printf("<%s, %d>, [TEXT_ENTRY], text==[%s]\n",  __FILE__, __LINE__, textEntry.data());
                AutoPtr<IText> text;
                CText::New(String(textEntry.data()), chapterIndex, (IText**)&text);
                mContents.PushBack(text);

                break;
            }
            default: {
                //printf("<%s, %d>, entryKind = [%d]\n",  __FILE__, __LINE__, it.entryKind());
            }
        }

        chapterIndex++;
    }
    return NOERROR;
}
