
#include "CCookieManager.h"
#include "CookieStoreImpl.h"
#include "CHttpCookie.h"
#include "CCookiePolicyHelper.h"
#include <StringBuffer.h>
#include <elastos/Math.h>

using namespace Elastos::Core;

const CString CCookieManager::VERSION_ZERO_HEADER = "Set-cookie";
const CString CCookieManager::VERSION_ONE_HEADER = "Set-cookie2";


ECode CCookieManager::constructor()
{
    return constructor(NULL, NULL);
}

ECode CCookieManager::constructor(
    /* [in] */ ICookieStore* store,
    /* [in] */ ICookiePolicy* cookiePolicy)
{
    mStore = store == NULL ? new CookieStoreImpl() : store;

    if (cookiePolicy == NULL) {
        mPolicy = CCookiePolicyHelper::GetCookiePolicy(
                CookiePolicyKind_ACCEPT_ORIGINAL_SERVER);
    }
    else mPolicy = cookiePolicy;

    return NOERROR;
}

ECode CCookieManager::Get(
    /* [in] */ IURI* uri,
    /* [in] */ IObjectStringMap* requestHeaders,
    /* [out] */ IObjectStringMap** cookiesMap)
{
    VALIDATE_NOT_NULL(cookiesMap);

    if (uri == NULL || requestHeaders == NULL) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List< AutoPtr<IHttpCookie> > result;
    AutoPtr<IObjectContainer> cookies;
    mStore->Get(uri, (IObjectContainer**)&cookies);
    assert(cookies != NULL);
    AutoPtr<IObjectEnumerator> enumerator;
    cookies->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IHttpCookie> cookie;
        enumerator->Current((IInterface**)&cookie);
        if (CHttpCookie::PathMatches(cookie, uri)
                && CHttpCookie::SecureMatches(cookie, uri)
                && CHttpCookie::PortMatches(cookie, uri)) {
            result.PushBack(cookie);
        }
    }

    return CookiesToHeaders(result, cookiesMap);
}

ECode CCookieManager::CookiesToHeaders(
    /* [in] */ List<AutoPtr<IHttpCookie> >& cookies,
    /* [out] */ IObjectStringMap** cookiesMap)
{
    assert(cookiesMap != NULL);

    if (cookies.IsEmpty()) {
//        return Collections.emptyMap();
        return CObjectStringMap::New(cookiesMap);
    }

    StringBuffer result;

    // If all cookies are version 1, add a version 1 header. No header for version 0 cookies.
    Int32 minVersion = 1;
    List<AutoPtr<IHttpCookie> >::Iterator it;
    for(it = cookies.Begin(); it != cookies.End(); ++it) {
        IHttpCookie* cookie = *it;
        Int32 version;
        cookie->GetVersion(&version);
        minVersion = Math::Min(minVersion, version);
    }
    if (minVersion == 1) {
        result += "$Version=\"1\"; ";
    }

    it = cookies.Begin();
    CHttpCookie* cookieObj = (CHttpCookie*)(*it).Get();
    result += cookieObj->ToString();
    for(++it; it != cookies.End(); ++it) {
        cookieObj = (CHttpCookie*)(*it).Get();
        result += "; ";
        result += cookieObj->ToString();
    }

    AutoPtr<IObjectContainer> container;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&container));
    AutoPtr<ICharSequence> cookiesStr;
    FAIL_RETURN(CStringWrapper::New(String(result), (ICharSequence**)&cookiesStr));
    container->Add(cookiesStr);

    FAIL_RETURN(CObjectStringMap::New(cookiesMap));
    (*cookiesMap)->Put(String("Cookie"), container);

    return NOERROR;
}

