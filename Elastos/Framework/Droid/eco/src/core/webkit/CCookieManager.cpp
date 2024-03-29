#include <elastos/System.h>
#include <Logger.h>
#include <StringBuffer.h>

#include "webkit/CCookieManager.h"
#include "webkit/CCookieSyncManager.h"
#include "os/Runnable.h"
#include "webkit/DebugFlags.h"

using namespace Core;
using namespace Elastos::Utility::Logging;

ICookieManager* CCookieManager::sRef;

const CString CCookieManager::LOGTAG("webkit");

const CString CCookieManager::DOMAIN("domain");

const CString CCookieManager::PATH("path");

const CString CCookieManager::EXPIRES("expires");

const CString CCookieManager::SECURE("secure");

const CString CCookieManager::MAX_AGE("max-age");

const CString CCookieManager::HTTP_ONLY("httponly");

const CString CCookieManager::HTTPS("https");

const Byte CCookieManager::PERIOD = '.';

const Byte CCookieManager::COMMA = ',';

const Byte CCookieManager::SEMICOLON = ';';

const Byte CCookieManager::EQUAL = '=';

const Byte CCookieManager::PATH_DELIM = '/';

const Byte CCookieManager::QUESTION_MARK = '?';

const Byte CCookieManager::WHITE_SPACE = ' ';

const Byte CCookieManager::QUOTATION = '\"';

const Int32 CCookieManager::SECURE_LENGTH = 0;// = SECURE.length();

const Int32 CCookieManager::HTTP_ONLY_LENGTH = 0;// = HTTP_ONLY.length();

// RFC2109 defines 4k as maximum size of a cookie
const Int32 CCookieManager::MAX_COOKIE_LENGTH;

// RFC2109 defines 20 as max cookie count per domain. As we track with base
// domain, we allow 50 per base domain
const Int32 CCookieManager::MAX_COOKIE_COUNT_PER_BASE_DOMAIN;

// RFC2109 defines 300 as max count of domains. As we track with base
// domain, we set 200 as max base domain count
const Int32 CCookieManager::MAX_DOMAIN_COUNT;

// max cookie count to limit RAM cookie takes less than 100k, it is based on
// average cookie entry size is less than 100 bytes
const Int32 CCookieManager::MAX_RAM_COOKIES_COUNT;

//  max domain count to limit RAM cookie takes less than 100k,
const Int32 CCookieManager::MAX_RAM_DOMAIN_COUNT;

/**
 * This contains a list of 2nd-level domains that aren't allowed to have
 * wildcards when combined with country-codes. For example: [.co.uk].
 */
const CString CCookieManager::BAD_COUNTRY_2LDS[] =
      { "ac", "co", "com", "ed", "edu", "go", "gouv", "gov", "info",
        "lg", "ne", "net", "or", "org" };

const Byte CCookieManager::Cookie::MODE_NEW;

const Byte CCookieManager::Cookie::MODE_NORMAL;

const Byte CCookieManager::Cookie::MODE_DELETED;

const Byte CCookieManager::Cookie::MODE_REPLACED;


ECode CCookieManager::GetInstance(
    /* [out] */ ICookieManager** instance)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCookieManager::SetAcceptCookie(
    /* [in] */ Boolean accept)
{
    Mutex::Autolock lock(_m_syncLock);

    mAcceptCookie = accept;
    
    return NOERROR;
}

ECode CCookieManager::AcceptCookie(
    /* [out] */ Boolean* acceptCookie)
{
    Mutex::Autolock lock(_m_syncLock);

    VALIDATE_NOT_NULL(acceptCookie);
    *acceptCookie = mAcceptCookie;

    return NOERROR;
}

ECode CCookieManager::SetCookie(
    /* [in] */ CString url,
    /* [in] */ CString value)
{
    AutoPtr<IWebAddress> uri;
    //try {
//        uri = new WebAddress(url);
    //} catch (ParseException ex) {
    //    Log.e(LOGTAG, "Bad address: " + url);
    //    return;
    //}

    SetCookieEx(uri, value);

    return NOERROR;
}

