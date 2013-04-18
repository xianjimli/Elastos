
#include "cmdef.h"
#include "SocketImpl.h"
#include <stdio.h>

SocketImpl::SocketImpl()
    : mPort(0)
    , mLocalport(0)
    , mStreaming(TRUE)
{
    printf("%s,%d\n", __FILE__, __LINE__);
    AutoPtr<IPlatform> platform;
    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
    ASSERT_SUCCEEDED(platform->GetNetworkSystem((INetworkSystem**)&mNetImpl));
}

ECode SocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    *fileDescriptor = mFd;
    if (*fileDescriptor != NULL) (*fileDescriptor)->AddRef();
    return NOERROR;
}

ECode SocketImpl::GetInetAddress(
    /* [out] */ IInetAddress** netAddress)
{
    VALIDATE_NOT_NULL(netAddress);
    *netAddress = mAddress;
    if (*netAddress != NULL) (*netAddress)->AddRef();
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

ECode SocketImpl::Write(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    // if (mStreaming) {
    //     return mNetImpl->Write(mFd, buffer, offset, count, number);
    // }
    // else {
    //     return mNetImpl->Send(mFd, buffer, offset, count, mPort, mAddress, number);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode SocketImpl::ShutdownInput()
{
//    throw new IOException("Method has not been implemented");
    return E_IO_EXCEPTION;
}

ECode SocketImpl::ShutdownOutput()
{
//    throw new IOException("Method has not been implemented");
    return E_IO_EXCEPTION;
}

ECode SocketImpl::SupportsUrgentData(
    /* [out] */ Boolean* isSupports)
{
    VALIDATE_NOT_NULL(isSupports);
    *isSupports = FALSE;

    return NOERROR;
}
