
#include "CTextAttribute.h"
ECode CTextAttribute::ReadResolve(
    /* [out] */ IInterface ** ppResolve)
{
    return TextAttribute::ReadResolve(ppResolve);
}

ECode CTextAttribute::GetName(
    /* [out] */ String * pName)
{
    return TextAttribute::GetName(pName);
}

ECode CTextAttribute::constructor(
    /* [in] */ const String& name)
{
    return TextAttribute::Init(name);
}

