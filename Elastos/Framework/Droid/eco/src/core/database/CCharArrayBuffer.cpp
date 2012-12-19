
#include "ext/frameworkdef.h"
#include "database/CCharArrayBuffer.h"


ECode CCharArrayBuffer::constructor(
    /* [in] */ Int32 size)
{
    return CharArrayBuffer::Init(size);
}

ECode CCharArrayBuffer::constructor(
    /* [in] */ ArrayOf<Char32>* buf)
{
    return CharArrayBuffer::Init(buf);
}

ECode CCharArrayBuffer::GetData(
    /* [out, callee] */ ArrayOf<Char32>** data)
{
    VALIDATE_NOT_NULL(data);
    return CharArrayBuffer::GetData(data);
}

ECode CCharArrayBuffer::SetData(
    /* [in] */ ArrayOf<Char32>* data)
{
    return CharArrayBuffer::SetData(data);
}
