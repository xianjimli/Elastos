
#include "net/CHierarchicalUri.h"
#include "net/CUriBuilder.h"

ECode HierarchicalUri::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ IUri** result)
{
    String str;
    FAIL_RETURN(parcel->ReadString(&str));

    AutoPtr<IPart> part1;
    FAIL_RETURN(Part::ReadFrom(parcel, (IPart**)&part1));

    AutoPtr<IPathPart> pathPart1;
    FAIL_RETURN(PathPart::ReadFrom(parcel, (IPathPart**)&pathPart1));

    AutoPtr<IPart> part2;
    FAIL_RETURN(Part::ReadFrom(parcel, (IPart**)&part2));

    AutoPtr<IPart> part3;
    FAIL_RETURN(Part::ReadFrom(parcel, (IPart**)&part3));

    return CHierarchicalUri::New(
        str,
        part1,
        pathPart1,
        part2,
        part3,
        result);
}

ECode HierarchicalUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CUriBuilder::New(result));

    FAIL_RETURN((*result)->Scheme(mScheme));
    FAIL_RETURN((*result)->Authority(mAuthority));
    FAIL_RETURN((*result)->Path(mPath));
    FAIL_RETURN((*result)->Query(mQuery));
    FAIL_RETURN((*result)->Fragment(mFragment));

    return NOERROR;
}

ECode CHierarchicalUri::GetDescription(
    /* [out] */ String* uriString)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CHierarchicalUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return HierarchicalUri::HashCode(hashCode);
}

ECode CHierarchicalUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    return HierarchicalUri::IsHierarchical(isHierarchical);
}

ECode CHierarchicalUri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    return HierarchicalUri::IsOpaque(isOpaque);
}

ECode CHierarchicalUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    return HierarchicalUri::IsRelative(isRelative);
}

ECode CHierarchicalUri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    return HierarchicalUri::IsAbsolute(isAbsolute);
}

ECode CHierarchicalUri::GetScheme(
    /* [out] */ String* scheme)
{
    return HierarchicalUri::GetScheme(scheme);
}

ECode CHierarchicalUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    return HierarchicalUri::GetSchemeSpecificPart(ssp);
}

ECode CHierarchicalUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    return HierarchicalUri::GetEncodedSchemeSpecificPart(essp);
}

ECode CHierarchicalUri::GetAuthority(
    /* [out] */ String* authority)
{
    return HierarchicalUri::GetAuthority(authority);
}

ECode CHierarchicalUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    return HierarchicalUri::GetEncodedAuthority(authority);
}

ECode CHierarchicalUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    return HierarchicalUri::GetUserInfo(userInfo);
}

ECode CHierarchicalUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    return HierarchicalUri::GetEncodedUserInfo(userInfo);
}

ECode CHierarchicalUri::GetHost(
    /* [out] */ String* host)
{
    return HierarchicalUri::GetHost(host);
}

ECode CHierarchicalUri::GetPort(
    /* [out] */ Int32* port)
{
    return HierarchicalUri::GetPort(port);
}

ECode CHierarchicalUri::GetPath(
    /* [out] */ String* path)
{
    return HierarchicalUri::GetPath(path);
}

ECode CHierarchicalUri::GetEncodedPath(
    /* [out] */ String* path)
{
    return HierarchicalUri::GetEncodedPath(path);
}

ECode CHierarchicalUri::GetQuery(
    /* [out] */ String* query)
{
    return HierarchicalUri::GetQuery(query);
}

ECode CHierarchicalUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    return HierarchicalUri::GetEncodedQuery(query);
}

ECode CHierarchicalUri::GetFragment(
    /* [out] */ String* fragment)
{
    return HierarchicalUri::GetFragment(fragment);
}

ECode CHierarchicalUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    return HierarchicalUri::GetEncodedFragment(fragment);
}

ECode CHierarchicalUri::GetPathSegments(
    /* [out, callee] */ ArrayOf<String>** pathSegments)
{
    return HierarchicalUri::GetPathSegments(pathSegments);
}

ECode CHierarchicalUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    return HierarchicalUri::GetLastPathSegment(pathSegment);
}

ECode CHierarchicalUri::Equals(
    /* [in] */ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    return HierarchicalUri::Equals(other, isEqual);
}

ECode CHierarchicalUri::HashCode(
    /* [out] */ Int32* hashCode)
{
    return HierarchicalUri::HashCode(hashCode);
}

ECode CHierarchicalUri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    return HierarchicalUri::CompareTo(other, result);
}

ECode CHierarchicalUri::ToString(
    /* [out] */ String* result)
{
    return HierarchicalUri::ToString(result);
}

ECode CHierarchicalUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    return HierarchicalUri::BuildUpon(result);
}

ECode CHierarchicalUri::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 typeId;
    FAIL_RETURN(source->ReadInt32(&typeId));
    FAIL_RETURN(source->ReadString(&mScheme));
    AutoPtr<IPart> part1;
    FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part1));
    mAuthority = part1;
    AutoPtr<IPathPart> pathPart1;
    FAIL_RETURN(PathPart::ReadFrom(source, (IPathPart**)&pathPart1));
    mPath = pathPart1;
    AutoPtr<IPart> part2;
    FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part2));
    mQuery = part2;
    AutoPtr<IPart> part3;
    FAIL_RETURN(Part::ReadFrom(source, (IPart**)&part3));
    mFragment = part2;

    return NOERROR;
}

ECode CHierarchicalUri::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    HierarchicalUri::WriteToParcel(dest, 0);

    return NOERROR;
}

ECode CHierarchicalUri::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ IPart* authority,
    /* [in] */ IPathPart* path,
    /* [in] */ IPart* query,
    /* [in] */ IPart* fragment)
{
    return HierarchicalUri::Init(scheme, authority, path, query, fragment);
}
