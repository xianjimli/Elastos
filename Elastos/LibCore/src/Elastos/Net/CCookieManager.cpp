
#include "CCookieManager.h"
#include "CookieStoreImpl.h"
#include "CHttpCookie.h"
#include "CCookiePolicyHelper.h"
#include <StringBuffer.h>
#include <elastos/Math.h>

using namespace Elastos::Core;

const CString CCookieManager::VERSION_ZERO_HEADER = "Set-cookie";
const CString CCookieManager::VERSION_ONE_HEADER = "Set-cookie2";

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
    return NOERROR;
}

ECode CCookieManager::Get(
	/* [in] */ IURI* uri,
    /* [in] */ IObjectStringMap* requestHeaders,
    /* [out] */ IObjectStringMap** cookiesMap)
{
	if (uri == NULL || requestHeaders == NULL) {
		return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    List<AutoPtr<IHttpCookie> > result;
    AutoPtr<IObjectContainer> cookies;
    mStore->Get(uri, (IObjectContainer**)&cookies);
    if (cookies != NULL) {
        AutoPtr<IObjectEnumerator> enumerator;
        cookies->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext;
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IHttpCookie> cookie;
            enumerator->Current((IInterface**)&cookie);
            if (CHttpCookie::PathMatches(cookie, uri)
	                && CHttpCookie::SecureMatches(cookie, uri)
	                && CHttpCookie::PortMatches(cookie, uri)) {
            	//begin from this
	            result.PushBack(cookie);
	        }
        }
    }

    return CookiesToHeaders(result, cookiesMap);
}

ECode CCookieManager::CookiesToHeaders(
	/* [in] */ List<AutoPtr<IHttpCookie> > cookies,
	/* [out] */ IObjectStringMap** cookiesMap)
{
	if (cookies.Begin() == cookies.End()) {
		return NOERROR;
//        return Collections.emptyMap();
    }

    StringBuffer result;

    // If all cookies are version 1, add a version 1 header. No header for version 0 cookies.
    Int32 minVersion = 1;
    List<AutoPtr<IHttpCookie> >::Iterator it;
    for(it = cookies.Begin(); it != cookies.End(); ++it) {
    	AutoPtr<IHttpCookie> cookie = *it;
        Int32 version;
        cookie->GetVersion(&version);
        minVersion = Math::Min(minVersion, version);
    }

    AutoPtr<IObjectContainer> stringWrappers;
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&stringWrappers));
    if (minVersion == 1) {
//        result += "$Version=\"1\"; ";
        AutoPtr<ICharSequence> sw;
        CStringWrapper::New(String("$Version=\"1\"; "), (ICharSequence**)&sw);
        stringWrappers->Add(sw);
    }

    for(it = cookies.Begin(); it != cookies.End(); ++it) {
    	AutoPtr<CHttpCookie> cookieCls = (CHttpCookie*)(*it).Get();
    	AutoPtr<ICharSequence> sw;
        ASSERT_SUCCEEDED(CStringWrapper::New(cookieCls->ToString(), (ICharSequence**)&sw));
        stringWrappers->Add(sw);
    }

    ASSERT_SUCCEEDED(CObjectStringMap::New(cookiesMap));
    (*cookiesMap)->Put(String("Cookie"), stringWrappers);

    return NOERROR;

    // for (int i = 1; i < cookies.size(); i++) {
    //     result.append("; ").append(cookies.get(i).toString());
    // }

    // return Collections.singletonMap("Cookie", Collections.singletonList(result.toString()));
}

