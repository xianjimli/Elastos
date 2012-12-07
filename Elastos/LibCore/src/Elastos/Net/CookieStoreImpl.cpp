
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
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException("cookie == null");
    }

    AutoPtr<IURI> temp = GetCookiesUri(uri);
    if (temp != NULL) {
        temp->AddRef();
    }
    uri = temp;

    List<AutoPtr<IHttpCookie> >* cookies;
    Map<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*>::Iterator it =
            mMap.Find(uri);
    if (it != mMap.End()) {
        cookies = it->mSecond;
    }

    if (cookies == NULL) {
        cookies = new List<AutoPtr<IHttpCookie> >();
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
    /* [out] */ IObjectContainer** httpCookies)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(httpCookies);

    if (uri == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException("uri == null");
    }

    AutoPtr<IObjectContainer> result;
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&result));

//    List<AutoPtr<IHttpCookie> >* result = new List<AutoPtr<IHttpCookie> >();

    // get cookies associated with given URI. If none, returns an empty list
    List<AutoPtr<IHttpCookie> >* cookiesForUri;
    Map<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*>::Iterator it =
            mMap.Find(uri);
    if (it != mMap.End()) {
        cookiesForUri = it->mSecond;
    }
    if (cookiesForUri != NULL) {
        List<AutoPtr<IHttpCookie> >::Iterator cookieIt;
        for (cookieIt = cookiesForUri->Begin(); cookieIt != cookiesForUri->End(); ++cookieIt) {
            AutoPtr<IHttpCookie> cookie = *cookieIt;
            Boolean hasExpired;
            cookie->HasExpired(&hasExpired);
            if (hasExpired) {
                cookieIt = cookiesForUri->Erase(cookieIt);
                --cookieIt;
                //i->Remove(); // remove expired cookies
            }
            else {
                result->Add(cookie);
            }
        }
    }

    // get all cookies that domain matches the URI
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        if (uri == it->mFirst/*uri.equals(entry.getKey()*/) {
            continue; // skip the given URI; we've already handled it
        }

        List<AutoPtr<IHttpCookie> >* entryCookies = it->mSecond;
        List<AutoPtr<IHttpCookie> >::Iterator cookieIt;
        for (cookieIt = entryCookies->Begin(); cookieIt != entryCookies->End(); ++cookieIt) {
            AutoPtr<IHttpCookie> cookie = *cookieIt;
            String attr, host;
            cookie->GetDomainAttr(&attr);
            uri->GetHost(&host);
            if (!CHttpCookie::DomainMatches(attr, host)) {
                continue;
            }
            Boolean hasExpired;
            cookie->HasExpired(&hasExpired);
            if (hasExpired) {
                cookieIt = entryCookies->Erase(cookieIt);
                --cookieIt;
//                i.remove(); // remove expired cookies
            }
            else {
                AutoPtr<IObjectEnumerator> enumerator;
                result->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
                Boolean hasNext;
                while(enumerator->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IHttpCookie> resultCookie;
                    enumerator->Current((IInterface**)&cookie);
                    if (resultCookie == cookie) {
                        break;
                    }
                }
                if (!hasNext)
                    result->Add(cookie);
            }
        }
    }
    *httpCookies = result;
//    return Collections.unmodifiableList(result);

    return NOERROR;
}

ECode CookieStoreImpl::GetCookies(
    /* [out] */ IObjectContainer** httpCookies)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(httpCookies);

    AutoPtr<IObjectContainer> result;
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&result));

    Map<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*>::Iterator it;
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        List<AutoPtr<IHttpCookie> >* list = it->mSecond;
        List<AutoPtr<IHttpCookie> >::Iterator listIt;
        for (listIt = list->Begin(); listIt!= list->End(); ++listIt) {
            AutoPtr<IHttpCookie> cookie = *listIt;
            Boolean hasExpired;
            cookie->HasExpired(&hasExpired);
            if (hasExpired) {
                listIt = list->Erase(listIt);
                --listIt;
            }
            else {
                AutoPtr<IObjectEnumerator> enumerator;
                result->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
                Boolean hasNext;
                while(enumerator->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IHttpCookie> resultCookie;
                    enumerator->Current((IInterface**)&cookie);
                    if (resultCookie == cookie) {
                        break;
                    }
                }
                if (!hasNext)
                    result->Add(cookie);
            }
        }
    }
    *httpCookies = result;
//    return Collections.unmodifiableList(result);

    return NOERROR;
}

ECode CookieStoreImpl::GetURIs(
    /* [out] */ IObjectContainer** URIs)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(URIs);

    AutoPtr<IObjectContainer> result;
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&result));

    Map<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*>::Iterator it;
    for (it = mMap.Begin(); it != mMap.End(); ++it) {
        AutoPtr<IURI> uri = it->mFirst;
        result->Add(uri);
    }
    *URIs = result;
//    result.remove(null); // sigh
//    return Collections.unmodifiableList(result);

    return NOERROR;
}

ECode CookieStoreImpl::Remove(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* succeeded)
{
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(succeeded);

    if (cookie == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException("cookie == null");
    }

    List<AutoPtr<IHttpCookie> >* cookies;
    Map<AutoPtr<IURI>, List<AutoPtr<IHttpCookie> >*>::Iterator it =
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
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(succeeded);
    *succeeded = mMap.Begin() != mMap.End();
    mMap.Clear();

    return NOERROR;
}