ECode CCookieManager::SetCookieEx(
    /* [in] */ IWebAddress* uri,
    /* [in] */ CString value)
{
    Mutex::Autolock lock(_m_syncLock);

    VALIDATE_NOT_NULL(uri);

    if (value.GetLength() > MAX_COOKIE_LENGTH) {
        return NOERROR;
    }

    if (!mAcceptCookie || uri == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    if (DebugFlags::sCOOKIE_MANAGER) {
        String webAddress;
//        uri->ToString(&webAddress);
        Logger::V(LOGTAG, StringBuffer("setCookie: uri: ") + webAddress + " value: " + value);
    }

    Vector<String> hostAndPath;
    GetHostAndPath(uri, hostAndPath);
    if (hostAndPath.GetSize() == 0) {
        return E_NOT_IMPLEMENTED;
    }
    
    // For default path, when setting a cookie, the spec says:
    //Path:   Defaults to the path of the request URL that generated the
    // Set-Cookie response, up to, but not including, the
    // right-most /.
    if (hostAndPath[1].GetLength() > 1) {
        Int32 index = hostAndPath[1].LastIndexOf(PATH_DELIM);
        hostAndPath[1] = hostAndPath[1].Substring(0, 
                index > 0 ? index : index + 1);
    }

    Vector<AutoPtr<Cookie> > cookies;// = null;
    String strValue((const char*)value);
    //try {
        ParseCookie(hostAndPath[0], hostAndPath[1], strValue, cookies);
    //} catch (RuntimeException ex) {
    //    Log.e(LOGTAG, "parse cookie failed for: " + value);
    //}

    if (cookies.GetSize() == 0/* || cookies.size() == 0*/) {
        return E_NOT_IMPLEMENTED;
    }

    String baseDomain;
    GetBaseDomain(hostAndPath[0], baseDomain);
    Vector<AutoPtr<Cookie> >* cookieList;
    mCookieMap->Get(baseDomain, (IInterface**)&cookieList);

    if (cookieList->GetSize() == 0) {
        CCookieSyncManager* pSync = NULL;
        CCookieSyncManager::AcquireSingletonByFriend(&pSync);
        pSync->GetCookiesForDomain(baseDomain, *cookieList);

        //cookieList = CookieSyncManager.getInstance()
        //        .getCookiesForDomain(baseDomain);
        mCookieMap->Put(baseDomain, (IInterface*)cookieList);
    }

    Int64 now = System::GetCurrentTimeMillis();
    Int32 size = cookies.GetSize();
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<Cookie> cookie = cookies[i];

        Boolean done = FALSE;
        //Iterator<Cookie> iter = cookieList.iterator();
        Int32 _size = cookieList->GetSize();
        //while (iter.hasNext()) 
        for (Int32 j = 0; j < _size; j++) {
            AutoPtr<Cookie> cookieEntry = (*cookieList)[j]; //iter.next();
            if (cookie->ExactMatch(*cookieEntry)) {
                // expires == -1 means no expires defined. Otherwise
                // negative means far future
                if (cookie->expires < 0 || cookie->expires > now) {
                    // secure cookies can't be overwritten by non-HTTPS url
                    if (!cookieEntry->secure/* || HTTPS.equals(uri.mScheme)*/) {
                        cookieEntry->value = cookie->value;
                        cookieEntry->expires = cookie->expires;
                        cookieEntry->secure = cookie->secure;
                        cookieEntry->lastAcessTime = now;
                        cookieEntry->lastUpdateTime = now;
                        cookieEntry->mode = Cookie::MODE_REPLACED;
                    }
                } else {
                    cookieEntry->lastUpdateTime = now;
                    cookieEntry->mode = Cookie::MODE_DELETED;
                }
                done = TRUE;
                break;
            }
        }

        // expires == -1 means no expires defined. Otherwise negative means
        // far future
        if (!done && (cookie->expires < 0 || cookie->expires > now)) {
            cookie->lastAcessTime = now;
            cookie->lastUpdateTime = now;
            cookie->mode = Cookie::MODE_NEW;
            Int32 cookieListSize = cookieList->GetSize();
            if (cookieListSize > MAX_COOKIE_COUNT_PER_BASE_DOMAIN) {
                AutoPtr<Cookie> toDelete = new Cookie();
                toDelete->lastAcessTime = now;
                //Iterator<Cookie> iter2 = cookieList.iterator();
                //while (iter2.hasNext())
                for (Int32 i = 0; i < cookieListSize; i++) {
                    AutoPtr<Cookie> cookieEntry2 = (*cookieList)[i];// iter2.next();
                    if ((cookieEntry2->lastAcessTime < toDelete->lastAcessTime)
                            && cookieEntry2->mode != Cookie::MODE_DELETED) {
                        toDelete = cookieEntry2;
                    }
                }
                toDelete->mode = Cookie::MODE_DELETED;
            }
            cookieList->PushBack(cookie);
        }
    }

    return NOERROR;
}

