#ifndef __PLAINSOCKETIMPL_H__
#define __PLAINSOCKETIMPL_H__

#include "SocketImpl.h"
#include "Elastos.IO.h"
#include "Socks4Message.h"
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class PlainSocketImpl : public SocketImpl
{
    friend class CSocketInputStream;
    friend class CSocketOutputStream;
public:
    PlainSocketImpl(
        /* [in] */ IFileDescriptor* fd);

    PlainSocketImpl(
        /* [in] */ IProxy* proxy);

    PlainSocketImpl();

    PlainSocketImpl(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 localport,
        /* [in] */ IInetAddress* addr,
        /* [in] */ Int32 port);

    ECode InitLocalPort(
        /* [in] */ Int32 localPort);

    ECode InitRemoteAddressAndPort(
        /* [in] */ IInetAddress* remoteAddress,
        /* [in] */ Int32 remotePort);

    //    ECode GetOption(int optID);


    ECode SetOption(
        /* [in] */ Int32 optID,
        /* [in] */ Boolean val);

    ECode SocksAccept();

    ECode GetFD(
        /* [out] */ IFileDescriptor** ppFd);

    ECode Read(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* value);

    ECode Write(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* value);

protected:
    ECode Accept(
        /* [in] */ ISocketImpl* newImpl);

    ECode Available(
        /* [out] */ Int32* value);

    ECode Bind(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    ECode Close();

    ECode Connect(
        /* [in] */ const String& aHost,
        /* [in] */ Int32 aPort);

    ECode Connect(
        /* [in] */ IInetAddress* anAddr,
        /* [in] */ Int32 aPort);

    ECode Create(
        /* [in] */ Boolean streaming);

    ECode Finalize();

    ECode GetInputStream(
        /* [out] */ IInputStream** in);

    ECode GetOutputStream(
        /* [out] */ IOutputStream** out);

    ECode Listen(
        /* [in] */ Int32 backlog);

    ECode ShutdownInput();

    ECode ShutdownOutput();

    ECode Connect(
        /* [in] */ ISocketAddress* remoteAddr,
        /* [in] */ Int32 timeout);

    ECode SupportsUrgentData(
        /* [out] */ Boolean* value);

    ECode SendUrgentData(
        /* [in] */ Int32 value);

private:
    Boolean UsingSocks();

//    Field getSocketImplField(final String fieldName);

    ECode CheckNotClosed();

    ECode Connect(
        /* [in] */ IInetAddress* anAddr,
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 timeout);

    Int32 SocksGetServerPort();

    AutoPtr<IInetAddress> SocksGetServerAddress();

    ECode SocksConnect(
        /* [in] */ IInetAddress* applicationServerAddress,
        /* [in] */ Int32 applicationServerPort,
        /* [in] */ Int32 timeout);

    ECode SocksRequestConnection(
        /* [in] */ IInetAddress* applicationServerAddress,
        /* [in] */ Int32 applicationServerPort);

    ECode SocksBind();

    static ECode IntToBytes(
        /* [in] */ Int32 value,
        /* [in] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 start);

    ECode SocksSendRequest(
        /* [in] */ Int32 command,
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    ECode SocksReadReply(
        /* [out] */ Socks4Message** ppMessage);

private:
    static AutoPtr<IInetAddress> sLastConnectedAddress;
    static Int32 sLastConnectedPort;
    //private static Field fdField;
//    Boolean mStreaming;
    Boolean mShutdownInput;
    AutoPtr<IProxy> mProxy;

    Mutex mLock;

    Int32 mIfd;
};

#endif //__PLAINSOCKETIMPL_H__
