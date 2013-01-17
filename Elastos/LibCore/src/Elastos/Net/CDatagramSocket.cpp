
#include "cmdef.h"
#include "CDatagramSocket.h"


ECode CDatagramSocket::Close()
{
    return DatagramSocket::Close();
}

ECode CDatagramSocket::Connect(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 aPort)
{
    return DatagramSocket::Connect(address, aPort);
}

ECode CDatagramSocket::Disconnect()
{
    return DatagramSocket::Disconnect();
}

ECode CDatagramSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return DatagramSocket::GetInetAddress(address);
}

ECode CDatagramSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return DatagramSocket::GetLocalAddress(address);
}

ECode CDatagramSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    return DatagramSocket::GetLocalPort(port);
}

ECode CDatagramSocket::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    return DatagramSocket::GetPort(port);
}

ECode CDatagramSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return DatagramSocket::GetSendBufferSize(size);
}

ECode CDatagramSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return DatagramSocket::GetReceiveBufferSize(size);
}

ECode CDatagramSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    return DatagramSocket::GetSoTimeout(timeout);
}

ECode CDatagramSocket::Receive(
    /* [in] */ IDatagramPacket* pack)
{
    return DatagramSocket::Receive(pack);
}

ECode CDatagramSocket::Send(
    /* [in] */ IDatagramPacket* pack)
{
    return DatagramSocket::Send(pack);
}

ECode CDatagramSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetSendBufferSize(size);
}

ECode CDatagramSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetReceiveBufferSize(size);
}

ECode CDatagramSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    return DatagramSocket::SetSoTimeout(timeout);
}

ECode CDatagramSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return DatagramSocket::Bind(localAddr);
}

ECode CDatagramSocket::ConnectEx(
    /* [in] */ ISocketAddress* remoteAddr)
{
    return DatagramSocket::ConnectEx(remoteAddr);
}

ECode CDatagramSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);
    return DatagramSocket::IsBound(isBound);
}

ECode CDatagramSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected);
    return DatagramSocket::IsConnected(isConnected);
}

ECode CDatagramSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return DatagramSocket::GetRemoteSocketAddress(address);
}

ECode CDatagramSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);
    return DatagramSocket::GetLocalSocketAddress(address);
}

ECode CDatagramSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    return DatagramSocket::SetReuseAddress(reuse);
}

ECode CDatagramSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);
    return DatagramSocket::GetReuseAddress(reuse);
}

ECode CDatagramSocket::SetBroadcast(
    /* [in] */ Boolean broadcast)
{
    return DatagramSocket::SetBroadcast(broadcast);
}

ECode CDatagramSocket::GetBroadcast(
    /* [out] */ Boolean* broadcast)
{
    VALIDATE_NOT_NULL(broadcast);
    return DatagramSocket::GetBroadcast(broadcast);
}

ECode CDatagramSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    return DatagramSocket::SetTrafficClass(value);
}

ECode CDatagramSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return DatagramSocket::GetTrafficClass(value);
}

ECode CDatagramSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);
    return DatagramSocket::IsClosed(isClosed);
}

ECode CDatagramSocket::GetChannel(
    /* [out] */ IDatagramChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    return DatagramSocket::GetChannel(channel);
}

ECode CDatagramSocket::constructor()
{
    return DatagramSocket::Init();
}

ECode CDatagramSocket::constructor(
    /* [in] */ Int32 aPort)
{
    return DatagramSocket::Init(aPort);
}

ECode CDatagramSocket::constructor(
    /* [in] */ Int32 aport,
    /* [in] */ IInetAddress* addr)
{
    return DatagramSocket::Init(aport, addr);
}

ECode CDatagramSocket::constructor(
    /* [in] */ ISocketAddress* localAddr)
{
    return DatagramSocket::Init(localAddr);
}

Mutex* CDatagramSocket::GetSelfLock()
{
    return &_m_syncLock;
}
