
#include "webkit/LoadListener.h"
#include "webkit/CCookieManager.h"
#include "webkit/CCacheManager.h"
#include "webkit/CacheLoader.h"
#include "webkit/Network.h"
#include "webkit/WebViewWorker.h"
#include "webkit/PerfChecker.h"
#include "webkit/CURLUtil.h"
#include "webkit/CBrowserFrame.h"
#include "webkit/CertTool.h"
#include "webkit/CByteArrayBuilder.h"
//#include "utils//CObjectStringMap.h"
#include "webkit/CertTool.h"
#include "os/CApartment.h"
#include "webkit/DebugFlags.h"
#include "content/CIntent.h"
#include "net/Uri.h"
#include "content/CResolveInfo.h"
//#include "webkit/CHttpAuthHandler.h"
#include "webkit/CCallbackProxy.h"
#include "../../../res/gen/R.h"
#include "webkit/CMimeTypeMap.h"

#include <elastos/AutoFree.h>
#include <Logger.h>

// This is the same regex that DOMImplementation uses to check for xml
// content. Use this to check if another Activity wants to handle the
// content before giving it to webkit.
const CString XML_MIME_TYPE("^[\\w_\\-+~!$\\^{}|.%'`#&*]+/[\\w_\\-+~!$\\^{}|.%'`#&*]+\\+xml$");

const CString LoadListener::LOGTAG("webkit");

// Messages used internally to communicate state between the
// Network thread and the WebCore thread.
const Int32 LoadListener::MSG_CONTENT_HEADERS;
const Int32 LoadListener::MSG_CONTENT_DATA;
const Int32 LoadListener::MSG_CONTENT_FINISHED;
const Int32 LoadListener::MSG_CONTENT_ERROR;
const Int32 LoadListener::MSG_LOCATION_CHANGED;
const Int32 LoadListener::MSG_LOCATION_CHANGED_REQUEST;
const Int32 LoadListener::MSG_STATUS;
const Int32 LoadListener::MSG_SSL_CERTIFICATE;
const Int32 LoadListener::MSG_SSL_ERROR;

// Standard HTTP status codes in a more representative format
const Int32 LoadListener::HTTP_OK;
const Int32 LoadListener::HTTP_PARTIAL_CONTENT;
const Int32 LoadListener::HTTP_MOVED_PERMANENTLY;
const Int32 LoadListener::HTTP_FOUND;
const Int32 LoadListener::HTTP_SEE_OTHER;
const Int32 LoadListener::HTTP_NOT_MODIFIED;
const Int32 LoadListener::HTTP_TEMPORARY_REDIRECT;
const Int32 LoadListener::HTTP_AUTH;
const Int32 LoadListener::HTTP_NOT_FOUND;
const Int32 LoadListener::HTTP_PROXY_AUTH;

AutoPtr<IObjectStringMap> LoadListener::sCertificateTypeMap;

Int32 LoadListener::sNativeLoaderCount;

CARAPI_(LoadListener*) LoadListener::GetLoadListener(
    /* [in] */ IContext* context,
    /* [in] */ IBrowserFrame* frame, 
    /* [in] */ const String& url, 
    /* [in] */ Int32 nativeLoader,
    /* [in] */ Boolean synchronous, 
    /* [in] */ Boolean isMainPageLoader,
    /* [in] */ Boolean isMainResource, 
    /* [in] */ Boolean userGesture, 
    /* [in] */ Int64 postIdentifier,
    /* [in] */ const String& username, 
    /* [in] */ const String& password)
{
    assert(context != NULL);
    assert(frame != NULL);

    sNativeLoaderCount += 1;

    return new LoadListener(context, frame, url, nativeLoader, synchronous,
            isMainPageLoader, isMainResource, userGesture, postIdentifier,
            username, password);
}

CARAPI_(Int32) LoadListener::GetNativeLoaderCount()
{
    return sNativeLoaderCount;
}

LoadListener::LoadListener(
    /* [in] */ IContext* context, 
    /* [in] */ IBrowserFrame* frame, 
    /* [in] */ const String& url,
    /* [in] */ Int32 nativeLoader, 
    /* [in] */ Boolean synchronous, 
    /* [in] */ Boolean isMainPageLoader,
    /* [in] */ Boolean isMainResource, 
    /* [in] */ Boolean userGesture, 
    /* [in] */ Int64 postIdentifier,
    /* [in] */ const String& username, 
    /* [in] */ const String& password) : mContext(context), mBrowserFrame(frame),
                                         mNativeLoader(nativeLoader), mSynchronous(synchronous),
                                         mIsMainPageLoader(isMainPageLoader), mIsMainResourceLoader(isMainResource),
                                         mUserGesture(userGesture), mPostIdentifier(postIdentifier),
                                         mUsername(username), mPassword(password)
{
    assert(SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment))
        && (mApartment != NULL));
    //sCertificateTypeMap=CObjectStringMap::New((IObjectStringMap**)&sCertificateTypeMap);
    sCertificateTypeMap->Put(String("application/x-x509-ca-cert"), (IInterface*)(&(CertTool::CERT)) );
    sCertificateTypeMap->Put(String("application/x-x509-user-cert"), (IInterface*)(&(CertTool::CERT)) );
    sCertificateTypeMap->Put(String("application/x-pkcs12"), (IInterface*)(&(CertTool::PKCS12)) );
    CByteArrayBuilder::New((IByteArrayBuilder**)&mDataBuilder);
    if (DebugFlags::sLOAD_LISTENER) {
        Utility::Logging::Logger::V(LOGTAG, String("LoadListener constructor url=") + url + "\n");
    }    
    SetUrl(url);       
    if (synchronous) {
//        mMessageQueue = new Vector<Message>();
    }    
}

/**
 * We keep a count of refs to the nativeLoader so we do not create
 * so many LoadListeners that the GREFs blow up
 */
CARAPI_(void) LoadListener::ClearNativeLoader()
{
    sNativeLoaderCount -= 1;
    mNativeLoader = 0;
}

//MSG_CONTENT_HEADERS
void LoadListener::HandleMsgContentHeaders(
    /* [in] */ IHeaders* headers)
 {
    /*
     * This message is sent when the LoadListener has headers
     * available. The headers are sent onto WebCore to see what we
     * should do with them.
     */
    HandleHeaders(headers);
 }

//MSG_CONTENT_DATA
void LoadListener::HandleMsgContentData()
{
    /*
     * This message is sent when the LoadListener has data available
     * in it's data buffer. This data buffer could be filled from a
     * file (this thread) or from http (Network thread).
     */
    if (mNativeLoader != 0 && !IgnoreCallbacks()) {
        CommitLoad();
    }    
}

//MSG_CONTENT_FINISHED
void LoadListener::HandleMsgContentFinished()
{
    /*
     * This message is sent when the LoadListener knows that the
     * load is finished. This message is not sent in the case of an
     * error.
     *
     */
    HandleEndData();    
}

//MSG_CONTENT_ERROR
void LoadListener::HandleMsgContentError(
    /* [in] */ Int32 arg1, 
    /* [in] */ String obj)
{
    /*
     * This message is sent when a load error has occured. The
     * LoadListener will clean itself up.
     */
    HandleError(arg1, obj);
}

//MSG_LOCATION_CHANGED
void LoadListener::HandleMsgLocationChanged()
{
    /*
     * This message is sent from LoadListener.endData to inform the
     * browser activity that the location of the top level page
     * changed.
     */
    DoRedirect();    
}

//MSG_LOCATION_CHANGED_REQUEST
void LoadListener::HandleMsgLocationChangedRequest()
{
    /*
     * This message is sent from endData on receipt of a 307
     * Temporary Redirect in response to a POST -- the user must
     * confirm whether to continue loading. If the user says Yes,
     * we simply call MSG_LOCATION_CHANGED. If the user says No,
     * we call MSG_CONTENT_FINISHED.
     */
    (((CBrowserFrame*)(mBrowserFrame.Get()))->GetCallbackProxy())->OnFormResubmission((IApartment*)this, MSG_CONTENT_FINISHED, NULL, (IApartment*)this, MSG_LOCATION_CHANGED, NULL);
}

//MSG_STATUS
void LoadListener::HandleMsgStatus(
    /* [in] */ IObjectStringMap* obj)
{
    /*
     * This message is sent from the network thread when the http
     * stack has received the status response from the server.
     */
   AutoPtr<IObjectStringMap> status = obj;
   Int32 *major, *minor, *code;
   String *reason;
   status->Get(String("major"), (IInterface**)(&major));
   status->Get(String("minor"), (IInterface**)(&minor));
   status->Get(String("code"), (IInterface**)(&code));
   status->Get(String("reason"), (IInterface**)(&reason));
   HandleStatus( *major, *minor, *code, *reason);
}

//MSG_SSL_CERTIFICATE
void LoadListener::HandleMsgSslCertificate(
    /* [in] */ ISslCertificate* obj)
{
    /*
     * This message is sent when the network thread receives a ssl
     * certificate.
     */
    HandleCertificate(obj);
}

//MSG_SSL_ERROR
void LoadListener::HandleMsgSslError(
    /* [in] */ ISslError* obj)
{
    /*
     * This message is sent when the network thread encounters a
     * ssl error.
     */
    HandleSslError(obj);
}

