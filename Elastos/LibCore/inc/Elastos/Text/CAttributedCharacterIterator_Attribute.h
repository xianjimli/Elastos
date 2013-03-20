
#ifndef __CATTRIBUTEDCHARACTERITERATOR_ATTRIBUTE_H__
#define __CATTRIBUTEDCHARACTERITERATOR_ATTRIBUTE_H__

#include "_CAttributedCharacterIterator_Attribute.h"
#include "AttributedCharacterIterator_Attribute.h"
CarClass(CAttributedCharacterIterator_Attribute), public AttributedCharacterIterator_Attribute
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI ReadResolve(
        /* [out] */ IInterface ** ppResolve);

private:
    // TODO: Add your private member variables here.
};

#endif // __CATTRIBUTEDCHARACTERITERATOR_ATTRIBUTE_H__
