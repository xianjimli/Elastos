
#include "cmdef.h"
#include "CBufferedInputStream.h"

ECode CBufferedInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(_m_syncLock);

    return BufferedInputStream::Available(number);
}

ECode CBufferedInputStream::Close()
{
    return BufferedInputStream::Close();
}

ECode CBufferedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Mutex::Autolock lock(_m_syncLock);

    return BufferedInputStream::Mark(readLimit);
}

ECode CBufferedInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return BufferedInputStream::IsMarkSupported(supported);
}

ECode CBufferedInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Mutex::Autolock lock(_m_syncLock);

    return BufferedInputStream::Read(value);
}

ECode CBufferedInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return BufferedInputStream::ReadBuffer(buffer, number);
}

ECode CBufferedInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(_m_syncLock);

    return BufferedInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CBufferedInputStream::Reset()
{
    Mutex::Autolock lock(_m_syncLock);

    return BufferedInputStream::Reset();
}

ECode CBufferedInputStream::Skip(
    /* [in] */ Int64 length,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(_m_syncLock);

    return BufferedInputStream::Skip(length, number);
}

ECode CBufferedInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return BufferedInputStream::Init(is, 8192);
}

ECode CBufferedInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 size)
{
    return BufferedInputStream::Init(is, size);
}

Mutex* CBufferedInputStream::GetSelfLock()
{
    return &_m_syncLock;
}
