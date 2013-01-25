#include "provider/CSettingsNameValueTable.h"
#include "provider/SettingsNameValueTable.h"
#include "content/CContentValues.h"
#include "net/Uri.h"
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode CSettingsNameValueTable::GetUriFor(
    /*[in]*/ IUri* uri,
    /*[in]*/ const String& name,
    /*[out]*/ IUri** outUri)
{
    return SettingsNameValueTable::GetUriFor(uri,name,outUri);
}

