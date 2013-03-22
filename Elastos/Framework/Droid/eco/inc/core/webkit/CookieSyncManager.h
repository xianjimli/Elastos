#ifndef __COOKIESYNCMANAGER_H__
#define __COOKIESYNCMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

#include "WebSyncManager.h"
#include "CCookieManager.h"

/**
 * The CookieSyncManager is used to synchronize the browser cookie store
 * between RAM and permanent storage. To get the best performance, browser cookies are
 * saved in RAM. A separate thread saves the cookies between, driven by a timer.
 * <p>
 *
 * To use the CookieSyncManager, the host application has to call the following
 * when the application starts:
 * <p>
 *
 * <pre class="prettyprint">CookieSyncManager.createInstance(context)</pre><p>
 *
 * To set up for sync, the host application has to call<p>
 * <pre class="prettyprint">CookieSyncManager.getInstance().startSync()</pre><p>
 *
 * in Activity.onResume(), and call
 * <p>
 *
 * <pre class="prettyprint">
 * CookieSyncManager.getInstance().stopSync()
 * </pre><p>
 *
 * in Activity.onPause().<p>
 *
 * To get instant sync instead of waiting for the timer to trigger, the host can
 * call
 * <p>
 * <pre class="prettyprint">CookieSyncManager.getInstance().sync()</pre><p>
 *
 * The sync interval is 5 minutes, so you will want to force syncs
 * manually anyway, for instance in {@link
 * WebViewClient#onPageFinished}. Note that even sync() happens
 * asynchronously, so don't do it just as your activity is shutting
 * down.
 */
class CookieSyncManager : public WebSyncManager 
{
public:
    /**
     * Singleton access to a {@link CookieSyncManager}. An
     * IllegalStateException will be thrown if
     * {@link CookieSyncManager#createInstance(Context)} is not called before.
     * 
     * @return CookieSyncManager
     */
    static CARAPI_(CookieSyncManager*) GetInstance();

    /**
     * Create a singleton CookieSyncManager within a context
     * @param context
     * @return CookieSyncManager
     */
    static CARAPI_(CookieSyncManager*) CreateInstance(
        /* [in] */ IContext* context);

public:
    /**
     * Package level api, called from CookieManager. Get all the cookies which
     * matches a given base domain.
     * @param domain
     * @return A list of Cookie
     */
    CARAPI_(void) GetCookiesForDomain(
        /* [in] */ const String& domain,
        /* [out] */ Vector<AutoPtr<CCookieManager::Cookie> >& cookielist);

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

protected:
    static Mutex sSyncLock;

private:
    CookieSyncManager(
        /* [in] */ IContext* context);

    CARAPI_(void) SyncFromRamToFlash(
        /* [in] */ Vector<AutoPtr<CCookieManager::Cookie> >& list);

private:
    static CookieSyncManager* sRef;

    // time when last update happened
    Int64 mLastUpdate;
};

#endif //__COOKIESYNCMANAGER_H__
