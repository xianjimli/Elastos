//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <stdio.h>
#include <stdlib.h>
#include "CByteArrayOutputStream.h"

ECode CByteArrayOutputStream::Close()
{
    m_pBuf = NULL;
    return NOERROR;
}

ECode CByteArrayOutputStream::Flush()
{
    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }
    return NOERROR;
}

/**
  * @param   byte the Byte to be written
  */
ECode CByteArrayOutputStream::Write(
    /* [in] */ Byte byte)
{
    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }
    if (m_pos >= m_tail) {
        return E_OUT_OF_STREAM;
    }

    m_pBuf->Replace(m_pos, &byte, 1);
    m_pos++;

    return NOERROR;
}

ECode CByteArrayOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (buffer.IsNullOrEmpty() || !pBytesWritten) {
        return E_INVALID_ARGUMENT;
    }
    if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }

    return WriteBufferEx(0, buffer.GetUsed(), buffer, pBytesWritten);
}

/**
  * Writes <code>len</code> bytes from the specified byte array
  * starting at offset <code>offset</code> to this byte array output stream.
  */
ECode CByteArrayOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (offset < 0 || buffer.IsNullOrEmpty() || length < 0 || (!pBytesWritten)
        || (offset + length > buffer.GetUsed())) {
        return E_INVALID_ARGUMENT;
    }
    else if (!m_pBuf) {
        return E_CLOSED_STREAM;
    }
    else if (length == 0) {
        *pBytesWritten = 0;
        return NOERROR;
    }

    if (m_pos >= m_tail) {
        return E_OUT_OF_STREAM;
    }

    if (length + m_pos > m_tail) {
        length = m_tail - m_pos;
    }

    m_pBuf->Replace(m_pos, buffer.GetPayload() + offset, length);
    m_pos += length;
    *pBytesWritten = length;
    return NOERROR;
}

/*
 * m_pBuf->GetUsed() presents the capacity for write.
 * default value = 32;
 */
ECode CByteArrayOutputStream::constructor(
    /* [in] */ const BufferOf<Byte> & buffer)
{
    if (buffer.IsNull()) {
        return E_INVALID_ARGUMENT;
    }
    return constructor(buffer, 0, buffer.GetCapacity());
}

ECode CByteArrayOutputStream::constructor(
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (buffer.IsNull() || offset < 0 || length < 0
        || (offset + length > buffer.GetCapacity())) {
        return E_INVALID_ARGUMENT;
    }

    m_pBuf = (BufferOf<Byte> *)&buffer;
    m_pos = offset;
    m_tail = offset + length;
    return NOERROR;
}
