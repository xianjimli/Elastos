#include "CCharsetHelper.h"
#include "Charset.h"

ECode CCharsetHelper::DefaultCharset(
    /* [out] */ICharset** charset)
{
    return Charset::DefaultCharset(charset);
}