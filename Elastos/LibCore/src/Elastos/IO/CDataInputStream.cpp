//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "CDataInputStream.h"

ECode CDataInputStream::Available(
    /* [out] */ Int32 * pBytes)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return m_pIis->Available(pBytes);
}

ECode CDataInputStream::Close()
{
    if (m_pIis) {
        m_pIis->Close();
        m_pIis->Release();
        m_pIis = NULL;
    }

    return NOERROR;
}

ECode CDataInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return m_pIis->Mark(readLimit);
}

ECode CDataInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    if (!pSupported) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    return m_pIis->IsMarkSupported(pSupported);
}

ECode CDataInputStream::Read(
    /* [out] */ Byte * pValue)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return m_pIis->Read(pValue);
}

ECode CDataInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return m_pIis->ReadBuffer(pBuffer);
}

ECode CDataInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return m_pIis->ReadBufferEx(offset, length, pBuffer);
}

ECode CDataInputStream::Reset()
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return m_pIis->Reset();
}

ECode CDataInputStream::Skip(
    /* [in] */ Int32 length)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return m_pIis->Skip(length);
}

ECode CDataInputStream::ReadInt16(
    /* [out] */ Int16 * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)pValue, sizeof(Int16));
    return m_pIis->ReadBuffer(&tmp);
}

ECode CDataInputStream::ReadInt32(
    /* [out] */ Int32 * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)pValue, sizeof(Int32));
    return m_pIis->ReadBuffer(&tmp);
}

ECode CDataInputStream::ReadInt64(
    /* [out] */ Int64 * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)pValue, sizeof(Int64));
    return m_pIis->ReadBuffer(&tmp);
}

ECode CDataInputStream::ReadFloat(
    /* [out] */ Float * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)pValue, sizeof(Float));
    return m_pIis->ReadBuffer(&tmp);
}

ECode CDataInputStream::ReadDouble(
    /* [out] */ Double * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)pValue, sizeof(Double));
    return m_pIis->ReadBuffer(&tmp);
}

ECode CDataInputStream::ReadChar8(
    /* [out] */ Char8 * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)pValue, sizeof(Char8));
    return m_pIis->ReadBuffer(&tmp);
}

ECode CDataInputStream::ReadChar16(
    /* [out] */ Char16 * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)pValue, sizeof(Char16));
    return m_pIis->ReadBuffer(&tmp);
}

ECode CDataInputStream::ReadBoolean(
    /* [out] */ Boolean * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    Byte value;
    ECode ec = m_pIis->Read(&value);
    if (FAILED(ec)) {
        return ec;
    }

    *pValue = (value != 0);
    return NOERROR;
}

ECode CDataInputStream::ReadEMuid(
    /* [out] */ EMuid * pValue)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return E_NOT_IMPLEMENTED;
}

ECode CDataInputStream::ReadEGuid(
    /* [out] */ EGuid * pValue)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return E_NOT_IMPLEMENTED;
}

ECode CDataInputStream::ReadECode(
    /* [out] */ ECode * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    BufferOf<Byte> tmp((Byte *)pValue, sizeof(ECode));
    return m_pIis->ReadBuffer(&tmp);
}

ECode CDataInputStream::constructor(
    /* [in] */ IInputStream * pStream)
{
    if (!pStream) {
        return E_INVALID_ARGUMENT;
    }

    m_pIis = pStream;
    m_pIis->AddRef();
    return NOERROR;
}

CDataInputStream::~CDataInputStream()
{
    Close();
}
