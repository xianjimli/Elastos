
#include "cmdef.h"
#include "URLStreamHandler.h"
#include "CURL.h"
#include "InetAddress.h"
#include <Com.Kortide.Platform.h>
#include <StringBuffer.h>
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

// {99E0A5EB-DDD7-4ab4-820E-7B926C0A7036}
extern "C" const InterfaceID EIID_URLStreamHandler =
    { 0x99e0a5eb, 0xddd7, 0x4ab4, { 0x82, 0xe, 0x7b, 0x92, 0x6c, 0xa, 0x70, 0x36 } };

ECode URLStreamHandler::OpenConnectionEx(
    /* [in] */ IURL* u,
    /* [in] */ IProxy* proxy,
    /* [out] */ IURLConnection** urlConnection)
{
//    throw new UnsupportedOperationException();
    return E_NOT_IMPLEMENTED;
}

ECode URLStreamHandler::ParseURL(
    /* [in] */ IURL* u,
    /* [in] */ String str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // For compatibility, refer to Harmony-2941
    String temp = str.Substring(start);
    if (temp.StartWith("//")
            && str.IndexOf('/', start + 2) == -1
            && end <= Math::INT32_MIN_VALUE + 1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;//todo: the error code should be changed
//        throw new StringIndexOutOfBoundsException(end - 2 - start);
    }

    if (end < start) {
        URLStreamHandler* handler =
                (URLStreamHandler*)((CURL*)u)->mStrmHandler->Probe(EIID_URLStreamHandler);
        if (this != handler) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;//todo: the error code should be changed
//            throw new SecurityException();
        }
        return NOERROR;
    }

    String parseString = String("");
    if (start < end) {
        parseString = str.Substring(start, end);
    }
    end -= start;
    Int32 fileIdx = 0;

    // Default is to use info from context
    String host;
    u->GetHost(&host);
    Int32 port;
    u->GetPort(&port);
    String ref, file, query, authority, userInfo;
    u->GetRef(&ref);
    u->GetPath(&file);
    u->GetQuery(&query);
    u->GetAuthority(&authority);
    u->GetUserInfo(&userInfo);

    Int32 refIdx = parseString.IndexOf('#', 0);
    if (parseString.StartWith("//")) {
        Int32 hostIdx = 2, portIdx = -1;
        port = -1;
        fileIdx = parseString.IndexOf('/', hostIdx);
        Int32 questionMarkIndex = parseString.IndexOf('?', hostIdx);
        if ((questionMarkIndex != -1)
                && ((fileIdx == -1) || (fileIdx > questionMarkIndex))) {
            fileIdx = questionMarkIndex;
        }
        if (fileIdx == -1) {
            fileIdx = end;
            // Use default
            file = String("");
        }
        Int32 hostEnd = fileIdx;
        if (refIdx != -1 && refIdx < fileIdx) {
            hostEnd = refIdx;
        }
        Int32 userIdx = parseString.LastIndexOf('@', hostEnd);
        authority = parseString.Substring(hostIdx, hostEnd);
        if (userIdx > -1) {
            userInfo = parseString.Substring(hostIdx, userIdx);
            hostIdx = userIdx + 1;
        }

        portIdx = parseString.IndexOf(':', userIdx == -1 ? hostIdx
                : userIdx);
        Int32 endOfIPv6Addr = parseString.IndexOf(']');
        // if there are square braces, ie. IPv6 address, use last ':'
        if (endOfIPv6Addr != -1) {
//            try {
            if (parseString.GetLength() > (UInt32)(endOfIPv6Addr + 1)) {
                Char32 c = parseString[endOfIPv6Addr + 1];
                if (c == ':') {
                    portIdx = endOfIPv6Addr + 1;
                }
                else {
                    portIdx = -1;
                }
            }
            else {
                portIdx = -1;
            }
//            } catch (Exception e) {
                // Ignored
//            }
        }

        if (portIdx == -1 || portIdx > fileIdx) {
            host = parseString.Substring(hostIdx, hostEnd);
        }
        else {
            host = parseString.Substring(hostIdx, portIdx);
            String portString = parseString.Substring(portIdx + 1, hostEnd);
            if (portString.GetLength() == 0) {
                port = -1;
            }
            else {
                port = portString.ToInt32();
            }
        }
    }

    if (refIdx > -1) {
        ref = parseString.Substring(refIdx + 1, end);
    }
    Int32 fileEnd = (refIdx == -1 ? end : refIdx);

    Int32 queryIdx = parseString.LastIndexOf('?', fileEnd);
    Boolean canonicalize = FALSE;
    if (queryIdx > -1) {
        query = parseString.Substring(queryIdx + 1, fileEnd);
        if (queryIdx == 0 && file != NULL) {
            if (file.IsEmpty()) {
                file = "/";
            }
            else if (file.StartWith("/")) {
                canonicalize = TRUE;
            }
            Int32 last = file.LastIndexOf('/') + 1;
            file = file.Substring(0, last);
        }
        fileEnd = queryIdx;
    } //else
    // Don't inherit query unless only the ref is changed
    if (refIdx != 0) {
        query = NULL;
    }

    if (fileIdx > -1) {
        if (fileIdx < end && parseString[fileIdx] == '/') {
            file = parseString.Substring(fileIdx, fileEnd);
        }
        else if (fileEnd > fileIdx) {
            if (file == NULL) {
                file = String("");
            }
            else if (file.IsEmpty()) {
                file = "/";
            }
            else if (file.StartWith("/")) {
                canonicalize = TRUE;
            }
            Int32 last = file.LastIndexOf('/') + 1;
            if (last == 0) {
                file = parseString.Substring(fileIdx, fileEnd);
            }
            else {
                file = file.Substring(0, last)
                        + parseString.Substring(fileIdx, fileEnd);
            }
        }
    }
    if (file == NULL) {
        file = String("");
    }

    if (host == NULL) {
        host = String("");
    }

    if (canonicalize) {
        // modify file if there's any relative referencing
//        file = URLUtil.canonicalizePath(file);
    }

    String protocol;
    u->GetProtocol(&protocol);
    return SetURLEx(u, protocol, host, port, authority, userInfo, file,
            query, ref);
}

