#include "AbstractHttpOutputStream.h"


ECode AbstractHttpOutputStream::Write(
        /* [in] */ Int32 data)
{
}

ECode AbstractHttpOutputStream::CheckBounds(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
}

ECode AbstractHttpOutputStream::CheckNotClosed()
{
}