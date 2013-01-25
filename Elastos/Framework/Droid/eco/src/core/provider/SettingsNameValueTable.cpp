#include "provider/SettingsNameValueTable.h"
#include "net/Uri.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

Boolean SettingsNameValueTable::PutString(
        /*[in]*/ IContentResolver* resolver,
        /*[in]*/ IUri* uri,
        /*[in]*/ const String& name,
        /*[in]*/ const String& value)
{
    // The database will take care of replacing duplicates.
    //try {
        AutoPtr<IContentValues> values;
        CContentValues::New((IContentValues**)&values);
        values->PutString(String(SettingsNameValueTable_NAME), name);
        values->PutString(String(SettingsNameValueTable_VALUE), value);
        AutoPtr<IUri> outUri;
        resolver->Insert(uri, values,(IUri**)&outUri);
        return TRUE;
    //} catch (/*SQLException e*/...) {
    //    Slogger::W(CSettings::TAG, StringBuffer("Can't set key ") + name + StringBuffer(" in ") + uri);
    //    return FALSE;
    //  }

}

ECode SettingsNameValueTable::GetUriFor(
    /*[in]*/ IUri* uri,
    /*[in]*/ const String& name,
    /*[out]*/ IUri** outUri)
{

    Uri::WithAppendedPath(uri, name,outUri);
    return NOERROR;

}
