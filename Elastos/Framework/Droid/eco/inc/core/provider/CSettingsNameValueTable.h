#ifndef __CSETTINGSNAMEVALUETABLE_H__
#define __CSETTINGSNAMEVALUETABLE_H__

#include "_CSettingsNameValueTable.h"
#include "Settings.h"


CarClass(CSettingsNameValueTable) , public Settings::NameValueTable
{
public:
    CARAPI GetUriFor(
        /* [in] */ IUri* uri,
        /* [in] */ const String& name,
        /* [out] */ IUri** value);
};

#endif //__CSETTINGSNAMEVALUETABLE_H__
