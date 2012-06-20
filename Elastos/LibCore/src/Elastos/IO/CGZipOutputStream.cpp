//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "CGZipOutputStream.h"
#include <stdio.h>
#include "local.h"

CGZipOutputStream::~CGZipOutputStream()
{
    Close();
}

ECode CGZipOutputStream::constructor(
    /* [in] */ IOutputStream * pStream)
{
    return constructor(pStream, MAX_WBITS, Z_DEFAULT_COMPRESSION,
                MAX_MEM_LEVEL, CompressStrategy_Default);
}

ECode CGZipOutputStream::constructor(
    /* [in] */ IOutputStream * pStream,
    /* [in] */ Int32 windowBits,
    /* [in] */ Int32 compressLevel,
    /* [in] */ Int32 memoryUsageLevel,
    /* [in] */ CompressStrategy strategy)
{
    if (!pStream) {
        return E_INVALID_ARGUMENT;
    }

    memset(&m_stream, 0, sizeof(m_stream));
    switch (deflateInit2(&m_stream, compressLevel, Z_DEFLATED, windowBits,
                memoryUsageLevel, strategy)) {
        case Z_STREAM_ERROR:
            return E_INVALID_ARGUMENT;

        case Z_MEM_ERROR:
            return E_OUT_OF_MEMORY;

        default:
            break;
    }

    m_pBuf = BufferOf<Byte>::Alloc(STREAM_BUFFER_SIZE);
    if (!m_pBuf) {
        deflateEnd(&m_stream);
        return E_OUT_OF_MEMORY;
    }

    m_pIos = pStream;
    m_pIos->AddRef();

    return NOERROR;
}

ECode CGZipOutputStream::constructor(
    /* [in] */ IOutputStream * pStream,
    /* [in] */ Int32 windowBits,
    /* [in] */ Int32 compressLevel,
    /* [in] */ Int32 memoryUsageLevel,
    /* [in] */ CompressStrategy strategy,
    /* [in] */ const BufferOf<Byte> & dictionary)
{
    ECode ec = constructor(pStream, windowBits, compressLevel, memoryUsageLevel,
                    strategy);
    if (FAILED(ec)) {
        return ec;
    }

    if (deflateSetDictionary(&m_stream, dictionary.GetPayload(),
                dictionary.GetUsed()) != Z_OK) {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CGZipOutputStream::Close()
{
    deflateEnd(&m_stream);

    if (m_pIos) {
        m_pIos->Release();
        m_pIos = NULL;
    }

    if (m_pBuf) {
        BufferOf<Byte>::Free(m_pBuf);
        m_pBuf = NULL;
    }

    return NOERROR;
}

ECode CGZipOutputStream::Flush()
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }
    return NOERROR;
}

ECode CGZipOutputStream::Write(
    /* [in] */ Byte value)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }

    ECode ec = DeflateStream(&value, 1, NULL);
    if (FAILED(ec)) {
        return ec;
    }

    return NOERROR;
}

ECode CGZipOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (!m_pIos) {
        return E_CLOSED_STREAM;
    }

    if (buffer.IsNullOrEmpty() || !pBytesWritten) {
        return E_INVALID_ARGUMENT;
    }

    return WriteBufferEx(0, buffer.GetUsed(), buffer, pBytesWritten);
}

ECode CGZipOutputStream::WriteBufferEx(
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

    ECode ec = DeflateStream(buffer.GetPayload() + offset, length, pBytesWritten);
    if (FAILED(ec)) {
        return ec;
    }

    return ec;
}

ECode CGZipOutputStream::DeflateStream(
    Byte * pBuffer,
    Int32 bufferSize,
    Int32 * pCount)
{
    if (pCount) *pCount = 0;

    while (1) {
        m_stream.next_in  = pBuffer;
        m_stream.avail_in  = bufferSize;
        m_stream.next_out = m_pBuf->GetPayload();
        m_stream.avail_out = m_pBuf->GetCapacity();

        Int32 count = 0;
        switch (deflate(&m_stream, Z_SYNC_FLUSH)) {
            case Z_OK:
                pBuffer += bufferSize - m_stream.avail_in;
                bufferSize = m_stream.avail_in;
                count = m_pBuf->GetCapacity() - m_stream.avail_out;
                break;

            case Z_STREAM_ERROR:
                return E_INVALID_ARGUMENT;

            default:
                break;
        }

        if (count) {
            m_pBuf->SetUsed(count);
            ECode ec = m_pIos->WriteBuffer(*m_pBuf, &count);
            if (FAILED(ec)) {
                return ec;
            }
            if (pCount) *pCount += count;
        }
        else {
            break;
        }
    }

    return NOERROR;
}
