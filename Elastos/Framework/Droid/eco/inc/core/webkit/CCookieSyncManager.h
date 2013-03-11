
#ifndef __CCOOKIESYNCMANAGER_H__
#define __CCOOKIESYNCMANAGER_H__

#include "_CCookieSyncManager.h"
#include "ext/frameworkext.h"
#include <elastos/Vector.h>

#include "CCookieManager.h"
#include "WebSyncManager.h"

CarClass(CCookieSyncManager), WebSyncManager
{
public:
//	CARAPI GetInstance(
//        /* [out] */ ICookieSyncManager ** ppCookieSyncManager);

//	CARAPI CreateInstance(
//        /* [in] */ IContext * pContext,
//        /* [out] */ ICookieSyncManager ** ppCookieSyncManager);

	CARAPI InitInstance(
             /* [in] */ IContext* context);

private:
//	CARAPI constructor(
//		/* [in] */ IContext* context);

protected:
    CCookieSyncManager();

    CCookieSyncManager(
        /* [in] */ IContext * pContext);

public:
    /**
     * Package level api, called from CookieManager. Get all the cookies which
     * matches a given base domain.
     * @param domain
     * @return A list of Cookie
     */
    CARAPI_(void) GetCookiesForDomain(
    	/* [in] */ String& domain,
    	/* [out] */ Vector<AutoPtr<CCookieManager::Cookie> >& list) const;

    /**
     * Package level api, called from CookieManager Clear all cookies in the
     * database
     */
    CARAPI_(void) ClearAllCookies();

    /**
     * Returns true if there are any saved cookies.
     */
    CARAPI_(Boolean) HasCookies();

    /**
     * Package level api, called from CookieManager Clear all session cookies in
     * the database
     */
    CARAPI_(void) ClearSessionCookies();

    /**
     * Package level api, called from CookieManager Clear all expired cookies in
     * the database
     */
    CARAPI_(void) ClearExpiredCookies(
    	/* [in] */ Int64 now);

protected:

	CARAPI_(void) SyncFromRamToFlash();

private:

	CARAPI_(void) SyncFromRamToFlash(
		/* [in] */ Vector<AutoPtr<CCookieManager::Cookie> > & list);

	CookieSyncManager(
		/* [in] */ IContext* context);            

    // time when last update happened
	Int64 mLastUpdate;

	static CCookieSyncManager* sRef;
};

#endif // __CCOOKIESYNCMANAGER_H__