/**
 * @return The loader's BrowserFrame.
 */
CARAPI_(AutoPtr<IBrowserFrame>) LoadListener::GetFrame() const
{
    return mBrowserFrame;
}

CARAPI_(AutoPtr<IContext>) LoadListener::GetContext() const
{
    return mContext;
}

/* package */ 
CARAPI_(Boolean) LoadListener::IsSynchronous()
{
    return mSynchronous;
}

/**
 * @return True iff the load has been cancelled
 */
CARAPI_(Boolean) LoadListener::Cancelled()
{
    return mCancelled;
}

/**
 * Parse the headers sent from the server.
 * @param headers gives up the HeaderGroup
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(void) LoadListener::Headers(
    /* [in] */ IHeaders* headers)
{
    assert(headers != NULL);

    //if (DebugFlags.LOAD_LISTENER) Log.v(LOGTAG, "LoadListener.headers");
    // call db (setCookie) in the non-WebCore thread
    if (mCancelled) {
        return;
    }

    Vector<String> cookies;// = headers.getSetCookie();
    Int32 size = cookies.GetSize();
    ICookieManager* pCookieManager = NULL;
    CCookieManager::AcquireSingleton(&pCookieManager);
    for (Int32 i = 0; i < size; ++i) {
        pCookieManager->SetCookieEx(mUri, cookies[i]);
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(headers);
    SendMessageInternal(MSG_CONTENT_HEADERS, params.Get());
}

// Does the header parsing work on the WebCore thread.
CARAPI_(void) LoadListener::HandleHeaders(
    /* [in] */ IHeaders* headers)
{
    if (mCancelled){
        return;
    }

    // Note: the headers we care in LoadListeners, like
    // content-type/content-length, should not be updated for partial
    // content. Just skip here and go ahead with adding data.
    if (mStatusCode == HTTP_PARTIAL_CONTENT) {
        // we don't support cache for partial content yet
        //JAVA:  WebViewWorker.getHandler().obtainMessage(WebViewWorker.MSG_REMOVE_CACHE, this).sendToTarget();
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(this);
        (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_REMOVE_CACHE, params);
        return;
    }

    mHeaders = headers;

    Int64 contentLength = 0;
    //headers->GetContentLength(&contentLength);
    if (contentLength != /*Headers_NO_CONTENT_LENGTH*/-1) {
        mContentLength = contentLength;
    }
    else {
        mContentLength = 0;
    }

    String contentType;
    //headers->GetContentType(&contentType);
    if (!(contentType.IsNull())) {
        ParseContentTypeHeader(contentType);

        // If we have one of "generic" MIME types, try to deduce
        // the right MIME type from the file extension (if any):
        if (mMimeType.Equals("text/plain") || mMimeType.Equals("application/octet-stream")) {
            // for attachment, use the filename in the Content-Disposition
            // to guess the mimetype
            String contentDisposition;
            //headers->GetContentDisposition(&contentDisposition);
            String url = String(NULL);
            if ( !(contentDisposition.IsNull()) ) {
                url = CURLUtil::ParseContentDisposition(contentDisposition);
            }
            if (url.IsNull()) {
                url = mUrl;
            }            
            String newMimeType;
            GuessMimeTypeFromExtension(url, &newMimeType);
            if (newMimeType.IsNull()) {
                mMimeType = newMimeType;
            }
        } 
        else if (mMimeType.Equals("text/vnd.wap.wml")) {
            // As we don't support wml, render it as plain text
            mMimeType = String("text/plain");
        } 
        else {
            // It seems that xhtml+xml and vnd.wap.xhtml+xml mime
            // subtypes are used interchangeably. So treat them the same.
            if (mMimeType.Equals("application/vnd.wap.xhtml+xml")) {
                mMimeType = String("application/xhtml+xml");
            }
        }
    } 
    else {
        /* Often when servers respond with 304 Not Modified or a
           Redirect, then they don't specify a MIMEType. When this
           occurs, the function below is called.  In the case of
           304 Not Modified, the cached headers are used rather
           than the headers that are returned from the server. */
        GuessMimeType();
    }
    // At this point, mMimeType has been set to non-null.
    if (mIsMainPageLoader && mIsMainResourceLoader && mUserGesture &&
            //Pattern::Matches(XML_MIME_TYPE, mMimeType) &&
            !mMimeType.EqualsIgnoreCase("application/xhtml+xml")) {
        AutoPtr<IIntent> i;
        CIntent::New(String(Intent_ACTION_VIEW), (IIntent**)&i);
        AutoPtr<IUri> uriP;
        String strUrl;
        Url(&strUrl);
        Uri::Parse(strUrl, (IUri**)&uriP);
        i->SetDataAndType(uriP, mMimeType);
        //JAVA:  ResolveInfo info = mContext.getPackageManager().resolveActivity(i, PackageManager.MATCH_DEFAULT_ONLY);
        // AutoPtr<IPackageManager> packageManager;
        // mContext->GetPackageName( (IPackageManager**)&packageManager);
        AutoPtr<IResolveInfo> info; 
        //packageManager->ResolveActivity(i, PackageManager_MATCH_DEFAULT_ONLY, (IResolveInfo**)&info);
        String strPackageName;
        //mContext->GetPackageName(&strPackageName);
        String strActivityPackageName;
        ((CResolveInfo*)(info.Get()))->mActivityInfo->GetName(&strActivityPackageName);
        if (info != NULL && !strPackageName.Equals(strActivityPackageName)) {
            // someone (other than the current app) knows how to
            // handle this mime type.
            
            //try {
                mContext->StartActivity(i);
                mBrowserFrame->StopLoading();
                return ;
            //} catch (ActivityNotFoundException ex) {
                // continue loading internally.
            //}
        }
    }

    // is it an authentication request?
    Boolean mustAuthenticate = (mStatusCode == HTTP_AUTH ||
                mStatusCode == HTTP_PROXY_AUTH);
    // is it a proxy authentication request?
    Boolean isProxyAuthRequest = (mStatusCode == HTTP_PROXY_AUTH);
    // is this authentication request due to a failed attempt to
    // authenticate ealier?
    mAuthFailed = FALSE;

    // if we tried to authenticate ourselves last time
    if ( /*mAuthHeader!=NULL*/FALSE ) {
        // we failed, if we must authenticate again now and
        // we have a proxy-ness match
        Boolean isProxy=FALSE;
        //mAuthHeader->isProxy(&isProxy);
        mAuthFailed = (mustAuthenticate && (isProxyAuthRequest == isProxy) );

        // if we did NOT fail and last authentication request was a
        // proxy-authentication request
        if (!mAuthFailed && isProxy) {
            AutoPtr<Network> network = Network::GetInstance(mContext);
            // if we have a valid proxy set
            if (network->IsValidProxySet()) {
                /* The proxy credentials can be read in the WebCore thread
                */
                Core::Threading::Mutex::Autolock lock(network->mSyncLock);
                // save authentication credentials for pre-emptive proxy
                // authentication
                String strUsername,strPassword;
                //mAuthHeader->GetUsername(&strUsername);
                //mAuthHeader->GetPassword(&strPassword);
                network->SetProxyUsername(strUsername);
                network->SetProxyPassword(strPassword);
            }
        }
    }

    // it is only here that we can reset the last mAuthHeader object
    // (if existed) and start a new one!!!
    //mAuthHeader = NULL;
    if (mustAuthenticate) {
        if (mStatusCode == HTTP_AUTH) {
            // String strWwwAuthenticate;
            // headers->GetWwwAuthenticate(&strWwwAuthenticate);
            // mAuthHeader = ParseAuthHeader(strWwwAuthenticate);
        }
        else {
            // String strProxyAuthenticate;
            // headers->GetProxyAuthenticate(&strProxyAuthenticate);
            // mAuthHeader = parseAuthHeader(strProxyAuthenticate);
            // if successfully parsed the header
            if (/*mAuthHeader != NULL*/FALSE) {
                // mark the auth-header object as a proxy
                //mAuthHeader->SetProxy();
            }
        }
    }

    // Only create a cache file if the server has responded positively.
    if ((mStatusCode == HTTP_OK ||
            mStatusCode == HTTP_FOUND ||
            mStatusCode == HTTP_MOVED_PERMANENTLY ||
            mStatusCode == HTTP_TEMPORARY_REDIRECT) && 
            mNativeLoader != 0) {
        // for POST request, only cache the result if there is an identifier
        // associated with it. postUrl() or form submission should set the
        // identifier while XHR POST doesn't.
        String strMethod;
        //mRequestHandle->GetMethod(&strMethod);
        if (!mFromCache /*&& mRequestHandle != NULL*/
                && (!strMethod.Equals("POST")
                        || mPostIdentifier != 0)) {
            WebViewWorker::CacheCreateData* data = new WebViewWorker::CacheCreateData();
            data->mListener = this;
            data->mUrl = mUrl;
            data->mMimeType = mMimeType;
            data->mStatusCode = mStatusCode;
            data->mPostId = mPostIdentifier;
            data->mHeaders = headers;
            //JAVA:  WebViewWorker.getHandler().obtainMessage( WebViewWorker.MSG_CREATE_CACHE, data).sendToTarget();
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteStruct((Handle32)data,sizeof(WebViewWorker::CacheCreateData));
            (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_CREATE_CACHE,params);
            delete data;
        }
        WebViewWorker::CacheEncoding* ce = new WebViewWorker::CacheEncoding();
        ce->mEncoding = mEncoding;
        ce->mListener = this;
        //JAVA:  WebViewWorker.getHandler().obtainMessage( WebViewWorker.MSG_UPDATE_CACHE_ENCODING, ce).sendToTarget();
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteStruct((Handle32)ce,sizeof(WebViewWorker::CacheEncoding));
        (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_UPDATE_CACHE_ENCODING,params);
        delete ce;
    }
    CommitHeadersCheckRedirect();
}

/**
 * @return True iff this loader is in the proxy-authenticate state.
 */
CARAPI_(Boolean) LoadListener::ProxyAuthenticate()
{
   if (/*mAuthHeader != NULL*/FALSE) {
       return /*mAuthHeader->IsProxy()*/FALSE;
   }
   return FALSE;
}

/**
 * Report the status of the response.
 * TODO: Comments about each parameter.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(void) LoadListener::Status(
    /* [in] */ Int32 majorVersion, 
    /* [in] */ Int32 minorVersion,
    /* [in] */ Int32 code, /* Status-Code value */ 
    /* [in] */ const String& reasonPhrase)
{
    if (DebugFlags::sLOAD_LISTENER) {
        String strOut=String("LoadListener: from: ")+mUrl;
        strOut+=String(" major: ")+String::FromInt32(majorVersion);
        strOut+=String(" minor: ")+String::FromInt32(minorVersion);
        strOut+=String(" code: ")+String::FromInt32(code);
        strOut+=String(" reason: ")+reasonPhrase;
        strOut+=String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }

    IObjectStringMap* status = NULL;
    CObjectStringMap::New(&status);
    assert(status);

    Int32* _majorVersion  = new Int32(majorVersion);
    Int32* _minorVersion  = new Int32(minorVersion);
    Int32* _code          = new Int32(code);
    String* _reasonPhrase = new String("reason");

    status->Put((String)"major",  (IInterface*)_majorVersion);
    status->Put((String)"minor",  (IInterface*)_minorVersion);
    status->Put((String)"code",   (IInterface*)_code);
    status->Put((String)"reason", (IInterface*)_reasonPhrase);

    // New status means new data. Clear the old.
    mDataBuilder->Clear();
    mMimeType = "";
    mEncoding = "";
    mTransferEncoding = "";

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(status);
    SendMessageInternal(MSG_STATUS, params.Get());
}

// Handle the status callback on the WebCore thread.
CARAPI_(void) LoadListener::HandleStatus(
    /* [in] */ Int32 major, 
    /* [in] */ Int32 minor, 
    /* [in] */ Int32 code, 
    /* [in] */ const String& reason)
{
    if (mCancelled) {
        return;
    }

    mStatusCode = code;
    mStatusText = reason;
    mPermanent = FALSE;
}

/**
 * Implementation of certificate handler for EventHandler. Called
 * before a resource is requested. In this context, can be called
 * multiple times if we have redirects
 *
 * IMPORTANT: as this is called from network thread, can't call
 * native directly
 *
 * @param certificate The SSL certifcate or null if the request
 * was not secure
 */
CARAPI_(void) LoadListener::Certificate(
    /* [in] */ ISslCertificate* certificate)
{    
    if (DebugFlags::sLOAD_LISTENER) {
        String strCertificate;
        //certificate->ToString(&strCertificate);
        String strOut=String("LoadListener::Certificate: ")+strCertificate+String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(certificate);
    SendMessageInternal(MSG_SSL_CERTIFICATE, params.Get());
}

// Handle the certificate on the WebCore thread.
CARAPI_(void) LoadListener::HandleCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    //assert(certificate != NULL);
    if (mIsMainPageLoader && mIsMainResourceLoader) {
        // update the browser frame with certificate
        ((CBrowserFrame*)mBrowserFrame.Get())->Certificate(certificate);
    }
}

/**
 * Implementation of error handler for EventHandler.
 * Subclasses should call this method to have error fields set.
 * @param id The error id described by EventHandler.
 * @param description A string description of the error.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(void) LoadListener::Error(
    /* [in] */ Int32 id, 
    /* [in] */ const String& description)
{
    if (DebugFlags::sLOAD_LISTENER) {
        String strUrl;
        Url(&strUrl);
        String strOut = String("LoadListener::error url:") + strUrl;
        strOut += String(" id:") + String::FromInt32(id);
        strOut += String(" description:") + description;
        strOut += String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(id);
    params->WriteString(description);
    SendMessageInternal(MSG_CONTENT_ERROR, params.Get());
}

// Handle the error on the WebCore thread.
CARAPI_(void) LoadListener::HandleError(
    /* [in] */ Int32 id, 
    /* [in] */ const String& description)
{
    mErrorID = id;
    mErrorDescription = description;
    DetachRequestHandle();
    NotifyError();
    TearDown();
}

/**
 * Add data to the internal collection of data. This function is used by
 * the data: scheme, about: scheme and http/https schemes.
 * @param data A byte array containing the content.
 * @param length The length of data.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 * XXX: Unlike the other network thread methods, this method can do the
 * work of decoding the data and appending it to the data builder.
 */
CARAPI_(void) LoadListener::Data(
    /* [in] */ ArrayOf<Byte>& data, 
    /* [in] */ Int32 length)
{
    if (DebugFlags::sLOAD_LISTENER) {
        String strUrl;
        Url(&strUrl);
        String strOut = String("LoadListener::Data(): url: ") + strUrl;
        strOut += String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }

    // The reason isEmpty() and append() need to synchronized together is
    // because it is possible for getFirstChunk() to be called multiple
    // times between isEmpty() and append(). This could cause commitLoad()
    // to finish before processing the newly appended data and no message
    // will be sent.
    Boolean sendMessage = FALSE;

    mDataBuilderMutex.Lock();
    {
        mDataBuilder->IsEmpty(&sendMessage);
        mDataBuilder->Append(data, 0, length);
    }
    mDataBuilderMutex.Unlock();

    if (sendMessage) {
        // Send a message whenever data comes in after a write to WebCore
        SendMessageInternal(MSG_CONTENT_DATA, NULL);
    }
}

/**
 * Event handler's endData call. Send a message to the handler notifying
 * them that the data has finished.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(void) LoadListener::EndData()
{
    if (DebugFlags::sLOAD_LISTENER) {
        String strUrl;
        Url(&strUrl);
        String strOut = String("LoadListener::EndData(): url: ") + strUrl;
        strOut += String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
    SendMessageInternal(MSG_CONTENT_FINISHED, NULL);
}

// Handle the end of data.
CARAPI_(void) LoadListener::HandleEndData()
{
    if (mCancelled){
        return;
    }
    switch (mStatusCode) {
        case HTTP_MOVED_PERMANENTLY:
            // 301 - permanent redirect
            mPermanent = TRUE;
        case HTTP_FOUND:
        case HTTP_SEE_OTHER:
        case HTTP_TEMPORARY_REDIRECT:
            // 301, 302, 303, and 307 - redirect
            if (mStatusCode == HTTP_TEMPORARY_REDIRECT) {
                String strMethod;
                //mRequestHandle->GetMethod(&strMethod);
                if (/*mRequestHandle != NULL &&*/ strMethod.Equals("POST")) {
                    SendMessageInternal(MSG_LOCATION_CHANGED_REQUEST, NULL);
                } 
                else if (mMethod.IsNull() && mMethod.Equals("POST")) {
                    SendMessageInternal(MSG_LOCATION_CHANGED_REQUEST, NULL);
                } 
                else {
                    SendMessageInternal(MSG_LOCATION_CHANGED, NULL);
                }
            } 
            else {
                SendMessageInternal(MSG_LOCATION_CHANGED, NULL);
            }
            return;

        case HTTP_AUTH:
        case HTTP_PROXY_AUTH:
        {
            // According to rfc2616, the response for HTTP_AUTH must include
            // WWW-Authenticate header field and the response for 
            // HTTP_PROXY_AUTH must include Proxy-Authenticate header field.
            AutoPtr<Network> network = Network::GetInstance(mContext);
            if (/*mAuthHeader != NULL &&*/
                    (network->IsValidProxySet() /*|| (!mAuthHeader->IsProxy())*/ )) {
                // If this is the first attempt to authenticate, try again with the username and
                // password supplied in the URL, if present.
                if (!mAuthFailed && mUsername.IsNull() && mPassword.IsNull()) {
                    Boolean bIsProxy=FALSE;
                    //mAuthHeader->IsProxy(&bIsProxy);
                    String host;
                    if(bIsProxy){
                        network->GetProxyHostname(/*&*/host);
                    }
                    else{
                        //host=((CWebAddress*)(mUri.Get()))->mHost;
                    }                    
                    String strRealm;
                    //mAuthHeader->GetRealm(&strRealm);
                    //CHttpAuthHandler::OnReceivedCredentials(this, host, strRealm, mUsername, mPassword);
                    MakeAuthResponse(mUsername, mPassword);
                } 
                else {
                    network->HandleAuthRequest(this);
                }
                return;
            }
            break;  // use default
        }
        case HTTP_NOT_MODIFIED:
            // Server could send back NOT_MODIFIED even if we didn't
            // ask for it, so make sure we have a valid CacheLoader
            // before calling it.
            if (mCacheLoader != NULL) {
                if (IsSynchronous()) {
                    mCacheLoader->Load();
                } 
                else {
                    // Load the cached file in a separate thread
                    AutoPtr<WebViewWorker> webViewWorker = WebViewWorker::GetHandler();
                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    params->WriteInterfacePtr(mCacheLoader.Get());
                    webViewWorker->SendMessage(WebViewWorker::MSG_ADD_STREAMLOADER, params);
                }
                mFromCache = TRUE;
                if (DebugFlags::sLOAD_LISTENER) {
                    String strUrl;
                    Url(&strUrl);
                    String strOut = String("LoadListener cache load url=") + strUrl;
                    strOut += String("\n");
                    Utility::Logging::Logger::V(LOGTAG, strOut);
                }
                return;
            }
            break;  // use default

        case HTTP_NOT_FOUND:
            // Not an error, the server can send back content.
        default:
            break;
    }
    DetachRequestHandle();
    TearDown();
}