ECode CCookieManager::Put(
    /* [in] */ IURI* uri,
    /* [in] */ IObjectStringMap* responseHeaders)
{
    if (uri == NULL || responseHeaders == NULL) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //parse and construct cookies according to the map
    //ToDo: when to delete cookies?
    List< AutoPtr<IHttpCookie> >* cookies = ParseCookie(responseHeaders);
    List< AutoPtr<IHttpCookie> >::Iterator it;
    for (it = cookies->Begin(); it != cookies->End(); ++it) {
        IHttpCookie* cookie = *it;

        // if the cookie doesn't have a domain, set one. The policy will do validation.
        String domain;
        cookie->GetDomainAttr(&domain);
        if (domain.IsNull()) {
            String host;
            uri->GetHost(&host);
            cookie->SetDomain(host);
        }

        // if the cookie doesn't have a path, set one. If it does, validate it.
        String path;
        cookie->GetPath(&path);
        if (path.IsNull()) {
            String uriPath;
            uri->GetPath(&uriPath);
            cookie->SetPath(PathToCookiePath(uriPath));
        }
        else if (!CHttpCookie::PathMatches(cookie, uri)) {
            continue;
        }

        // if the cookie has the placeholder port list "", set the port. Otherwise validate it.
        String portList;
        cookie->GetPortList(&portList);
        if (portList.Equals("")) {
            Int32 port;
            uri->GetEffectivePort(&port);
            cookie->SetPortList(String::FromInt32(port));
        }
        else if (!portList.IsNull() && !CHttpCookie::PortMatches(cookie, uri)) {
            continue;
        }

        // if the cookie conforms to the policy, add it into the store
        Boolean result;
        mPolicy->ShouldAccept(uri, cookie, &result);
        if (result) {
            mStore->Add(uri, cookie);
        }
    }

    return NOERROR;
}

String CCookieManager::PathToCookiePath(
    /* [in] */ const String& path)
{
    if (path.IsNull()) {
        return String("/");
    }
    Int32 lastSlash = path.LastIndexOf('/'); // -1 yields the empty string
    return path.Substring(0, lastSlash + 1);
}

List<AutoPtr<IHttpCookie> >* CCookieManager::ParseCookie(
    /* [in] */ IObjectStringMap* responseHeaders)
{
    List< AutoPtr<IHttpCookie> >* cookies = new List< AutoPtr<IHttpCookie> >;
    ArrayOf<String>* keys;
    AutoPtr<IObjectContainer> values;
    responseHeaders->GetAllItems(&keys, (IObjectContainer**)&values);

    AutoPtr<IObjectEnumerator> enumerator;
    values->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    for(Int32 i = 0; i < keys->GetLength(); ++i) {
        String key = (*keys)[i];
        enumerator->MoveNext(&hasNext);
        assert(hasNext);
        // Only "Set-cookie" and "Set-cookie2" pair will be parsed
        if (!key.IsNull() && (key.EqualsIgnoreCase(VERSION_ZERO_HEADER)
                || key.EqualsIgnoreCase(VERSION_ONE_HEADER))) {
            AutoPtr<IObjectContainer> container;
            enumerator->Current((IInterface**)&container);
            AutoPtr<IObjectEnumerator> conEnumrator;
            container->GetObjectEnumerator((IObjectEnumerator**)&conEnumrator);
            Boolean hasNext;
            while(conEnumrator->MoveNext(&hasNext), hasNext) {
                // try{
                AutoPtr<ICharSequence> wrapper;
                conEnumrator->Current((IInterface**)&wrapper);
                String cookiesStr;
                wrapper->ToString(&cookiesStr);
                //Todo: CHttpCookie::Parse maybe should return IObjectContainer
                List< AutoPtr<IHttpCookie> > httpCookieList;
                CHttpCookie::Parse(cookiesStr, httpCookieList);
                List< AutoPtr<IHttpCookie> >::Iterator it;
                for (it = httpCookieList.Begin(); it != httpCookieList.End(); ++it) {
                    cookies->PushBack(*it);
                }
                // } catch (IllegalArgumentException ignored) {
                //     // this string is invalid, jump to the next one.
                // }
            }
        }
    }

    FREE_ARRAY_OF_STRING(keys);

    return cookies;
}

ECode CCookieManager::SetCookiePolicy(
    /* [in] */ ICookiePolicy* cookiePolicy)
{
    if (cookiePolicy != NULL) {
        mPolicy = cookiePolicy;
    }

    return NOERROR;
}

ECode CCookieManager::GetCookieStore(
    /* [out] */ ICookieStore** cookieStore)
{
    VALIDATE_NOT_NULL(cookieStore);
    *cookieStore = mStore;
    if (*cookieStore != NULL) (*cookieStore)->AddRef();
    return NOERROR;
}
