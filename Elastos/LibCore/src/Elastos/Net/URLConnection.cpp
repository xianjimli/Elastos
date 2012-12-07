
#include "cmdef.h"
#include "URLConnection.h"
#include "DefaultFileNameMap.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

extern "C" const InterfaceID EIID_URLConnection =
    { 0x6ef700a4, 0x2101, 0x4d37, { 0xbe, 0x26, 0x1f, 0xf9, 0xf6, 0x46, 0x35, 0x4f } };


Boolean URLConnection::mDefaultAllowUserInteraction = FALSE;
Boolean URLConnection::mDefaultUseCaches = TRUE;
HashMap<String, AutoPtr<IContentHandler> > URLConnection::mContentHandlers;
AutoPtr<IContentHandlerFactory> URLConnection::mContentHandlerFactory;
AutoPtr<IFileNameMap> URLConnection::mFileNameMap;

PInterface URLConnection::DefaultContentHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContentHandler) {
        return (IContentHandler*)this;
    }

    return NULL;
}

UInt32 URLConnection::DefaultContentHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 URLConnection::DefaultContentHandler::Release()
{
    return ElRefBase::Release();
}

ECode URLConnection::DefaultContentHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode URLConnection::DefaultContentHandler::GetContent(
    /* [in] */ IURLConnection* uConn,
    /* [out] */ IInterface** obj)
{
    return E_NOT_IMPLEMENTED;
}

ECode URLConnection::DefaultContentHandler::GetContentEx(
    /* [in] */ IURLConnection* uConn,
    /* [in] */ const ArrayOf<IInterface*>& types,
    /* [out] */ IInterface** obj)
{
    return E_NOT_IMPLEMENTED;
}

URLConnection::URLConnection()
    : mIfModifiedSince(0)
    , mUseCaches(mDefaultUseCaches)
    , mConnected(FALSE)
    , mDoOutput(FALSE)
    , mDoInput(TRUE)
    , mAllowUserInteraction(mDefaultAllowUserInteraction)
    , mLastModified(-1)
    , mReadTimeout(0)
    , mConnectTimeout(0)
{
    mDefaultHandler = (IContentHandler*)new DefaultContentHandler();
}

void URLConnection::Init(
    /* [in] */ IURL* url)
{
    mUrl = url;
}

URLConnection::~URLConnection()
{}

ECode URLConnection::GetAllowUserInteraction(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mAllowUserInteraction;
    return NOERROR;
}

ECode URLConnection::GetContent(
    /* [out] */ IInterface** content)
{
    VALIDATE_NOT_NULL(content);

    if (!mConnected) {
        Connect();
    }

    GetContentType(&mContentType);
    if (mContentType.IsNull()) {
        String fileName;
        mUrl->GetFile(&fileName);
        mContentType = GuessContentTypeFromName(fileName);
        if (mContentType.IsNull()) {
            AutoPtr<IInputStream> inputStream;
            GetInputStream((IInputStream**)&inputStream);
            mContentType = GuessContentTypeFromStream(inputStream);
        }
    }
    if (!mContentType.IsNull()) {
        AutoPtr<IContentHandler> handler;
        GetContentHandler(mContentType, (IContentHandler**)&handler);
        return handler->GetContent((IURLConnection*)this, content);
    }

    *content = NULL;

    return NOERROR;
}

//        public Object getContent(Class[] types);

ECode URLConnection::GetContentEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);

    return GetHeaderFieldByKey(String("Content-Encoding"), encoding);
}

