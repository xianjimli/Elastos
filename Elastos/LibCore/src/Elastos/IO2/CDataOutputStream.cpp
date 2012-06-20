#include "cmdef.h"
#include "CDataOutputStream.h"

CDataOutputStream::CDataOutputStream()
{
}
CDataOutputStream::~CDataOutputStream()
{
}

ECode CDataOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    return DataOutputStream::Init(out);
}

ECode CDataOutputStream::Close()
{
    return DataOutputStream::Close();
}

ECode CDataOutputStream::Flush()
{
    return DataOutputStream::Flush();
}

ECode CDataOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return DataOutputStream::Write(oneByte);
}

ECode CDataOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte> & buffer)
{
    return DataOutputStream::WriteBuffer(buffer);
}

ECode CDataOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const BufferOf<Byte> & buffer)
{
    return DataOutputStream::WriteBufferEx(offset, count, buffer);
}

ECode CDataOutputStream::CheckError(
     /* [out] */ Boolean* checkerror)
{
    VALIDATE_NOT_NULL(checkerror);

    return DataOutputStream::CheckError(checkerror);
}

ECode CDataOutputStream::WriteBoolean(
    /*[in]*/ Boolean val)
{
    return DataOutputStream::WriteBoolean(val);
}

ECode CDataOutputStream::WriteByte(
    /*[in]*/ Int32 val)
{
    return DataOutputStream::WriteByte(val);
}

ECode CDataOutputStream::WriteBytes(
    /*[in]*/ const BufferOf<Byte>& buffer)
{
    return DataOutputStream::WriteBytes(buffer);
}

ECode CDataOutputStream::WriteBytesEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const BufferOf<Byte>& buffer)
{
    return DataOutputStream::WriteBytesEx(offset, count, buffer);
}

ECode CDataOutputStream::WriteChar(
    /*[in]*/ Int32 val)
{
    return DataOutputStream::WriteChar(val);
}

ECode CDataOutputStream::WriteDouble(
    /*[in]*/ Double val)
{
    return DataOutputStream::WriteDouble(val);
}

ECode CDataOutputStream::WriteFloat(
    /*[in]*/ Float val)
{
    return DataOutputStream::WriteFloat(val);
}

ECode CDataOutputStream::WriteInt32(
    /*[in]*/ Int32 val)
{
    return DataOutputStream::WriteInt32(val);
}

ECode CDataOutputStream::WriteInt64(
    /*[in]*/ Int64 val)
{
    return DataOutputStream::WriteInt64(val);
}

ECode CDataOutputStream::WriteInt16(
    /*[in]*/ Int32 val)
{
    return DataOutputStream::WriteInt16(val);
}

Mutex* CDataOutputStream::GetSelfLock()
{
    return &_m_syncLock;
}
