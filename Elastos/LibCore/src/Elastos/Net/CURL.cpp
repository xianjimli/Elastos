
#include "cmdef.h"
#include "CURL.h"
#include "CURI.h"

const Int64 CURL::mSerialVersionUID = -7627629688361524110L;
HashMap<String, AutoPtr<IURLStreamHandler> > CURL::mStreamHandlers;
AutoPtr<IURLStreamHandlerFactory> CURL::mStreamHandlerFactory;
Mutex* CURL::mLock;

CURL::CURL()
    : mHashCode(0)
    , mProtocol(String(NULL))
    , mPort(-1)
    , mUserInfo(String(NULL))
    , mPath(String(NULL))
    , mQuery(String(NULL))
    , mRef(String(NULL))
{
}

ECode CURL::SetURLStreamHandlerFactory(
    /* [in] */ IURLStreamHandlerFactory* streamFactory)
{
    Mutex::Autolock lock(mLock);

    if (mStreamHandlerFactory != NULL) {
        return NOERROR;
//        throw new Error("Factory already set");
    }
//    SecurityManager sm = System.getSecurityManager();
//    if (sm != null) {
//        sm.checkSetFactory();
//    }
    mStreamHandlers.Clear();
    mStreamHandlerFactory = streamFactory;

    return NOERROR;
}

void CURL::FixURL(
    /* [in] */ Boolean fixHost)
{
    Int32 index;
    if (!mHost.IsNull() && mHost.GetLength() > 0) {
        mAuthority = mHost;
        if (mPort != -1) {
            mAuthority = mAuthority + String(":") + mPort;
        }
    }
    if (fixHost) {
        if (!mHost.IsNull() && (index = mHost.LastIndexOf('@')) > -1) {
            mUserInfo = mHost.Substring(0, index);
            mHost = mHost.Substring(index + 1);
        }
        else {
            mUserInfo = String(NULL);
        }
    }
    if (!mFile.IsNull() && (index = mFile.IndexOf('?')) > -1) {
        mQuery = mFile.Substring(index + 1);
        mPath = mFile.Substring(0, index);
    }
    else {
        mQuery = String(NULL);
        mPath = mFile;
    }
}

void CURL::Set(
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file,
    /* [in] */ const String& ref)
{
    if (mProtocol.IsNull()) {
        mProtocol = protocol;
    }
    mHost = host;
    mFile = file;
    mPort = port;
    mRef = ref;
    mHashCode = 0;
    FixURL(TRUE);
}

ECode CURL::SameFile(
    /* [in] */ IURL* otherURL,
    /* [out] */ Boolean* isSame)
{
    return mStrmHandler->SameFile((IURL*)this, otherURL, isSame);
}

Int32 CURL::HashCode()
{
    if (mHashCode == 0) {
        mStrmHandler->HashCode((IURL*)this, &mHashCode);
    }
    return mHashCode;
}

void CURL::SetupStreamHandler()
{
    // Check for a cached (previously looked up) handler for
    // the requested protocol.
    HashMap<String, AutoPtr<IURLStreamHandler> >::Iterator it =
        mStreamHandlers.Find(mProtocol);
    if (it != mStreamHandlers.End()) {
        mStrmHandler = it->mSecond;
        return;
    }

    // If there is a stream handler factory, then attempt to
    // use it to create the handler.
    if (mStreamHandlerFactory != NULL) {
        mStreamHandlerFactory->CreateURLStreamHandler(mProtocol, (IURLStreamHandler**)&mStrmHandler);
        if (mStrmHandler != NULL) {
            mStreamHandlers[mProtocol] = mStrmHandler;
            return;
        }
    }

    // Check if there is a list of packages which can provide handlers.
    // If so, then walk this list looking for an applicable one.
//    String packageList = AccessController
//            .doPrivileged(new PriviAction<String>(
//                    "java.protocol.handler.pkgs"));
//    if (packageList != null) {
//        StringTokenizer st = new StringTokenizer(packageList, "|");
//        while (st.hasMoreTokens()) {
//            String className = st.nextToken() + "." + protocol + ".Handler";

//            try {
//                strmHandler = (URLStreamHandler) Class.forName(className,
//                        true, ClassLoader.getSystemClassLoader())
//                        .newInstance();
//                if (strmHandler != null) {
//                    streamHandlers.put(protocol, strmHandler);
//                }
//                return;
//            } catch (IllegalAccessException e) {
//            } catch (InstantiationException e) {
//            } catch (ClassNotFoundException e) {
//            }
//        }
//    }

    // No one else has provided a handler, so try our internal one.

//    String className = "org.apache.harmony.luni.internal.net.www.protocol." + mProtocol
//            + ".Handler";
//    try {
//        mStrmHandler = (URLStreamHandler) Class.forName(className)
//                .newInstance();
//    } catch (IllegalAccessException e) {
//    } catch (InstantiationException e) {
//    } catch (ClassNotFoundException e) {
//    }
    if (mStrmHandler != NULL) {
        mStreamHandlers[mProtocol] = mStrmHandler;
    }

}

