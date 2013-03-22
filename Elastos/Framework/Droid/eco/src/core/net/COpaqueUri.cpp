
#include "net/CPart.h"
#include "net/CPathPart.h"
#include "net/COpaqueUri.h"
#include "net/CUriBuilder.h"

ECode OpaqueUri::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result);

    String str1;
    FAIL_RETURN(parcel->ReadString(&str1));

    AutoPtr<IPart> part1;
    FAIL_RETURN(Part::ReadFrom(parcel, (IPart**)&part1));

    AutoPtr<IPart> part2;
    FAIL_RETURN(Part::ReadFrom(parcel, (IPart**)&part2));

    return COpaqueUri::New(str1, part1, part2, result);
}

ECode OpaqueUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CUriBuilder::New(result));

    FAIL_RETURN((*result)->Scheme(mScheme));
    FAIL_RETURN((*result)->OpaquePart(mSsp));
    FAIL_RETURN((*result)->Fragment(mFragment));

    return NOERROR;
}

ECode COpaqueUri::GetDescription(
    /* [out] */ String* uriString)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode COpaqueUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return OpaqueUri::HashCode(hashCode);
}

ECode COpaqueUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    return OpaqueUri::IsHierarchical(isHierarchical);
}

ECode COpaqueUri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    return OpaqueUri::IsOpaque(isOpaque);
}

ECode COpaqueUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    return OpaqueUri::IsRelative(isRelative);
}

ECode COpaqueUri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    return OpaqueUri::IsAbsolute(isAbsolute);
}

ECode COpaqueUri::GetScheme(
    /* [out] */ String* scheme)
{
    return OpaqueUri::GetScheme(scheme);
}

ECode COpaqueUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    return OpaqueUri::GetSchemeSpecificPart(ssp);
}

ECode COpaqueUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    return OpaqueUri::GetEncodedSchemeSpecificPart(essp);
}

ECode COpaqueUri::GetAuthority(
    /* [out] */ String* authority)
{
    return OpaqueUri::GetAuthority(authority);
}

ECode COpaqueUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    return OpaqueUri::GetEncodedAuthority(authority);
}

ECode COpaqueUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    return OpaqueUri::GetUserInfo(userInfo);
}

ECode COpaqueUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    return OpaqueUri::GetEncodedUserInfo(userInfo);
}

ECode COpaqueUri::GetHost(
    /* [out] */ String* host)
{
    return OpaqueUri::GetHost(host);
}

ECode COpaqueUri::GetPort(
    /* [out] */ Int32* port)
{
    return OpaqueUri::GetPort(port);
}

ECode COpaqueUri::GetPath(
    /* [out] */ String* path)
{
    return OpaqueUri::GetPath(path);
}

ECode COpaqueUri::GetEncodedPath(
    /* [out] */ String* path)
{
    return OpaqueUri::GetEncodedPath(path);
}

ECode COpaqueUri::GetQuery(
    /* [out] */ String* query)
{
    return OpaqueUri::GetQuery(query);
}

ECode COpaqueUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    return OpaqueUri::GetEncodedQuery(query);
}

ECode COpaqueUri::GetFragment(
    /* [out] */ String* fragment)
{
    return OpaqueUri::GetFragment(fragment);
}

ECode COpaqueUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    return OpaqueUri::GetEncodedFragment(fragment);
}

ECode COpaqueUri::GetPathSegments(
    /* [out, callee] */ ArrayOf<String>** pathSegments)
{
    return OpaqueUri::GetPathSegments(pathSegments);
}

ECode COpaqueUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    return OpaqueUri::GetLastPathSegment(pathSegment);
}

ECode COpaqueUri::Equals(
    /* [in] */ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    return OpaqueUri::Equals(other, isEqual);
}

ECode COpaqueUri::HashCode(
    /* [out] */ Int32* hashCode)
{
    return OpaqueUri::HashCode(hashCode);
}

ECode COpaqueUri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    return OpaqueUri::CompareTo(other, result);
}

ECode COpaqueUri::ToString(
    /* [out] */ String* result)
{
    return OpaqueUri::ToString(result);
}

ECode COpaqueUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    return OpaqueUri::BuildUpon(result);
}

ECode COpaqueUri::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 typeId;
    FAIL_RETURN(source->ReadInt32(&typeId));
    FAIL_RETURN(source->ReadString(&mScheme));
    AutoPtr<IPart> part1;
    FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part1));
    mSsp = part1;
    AutoPtr<IPart> part2;
    FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part2));
    mFragment = part2;

    return NOERROR;
}

ECode COpaqueUri::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    OpaqueUri::WriteToParcel(dest, 0);

    return NOERROR;
}

ECode COpaqueUri::constructor(
    /* [in] */ String scheme,
    /* [in] */ IPart* ssp,
    /* [in] */ IPart* fragment)
{
    return OpaqueUri::Init(scheme, ssp, fragment);
}