ECode URLStreamHandler::SetURL(
    /* [in] */ IURL* u,
    /* [in] */ String protocol,
    /* [in] */ String host,
    /* [in] */ Int32 port,
    /* [in] */ String file,
    /* [in] */ String ref)
{
    URLStreamHandler* handler =
            (URLStreamHandler*)((CURL*)u)->mStrmHandler->Probe(EIID_URLStreamHandler);
    if (this != handler) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;//todo: the error code should be changed
//            throw new SecurityException();
    }
    ((CURL*)u)->Set(protocol, host, port, file, ref);

    return NOERROR;
}

ECode URLStreamHandler::SetURLEx(
    /* [in] */ IURL* u,
    /* [in] */ String protocol,
    /* [in] */ String host,
    /* [in] */ Int32 port,
    /* [in] */ String authority,
    /* [in] */ String userInfo,
    /* [in] */ String file,
    /* [in] */ String query,
    /* [in] */ String ref)
{
    URLStreamHandler* handler =
            (URLStreamHandler*)((CURL*)u)->mStrmHandler->Probe(EIID_URLStreamHandler);
    if (this != handler) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;//todo: the error code should be changed
//            throw new SecurityException();
    }
    ((CURL*)u)->Set(protocol, host, port, file, ref);

    return NOERROR;
}

ECode URLStreamHandler::ToExternalForm(
    /* [in] */ IURL* url,
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s);

    StringBuffer answer;
    String protocol;
    url->GetProtocol(&protocol);
    answer += protocol;
    answer += ':';
    String authority;
    url->GetAuthority(&authority);
    if (!authority.IsNull() && authority.GetLength() > 0) {
        answer += "//";
        answer += authority;
    }

    String file, ref;
    url->GetFile(&file);
    url->GetRef(&ref);
    if (!file.IsNull()) {
        answer += file;
    }
    if (!ref.IsNull()) {
        answer += '#';
        answer += ref;
    }
    *s = String(answer);

    return NOERROR;
}

