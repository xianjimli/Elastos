#ifndef __CRANDOMACCESSFILE_H__
#define __CRANDOMACCESSFILE_H__

#include "_CRandomAccessFile.h"
#include "CFileDescriptor.h"
#include "Com.Kortide.Platform.h"

CarClass(CRandomAccessFile)
{
public:
    CRandomAccessFile();
    virtual ~CRandomAccessFile();


   CARAPI Close();

   CARAPI GetChannel(
        /* out */ IFileChannel ** channel);

   CARAPI GetFD(
        /* out */ IFileDescriptor **ppfd);

    CARAPI GetFilePointer(
        /* out */ Int64 *pSeek);
    
    CARAPI Seek(
        /* in */ Int64 nSeek);

    CARAPI GetLength(
        /* out */ Int64 *pLen);

    CARAPI SetLength(
        /* in */ Int64 len);

    CARAPI constructor(
        /* in */ IFile *pFile);

    CARAPI ReadBoolean(
        /* out */ Boolean *pbValue);

    CARAPI ReadBuffer(
        /* out */ ArrayOf<Byte> *buffer,
        /* out */ Int32 *number);

    CARAPI ReadByte(
        /* out */ Byte *pValue);

    CARAPI ReadDouble(
        /* out */ Double *pValue);

    CARAPI ReadFloat(
        /* out */ Float *pValue);

    CARAPI ReadChar(
        /* out */ Char32 *pValue);

    CARAPI ReadFully(
        /* out */ ArrayOf<byte>* buffer);

    CARAPI ReadFullyEx(
        /* in */ Int32 offset,
        /* in */ Int32 length,
        /* out */ ArrayOf<Byte>* buffer);

    CARAPI ReadInt32(
        /* out */ Int32 *pValue);

    CARAPI ReadLine(
        /* out */ String* pValue);

    CARAPI ReadInt64(
        /* out */ Int64* pValue);

    CARAPI ReadInt16(
        /* out */ Int16* pValue);

    CARAPI ReadUnsignedByte(
        /* out */ Int32* pValue);

    CARAPI ReadUnsignedInt16(
        /* out */ Int32* pValue);

    CARAPI SkipBytes(
        /* in */ Int32 count,
        /* out */ Int32* pValue);

    CARAPI WriteBoolean(
        /* in */ Boolean Value);

    CARAPI WriteByte(
        /* in */ Int32 Value);

    CARAPI WriteBytes(
        /* in */ const ArrayOf<byte>& buffer);

    CARAPI WriteBytesEx(
        /* in */ Int32 offset,
        /* in */ Int32 count,
        /* in */ const ArrayOf<byte>& buffer);
    
    CARAPI WriteChar(
        /* in */ Int32 value);

    CARAPI WriteDouble(
        /* in */ Double value);

    CARAPI WriteFloat(
        /* in */ Float value);

    CARAPI WriteInt32(
        /* in */ Int32 value);
        
    CARAPI WriteInt64( 
        /* in */ Int64 value);

    CARAPI WriteInt16(
        /* in */ Int32 value);

    CARAPI Read(
        /* out */ Int32 *pValue);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

private:

    ECode ReadToBuff(
       /* in */ Int32 count,
       /* out */ Int32 *number);

    ECode ReadBufferExLocked(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    ECode OpenCheck();
    
    ECode CloseLocked();
    ECode GetChannelLocked(
        /* out */ IFileChannel **channel);

    Int32 CountUTFBytes(
        /* [in] */ Char32 srcChar);

    void WriteUTFBytesToBuffer(
        /* [in] */ Byte* dstP,
        /* [in] */ Char32 srcChar,
        /* [in] */ Int32 bytes);

    ECode Write(
        /* in */ Int32 oneByte);

    ECode WriteBuffer(
        /* in */ const ArrayOf<Byte>& buffer);

    ECode WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);
private:
    AutoPtr<CFileDescriptor> mFd;
    AutoPtr<IFileSystem> mFileSystem;
    ArrayOf<Byte>*  mBuff;
    ArrayOf<Byte>*  mWriteBuff; 
    Int32 mWritten;
};


#endif __CRANDOMACCESSFILE_H__
