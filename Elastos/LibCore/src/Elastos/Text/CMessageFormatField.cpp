
#include "CMessageFormatField.h"

ECode CMessageFormatField::constructor(
    /* [in] */ const String& name)
{
    return MessageFormat::MessageFormatField::Init(name);
}

ECode CMessageFormatField::GetClassID(
    /* [out] */ ClassID* clsid)
{
    return CMessageFormatField::GetClassID(clsid);
}

ECode CMessageFormatField::GetName(
    /* [out] */ String* name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
