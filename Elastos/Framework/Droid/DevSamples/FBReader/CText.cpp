
#include "CText.h"

CText::CText()
    : mIndex(-1)
{}

CText::~CText()
{
    String::Free(mText);
}

ECode CText::constructor()
{
    return NOERROR;
}

ECode CText::constructor(
    /* [in] */ String text,
    /* [in] */ Int32 index)
{
    mText = text;
    mIndex = index;
    return NOERROR;
}
