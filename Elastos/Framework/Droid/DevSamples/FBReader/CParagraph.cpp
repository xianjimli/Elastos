
#include "FBReaderDef.h"
#include "CParagraph.h"

#include <stdio.h>

const String CParagraph::INDENT("    ");

CParagraph::CParagraph()
    : mText(NULL)
    , mIndex(0)
{}

CParagraph::~CParagraph()
{
    ArrayOf<Byte>::Free(mText);
}

ECode CParagraph::constructor(
    /* [in] */ Int32 size)
{
    assert(size >= 0);

    if (size > 0) {
        mIndex = INDENT.GetLength();
        size += mIndex;
    }

    mText = ArrayOf<Byte>::Alloc(size + 1);
    assert(mText != NULL);

    if (size > 0) {
        memcpy(mText->GetPayload(), INDENT, INDENT.GetLength());
    }

    (*mText)[size] = '\n';
    return NOERROR;
}

ECode CParagraph::Append(
    /* [in] */ const String& text,
    /* [in] */ Int32 length)
{
//    printf("<%s, %d, [Append]>, [%d], [len==[%d]], , [m_length==[%d]], [%s]\n",  __FILE__, __LINE__, m_currentIndex, len, m_length, text);
    if (text.IsNullOrEmpty()) return NOERROR;

    //TODO: There is an character of '\n' come form fbreader engine, so it should be processed.
    if (length == 1 && text[0] == '\n') {
//        printf("<%s, %d, [Append]>, [text==[%d]]\n",  __FILE__, __LINE__, *text);
        if (mText->GetLength() >= 2) {
            (*mText)[mText->GetLength() - 2] = '\n';
            (*mText)[mText->GetLength() - 1] = '\r';
        }

        return NOERROR;
    }

    assert(length >= 0 && length <= mText->GetLength());
    assert(mIndex >= 0 && mIndex <= mText->GetLength());

    memcpy(mText->GetPayload() + mIndex, (const char*)text, length);
    mIndex += length;
	//printf("<%s, %s, %d, [Append]>, [text==[%s]], length = [%d]\n",  __FILE__, __FUNCTION__, __LINE__, mText->GetPayload(), length);

    return NOERROR;
}

ECode CParagraph::GetData(
    /* [out] */ Handle32* data)
{
    VALIDATE_NOT_NULL(data);
    *data = (Handle32)mText;
    return NOERROR;
}

ECode CParagraph::GetLength(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    if (mText != NULL) {
        *size = mText->GetLength();
    }
    else {
        *size = -1;
    }
    return NOERROR;
}
