//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "CGZipInputStream.h"
#include <stdio.h>
#include "local.h"

CGZipInputStream::~CGZipInputStream()
{
    Close();
}

ECode CGZipInputStream::constructor(
    /* [in] */ IInputStream * pStream)
{
    return constructor(pStream, MAX_WBITS);
}

ECode CGZipInputStream::constructor(
    /* [in] */ IInputStream * pStream,
    /* [in] */ Int32 windowBits)
{
    if (!pStream) {
        return E_INVALID_ARGUMENT;
    }

    memset(&m_stream, 0, sizeof(m_stream));
    switch (inflateInit2(&m_stream, windowBits)) {
        case Z_STREAM_ERROR:
            return E_INVALID_ARGUMENT;

        case Z_MEM_ERROR:
            return E_OUT_OF_MEMORY;

        default:
            break;
    }

    m_pBuf = BufferOf<Byte>::Alloc(STREAM_BUFFER_SIZE);
    if (!m_pBuf) {
        inflateEnd(&m_stream);
        return E_OUT_OF_MEMORY;
    }

    m_pIis = pStream;
    m_pIis->AddRef();

    return NOERROR;
}

ECode CGZipInputStream::constructor(
    /* [in] */ IInputStream * pStream,
    /* [in] */ Int32 windowBits,
    /* [in] */ const BufferOf<Byte> & dictionary)
{
    ECode ec = constructor(pStream, windowBits);
    if (FAILED(ec)) {
        return ec;
    }

    m_pDict = &dictionary;
    return NOERROR;
}

ECode CGZipInputStream::fill()
{
    if (m_stream.avail_in <= 0) {
        ECode ec = m_pIis->ReadBuffer(m_pBuf);
        if (FAILED(ec) && ec != E_OUT_OF_STREAM) {
            return ec;
        }

        m_stream.next_in = m_pBuf->GetPayload();
        m_stream.avail_in = m_pBuf->GetUsed();

        if (m_pBuf->GetAvailableSpace() > 0) {
            m_isEof = TRUE;
        }

        m_pBuf->SetUsed(0);
    }
    return NOERROR;
}

ECode CGZipInputStream::Available(
    /* [out] */ Int32 * pBytes)
{
    ECode ec = NOERROR;
    if (!pBytes) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    BufferOf<Byte> *temp = BufferOf<Byte>::Alloc(STREAM_BUFFER_SIZE);
    if (!temp) {
        return E_OUT_OF_MEMORY;
    }

    *pBytes = 0;
    Int32 count = bytesRead;
    Int32 read = 0;
    while (1) {
        ec = InflateStream(temp->GetPayload(), STREAM_BUFFER_SIZE, &read);
        if (FAILED(ec) && ec != E_OUT_OF_STREAM) {
            return ec;
        }

        if (read) *pBytes += read;
        else break;
    }

    // reset the stream and start again..
    ec = Reset();
    if (FAILED(ec)) {
        return ec;
    }

    while (count > 0) {
        Int32 len = count < STREAM_BUFFER_SIZE ? count : STREAM_BUFFER_SIZE;
        ec = InflateStream(temp->GetPayload(), len, &read);
        if (FAILED(ec)) {
            return ec;
        }

        count -= read;
    }

    BufferOf<Byte>::Free(temp);
    return NOERROR;
}

ECode CGZipInputStream::Close()
{
    inflateEnd(&m_stream);

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

ECode CGZipInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }
    return E_NOT_SUPPORTED;
}

ECode CGZipInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    if (!pSupported) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    *pSupported = FALSE;
    return NOERROR;
}

ECode CGZipInputStream::Read(
    /* [out] */ Byte * pValue)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }

    return InflateStream(pValue, 1, NULL);

}

ECode CGZipInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    if (!pBuffer || !pBuffer->GetCapacity()) {
        return E_INVALID_ARGUMENT;
    }

    return ReadBufferEx(0, pBuffer->GetCapacity(), pBuffer);
}

ECode CGZipInputStream::ReadBufferEx(
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
    else if (length == 0) {
        pBuffer->SetUsed(0);
        return NOERROR;
    }

    Int32 bytestoread = 0;
    ECode ec = InflateStream(pBuffer->GetPayload() + offset, length, &bytestoread);
    if (FAILED(ec)) {
        return ec;
    }

    pBuffer->SetUsed(bytestoread + offset);
    return NOERROR;
}

ECode CGZipInputStream::Reset()
{
    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    m_stream.next_in = NULL;
    m_stream.avail_in = 0;
    m_isEof = FALSE;
    bytesRead = 0;

    if (inflateReset(&m_stream) != Z_OK) {
        return E_INVALID_POINTER;
    }

    return m_pIis->Reset();
}

ECode CGZipInputStream::Skip(
    /* [in] */ Int32 length)
{
    if (length <= 0) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIis) {
        return E_CLOSED_STREAM;
    }

    Int32 skip = length > STREAM_BUFFER_SIZE ? STREAM_BUFFER_SIZE : length;
    BufferOf<Byte> *temp = BufferOf<Byte>::Alloc(skip);
    if (!temp) {
        return E_OUT_OF_MEMORY;
    }

    while (length > 0) {
        ECode ec = ReadBuffer(temp);
        if (FAILED(ec)) {
            return ec;
        }
        length -= temp->GetUsed();

        if (m_isEof) break;
    }

    BufferOf<Byte>::Free(temp);
    return NOERROR;
}

ECode CGZipInputStream::InflateStream(
    Byte *pBuffer,
    Int32 bufferSize,
    Int32 *pCount)
{
    Int32 readCount = 0;

    while(bufferSize > 0) {
        m_stream.next_out = pBuffer;
        m_stream.avail_out = bufferSize;

        Int32 count = 0;
        switch (inflate(&m_stream, Z_SYNC_FLUSH)) {
            case Z_OK:
                count = bufferSize - m_stream.avail_out;
                break;

            case Z_NEED_DICT:
                if (!m_pDict) return E_INVALID_ARGUMENT;

                if (inflateSetDictionary(&m_stream, m_pDict->GetPayload(),
                            m_pDict->GetUsed()) != Z_OK) {
                    return E_BAD_FILE_FORMAT;
                }

                if (inflate(&m_stream, Z_SYNC_FLUSH) == Z_OK) {
                    count = bufferSize - m_stream.avail_out;
                    break;
                }

            case Z_DATA_ERROR:
                return E_BAD_FILE_FORMAT;

            case Z_MEM_ERROR:
                return E_OUT_OF_MEMORY;

            default:
                break;
        }

        if (count == 0) {
            if (m_isEof) {
                break;
            }

            ECode ec = fill();
            if (FAILED(ec)) {
                return ec;
            }
        }
        else {
            readCount += count;
            pBuffer += count;
            bufferSize -= count;
        }
    }

    bytesRead += readCount;
    if(pCount) *pCount = readCount;

    if (readCount == 0) {
        return E_OUT_OF_STREAM;
    }

    return NOERROR;
}
