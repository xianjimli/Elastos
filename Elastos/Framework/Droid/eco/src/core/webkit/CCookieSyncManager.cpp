
#include "webkit/CCookieSyncManager.h"
#include "webkit/CWebViewDatabase.h"

CCookieSyncManager* CCookieSyncManager::sRef;

#if 0
ECode CCookieSyncManager::GetInstance(
    /* [out] */ ICookieSyncManager ** ppCookieSyncManager)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCookieSyncManager::CreateInstance(
    /* [in] */ IContext * pContext,
    /* [out] */ ICookieSyncManager ** ppCookieSyncManager)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
#endif

CCookieSyncManager::CCookieSyncManager()
                : WebSyncManager(NULL, (String)"CCookieSyncManager")
{

}

CCookieSyncManager::CCookieSyncManager(
    /* [in] */ IContext* context) : WebSyncManager(context, (String)"CCookieSyncManager")
{

}


ECode CCookieSyncManager::InitInstance(
    /* [in] */ IContext* context)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

CARAPI_(void) CCookieSyncManager::GetCookiesForDomain(
    /* [in] */ String& domain,
    /* [out] */ Vector<AutoPtr<CCookieManager::Cookie> >& list) const
{
    // null mDataBase implies that the host application doesn't support
    // persistent cookie. No sync needed.
    if (mDataBase == NULL) {
        return;
    }

    ((CWebViewDatabase*)mDataBase)->GetCookiesForDomain(domain, list);
}

CARAPI_(void) CCookieSyncManager::ClearAllCookies()
{
    // null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if (mDataBase == NULL) {
        return;
    }

    ((CWebViewDatabase*)mDataBase)->ClearCookies();
}

CARAPI_(Boolean) CCookieSyncManager::HasCookies()
{
    // null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if (mDataBase == NULL) {
        return FALSE;
    }

    return ((CWebViewDatabase*)mDataBase)->HasCookies();
}

/**
 * Package level api, called from CookieManager Clear all session cookies in
 * the database
 */
CARAPI_(void) CCookieSyncManager::ClearSessionCookies()
{
    // null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if (mDataBase == NULL) {
        return;
    }

    ((CWebViewDatabase*)mDataBase)->ClearSessionCookies();
}

/**
 * Package level api, called from CookieManager Clear all expired cookies in
 * the database
 */
CARAPI_(void) CCookieSyncManager::ClearExpiredCookies(
    /* [in] */ Int64 now)
{
    // null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if (mDataBase == NULL) {
        return;
    }

    ((CWebViewDatabase*)mDataBase)->ClearExpiredCookies(now);
}

CARAPI_(void) CCookieSyncManager::SyncFromRamToFlash()
{
//    if (DebugFlags.COOKIE_SYNC_MANAGER) {
//        Log.v(LOGTAG, "CookieSyncManager::syncFromRamToFlash STARTS");
//    }

    CCookieManager* cookieManager = NULL;
    CCookieManager::AcquireSingletonByFriend(&cookieManager);

    Boolean bFlag = FALSE;
    cookieManager->AcceptCookie(&bFlag);
    if (!bFlag) {
        return;
    }

    Vector<AutoPtr<CCookieManager::Cookie> > cookieList;
    cookieManager->GetUpdatedCookiesSince(mLastUpdate, cookieList);
//    mLastUpdate = System.currentTimeMillis();
    SyncFromRamToFlash(cookieList);

    Vector<AutoPtr<CCookieManager::Cookie> > lruList;
    cookieManager->DeleteLRUDomain(lruList);
    SyncFromRamToFlash(lruList);

//    if (DebugFlags.COOKIE_SYNC_MANAGER) {
//        Log.v(LOGTAG, "CookieSyncManager::syncFromRamToFlash DONE");
//    }
}

CARAPI_(void) CCookieSyncManager::SyncFromRamToFlash(
    /* [in] */ Vector<AutoPtr<CCookieManager::Cookie> > & list)
{
    CCookieManager* cookieManager = NULL;
    CCookieManager::AcquireSingletonByFriend(&cookieManager);

    //Iterator<Cookie> iter = list.iterator();
    Int32 size = list.GetSize();
    //while (iter.hasNext())
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<CCookieManager::Cookie> cookie = list[i];
        if (cookie->mode != CCookieManager::Cookie::MODE_NORMAL) {
            if (cookie->mode != CCookieManager::Cookie::MODE_NEW) {
                ((CWebViewDatabase*)mDataBase)->DeleteCookies(cookie->domain, cookie->path,
                        cookie->name);
            }

            if (cookie->mode != CCookieManager::Cookie::MODE_DELETED) {
                ((CWebViewDatabase*)mDataBase)->AddCookie(*cookie);
                cookieManager->SyncedACookie(*cookie);
            } else {
                cookieManager->DeleteACookie(*cookie);
            }
        }
    }
}