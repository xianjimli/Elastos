
#ifndef __CATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__
#define __CATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__

#include "_CAttributedCharacterIteratorAttribute.h"
#include "AttributedCharacterIteratorAttribute.h"

CarClass(CAttributedCharacterIteratorAttribute), public AttributedCharacterIteratorAttribute
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI GetClassID(
        /* [out] */ ClassID* clsid);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI ReadResolve(
        /* [out] */ IInterface** resolve);
};

#endif // __CATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__
