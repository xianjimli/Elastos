
#include "ext/frameworkdef.h"
#include "provider/CSettingsNameValueTable.h"


ECode CSettingsNameValueTable::GetUriFor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& name,
    /* [out] */ IUri** value)
{
    VALIDATE_NOT_NULL(value);
    return Settings::NameValueTable::GetUriFor(uri, name, value);
}
