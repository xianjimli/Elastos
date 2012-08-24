
#include "CTextFactory.h"
#include <stdio.h>
#include "CText.h"
#include <elastos/Character.h>


#define ASSERT_SUCCEEDED(expr) assert(SUCCEEDED(expr));


ECode CTextFactory::ProcessText(
    /* [in] */ ArrayOf<Byte>* text,
    /* [in] */ Int32 width,
	/* [in] */ Int32 height,
	/* [in] */ Int32 lineHeight,
    /* [in] */ ITextPaint* textPaint,
    /* [out] */ IObjectContainer** texts)
{
    if (text == NULL) return E_INVALID_ARGUMENT;
	assert(width > 0 && height > 0);

	mTextPaint = textPaint;
	assert(mTextPaint != NULL);

	mWidth = width;
	mHeight = height;

	LayoutAlignment alignment = LayoutAlignment_ALIGN_NORMAL;

	AutoPtr<ICharSequence> csq;
	String strText((const char*)text->GetPayload());
	CStringWrapper::New(strText, (ICharSequence**)&csq);

	ASSERT_SUCCEEDED(CStaticLayout::New(
		csq, mTextPaint, mWidth, alignment, mSpacingMult,
		mSpacingAdd, mIncludePad, (IStaticLayout**)&mLayout));

	assert(mLayout != NULL);
	Int32 lineCount = 0;
	mLayout->GetLineCount(&lineCount);	

	Int32 limitCount = height / lineHeight;
	if (limitCount == 0) limitCount = 1;

	Int32 pageIndex = 0;
	Int32 lineStart = 0, lineEnd = 0;
	Int32 sumStart = 0, sumEnd = 0;
	String nodeText("");
	String tempStr("");

	printf("new page==== File: %s, Line: %d ==== limitCount  = [%d], lineCount  = [%d]\n", __FILE__, __LINE__, limitCount, lineCount);

	if (lineCount <= limitCount) {
		AutoPtr<IText> textNode = NULL;
		for(Int32 i = 0; i < lineCount; i ++) {
			mLayout->GetLineStart(i, &lineStart);

			if (i + 1 < lineCount) {
				//Get char's index , not length.
				mLayout->GetLineStart(i + 1, &lineEnd);
			}

			if (lineEnd > lineStart) {
				Int32 startLen = 0, endLen = 0;
				System::Character::GetOffsetByChars(strText, 0, lineStart, &startLen);
				System::Character::GetOffsetByChars(strText, 0, lineEnd, &endLen);
				String subText = strText.Substring(startLen, endLen - startLen);
				nodeText.Append(subText);
			}
		}

		ASSERT_SUCCEEDED(CText::New(nodeText, pageIndex, (IText**)&textNode));
		nodeText = "";
		pageIndex ++;

		(*texts)->Add(textNode);
	}
	else {
		Int32 i = 0;
		for(i = 0; i < lineCount; i ++) {
			printf("new page==== File: %s, Line: %d ==== i  = [%d]\n", __FILE__, __LINE__, i);
			if (i == 0) {
				mLayout->GetLineStart(i, &sumStart);
			}

			if (i != 0 && i % limitCount == 0) {
				Int32 startLen = 0, endLen = 0;
				System::Character::GetOffsetByChars(strText, 0, sumStart, &startLen);
				System::Character::GetOffsetByChars(strText, 0, sumEnd, &endLen);
				nodeText = strText.Substring(startLen, endLen - startLen);

				printf("new page==== File: %s, Line: %d ==== sumStart  = [%d], sumEnd  = [%d]\n", __FILE__, __LINE__, sumStart, sumEnd);
				if (IsValidNodeText(nodeText) && endLen > startLen) {
					printf("new page==== File: %s, Line: %d ==== pageIndex  = [%d]\n", __FILE__, __LINE__, pageIndex);
					AutoPtr<IText> textNode = NULL;
					ASSERT_SUCCEEDED(CText::New(nodeText, pageIndex, (IText**)&textNode));

					textNode->GetText(&tempStr);
					(*texts)->Add(textNode);
				}

				pageIndex ++;

				mLayout->GetLineStart(i, &lineStart);

				sumStart = lineStart;
				sumEnd = 0;

				if (lineCount - pageIndex * limitCount < limitCount) {
					//last left lines, its count < limitCount.
					sumStart = 0;
					break;
				}
				else {
					//mLayout->GetLineStart(i, &sumStart);
				}
			}

			if (i + 1 < lineCount) {
				mLayout->GetLineStart(i + 1, &lineEnd);
				sumEnd = lineEnd;
			}
		}

		AutoPtr<IText> textNode = NULL;
		nodeText = "";
		for(i = pageIndex * limitCount; i < lineCount; i ++) {
			printf("new page==== File: %s, Line: %d ==== i  = [%d]\n", __FILE__, __LINE__, i);
			mLayout->GetLineStart(i, &lineStart);

			if (i + 1 < lineCount) {
				mLayout->GetLineStart(i + 1, &lineEnd);
			}

			if (lineEnd > lineStart) {
				Int32 startLen = 0, endLen = 0;
				System::Character::GetOffsetByChars(strText, 0, lineStart, &startLen);
				System::Character::GetOffsetByChars(strText, 0, lineEnd, &endLen);
				String subText = strText.Substring(startLen, endLen - startLen);

				nodeText.Append(subText);
			}
		}

		if (IsValidNodeText(nodeText)) {
			ASSERT_SUCCEEDED(CText::New(nodeText, pageIndex, (IText**)&textNode));
			nodeText = "";
			pageIndex ++;

			textNode->GetText(&tempStr);		
			(*texts)->Add(textNode);
		}

		printf("new page==== File: %s, Line: %d ==== pageIndex  = [%d]\n", __FILE__, __LINE__, pageIndex);
	}

    return NOERROR;
}

Boolean CTextFactory::IsValidNodeText(
	/* [in] */ const String& text)
{
	for (UInt32 i = 0; i < text.GetCharCount(); i ++) {		
		Char32 c = text.GetChar(i);

		if (c == '\n' || c == '\t' || c == ' ') {
			continue;
		}
		else {
			return TRUE;
		}
	}

	printf("==== File: %s, Line: %d ==== , FUNCTION == [%s].\n", __FILE__, __LINE__, __FUNCTION__);

	return FALSE;
}