ECode CCookieManager::Put(
	/* [in] */ IURI* uri,
	/* [in] */ IObjectStringMap* responseHeaders)
{
	if (uri == NULL || responseHeaders == NULL) {
		return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    //parse and construct cookies according to the map
    List<AutoPtr<IHttpCookie> > cookies = ParseCookie(responseHeaders);
    List<AutoPtr<IHttpCookie> >::Iterator it;
    for (it = cookies.Begin(); it != cookies.End(); ++it) {
    	AutoPtr<IHttpCookie> cookie = *it;

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
        if (String("").Equals(portList)) {
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

List<AutoPtr<IHttpCookie> > CCookieManager::ParseCookie(
	/* [in] */ IObjectStringMap* responseHeaders)
{
	List<AutoPtr<IHttpCookie> > cookies;
	ArrayOf<String>* keys;
	AutoPtr<IObjectContainer> container;
	responseHeaders->GetAllItems(&keys, (IObjectContainer**)&container);

	AutoPtr<IObjectEnumerator> enumerator;
    container->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    enumerator->MoveNext(&hasNext);
	for(Int32 i = 0; i < keys->GetLength(); ++i) {
		String key = (*keys)[i];
		AutoPtr<IObjectContainer> stringWrappers;
		if (hasNext) {
            enumerator->Current((IInterface**)&stringWrappers);
            enumerator->MoveNext(&hasNext);
		}
		else {
			break;
		}

		if (!key.IsNull() && (key.EqualsIgnoreCase(String(VERSION_ZERO_HEADER)))
				|| key.EqualsIgnoreCase(String(VERSION_ONE_HEADER))) {
			AutoPtr<IObjectEnumerator> swenum;
		    stringWrappers->GetObjectEnumerator((IObjectEnumerator**)&swenum);
		    Boolean hasNext;
		    while(enumerator->MoveNext(&hasNext), hasNext) {
		    	AutoPtr<ICharSequence> stringWrapper;
		    	swenum->Current((IInterface**)&stringWrapper);
		    	String cookieStr;
		    	stringWrapper->ToString(&cookieStr);
		    	List<AutoPtr<IHttpCookie> > httpCookieList;
		    	CHttpCookie::Parse(cookieStr, &httpCookieList);
		    	List<AutoPtr<IHttpCookie> >::Iterator it;
		    	for (it = httpCookieList.Begin(); it != httpCookieList.End(); ++it) {
		    		cookies.PushBack(*it);
		    	}
		    }
		}
	}

    // for (Map.Entry<String, List<String>> entry : responseHeaders.entrySet()) {
    //     String key = entry.getKey();
    //     // Only "Set-cookie" and "Set-cookie2" pair will be parsed
    //     if (key != null && (key.equalsIgnoreCase(VERSION_ZERO_HEADER)
    //             || key.equalsIgnoreCase(VERSION_ONE_HEADER))) {
    //         // parse list elements one by one
    //         for (String cookieStr : entry.getValue()) {
    //             try {
    //                 for (HttpCookie cookie : HttpCookie.parse(cookieStr)) {
    //                     cookies.add(cookie);
    //                 }
    //             } catch (IllegalArgumentException ignored) {
    //                 // this string is invalid, jump to the next one.
    //             }
    //         }
    //     }
    // }
    return cookies;
}

ECode CCookieManager::constructor()
{
	return constructor(NULL, NULL);
}

ECode CCookieManager::constructor(
    /* [in] */ ICookieStore* store,
    /* [in] */ ICookiePolicy* cookiePolicy)
{
	if (store == NULL) {
		CookieStoreImpl* storeImpl = new CookieStoreImpl();
		if (storeImpl != NULL && storeImpl->Probe(EIID_ICookieStore) != NULL) {
			mStore = (ICookieStore*)storeImpl->Probe(EIID_ICookieStore);
		}
		else {
			mStore = NULL;
		}
	}
	else {
		mStore = store;
	}

	if (cookiePolicy == NULL) {
		AutoPtr<ICookiePolicyHelper> helper;
	    CCookiePolicyHelper::AcquireSingleton((ICookiePolicyHelper**)&helper);
	    helper->GetCookiePolicy(
	    		String(CookiePolicy_ACCEPT_ORIGINAL_SERVER), (ICookiePolicy**)&mPolicy);
	}
	else {
		mPolicy = cookiePolicy;
	}

	return NOERROR;
}
