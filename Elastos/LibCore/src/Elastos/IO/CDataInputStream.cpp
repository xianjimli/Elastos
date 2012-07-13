#include "cmdef.h"
#include "CDataInputStream.h"

CDataInputStream::CDataInputStream()
{
}

CDataInputStream::~CDataInputStream()
{
}

ECode CDataInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return DataInputStream::Init(is);
}

ECode CDataInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return DataInputStream::Available(number);
}

ECode CDataInputStream::Close()
{
    return DataInputStream::Close();
}

ECode CDataInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return DataInputStream::Mark(readLimit);
}

ECode CDataInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return DataInputStream::IsMarkSupported(supported);
}

ECode CDataInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::Read(value);
}

ECode CDataInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return DataInputStream::ReadBuffer(buffer, number);
}

ECode CDataInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return DataInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CDataInputStream::Reset()
{
    return DataInputStream::Reset();
}

ECode CDataInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return DataInputStream::Skip(count, number);
}

ECode CDataInputStream::ReadBoolean(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadBoolean(value);
}

ECode CDataInputStream::ReadByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadByte(value);
}

ECode CDataInputStream::ReadChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadChar(value);
}

ECode CDataInputStream::ReadDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadDouble(value);
}

ECode CDataInputStream::ReadFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadFloat(value);
}

ECode CDataInputStream::ReadFully(
    /* [out] */ ArrayOf<Byte> * buffer)
{
    VALIDATE_NOT_NULL(buffer);

    return DataInputStream::ReadFully(buffer);
}

ECode CDataInputStream::ReadFullyEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte> * buffer)
{
    VALIDATE_NOT_NULL(buffer);

    return DataInputStream::ReadFullyEx(offset, length, buffer);
}

ECode CDataInputStream::ReadInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadInt32(value);
}

ECode CDataInputStream::ReadLine(
     /* [out] */ String* value)
{
    return DataInputStream::ReadLine(value);
}

ECode CDataInputStream::ReadInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadInt64(value);
}

ECode CDataInputStream::ReadInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadInt16(value);
}

ECode CDataInputStream::ReadUnsignedByte(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadUnsignedByte(value);
}

ECode CDataInputStream::ReadUnsignedInt16(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadUnsignedInt16(value);
}

ECode CDataInputStream::SkipBytes(
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return DataInputStream::SkipBytes(count, number);
}

Mutex* CDataInputStream::GetSelfLock()
{
    return &_m_syncLock;
}
