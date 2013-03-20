#include "cmdef.h"
#include "CAttributedCharacterIterator_Attribute.h"

ECode CAttributedCharacterIterator_Attribute::ReadResolve(
    /* [out] */ IInterface ** ppResolve)
{
    return AttributedCharacterIterator_Attribute::ReadResolve(ppResolve);
}

ECode CAttributedCharacterIterator_Attribute::GetName(
    /* [out] */ String * pName)
{
    VALIDATE_NOT_NULL(pName);

    return AttributedCharacterIterator_Attribute::GetName(pName);
}

ECode CAttributedCharacterIterator_Attribute::constructor(
    /* [in] */ const String& name)
{
    AttributedCharacterIterator_Attribute::Init(name);
    return NOERROR;
}

