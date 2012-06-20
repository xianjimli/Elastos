//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CTEXTOUTPUTSTREAM_H__
#define __CTEXTOUTPUTSTREAM_H__

#include "_CTextOutputStream.h"

CarClass(CTextOutputStream)
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Byte byte);

    CARAPI WriteBuffer(
        /* [in] */ const BufferOf<Byte> & buffer,
        /* [out] */ Int32 * pBytesWritten);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ const BufferOf<Byte> & buffer,
        /* [out] */ Int32 * pBytesWritten);

    CARAPI GetEncoding(
        /* [out] */ Encoding * pEncoding);

    CARAPI WriteChar(
        /* [in] */ Char16 character);

    CARAPI WriteCharArray(
        /* [in] */ const BufferOf<Char16> & charArray,
        /* [out] */ Int32 * pCharsWritten);

    CARAPI WriteCharArrayEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ const BufferOf<Char16> & charArray,
        /* [out] */ Int32 * pCharsWritten);

    CARAPI WriteString(
        /* [in] */ String string);

    CARAPI constructor(
        /* [in] */ IOutputStream * pStream);

    CARAPI constructor(
        /* [in] */ IOutputStream * pStream,
        /* [in] */ Encoding encoding);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTEXTOUTPUTSTREAM_H__
