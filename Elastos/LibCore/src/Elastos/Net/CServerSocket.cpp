
#include "cmdef.h"
#include "CServerSocket.h"

CServerSocket::CServerSocket()
{}

ECode CServerSocket::Accept(
    /* [out] */ ISocket** socket)
{
    return ServerSocket::Accept(socket);
}

ECode CServerSocket::Close()
{
    return ServerSocket::Close();
}

ECode CServerSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    return ServerSocket::GetInetAddress(address);
}

ECode CServerSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    return ServerSocket::GetLocalPort(port);
}

ECode CServerSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    return ServerSocket::GetSoTimeout(timeout);
}

ECode CServerSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    return ServerSocket::SetSoTimeout(timeout);
}

ECode CServerSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return ServerSocket::Bind(localAddr);
}

ECode CServerSocket::BindEx(
    /* [in] */ ISocketAddress* localAddr,
    /* [in] */ Int32 backlog)
{
    return ServerSocket::BindEx(localAddr, backlog);
}

ECode CServerSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    return ServerSocket::GetLocalSocketAddress(address);
}

ECode CServerSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    return ServerSocket::IsBound(isBound);
}

ECode CServerSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    return ServerSocket::IsClosed(isClosed);
}

ECode CServerSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    return ServerSocket::SetReuseAddress(reuse);
}

ECode CServerSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    return ServerSocket::GetReuseAddress(reuse);
}

ECode CServerSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return ServerSocket::SetReceiveBufferSize(size);
}

ECode CServerSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    return ServerSocket::GetReceiveBufferSize(size);
}

ECode CServerSocket::GetChannel(
    /* [out] */ IServerSocketChannel** channel)
{
    return ServerSocket::GetChannel(channel);
}

ECode CServerSocket::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    return ServerSocket::SetPerformancePreferences(connectionTime, latency, bandwidth);
}

ECode CServerSocket::constructor()
{
    return ServerSocket::Init();
}

ECode CServerSocket::constructor(
    /* [in] */ Int32 aPort)
{
    return ServerSocket::Init(aPort);
}

ECode CServerSocket::constructor(
    /* [in] */ Int32 aport,
    /* [in] */ Int32 backlog)
{
    return ServerSocket::Init(aport, backlog);
}

ECode CServerSocket::constructor(
    /* [in] */ Int32 aport,
    /* [in] */ Int32 backlog,
    /* [in] */ IInetAddress* localAddr)
{
    return ServerSocket::Init(aport, backlog, localAddr);
}

