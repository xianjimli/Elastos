#include "CollationKey.h"
CollationKey::CollationKey(){}

ECode CollationKey::Init(
    /* [in] */ String source)
{
    this->source = String(source);
    return NOERROR;
}

ECode CollationKey::GetSourceString(
    /* [out] */ String* sourceString)
{
    VALIDATE_NOT_NULL(sourceString);
    *sourceString = String(source);
    return NOERROR;
}