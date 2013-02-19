
#ifndef __CCOOKIESYNCMANAGER_H__
#define __CCOOKIESYNCMANAGER_H__

#include "_CCookieSyncManager.h"

CarClass(CCookieSyncManager)
{
public:
    CARAPI GetInstance(
        /* [out] */ ICookieSyncManager ** ppCookieSyncManager);

    CARAPI CreateInstance(
        /* [in] */ IContext * pContext,
        /* [out] */ ICookieSyncManager ** ppCookieSyncManager);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCOOKIESYNCMANAGER_H__
