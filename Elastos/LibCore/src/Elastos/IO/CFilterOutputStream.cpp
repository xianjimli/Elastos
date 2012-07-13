
#include "cmdef.h"
#include "CFilterOutputStream.h"

CFilterOutputStream::CFilterOutputStream()
{
}

CFilterOutputStream::~CFilterOutputStream()
{
}

ECode CFilterOutputStream::Close()
{
    return FilterOutputStream::Close();
}

ECode CFilterOutputStream::Flush()
{
    return FilterOutputStream::Flush();
}

ECode CFilterOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return FilterOutputStream::Write(oneByte);
}

ECode CFilterOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return FilterOutputStream::WriteBuffer(buffer);
}

ECode CFilterOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return FilterOutputStream::WriteBufferEx(offset, count, buffer);
}

ECode CFilterOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return FilterOutputStream::CheckError(hasError);
}

ECode CFilterOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    VALIDATE_NOT_NULL(out);

    return FilterOutputStream::Init(out);
}

Mutex* CFilterOutputStream::GetSelfLock()
{
    return &_m_syncLock;
}
