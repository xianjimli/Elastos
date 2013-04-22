
#include "CMessageFormatField.h"
ECode CMessageFormatField::GetName(
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMessageFormatField::constructor(
    /* [in] */ const String& name)
{
    return MessageFormat::MessageFormatField::Init(name);
}

