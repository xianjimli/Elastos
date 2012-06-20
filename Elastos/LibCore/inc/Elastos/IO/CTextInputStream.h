//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CTEXTINPUTSTREAM_H__
#define __CTEXTINPUTSTREAM_H__

#include "_CTextInputStream.h"

CarClass(CTextInputStream)
{
public:
    CARAPI Available(
        /* [out] */ Int32 * pBytes);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean * pSupported);

    CARAPI Read(
        /* [out] */ Byte * pByte);

    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte> * pBuffer);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte> * pBuffer);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int32 length);

    CARAPI GetEncoding(
        /* [out] */ Encoding * pEncoding);

    CARAPI ReadChar(
        /* [out] */ Char16 * pCharacter);

    CARAPI ReadCharArray(
        /* [out] */ BufferOf<Char16> * pcharArray);

    CARAPI ReadCharArrayEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Char16> * pcharArray);

    CARAPI ReadString(
        /* [out] */ StringBuf * pString);

    CARAPI constructor(
        /* [in] */ IInputStream * pStream);

    CARAPI constructor(
        /* [in] */ IInputStream * pStream,
        /* [in] */ Encoding encoding);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTEXTINPUTSTREAM_H__
