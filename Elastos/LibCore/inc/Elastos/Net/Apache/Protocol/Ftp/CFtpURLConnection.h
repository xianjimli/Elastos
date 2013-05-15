
#ifndef __CFTPURLCONNECTION_H__
#define __CFTPURLCONNECTION_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif


#include "_CFtpURLConnection.h"
#include "Elastos.IO.h"
#include "URLConnection.h"
#include "CSocket.h"
#include "CInet4Address.h"
//#include "CInetAddress.h"
#include "CServerSocket.h"
#include "CURI.h"
#include "CProxy.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>
#include "CFtpURLInputStream.h"

using namespace Elastos;
using namespace Elastos::Core::Threading;

CarClass(CFtpURLConnection), public URLConnection
{
public:
    CFtpURLConnection();

public:
    CARAPI Connect();

    CARAPI GetAllowUserInteraction(
        /* [out] */ Boolean * pValue);

    CARAPI GetContent(
        /* [out] */ IInterface ** ppContent);

    CARAPI GetContentEncoding(
        /* [out] */ String * pEncoding);

    CARAPI GetContentLength(
        /* [out] */ Int32 * pLength);

    CARAPI GetContentType(
        /* [out] */ String * pType);

    CARAPI GetDate(
        /* [out] */ Int64 * pDate);

    CARAPI GetDefaultUseCaches(
        /* [out] */ Boolean * pValue);

    CARAPI GetDoInput(
        /* [out] */ Boolean * pAllowed);

    CARAPI GetDoOutput(
        /* [out] */ Boolean * pAllowed);

    CARAPI GetExpiration(
        /* [out] */ Int64 * pExpiration);

    CARAPI GetHeaderFieldByPosition(
        /* [in] */ Int32 pos,
        /* [out] */ String * pValue);

    CARAPI AddRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    CARAPI GetHeaderFieldByKey(
        /* [in] */ const String& key,
        /* [out] */ String * pValue);

    CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64 * pValue);

    CARAPI GetHeaderFieldInt32(
        /* [in] */ const String& field,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * pValue);

    CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 posn,
        /* [out] */ String * pKey);

    CARAPI GetIfModifiedSince(
        /* [out] */ Int64 * pTime);

    CARAPI GetInputStream(
        /* [out] */ IInputStream ** ppIs);

    CARAPI GetLastModified(
        /* [out] */ Int64 * pValue);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream ** ppOs);

    CARAPI GetRequestProperty(
        /* [in] */ const String& field,
        /* [out] */ String * pProperty);

    CARAPI GetURL(
        /* [out] */ IURL ** ppUrl);

    CARAPI GetUseCaches(
        /* [out] */ Boolean * pAllowed);

    CARAPI SetAllowUserInteraction(
        /* [in] */ Boolean newValue);

    CARAPI SetDefaultUseCaches(
        /* [in] */ Boolean newValue);

    CARAPI SetDoInput(
        /* [in] */ Boolean newValue);

    CARAPI SetDoOutput(
        /* [in] */ Boolean newValue);

    CARAPI SetIfModifiedSince(
        /* [in] */ Int64 newValue);

    CARAPI SetRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    CARAPI SetUseCaches(
        /* [in] */ Boolean newValue);

    CARAPI SetConnectTimeout(
        /* [in] */ Int32 timeout);

    CARAPI GetConnectTimeout(
        /* [out] */ Int32 * pTimeout);

    CARAPI SetReadTimeout(
        /* [in] */ Int32 timeout);

    CARAPI GetReadTimeout(
        /* [out] */ Int32 * pTimeout);

    CARAPI constructor(
        /* [in] */ IURL * pUrl);

    CARAPI constructor(
        /* [in] */ IURL * pUrl,
        /* [in] */ IProxy * pProxy);

    CARAPI_(PInterface) Probe(
    /* [in]  */ REIID riid);

private:
    ECode CD();

    ECode ConnectInternal();

    ECode GetFile();

    Int32 GetReply();

    ECode Login();

    ECode Port();

    String ReadLine();

    Boolean ReadMultiLine();

    ECode SendFile();

    ECode SetType();

    ECode Write(
        /* [in] */ const String& command);

private:
    // TODO: Add your private member variables here.
     static Int32 FTP_PORT;
    // FTP Reply Constants
     static  Int32 FTP_DATAOPEN;
     static  Int32 FTP_OPENDATA;
     static  Int32 FTP_OK;
     static  Int32 FTP_USERREADY;
     static  Int32 FTP_TRANSFEROK;
    //  static  Int32 FTP_PASV = 227;
     static  Int32 FTP_LOGGEDIN;
     static  Int32 FTP_FILEOK;
     static  Int32 FTP_PASWD;
    //  static  Int32 FTP_DATAERROR = 451;
    //  static  Int32 FTP_ERROR = 500;
     static  Int32 FTP_NOTFOUND;

    ISocket* mControlSocket;

    ISocket* mDataSocket;

    IServerSocket* mAcceptSocket;

    IInputStream* mCtrlInput;

    IInputStream* mInputStream;

    IOutputStream* mCtrlOutput;

    Int32 mDataPort;

//    String username = "anonymous";
//    private String password = "";
    String mUsername;
    String mPassword;

    String mReplyCode;

    String mHostName;

    IProxy* mProxy;

    IProxy* mCurrentProxy;

    IURI* mUri;
};

#endif // __CFTPURLCONNECTION_H__
