
#include "CAttributedString.h"

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator * pIterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AttributedString::Init(pIterator, start, end);
}

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator * pIterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute * > * pAttributes)
{
    return AttributedString::Init(pIterator, start, end, pAttributes);
}

ECode CAttributedString::constructor(
    /* [in] */ const String& value)
{
    return AttributedString::Init(value);
}

ECode CAttributedString::AddAttribute(
    /* [in] */ IAttributedCharacterIterator_Attribute * pAttribute,
    /* [in] */ IInterface * pValue)
{
    return AttributedString::AddAttribute(pAttribute, pValue);
}

ECode CAttributedString::AddAttributeEx(
    /* [in] */ IAttributedCharacterIterator_Attribute * pAttribute,
    /* [in] */ IInterface * pValue,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AttributedString::AddAttributeEx(pAttribute, pValue, start, end);
}

ECode CAttributedString::GetIterator(
    /* [out] */ IAttributedCharacterIterator ** ppIterator)
{
    return AttributedString::GetIterator(ppIterator);
}

ECode CAttributedString::GetIteratorEx(
    /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute *> * pAttributes,
    /* [out] */ IAttributedCharacterIterator ** ppIterator)
{
    return AttributedString::GetIteratorEx(pAttributes, ppIterator);
}

ECode CAttributedString::GetIteratorEx2(
    /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute *> * pAttributes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IAttributedCharacterIterator ** ppIterator)
{
    return AttributedString::GetIteratorEx2(pAttributes, start, end, ppIterator);
}