ECode CURL::GetContent(
    /* [out] */ IInterface** content)
{
    AutoPtr<IURLConnection> connection;
    FAIL_RETURN(OpenConnection((IURLConnection**)&connection));

    return connection->GetContent(content);
}

//        public final Object getContent(Class[] types)

ECode CURL::OpenStream(
    /* [out] */ IInputStream** is)
{
    AutoPtr<IURLConnection> connection;
    FAIL_RETURN(OpenConnection((IURLConnection**)&connection));

    return connection->GetInputStream(is);
}

ECode CURL::OpenConnection(
    /* [out] */ IURLConnection** connection)
{
    return mStrmHandler->OpenConnection((IURL*)this, connection);
}

ECode CURL::OpenConnectionEx(
    /* [in] */ IProxy* proxy,
    /* [out] */ IURLConnection** connection)
{
    if (proxy == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("proxy == null");
    }

//    SecurityManager sm = System.getSecurityManager();
//    if (sm != NULL) {
//        ProxyType type;
//        proxy->GetType(&type);
//        if (type != ProxyType_DIRECT) {
//            AutoPtr<IInetSocketAddress> pAddress;
//             AutoPtr<ISocketAddress> addr;
//             proxy->GetAddress((ISocketAddress**)&addr);
//             if (addr != NULL && addr->Probe(EIID_IInetSocketAddress) != NULL) {
//                 pAddress = (IInetSocketAddress*)addr->Probe(EIID_IInetSocketAddress);
//             }

//             String pHostName;
//             Boolean isUnresolved;
//             pAddress->IsUnresolved(&isUnresolved);
//             if (isUnresolved) {
//                 pAddress->GetHostName(&pHostName);
//             }
//             else {
//                 AutoPtr<IInetAddress> netAddr;
//                 pAddress->GetAddress((IInetAddress**)&netAddr);
//                 if (netAddr) {
//                     netAddr->GetHostAddress(&pHostName);
//                 }
//             }

//             Int32 port;
//             pAddress->GetPort(&port);
//             sm->CheckConnect(pHostName, port);
//         }
//    }

    return mStrmHandler->OpenConnectionEx((IURL*)this, proxy, connection);
}

ECode CURL::ToURI(
    /* [out] */ IURI** uri)
{
    String url;
    ToExternalForm(&url);

    return CURI::New(url, uri);
}

ECode CURL::ToExternalForm(
    /* [out] */ String* url)
{
    if (mStrmHandler == NULL) {
        *url = String("unknown protocol(") + mProtocol + String(")://") + mHost + mFile;
        return NOERROR;
    }
    return mStrmHandler->ToExternalForm((IURL*)this, url);
}

ECode CURL::GetFile(
    /* [out] */ String* fileName)
{
    VALIDATE_NOT_NULL(fileName);
    *fileName = mFile;

    return NOERROR;
}

ECode CURL::GetHost(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    *hostName = mHost;

    return NOERROR;
}

ECode CURL::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mPort;

    return NOERROR;
}

ECode CURL::GetEffectivePort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port =  CURI::GetEffectivePortEx(mProtocol, mPort);

    return NOERROR;
}

ECode CURL::GetProtocol(
    /* [out] */ String* protocol)
{
    VALIDATE_NOT_NULL(protocol);
    *protocol = mProtocol;

    return NOERROR;
}

