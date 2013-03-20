
#include "CAnnotation.h"

ECode CAnnotation::constructor(
    /* [in] */ IInterface * pAttribute)
{
    return Annotation::Init(pAttribute);
}

ECode CAnnotation::GetValue(
    /* [out] */ IInterface ** ppValue)
{
    return Annotation::GetValue(ppValue);
}


