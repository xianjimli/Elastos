
#include "CNumberFormatField.h"


ECode CNumberFormatField::constructor(
    /* [in] */ const String& name)
{
    return NumberFormat::Field::Init(name);
}

PInterface CNumberFormatField::Probe(
    /* [in]  */ REIID riid)
{
    return _CNumberFormatField::Probe(riid);
}

ECode CNumberFormatField::GetClassID(
    /* [out] */ ClassID* clsid)
{
    return _CNumberFormatField::GetClassID(clsid);
}

ECode CNumberFormatField::GetName(
    /* [out] */ String* name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
