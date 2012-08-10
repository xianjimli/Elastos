
#include "cmdef.h"
#include "CFilterInputStream.h"

CFilterInputStream::CFilterInputStream()
{
}

CFilterInputStream::~CFilterInputStream()
{
}

ECode CFilterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return FilterInputStream::Available(number);
}

ECode CFilterInputStream::Close()
{
    return FilterInputStream::Close();
}

ECode CFilterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Mutex::Autolock lock(_m_syncLock);

    return MarkLocked(readLimit);
}

ECode CFilterInputStream::MarkLocked(
    /* [in] */ Int32 readLimit)
{
    return FilterInputStream::Mark(readLimit);
}

ECode CFilterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return FilterInputStream::IsMarkSupported(supported);
}

ECode CFilterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return FilterInputStream::Read(value);
}

ECode CFilterInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return FilterInputStream::ReadBuffer(buffer, number);
}

ECode CFilterInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return FilterInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CFilterInputStream::Reset()
{
    Mutex::Autolock lock(_m_syncLock);

    return ResetLocked();
}

ECode CFilterInputStream::ResetLocked()
{
    return FilterInputStream::Reset();
}

ECode CFilterInputStream::Skip(
    /* [in] */ Int64 length,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    return FilterInputStream::Skip(length, value);
}

ECode CFilterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return FilterInputStream::Init(is);
}

Mutex* CFilterInputStream::GetSelfLock()
{
    return &_m_syncLock;
}
