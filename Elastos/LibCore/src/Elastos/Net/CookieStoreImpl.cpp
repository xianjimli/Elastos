
#include "CookieStoreImpl.h"
#include "CURI.h"
#include "CHttpCookie.h"


CookieStoreImpl::~CookieStoreImpl()
{
    mMap.Clear();
}

PInterface CookieStoreImpl::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ICookieStore) {
        return (ICookieStore*)this;
    }

    return NULL;
}

UInt32 CookieStoreImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CookieStoreImpl::Release()
{
    return ElRefBase::Release();
}

ECode CookieStoreImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CookieStoreImpl::Add(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie)
{
    Mutex::Autolock lock(mLock);

    if (cookie == NULL) {
//        throw new NullPointerException("cookie == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IURI> cookiesUri = GetCookiesUri(uri);

    List< AutoPtr<IHttpCookie> >* cookies = NULL;
    HashMap<AutoPtr<IURI>, List< AutoPtr<IHttpCookie> >*>::Iterator it =
            mMap.Find(cookiesUri);
    if (it != mMap.End()) {
        cookies = it->mSecond;
    }

    if (cookies == NULL) {
        cookies = new List< AutoPtr<IHttpCookie> >();
        mMap[uri] = cookies;
    }
    else {
        cookies->Remove(cookie);
    }
    cookies->PushBack(cookie);

    return NOERROR;
}

AutoPtr<IURI> CookieStoreImpl::GetCookiesUri(
    /* [in] */ IURI* uri)
{
    if (uri == NULL) {
        return NULL;
    }
//    try {
    String host;
    uri->GetHost(&host);
    AutoPtr<IURI> cookiesUri;
    ECode ec = CURI::New(String("http"), host, String(NULL), String(NULL), (IURI**)&cookiesUri);
    if (FAILED(ec)) {
        cookiesUri = uri;
    }

    return cookiesUri;
    // } catch (URISyntaxException e) {
    //     return uri; // probably a URI with no host
    // }
}

ECode CookieStoreImpl::Get(
    /* [in] */ IURI* uri,
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mLock);

    if (uri == NULL) {
//        throw new NullPointerException("uri == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

//    List<AutoPtr<IHttpCookie> >* result = new List<AutoPtr<IHttpCookie> >();
    FAIL_RETURN(CObjectContainer::New(result));

    // get cookies associated with given URI. If none, returns an empty list
    List< AutoPtr<IHttpCookie> >* cookiesForUri = NULL;
    HashMap<AutoPtr<IURI>, List< AutoPtr<IHttpCookie> >*>::Iterator it =
            mMap.Find(uri);
    if (it != mMap.End()) {
        cookiesForUri = it->mSecond;
    }
    if (cookiesForUri != NULL) {
        List<AutoPtr<IHttpCookie> >::Iterator cookieIt = cookiesForUri->Begin();
        while(cookieIt != cookiesForUri->End()) {
            AutoPtr<IHttpCookie> cookie = *cookieIt;
            Boolean hasExpired;
            if (cookie->HasExpired(&hasExpired), hasExpired) {
                cookieIt = cookiesForUri->Erase(cookieIt);
            }
            else {
                (*result)->Add(cookie);
                ++cookieIt;
            }
        }
    }

    // get all cookies that domain matches the URI
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        if (uri == it->mFirst/*uri.equals(entry.getKey()*/) {
            continue; // skip the given URI; we've already handled it
        }

        List<AutoPtr<IHttpCookie> >* entryCookies = it->mSecond;
        List<AutoPtr<IHttpCookie> >::Iterator cookieIt = entryCookies->Begin();
        while (cookieIt != entryCookies->End()) {
            AutoPtr<IHttpCookie> cookie = *cookieIt;
            String attr, host;
            cookie->GetDomainAttr(&attr);
            uri->GetHost(&host);
            if (!CHttpCookie::DomainMatches(attr, host)) {
                ++cookieIt;
                continue;
            }
            Boolean hasExpired;
            if (cookie->HasExpired(&hasExpired), hasExpired) {
                cookieIt = entryCookies->Erase(cookieIt);
            }
            else {
                AutoPtr<IObjectEnumerator> enumerator;
                (*result)->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
                Boolean hasNext, found = FALSE;
                while(enumerator->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IHttpCookie> resultCookie;
                    enumerator->Current((IInterface**)&cookie);
                    if (resultCookie == cookie) {
                        found = TRUE;
                        break;
                    }
                }
                if (!found) {
                    (*result)->Add(cookie);
                }
                ++cookieIt;
            }
        }
    }

//    return Collections.unmodifiableList(result);
    return NOERROR;
}

ECode CookieStoreImpl::GetCookies(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mLock);

    FAIL_RETURN(CObjectContainer::New(result));

    HashMap<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*>::Iterator it = mMap.Begin();
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        List<AutoPtr<IHttpCookie> >* list = it->mSecond;
        List<AutoPtr<IHttpCookie> >::Iterator listIt = list->Begin();
        while (listIt!= list->End()) {
            AutoPtr<IHttpCookie> cookie = *listIt;
            Boolean hasExpired;
            if (cookie->HasExpired(&hasExpired), hasExpired) {
                listIt = list->Erase(listIt);
            }
            else {
                AutoPtr<IObjectEnumerator> enumerator;
                (*result)->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
                Boolean hasNext, found = FALSE;
                while(enumerator->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IHttpCookie> resultCookie;
                    enumerator->Current((IInterface**)&cookie);
                    if (resultCookie == cookie) {
                        found = TRUE;
                        break;
                    }
                }
                if (!found) {
                    (*result)->Add(cookie);
                }
                ++listIt;
            }
        }
    }
//    return Collections.unmodifiableList(result);
    return NOERROR;
}

ECode CookieStoreImpl::GetURIs(
    /* [out] */ IObjectContainer** URIs)
{
    VALIDATE_NOT_NULL(URIs);

    Mutex::Autolock lock(mLock);

    FAIL_RETURN(CObjectContainer::New(URIs));

    HashMap<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*>::Iterator it;
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        AutoPtr<IURI> uri = it->mFirst;
        (*URIs)->Add(uri);
    }
//    result.remove(null); // sigh
//    return Collections.unmodifiableList(result);

    return NOERROR;
}

ECode CookieStoreImpl::Remove(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(mLock);

    if (cookie == NULL) {
//        throw new NullPointerException("cookie == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List<AutoPtr<IHttpCookie> >* cookies = NULL;
    HashMap<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*>::Iterator it =
            mMap.Find(GetCookiesUri(uri));
    if (it != mMap.End()) {
        cookies = it->mSecond;
    }
    if (cookies != NULL) {
        cookies->Remove(cookie);
        *succeeded = TRUE;
    }
    else {
        *succeeded = FALSE;
    }

    return NOERROR;
}

ECode CookieStoreImpl::RemoveAll(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    Mutex::Autolock lock(mLock);

    *succeeded = mMap.Begin() != mMap.End();
    mMap.Clear();

    return NOERROR;
}
