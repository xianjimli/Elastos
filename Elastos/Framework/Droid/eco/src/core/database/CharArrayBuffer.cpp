
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
        /* [in] */ const ArrayOf<Char8>& buf)
{
    data = buf.Clone();
    return NOERROR;
}

ECode CharArrayBuffer::GetData(
    /* [out,callee] */ ArrayOf<Char8>** data)
{
    *data = this->data;
    return NOERROR;
}

ECode CharArrayBuffer::SetData(
    /* [in] */ const ArrayOf<Char8>& data)
{
    this->data = const_cast<ArrayOf<Char8>*>(&data);
    return NOERROR;
}