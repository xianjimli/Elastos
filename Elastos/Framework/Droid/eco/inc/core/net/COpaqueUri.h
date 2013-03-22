
#ifndef __COPAQUEURI_H__
#define __COPAQUEURI_H__

#include "_COpaqueUri.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "net/Uri.h"
#include "net/CPart.h"

/**
 * Opaque URI.
 */
class OpaqueUri : public Uri
{
protected:
    /** Used in parcelling. */
    static const Int32 TYPE_ID = 2;

    String mScheme;
    AutoPtr<IPart> mSsp;
    AutoPtr<IPart> mFragment;
    String mCachedString;

public:
    OpaqueUri()
        : mCachedString(Uri::NOT_CACHED)
    {
    }

    CARAPI Init(
        /* [in] */ const String& scheme,
        /* [in] */ IPart* ssp,
        /* [in] */ IPart* fragment)
    {
        mScheme = scheme;
        mSsp = ssp;
        mFragment = (fragment == NULL) ?
            (IPart*)Part::PartNULL : fragment;

        return NOERROR;
    }

    static CARAPI ReadFrom(
        /* [in] */ IParcel* parcel,
        /* [out] */ IUri** result);

    CARAPI_(Void) WriteToParcel(
        /* [in] */ IParcel* parcel,
        /* [in] */ Int32 flags)
    {
        parcel->WriteInt32(OpaqueUri::TYPE_ID);
        parcel->WriteString(mScheme);
        ((AbstractPart*)(IPart*)mSsp)->WriteTo(parcel);
        ((AbstractPart*)(IPart*)mFragment)->WriteTo(parcel);
    }

    virtual CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical)
    {
        VALIDATE_NOT_NULL(isHierarchical);
        *isHierarchical = FALSE;
        return NOERROR;
    }

    virtual CARAPI IsRelative(
        /* [out] */ Boolean* isRelative)
    {
        VALIDATE_NOT_NULL(isRelative);
        *isRelative = mScheme.IsNull();
        return NOERROR;
    }

    virtual CARAPI GetScheme(
        /* [out] */ String* scheme)
    {
        VALIDATE_NOT_NULL(scheme);
        *scheme = mScheme;
        return NOERROR;
    }

    virtual CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp)
    {
        return mSsp->GetEncoded(essp);
    }

    virtual CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp)
    {
        return mSsp->GetDecoded(ssp);
    }

    virtual CARAPI GetAuthority(
        /* [out] */ String* authority)
    {
        VALIDATE_NOT_NULL(authority);
        authority->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetEncodedAuthority(
        /* [out] */ String* authority)
    {
        VALIDATE_NOT_NULL(authority);
        authority->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetPath(
        /* [out] */ String* path)
    {
        VALIDATE_NOT_NULL(path);
        path->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetEncodedPath(
        /* [out] */ String* path)
    {
        VALIDATE_NOT_NULL(path);
        path->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetQuery(
        /* [out] */ String* query)
    {
        VALIDATE_NOT_NULL(query);
        query->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetEncodedQuery(
        /* [out] */ String* query)
    {
        VALIDATE_NOT_NULL(query);
        query->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetFragment(
        /* [out] */ String* fragment)
    {
        return mFragment->GetDecoded(fragment);
    }

    virtual CARAPI GetEncodedFragment(
        /* [out] */ String* fragment)
    {
        return mFragment->GetEncoded(fragment);
    }

    virtual CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments)
    {
        VALIDATE_NOT_NULL(pathSegments);
        *pathSegments = NULL;
        return NOERROR;
    }

    virtual CARAPI GetLastPathSegment(
        /* [out] */ String* pathSegment)
    {
        VALIDATE_NOT_NULL(pathSegment);
        pathSegment->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetUserInfo(
        /* [out] */ String* userInfo)
    {
        VALIDATE_NOT_NULL(userInfo);
        userInfo->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetEncodedUserInfo(
        /* [out] */ String* userInfo)
    {
        VALIDATE_NOT_NULL(userInfo);
        userInfo->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetHost(
        /* [out] */ String* host)
    {
        VALIDATE_NOT_NULL(host);
        host->SetTo(NULL);
        return NOERROR;
    }

    virtual CARAPI GetPort(
        /* [out] */ Int32* port)
    {
        VALIDATE_NOT_NULL(port);
        *port = -1;
        return NOERROR;
    }

    virtual CARAPI ToString(
        /* [out] */ String* result)
    {
        VALIDATE_NOT_NULL(result);

        // @SuppressWarnings("StringEquality")
        Boolean cached = (!mCachedString.Equals(Uri::NOT_CACHED));
        if (!cached) {
            String str1 = mScheme;
            str1.Append(":");

            String str2;
            FAIL_RETURN(GetEncodedSchemeSpecificPart(&str2));
            str1.Append(str2);

            Boolean isEmpty;
            FAIL_RETURN(mFragment->IsEmpty(&isEmpty));
            if (!isEmpty) {
                str1.Append("#");
                String str3;
                FAIL_RETURN(mFragment->GetEncoded(&str3));
                str1.Append(str3);
            }

            mCachedString = str1;
        }

        *result = mCachedString;

        return NOERROR;
    }

    virtual CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result);
};

CarClass(COpaqueUri), public OpaqueUri
{
public:
    CARAPI GetDescription(
        /* [out] */ String* uriString);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical);

    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    CARAPI IsRelative(
        /* [out] */ Boolean* isRelative);

    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp);

    CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetEncodedAuthority(
        /* [out] */ String* authority);

    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetEncodedUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetHost(
        /* [out] */ String* host);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetEncodedPath(
        /* [out] */ String* path);

    CARAPI GetQuery(
        /* [out] */ String* query);

    CARAPI GetEncodedQuery(
        /* [out] */ String* query);

    CARAPI GetFragment(
        /* [out] */ String* fragment);

    CARAPI GetEncodedFragment(
        /* [out] */ String* fragment);

    CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments);

    CARAPI GetLastPathSegment(
        /* [out] */ String* pathSegment);

    CARAPI Equals(
        /* [in] */ IUri* other,
        /* [out] */ Boolean* isEqual);

    CARAPI HashCode(
        /* [out] */ Int32* hashCode);

    CARAPI CompareTo(
        /* [in] */ IUri* other,
        /* [out] */ Int32* result);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ String scheme,
        /* [in] */ IPart* ssp,
        /* [in] */ IPart* fragment);

private:
    // TODO: Add your private member variables here.
};

#endif // __COPAQUEURI_H__
