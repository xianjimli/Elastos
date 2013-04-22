
#include "CNumberFormatField.h"


ECode CNumberFormatField::GetName(
    /* [out] */ String * name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CNumberFormatField::constructor(
    /* [in] */ const String& fieldName)
{
    return NumberFormat::Field::Init(fieldName);
}

