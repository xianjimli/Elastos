#ifndef __CDATAINPUTSTREAM_H__
#define __CDATAINPUTSTREAM_H__

#include "_CDataInputStream.h"
#include "DataInputStream.h"

using namespace Elastos;

CarClass(CDataInputStream), public DataInputStream
{
public:
    CDataInputStream();

    ~CDataInputStream();

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI ReadBoolean(
        /* [out] */ Boolean* value);

    CARAPI ReadByte(
        /* [out] */ Byte* value);

    CARAPI ReadChar(
        /* [out] */ Char32* value);

    CARAPI ReadDouble(
        /* [out] */ Double* value);

    CARAPI ReadFloat(
        /* [out] */ Float* value);

    CARAPI ReadFully(
        /* [out] */ ArrayOf<Byte>* buffer);

    CARAPI ReadFullyEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer);

    CARAPI ReadInt32(
        /* [out] */ Int32* value);

    CARAPI ReadLine(
         /* [out] */ String* value);

    CARAPI ReadInt64(
        /* [out] */ Int64* value);

    CARAPI ReadInt16(
        /* [out] */ Int16* value);

    CARAPI ReadUnsignedByte(
        /* [out] */ Int32* value);

    CARAPI ReadUnsignedInt16(
        /* [out] */ Int32* value);

    CARAPI SkipBytes(
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif