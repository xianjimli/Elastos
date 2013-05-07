
#ifndef __CPRIVACYSETTINGS_H__
#define __CPRIVACYSETTINGS_H__

#include "_CPrivacySettings.h"

CarClass(CPrivacySettings)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 _id,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPRIVACYSETTINGS_H__
