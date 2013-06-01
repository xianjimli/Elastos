
#include "CAttributedString.h"

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator)
{
    return AttributedString::Init(iterator);
}

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AttributedString::Init(iterator, start, end);
}

ECode CAttributedString::constructor(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IObjectContainer* attributes)
{
    return AttributedString::Init(iterator, start, end, attributes);
}

ECode CAttributedString::constructor(
    /* [in] */ const String& value)
{
    return AttributedString::Init(value);
}

ECode CAttributedString::AddAttribute(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value)
{
    return AttributedString::AddAttribute(attribute, value);
}

ECode CAttributedString::AddAttributeEx(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return AttributedString::AddAttributeEx(attribute, value, start, end);
}

ECode CAttributedString::GetIterator(
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIterator(iterator);
}

ECode CAttributedString::GetIteratorEx(
    /* [in] */ IObjectContainer* attributes,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIteratorEx(attributes, iterator);
}

ECode CAttributedString::GetIteratorEx2(
    /* [in] */ IObjectContainer* attributes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    return AttributedString::GetIteratorEx2(attributes, start, end, iterator);
}


