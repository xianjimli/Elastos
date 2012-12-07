
#ifndef __SOCKETIMPL_H__
#define __SOCKETIMPL_H__

#include "Elastos.Net_server.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>

//extern "C" const InterfaceID EIID_ProxySelectorImpl;

/**
 * This class is the base of all streaming socket implementation classes.
 * Streaming sockets are wrapped by two classes, {@code ServerSocket} and
 * {@code Socket} at the server and client end of a connection. At the server,
 * there are two types of sockets engaged in communication, the {@code
 * ServerSocket} on a well known port (referred to as listener) used to
 * establish a connection and the resulting {@code Socket} (referred to as
 * host).
 */
class SocketImpl
{
public:
    SocketImpl();

    virtual CARAPI Accept(
        /* [in] */ ISocketImpl* newSocket) = 0;

    virtual CARAPI Available(
        /* [out] */ Int32* num) = 0;

    virtual CARAPI Bind(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port) = 0;

    virtual CARAPI Close() = 0;

    virtual CARAPI Connect(
        /* [in] */ String host,
        /* [in] */ Int32 port) = 0;

    virtual CARAPI ConnectEx(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port) = 0;

    virtual CARAPI Create(
        /* [in] */ Boolean isStreaming) = 0;

    virtual CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fileDescriptor);

    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** netAddress);

    virtual CARAPI GetInputStream(
        /* [out] */ IInputStream** inputStream) = 0;

    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* num);

    virtual CARAPI GetOutputStream(
        /* [out] */ IOutputStream** outputStream) = 0;

    virtual CARAPI GetPort(
        /* [out] */ Int32* port);

    virtual CARAPI Listen(
        /* [in] */ Int32 backlog) = 0;

    virtual CARAPI ShutdownInput();

    virtual CARAPI ShutdownOutput();

    virtual CARAPI ConnectEx2(
        /* [in] */ ISocketAddress* remoteAddr,
        /* [in] */ Int32 timeout) = 0;

    virtual CARAPI SupportsUrgentData(
        /* [out] */ Boolean* isSupports);

    virtual CARAPI SendUrgentData(
        /* [in] */ Int32 value) = 0;

    virtual CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth) = 0;

    virtual CARAPI GetOption(
        /* [in] */ Int32 optID,
        /* [out] */ Int32* optVal) = 0;

    virtual CARAPI SetOption(
        /* [in] */ Int32 optID,
        /* [in] */ Int32 optVal) = 0;

protected:
    /**
     * The remote address this socket is connected to.
     */
    AutoPtr<IInetAddress> mAddress;

    /**
     * The remote port this socket is connected to.
     */
    Int32 mPort;

    /**
     * The file descriptor of this socket.
     */
    AutoPtr<IFileDescriptor> mFd;

    /**
     * The local port this socket is connected to.
     */
    Int32 mLocalport;

public:
    AutoPtr<INetworkSystem> mNetImpl;

    Boolean mStreaming;
};

#endif //__SOCKETIMPL_H__
