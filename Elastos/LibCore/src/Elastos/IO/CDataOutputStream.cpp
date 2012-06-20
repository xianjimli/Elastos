//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "CDataOutputStream.h"

ECode CDataOutputStream::Close()
{
    if (m_pIos) {
        m_pIos->Close();
        m_pIos->Release();
        m_pIos = NULL;
    }

    return NOERROR;
}

ECode CDataOutputStream::Flush()
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    return m_pIos->Flush();
}

ECode CDataOutputStream::Write(
    /* [in] */ Byte byte)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    return m_pIos->Write(byte);
}

ECode CDataOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    return m_pIos->WriteBuffer(buffer, pBytesWritten);
}

ECode CDataOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    return m_pIos->WriteBufferEx(offset, length, buffer, pBytesWritten);
}

ECode CDataOutputStream::WriteInt16(
    /* [in] */ Int16 value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)&value, sizeof(Int16));
    Int32 nWrite;
    return m_pIos->WriteBuffer(tmp, &nWrite);
}

ECode CDataOutputStream::WriteInt32(
    /* [in] */ Int32 value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)&value, sizeof(Int32));
    Int32 nWrite;
    return m_pIos->WriteBuffer(tmp, &nWrite);
}

ECode CDataOutputStream::WriteInt64(
    /* [in] */ Int64 value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)&value, sizeof(Int64));
    Int32 nWrite;
    return m_pIos->WriteBuffer(tmp, &nWrite);
}

ECode CDataOutputStream::WriteFloat(
    /* [in] */ Float value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)&value, sizeof(Float));
    Int32 nWrite;
    return m_pIos->WriteBuffer(tmp, &nWrite);
}

ECode CDataOutputStream::WriteDouble(
    /* [in] */ Double value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)&value, sizeof(Double));
    Int32 nWrite;
    return m_pIos->WriteBuffer(tmp, &nWrite);
}

ECode CDataOutputStream::WriteChar8(
    /* [in] */ Char8 value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    return m_pIos->Write(value);
}

ECode CDataOutputStream::WriteChar16(
    /* [in] */ Char16 value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)&value, sizeof(Char16));
    Int32 nWrite;
    return m_pIos->WriteBuffer(tmp, &nWrite);
}

ECode CDataOutputStream::WriteBoolean(
    /* [in] */ Boolean value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    Byte tmp = value ? 1 : 0;
    return m_pIos->Write(tmp);
}

ECode CDataOutputStream::WriteEMuid(
    /* [in] */ const EMuid& value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    return E_NOT_IMPLEMENTED;
}

ECode CDataOutputStream::WriteEGuid(
    /* [in] */ const EGuid& value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    return E_NOT_IMPLEMENTED;
}

ECode CDataOutputStream::WriteECode(
    /* [in] */ ECode value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)&value, sizeof(ECode));
    Int32 nWrite;
    return m_pIos->WriteBuffer(tmp, &nWrite);
}

ECode CDataOutputStream::constructor(
    /* [in] */ IOutputStream * pStream)
{
    if (!pStream) {
        return E_INVALID_ARGUMENT;
    }
    m_pIos = pStream;
    m_pIos->AddRef();
    return NOERROR;
}

CDataOutputStream::~CDataOutputStream()
{
    Close();
}
