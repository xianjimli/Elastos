
#ifndef __CHIERARCHICALURI_H__
#define __CHIERARCHICALURI_H__

#include "_CHierarchicalUri.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "net/Uri.h"
#include "net/CPart.h"
#include "net/CPathPart.h"

/**
 * Hierarchical Uri.
 */
class HierarchicalUri : public AbstractHierarchicalUri
{
protected:
    /** Used in parcelling. */
    static const Int32 TYPE_ID = 3;

    String mScheme; // can be null
    AutoPtr<IPart> mAuthority;
    AutoPtr<IPathPart> mPath;
    AutoPtr<IPart> mQuery;
    AutoPtr<IPart> mFragment;
    AutoPtr<IPart> mSsp;
    String mUriString;

public:
    HierarchicalUri()
        : mUriString(Uri::NOT_CACHED)
    {
    }

    CARAPI Init(
        /* [in] */ const String& scheme,
        /* [in] */ IPart* authority,
        /* [in] */ IPathPart* path,
        /* [in] */ IPart* query,
        /* [in] */ IPart* fragment)
    {
        mScheme = scheme;
        FAIL_RETURN(Part::NonNull(authority, (IPart**)&mAuthority));
        mPath = (path == NULL) ? (IPathPart*)PathPart::PathPartNULL : path;
        FAIL_RETURN(Part::NonNull(query, (IPart**)&mQuery));
        FAIL_RETURN(Part::NonNull(fragment, (IPart**)&mFragment));

        return NOERROR;
    }

    static CARAPI ReadFrom(
        /* [in] */ IParcel* parcel,
        /* [out] */ IUri** result);

    CARAPI_(Void) WriteToParcel(
        /* [in] */ IParcel* parcel,
        /* [in] */ Int32 flags)
    {
        parcel->WriteInt32(HierarchicalUri::TYPE_ID);
        parcel->WriteString(mScheme);
        ((Part*)(IPart*)mAuthority)->WriteTo(parcel);
        ((PathPart*)(IPathPart*)mPath)->WriteTo(parcel);
        ((Part*)(IPart*)mQuery)->WriteTo(parcel);
        ((Part*)(IPart*)mFragment)->WriteTo(parcel);
    }

    virtual CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical)
    {
        VALIDATE_NOT_NULL(isHierarchical);
        *isHierarchical = TRUE;
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
        AutoPtr<IPart> part;
        FAIL_RETURN(GetSsp((IPart**)&part));

        return part->GetEncoded(essp);
    }

    virtual CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetSsp((IPart**)&part));

        return part->GetDecoded(ssp);
    }

    virtual CARAPI GetAuthority(
        /* [out] */ String* authority)
    {
        return mAuthority->GetDecoded(authority);
    }

    virtual CARAPI GetEncodedAuthority(
        /* [out] */ String* authority)
    {
        return mAuthority->GetEncoded(authority);
    }

    virtual CARAPI GetEncodedPath(
        /* [out] */ String* path)
    {
        return mPath->GetEncoded(path);
    }

    virtual CARAPI GetPath(
        /* [out] */ String* path)
    {
        return mPath->GetDecoded(path);
    }

    virtual CARAPI GetQuery(
        /* [out] */ String* query)
    {
        return mQuery->GetDecoded(query);
    }

    virtual CARAPI GetEncodedQuery(
        /* [out] */ String* query)
    {
        return mQuery->GetEncoded(query);
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

        AutoPtr<IPathSegments> segments;
        FAIL_RETURN(mPath->GetPathSegments((IPathSegments**)&segments));

        Int32 size;
        FAIL_RETURN(segments->Size(&size));

        ArrayOf<String>* array = ArrayOf<String>::Alloc(size);
        if (!array)
            return E_OUT_OF_MEMORY;

        for (Int32 i = 0; i < size; ++i) {
            String str;
            ECode ec = segments->Get(i, &str);
            if (FAILED(ec)) {
                ArrayOf<String>::Free(array);
                return ec;
            }

            (*array)[i] = str;
        }

        *pathSegments = array;

        return NOERROR;
    }

    // @Override
    virtual CARAPI ToString(
        /* [out] */ String* result)
    {
        VALIDATE_NOT_NULL(result);

        // @SuppressWarnings("StringEquality")
        Boolean cached = (!mUriString.Equals(Uri::NOT_CACHED));
        if (!cached) {
            FAIL_RETURN(MakeUriString(&mUriString));
        }

        *result = mUriString;

        return NOERROR;
    }

    CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result);

private:
    CARAPI GetSsp(
        /* [out] */ IPart** ssp)
    {
        VALIDATE_NOT_NULL(ssp);

        if (mSsp == NULL) {
            String str;
            FAIL_RETURN(MakeSchemeSpecificPart(&str));
            FAIL_RETURN(Part::FromEncoded(str, ssp));
            mSsp = *ssp;
        }
        else {
            *ssp = mSsp;
            (*ssp)->AddRef();
        }

        return NOERROR;
    }

    /**
     * Creates the encoded scheme-specific part from its sub parts.
     */
    CARAPI MakeSchemeSpecificPart(
        /* [out] */ String* result)
    {
        return AppendSspTo(String(NULL), result);
    }

    CARAPI AppendSspTo(
        /* [in] */ const String& builder,
        /* [out] */ String* result)
    {
        VALIDATE_NOT_NULL(result);
        result->SetTo(builder);

        String encodedAuthority;
        FAIL_RETURN(mAuthority->GetEncoded(&encodedAuthority));
        if (!encodedAuthority.IsNull()) {
            // Even if the authority is "", we still want to append "//".
            result->SetTo(builder);
            result->Append("//");
            result->Append(encodedAuthority);
        }

        String encodedPath;
        FAIL_RETURN(mPath->GetEncoded(&encodedPath));
        if (!encodedPath.IsNull()) {
            result->Append(encodedPath);
        }

        Boolean isEmpty;
        FAIL_RETURN(mQuery->IsEmpty(&isEmpty));
        if (!isEmpty) {
            result->Append("?");
            String encoded;
            FAIL_RETURN(mQuery->GetEncoded(&encoded));
            result->Append(encoded);
        }

        return NOERROR;
    }

    CARAPI MakeUriString(
        /* [out] */ String* result)
    {
        VALIDATE_NOT_NULL(result);

        String builder;

        if (!mScheme.IsNull()) {
            builder.Append(mScheme);
            builder.Append(":");
        }

        FAIL_RETURN(AppendSspTo(builder, result));

        Boolean isEmpty;
        FAIL_RETURN(mFragment->IsEmpty(&isEmpty));

        if (!isEmpty) {
            result->Append("#");
            String str;
            FAIL_RETURN(mFragment->GetEncoded(&str));
            result->Append(str);
        }

        return NOERROR;
    }
};

CarClass(CHierarchicalUri), public HierarchicalUri
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
        /* [in] */ const String& scheme,
        /* [in] */ IPart* authority,
        /* [in] */ IPathPart* path,
        /* [in] */ IPart* query,
        /* [in] */ IPart* fragment);

private:
    // TODO: Add your private member variables here.
};

#endif // __CHIERARCHICALURI_H__
