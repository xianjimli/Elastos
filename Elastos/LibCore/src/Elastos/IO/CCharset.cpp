#include "cmdef.h"
#include "CCharset.h"

ECode CCharset::constructor(
    /**in**/ String canonicalName, 
    /**in**/const ArrayOf<String>& aliases)
{
    return Charset::Init(canonicalName, aliases);
}

ECode CCharset::DisplayName(
    /**out**/ String* name)
{
    VALIDATE_NOT_NULL(name);

    return Charset::DisplayName(name);
}

ECode CCharset::DefaultCharset(
    /**out**/ ICharset** charset)
{
    return Charset::DefaultCharset(charset);
}