ECode URLStreamHandler::Equals(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isEquals)
{
    VALIDATE_NOT_NULL(isEquals);

    Boolean isSame;
    SameFile(url1, url2, &isSame);
    if (!isSame) {
        *isEquals = FALSE;
        return NOERROR;
    }
    String ref1, ref2, query1, query2;
    url1->GetRef(&ref1);
    url2->GetRef(&ref2);
    url1->GetQuery(&query1);
    url2->GetQuery(&query2);
    *isEquals = ref1.Equals(ref2)
            && query1.Equals(query2);

    return NOERROR;
}

ECode URLStreamHandler::GetDefaultPort(
    /* [out] */ Int32* portNum)
{
    VALIDATE_NOT_NULL(portNum);
    *portNum = -1;
    return NOERROR;
}

ECode URLStreamHandler::GetHostAddress(
    /* [in] */ IURL* url,
    /* [out] */ IInetAddress** hostAddress)
{
    VALIDATE_NOT_NULL(hostAddress);
//    try {
    String host;
    url->GetHost(&host);
    if (host.IsNull() || host.GetLength() == 0) {
        *hostAddress = NULL;
        return NOERROR;
    }
    return InetAddress::GetByName(host, hostAddress);
//    } catch (UnknownHostException e) {
//        return null;
//    }
}

ECode URLStreamHandler::HashCode(
    /* [in] */ IURL* url,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);

    String s;
    ToExternalForm(url, &s);
    *code = s.GetHashCode();

    return NOERROR;
}

ECode URLStreamHandler::HostsEqual(
    /* [in] */ IURL* a,
    /* [in] */ IURL* b,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);

    /*
     * URLs with the same case-insensitive host name have equal hosts
     */
    if (!GetHost(a).IsNull() && GetHost(a).EqualsIgnoreCase(GetHost(a))) {
        *isEqual = TRUE;
        return NOERROR;
    }

    /*
     * Call out to DNS to resolve the host addresses. If this succeeds for
     * both addresses and both addresses yield the same InetAddress, report
     * equality.
     *
     * Although it's consistent with historical behavior of the RI, this
     * approach is fundamentally broken. In particular, acting upon this
     * result is bogus because a single server may serve content for many
     * unrelated host names.
     */
    AutoPtr<IInetAddress> aResolved, bResolved;
    GetHostAddress(a, (IInetAddress**)&aResolved);
    GetHostAddress(b, (IInetAddress**)&bResolved);
    *isEqual = aResolved != NULL && aResolved == bResolved;
//    return aResolved != null && aResolved.equals(getHostAddress(b));

    return NOERROR;
}

ECode URLStreamHandler::SameFile(
    /* [in] */ IURL* url1,
    /* [in] */ IURL* url2,
    /* [out] */ Boolean* isSame)
{
    VALIDATE_NOT_NULL(isSame);

    String protocol1, protocol2, file1, file2;
    url1->GetProtocol(&protocol1);
    url2->GetProtocol(&protocol2);
    url1->GetFile(&file1);
    url2->GetFile(&file2);

    Boolean isEqual;
    HostsEqual(url1, url2, &isEqual);

    Int32 port1, port2;
    url1->GetEffectivePort(&port1);
    url2->GetEffectivePort(&port2);

    *isSame = protocol1.Equals(protocol2)
            && file1.Equals(file2)
            && isEqual
            && port1 == port2;

    // return Objects.equal(url1.getProtocol(), url2.getProtocol())
    //         && Objects.equal(url1.getFile(), url2.getFile())
    //         && hostsEqual(url1, url2)
    //         && url1.getEffectivePort() == url2.getEffectivePort();

    return NOERROR;
}

String URLStreamHandler::GetHost(
    /* [in] */ IURL* url)
{
    String host;
    url->GetHost(&host);
    String protocol;
    url->GetProtocol(&protocol);
    if (String("file").Equals(protocol) && host.IsEmpty()) {
        host = String("localhost");
    }
    return host;
}
