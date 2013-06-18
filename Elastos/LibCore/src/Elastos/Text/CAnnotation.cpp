
#include "CAnnotation.h"

ECode CAnnotation::constructor(
    /* [in] */ IInterface* attribute)
{
    return Annotation::Init(attribute);
}

ECode CAnnotation::GetValue(
    /* [out] */ IInterface** value)
{
    return Annotation::GetValue(value);
}


