#ifndef __PLAINSERVERSOCKETIMPL_H__
#define __PLAINSERVERSOCKETIMPL_H__

#include "PlainSocketImpl.h"
#include "Elastos.IO.h"

class PlainServerSocketImpl : public PlainSocketImpl
{
public:
    Init();

    Init(
        /* [in] */ IFileDescriptor *fd);

protected:
    ECode Create(
        /* [in] */ Boolean isStreaming);
};


#endif //__PLAINSERVERSOCKETIMPL_H__
