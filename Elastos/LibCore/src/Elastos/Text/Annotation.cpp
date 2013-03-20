#include "Annotation.h"

ECode Annotation::Init(
        /* [in] */ IInterface* attribute)
{
    value = attribute;
    return NOERROR;
}

ECode Annotation::GetValue(
        /* [out] */ IInterface** v)
{
    *v = value;
    return NOERROR;
}