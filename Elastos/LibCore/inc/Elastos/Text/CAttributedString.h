
#ifndef __CATTRIBUTEDSTRING_H__
#define __CATTRIBUTEDSTRING_H__

#include "_CAttributedString.h"
#include "AttributedString.h"
CarClass(CAttributedString), public AttributedString
{
public:

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator * pIterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator * pIterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute * > * pAttributes);

    CARAPI constructor(
        /* [in] */ const String& value);

    CARAPI AddAttribute(
        /* [in] */ IAttributedCharacterIterator_Attribute * pAttribute,
        /* [in] */ IInterface * pValue);

    CARAPI AddAttributeEx(
        /* [in] */ IAttributedCharacterIterator_Attribute * pAttribute,
        /* [in] */ IInterface * pValue,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI GetIterator(
        /* [out] */ IAttributedCharacterIterator ** ppIterator);

    CARAPI GetIteratorEx(
        /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute *> * pAttributes,
        /* [out] */ IAttributedCharacterIterator ** ppIterator);

    CARAPI GetIteratorEx2(
        /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute *> * pAttributes,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IAttributedCharacterIterator ** ppIterator);

private:
    // TODO: Add your private member variables here.
};

#endif // __CATTRIBUTEDSTRING_H__
