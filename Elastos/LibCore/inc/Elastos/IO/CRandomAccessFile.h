#ifndef __CRANDOMACCESSFILE_H__
#define __CRANDOMACCESSFILE_H__

#include "_CRandomAccessFile.h"
#include "CFileDescriptor.h"
#include "Com.Kortide.Platform.h"

CarClass(CRandomAccessFile)
{
public:
    CRandomAccessFile();

    ~CRandomAccessFile();

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ const String& mode);

    CARAPI constructor(
        /* [in] */ const String& fileName,
        /* [in] */ const String& mode);

    CARAPI Close();

    CARAPI GetChannel(
        /* out */ IFileChannel** channel);

    CARAPI GetFD(
        /* out */ IFileDescriptor** fd);

    CARAPI GetFilePointer(
        /* out */ Int64* offset);

    CARAPI Seek(
        /* in */ Int64 offset);

    CARAPI GetLength(
        /* out */ Int64* length);

    CARAPI SetLength(
        /* in */ Int64 length);

    CARAPI Read(
        /* out */ Int32* value);

    CARAPI ReadBuffer(
        /* out */ ArrayOf<Byte>* buffer,
        /* out */ Int32* number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBoolean(
        /* out */ Boolean* value);

    CARAPI ReadByte(
        /* out */ Byte* value);

    CARAPI ReadDouble(
        /* out */ Double* value);

    CARAPI ReadFloat(
        /* out */ Float* value);

    CARAPI ReadChar(
        /* out */ Char32* value);

    CARAPI ReadFully(
        /* out */ ArrayOf<byte>* buffer);

    CARAPI ReadFullyEx(
        /* in */ Int32 offset,
        /* in */ Int32 length,
        /* out */ ArrayOf<Byte>* buffer);

    CARAPI ReadInt32(
        /* out */ Int32* value);

    CARAPI ReadLine(
        /* out */ String* value);

    CARAPI ReadInt64(
        /* out */ Int64* value);

    CARAPI ReadInt16(
        /* out */ Int16* value);

    CARAPI ReadUnsignedByte(
        /* out */ Int32* value);

    CARAPI ReadUnsignedInt16(
        /* out */ Int32* value);

    CARAPI SkipBytes(
        /* in */ Int32 count,
        /* out */ Int32* value);

    CARAPI Write(
        /* in */ Int32 oneByte);

    CARAPI WriteBuffer(
        /* in */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBoolean(
        /* in */ Boolean value);

    CARAPI WriteByte(
        /* in */ Int32 value);

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

private:
    CARAPI OpenCheck();

private:
    /**
     * The FileDescriptor representing this RandomAccessFile.
     */
    AutoPtr<CFileDescriptor> mFd;

    Boolean mSyncMetadata;

    // The unique file channel associated with this FileInputStream (lazily
    // initialized).
    // private FileChannel channel;

    AutoPtr<IFileSystem> mFileSystem;

    Boolean mIsReadOnly;

    // BEGIN android-added
    Int32 mOptions;
    // END android-added
};

#endif //__CRANDOMACCESSFILE_H__
