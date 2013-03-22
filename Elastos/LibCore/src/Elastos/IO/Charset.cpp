#include "Charset.h"

Charset::Charset()
{
}

ECode Charset::Init(
    /**in**/ String canonicalName, 
    /**in**/const ArrayOf<String>& aliases)
{
    return NOERROR;
}

ECode Charset::DisplayName(
    /**out**/ String* name)
{
    return NOERROR;
}

ECode Charset::DefaultCharset(
    /**out**/ ICharset** charset)
{
    return NOERROR;
}