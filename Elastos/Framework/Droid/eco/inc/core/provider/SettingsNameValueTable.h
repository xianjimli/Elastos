#ifndef __SETTINGSNAMEVALUETABLE_H__
#define __SETTINGSNAMEVALUETABLE_H__

#include "Elastos.Framework.Core.h"

using namespace Elastos;

class SettingsNameValueTable
{
public:

   static CARAPI GetUriFor(
        /*[in]*/ IUri* uri,
        /*[in]*/ const String& name,
        /*[out]*/ IUri** outUri);

protected:
    static CARAPI_(Boolean) PutString(
        /*[in]*/ IContentResolver* resolver,
        /*[in]*/ IUri* uri,
        /*[in]*/ const String& name,
        /*[in]*/ const String& value);

};
#endif //__SETTINGSNAMEVALUETABLE_H__
