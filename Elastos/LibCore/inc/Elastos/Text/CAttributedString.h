
#ifndef __CATTRIBUTEDSTRING_H__
#define __CATTRIBUTEDSTRING_H__

#include "_CAttributedString.h"
#include "AttributedString.h"

CarClass(CAttributedString), public AttributedString
{
public:
    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator* iterator);

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IObjectContainer* attributes);

    CARAPI constructor(
        /* [in] */ const String& value);

    CARAPI AddAttribute(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ IInterface* value);

    CARAPI AddAttributeEx(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ IInterface* value,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI GetIterator(
        /* [out] */ IAttributedCharacterIterator** iterator);

    CARAPI GetIteratorEx(
        /* [in] */ IObjectContainer* attributes,
        /* [out] */ IAttributedCharacterIterator** iterator);

    CARAPI GetIteratorEx2(
        /* [in] */ IObjectContainer* attributes,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IAttributedCharacterIterator** iterator);

private:
    // TODO: Add your private member variables here.
};

#endif // __CATTRIBUTEDSTRING_H__
