
#ifndef __CCOOKIEPOLICYHELPER_H__
#define __CCOOKIEPOLICYHELPER_H__

#include "_CCookiePolicyHelper.h"

CarClass(CCookiePolicyHelper)
{
public:
    CARAPI GetCookiePolicy(
        /* [in] */ const String& type,
        /* [out] */ ICookiePolicy** policy);

};

#endif //__CCOOKIEPOLICYHELPER_H__
