
#include "net/CStringUri.h"
#include <StringBuffer.h>
#include <elastos/etl_hash_fun.h>
#include "net/CUriBuilder.h"

using namespace Elastos::Core;

ECode StringUri::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ IUri** result)
{
    String str;
    FAIL_RETURN(parcel->ReadString(&str));

    return CStringUri::New(str, result);
}

ECode StringUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isHierarchical;
    FAIL_RETURN(IsHierarchical(&isHierarchical));

    if (isHierarchical) {
        String str;
        FAIL_RETURN(GetScheme(&str));

        AutoPtr<IPart> part1;
        FAIL_RETURN(GetAuthorityPart((IPart**)&part1));

        AutoPtr<IPathPart> pathPart1;
        FAIL_RETURN(GetPathPart((IPathPart**)&pathPart1));

        AutoPtr<IPart> part2;
        FAIL_RETURN(GetQueryPart((IPart**)&part2));

        AutoPtr<IPart> part3;
        FAIL_RETURN(GetFragmentPart((IPart**)&part3));

        FAIL_RETURN(CUriBuilder::New(result));

        FAIL_RETURN((*result)->Scheme(str));
        FAIL_RETURN((*result)->Authority(part1));
        FAIL_RETURN((*result)->Path(pathPart1));
        FAIL_RETURN((*result)->Query(part2));
        FAIL_RETURN((*result)->Fragment(part3));

        return NOERROR;
    } else {
        String str;
        FAIL_RETURN(GetScheme(&str));

        AutoPtr<IPart> part1;
        FAIL_RETURN(GetSsp((IPart**)&part1));

        AutoPtr<IPart> part2;
        FAIL_RETURN(GetFragmentPart((IPart**)&part2));

        FAIL_RETURN(CUriBuilder::New(result));

        FAIL_RETURN((*result)->Scheme(str));
        FAIL_RETURN((*result)->OpaquePart(part1));
        FAIL_RETURN((*result)->Fragment(part2));

        return NOERROR;
    }
}


ECode CStringUri::constructor(
    /* [in] */ const String& uriString)
{
    if (uriString.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }

    mUriString = uriString;

    return NOERROR;
}

ECode CStringUri::constructor(
    /* [in] */ Handle32 uri)
{
    return NOERROR;
}

ECode CStringUri::constructor()
{
    return NOERROR;
}

ECode CStringUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    return StringUri::IsHierarchical(isHierarchical);
}

ECode CStringUri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    return StringUri::IsOpaque(isOpaque);
}

ECode CStringUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    return StringUri::IsRelative(isRelative);
}

ECode CStringUri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    return StringUri::IsAbsolute(isAbsolute);
}

ECode CStringUri::GetScheme(
    /* [out] */ String* scheme)
{
    return StringUri::GetScheme(scheme);
}

ECode CStringUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    return StringUri::GetSchemeSpecificPart(ssp);
}

ECode CStringUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    return StringUri::GetEncodedSchemeSpecificPart(essp);
}

ECode CStringUri::GetAuthority(
    /* [out] */ String* authority)
{
    return StringUri::GetAuthority(authority);
}

ECode CStringUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    return StringUri::GetEncodedAuthority(authority);
}

ECode CStringUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    return StringUri::GetUserInfo(userInfo);
}

ECode CStringUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    return StringUri::GetEncodedUserInfo(userInfo);
}

ECode CStringUri::GetHost(
    /* [out] */ String* host)
{
    return StringUri::GetHost(host);
}

ECode CStringUri::GetPort(
    /* [out] */ Int32* port)
{
    return StringUri::GetPort(port);
}

ECode CStringUri::GetPath(
    /* [out] */ String* path)
{
    return StringUri::GetPath(path);
}

ECode CStringUri::GetEncodedPath(
    /* [out] */ String* path)
{
    return StringUri::GetEncodedPath(path);
}

ECode CStringUri::GetQuery(
    /* [out] */ String* query)
{
    return StringUri::GetQuery(query);
}

ECode CStringUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    return StringUri::GetEncodedQuery(query);
}

ECode CStringUri::GetFragment(
    /* [out] */ String* fragment)
{
    return StringUri::GetFragment(fragment);
}

ECode CStringUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    return StringUri::GetEncodedFragment(fragment);
}

ECode CStringUri::GetPathSegments(
    /* [out, callee] */ ArrayOf<String>** pathSegments)
{
    return StringUri::GetPathSegments(pathSegments);
}

ECode CStringUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    return StringUri::GetLastPathSegment(pathSegment);
}

ECode CStringUri::Equals(
    /* [in] */ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    return StringUri::Equals(other, isEqual);
}

ECode CStringUri::HashCode(
    /* [out] */ Int32* hashCode)
{
    return StringUri::HashCode(hashCode);
}

ECode CStringUri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    return StringUri::CompareTo(other, result);
}

ECode CStringUri::ToString(
    /* [out] */ String* result)
{
    return StringUri::ToString(result);
}

ECode CStringUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    return StringUri::BuildUpon(result);
}

ECode CStringUri::GetDescription(
    /* [out] */ String* uriString)
{
    *uriString = mUriString;

    return NOERROR;
}

ECode CStringUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return StringUri::HashCode(hashCode);
}

ECode CStringUri::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 typeId;
    FAIL_RETURN(source->ReadInt32(&typeId));
    FAIL_RETURN(source->ReadString(&mUriString));

    return NOERROR;
}

ECode CStringUri::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    StringUri::WriteToParcel(dest, 0);

    return NOERROR;
}
