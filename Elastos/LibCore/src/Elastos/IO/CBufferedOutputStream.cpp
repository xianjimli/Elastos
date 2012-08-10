
#include "cmdef.h"
#include "CBufferedOutputStream.h"

ECode CBufferedOutputStream::Close()
{
    Mutex::Autolock lock(_m_syncLock);

    return CloseLocked();
}

ECode CBufferedOutputStream::CloseLocked()
{
    return BufferedOutputStream::Close();
}

ECode CBufferedOutputStream::Flush()
{
    Mutex::Autolock lock(_m_syncLock);

    return FlushLocked();
}

ECode CBufferedOutputStream::FlushLocked()
{
    return BufferedOutputStream::Flush();
}

ECode CBufferedOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    Mutex::Autolock lock(_m_syncLock);

    return WriteLocked(oneByte);
}

ECode CBufferedOutputStream::WriteLocked(
    /* [in] */ Int32 oneByte)
{
    return BufferedOutputStream::Write(oneByte);
}

ECode CBufferedOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    return BufferedOutputStream::WriteBuffer(buffer);
}

ECode CBufferedOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    Mutex::Autolock lock(_m_syncLock);

    return WriteBufferExLocked(offset, count, buffer);
}

ECode CBufferedOutputStream::WriteBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    return BufferedOutputStream::WriteBufferEx(offset, count, buffer);
}

ECode CBufferedOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return BufferedOutputStream::CheckError(hasError);
}

ECode CBufferedOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    VALIDATE_NOT_NULL(out);

    return BufferedOutputStream::Init(out, 8192);
}

ECode CBufferedOutputStream::constructor(
    /* [in] */ IOutputStream* out,
    /* [in] */ Int32 size)
{
    VALIDATE_NOT_NULL(out);

    return BufferedOutputStream::Init(out, size);
}

Mutex* CBufferedOutputStream::GetSelfLock()
{
    return &_m_syncLock;
}
