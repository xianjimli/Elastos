
#include "database/CCharArrayBuffer.h"
ECode CCharArrayBuffer::GetData(
    /* [out, callee] */ ArrayOf<Char8> ** ppData)
{
    CharArrayBuffer::GetData(ppData);
    return NOERROR;
}

ECode CCharArrayBuffer::SetData(
    /* [in] */ const ArrayOf<Char8> & data)
{
    CharArrayBuffer::SetData(data);
    return NOERROR;
}

ECode CCharArrayBuffer::constructor()
{
    CharArrayBuffer::CharArrayBuffer();
    return NOERROR;
}

ECode CCharArrayBuffer::constructor(
    /* [in] */ Int32 size)
{
    CharArrayBuffer::Init(size);
    return NOERROR;
}

ECode CCharArrayBuffer::constructor(
    /* [in] */ const ArrayOf<Char8> & buf)
{
    CharArrayBuffer::Init(buf);
    return NOERROR;
}

