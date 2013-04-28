
#include "CPlainServerSocketImpl.h"

ECode CPlainServerSocketImpl::GetOption(
    /* [in] */ Int32 optID,
    /* [out] */ IInterface ** ppOptVal)
{
    // TODO: Add your code here
//    return PlainServerSocketImpl::GetOption(optID, ppOptVal);
}

ECode CPlainServerSocketImpl::SetOption(
    /* [in] */ Int32 optID,
    /* [in] */ IInterface * pOptVal)
{
    // TODO: Add your code here
//    return PlainServerSocketImpl::SetOption(optID, pOptVal);
}

ECode CPlainServerSocketImpl::Accept(
    /* [in] */ ISocketImpl * pNewSocket)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::Accept(pNewSocket);
}

ECode CPlainServerSocketImpl::Available(
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainServerSocketImpl::Bind(
    /* [in] */ IInetAddress * pAddress,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::Bind(pAddress, port);
}

ECode CPlainServerSocketImpl::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainServerSocketImpl::Connect(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainServerSocketImpl::ConnectEx(
    /* [in] */ IInetAddress * pAddress,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::Connect(pAddress, port);
}

ECode CPlainServerSocketImpl::Create(
    /* [in] */ Boolean isStreaming)
{
    // TODO: Add your code here
    ECode ec = NOERROR;
    ec = PlainServerSocketImpl::Create(isStreaming);
    return ec;
}

ECode CPlainServerSocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor ** ppFileDescriptor)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::GetFD(ppFileDescriptor);
}

ECode CPlainServerSocketImpl::GetInetAddress(
    /* [out] */ IInetAddress ** ppNetAddress)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::GetInetAddress(ppNetAddress);
}

ECode CPlainServerSocketImpl::GetInputStream(
    /* [out] */ IInputStream ** ppInputStream)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::GetInputStream(ppInputStream);
}

ECode CPlainServerSocketImpl::GetLocalPort(
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::GetLocalPort(pNum);
}

ECode CPlainServerSocketImpl::GetOutputStream(
    /* [out] */ IOutputStream ** ppOutputStream)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::GetOutputStream(ppOutputStream);
}

ECode CPlainServerSocketImpl::GetPort(
    /* [out] */ Int32 * pPort)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::GetPort(pPort);
}

ECode CPlainServerSocketImpl::Listen(
    /* [in] */ Int32 backlog)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::Listen(backlog);
}

ECode CPlainServerSocketImpl::ShutdownInput()
{
    // TODO: Add your code here
    return PlainServerSocketImpl::ShutdownInput();
}

ECode CPlainServerSocketImpl::ShutdownOutput()
{
    // TODO: Add your code here
    return PlainServerSocketImpl::ShutdownOutput();
}

ECode CPlainServerSocketImpl::ConnectEx2(
    /* [in] */ ISocketAddress * pRemoteAddr,
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::Connect(pRemoteAddr, timeout);
}

ECode CPlainServerSocketImpl::SupportsUrgentData(
    /* [out] */ Boolean * pIsSupports)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::SupportsUrgentData(pIsSupports);
}

ECode CPlainServerSocketImpl::SendUrgentData(
    /* [in] */ Int32 value)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::SendUrgentData(value);
}

ECode CPlainServerSocketImpl::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    // TODO: Add your code here
//    return PlainServerSocketImpl::SetPerformancePreferences(connectionTime, latency, bandwidth);
    return E_NOT_IMPLEMENTED;
}

ECode CPlainServerSocketImpl::constructor()
{
    // TODO: Add your code here
    return PlainServerSocketImpl::Init();
}

ECode CPlainServerSocketImpl::constructor(
    /* [in] */ IFileDescriptor * pFd)
{
    // TODO: Add your code here
    return PlainServerSocketImpl::Init(pFd);
}

