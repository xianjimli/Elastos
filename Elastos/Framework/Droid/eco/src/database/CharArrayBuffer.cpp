
#include "database/CharArrayBuffer.h"

CharArrayBuffer::CharArrayBuffer()
{
}

CharArrayBuffer::~CharArrayBuffer()
{
    ArrayOf<Char8>::Free(data);
}

ECode CharArrayBuffer::Init(
        /* [in] */ Int32 size)
{
    data = ArrayOf<Char8>::Alloc(size);
    return NOERROR;
}

ECode CharArrayBuffer::Init(
        /* [in] */ ArrayOf<Char8>& buf)
{
    data = buf.Clone();
    return NOERROR;
}