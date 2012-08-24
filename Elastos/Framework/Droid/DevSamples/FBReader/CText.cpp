
#include "CText.h"
#include "FBReaderDef.h"
#include <stdio.h>

CText::CText()
    : mIndex(-1)
{}

CText::~CText()
{
    //String::Free(mText);
}

ECode CText::constructor()
{
    return NOERROR;
}

ECode CText::constructor(
    /* [in] */ const String& text,
    /* [in] */ Int32 index)
{
    mText = text;
    mIndex = index;
    return NOERROR;
}

ECode CText::GetText(
    /* [out] */ String* text)
{
    VALIDATE_NOT_NULL(text);

	*text = mText;
    return NOERROR;
}

ECode CText::GetOrder(
	/* [out] */  Int32* order)
{
    VALIDATE_NOT_NULL(order);

	*order = mIndex;
	return NOERROR;
}


