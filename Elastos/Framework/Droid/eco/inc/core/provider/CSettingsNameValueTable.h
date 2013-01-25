#ifndef __CSETTINGSNAMEVALUETABLE_H__
#define __CSETTINGSNAMEVALUETABLE_H__

#include "_CSettingsNameValueTable.h"
#include "Elastos.Framework.Core.h"

using namespace Elastos;

CarClass(CSettingsNameValueTable)
{
public:

    CARAPI GetUriFor(
        /*[in]*/ IUri* uri,
        /*[in]*/ const String& name,
        /*[out]*/ IUri** outUri);

};
#endif //__CSETTINGSNAMEVALUETABLE_H__
