
#ifndef __CACCOUNTMANAGERSERVICEHELPER_H__
#define __CACCOUNTMANAGERSERVICEHELPER_H__

#include "_CAccountManagerServiceHelper.h"

CarClass(CAccountManagerServiceHelper)
{
public:
    /**
     * This should only be called by system code. One should only call this after the service
     * has started.
     * @return a reference to the AccountManagerService instance
     * @hide
     */
    CARAPI GetSingleton(
        /* [out] */ IAccountManager** service);
};

#endif //__CACCOUNTMANAGERSERVICEHELPER_H__