ECode CCookieManager::GetCookie(
    /* [in] */ CString url,
    /* [out] */ String* cookie)
{
    VALIDATE_NOT_NULL(cookie);

    AutoPtr<IWebAddress> uri;
    //try {
//        uri = new WebAddress(url);
    //} catch (ParseException ex) {
    //    Log.e(LOGTAG, "Bad address: " + url);
    //    return null;
    //}
    return GetCookieEx(uri, cookie);
}

ECode CCookieManager::GetCookieEx(
    /* [in] */ IWebAddress* uri,
    /* [out] */ String* cookie)
{
    Mutex::Autolock lock(_m_syncLock);

//    VALIDATE_NOT_NULL(uri);
    VALIDATE_NOT_NULL(cookie);

    if (!mAcceptCookie || uri == NULL) {
        return E_NOT_IMPLEMENTED;
    }

    Vector<String> hostAndPath;
    GetHostAndPath(uri, hostAndPath);

    if (hostAndPath.GetSize() == 0) {
        return E_NOT_IMPLEMENTED;
    }

    String baseDomain;
    GetBaseDomain(hostAndPath[0], baseDomain);
    Vector<AutoPtr<Cookie> >* cookieList;
    mCookieMap->Get(baseDomain, (IInterface**)&cookieList);
    if (cookieList->GetSize() == 0) {
        CCookieSyncManager* pSync = NULL;
        CCookieSyncManager::AcquireSingletonByFriend(&pSync);
        pSync->GetCookiesForDomain(baseDomain, *cookieList);
        //cookieList = CookieSyncManager.getInstance()
        //        .getCookiesForDomain(baseDomain);
        mCookieMap->Put(baseDomain, (IInterface*)cookieList);
    }
#if 0
    long now = System::GetCurrentTimeMillis();
    Boolean secure;// = HTTPS.equals(pUri.mScheme);
    Iterator<Cookie> iter = cookieList.iterator();

    SortedSet<Cookie> cookieSet = new TreeSet<Cookie>(COMPARATOR);
    while (iter.hasNext())
    {
        Cookie cookie = iter.next();
        if (cookie.domainMatch(hostAndPath[0]) &&
                cookie.pathMatch(hostAndPath[1])
                // expires == -1 means no expires defined. Otherwise
                // negative means far future
                && (cookie.expires < 0 || cookie.expires > now)
                && (!cookie.secure || secure)
                && cookie.mode != Cookie.MODE_DELETED)
        {
            cookie.lastAcessTime = now;
            cookieSet.add(cookie);
        }
    }

    StringBuilder ret = new StringBuilder(256);
    Iterator<Cookie> setIter = cookieSet.iterator();
    while (setIter.hasNext())
    {
        Cookie cookie = setIter.next();
        if (ret.length() > 0)
        {
            ret.append(SEMICOLON);
            // according to RC2109, SEMICOLON is official separator,
            // but when log in yahoo.com, it needs WHITE_SPACE too.
            ret.append(WHITE_SPACE);
        }

        ret.append(cookie.name);
        if (cookie.value != null)
        {
            ret.append(EQUAL);
            ret.append(cookie.value);
        }
    }

    if (ret.length() > 0) {
        if (DebugFlags.COOKIE_MANAGER) {
            Log.v(LOGTAG, "getCookie: uri: " + pUri + " value: " + ret);
        }
        return ret.toString();
    } else {
        if (DebugFlags.COOKIE_MANAGER) {
            Log.v(LOGTAG, "getCookie: uri: " + pUri
                    + " But can't find cookie.");
        }
        return E_NOT_IMPLEMENTED;
    }
#endif

    return NOERROR;
}

