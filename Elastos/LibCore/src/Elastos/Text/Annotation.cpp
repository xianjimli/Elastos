#include "Annotation.h"

ECode Annotation::Init(
        /* [in] */ IInterface* attribute)
{
    value = attribute;
    value->AddRef();
    return NOERROR;
}

ECode Annotation::GetValue(
        /* [out] */ IInterface** v)
{
    *v = value;
    (*v)->AddRef();
    return NOERROR;
}