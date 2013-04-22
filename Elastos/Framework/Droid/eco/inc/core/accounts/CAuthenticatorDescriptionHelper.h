
#ifndef __CAUTHENTICATORDESCRIPTIONHELPER_H__
#define __CAUTHENTICATORDESCRIPTIONHELPER_H__

#include "_CAuthenticatorDescriptionHelper.h"

CarClass(CAuthenticatorDescriptionHelper)
{
public:
    CARAPI NewKey(
        /* [in] */ const String& type,
        /* [out] */ IAuthenticatorDescription** description);
};

#endif //__CAUTHENTICATORDESCRIPTIONHELPER_H__