ECode CCookieManager::RemoveSessionCookie()
{
    class runable : public Runnable {
    public:
        runable(CCookieManager* cm) : cookieManager(cm) {}

        ECode Run() {
            CCookieManager* cm = (CCookieManager*)(cookieManager.Get());
            Mutex::Autolock lock(cm->_m_syncLock);
            
            ArrayOf<String> *cookieList = NULL;
            cm->mCookieMap->GetAllItems(&cookieList, NULL);
            Int32 size = cookieList->GetLength();
            for (Int32 i = 0; i < size; i++) {
                Vector<AutoPtr<Cookie> >* list = NULL;
                cm->mCookieMap->Get((*cookieList)[i], (IInterface**)&list);

                Int32 _size = list->GetSize();
                for (Int32 j = 0; j < _size; j++) {
                    if ((*list)[j]->expires == -1) {
                        list->Erase(list->Begin() + j);
                    }
                }
            }

            AutoPtr<ICookieSyncManager> csm = NULL;
            CCookieSyncManager::AcquireSingleton((ICookieSyncManager**)&csm);
            ((CCookieSyncManager*)(csm.Get()))->ClearSessionCookies();
        }

    private:
        AutoPtr<ICookieManager> cookieManager;
    };

    runable clearCache(this);// = new Runnable()

    AutoPtr<IThread> thread;
    CThread::New((IRunnable*)&clearCache, (IThread**)&thread);
    thread->Start();

    return NOERROR;
}

