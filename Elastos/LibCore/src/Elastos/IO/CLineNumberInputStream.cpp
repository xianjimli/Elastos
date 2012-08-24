#include "cmdef.h"
#include "CLineNumberInputStream.h"

ECode CLineNumberInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number != NULL);

    return LineNumberInputStream::Available(number);
}

ECode CLineNumberInputStream::Close()
{
    return LineNumberInputStream::Close();
}

ECode CLineNumberInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return LineNumberInputStream::Mark(readLimit);
}

ECode CLineNumberInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported != NULL);

    return LineNumberInputStream::IsMarkSupported(supported);
}

ECode CLineNumberInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value != NULL);

    return LineNumberInputStream::Read(value);
}

ECode CLineNumberInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer != NULL);
    VALIDATE_NOT_NULL(number != NULL);

    return LineNumberInputStream::ReadBuffer(buffer, number);
}

ECode CLineNumberInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer != NULL);
    VALIDATE_NOT_NULL(number != NULL);

    return LineNumberInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CLineNumberInputStream::Reset()
{
    return LineNumberInputStream::Reset();
}

ECode CLineNumberInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number != NULL);

    return LineNumberInputStream::Skip(count, number);
}

ECode CLineNumberInputStream::GetLineNumber(
    /* [out] */ Int32* lineNumber)
{
    VALIDATE_NOT_NULL(lineNumber != NULL);

    return LineNumberInputStream::GetLineNumber(lineNumber);
}

ECode CLineNumberInputStream::SetLineNumber(
    /* [in] */ Int32 lineNumber)
{
    return LineNumberInputStream::SetLineNumber(lineNumber);
}

ECode CLineNumberInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return LineNumberInputStream::Init(is);
}

Mutex* CLineNumberInputStream::GetSelfLock()
{
    return &_m_syncLock;
}