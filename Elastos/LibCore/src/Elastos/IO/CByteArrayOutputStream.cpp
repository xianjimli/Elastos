
#include "cmdef.h"
#include "CByteArrayOutputStream.h"

ECode CByteArrayOutputStream::Close()
{
    return ByteArrayOutputStream::Close();
}

ECode CByteArrayOutputStream::Reset()
{
    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayOutputStream::Reset();
}

ECode CByteArrayOutputStream::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    return ByteArrayOutputStream::GetSize(size);
}

ECode CByteArrayOutputStream::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);

    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayOutputStream::ToByteArray(bytes);
}

ECode CByteArrayOutputStream::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return ByteArrayOutputStream::ToString(str);
}

ECode CByteArrayOutputStream::ToStringEx(
    /* [in] */ const String& enc,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return ByteArrayOutputStream::ToStringEx(enc, str);
}

ECode CByteArrayOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayOutputStream::WriteBufferEx(offset, count, buffer);
}

ECode CByteArrayOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayOutputStream::Write(oneByte);
}

ECode CByteArrayOutputStream::WriteTo(
    /* [in] */ IOutputStream* os)
{
    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayOutputStream::WriteTo(os);
}

ECode CByteArrayOutputStream::Flush()
{
    return ByteArrayOutputStream::Flush();
}

ECode CByteArrayOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    Mutex::Autolock lock(_m_syncLock);

    return ByteArrayOutputStream::WriteBuffer(buffer);
}

ECode CByteArrayOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return ByteArrayOutputStream::CheckError(hasError);
}

ECode CByteArrayOutputStream::constructor()
{
    return ByteArrayOutputStream::Init(32);
}

ECode CByteArrayOutputStream::constructor(
    /* [in] */ Int32 size)
{
    return ByteArrayOutputStream::Init(size);
}

Mutex* CByteArrayOutputStream::GetSelfLock()
{
    return &_m_syncLock;
}