ECode CCookieManager::RemoveAllCookie()
{
#if 0
    final Runnable clearCache = new Runnable() {
        public void run() {
            synchronized(CookieManager.this) {
                mCookieMap = new LinkedHashMap<String, ArrayList<Cookie>>(
                        MAX_DOMAIN_COUNT, 0.75f, true);
                CookieSyncManager.getInstance().clearAllCookies();
            }
        }
    };
    new Thread(clearCache).start();
#endif
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCookieManager::HasCookies(
    /* [out] */ Boolean * flag)
{
    Mutex::Autolock lock(_m_syncLock);

    VALIDATE_NOT_NULL(flag);
    
    ICookieSyncManager* csm = NULL;
    CCookieSyncManager::AcquireSingleton(&csm);
    *flag = ((CCookieSyncManager*)csm)->HasCookies();

    return NOERROR;
}

ECode CCookieManager::RemoveExpiredCookie()
{
    //final Runnable clearCache = new Runnable() 
    class runable : public Runnable {
    public:
        runable(CCookieManager* cm) : cookieManager(cm) {}

        ECode Run() {
            CCookieManager* cm = (CCookieManager*)(cookieManager.Get());
            Mutex::Autolock lock(cm->_m_syncLock);

            Int64 now = System::GetCurrentTimeMillis();
            ArrayOf<String>* cookieList = NULL;//mCookieMap.values();
            cm->mCookieMap->GetAllItems(&cookieList, NULL);
            Int32 size = cookieList->GetLength();
            for (Int32 i = 0; i < size; i++) {
                Vector<AutoPtr<Cookie> >* list = NULL;
                cm->mCookieMap->Get((*cookieList)[i], (IInterface**)&list);

                Int32 _size = list->GetSize();
                for (Int32 j = 0; j < _size; j++) {
                    // expires == -1 means no expires defined. Otherwise 
                    // negative means far future
                    if (((*list)[j])->expires > 0 && ((*list)[j])->expires < now) {
                        //iter.remove();
                        list->Erase(list->Begin() + j);
                    }
                }
            }

            ICookieSyncManager* csm = NULL;
            CCookieSyncManager::AcquireSingleton(&csm);
            ((CCookieSyncManager*)csm)->ClearExpiredCookies(now);
        }

    private:
        AutoPtr<ICookieManager> cookieManager;
    };

    runable clearCache(this);// = new Runnable()

    AutoPtr<IThread> thread;
    CThread::New((IRunnable*)&clearCache, (IThread**)&thread);
    thread->Start();

    return NOERROR;
}


/*synchronized*/
CARAPI_(void) CCookieManager::GetUpdatedCookiesSince(
    /* [in] */ Int64 last,
    /* [out] */ Vector<AutoPtr<Cookie> >& cookies)
{
    Mutex::Autolock lock(_m_syncLock);

    //ArrayList<Cookie> cookies = new ArrayList<Cookie>();
    //Collection<Vector<Cookie>> cookieList = mCookieMap.values();
    //Iterator<ArrayList<Cookie>> listIter = cookieList.iterator();    
    //while (listIter.hasNext())
    //{
    //    ArrayList<Cookie> list = listIter.next();
    //    Iterator<Cookie> iter = list.iterator();
    //    while (iter.hasNext())
    //    {
    //        Cookie cookie = iter.next();
    //        if (cookie.lastUpdateTime > last)
    //        {
    //            cookies.add(cookie);
    //        }
    //    }
    //}
    //return cookies;

    ArrayOf<String> *cookieList = NULL;
    mCookieMap->GetAllItems(&cookieList, NULL);
    Int32 size = cookieList->GetLength();
    for (Int32 i = 0; i < size; i++) {

        Vector<AutoPtr<Cookie> >* list = NULL;
        mCookieMap->Get((*cookieList)[i], (IInterface**)&list);
        
        Int32 _size = list->GetSize();
        for (Int32 j = 0; j < _size; j++) {
            if ((*list)[j]->lastUpdateTime > last) {
                cookies.PushBack((*list)[j]);
            }
        }
    }

}

/*synchronized*/
CARAPI_(void) CCookieManager::DeleteACookie(
    /* [in] */ Cookie& cookie)
{
    Mutex::Autolock lock(_m_syncLock);

    if (cookie.mode == Cookie::MODE_DELETED) {
        String baseDomain;
        GetBaseDomain(cookie.domain, baseDomain);
        Vector<AutoPtr<Cookie> > *cookieList = NULL;
        mCookieMap->Get(baseDomain, (IInterface**)&cookieList);
        if (cookieList != NULL) {
//            cookieList->remove(cookie);
//            if (cookieList.isEmpty())
            {
//                mCookieMap.remove(baseDomain);
            }
        }
    }
}

/*synchronized*/
CARAPI_(void) CCookieManager::SyncedACookie(
    /* [in] */ Cookie& cookie)
{
    Mutex::Autolock lock(_m_syncLock);
    cookie.mode = Cookie::MODE_NORMAL;
}

/*synchronized*/
CARAPI_(void) CCookieManager::DeleteLRUDomain(
    /* [out] */ Vector<AutoPtr<Cookie> >& retlist)
{
    Mutex::Autolock lock(_m_syncLock);

    Int32 count = 0;
    Int32 byteCount = 0;
    Int32 mapSize = 0;

    mCookieMap->GetSize(&mapSize);

    if (mapSize < MAX_RAM_DOMAIN_COUNT) {
        ArrayOf<String> *cookieLists = NULL;
        mCookieMap->GetAllItems(&cookieLists, NULL);
        Int32 size = cookieLists->GetLength();
        for (Int32 i = 0; i < size; i++) {
            Vector<AutoPtr<Cookie> >* list = NULL;
            mCookieMap->Get((*cookieLists)[i], (IInterface**)&list);

            if (DebugFlags::sCOOKIE_MANAGER) {
                Int32 _size = list->GetSize();
                for (Int32 j = 0; j < _size; j++) {
                    Cookie* cookie = (*list)[j];
                    // 14 is 3 * sizeof(long) + sizeof(boolean)
                    // + sizeof(byte)
                    byteCount += cookie->domain.GetSize()
                            + cookie->path.GetSize()
                            + cookie->name.GetSize()
                            + cookie->value.GetSize()
                            + 14;

                    count++;
                    if (count >= MAX_RAM_COOKIES_COUNT) break;
                }
            }
            else {
                count += list->GetSize();
            }
        }
    }
#if 0
    ArrayList<Cookie> retlist = new ArrayList<Cookie>();
    if (mapSize >= MAX_RAM_DOMAIN_COUNT || count >= MAX_RAM_COOKIES_COUNT) {
        if (DebugFlags.COOKIE_MANAGER) {
            Log.v(LOGTAG, count + " cookies used " + byteCount
                    + " bytes with " + mapSize + " domains");
        }
        Object[] domains = mCookieMap.keySet().toArray();
        int toGo = mapSize / 10 + 1;
        while (toGo-- > 0){
            String domain = domains[toGo].toString();
            if (DebugFlags.COOKIE_MANAGER) {
                Log.v(LOGTAG, "delete domain: " + domain
                        + " from RAM cache");
            }
            retlist.addAll(mCookieMap.get(domain));
            mCookieMap.remove(domain);
        }
    }
    return retlist;
#endif
}

CARAPI_(void) CCookieManager::GetHostAndPath(
    /* [in] */ IWebAddress* uri,
    /* [out] */ Vector<String>& ret) const
{
    assert(uri != NULL);

//    if (uri.mHost != null && uri.mPath != null)
    {

        /*
         * The domain (i.e. host) portion of the cookie is supposed to be
         * case-insensitive. We will consistently return the domain in lower
         * case, which allows us to do the more efficient equals comparison
         * instead of equalIgnoreCase.
         *
         * See: http://www.ieft.org/rfc/rfc2965.txt (Section 3.3.3)
         */
        //String[] ret = new String[2];
//        ret[0] = uri.mHost.toLowerCase();
//        ret[1] = uri.mPath;

        Int32 index = ret[0].IndexOf(PERIOD);
        if (index == -1) {
//            if (uri.mScheme.equalsIgnoreCase("file"))
            {
                // There is a potential bug where a local file path matches
                // another file in the local web server directory. Still
                // "localhost" is the best pseudo domain name.
                ret[0] = "localhost";
            }
        } else if (index == ret[0].LastIndexOf(PERIOD)) {
            // cookie host must have at least two periods
            ret[0] = (const char*)&PERIOD + ret[0];
        }

        if (ret[1].GetChar(0) != PATH_DELIM) {
            ret.Clear();
            return;
        }

        /*
         * find cookie path, e.g. for http://www.google.com, the path is "/"
         * for http://www.google.com/lab/, the path is "/lab"
         * for http://www.google.com/lab/foo, the path is "/lab/foo"
         * for http://www.google.com/lab?hl=en, the path is "/lab"
         * for http://www.google.com/lab.asp?hl=en, the path is "/lab.asp"
         * Note: the path from URI has at least one "/"
         * See:
         * http://www.unix.com.ua/rfc/rfc2109.html
         */
        index = ret[1].IndexOf(QUESTION_MARK);
        if (index != -1) {
            ret[1] = ret[1].Substring(0, index);
        }
    }
}

CARAPI_(void) CCookieManager::GetBaseDomain(
    /* [in] */ String& host,
    /* [out] */ String& strOut) const
{
    Int32 startIndex = 0;
    Int32 nextIndex = host.IndexOf(PERIOD);
    Int32 lastIndex = host.LastIndexOf(PERIOD);

    while (nextIndex < lastIndex) {
        startIndex = nextIndex + 1;
        nextIndex = host.IndexOf(PERIOD, startIndex);
    }

    if (startIndex > 0) {
        strOut = host.Substring(startIndex);
    } else {
        strOut = host;
    }
}

CARAPI_(void) CCookieManager::ParseCookie(
    /* [in] */ String& host,
    /* [in] */ String& path,
    /* [in] */ String& cookieString,
    /* [out] */ Vector<AutoPtr<Cookie> >& ret)
{
    //ArrayList<Cookie> ret = new ArrayList<Cookie>();

    Int32 index = 0;
    Int32 length = cookieString.GetSize();
    while (true) {
        Cookie* cookie = NULL;

        // done
        if (index < 0 || index >= length) {
            break;
        }

        // skip white space
        if (cookieString.GetChar(index) == WHITE_SPACE) {
            index++;
            continue;
        }

        /*
         * get NAME=VALUE; pair. detecting the end of a pair is tricky, it
         * can be the end of a string, like "foo=bluh", it can be semicolon
         * like "foo=bluh;path=/"; or it can be enclosed by \", like
         * "foo=\"bluh bluh\";path=/"
         *
         * Note: in the case of "foo=bluh, bar=bluh;path=/", we interpret
         * it as one cookie instead of two cookies.
         */
        Int32 semicolonIndex = cookieString.IndexOf(SEMICOLON, index);
        Int32 equalIndex = cookieString.IndexOf(EQUAL, index);
        cookie = new Cookie(host, path);

        // Cookies like "testcookie; path=/;" are valid and used
        // (lovefilm.se).
        // Look for 2 cases:
        // 1. "foo" or "foo;" where equalIndex is -1
        // 2. "foo; path=..." where the first semicolon is before an equal
        //    and a semicolon exists.
        if ((semicolonIndex != -1 && (semicolonIndex < equalIndex)) ||
                equalIndex == -1) {
            // Fix up the index in case we have a string like "testcookie"
            if (semicolonIndex == -1) {
                semicolonIndex = length;
            }
            cookie->name = cookieString.Substring(index, semicolonIndex);
            //cookie->value = null;
        } else {
            cookie->name = cookieString.Substring(index, equalIndex);
            // Make sure we do not throw an exception if the cookie is like
            // "foo="
            if ((equalIndex < length - 1) &&
                    (cookieString.GetChar(equalIndex + 1) == QUOTATION)) {

                index = cookieString.IndexOf(QUOTATION, equalIndex + 2);
                if (index == -1) {
                    // bad format, force return
                    break;
                }
            }
            // Get the semicolon index again in case it was contained within
            // the quotations.
            semicolonIndex = cookieString.IndexOf(SEMICOLON, index);
            if (semicolonIndex == -1) {
                semicolonIndex = length;
            }

            if (semicolonIndex - equalIndex > MAX_COOKIE_LENGTH) {
                // cookie is too big, trim it
                cookie->value = cookieString.Substring(equalIndex + 1,
                        equalIndex + 1 + MAX_COOKIE_LENGTH);
            } else if (equalIndex + 1 == semicolonIndex
                    || semicolonIndex < equalIndex) {
                // this is an unusual case like "foo=;" or "foo="
                cookie->value = NULL;
            } else {
                cookie->value = cookieString.Substring(equalIndex + 1,
                        semicolonIndex);
            }
        }
        // get attributes
        index = semicolonIndex;
        while (TRUE) {
            // done
            if (index < 0 || index >= length) {
                break;
            }

            // skip white space and semicolon
            if (cookieString.GetChar(index) == WHITE_SPACE
                    || cookieString.GetChar(index) == SEMICOLON) {
                index++;
                continue;
            }

            // comma means next cookie
            if (cookieString.GetChar(index) == COMMA) {
                index++;
                break;
            }

            // "secure" is a known attribute doesn't use "=";
            // while sites like live.com uses "secure="
            if (length - index >= SECURE_LENGTH
                    && cookieString.Substring(index, index + SECURE_LENGTH).
                    EqualsIgnoreCase(SECURE)) {
                index += SECURE_LENGTH;
                cookie->secure = true;
                if (index == length) break;
                if (cookieString.GetChar(index) == EQUAL) index++;
                continue;
            }

            // "httponly" is a known attribute doesn't use "=";
            // while sites like live.com uses "httponly="
            if (length - index >= HTTP_ONLY_LENGTH
                    && cookieString.Substring(index,
                        index + HTTP_ONLY_LENGTH).
                    EqualsIgnoreCase(HTTP_ONLY)) {
                index += HTTP_ONLY_LENGTH;
                if (index == length) break;
                if (cookieString.GetChar(index) == EQUAL) index++;
                // FIXME: currently only parse the attribute
                continue;
            }

            equalIndex = cookieString.IndexOf(EQUAL, index);
            if (equalIndex > 0) {
                cookieString.Substring(index, equalIndex)
                        .ToLowerCase();
                String name = cookieString;
                if (name.Equals(EXPIRES)) {
                    int comaIndex = cookieString.IndexOf(COMMA, equalIndex);

                    // skip ',' in (Wdy, DD-Mon-YYYY HH:MM:SS GMT) or
                    // (Weekday, DD-Mon-YY HH:MM:SS GMT) if it applies.
                    // "Wednesday" is the longest Weekday which has length 9
                    if ((comaIndex != -1) &&
                            (comaIndex - equalIndex <= 10)) {
                        index = comaIndex + 1;
                    }
                }
                semicolonIndex = cookieString.IndexOf(SEMICOLON, index);
                Int32 commaIndex = cookieString.IndexOf(COMMA, index);
                if (semicolonIndex == -1 && commaIndex == -1) {
                    index = length;
                } else if (semicolonIndex == -1) {
                    index = commaIndex;
                } else if (commaIndex == -1) {
                    index = semicolonIndex;
                } else {
                    //index = Math.min(semicolonIndex, commaIndex);
                    index = semicolonIndex < commaIndex ? semicolonIndex : commaIndex;
                }
                String value =
                        cookieString.Substring(equalIndex + 1, index);
                
                // Strip quotes if they exist
                if (value.GetLength() > 2 && value.GetChar(0) == QUOTATION) {
                    Int32 endQuote = value.IndexOf(QUOTATION, 1);
                    if (endQuote > 0) {
                        value = value.Substring(1, endQuote);
                    }
                }

                if (name.Equals(EXPIRES)) {
                    //try {
//                        cookie.expires = AndroidHttpClient.parseDate(value);
                    //} catch (IllegalArgumentException ex) {
                    //    Log.e(LOGTAG,
                    //            "illegal format for expires: " + value);
                    //}
                } else if (name.Equals(MAX_AGE)) {
                    //try {
//                        cookie.expires = System.currentTimeMillis() + 1000
//                                * Long.parseLong(value);
                    //} catch (NumberFormatException ex) {
                    //    Log.e(LOGTAG,
                    //            "illegal format for max-age: " + value);
                    //}
                } else if (name.Equals(PATH)) {
                    // only allow non-empty path value
                    if (value.GetLength() > 0) {
                        cookie->path = value;
                    }
                } else if (name.Equals(DOMAIN)) {

                    Int32 lastPeriod = value.LastIndexOf(PERIOD);
                    if (lastPeriod == 0) {
                        // disallow cookies set for TLDs like [.com]
                        cookie->domain = NULL;
                        continue;
                    }
                    //try {
//                        Integer.parseInt(value.substring(lastPeriod + 1));
                        // no wildcard for ip address match
                        if (!value.Equals(host)) {
                            // no cross-site cookie
                            cookie->domain = NULL;
                        }
                        continue;
                    //} catch (NumberFormatException ex) {
                        // ignore the exception, value is a host name
                    //}
                    value.ToLowerCase();
                    if (value.GetChar(0) != PERIOD) {
                        // pre-pended dot to make it as a domain cookie
                        value = (const char*)&PERIOD + value;
                        lastPeriod++;
                    }

                    if (host.EndWith(value.Substring(1))) {
                        
                        Int32 len = value.GetLength();
                        Int32 hostLen = host.GetLength();
                        
                        if (hostLen > (len - 1)
                                && host.GetChar(hostLen - len) != PERIOD) {
                            // make sure the bar.com doesn't match .ar.com
                            cookie->domain = NULL;
                            continue;
                        }

                        // disallow cookies set on ccTLDs like [.co.uk]
                        if ((len == lastPeriod + 3)
                                && (len >= 6 && len <= 8)) {
                            String s = value.Substring(1, lastPeriod);
//                            if (Arrays.binarySearch(BAD_COUNTRY_2LDS, s) >= 0)
                            {
                                cookie->domain = NULL;
                                continue;
                            }
                        }
                        cookie->domain = value;
                    } else {
                        // no cross-site or more specific sub-domain cookie
                        cookie->domain = NULL;
                    }
                }
            } else {
                // bad format, force return
                index = length;
            }
        }

        if (cookie != NULL && cookie->domain.GetLength() != 0) {
            ret.PushBack(cookie);
        }
    }
}

/*********************************Cookie**************************************/

CCookieManager::Cookie::Cookie()
{}

CCookieManager::Cookie::Cookie(
    /* [in] */ String& defaultDomain,
    /* [in] */ String& defaultPath)
{
    domain = defaultDomain;
    path = defaultPath;
    expires = -1;
}

CARAPI_(Boolean) CCookieManager::Cookie::ExactMatch(
    /* [in] */ Cookie& in)
{
    // An exact match means that domain, path, and name are equal. If
    // both values are null, the cookies match. If both values are
    // non-null, the cookies match. If one value is null and the other
    // is non-null, the cookies do not match (i.e. "foo=;" and "foo;")
    Boolean valuesMatch = !((value.GetLength() == 0) ^ (in.value.GetLength() == 0));
    return domain.Equals(in.domain) && path.Equals(in.path) &&
            name.Equals(in.name) && valuesMatch;
}

CARAPI_(Boolean) CCookieManager::Cookie::DomainMatch(
    /* [in] */ String& urlHost)
{
    if (domain.StartWith(".")) {
        if (urlHost.EndWith(domain.Substring(1))) {
            
            Int32 len = domain.GetLength();
            Int32 urlLen = urlHost.GetLength();
            
            if (urlLen > len - 1) {
                // make sure bar.com doesn't match .ar.com
                return (urlHost.GetChar(urlLen - len) == PERIOD);
            }

            return TRUE;
        }

        return FALSE;
    } else {
        // exact match if domain is not leading w/ dot
        return urlHost.Equals(domain);
    }
}

CARAPI_(Boolean) CCookieManager::Cookie::PathMatch(
    /* [in] */ String& urlPath)
{
    if (urlPath.StartWith(path)) {

        Int32 len = path.GetLength();
        if (len == 0) {
            Logger::W(LOGTAG, "Empty cookie path");
            return FALSE;
        }

        Int32 urlLen = urlPath.GetLength();
        if (path.GetChar(len-1) != PATH_DELIM && urlLen > len) {
            // make sure /wee doesn't match /we
            return (urlPath.GetChar(len) == PATH_DELIM);
        }

        return TRUE;
    }

    return FALSE;
}

CARAPI_(void) CCookieManager::Cookie::ToString(String& str)
{
    str = "domain: ";
    str += domain;
    str += "; path: ";
    str += path;
    str += "; name: ";
    str += name;
    str += "; value: ";
    str += value;
}