ECode URLConnection::GetContentHandler(
    /* [in] */ const String& type,
    /* [out] */ IContentHandler** contentHandler)
{
    // Replace all non-alphanumeric character by '_'
//    String typeString = ParseTypeString(type.Replace('/', '.'));

    // if there's a cached content handler, use it
    HashMap<String, AutoPtr<IContentHandler> >::Iterator it = mContentHandlers.Find(type);
    if (it != mContentHandlers.End()) {
        *contentHandler = it->mSecond;
        return NOERROR;
    }

    AutoPtr<IContentHandler> cHandler;
    if (mContentHandlerFactory != NULL) {
        mContentHandlerFactory->CreateContentHandler(type, (IContentHandler**)&cHandler);
        mContentHandlers[type] = cHandler;
        *contentHandler = cHandler;
        return NOERROR;
    }

    // search through the package list for the right class for the Content
    // Type
//    String packageList = AccessController
//            .doPrivileged(new PriviAction<String>(
//                    "java.content.handler.pkgs"));
//    if (!packageList.IsNull()) {
//        StringTokenizer* st = new StringTokenizer(packageList, ".");
//        while (st->HasMoreTokens()) {
//            try {
//                Class<?> cl = Class.forName(st.nextToken() + "."
//                        + typeString, true, ClassLoader
//                        .getSystemClassLoader());
//                cHandler = cl.newInstance();
//            } catch (ClassNotFoundException e) {
//            } catch (IllegalAccessException e) {
//            } catch (InstantiationException e) {
//            }
//        }
//    }

    if (cHandler == NULL) {
//        cHandler = AccessController
//                .doPrivileged(new PrivilegedAction<Object>() {
//                    public Object run() {
//                        try {
//                            // Try looking up AWT image content handlers
//                            String className = "org.apache.harmony.awt.www.content."
//                                    + typeString;
//                            return Class.forName(className).newInstance();
//                        } catch (ClassNotFoundException e) {
//                        } catch (IllegalAccessException e) {
//                        } catch (InstantiationException e) {
//                        }
//                        return null;
//                   }
//                });
    }
    if (cHandler != NULL) {
//        if (!(cHandler instanceof ContentHandler)) {
//            throw new UnknownServiceException();
//        }
        mContentHandlers[type] = cHandler; // if we got the handler,
        // cache it for next time
        *contentHandler = cHandler;
        return NOERROR;
    }

    *contentHandler = mDefaultHandler;

    return NOERROR;
}

ECode URLConnection::GetContentLength(
    /* [out] */ Int32* length)
{
    return GetHeaderFieldInt32(String("Content-Length"), -1, length);
}

ECode URLConnection::GetContentType(
    /* [out] */ String* type)
{
    return GetHeaderFieldByKey(String("Content-Type"), type);
}

ECode URLConnection::GetDate(
    /* [out] */ Int64* date)
{
    return GetHeaderFieldDate(String("Date"), 0, date);
}

Boolean URLConnection::GetDefaultAllowUserInteraction()
{
    return mDefaultAllowUserInteraction;
}

String URLConnection::GetDefaultRequestProperty(
    /* [in] */ const String& field)
{
    return String(NULL);
}

ECode URLConnection::GetDefaultUseCaches(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mDefaultUseCaches;
    return NOERROR;
}

ECode URLConnection::GetDoInput(
    /* [out] */ Boolean* allowed)
{
    VALIDATE_NOT_NULL(allowed);
    *allowed = mDoInput;
    return NOERROR;
}

ECode URLConnection::GetDoOutput(
    /* [out] */ Boolean* allowed)
{
    VALIDATE_NOT_NULL(allowed);
    *allowed = mDoOutput;
    return NOERROR;
}

ECode URLConnection::GetExpiration(
    /* [out] */ Int64* expiration)
{
    return GetHeaderFieldDate(String("Expires"), 0, expiration);
}

AutoPtr<IFileNameMap> URLConnection::GetFileNameMap()
{
//    synchronized (URLConnection.class) {
    if (mFileNameMap == NULL) {
        mFileNameMap = (IFileNameMap*)new DefaultFileNameMap();
    }
    return mFileNameMap;
//    }
}

ECode URLConnection::GetHeaderFieldByPosition(
    /* [in] */ Int32 pos,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = String(NULL);
    return NOERROR;
}

//    public Map<String, List<String>> getHeaderFields()

//    public Map<String, List<String>> getRequestProperties()

ECode URLConnection::CheckNotConnected()
{
    if (mConnected) {
        return E_ILLEGAL_STATE_EXCEPTION;
//        throw new IllegalStateException("Already connected");
    }

    return NOERROR;
}

ECode URLConnection::AddRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    CheckNotConnected();
    if (field.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("field == null");
    }

    return NOERROR;
}

ECode URLConnection::GetHeaderFieldByKey(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = String(NULL);
    return NOERROR;
}

ECode URLConnection::GetHeaderFieldDate(
    /* [in] */ const String& field,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

//    String date;
//    GetHeaderField(field, date);
//    if (date.IsNull()) {
        *value = defaultValue;
        return NOERROR;
//    }
//    try {
//        return Date.parse(date);
//    } catch (Exception e) {
//        return defaultValue;
//    }
}

