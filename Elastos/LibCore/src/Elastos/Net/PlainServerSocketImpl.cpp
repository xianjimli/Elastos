#include "PlainServerSocketImpl.h"

ECode PlainServerSocketImpl::Init()
{
    return PlainSocketImpl::Init();
}


ECode PlainServerSocketImpl::Init(
    /* [in] */ IFileDescriptor* fd)
{
    return PlainSocketImpl::Init(fd);
}


ECode PlainServerSocketImpl::Create(
    /* [in] */ Boolean isStreaming)
{
    ECode ec = NOERROR;
    ec = PlainSocketImpl::Create(isStreaming);
    if (FAILED(ec)) {
        return ec;
    }

    if (isStreaming) {
        SetOption(0, TRUE);
    }
    return ec;
}