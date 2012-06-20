
#include "cmdef.h"
#include "CBufferedOutputStream.h"

ECode CBufferedOutputStream::Close()
{
    Mutex::Autolock lock(_m_syncLock);

    return BufferedOutputStream::Close();
}

ECode CBufferedOutputStream::Flush()
{
    Mutex::Autolock lock(_m_syncLock);

    return BufferedOutputStream::Flush();
}

ECode CBufferedOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    Mutex::Autolock lock(_m_syncLock);

    return BufferedOutputStream::Write(oneByte);
}

ECode CBufferedOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte> & buffer)
{
    return BufferedOutputStream::WriteBuffer(buffer);
}

ECode CBufferedOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const BufferOf<Byte> & buffer)
{
    Mutex::Autolock lock(_m_syncLock);

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
