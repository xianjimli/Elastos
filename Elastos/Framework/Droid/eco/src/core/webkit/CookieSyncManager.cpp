
#include "webkit/CookieSyncManager.h"
#include "webkit/CWebViewDatabase.h"
#include "webkit/CCookieManager.h"

#include <elastos/Mutex.h>

CookieSyncManager* CookieSyncManager::sRef = NULL;

/**
 * Singleton access to a {@link CookieSyncManager}. An
 * IllegalStateException will be thrown if
 * {@link CookieSyncManager#createInstance(Context)} is not called before.
 * 
 * @return CookieSyncManager
 */
CARAPI_(CookieSyncManager*) CookieSyncManager::GetInstance()
{
	Mutex mutex;
	Mutex::Autolock lock(mutex);

	if (sRef == NULL)
	{
//        throw new IllegalStateException(
//                "CookieSyncManager::createInstance() needs to be called "
//                        + "before CookieSyncManager::getInstance()");
    }

    return sRef;
}

/**
 * Create a singleton CookieSyncManager within a context
 * @param context
 * @return CookieSyncManager
 */
CARAPI_(CookieSyncManager*) CookieSyncManager::CreateInstance(
    /* [in] */ IContext* context)
{
	Mutex mutex;
	Mutex::Autolock lock(mutex);

	if (context == NULL)
	{
		return NULL;
	}

	if (sRef == NULL)
	{
		IContext* pContext = NULL;
		context->GetApplicationContext(&pContext);
		if (pContext == NULL)
		{
			return NULL;
		}

        sRef = new CookieSyncManager(pContext);
    }

    return sRef;
}

/**
 * Package level api, called from CookieManager. Get all the cookies which
 * matches a given base domain.
 * @param domain
 * @return A list of Cookie
 */
CARAPI_(void) CookieSyncManager::GetCookiesForDomain(
    /* [in] */ const String& domain,
    /* [out] */ Vector<AutoPtr<CCookieManager::Cookie> >& cookielist)
{
	// null mDataBase implies that the host application doesn't support
    // persistent cookie. No sync needed.
    if (mDataBase == NULL) {
        return;
    }

    ((CWebViewDatabase*)mDataBase)->GetCookiesForDomain(domain, cookielist);
}

/**
 * Package level api, called from CookieManager Clear all cookies in the
 * database
 */
CARAPI_(void) CookieSyncManager::ClearAllCookies()
{
	// null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if (mDataBase == NULL)
    {
        return;
    }

    ((CWebViewDatabase*)mDataBase)->ClearCookies();
}

/**
 * Returns true if there are any saved cookies.
 */
CARAPI_(Boolean) CookieSyncManager::HasCookies()
{
	// null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if (mDataBase == NULL)
    {
        return false;
    }

    return ((CWebViewDatabase*)mDataBase)->HasCookies();
}

/**
 * Package level api, called from CookieManager Clear all session cookies in
 * the database
 */
CARAPI_(void) CookieSyncManager::ClearSessionCookies()
{
	// null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if (mDataBase == NULL)
    {
        return;
    }

    ((CWebViewDatabase*)mDataBase)->ClearSessionCookies();
}

/**
 * Package level api, called from CookieManager Clear all expired cookies in
 * the database
 */
CARAPI_(void) CookieSyncManager::ClearExpiredCookies(
    /* [in] */ Int64 now)
{
	// null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if (mDataBase == NULL)
    {
        return;
    }

    ((CWebViewDatabase*)mDataBase)->ClearExpiredCookies(now);
}

CARAPI_(void) CookieSyncManager::SyncFromRamToFlash()
{
//	if (DebugFlags.COOKIE_SYNC_MANAGER) {
//        Log.v(LOGTAG, "CookieSyncManager::syncFromRamToFlash STARTS");
//    }

	ICookieManager* cookieManager = NULL;
	///////////////////////   new  //////////////////////////////
	Boolean bFlag = false;
	cookieManager->AcceptCookie(&bFlag);
    if (!bFlag)
    {
        return;
    }

    Vector<AutoPtr<CCookieManager::Cookie> > cookieList;
    ((CCookieManager*)cookieManager)->GetUpdatedCookiesSince(mLastUpdate, cookieList);
//    mLastUpdate = System.currentTimeMillis();
    SyncFromRamToFlash(cookieList);

    Vector<AutoPtr<CCookieManager::Cookie> > lruList;
    ((CCookieManager*)cookieManager)->DeleteLRUDomain(lruList);
    SyncFromRamToFlash(lruList);

//    if (DebugFlags.COOKIE_SYNC_MANAGER) {
//        Log.v(LOGTAG, "CookieSyncManager::syncFromRamToFlash DONE");
//    }
}

CookieSyncManager::CookieSyncManager(
    /* [in] */ IContext* context) : WebSyncManager(context, (String)"CookieSyncManager")
{
}

CARAPI_(void) CookieSyncManager::SyncFromRamToFlash(
    /* [in] */ Vector<AutoPtr<CCookieManager::Cookie> >& list)
{
	ICookieManager* cookieManager = NULL;
	//Iterator<Cookie> iter = list.iterator();
	Int32 size = list.GetSize();
	for (Int32 i = 0; i < size; i++)
    //while (iter.hasNext())
    {
        CCookieManager::Cookie* cookie = list[i];
        if (cookie->mode != CCookieManager::Cookie::MODE_NORMAL)
        {
            if (cookie->mode != CCookieManager::Cookie::MODE_NEW)
            {
                ((CWebViewDatabase*)mDataBase)->DeleteCookies(cookie->domain,
                	           cookie->path, cookie->name);
            }

            if (cookie->mode != CCookieManager::Cookie::MODE_DELETED)
            {
                ((CWebViewDatabase*)mDataBase)->AddCookie(*cookie);
                ((CCookieManager*)cookieManager)->SyncedACookie(*cookie);
            }
            else
            {
                ((CCookieManager*)cookieManager)->DeleteACookie(*cookie);
            }
        }
    }
}