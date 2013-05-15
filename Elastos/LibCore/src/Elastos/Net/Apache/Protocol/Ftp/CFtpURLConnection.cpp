
#include "CFtpURLConnection.h"
#include <elastos/List.h>
//#include "CProxySelector.h"
//#include "ProxySelector.h"
#include "CInetSocketAddress.h"

Int32 CFtpURLConnection::FTP_PORT = 21;
// FTP Reply Constants
Int32 CFtpURLConnection::FTP_DATAOPEN = 125;
Int32 CFtpURLConnection::FTP_OPENDATA = 150;
Int32 CFtpURLConnection::FTP_OK = 200;
Int32 CFtpURLConnection::FTP_USERREADY = 220;
Int32 CFtpURLConnection::FTP_TRANSFEROK = 226;
//    Int32 FTP_PASV = 227;
Int32 CFtpURLConnection::FTP_LOGGEDIN = 230;
Int32 CFtpURLConnection::FTP_FILEOK = 250;
Int32 CFtpURLConnection::FTP_PASWD = 331;
//    Int32 FTP_DATAERROR = 451;
//    Int32 FTP_ERROR = 500;
Int32 CFtpURLConnection::FTP_NOTFOUND = 550;

CFtpURLConnection::CFtpURLConnection()
    : mUsername("anonymous"), mPassword("")
{
}

ECode CFtpURLConnection::Connect()
{
    // TODO: Add your code here
        // Use system-wide ProxySelect to select proxy list,
        // then try to connect via elements in the proxy list.
    List<IProxy*> *proxyList = NULL;
    if (mProxy != NULL) {
        proxyList = new List<IProxy*>(1);
//        proxyList->Add(mProxy);
    } else {
        assert(0);
//        IProxySelector* selector = ProxySelector.getDefault();
//        if (selector != NULL) {
//            proxyList = selector.select(uri);
//        }
    }
    if (NULL == proxyList) {
        mCurrentProxy = NULL;
        ConnectInternal();
    } else {
//        AutoPtr<IProxySelector> selector;
//        ProxySelector::GetDefault((IProxySelector**)&selector);
        //List::Iterator<Proxy> iter = proxyList.iterator();
        List<IProxy*>::Iterator iter = proxyList->Begin();

        Boolean connectOK = FALSE;
        String failureReason("");

        while(iter != proxyList->End() && !connectOK) {
        }
//        while (iter.hasNext() && !connectOK) {
//            currentProxy = iter.next();
//            try {
//                connectInternal();
//                connectOK = true;
//            } catch (IOException ioe) {
//                failureReason = ioe.getLocalizedMessage();
//                // If connect failed, callback "connectFailed"
//                // should be invoked.
//                if (null != selector && Proxy.NO_PROXY != currentProxy) {
//                    selector.connectFailed(uri, currentProxy.address(), ioe);
//                }
//            }
//        }
//        if (!connectOK) {
//            throw new IOException("Unable to connect to server: " + failureReason);
//        }
    }
}

