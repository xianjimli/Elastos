//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "CByteArrayInputStream.h"

ECode CByteArrayInputStream::Available(
    /* [out] */ Int32 * pBytes)
{
    if (!pBytes) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    *pBytes = m_tail - m_pos;
    return NOERROR;
}

ECode CByteArrayInputStream::Close()
{
    m_pBuf = NULL;
    return NOERROR;
}

ECode CByteArrayInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    return E_NOT_SUPPORTED;
}

ECode CByteArrayInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    if (!pSupported) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }
    *pSupported = FALSE;
    return NOERROR;
}

ECode CByteArrayInputStream::Read(
    /* [out] */ Byte * pByte)
{
    if (!pByte) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    if (m_pos >= m_tail) {
        return E_OUT_OF_STREAM;
    }

    *pByte = (*m_pBuf)[m_pos++];
    return NOERROR;
}

ECode CByteArrayInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (pBuffer == NULL || !pBuffer->GetCapacity()) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    return ReadBufferEx(0, pBuffer->GetCapacity(), pBuffer);
}

ECode CByteArrayInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (pBuffer == NULL || offset < 0 || length < 0
        || (pBuffer->GetCapacity() < length + offset)
        || (pBuffer->GetUsed() < offset)) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    if (m_pos >= m_tail) {
        return E_OUT_OF_STREAM;
    }
    if (m_pos + length > m_tail) {
        length = m_tail - m_pos;
    }

    if (length > 0) {
        pBuffer->Replace(offset, m_pBuf->GetPayload() + m_pos, length);
        m_pos += length;
    }
    pBuffer->SetUsed(offset + length);
    return NOERROR;
}

ECode CByteArrayInputStream::Reset()
{
    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    m_pos = m_mark;
    return NOERROR;
}

ECode CByteArrayInputStream::Skip(
    /* [in] */ Int32 length)
{
    if (length <= 0) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    if (m_pos + length > m_tail) {
        m_pos = m_tail;
    } else {
        m_pos += length;
    }
    return NOERROR;
}

ECode CByteArrayInputStream::constructor(
    /* [in] */ const BufferOf<Byte> & buffer)
{
    if (buffer.IsNull()) {
        return E_INVALID_ARGUMENT;
    }

    return constructor(buffer, 0, buffer.GetUsed());
}

ECode CByteArrayInputStream::constructor(
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (buffer.IsNull() || length < 0 || offset < 0
                || offset + length > buffer.GetUsed()) {
        return E_INVALID_ARGUMENT;
    }

    m_pBuf = (BufferOf<Byte> *)&buffer;
    m_pos = offset;
    m_mark = offset;
    m_tail = offset + length;
    return NOERROR;
}