ECode CURL::GetRef(
    /* [out] */ String* ref)
{
    VALIDATE_NOT_NULL(ref);
    *ref = mRef;

    return NOERROR;
}

ECode CURL::GetQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = mQuery;

    return NOERROR;
}

ECode CURL::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mPath;

    return NOERROR;
}

ECode CURL::GetUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    *userInfo = mUserInfo;

    return NOERROR;
}

ECode CURL::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority;

    return NOERROR;
}

void CURL::SetEx(
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& authority,
    /* [in] */ const String& userInfo,
    /* [in] */ const String& path,
    /* [in] */ const String& query,
    /* [in] */ const String& ref)
{
    String filePart = path;
    if (!query.IsNull() && !query.IsEmpty()) {
        if (!filePart.IsNull()) {
            filePart = filePart + String("?") + query;
        }
        else {
            filePart = String("?") + query;
        }
    }
    Set(protocol, host, port, filePart, ref);
    mAuthority = authority;
    mUserInfo = userInfo;
    mPath = path;
    mQuery = query;
}

ECode CURL::GetDefaultPort(
    /* [out] */ Int32* port)
{
    return mStrmHandler->GetDefaultPort(port);
}

ECode CURL::constructor(
    /* [in] */ const String& spec)
{
    return constructor(NULL, spec, NULL);
}

ECode CURL::constructor(
    /* [in] */ IURL* context,
    /* [in] */ const String& spec)
{
    return constructor(context, spec, NULL);
}

ECode CURL::constructor(
    /* [in] */ IURL* context,
    /* [in] */ const String& spec,
    /* [in] */ IURLStreamHandler* handler)
{
    if (handler != NULL) {
//        SecurityManager sm = System.getSecurityManager();
//        if (sm != null) {
//            sm.checkPermission(specifyStreamHandlerPermission);
//        }
        mStrmHandler = handler;
    }

    if (spec == NULL) {
        return E_MALFORMED_URL_EXCEPTION;
//        throw new MalformedURLException();
    }

    //todo: the spec has been changed
    String temp = spec.Trim();

    // The spec includes a protocol if it includes a colon character
    // before the first occurrence of a slash character. Note that,
    // "protocol" is the field which holds this URLs protocol.
//    try {
    Int32 index = temp.IndexOf(':');
//    } catch (NullPointerException e) {
//        throw new MalformedURLException(e.toString());
//    }
    Int32 startIPv6Addr = temp.IndexOf('[');
    if (index >= 0) {
        if ((startIPv6Addr == -1) || (index < startIPv6Addr)) {
            mProtocol = temp.Substring(0, index);
            // According to RFC 2396 scheme part should match
            // the following expression:
            // alpha *( alpha | digit | "+" | "-" | "." )
            Char32 c = mProtocol[0];
            Boolean valid = ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
            for (Int32 i = 1; valid && ((UInt32)i < mProtocol.GetLength()); i++) {
                c = mProtocol[i];
                valid = ('a' <= c && c <= 'z') ||
                        ('A' <= c && c <= 'Z') ||
                        ('0' <= c && c <= '9') ||
                        (c == '+') ||
                        (c == '-') ||
                        (c == '.');
            }
            if (!valid) {
                mProtocol = String(NULL);
                index = -1;
            }
            else {
                // Ignore case in protocol names.
                // Scheme is defined by ASCII characters.
//                protocol = Util.toASCIILowerCase(protocol);
            }
        }
    }

    if (!mProtocol.IsNull()) {
        // If the context was specified, and it had the same protocol
        // as the spec, then fill in the receiver's slots from the values
        // in the context but still allow them to be over-ridden later
        // by the values in the spec.
        String s;
        context->GetProtocol(&s);
        if (context != NULL && mProtocol.Equals(s)) {
            String cPath;
            context->GetPath(&cPath);
            if (!cPath.IsNull() && cPath.StartWith("/")) {
                String hostName, authority, userInfo, query;
                Int32 port;
                context->GetHost(&hostName);
                context->GetPort(&port);
                context->GetAuthority(&authority);
                context->GetUserInfo(&userInfo);
                context->GetQuery(&query);
                SetEx(mProtocol, hostName, port, authority,
                        userInfo, cPath, query, String(NULL));
            }
            if (mStrmHandler == NULL) {
                mStrmHandler = ((CURL*)context)->mStrmHandler;
            }
        }
    }
    else {
        // If the spec did not include a protocol, then the context
        // *must* be specified. Fill in the receiver's slots from the
        // values in the context, but still allow them to be over-ridden
        // by the values in the ("relative") spec.
        if (context == NULL) {
            return E_MALFORMED_URL_EXCEPTION;
//            throw new MalformedURLException("Protocol not found: " + spec);
        }
        String protocol, hostName, authority, userInfo, path, query;
        Int32 port;
        context->GetProtocol(&protocol);
        context->GetHost(&hostName);
        context->GetPort(&port);
        context->GetAuthority(&authority);
        context->GetUserInfo(&userInfo);
        context->GetPath(&path);
        context->GetQuery(&query);
        SetEx(protocol, hostName, port, authority, userInfo,
                path, query, String(NULL));
        if (mStrmHandler == NULL) {
            mStrmHandler = ((CURL*)context)->mStrmHandler;
        }
    }

    // If the stream handler has not been determined, set it
    // to the default for the specified protocol.
    if (mStrmHandler == NULL) {
        SetupStreamHandler();
        if (mStrmHandler == NULL) {
            return E_MALFORMED_URL_EXCEPTION;
//            throw new MalformedURLException("Unknown protocol: " + protocol);
        }
    }

    // Let the handler parse the URL. If the handler throws
    // any exception, throw MalformedURLException instead.
    //
    // Note: We want "index" to be the index of the start of the scheme
    // specific part of the URL. At this point, it will be either
    // -1 or the index of the colon after the protocol, so we
    // increment it to point at either character 0 or the character
    // after the colon.
//    try {
    ECode ec = mStrmHandler->ParseURL((IURL*)this, temp, ++index, temp.GetLength());
    if (FAILED(ec)) {
        return E_MALFORMED_URL_EXCEPTION;
    }
//    } catch (Exception e) {
//        throw new MalformedURLException(e.toString());
//    }

    if (mPort < -1) {
        return E_MALFORMED_URL_EXCEPTION;
//        throw new MalformedURLException("Port out of range: " + port);
    }

    return NOERROR;
}