ECode URLConnection::GetHeaderFieldInt32(
    /* [in] */ const String& field,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
//    try {
//    return Integer.parseInt(getHeaderField(field));
    String s;
    ECode ec = GetHeaderFieldByKey(field, &s);
    if (FAILED(ec)) {
        *value = defaultValue;
    }

    *value = s.ToInt32();
    return NOERROR;
//    } catch (NumberFormatException e) {
//        return defaultValue;
//    }
}

ECode URLConnection::GetHeaderFieldKey(
    /* [in] */ Int32 posn,
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key);
    *key = String(NULL);
    return NOERROR;
}

ECode URLConnection::GetIfModifiedSince(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mIfModifiedSince;
    return NOERROR;
}

ECode URLConnection::GetInputStream(
    /* [out] */ IInputStream** is)
{
    return E_UNKNOWN_SERVICE_EXCEPTION;
//    throw new UnknownServiceException("Does not support writing to the input stream");
}

ECode URLConnection::GetLastModified(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    if (mLastModified != -1) {
        *value = mLastModified;
        return NOERROR;
    }
    GetHeaderFieldDate(String("Last-Modified"), 0, &mLastModified);
    *value = mLastModified;

    return NOERROR;
}

ECode URLConnection::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    return E_UNKNOWN_SERVICE_EXCEPTION;
//    throw new UnknownServiceException("Does not support writing to the output stream");
}

//        public java.security.Permission getPermission();

ECode URLConnection::GetRequestProperty(
    /* [in] */ const String& field,
    /* [out] */ String* property)
{
    VALIDATE_NOT_NULL(property);

    CheckNotConnected();
    *property = String(NULL);
    return NOERROR;
}

ECode URLConnection::GetURL(
    /* [out] */ IURL** url)
{
    VALIDATE_NOT_NULL(url);
    *url = mUrl;
    return NOERROR;
}

ECode URLConnection::GetUseCaches(
    /* [out] */ Boolean* allowed)
{
    VALIDATE_NOT_NULL(allowed);
    *allowed = mUseCaches;
    return NOERROR;
}

String URLConnection::GuessContentTypeFromName(
    /* [in] */ const String& url)
{
    String s;
    GetFileNameMap()->GetContentTypeFor(url, &s);
    return s;
}

String URLConnection::GuessContentTypeFromStream(
    /* [in] */ IInputStream* is)
{
    Boolean supported;
    is->IsMarkSupported(&supported);
    if (!supported) {
        return String(NULL);
    }
    // Look ahead up to 64 bytes for the longest encoded header
    is->Mark(64);
    ArrayOf<Byte>* bytes = ArrayOf<Byte>::Alloc(64);
    Int32 length;
    is->ReadBuffer(bytes, &length);
    is->Reset();

    // If there is no data from the input stream, we can't determine content type.
    if (length == -1) {
        ArrayOf<Byte>::Free(bytes);
        return String(NULL);
    }

    // Check for Unicode BOM encoding indicators
    String encoding("US-ASCII");
    Int32 start = 0;
    if (length > 1) {
        if (((*bytes)[0] == (Byte) 0xFF) && ((*bytes)[1] == (Byte) 0xFE)) {
            encoding = "UTF-16LE";
            start = 2;
            length -= length & 1;
        }
        if (((*bytes)[0] == (Byte) 0xFE) && ((*bytes)[1] == (Byte) 0xFF)) {
            encoding = "UTF-16BE";
            start = 2;
            length -= length & 1;
        }
        if (length > 2) {
            if (((*bytes)[0] == (Byte) 0xEF) && ((*bytes)[1] == (Byte) 0xBB)
                    && ((*bytes)[2] == (Byte) 0xBF)) {
                encoding = "UTF-8";
                start = 3;
            }
            if (length > 3) {
                if (((*bytes)[0] == (Byte) 0x00) && ((*bytes)[1] == (Byte) 0x00)
                        && ((*bytes)[2] == (Byte) 0xFE)
                        && ((*bytes)[3] == (Byte) 0xFF)) {
                    encoding = "UTF-32BE";
                    start = 4;
                    length -= length & 3;
                }
                if (((*bytes)[0] == (Byte) 0xFF) && ((*bytes)[1] == (Byte) 0xFE)
                        && ((*bytes)[2] == (Byte) 0x00)
                        && ((*bytes)[3] == (Byte) 0x00)) {
                    encoding = "UTF-32LE";
                    start = 4;
                    length -= length & 3;
                }
            }
        }
    }

    String temp((const char*)bytes->GetPayload());
    String header = temp.Substring(start, length - start/*, encoding*/);

    // Check binary types
    if (header.StartWith("PK")) {
        ArrayOf<Byte>::Free(bytes);
        return String("application/zip");
    }
    if (header.StartWith("GI")) {
        ArrayOf<Byte>::Free(bytes);
        return String("image/gif");
    }

    // Check text types
    String textHeader = header.Trim();
    textHeader.ToUpperCase();
    if (textHeader.StartWith("<!DOCTYPE HTML") ||
            textHeader.StartWith("<HTML") ||
            textHeader.StartWith("<HEAD") ||
            textHeader.StartWith("<BODY") ||
            textHeader.StartWith("<HEAD")) {
        ArrayOf<Byte>::Free(bytes);
        return String("text/html");
    }

    if (textHeader.StartWith("<?XML")) {
        ArrayOf<Byte>::Free(bytes);
        return String("application/xml");
    }

    // Give up
    ArrayOf<Byte>::Free(bytes);
    return String(NULL);
}

