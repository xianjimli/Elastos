//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <stdlib.h>
#include <elasys_server.h>
#include <elapi.h>
#include <_pubcrt.h>
#include "CFileOutputStream.h"

CFileOutputStream::~CFileOutputStream()
{
    Close();
}

ECode CFileOutputStream::Close()
{
    if (m_pIab) {
        m_pIab->Release();
        m_pIab = NULL;
    }

    return NOERROR;
}

ECode CFileOutputStream::Flush()
{
    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    return m_pIab->Flush();
}

ECode CFileOutputStream::Write(
    /* [in] */ Byte byte)
{
    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    BufferOf<Byte> tmp(&byte, 1);
    Int32 nWritten;
    return WriteBufferEx(0, 1, tmp, &nWritten);
}

ECode CFileOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (buffer.IsNullOrEmpty() || !pBytesWritten) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    return WriteBufferEx(0, buffer.GetUsed(), buffer, pBytesWritten);
}

/**
  * Writes <code>len</code> bytes from the specified byte array
  * starting at offset <code>offset</code> to this file output stream.
  */
ECode CFileOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    if (offset < 0 || length <= 0 || offset+length > buffer.GetUsed() || !pBytesWritten) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pIab) {
        return E_CLOSED_STREAM;
    }

    MemoryBuf tmp(buffer.GetPayload() + offset, length);

    ECode ec = m_pIab->Write(m_pos, tmp, pBytesWritten);
    if (SUCCEEDED(ec)) {
        m_pos += *pBytesWritten;
    }

    return ec;
}

ECode CFileOutputStream::constructor(
    /* [in] */ String fileName,
    /* [in] */ Boolean append)
{
    if (fileName.IsNullOrEmpty())
        return E_INVALID_ARGUMENT;

    m_pIab = new FileStream;
    if (!m_pIab) {
        return E_OUT_OF_MEMORY;
    }
    return m_pIab->OpenWrite(fileName, append);
}