ECode CURL::constructor(
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ const String& file)
{
    return constructor(protocol, host, -1, file, NULL);
}

ECode CURL::constructor(
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file)
{
    return constructor(protocol, host, port, file, NULL);
}

ECode CURL::constructor(
    /* [in] */ const String& protocol,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& file,
    /* [in] */ IURLStreamHandler* handler)
{
    if (port < -1) {
        return E_MALFORMED_URL_EXCEPTION;
//        throw new MalformedURLException("Port out of range: " + port);
    }

    String hostNew;
    if (!host.IsNull() && host.IndexOf(":") != -1 && host[0] != '[') {
        //todo: the host has been changed
        hostNew = String("[") + host + String("]");
    }

    if (protocol.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("Unknown protocol: null");
    }

    mProtocol = protocol;
    mHost = host;
    mPort = port;

    // Set the fields from the arguments. Handle the case where the
    // passed in "file" includes both a file and a reference part.
    Int32 index = -1;
    index = file.IndexOf("#", file.LastIndexOf("/"));
    if (index >= 0) {
        mFile = file.Substring(0, index);
        mRef = file.Substring(index + 1);
    }
    else {
        mFile = file;
    }
    FixURL(FALSE);

    // Set the stream handler for the URL either to the handler
    // argument if it was specified, or to the default for the
    // receiver's protocol if the handler was null.
    if (handler == NULL) {
        SetupStreamHandler();
        if (mStrmHandler == NULL) {
            return E_MALFORMED_URL_EXCEPTION;
//            throw new MalformedURLException("Unknown protocol: " + protocol);
        }
    }
    else {
//        SecurityManager sm = System.getSecurityManager();
//        if (sm != null) {
//            sm.checkPermission(specifyStreamHandlerPermission);
//        }
        mStrmHandler = handler;
    }

    return NOERROR;
}
