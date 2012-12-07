
#include "cmdef.h"
#include "DatagramSocketImpl.h"

DatagramSocketImpl::DatagramSocketImpl()
    : mLocalPort(-1)
{}

ECode DatagramSocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    *fileDescriptor = mFd;
    return NOERROR;
}

ECode DatagramSocketImpl::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    AutoPtr<IPlatform> platform;
    AutoPtr<INetworkSystem> netSystem;
    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
    ASSERT_SUCCEEDED(platform->GetNetworkSystem((INetworkSystem**)&netSystem));

    return E_NOT_IMPLEMENTED;//netSystem->GetSocketLocalAddress(mFd, address);
}

ECode DatagramSocketImpl::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mLocalPort;
    return NOERROR;
}

ECode DatagramSocketImpl::Connect(
    /* [in] */ IInetAddress* inetAddr,
    /* [in] */ Int32 port)
{
    // do nothing
    return NOERROR;
}

ECode DatagramSocketImpl::Disconnect()
{
    // do nothing
    return NOERROR;
}