ECode CFtpURLConnection::GetAllowUserInteraction(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetContent(
    /* [out] */ IInterface ** ppContent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetContentEncoding(
    /* [out] */ String * pEncoding)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetContentLength(
    /* [out] */ Int32 * pLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetContentType(
    /* [out] */ String * pType)
{
    // TODO: Add your code here
    String file;
    mUrl->GetFile(&file);
    String result = GuessContentTypeFromName(file);
    if (result.IsNull()) {
        *pType = String("content/unknown");
    } else {
        *pType = result;
    }
    return NOERROR;
}

ECode CFtpURLConnection::GetDate(
    /* [out] */ Int64 * pDate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetDefaultUseCaches(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetDoInput(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetDoOutput(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetExpiration(
    /* [out] */ Int64 * pExpiration)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetHeaderFieldByPosition(
    /* [in] */ Int32 pos,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::AddRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetHeaderFieldByKey(
    /* [in] */ const String& key,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetHeaderFieldDate(
    /* [in] */ const String& field,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetHeaderFieldInt32(
    /* [in] */ const String& field,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetHeaderFieldKey(
    /* [in] */ Int32 posn,
    /* [out] */ String * pKey)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetIfModifiedSince(
    /* [out] */ Int64 * pTime)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetInputStream(
    /* [out] */ IInputStream ** ppIs)
{
    // TODO: Add your code here
    if (!mConnected) {
        Connect();
    }
    *ppIs = mInputStream;
    return NOERROR;
}

ECode CFtpURLConnection::GetLastModified(
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetOutputStream(
    /* [out] */ IOutputStream ** ppOs)
{
    // TODO: Add your code here
    if (!mConnected) {
        Connect();
    }

    return mDataSocket->GetOutputStream(ppOs);
}

ECode CFtpURLConnection::GetRequestProperty(
    /* [in] */ const String& field,
    /* [out] */ String * pProperty)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetURL(
    /* [out] */ IURL ** ppUrl)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetUseCaches(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::SetAllowUserInteraction(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::SetDefaultUseCaches(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::SetDoInput(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    if (mConnected) {
        return E_IO_EXCEPTION;

    }
    mDoInput = newValue;
    mDoOutput = !newValue;
    return NOERROR;
}

ECode CFtpURLConnection::SetDoOutput(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    if (mConnected) {
        return E_IO_EXCEPTION;
    }

    mDoOutput = newValue;
    mDoInput = !newValue;
    return NOERROR;
}

ECode CFtpURLConnection::SetIfModifiedSince(
    /* [in] */ Int64 newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::SetRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::SetUseCaches(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::SetConnectTimeout(
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetConnectTimeout(
    /* [out] */ Int32 * pTimeout)
{
    // TODO: Add your code here
    return URLConnection::GetConnectTimeout(pTimeout);
}

ECode CFtpURLConnection::SetReadTimeout(
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::GetReadTimeout(
    /* [out] */ Int32 * pTimeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFtpURLConnection::constructor(
    /* [in] */ IURL * pUrl)
{
    // TODO: Add your code here
    URLConnection::Init(pUrl);

    pUrl->GetHost(&mHostName);

    String parse;
    pUrl->GetUserInfo(&parse);
    if (!parse.IsNull()) {
        Int32 split = parse.IndexOf(':');
        if (split >= 0) {
            mUsername = parse.Substring(0, split);
            mPassword = parse.Substring(split + 1);
        } else {
            mUsername = parse;
        }
    }

    mUri = NULL;
    ECode ec = NOERROR;
    ec = pUrl->ToURI(&mUri);
    if (FAILED(ec)) {
        return ec;
    }

    return NOERROR;
}

ECode CFtpURLConnection::constructor(
    /* [in] */ IURL * pUrl,
    /* [in] */ IProxy * pProxy)
{
    // TODO: Add your code here
    constructor(pUrl);
    mProxy = pProxy;
}

ECode CFtpURLConnection::CD()
{
    Int32 idx;

    String file;
    mUrl->GetFile(&file);
    idx = file.LastIndexOf('/');

    if (idx > 0) {
        String dir;
        file.Substring(0, idx);
        StringBuffer buf;
        buf += "CWD";
        buf += dir;
        buf += "\r\n";
        Write(buf.ToString());

        Int32 reply = GetReply();
        if (reply != FTP_FILEOK && dir.GetLength() > 0 && (char)dir.GetChar(0) == '/') {
            StringBuffer buf1;
            buf1 += "CWD";
            buf1 += dir.Substring(1);
            buf1 += "\r\n";
            Write(buf1.ToString());
            reply = GetReply();
        }

        if (reply != FTP_FILEOK) {
            return E_IO_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode CFtpURLConnection::ConnectInternal()
{
    Int32 port;
    mUrl->GetPort(&port);

    Int32 connectTimeout;
    GetConnectTimeout(&connectTimeout);
    if (port <= 0) {
        port = FTP_PORT;
    }

    if (NULL == mCurrentProxy) {
        //controlSocket = new Socket();
        CSocket::New(&mControlSocket);
    } else {
        CSocket::New(mCurrentProxy, &mControlSocket);
    }

    IInetSocketAddress *addr;
    CInetSocketAddress::New(mHostName, port, &addr);

    //mControlSocket->Connect(addr, connectTimeout);
    mConnected = true;

    mControlSocket->GetOutputStream(&mCtrlOutput);
    mControlSocket->GetInputStream(&mCtrlInput);

    Login();
    SetType();
    Boolean doin;
    if (GetDoInput(&doin), !doin) {
        CD();
    }

    CServerSocket::New(0,&mAcceptSocket);
    mAcceptSocket->GetLocalPort(&mDataPort);
    /* Cannot set REUSEADDR so we need to send a PORT command */
    Port();

    if (connectTimeout == 0) {
        // set timeout rather than zero as before
        connectTimeout = 3000;
    }

    Int32 to;
    GetConnectTimeout(&to);
    mAcceptSocket->SetSoTimeout(to);

    if (GetDoInput(&doin), doin) {
        GetFile();
    } else {
        SendFile();
    }

    mAcceptSocket->Accept(&mDataSocket);
    Int32 timeout;
    GetReadTimeout(&timeout);
    mDataSocket->SetSoTimeout(timeout);
    mAcceptSocket->Close();


    if (GetDoInput(&doin), doin) {
        IBufferedInputStream* bin;
        IInputStream* in;
        mDataSocket->GetInputStream((IInputStream**)&in);
        CBufferedInputStream::New(in, (IBufferedInputStream**)&bin);
        CFtpURLInputStream::New(bin, mControlSocket, (IFtpURLInputStream**)&mInputStream);
    }

    return NOERROR;
}

ECode CFtpURLConnection::GetFile()
{
    Int32 reply;
    String file;
    mUrl->GetFile(&file);

    StringBuffer buf;
    buf += "RETR";
    buf += file;
    buf += "\r\n";
    Write(buf.ToString());

    reply = GetReply();
    if (reply == FTP_NOTFOUND && file.GetLength() > 0 && (char)file.GetChar(0) == '/') {
        StringBuffer buf1;
        buf1 += "RETR";
        buf1 += file;
        buf1 += "\r\n";
        Write(buf1.ToString());
        reply = GetReply();
    }
    if (!(reply == FTP_OPENDATA || reply == FTP_TRANSFEROK)) {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    return NOERROR;
}


Int32 CFtpURLConnection::GetReply()
{
    ArrayOf<Byte> *code = ArrayOf<Byte>::Alloc(3);

    for (Int32 i = 0; i < code->GetLength(); i++) {
        Int32 tmp;
        mCtrlInput->Read(&tmp);
        if (tmp == -1) {
            return E_EOF_EXCEPTION;
        }

        (*code)[i] = (Byte) tmp;
    }

    mReplyCode = String((const char *)code, code->GetLength());

    Boolean multiline = FALSE;
    Int32 value;
    mCtrlInput->Read(&value);
    if (value == '-') {
        multiline = TRUE;
    }

    ReadLine(); /* Skip the rest of the first line */

    if (multiline) {
        while (ReadMultiLine()) {/* Read all of a multiline reply */
        }
    }

//    try {
//        return Integer.parseInt(replyCode);
//    } catch (NumberFormatException e) {
//        throw (IOException)(new IOException("reply code is invalid").initCause(e));
//    }
}

ECode CFtpURLConnection::Login()
{
    Int32 reply;
    reply = GetReply();
    if (reply == FTP_USERREADY) {
    } else {
        return E_IO_EXCEPTION;
    }

    StringBuffer buf;
    buf += "USER ";
    buf += mUsername;
    buf += "\r\n";
    Write(buf.ToString());

    reply = GetReply();
    if (reply == FTP_PASWD || reply == FTP_LOGGEDIN) {
    } else {
        return E_IO_EXCEPTION;
    }

    StringBuffer buf1;
    if (reply == FTP_PASWD) {
        buf1 += "PASS ";
        buf1 += mPassword;
        buf1 += "\r\n";
        Write(buf1.ToString());
        reply = GetReply();
        if (!(reply == FTP_OK || reply == FTP_USERREADY || reply == FTP_LOGGEDIN)) {
            return E_IO_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode CFtpURLConnection::Port()
{
    StringBuffer buf;
    buf += "PORT ";
    AutoPtr<IInet4Address> addr;
    mControlSocket->GetLocalAddress((IInetAddress**)&addr);
    String hostAddr;
    addr->GetHostAddress(&hostAddr);
    hostAddr.Replace(".", ",");
    buf += hostAddr;
    buf += ',';
    buf += (Int32)(mDataPort >> 8);
    buf += ",";
    buf += (Int32)(mDataPort & 255);
    buf += "\r\n";
//    write("PORT "
//            + controlSocket.getLocalAddress().getHostAddress().replace('.',
//                    ',') + ',' + (dataPort >> 8) + ','
//            + (dataPort & 255)
//            + "\r\n");
    Write(buf.ToString());

    if (GetReply() != FTP_OK) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

String CFtpURLConnection::ReadLine()
{
    StringBuffer buf;

    int c;
    mCtrlInput->Read(&c);
    while ( c != '\n' ) {
        buf += (char) c;
        mCtrlInput->Read(&c);
    }
    return buf.ToString();
}

Boolean CFtpURLConnection::ReadMultiLine()
{
    String line = ReadLine();
    if (line.GetLength() < 4) {
        return TRUE;
    }
    if (line.Substring(0, 3).Equals(mReplyCode)
            && ((char)line.GetChar(3) == (char) 32)) {
        return FALSE;
    }
    return TRUE;
}

ECode CFtpURLConnection::SendFile()
{
    Int32 reply;
    StringBuffer buf;
    String file;
    mUrl->GetFile(&file);
    Int32 index;
    file.LastIndexOf('/');
    String name = file.Substring(index + 1, file.GetLength() -index -1);
    buf += "STOR ";
    buf += name;
    buf += "\r\n";

    Write(buf.ToString());

    reply = GetReply();
    if (!(reply == FTP_OPENDATA || reply == FTP_OK || reply == FTP_DATAOPEN)) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CFtpURLConnection::SetType()
{
    Write(String("TYPE I\r\n"));
    if (GetReply() != FTP_OK) {
        return E_IO_EXCEPTION;
    }
}

ECode CFtpURLConnection::Write(
    /* [in] */ const String& command)
{
    return mCtrlOutput->Write(command.GetBytes());
}

CARAPI_(PInterface) CFtpURLConnection::Probe(
    /* [in]  */ REIID riid)
{
}