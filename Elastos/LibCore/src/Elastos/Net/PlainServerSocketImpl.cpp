#include "PlainServerSocketImpl.h"

PlainServerSocketImpl::PlainServerSocketImpl()
{
}


PlainServerSocketImpl::PlainServerSocketImpl(
    /* [in] */ IFileDescriptor* fd)
    : PlainSocketImpl(fd)
{
}


ECode PlainServerSocketImpl::Create(
    /* [in] */ Boolean isStreaming)
{
    PlainSocketImpl::Create(isStreaming);
    if (isStreaming) {
        SetOption(0, TRUE);
    }
}