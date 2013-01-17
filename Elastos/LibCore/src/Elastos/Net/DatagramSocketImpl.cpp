
#include "cmdef.h"
#include "DatagramSocketImpl.h"
#include <Com.Kortide.Platform.h>

DatagramSocketImpl::DatagramSocketImpl()
    : mLocalPort(-1)
{}

ECode DatagramSocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fileDescriptor)
{
    assert(fileDescriptor != NULL);
    *fileDescriptor = mFd;
    if (*fileDescriptor != NULL) (*fileDescriptor)->AddRef();
    return NOERROR;
}

ECode DatagramSocketImpl::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    assert(address != NULL);

    AutoPtr<IPlatform> platform;
    AutoPtr<INetworkSystem> netSystem;
    FAIL_RETURN(CPlatform::AcquireSingleton((IPlatform**)&platform));
    FAIL_RETURN(platform->GetNetworkSystem((INetworkSystem**)&netSystem));
    //Todo:
    assert(0);
    return E_NOT_IMPLEMENTED;
    // return netSystem->GetSocketLocalAddress(mFd, address);
}

ECode DatagramSocketImpl::GetLocalPort(
    /* [out] */ Int32* port)
{
    assert(port != NULL);
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
