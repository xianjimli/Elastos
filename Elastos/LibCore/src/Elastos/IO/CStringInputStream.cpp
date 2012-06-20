//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "CStringInputStream.h"

ECode CStringInputStream::Available(
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

ECode CStringInputStream::Close()
{
    m_pBuf = NULL;
    return NOERROR;
}

ECode CStringInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }
    return E_NOT_SUPPORTED;
}

ECode CStringInputStream::IsMarkSupported(
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

ECode CStringInputStream::Read(
    /* [out] */ Byte * pByte)
{
    if (!pByte) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    if (m_pos >= m_tail) {//todo: if read '\0'
        return E_OUT_OF_STREAM;
    }

    *pByte = m_pBuf[m_pos++];
    return NOERROR;
}

ECode CStringInputStream::ReadBuffer(
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

ECode CStringInputStream::ReadBufferEx(
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

    if (length > 0) {
        if (m_pos + length > m_tail) {//todo: if read '\0'
            length = m_tail - m_pos;
        }

        pBuffer->Replace(offset, (Byte *)m_pBuf + m_pos, length);
        m_pos += length;
    }
    return NOERROR;
}

ECode CStringInputStream::Reset()
{
    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }
    m_pos = 0;
    return NOERROR;
}

ECode CStringInputStream::Skip(//todo: if number<=0 means skip back?
    /* [in] */ Int32 length)
{
    if (length <= 0) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    if (m_pos + length > m_tail)
        m_pos = m_tail;
    else
        m_pos += length;

    return NOERROR;
}

ECode CStringInputStream::ReadChar8(
    /* [out] */ Char8 * pCharacter)
{
    if (!pCharacter) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    if (m_pos >= m_tail) {
        return E_OUT_OF_STREAM;
    }

    *pCharacter = m_pBuf[m_pos++];
    return NOERROR;
}

ECode CStringInputStream::ReadChar8Array(
    /* [out] */ BufferOf<Char8> * pcharArray)
{
    if (pcharArray == NULL || !pcharArray->GetCapacity()) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    return ReadChar8ArrayEx(0, pcharArray->GetCapacity(), pcharArray);
}

ECode CStringInputStream::ReadChar8ArrayEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Char8> * pcharArray)
{
    if (pcharArray == NULL || offset < 0 || length < 0
        || pcharArray->GetCapacity() < length + offset) {
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

    pcharArray->Replace(offset, m_pBuf + m_pos, length);
    m_pos += length;
    return NOERROR;
}

ECode CStringInputStream::ReadString(
    /* [out] */ StringBuf * pString)
{
    if (pString == NULL || !pString->GetCapacity()) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    if (m_pos >= m_tail) {
        return E_OUT_OF_STREAM;
    }

    Int32 len;
    if (pString->GetCapacity() < m_tail - m_pos) {
        len = pString->GetCapacity();
    } else {
        len = m_tail - m_pos;
    }

    pString->Copy(m_pBuf + m_pos);
    m_pos += len;
    return NOERROR;
}

ECode CStringInputStream::constructor(
    /* [in] */ String string)
{
    if (string.IsNull()) {
        return E_INVALID_ARGUMENT;
    }

    m_pBuf = (char *)(const char *)string;
    m_pos = 0;
    m_tail = string.GetLength();
    return NOERROR;
}
