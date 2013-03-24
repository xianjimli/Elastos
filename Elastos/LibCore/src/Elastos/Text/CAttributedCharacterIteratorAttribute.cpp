
#include "cmdef.h"
#include "CAttributedCharacterIteratorAttribute.h"

ECode CAttributedCharacterIteratorAttribute::constructor(
    /* [in] */ const String& name)
{
    return AttributedCharacterIteratorAttribute::Init(name);
}

ECode CAttributedCharacterIteratorAttribute::GetClassID(
     /* [out] */ ClassID* clsid)
{
    return _CAttributedCharacterIteratorAttribute::GetClassID(clsid);
}

ECode CAttributedCharacterIteratorAttribute::ReadResolve(
    /* [out] */ IInterface** resolve)
{
    VALIDATE_NOT_NULL(resolve);

    return AttributedCharacterIteratorAttribute::ReadResolve(resolve);
}

ECode CAttributedCharacterIteratorAttribute::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    return AttributedCharacterIteratorAttribute::GetName(name);
}
