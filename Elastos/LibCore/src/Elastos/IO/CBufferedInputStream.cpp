//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <stdio.h>
#include "CBufferedInputStream.h"
#include "local.h"

ECode CBufferedInputStream::fill()
{
    if (m_markpos < 0) {
        m_pos = 0;  /* no mark: throw away the buffer */
    }
    else if (m_pos >= m_pBuf->GetCapacity()) { /* no room left in m_pBuf */
        if (m_markpos > 0) { /* can throw away early part of the buffer */
            Int32 sz = m_pos - m_markpos;
            PByte pBuf = m_pBuf->GetPayload();
            memmove(pBuf, pBuf + m_markpos, sz);
            m_pos = sz;
            m_pBuf->SetUsed(sz);
            m_markpos = 0;
        }
        else if (m_pBuf->GetCapacity() >= m_markLimit) {
            m_markpos = -1; /* m_pBuf got too big, invalidate mark */
            m_pos = 0; /* drop buffer contents */
        }
        else {  /* grow buffer */
            Int32 nsz = m_pos * 2;
            if (nsz > m_markLimit) nsz = m_markLimit;
            BufferOf<Byte>* pNewBuf = BufferOf<Byte>::Alloc(nsz);
            if (!pNewBuf) {
                return E_OUT_OF_MEMORY;
            }
            pNewBuf->Copy(m_pBuf->GetPayload(), m_pos);
            BufferOf<Byte>::Free(m_pBuf);
            m_pBuf = pNewBuf;
        }
    }
    return m_pIis->ReadBufferEx(m_pos, m_pBuf->GetCapacity() - m_pos, m_pBuf);
}

ECode CBufferedInputStream::Available(
    /* [out] */ Int32 * pBytes)
{
    Int32 available;
    if (!pBytes) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    ECode ec = m_pIis->Available(&available);
    if (FAILED(ec))  return ec;

    *pBytes = available + (m_pBuf->GetUsed() - m_pos);
    return NOERROR;
}

ECode CBufferedInputStream::Close()
{
    if (m_pIis) {
        m_pIis->Release();
        m_pIis = NULL;
    }

    if (m_pBuf) {
        BufferOf<Byte>::Free(m_pBuf);
        m_pBuf = NULL;
    }

    return NOERROR;
}

ECode CBufferedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    m_markLimit = readLimit;
    m_markpos = m_pos;
    return NOERROR;
}

ECode CBufferedInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    if (!pSupported) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    *pSupported = TRUE;
    return NOERROR;
}

ECode CBufferedInputStream::Read(
    /* [out] */ Byte * pByte)
{
    ECode ec = NOERROR;
    if (!pByte) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    if (m_pos >= m_pBuf->GetUsed()) {
        ec = fill();
        if (FAILED(ec)) {
            return ec;
        }
        if (m_pos >= m_pBuf->GetUsed()) {
            return E_OUT_OF_STREAM;
        }
    }

    *pByte = (*m_pBuf)[m_pos++];
    return ec;
}

/**
  * Read characters into a portion of an array, reading from the underlying
  * stream at most once if necessary.
  */
ECode CBufferedInputStream::read(BufferOf<Byte> *pBuf, Int32 offset, Int32 len,
     Int32 *pRead)
{
    Int32 avail = m_pBuf->GetUsed() - m_pos;

    if (avail <= 0) {
        /* If the requested length is at least as large as the buffer, and
           if there is no mark/reset activity, do not bother to copy the
           bytes into the local buffer.  In this way buffered streams will
           cascade harmlessly. */
        ECode ec;
        if (len >= m_pBuf->GetCapacity() && m_markpos < 0) {
            ec = m_pIis->ReadBufferEx(offset, len, pBuf);
            if (FAILED(ec)) {
                return ec;
            }

            *pRead = pBuf->GetUsed() - offset;
            return NOERROR;
        }
        ec = fill();
        if (FAILED(ec)) {
            return ec;
        }
        avail = m_pBuf->GetUsed() - m_pos;
        if (avail <= 0) {
            return E_OUT_OF_STREAM;
        }
    }

    Int32 cnt = (avail < len) ? avail : len;
    pBuf->Replace(offset, m_pBuf->GetPayload() + m_pos, cnt);
    m_pos += cnt;
    *pRead = cnt;
    return NOERROR;
}

ECode CBufferedInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (pBuffer == NULL || !pBuffer->GetCapacity()) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return ReadBufferEx(0, pBuffer->GetCapacity(), pBuffer);
}

ECode CBufferedInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (pBuffer == NULL || offset < 0 || length < 0
        || (pBuffer->GetCapacity() < length + offset)
        || (pBuffer->GetUsed() < offset)) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    if (length == 0)
        return NOERROR;

    Int32 n = 0;
    Int32 avail;
    ECode ec;
    while (TRUE) {
        Int32 nRead;
        ec = read(pBuffer, offset + n, length - n, &nRead);
        if (FAILED(ec)) {
            return ec;
        }
        n += nRead;

        if (n >= length) {
            break;
        }

        ec = m_pIis->Available(&avail);
        if (FAILED(ec)) {
            return ec;
        }

        if (avail <= 0)
            break;
    }
    return NOERROR;
}

ECode CBufferedInputStream::Reset()
{
    if (m_markpos < 0) {
        return E_INVALID_OPTIONS;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    m_pos = m_markpos;
    return NOERROR;
}

ECode CBufferedInputStream::Skip(
    /* [in] */ Int32 length)
{
    if (length <= 0) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    Int32 avail = m_pBuf->GetUsed() - m_pos;

    if (avail <= 0) {
        if (m_markpos < 0) {
            return m_pIis->Skip(length);
        }

        // Fill in buffer to save bytes for reset
        ECode ec = fill();
        if (FAILED(ec)) {
            return ec;
        }
        avail = m_pBuf->GetUsed() - m_pos;
        if (avail <= 0)
            return E_OUT_OF_STREAM;
    }

    Int32 skipped = (avail < length) ? avail : length;
    m_pos += skipped;
    return NOERROR;
}

ECode CBufferedInputStream::constructor(
    /* [in] */ IInputStream * pStream)
{
    return constructor(pStream, STREAM_BUFFER_SIZE);
}

ECode CBufferedInputStream::constructor(
    /* [in] */ IInputStream * pStream,
    /* [in] */ Int32 bufferSize)
{
    if (!pStream || bufferSize <= 0)
        return E_INVALID_ARGUMENT;

    bufferSize = bufferSize > STREAM_BUFFER_SIZE ?
                 bufferSize : STREAM_BUFFER_SIZE;
    m_pBuf = BufferOf<Byte>::Alloc(bufferSize);
    if (!m_pBuf) {
        return E_OUT_OF_MEMORY;
    }

    m_pIis = pStream;
    m_pIis->AddRef();

    m_pos = 0;
    m_markLimit = -1;
    m_markpos = -1;
    return NOERROR;
}

CBufferedInputStream::~CBufferedInputStream()
{
    Close();
}
