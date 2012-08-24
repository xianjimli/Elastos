//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "CTextOutputStream.h"

ECode CTextOutputStream::Close()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::Flush()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::Write(
    /* [in] */ Byte byte)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const BufferOf<Byte> & buffer,
    /* [out] */ Int32 * pBytesWritten)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::GetEncoding(
    /* [out] */ Encoding * pEncoding)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::WriteChar(
    /* [in] */ Char16 character)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::WriteCharArray(
    /* [in] */ const BufferOf<Char16> & charArray,
    /* [out] */ Int32 * pCharsWritten)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::WriteCharArrayEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const BufferOf<Char16> & charArray,
    /* [out] */ Int32 * pCharsWritten)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::WriteString(
    /* [in] */ String string)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::constructor(
    /* [in] */ IOutputStream * pStream)
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CTextOutputStream::constructor(
    /* [in] */ IOutputStream * pStream,
    /* [in] */ Encoding encoding)
{
    // TODO: Add your code here
    return NOERROR;
}
