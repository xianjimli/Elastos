
#include "cmdef.h"
#include "CByteArrayInputStream.h"

ECode CByteArrayInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayInputStream::Available(number);
}

ECode CByteArrayInputStream::Close()
{
    return ByteArrayInputStream::Close();
}

ECode CByteArrayInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayInputStream::Mark(readLimit);
}

ECode CByteArrayInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return ByteArrayInputStream::IsMarkSupported(supported);
}

ECode CByteArrayInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayInputStream::Read(value);
}

ECode CByteArrayInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayInputStream::ReadBuffer(buffer, number);
}

ECode CByteArrayInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CByteArrayInputStream::Reset()
{
    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayInputStream::Reset();
}

ECode CByteArrayInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayInputStream::Skip(count, number);
}

ECode CByteArrayInputStream::constructor(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return ByteArrayInputStream::Init(buffer);
}

ECode CByteArrayInputStream::constructor(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return ByteArrayInputStream::Init(buffer, offset, length);
}

Mutex* CByteArrayInputStream::GetSelfLock()
{
    return &_m_syncLock;
}
