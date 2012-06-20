//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <elasys_server.h>
#include <elapi.h>
#include <stdlib.h>
#include <_pubcrt.h>
#include "CFileInputStream.h"

ECode CFileInputStream::Available(
    /* [out] */ Int32 * pBytes)
{
    if (!pBytes) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    Int32 size;
    ECode ec = m_pIab->GetSize(&size);
    if(FAILED(ec)) {
        return ec;
    }

    *pBytes = size - m_pos;
    return NOERROR;
}

ECode CFileInputStream::Close()
{
    if (m_pIab) {
        m_pIab->Release();
        m_pIab = NULL;
    }

    return NOERROR;
}

ECode CFileInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    m_mark = m_pos;
    return NOERROR;
}

ECode CFileInputStream::IsMarkSupported(
    /* [out] */ Boolean * pSupported)
{
    if (!pSupported) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    *pSupported = TRUE;
    return NOERROR;
}

/**
 * Read a byte from File InputStream when success, whereas return null;
 */
ECode CFileInputStream::Read(
    /* [out] */ Byte * pByte)
{
    if (!pByte) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    BufferOf<Byte> tmp(pByte, 1);
    return ReadBufferEx(0, 1, &tmp);
}

ECode CFileInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (pBuffer == NULL || !pBuffer->GetCapacity()) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    return ReadBufferEx(0, pBuffer->GetCapacity(), pBuffer);
}

/**
  * Reads up to <code>len</code> bytes of data from this input stream
  * into an array of bytes. If <code>len</code> is not zero, the method
  * blocks until some input is available; otherwise, no
  * bytes are read and <code>0</code> is returned.
  *
  * @param      buffer     the buffer into which the data is read.
  * @param      offset   the start offset in the destination array
  *                      <code>buffer</code>
  * @param      len   the maximum number of bytes read.
  */
ECode CFileInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte> * pBuffer)
{
    if (pBuffer == NULL || offset < 0 || length < 0
        || (pBuffer->GetCapacity() < length + offset)
        || (pBuffer->GetUsed() < offset)) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    if (length > 0) {
        MemoryBuf tmp(pBuffer->GetPayload() + offset, length);
        ECode ec = m_pIab->Read(m_pos, length, &tmp);
        if (FAILED(ec)) {
            return ec;
        }

        if (tmp.GetUsed() == 0)
            return E_OUT_OF_STREAM;

        pBuffer->SetUsed(offset + tmp.GetUsed());
        m_pos += tmp.GetUsed();
    }
    return NOERROR;
}

ECode CFileInputStream::Reset()
{
    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    m_pos = m_mark;
    return NOERROR;
}

ECode CFileInputStream::Skip(
    /* [in] */ Int32 length)
{
    if (length <= 0) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    Int32 size;
    ECode ec = m_pIab->GetSize(&size);
    if (FAILED(ec))  {
        return ec;
    }

    if (m_pos + length > size)
        m_pos = size;
    else
        m_pos += length;

    return NOERROR;
}

CFileInputStream::~CFileInputStream()
{
    Close();
}

/*
 * If the named file does not exist, is a directory rather than a regular
 * file, or for some other reason cannot be opened for reading then a
 * <code>FileNotFoundException</code> is thrown.
 */
ECode CFileInputStream::constructor(
    /* [in] */ String fileName)
{
    if (fileName.IsNullOrEmpty())
        return E_INVALID_ARGUMENT;

    m_pIab = new FileStream;
    if (!m_pIab) {
        return E_OUT_OF_MEMORY;
    }
	return m_pIab->OpenRead(fileName);
}
