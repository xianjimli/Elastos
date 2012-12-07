
#include "cmdef.h"
#include "SocketImpl.h"

SocketImpl::SocketImpl()
    : mPort(0)
    , mLocalport(0)
    , mStreaming(TRUE)
{
    AutoPtr<IPlatform> platform;
    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
    ASSERT_SUCCEEDED(platform->GetNetworkSystem((INetworkSystem**)&mNetImpl));
}

ECode SocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    *fileDescriptor = mFd;

    return NOERROR;
}

ECode SocketImpl::GetInetAddress(
    /* [out] */ IInetAddress** netAddress)
{
    VALIDATE_NOT_NULL(netAddress);
    *netAddress = mAddress;

    return NOERROR;
}

ECode SocketImpl::GetLocalPort(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    *num = mLocalport;

    return NOERROR;
}

ECode SocketImpl::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mPort;

    return NOERROR;
}

ECode SocketImpl::ShutdownInput()
{
    return E_IO_EXCEPTION;
//    throw new IOException("Method has not been implemented");
}

ECode SocketImpl::ShutdownOutput()
{
    return E_IO_EXCEPTION;
//    throw new IOException("Method has not been implemented");
}

ECode SocketImpl::SupportsUrgentData(
    /* [out] */ Boolean* isSupports)
{
    VALIDATE_NOT_NULL(isSupports);
    *isSupports = FALSE;

    return NOERROR;
}
