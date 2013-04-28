
#include "CPlainSocketImpl.h"
#include <stdio.h>

ECode CPlainSocketImpl::GetOption(
    /* [in] */ Int32 optID,
    /* [out] */ IInterface ** ppOptVal)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::SetOption(
    /* [in] */ Int32 optID,
    /* [in] */ IInterface * pOptVal)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::Accept(
    /* [in] */ ISocketImpl * pNewSocket)
{
    // TODO: Add your code here
    return PlainSocketImpl::Accept(pNewSocket);
}

ECode CPlainSocketImpl::Available(
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::Bind(
    /* [in] */ IInetAddress * pAddress,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    return PlainSocketImpl::Bind(pAddress, port);
}

ECode CPlainSocketImpl::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::Connect(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::ConnectEx(
    /* [in] */ IInetAddress * pAddress,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    return PlainSocketImpl::Connect(pAddress, port);
}

ECode CPlainSocketImpl::Create(
    /* [in] */ Boolean isStreaming)
{
    // TODO: Add your code here
    printf("---------------Socket Create---------------\n");
    return PlainSocketImpl::Create(isStreaming);
}

ECode CPlainSocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor ** ppFileDescriptor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::GetInetAddress(
    /* [out] */ IInetAddress ** ppNetAddress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::GetInputStream(
    /* [out] */ IInputStream ** ppInputStream)
{
    // TODO: Add your code here
    return PlainSocketImpl::GetInputStream(ppInputStream);
}

ECode CPlainSocketImpl::GetLocalPort(
    /* [out] */ Int32 * pNum)
{
    // TODO: Add your code here
    return PlainSocketImpl::GetLocalPort(pNum);
}

ECode CPlainSocketImpl::GetOutputStream(
    /* [out] */ IOutputStream ** ppOutputStream)
{
    // TODO: Add your code here
    return PlainSocketImpl::GetOutputStream(ppOutputStream);
}

ECode CPlainSocketImpl::GetPort(
    /* [out] */ Int32 * pPort)
{
    // TODO: Add your code here
    return PlainSocketImpl::GetPort(pPort);
}

ECode CPlainSocketImpl::Listen(
    /* [in] */ Int32 backlog)
{
    // TODO: Add your code here
    return PlainSocketImpl::Listen(backlog);
}

ECode CPlainSocketImpl::ShutdownInput()
{
    // TODO: Add your code here
    return PlainSocketImpl::ShutdownInput();
}

ECode CPlainSocketImpl::ShutdownOutput()
{
    // TODO: Add your code here
    return PlainSocketImpl::ShutdownOutput();
}

ECode CPlainSocketImpl::ConnectEx2(
    /* [in] */ ISocketAddress * pRemoteAddr,
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::SupportsUrgentData(
    /* [out] */ Boolean * pIsSupports)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::SendUrgentData(
    /* [in] */ Int32 value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::constructor()
{
    // TODO: Add your code here
    return PlainSocketImpl::Init();
}

ECode CPlainSocketImpl::constructor(
    /* [in] */ IFileDescriptor * pFd)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::constructor(
    /* [in] */ IProxy * pProxy)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPlainSocketImpl::constructor(
    /* [in] */ IFileDescriptor * pFd,
    /* [in] */ Int32 localport,
    /* [in] */ IInetAddress * pAddr,
    /* [in] */ Int32 port)
{
    // TODO: Add your code here
    return PlainSocketImpl::Init(pFd, localport, pAddr, port);
}

//ECode CPlainSocketImpl::Read(
//        /* [in] */ ArrayOf<Byte>* buffer,
//        /* [in] */ Int32 offset,
//        /* [in] */ Int32 count,
//        /* [out] */ Int32* value)
//{
//    printf("%s,%d\n, this is %x\n", __FILE__, __LINE__, this);
//    return PlainSocketImpl::Read(buffer, offset, count, value);
//}
//
