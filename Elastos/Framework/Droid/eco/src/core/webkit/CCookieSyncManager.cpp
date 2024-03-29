#include <Logger.h>
#include <elastos/System.h>

#include "webkit/CCookieSyncManager.h"
#include "webkit/CWebViewDatabase.h"
#include "webkit/DebugFlags.h"

using namespace Elastos::Utility::Logging;

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
    if (mDataBase.Get() == NULL) {
        return;
    }

    ( (CWebViewDatabase*)(mDataBase.Get()) )->GetCookiesForDomain(domain, list);
}

CARAPI_(void) CCookieSyncManager::ClearAllCookies()
{
    // null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if ( (mDataBase.Get()) == NULL) {
        return;
    }

    ((CWebViewDatabase*)(mDataBase.Get()))->ClearCookies();
}

CARAPI_(Boolean) CCookieSyncManager::HasCookies()
{
    // null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if ( (mDataBase.Get()) == NULL) {
        return FALSE;
    }

    return ( (CWebViewDatabase*)(mDataBase.Get()))->HasCookies();
}

/**
 * Package level api, called from CookieManager Clear all session cookies in
 * the database
 */
CARAPI_(void) CCookieSyncManager::ClearSessionCookies()
{
    // null mDataBase implies that the host application doesn't support
    // persistent cookie.
    if ( (mDataBase.Get()) == NULL) {
        return;
    }

    ( (CWebViewDatabase*)(mDataBase.Get()) )->ClearSessionCookies();
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
    if ( (mDataBase.Get()) == NULL) {
        return;
    }

    ( (CWebViewDatabase*)(mDataBase.Get()) )->ClearExpiredCookies(now);
}

CARAPI_(void) CCookieSyncManager::SyncFromRamToFlash()
{
    if (DebugFlags::sCOOKIE_SYNC_MANAGER) {
        Logger::V(LOGTAG, "CookieSyncManager::syncFromRamToFlash STARTS");
    }

    CCookieManager* cookieManager = NULL;
    CCookieManager::AcquireSingletonByFriend(&cookieManager);

    Boolean bFlag = FALSE;
    cookieManager->AcceptCookie(&bFlag);
    if (!bFlag) {
        return;
    }

    Vector<AutoPtr<CCookieManager::Cookie> > cookieList;
    cookieManager->GetUpdatedCookiesSince(mLastUpdate, cookieList);
    mLastUpdate = System::GetCurrentTimeMillis();
    SyncFromRamToFlash(cookieList);

    Vector<AutoPtr<CCookieManager::Cookie> > lruList;
    cookieManager->DeleteLRUDomain(lruList);
    SyncFromRamToFlash(lruList);

    if (DebugFlags::sCOOKIE_SYNC_MANAGER) {
        Logger::V(LOGTAG, "CookieSyncManager::syncFromRamToFlash DONE");
    }
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
                ((CWebViewDatabase*)(mDataBase.Get()))->DeleteCookies(cookie->domain, cookie->path,
                        cookie->name);
            }

            if (cookie->mode != CCookieManager::Cookie::MODE_DELETED) {
                ((CWebViewDatabase*)(mDataBase.Get()) )->AddCookie(*cookie);
                cookieManager->SyncedACookie(*cookie);
            } else {
                cookieManager->DeleteACookie(*cookie);
            }
        }
    }
}