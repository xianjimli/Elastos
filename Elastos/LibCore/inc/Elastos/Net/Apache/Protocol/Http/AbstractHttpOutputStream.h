#ifndef __ABSTRACTHTTPOUTPUTSTREAM_H__
#define __ABSTRACTHTTPOUTPUTSTREAM_H__

#include "Elastos.IO.h"

using namespace Elastos;

class AbstractHttpOutputStream
{
public:
    ECode Write(
        /* [in] */ Int32 data);
protected:
    ECode CheckBounds(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    ECode CheckNotClosed();

protected:
    Boolean mClosed;
};

#endif //__ABSTRACTHTTPOUTPUTSTREAM_H__