/* This method is called from CacheLoader when the initial request is
 * serviced by the Cache. */
/* package */ 
CARAPI_(void) LoadListener::SetCacheLoader(
    /* [in] */ CacheLoader* c)
{
    assert(c != NULL);

    mCacheLoader = c;
    mFromCache = TRUE;
}

/**
 * Check the cache for the current URL, and load it if it is valid.
 *
 * @param headers for the request
 * @return true if cached response is used.
 */
CARAPI_(Boolean) LoadListener::CheckCache(
    /* [in] */ IObjectStringMap* headers)
{
    assert(headers != NULL);

    // Get the cache file name for the current URL
    String strUrl;
    Url(&strUrl);

    AutoPtr<ICacheManagerCacheResult> result = NULL;
    result = CCacheManager::GetCacheFile(strUrl, mPostIdentifier, headers);

    // Go ahead and set the cache loader to null in case the result is
    // null.
    mCacheLoader = NULL;
    // reset the flag
    mFromCache = FALSE;

    if (result != NULL) {
        // The contents of the cache may need to be revalidated so just
        // remember the cache loader in the case that the server responds
        // positively to the cached content. This is also used to detect if
        // a redirect came from the cache.
        mCacheLoader = new CacheLoader(this, (CCacheManager::CacheResult*)result.Get());

        // If I got a cachedUrl and the revalidation header was not
        // added, then the cached content valid, we should use it.
        IInterface* value1 = NULL;
        IInterface* value2 = NULL;
        headers->Get((String)CCacheManager::HEADER_KEY_IFNONEMATCH, &value1);
        headers->Get((String)CCacheManager::HEADER_KEY_IFMODIFIEDSINCE, &value2);
        if (value1 == NULL && value2 == NULL) {            
            if (DebugFlags::sLOAD_LISTENER) {
                String strUrl;
                Url(&strUrl);
                String strOut = String("FrameLoader: HTTP URL in cache and usable: ") + strUrl;
                strOut += String("\n");
                Utility::Logging::Logger::V(LOGTAG, strOut);
            }
            if (IsSynchronous()) {
                mCacheLoader->Load();
            } else {
                // Load the cached file in a separate thread
                AutoPtr<WebViewWorker> webViewWorker = WebViewWorker::GetHandler();
                AutoPtr<IParcel> params;
                CCallbackParcel::New((IParcel**)&params);
                params->WriteInterfacePtr(mCacheLoader.Get());
                webViewWorker->SendMessage(WebViewWorker::MSG_ADD_STREAMLOADER, params);
            }
            mFromCache = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * SSL certificate error callback. Handles SSL error(s) on the way up
 * to the user.
 * IMPORTANT: as this is called from network thread, can't call native
 * directly
 */
CARAPI_(Boolean) LoadListener::HandleSslErrorRequest(
    /* [in] */ ISslError* error)
{
    //assert(error != NULL);
    if (DebugFlags::sLOAD_LISTENER) {        
        String strUrl;
        Url(&strUrl);
        Int32 primaryError=0;
        //error->GetPrimaryError(&primaryError);
        AutoPtr<ISslCertificate> sslCertificate;
        //error->GetCertificate((ISslCertificate**)&sslCertificate);
        String strCertificate;
        //sslCertificate->ToString(&strCertificate);
        String strOut = String("LoadListener::handleSslErrorRequest(): url:") + strUrl;
        strOut += String(" primary error: ")+String::FromInt32(primaryError);
        strOut += String(" certificate: ")+strCertificate;
        strOut += String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
    // Check the cached preference table before sending a message. This
    // will prevent waiting for an already available answer.
    if (Network::GetInstance(mContext)->CheckSslPrefTable(this, error)) {
        return TRUE;
    }
    // Do not post a message for a synchronous request. This will cause a
    // deadlock. Just bail on the request.
    if (IsSynchronous()) {
//        mRequestHandle->HandleSslErrorResponse(FALSE);
        return TRUE;
    }
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(error);
    SendMessageInternal(MSG_SSL_ERROR, params.Get());
    // if it has been canceled, return false so that the network thread
    // won't be blocked. If it is not canceled, save the mRequestHandle
    // so that if it is canceled when MSG_SSL_ERROR is handled, we can
    // still call handleSslErrorResponse which will call restartConnection
    // to unblock the network thread.
    if (!mCancelled) {
//        mSslErrorRequestHandle = mRequestHandle;
    }

    return !mCancelled;
}

// Handle the ssl error on the WebCore thread.
CARAPI_(void) LoadListener::HandleSslError(
    /* [in] */ ISslError* error)
{
    //assert(error != NULL);

    if (!mCancelled) {
        mSslError = error;
        Network::GetInstance(mContext)->HandleSslErrorRequest(this);
    }
//    else if (mSslErrorRequestHandle != NULL)
//    {
//        mSslErrorRequestHandle.handleSslErrorResponse(true);
//    }
//    mSslErrorRequestHandle = NULL;
}

/**
 * @return HTTP authentication realm or null if none.
 */
CARAPI_(void) LoadListener::Realm(
    /* [out] */ String* strOut)
{    
    // if (mAuthHeader == NULL) {
    //     *strOut = String(NULL);
    // } else {
    //     mAuthHeader->GetRealm(strOut);
    // }
}

/**
 * Returns true iff an HTTP authentication problem has
 * occured (credentials invalid).
 */
CARAPI_(Boolean) LoadListener::AuthCredentialsInvalid()
{    
    // if it is digest and the nonce is stale, we just
    // resubmit with a new nonce
    Boolean isDigest=FALSE;
    Boolean stale=FALSE;
    //mAuthHeader->IsDigest(&isDigest);
    //mAuthHeader->GetStale(&stale);
    return ( mAuthFailed && !(isDigest && stale) );
}

/**
 * @return The last SSL error or null if there is none
 */
CARAPI_(AutoPtr<ISslError>) LoadListener::SslError()
{
    return mSslError;
}

/**
 * Handles SSL error(s) on the way down from the user
 * (the user has already provided their feedback).
 */
CARAPI_(void) LoadListener::HandleSslErrorResponse(
    /* [in] */ Boolean proceed)
{
//    if (mRequestHandle != null) {
//        mRequestHandle.handleSslErrorResponse(proceed);
//    }
  
    if (!proceed) {
        // Commit whatever data we have and tear down the loader.
        CommitLoad();
        TearDown();
    }
}

/**
 * Uses user-supplied credentials to restart a request. If the credentials
 * are null, cancel the request.
 */
CARAPI_(void) LoadListener::HandleAuthResponse(
    /* [in] */ const String& username, 
    /* [in] */ const String& password)
{
    if (DebugFlags::sLOAD_LISTENER) {
        String strOut = String("LoadListener::HandleAuthResponse(): url:") + mUrl;
        strOut += String(" username: ")+username;
        strOut += String(" password: ")+password;
        strOut += String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
    if (username.GetLength() != 0 && password.GetLength() != 0) {
        MakeAuthResponse(username, password);
    } else {
        // Commit whatever data we have and tear down the loader.
        CommitLoad();
        TearDown();
    }
}

CARAPI_(void) LoadListener::MakeAuthResponse(
    /* [in] */ const String& username, 
    /* [in] */ const String& password)
{
    if (/*mAuthHeader == NULL || mRequestHandle == NULL*/TRUE) {
        return;
    }

    //mAuthHeader->SetUsername(username);
    //mAuthHeader->SetPassword(password);

    Int32 scheme;
    //mAuthHeader->GetScheme(&scheme);
    if (scheme == /*HttpAuthHeader_BASIC*/1) {
        // create a basic response
        Boolean isProxy = FALSE;
        //mAuthHeader->IsProxy(&isProxy);
        //mRequestHandle->SetupBasicAuthResponse(isProxy, username, password);
    } else if (scheme == /*HttpAuthHeader_DIGEST*/2) {
        // create a digest response
        Boolean isProxy = FALSE;
        //mAuthHeader->IsProxy(&isProxy);

        String realm, nonce, qop, algorithm, opaque; 
        //mAuthHeader->GetRealm(&realm);
        //mAuthHeader->GetNonce(&nonce);
        //mAuthHeader->GetQop(&qop);
        //mAuthHeader->GetAlgorithm(&algorithm);
        //mAuthHeader->GetOpaque(&opaque);

        //mRequestHandle->SetupDigestAuthResponse(isProxy, username, password, realm, nonce, qop, algorithm, opaque);
    }
}

/**
 * This is called when a request can be satisfied by the cache, however,
 * the cache result could be a redirect. In this case we need to issue
 * the network request.
 * @param method
 * @param headers
 * @param postData
 */
CARAPI_(void) LoadListener::SetRequestData(
    /* [in] */ const String& method, 
    /* [in] */ IObjectStringMap* headers, 
    /* [in] */ ArrayOf<Byte>& postData)
{
    //assert(headers != NULL);
    mMethod = method;
    mRequestHeaders = headers;
    mPostData = &postData;
}

/**
 * @return The current URL associated with this load.
 */
CARAPI_(void) LoadListener::Url(
    /* [out] */ String* strOut)
{
    *strOut = mUrl;
}

/**
 * @return The current WebAddress associated with this load.
 */
CARAPI_(AutoPtr<IWebAddress>) LoadListener::GetWebAddress()
{
    return mUri;
}

/**
 * @return URL hostname (current URL).
 */
CARAPI_(void) LoadListener::Host(
    /* [out] */ String* strOut)
{
    if (mUri != NULL) {
//        *strOut = mUri->mHost;
    }
}

/**
 * @return The original URL associated with this load.
 */
CARAPI_(void) LoadListener::OriginalUrl(
    /* [out] */ String* strOut)
{
    if (mOriginalUrl != NULL) {
        *strOut = mOriginalUrl;
    } else {
        *strOut = mUrl;
    }
}

CARAPI_(Int64) LoadListener::PostIdentifier()
{
    return mPostIdentifier;
}

CARAPI_(void) LoadListener::AttachRequestHandle(
    /* [in] */ /*RequestHandle* requestHandle*/)
{
    if (DebugFlags::sLOAD_LISTENER) {
        //JAVA:  Log.v(LOGTAG, "LoadListener.attachRequestHandle(): " + "requestHandle: " +  requestHandle);
        String strMethod;
        //requestHandle->GetMethod(&strMethod);
        Utility::Logging::Logger::V(LOGTAG, String("LoadListener::AttachRequestHandle(): RequestHandle:(HTTP request method) ")+strMethod+String("\n"));
    }
    //mRequestHandle = requestHandle;
}

CARAPI_(void) LoadListener::DetachRequestHandle()
{
    if (DebugFlags::sLOAD_LISTENER) {
        //Log.v(LOGTAG, "LoadListener.detachRequestHandle(): " + "requestHandle: " + mRequestHandle);
        String strMethod;
        //requestHandle->GetMethod(&strMethod);
        Utility::Logging::Logger::V(LOGTAG, String("LoadListener::DetachRequestHandle(): RequestHandle:(HTTP request method) ")+strMethod+String("\n"));
    }
    //mRequestHandle = NULL;
}

/*
 * This function is called from native WebCore code to
 * notify this LoadListener that the content it is currently
 * downloading should be saved to a file and not sent to
 * WebCore.
 */
CARAPI_(void) LoadListener::DownloadFile()
{    
    // remove the cache
    //JAVA:  WebViewWorker.getHandler().obtainMessage(WebViewWorker.MSG_REMOVE_CACHE, this).sendToTarget();
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(this);
    (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_REMOVE_CACHE,params);

    // Inform the client that they should download a file
    AutoPtr<ICallbackProxy> callBackProxy;
    callBackProxy=((CBrowserFrame*)(mBrowserFrame.Get()))->GetCallbackProxy();
    String strUserAgentString;
    ((CBrowserFrame*)(mBrowserFrame.Get()))->GetUserAgentString(/*&*/strUserAgentString);
    String strContentDisposition;
    //mHeaders->GetContentDisposition(&strContentDisposition);
    Boolean bCallBackProxy=FALSE;
    callBackProxy->OnDownloadStart(mUrl, 
                strUserAgentString,
                strContentDisposition, 
                mMimeType, mContentLength, &bCallBackProxy);

    // Cancel the download. We need to stop the http load.
    // The native loader object will get cleared by the call to
    // cancel() but will also be cleared on the WebCore side
    // when this function returns.
    Cancel();
}

/*
 * This function is called from native WebCore code to
 * find out if the given URL is in the cache, and if it can
 * be used. This is just for forward/back navigation to a POST
 * URL.
 */
CARAPI_(Boolean) LoadListener::WillLoadFromCache(
    /* [in] */ const String& url, 
    /* [in] */ Int64 identifier)
{
    Boolean inCache =
            CCacheManager::GetCacheFile(url, identifier, NULL) != NULL;
    
    if (DebugFlags::sLOAD_LISTENER) {
        //JAVA:  Log.v(LOGTAG, "willLoadFromCache: " + url + " in cache: " +  inCache);
        String strOut=String("willLoadFromCache: ")+url;
        strOut+=String(" in cache: ")+String::FromBoolean(inCache);
        strOut+=String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
    
    return inCache;
}

/*
 * Reset the cancel flag. This is used when we are resuming a stopped
 * download. To suspend a download, we cancel it. It can also be cancelled
 * when it has run out of disk space. In this situation, the download
 * can be resumed.
 */
CARAPI_(void) LoadListener::ResetCancel()
{
    mCancelled = FALSE;
}

CARAPI_(void) LoadListener::MimeType(
    /* [out] */ String* strOut)
{
    *strOut = mMimeType;
}

CARAPI_(void) LoadListener::TransferEncoding(
    /* [out] */ String* strOut)
{
    *strOut = mTransferEncoding;
}

/*
 * Return the size of the content being downloaded. This represents the
 * full content size, even under the situation where the download has been
 * resumed after interruption.
 *
 * @ return full content size
 */
CARAPI_(Int64) LoadListener::ContentLength() const
{
    return mContentLength;
}

// Commit the headers if the status code is not a redirect.
CARAPI_(void) LoadListener::CommitHeadersCheckRedirect()
{
    if (mCancelled) return;

    // do not call webcore if it is redirect. According to the code in
    // InspectorController::willSendRequest(), the response is only updated
    // when it is not redirect. If we received a not-modified response from
    // the server and mCacheLoader is not null, do not send the response to
    // webkit. This is just a validation response for loading from the
    // cache.
    if ((mStatusCode >= 301 && mStatusCode <= 303) || mStatusCode == 307 ||
            (mStatusCode == 304 && mCacheLoader != NULL)) {
        return;
    }

    CommitHeaders();
}

// This commits the headers without checking the response status code.
CARAPI_(void) LoadListener::CommitHeaders()
{
    if (mIsMainPageLoader /*&& sCertificateTypeMap.containsKey(mMimeType)*/)
    {
        // In the case of downloading certificate, we will save it to the
        // KeyStore in commitLoad. Do not call webcore.
        return;
    }

    // If the response is an authentication and we've resent the
    // request with some credentials then don't commit the headers
    // of this response; wait for the response to the request with the
    // credentials.
    if (/*mAuthHeader != NULL*/FALSE)
        return;

    // Commit the headers to WebCore
    Int32 nativeResponse = CreateNativeResponse();
    // The native code deletes the native response object.
    NativeReceivedResponse(nativeResponse);
}

/**
 * Create a WebCore response object so that it can be used by
 * nativeReceivedResponse or nativeRedirectedToUrl
 * @return native response pointer
 */
CARAPI_(Int32) LoadListener::CreateNativeResponse()
{
    // If WebCore sends if-modified-since, mCacheLoader is null. If 
    // CacheManager sends it, mCacheLoader is not null. In this case, if the
    // server responds with a 304, then we treat it like it was a 200 code 
    // and proceed with loading the file from the cache.
    Int32 statusCode = (mStatusCode == HTTP_NOT_MODIFIED &&
            mCacheLoader != NULL) ? HTTP_OK : mStatusCode;
    // pass content-type content-length and content-encoding
    String strUrl;
    OriginalUrl(&strUrl);
    const Int32 nativeResponse = NativeCreateResponse(
            strUrl, statusCode, mStatusText,
            mMimeType, mContentLength, mEncoding);
    
//    if (mHeaders != null) {
//        mHeaders.getHeaders(new Headers.HeaderCallback() {
//                public void header(String name, String value) {
//                    nativeSetResponseHeader(nativeResponse, name, value);
//                }
//            });
//    }

    return nativeResponse;
}

/**
 * Commit the load.  It should be ok to call repeatedly but only before
 * tearDown is called.
 */
CARAPI_(void) LoadListener::CommitLoad()
{
    if (mCancelled) return;

    if (mIsMainPageLoader) {
        String* type = NULL;
        sCertificateTypeMap->Get(mMimeType, (IInterface**)&type);
        if (type != NULL) {
            // This must be synchronized so that no more data can be added
            // after getByteSize returns.
            
            Core::Threading::Mutex::Autolock lock(mDataBuilderMutex);

            // In the case of downloading certificate, we will save it
            // to the KeyStore and stop the current loading so that it
            // will not generate a new history page
            Int32 byteSize = 0;
            mDataBuilder->GetByteSize(&byteSize);
            AutoFree<ArrayOf<Byte> > cert = ArrayOf<Byte>::Alloc(byteSize);
            Int32 offset = 0;
            while (TRUE) {
                //ByteArrayBuilder.Chunk c = mDataBuilder.getFirstChunk();
                AutoPtr<IByteArrayBuilderChunk> c;
                mDataBuilder->GetFirstChunk((IByteArrayBuilderChunk**)&c);
                if (c == NULL) break;

                // if (c.mLength != 0) {
                //     System.arraycopy(c.mArray, 0, cert, offset, c.mLength);
                //     offset += c.mLength;
                // }

                c->ChunkRelease();
            }
            CertTool::AddCertificate(mContext.Get(), (const String)(*type), *(cert.Get()) );
            mBrowserFrame->StopLoading();
            return;
        }
    }

    // Give the data to WebKit now. We don't have to synchronize on
    // mDataBuilder here because pulling each chunk removes it from the
    // internal list so it cannot be modified.
    PerfChecker* checker = new PerfChecker();
    AutoPtr<IByteArrayBuilderChunk> ic;
    CByteArrayBuilder::Chunk* c = NULL;
    while (TRUE) {
        mDataBuilder->GetFirstChunk((IByteArrayBuilderChunk**)&ic);
        if (ic == NULL) break;

        c = (CByteArrayBuilder::Chunk*)(ic.Get());

        if (c->mLength != 0) {
            NativeAddData(*c->mArray.Get(), c->mLength);
            WebViewWorker::CacheData* data = new WebViewWorker::CacheData();
            data->mListener = this;
            data->mChunk = c;
            //JAVA:  WebViewWorker.getHandler().obtainMessage(WebViewWorker.MSG_APPEND_CACHE, data).sendToTarget();
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);            
            params->WriteStruct((Handle32)data,sizeof(WebViewWorker::CacheData));
            (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_APPEND_CACHE,params);
        } else {
            c->ChunkRelease();
        }
        checker->ResponseAlert("res nativeAddData");
    }
}

/**
 * Tear down the load. Subclasses should clean up any mess because of
 * cancellation or errors during the load.
 */
CARAPI_(void) LoadListener::TearDown()
{
    if (GetErrorID() == /*OK*/0) {
        WebViewWorker::CacheSaveData* data = new WebViewWorker::CacheSaveData();
        data->mListener = this;
        data->mUrl = mUrl;
        data->mPostId = mPostIdentifier;
        //JAVA:  WebViewWorker.getHandler().obtainMessage(WebViewWorker.MSG_SAVE_CACHE, data).sendToTarget();
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);            
        params->WriteStruct((Handle32)data,sizeof(WebViewWorker::CacheSaveData));
        (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_SAVE_CACHE,params);
    } else {
        //JAVA:  WebViewWorker.getHandler().obtainMessage(WebViewWorker.MSG_REMOVE_CACHE, this).sendToTarget();
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);            
        params->WriteInterfacePtr(this);
        (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_REMOVE_CACHE,params);
    }

    if (mNativeLoader != 0) {
        PerfChecker* checker = new PerfChecker();
        NativeFinished();
        checker->ResponseAlert("res nativeFinished");
        ClearNativeLoader();
    }
}

/**
 * Helper for getting the error ID.
 * @return errorID.
 */
CARAPI_(Int32) LoadListener::GetErrorID() const
{
    return mErrorID;
}

/**
 * Return the error description.
 * @return errorDescription.
 */
CARAPI_(void) LoadListener::GetErrorDescription(
    /* [out] */ String* strOut)
{
    *strOut = mErrorDescription;
}

/**
 * Notify the loader we encountered an error.
 */
CARAPI_(void) LoadListener::NotifyError()
{
    if (mNativeLoader != 0) {
        String description;
        GetErrorDescription(&description);

        if (description.GetLength() == 0) {
            description = "";
        }

        String strOut;
        Url(&strOut);
        NativeError(GetErrorID(), description, strOut);
        ClearNativeLoader();
    }
}

/**
 * Pause the load. For example, if a plugin is unable to accept more data,
 * we pause reading from the request. Called directly from the WebCore thread.
 */
CARAPI_(void) LoadListener::PauseLoad(
    /* [in] */ Boolean pause)
{
    // if (mRequestHandle != NULL) {
    //     mRequestHandle->PauseRequest(pause);
    // }    
}

/**
 * Cancel a request.
 * FIXME: This will only work if the request has yet to be handled. This
 * is in no way guarenteed if requests are served in a separate thread.
 * It also causes major problems if cancel is called during an
 * EventHandler's method call.
 */
CARAPI_(void) LoadListener::Cancel()
{
    if (DebugFlags::sLOAD_LISTENER) {
        if (/*mRequestHandle == NULL*/FALSE) {
            //JAVA:  Log.v(LOGTAG, "LoadListener.cancel(): no requestHandle");
            String strOut=String("LoadListener::Cancel(): no requestHandle");
            strOut+=String("\n");
            Utility::Logging::Logger::V(LOGTAG, strOut);
        } else {
            //JAVA:  Log.v(LOGTAG, "LoadListener.cancel()");
            String strOut=String("LoadListener::Cancel()");
            strOut+=String("\n");
            Utility::Logging::Logger::V(LOGTAG, strOut);
        }
    }
    if (/*mRequestHandle != NULL*/FALSE) {
        // mRequestHandle->Cancel();
        // mRequestHandle = NULL;
    }

    //JAVA:  WebViewWorker.getHandler().obtainMessage(WebViewWorker.MSG_REMOVE_CACHE, this).sendToTarget();
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);            
    params->WriteInterfacePtr(this);
    (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_REMOVE_CACHE,params);
    mCancelled = TRUE;

    ClearNativeLoader();
}

/*
 * Perform the actual redirection. This involves setting up the new URL,
 * informing WebCore and then telling the Network to start loading again.
 */
CARAPI_(void) LoadListener::DoRedirect()
{
    // as cancel() can cancel the load before doRedirect() is
    // called through handleMessage, needs to check to see if we
    // are canceled before proceed
    if (mCancelled) {
        return;
    }

    // Do the same check for a redirect loop that
    //RequestHandle.setupRedirect does.
    if (mCacheRedirectCount >= /*RequestHandle_MAX_REDIRECT_COUNT*/16) {
        String strContext;
        mContext->GetString(R::string::httpErrorRedirectLoop, &strContext);
        HandleError(/*EventHandler_ERROR_REDIRECT_LOOP*/-9, strContext);
        return;
    }

    String redirectTo;// = mHeaders.getLocation();
    if (redirectTo.GetLength() != 0) {
        Int32 nativeResponse = CreateNativeResponse();
        NativeRedirectedToUrl(mUrl, redirectTo, nativeResponse, &redirectTo);
        // nativeRedirectedToUrl() may call cancel(), e.g. when redirect
        // from a https site to a http site, check mCancelled again
        if (mCancelled) {
            return;
        }
        Boolean bFlag = FALSE;

        if (redirectTo.GetLength() == 0) {
            //JAVA:  Log.d(LOGTAG, "Redirection failed for " + mHeaders.getLocation());
            String strLocation;
            //mHeaders->GetLocation(&strLocation);
            String strOut=String("Redirection failed for ") + strLocation;
            strOut+=String("\n");
            Utility::Logging::Logger::V(LOGTAG, strOut);
            Cancel();
            return;
        } else if (bFlag=CURLUtil::IsNetworkUrl(redirectTo), !bFlag) {
            const String text;// = mContext->GetString(R.string.open_permission_deny) + "\n" + redirectTo;
            ArrayOf<Byte>* textArray = ArrayOf<Byte>::Alloc((Byte*)(const char*)text, text.GetLength());
            NativeAddData(*textArray, text.GetLength());
            NativeFinished();
            ClearNativeLoader();
            return;
        }

        // Cache the redirect response
        if (GetErrorID() == /*OK*/0) {
            WebViewWorker::CacheSaveData* data = new WebViewWorker::CacheSaveData();
            data->mListener = this;
            data->mUrl = mUrl;
            data->mPostId = mPostIdentifier;
            //JAVA:  WebViewWorker.getHandler().obtainMessage(WebViewWorker.MSG_SAVE_CACHE, data).sendToTarget();
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteStruct((Handle32)data,sizeof(WebViewWorker::CacheSaveData));
            (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_SAVE_CACHE,params);
        } else {
            //JAVA:  WebViewWorker.getHandler().obtainMessage(WebViewWorker.MSG_REMOVE_CACHE, this).sendToTarget();
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInterfacePtr(this);
            (WebViewWorker::GetHandler())->SendMessage(WebViewWorker::MSG_REMOVE_CACHE,params);
        }

        // Saving a copy of the unstripped url for the response
        mOriginalUrl = redirectTo;
        // This will strip the anchor
        SetUrl(redirectTo);

        // Redirect may be in the cache
        if (mRequestHeaders == NULL) {
            CObjectStringMap::New((IObjectStringMap**)&mRequestHeaders);
        }
        Boolean fromCache = FALSE;
        if (mCacheLoader != NULL) {
            // This is a redirect from the cache loader. Increment the
            // redirect count to avoid redirect loops.
            mCacheRedirectCount++;
            fromCache = TRUE;
        }
        if (!CheckCache(mRequestHeaders)) {
            // mRequestHandle can be null when the request was satisfied
            // by the cache, and the cache returned a redirect
            if (/*mRequestHandle != null*/1) {
//                mRequestHandle.setupRedirect(mUrl, mStatusCode,
//                        mRequestHeaders);
            } else {
                // If the original request came from the cache, there is no
                // RequestHandle, we have to create a new one through
                // Network.requestURL.
                Network* network = Network::GetInstance(GetContext());
                if (!network->RequestURL(mMethod, mRequestHeaders, *mPostData.Get(), this)) {
                    // Signal a bad url error if we could not load the
                    // redirection.
                    String strContext;
                    mContext->GetString(R::string::httpErrorBadUrl, &strContext);
                    HandleError(/*EventHandler_ERROR_BAD_URL*/-12, strContext);
                    return;
                }
            }
            if (fromCache) {
                // If we are coming from a cache load, we need to transfer
                // the redirect count to the new (or old) RequestHandle to
                // keep the redirect count in sync.
//                mRequestHandle.setRedirectCount(mCacheRedirectCount);
            }
        } else if (!fromCache) {
            // Switching from network to cache means we need to grab the
            // redirect count from the RequestHandle to keep the count in
            // sync. Add 1 to account for the current redirect.
//            mCacheRedirectCount = mRequestHandle.getRedirectCount() + 1;
        }
    } else {
        CommitHeaders();
        CommitLoad();
        TearDown();
    }

    if (DebugFlags::sLOAD_LISTENER) {
        //JAVA:  Log.v(LOGTAG, "LoadListener.onRedirect(): redirect to: " + redirectTo);
        String strOut=String("LoadListener::OnRedirect(): redirect to: ") + redirectTo;
        strOut+=String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
}

/* package */ 
CARAPI_(void) LoadListener::ParseContentTypeHeader(
    /* [in] */ const String& contentType)
{
    if (DebugFlags::sLOAD_LISTENER) {
        //JAVA:  Log.v(LOGTAG, "LoadListener.parseContentTypeHeader: " + "contentType: " + contentType);
        String strOut=String("LoadListener::ParseContentTypeHeader: contentType: ") + contentType;
        strOut+=String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }

    if (contentType.GetLength() != 0) {
        Int32 i = contentType.IndexOf(';');
        if (i >= 0) {
            mMimeType = contentType.Substring(0, i);

            Int32 j = contentType.IndexOf('=', i);
            if (j > 0) {
                i = contentType.IndexOf(';', j);
                if (i < j) {
                    i = contentType.GetLength();
                }
                mEncoding = contentType.Substring(j + 1, i);
            } else {
                mEncoding = contentType.Substring(i + 1);
            }
            // Trim excess whitespace.
            /*mEncoding = */mEncoding.Trim().ToLowerCase();

            if (i < contentType.GetLength() - 1) {
                // for data: uri the mimeType and encoding have
                // the form image/jpeg;base64 or text/plain;charset=utf-8
                // or text/html;charset=utf-8;base64
                contentType.Substring(i + 1).Trim().ToLowerCase();
                mTransferEncoding = contentType;
            }
        } else {
            mMimeType = contentType;
        }

        // Trim leading and trailing whitespace
        mMimeType = mMimeType.Trim();
#if 0
        try {
            Matcher m = CONTENT_TYPE_PATTERN.matcher(mMimeType);
            if (m.find()) {
                mMimeType = m.group(1);
            } else {
                guessMimeType();
            }
        } catch (IllegalStateException ex) {
            guessMimeType();
        }
#endif        
    }
    // Ensure mMimeType is lower case.
    /*mMimeType = */mMimeType.ToLowerCase();
}

/**
 * @return The HTTP-authentication object or null if there
 * is no supported scheme in the header.
 * If there are several valid schemes present, we pick the
 * strongest one. If there are several schemes of the same
 * strength, we pick the one that comes first.
 */
ECode LoadListener::ParseAuthHeader(
        /* [in] */ String header//,
        /* [out] */ /*IHttpAuthHeader** httpAuthHeader*/)
{
    if (header.IsNull()) {
        Int32 posMax = 256;
        Int32 posLen = 0;
        //JAVA:  int[] pos = new int [posMax];
        AutoFree < ArrayOf < Int32 > > pos;
        ArrayOf<Int32>::Free(pos.Get());
        pos = ArrayOf<Int32>::Alloc(posMax);

        Int32 headerLen = header.GetLength();
        if (headerLen > 0) {
            // first, we find all unquoted instances of 'Basic' and 'Digest'
            Boolean quoted = FALSE;
            for (Int32 i = 0; i < headerLen && posLen < posMax; ++i) {
                if (header.Substring(i,1) == String("\"")) {
                    quoted = !quoted;
                } else {
                    if (!quoted) {
                        if (/*header.regionMatches(TRUE, i,
                                    HttpAuthHeader_BASIC_TOKEN, 0,
                                    HttpAuthHeader_BASIC_TOKEN.GetLength())*/FALSE) {
                            (*pos)[posLen++] = i;
                            continue;
                        }

                        if (/*header.regionMatches(TRUE, i,
                                    HttpAuthHeader_DIGEST_TOKEN, 0,
                                    HttpAuthHeader_DIGEST_TOKEN.GetLength())*/FALSE) {
                            (*pos)[posLen++] = i;
                            continue;
                        }
                    }
                }
            }
        }

        if (posLen > 0) {
            // consider all digest schemes first (if any)
            for (Int32 i = 0; i < posLen; i++) {
                if (/*header.regionMatches(TRUE, (*pos)[i],
                                HttpAuthHeader_DIGEST_TOKEN, 0,
                                HttpAuthHeader_DIGEST_TOKEN.GetLength())*/FALSE) {
                    String sub = header.Substring((*pos)[i],
                                (i + 1 < posLen ? (*pos)[i + 1] : headerLen));

                    // AutoPtr<IHttpAuthHeader> rval;
                    // CHttpAuthHeader::New((IHttpAuthHeader**)&rval); 
                    // if (rval->IsSupportedScheme()) {
                    //     // take the first match
                    //     *httpAuthHeader = rval;
                    // }
                }
            }

            // ...then consider all basic schemes (if any)
            for (Int32 i = 0; i < posLen; i++) {
                if (/*header.regionMatches(true, (*pos)[i],
                                HttpAuthHeader_BASIC_TOKEN, 0,
                                HttpAuthHeader_BASIC_TOKEN.GetLength())*/FALSE) {
                    String sub = header.Substring((*pos)[i],
                                (i + 1 < posLen ? (*pos)[i + 1] : headerLen));

                    // AutoPtr<IHttpAuthHeader> rval;
                    // CHttpAuthHeader::New((IHttpAuthHeader**)&rval);
                    // if (rval->IsSupportedScheme()) {
                    //     // take the first match
                    //     *httpAuthHeader = rval;
                    // }
                }
            }
        }
    }
    //*httpAuthHeader = NULL;
    return NOERROR;
}

/**
 * If the content is a redirect or not modified we should not send
 * any data into WebCore as that will cause it create a document with
 * the data, then when we try to provide the real content, it will assert.
 *
 * @return True iff the callback should be ignored.
 */
CARAPI_(Boolean) LoadListener::IgnoreCallbacks()
{
    return (mCancelled || /*mAuthHeader != NULL ||*/
                // Allow 305 (Use Proxy) to call through.
                (mStatusCode > 300 && mStatusCode < 400 && mStatusCode != 305));
}

/**
 * Sets the current URL associated with this load.
 */
CARAPI_(void) LoadListener::SetUrl(
    /* [in] */ const String& url)
{
    if (url.GetLength() != 0) {
        mUri = NULL;
        Boolean bFlag = FALSE;
        bFlag = CURLUtil::IsNetworkUrl(url);
        if (bFlag) {
            mUrl = CURLUtil::StripAnchor(url);
//            mUri = new WebAddress(mUrl);
        } else {
            mUrl = url;
        }
    }
}

/**
 * Guesses MIME type if one was not specified. Defaults to 'text/html'. In
 * addition, tries to guess the MIME type based on the extension.
 *
 */
CARAPI_(void) LoadListener::GuessMimeType()
{
    // Data urls must have a valid mime type or a blank string for the mime
    // type (implying text/plain).
    Boolean bFlag = FALSE;
    bFlag = CURLUtil::IsDataUrl(mUrl);
    if (bFlag && mMimeType.GetLength() != 0) {
        Cancel();
        String text;
        mContext->GetString(R::string::httpErrorBadUrl, &text);
        HandleError(/*EventHandler_ERROR_BAD_URL*/-12, text);
    } else {
        // Note: This is ok because this is used only for the main content
        // of frames. If no content-type was specified, it is fine to
        // default to text/html.
        mMimeType = "text/html";
        String newMimeType;
        GuessMimeTypeFromExtension(mUrl, &newMimeType);
        if (newMimeType.GetLength() != 0) {
            mMimeType = newMimeType;
        }
    }
}

/**
 * guess MIME type based on the file extension.
 */
CARAPI_(void) LoadListener::GuessMimeTypeFromExtension(
    /* [in] */ const String& url,
    /* [out] */ String* strOut)
{
    assert(strOut!=NULL);
    // PENDING: need to normalize url
    if (DebugFlags::sLOAD_LISTENER) {
        //JAVA:  Log.v(LOGTAG, "guessMimeTypeFromExtension: url = " + url);
        String strOut=String("GuessMimeTypeFromExtension: url = ") + url;
        strOut+=String("\n");
        Utility::Logging::Logger::V(LOGTAG, strOut);
    }
    String strFileExtensionFromUrl;
    //strFileExtensionFromUrl = CMimeTypeMap::GetFileExtensionFromUrl(url);
    //CMimeTypeMap::GetSingleton()->GetMimeTypeFromExtension(strFileExtensionFromUrl,strOut);
}

/**
 * Either send a message to ourselves or queue the message if this is a
 * synchronous load.
 */
CARAPI_(void) LoadListener::SendMessageInternal(
    /* [in] */ Int32 message,
    /* [in] */ IParcel *params)
{
    if (mSynchronous) {
        AutoPtr<Msg> msg = new Msg();
        msg->message = message;
        msg->params = params;
        mMessageQueue.PushBack(msg);
    } else {
        SendMessage(message, params);
    }
}

/**
 * Cycle through our messages for synchronous loads.
 */
/* package */ 
CARAPI_(void) LoadListener::LoadSynchronousMessages()
{    
    if (DebugFlags::sLOAD_LISTENER && !mSynchronous) {
        //return E_ASSERTION_ERROR;
    }
    // Note: this can be called twice if it is a synchronous network load,
    // and there is a cache, but it needs to go to network to validate. If 
    // validation succeed, the CacheLoader is used so this is first called 
    // from http thread. Then it is called again from WebViewCore thread 
    // after the load is completed. So make sure the queue is cleared but
    // don't set it to null.
    while (!mMessageQueue.IsEmpty()) {
        HandleMessage((mMessageQueue.At(0)).Get());
        mMessageQueue.Erase(mMessageQueue.Begin());
    }
}

//=========================================================================
// native functions
//=========================================================================

/**
 * Create a new native response object.
 * @param url The url of the resource.
 * @param statusCode The HTTP status code.
 * @param statusText The HTTP status text.
 * @param mimeType HTTP content-type.
 * @param expectedLength An estimate of the content length or the length
 *                       given by the server.
 * @param encoding HTTP encoding.
 * @return The native response pointer.
 */
CARAPI_(Int32) LoadListener::NativeCreateResponse(
    /* [in] */ const String& url, 
    /* [in] */ Int32 statusCode,
    /* [in] */ const String& statusText, 
    /* [in] */ const String& mimeType, 
    /* [in] */ Int64 expectedLength,
    /* [in] */ const String& encoding)
{
    return -1;
}

/**
 * Add a response header to the native object.
 * @param nativeResponse The native pointer.
 * @param key String key.
 * @param val String value.
 */
CARAPI_(void) LoadListener::NativeSetResponseHeader(
    /* [in] */ Int32 nativeResponse, 
    /* [in] */ const String& key,
    /* [in] */ const String& val)
{}

/**
 * Dispatch the response.
 * @param nativeResponse The native pointer.
 */
CARAPI_(void) LoadListener::NativeReceivedResponse(
    /* [in] */ Int32 nativeResponse)
{}

/**
 * Add data to the loader.
 * @param data Byte array of data.
 * @param length Number of objects in data.
 */
CARAPI_(void) LoadListener::NativeAddData(
    /* [in] */ const ArrayOf<Byte>& data, 
    /* [in] */ Int32 length)
{}

/**
 * Tell the loader it has finished.
 */
CARAPI_(void) LoadListener::NativeFinished()
{}

/**
 * tell the loader to redirect
 * @param baseUrl The base url.
 * @param redirectTo The url to redirect to.
 * @param nativeResponse The native pointer.
 * @return The new url that the resource redirected to.
 */
CARAPI_(void) LoadListener::NativeRedirectedToUrl(
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& redirectTo, 
    /* [in] */ Int32 nativeResponse,
    /* [out] */ String* str)
{}

/**
 * Tell the loader there is error
 * @param id
 * @param desc
 * @param failingUrl The url that failed.
 */
CARAPI_(void) LoadListener::NativeError(
    /* [in] */ Int32 id, 
    /* [in] */ const String& desc, 
    /* [in] */ const String& failingUrl)
{}


PInterface LoadListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IApartment*)this;
    }
    else if (riid == EIID_IApartment) {
        return (IApartment*)this;
    }
    return NULL;
}