String URLConnection::ParseTypeString(
    /* [in] */ const String& typeString)
{
    StringBuf typeStringBuffer(const_cast<char*>((const char*)typeString), typeString.GetLength());
    for (Int32 i = 0; i < typeStringBuffer.GetLength(); i++) {
        // if non-alphanumeric, replace it with '_'
        Char32 c = typeStringBuffer[i];
        if (!(Character::IsLetter(c) || Character::IsDigit(c) || c == '.')) {
            typeStringBuffer.Replace(i, 1, "_");
        }
    }
    return (String)typeStringBuffer;
}

ECode URLConnection::SetAllowUserInteraction(
    /* [in] */ Boolean newValue)
{
    CheckNotConnected();
    mAllowUserInteraction = newValue;

    return NOERROR  ;
}

ECode URLConnection::SetContentHandlerFactory(
    /* [in] */ IContentHandlerFactory* contentFactory)
{
    if (mContentHandlerFactory != NULL) {
        return NOERROR;
//        throw new Error("Factory already set");
    }
//    SecurityManager sManager = System.getSecurityManager();
//    if (sManager != null) {
//        sManager.checkSetFactory();
//    }
    mContentHandlerFactory = contentFactory;
    return NOERROR;
}

void URLConnection::SetDefaultAllowUserInteraction(
    /* [in] */ Boolean allows)
{
    mDefaultAllowUserInteraction = allows;
}

void URLConnection::SetDefaultRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& value)
{
}

ECode URLConnection::SetDefaultUseCaches(
    /* [in] */ Boolean newValue)
{
    mDefaultUseCaches = newValue;

    return NOERROR;
}

ECode URLConnection::SetDoInput(
    /* [in] */ Boolean newValue)
{
    CheckNotConnected();
    mDoInput = newValue;

    return NOERROR;
}

ECode URLConnection::SetDoOutput(
    /* [in] */ Boolean newValue)
{
    CheckNotConnected();
    mDoOutput = newValue;

    return NOERROR;
}

void URLConnection::SetFileNameMap(
    /* [in] */ IFileNameMap* map)
{
//    SecurityManager manager = System.getSecurityManager();
//    if (manager != null) {
//        manager.checkSetFactory();
//    }
//    synchronized (URLConnection.class) {
    mFileNameMap = map;
//    }
}

ECode URLConnection::SetIfModifiedSince(
    /* [in] */ Int64 newValue)
{
    CheckNotConnected();
    mIfModifiedSince = newValue;

    return NOERROR;
}

ECode URLConnection::SetRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    CheckNotConnected();
    if (field.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("field == null");
    }

    return NOERROR;
}

ECode URLConnection::SetUseCaches(
    /* [in] */ Boolean newValue)
{
    CheckNotConnected();
    mUseCaches = newValue;

    return NOERROR;
}

ECode URLConnection::SetConnectTimeout(
    /* [in] */ Int32 timeout)
{
    if (timeout < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("timeout < 0");
    }
    mConnectTimeout = timeout;

    return NOERROR;
}

ECode URLConnection::GetConnectTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    *timeout = mConnectTimeout;
    return NOERROR;
}

ECode URLConnection::SetReadTimeout(
    /* [in] */ Int32 timeout)
{
    if (timeout < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("timeout < 0");
    }
    mReadTimeout = timeout;

    return NOERROR;
}

ECode URLConnection::GetReadTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    *timeout = mReadTimeout;
    return NOERROR;
}
