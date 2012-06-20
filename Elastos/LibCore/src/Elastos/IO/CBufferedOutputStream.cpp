//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <stdio.h>
#include "CBufferedOutputStream.h"
#include "local.h"

ECode CBufferedOutputStream::Close()
{
    if (m_pIos) {
        ECode ec = flushBuffer();
        if (FAILED(ec)) {
            return ec;
        }

        m_pIos->Release();
        m_pIos = NULL;
    }

    if (m_pBuf) {
        BufferOf<Byte>::Free(m_pBuf);
        m_pBuf = NULL;
    }

    return NOERROR;
}

ECode CBufferedOutputStream::Flush()
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }

    ECode ec = flushBuffer();
    if (FAILED(ec)) {
        return ec;
    }

    return m_pIos->Flush();
}

ECode CBufferedOutputStream::Write(
    /* [in] */ Byte byte)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }

    if (m_pBuf->GetUsed() >= m_pBuf->GetCapacity()) {
        ECode ec = flushBuffer();
        if (FAILED(ec)) {
            return ec;
        }
    }

    m_pBuf->Append(byte);
    return NOERROR;
}

ECode CBufferedOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (buffer.IsNullOrEmpty() || !pBytesWritten) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }

    return WriteBufferEx(0, buffer.GetUsed(), buffer, pBytesWritten);
}

ECode CBufferedOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (offset < 0 || buffer.IsNullOrEmpty() || length < 0
        || (offset + length > buffer.GetUsed()) || (!pBytesWritten)) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    else if (length == 0) {
        *pBytesWritten = 0;
        return NOERROR;
    }

    ECode ec;
    if (length >= m_pBuf->GetCapacity()) {
        /* If the request length exceeds the size of the output buffer,
           flush the output buffer and then write the data directly.
           In this way buffered streams will cascade harmlessly. */
        ec = flushBuffer();
        if (FAILED(ec)) {
            return ec;
        }

        return m_pIos->WriteBufferEx(offset, length, buffer, pBytesWritten);
    }
    if (length > m_pBuf->GetCapacity() - m_pBuf->GetUsed()) {
        ec = flushBuffer();
        if (FAILED(ec)) {
            return ec;
        }
    }

    m_pBuf->Append(buffer.GetPayload() + offset, length);
    *pBytesWritten = length;
    return NOERROR;
}

ECode CBufferedOutputStream::constructor(
    /* [in] */ IOutputStream * pStream)
{
    return constructor(pStream, STREAM_BUFFER_SIZE);
}

ECode CBufferedOutputStream::constructor(
    /* [in] */ IOutputStream * pStream,
    /* [in] */ Int32 bufferSize)
{
    if (bufferSize <= 0 || !pStream) {
        return E_INVALID_ARGUMENT;
    }

    bufferSize = bufferSize > STREAM_BUFFER_SIZE ?
                 bufferSize : STREAM_BUFFER_SIZE;
    m_pBuf = BufferOf<Byte>::Alloc(bufferSize);
    if (!m_pBuf) {
        return E_OUT_OF_MEMORY;
    }

    m_pIos = pStream;
    m_pIos->AddRef();

    return NOERROR;
}

CBufferedOutputStream::~CBufferedOutputStream()
{
    Close();
}

ECode CBufferedOutputStream::flushBuffer()
{
    Int32 written;

    if (m_pBuf->GetUsed() > 0) {
        ECode ec = m_pIos->WriteBufferEx(0, m_pBuf->GetUsed(),
            *m_pBuf, &written);
        if (FAILED(ec)) {
            return ec;
        }
        m_pBuf->SetUsed(0);
    }
    return NOERROR;
}