UInt32 LoadListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LoadListener::Release()
{
    return ElRefBase::Release();
}

ECode LoadListener::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IApartment*)this) {
        *iID = EIID_IApartment;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode LoadListener::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LoadListener::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode LoadListener::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode LoadListener::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode LoadListener::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode LoadListener::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode LoadListener::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode LoadListener::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode LoadListener::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode LoadListener::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode LoadListener::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{    
    if (message == MSG_CONTENT_HEADERS) {
        void (STDCALL LoadListener::*pHandlerFunc)(IHeaders*);
        pHandlerFunc = &LoadListener::HandleMsgContentHeaders;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_CONTENT_DATA) {
        void (STDCALL LoadListener::*pHandlerFunc)();
        pHandlerFunc = &LoadListener::HandleMsgContentData;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_CONTENT_FINISHED) {
        void (STDCALL LoadListener::*pHandlerFunc)();
        pHandlerFunc = &LoadListener::HandleMsgContentFinished;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_CONTENT_ERROR) {
        void (STDCALL LoadListener::*pHandlerFunc)(Int32, String);
        pHandlerFunc = &LoadListener::HandleMsgContentError;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_LOCATION_CHANGED) {
        void (STDCALL LoadListener::*pHandlerFunc)();
        pHandlerFunc = &LoadListener::HandleMsgLocationChanged;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_LOCATION_CHANGED_REQUEST) {
        void (STDCALL LoadListener::*pHandlerFunc)();
        pHandlerFunc = &LoadListener::HandleMsgLocationChangedRequest;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_STATUS) {
        void (STDCALL LoadListener::*pHandlerFunc)(IObjectStringMap*);
        pHandlerFunc = &LoadListener::HandleMsgStatus;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_SSL_CERTIFICATE) {
        void (STDCALL LoadListener::*pHandlerFunc)(ISslCertificate*);
        pHandlerFunc = &LoadListener::HandleMsgSslCertificate;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    if (message == MSG_SSL_ERROR) {
        void (STDCALL LoadListener::*pHandlerFunc)(ISslError*);
        pHandlerFunc = &LoadListener::HandleMsgSslError;
        return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

/*
 * This message handler is to facilitate communication between the network
 * thread and the browser thread.
 */
//LoadListener::HandleMessage
CARAPI_(void) LoadListener::HandleMessage(
    /* [in] */ Msg* msg)
{
    Int32 message = msg->message;
    if (message == MSG_CONTENT_HEADERS) {
        AutoPtr<IHeaders> obj;
        msg->params->ReadInterfacePtr((Handle32*)((IHeaders**)&obj));
        HandleMsgContentHeaders(obj.Get());
    }
    
    if (message == MSG_CONTENT_DATA) {
        HandleMsgContentData();
    }
    if (message == MSG_CONTENT_FINISHED) {
        HandleMsgContentFinished();
    }
    if (message == MSG_CONTENT_ERROR) {
        Int32 arg1 = 0;
        msg->params->ReadInt32(&arg1);
        String obj;
        msg->params->ReadString(&obj);
        HandleMsgContentError(arg1, obj);
    }
    if (message == MSG_LOCATION_CHANGED) {
        HandleMsgLocationChanged();
    }
    if (message == MSG_LOCATION_CHANGED_REQUEST) {
        HandleMsgLocationChangedRequest();
    }
    if (message == MSG_STATUS) {
        AutoPtr<IObjectStringMap> obj;
        msg->params->ReadInterfacePtr((Handle32*)((IObjectStringMap**)&obj));
        HandleMsgStatus(obj.Get());
    }
    if (message == MSG_SSL_CERTIFICATE) {        
        AutoPtr<ISslCertificate> obj;
        msg->params->ReadInterfacePtr((Handle32*)((ISslCertificate**)&obj));
        HandleMsgSslCertificate(obj.Get());
    }
    if (message == MSG_SSL_ERROR) {
        AutoPtr<ISslError> obj;
        msg->params->ReadInterfacePtr((Handle32*)((ISslError**)&obj));
        HandleMsgSslError(obj.Get());
    }    
    return ;//E_ILLEGAL_ARGUMENT_EXCEPTION;
}