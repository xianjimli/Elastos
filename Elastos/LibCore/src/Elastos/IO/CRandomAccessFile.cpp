#include "CRandomAccessFile.h"
#include "CFile.h"
#include "cmdef.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <elastos/Math.h>
#include <elastos/Character.h>

using namespace Elastos::Core;

static const Char32 kByteMask = 0x000000BF;
static const Char32 kByteMark = 0x00000080;

static const Char32 kFirstByteMark[] = {
    0x00000000, 0x00000000, 0x000000C0, 0x000000E0, 0x000000F0
};

extern ECode Platform2IO(ECode ec);

CRandomAccessFile::CRandomAccessFile()
{
   AutoPtr<IPlatform> platform;
   ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
   ASSERT_SUCCEEDED(platform->GetFileSystem((IFileSystem**)&mFileSystem));
}

CRandomAccessFile::~CRandomAccessFile()
{
}

ECode CRandomAccessFile::Close()
{
//    Mutex* selfLock = GetSelfLock();
//    Mutex::Autolock lock(*selfLock);

    return CloseLocked();
}

ECode CRandomAccessFile::CloseLocked()
{
    Boolean isValid;
    if (mFd != NULL && (mFd->Valid(&isValid), isValid)) {
        Int32 fd =  mFd->mDescriptor;
        Int32 rc = TEMP_FAILURE_RETRY(close(fd));
        if (rc == -1) {
//            jniThrowIOException(env, errno);
            return E_IO_EXCEPTION;
        }
        mFd->mDescriptor = -1;
    }
    // END android-changed
    return NOERROR;
}

ECode CRandomAccessFile::GetChannel(
    /* out */ IFileChannel **channel)
{
//    Mutex* selfLock = GetSelfLock();
//    Mutex::Autolock lock(*selfLock);
        
    return GetChannelLocked(channel);
}

ECode CRandomAccessFile::GetChannelLocked(
   /* out */ IFileChannel **channel)
{
    return E_NOT_IMPLEMENTED;;
}

ECode CRandomAccessFile::GetFD(
    /* out */ IFileDescriptor **ppfd)
{
    assert(ppfd != NULL);
    *ppfd = mFd;
    if (*ppfd != NULL) (*ppfd)->AddRef();
    return NOERROR;
}

ECode CRandomAccessFile::GetFilePointer(
    /* out */ Int64 *pSeek)
{
    assert(pSeek != NULL);
    mFileSystem->Seek(mFd->mDescriptor, 0, 2, pSeek);
    return NOERROR;
}

ECode CRandomAccessFile::Seek(
    /* in */ Int64 nSeek)
{
    Int64 result;
    ECode ec = NOERROR;
    ec = mFileSystem->Seek(mFd->mDescriptor, nSeek, 1, &result);
    if (result == -1) {
    } else {
    }
    return ec;
}

ECode CRandomAccessFile::GetLength(
    /* out */ Int64 *pLen)
{
    assert(pLen != NULL);
    mFileSystem->GetLength(mFd->mDescriptor, pLen);
    return NOERROR;
}

ECode CRandomAccessFile::SetLength(
   /* in */ Int64 len)
{
    return NOERROR;
}

ECode CRandomAccessFile::OpenCheck()
{
    return NOERROR;
}

ECode CRandomAccessFile::constructor(
    /* in */ IFile *pFile)
//    /* in */ String mode)
{
    if ( NULL == pFile ) {
        return E_NULL_POINTER_EXCEPTION;
    }	
 
    ECode ec = NOERROR;
    String path;
    pFile->GetAbsolutePath(&path);
    ec = CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd);
    if (FAILED(ec)) {
    	return ec;
    }
    ec = mFileSystem->Open(path, IFileSystem_O_RDWR, &mFd->mDescriptor);
    return ec;
}

ECode CRandomAccessFile::Read(
    /* out */ Int32 *pValue)
{
    assert(pValue != NULL);
    ArrayOf_<Byte, 1> readed;
    Int32 result;
    FAIL_RETURN(ReadBufferEx(0, 1, &readed, &result));
    *pValue = result == -1 ? -1 : readed[0] & 0xff;
    return NOERROR;;
}

ECode CRandomAccessFile::ReadBuffer(
    /* out */ ArrayOf<Byte> *buffer,
    /* out */ Int32 *number)
{
    assert(number != NULL);
    assert(buffer != NULL);
    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

ECode CRandomAccessFile::ReadToBuff(
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    Int32 offset = 0;
    while(offset < count) {
        FAIL_RETURN(ReadBufferEx(offset, count - offset, mBuff, number));
        if(*number == -1) return NOERROR;
        offset += *number;
    }

    return NOERROR;
}

ECode CRandomAccessFile::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // made implicit null check explicit,
    // used (offset | count) < 0 instead of (offset < 0) || (count < 0)
    // to safe one operation
    if (buffer == NULL) {
//      throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if ((offset | length) < 0 || length > buffer->GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed

    if (0 == length) {
        *number = 0;
        return NOERROR;
    }

    FAIL_RETURN(OpenCheck());
    //Mutex::Autolock lock(repositioningLock);
    return ReadBufferExLocked(offset, length, buffer, number);
}

ECode CRandomAccessFile::ReadBufferExLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{   
    assert(number != NULL);
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // made implicit null check explicit,
    // used (offset | count) < 0 instead of (offset < 0) || (count < 0)
    // to safe one operation
    if (buffer == NULL) {
//      throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    
    if ((offset | length) < 0 || length > buffer->GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
        
    if (0 == length) {
        *number = 0;
        return NOERROR;
    }
        
    FAIL_RETURN(OpenCheck());

    // BEGIN android-changed
    // If you only support Linux, there's nothing special about stdin.
    Int64 val;
    FAIL_RETURN(Platform2IO(mFileSystem->Read(
            mFd->mDescriptor, buffer, offset, length, &val)));
    *number = (Int32)val;
    return NOERROR;
    // END android-changed
}

ECode CRandomAccessFile::ReadBoolean(
    /* out */ Boolean *value)
{
    assert(value != NULL);

    Int32 temp;
    FAIL_RETURN(Read(&temp));
    if (temp < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = temp != 0;
    return NOERROR;
}


ECode CRandomAccessFile::ReadByte(
    /* out */ Byte *value)
{
    assert(value != NULL);

    Int32 temp;
    FAIL_RETURN(Read(&temp));
    if (temp < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = (Byte)temp;
    return NOERROR;
}


ECode CRandomAccessFile::ReadChar(
    /* out */ Char32 *value)
{
    assert(value != NULL);

    Int32 number;
    FAIL_RETURN(ReadToBuff(4, &number));
    if (number < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = String((const char*)mBuff->GetPayload()).GetChar(0);

    return NOERROR;
}


ECode CRandomAccessFile::ReadDouble(
    /* out */ Double *value)
{
    assert(value != NULL);

    Int64 int64Value;
    FAIL_RETURN(ReadInt64(&int64Value));
    *value = Math::Int64BitsToDouble(int64Value);
    return NOERROR;
}


ECode CRandomAccessFile::ReadFloat(
    /* out */ Float *value)
{
    assert(value != NULL);

    Int32 int32Value;
    FAIL_RETURN(ReadInt32(&int32Value));
    *value = Math::Int32BitsToFloat(int32Value);
    return NOERROR;
}


ECode CRandomAccessFile::ReadFully(
    /* out */ ArrayOf<Byte>* buffer)
{
    return ReadFullyEx(0, buffer->GetLength(), buffer);
}

ECode CRandomAccessFile::ReadFullyEx(
        /* in */ Int32 offset,
        /* in */ Int32 length,
        /* out */ ArrayOf<byte>* buffer)
{
    // BEGIN android-removed
    // if (length < 0) {
    //     throw new IndexOutOfBoundsException();
    // }
    // END android-removed
    if(length == 0) return NOERROR;

    if (buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // used (offset | length) < 0 instead of separate (offset < 0) and
    // (length < 0) check to safe one operation
    if ((offset | length) < 0 || offset > buffer->GetLength() - length) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    Int32 number;
    while (length > 0) {
        FAIL_RETURN(ReadBufferEx(offset, length, buffer, &number));
        if (number < 0) return E_EOF_EXCEPTION;
        offset += number;
        length -= number;
    }
    return NOERROR;
}

ECode CRandomAccessFile::ReadInt32(
        /* out */ Int32 *value)
{
    assert(value != NULL);

    Int32 number;
    FAIL_RETURN(ReadToBuff(4, &number));
    if (number < 0) return E_EOF_EXCEPTION;
    *value = (((*mBuff)[0] & 0xff) << 24) | (((*mBuff)[1] & 0xff) << 16) |
             (((*mBuff)[2] & 0xff) << 8) | ((*mBuff)[3] & 0xff);

    return NOERROR;
}

ECode CRandomAccessFile::ReadLine(
        /* out */ String* str)
{
    assert(str != NULL);
    StringBuf* line = StringBuf::Alloc(80);
    Boolean foundTerminator = FALSE;

    while (TRUE) {
        Int32 nextByte;
        FAIL_RETURN(Read(&nextByte));
        switch (nextByte) {
            case -1:
                if (line->GetLength() == 0 && !foundTerminator) {
                    *str = NULL;
                    StringBuf::Free(line);
                    return NOERROR;
                }
                *str = line->GetPayload();
                StringBuf::Free(line);
                return NOERROR;
            case (Byte)'\r':
                if (foundTerminator) {
//                    ((PushbackInputStream) mIn).unread(nextByte);///////////not implemented
                    *str = line->GetPayload();
                    StringBuf::Free(line);
                    return NOERROR;
                }
                foundTerminator = TRUE;
                /* Have to be able to peek ahead one byte */ ///////////////not implemented
//                if (!(in.getClass() == PushbackInputStream.class)) {
//                    in = new PushbackInputStream(in);
//                }
                break;
            case (Byte)'\n':
                *str = line->GetPayload();
                StringBuf::Free(line);
                return NOERROR;
            default:
                if (foundTerminator) {
//                    ((PushbackInputStream) in).unread(nextByte);//////////////not implemented
                    *str = line->GetPayload();
                    StringBuf::Free(line);
                    return NOERROR;
                }
                line->Append((Char8)nextByte);
        }
    }
    return NOERROR;
}

ECode CRandomAccessFile::ReadInt64(
        /* out */ Int64* value)
{
    assert(value != NULL);

    Int32 number;
    FAIL_RETURN(ReadToBuff(8, &number));
    if (number < 0) return E_EOF_EXCEPTION;
    Int32 i1 = (((*mBuff)[0] & 0xff) << 24) | (((*mBuff)[1] & 0xff) << 16) |
               (((*mBuff)[2] & 0xff) << 8) | ((*mBuff)[3] & 0xff);
    Int32 i2 = (((*mBuff)[4] & 0xff) << 24) | (((*mBuff)[5] & 0xff) << 16) |
               (((*mBuff)[6] & 0xff) << 8) | ((*mBuff)[7] & 0xff);

    *value = ((i1 & 0xffffffffLL) << 32) | (i2 & 0xffffffffLL);

    return NOERROR;
}

ECode CRandomAccessFile::ReadInt16(
        /* out */ Int16* value)
{
    assert(value != NULL);

    Int32 number;

    FAIL_RETURN(ReadToBuff(2, &number));
    if (number < 0) return E_EOF_EXCEPTION;
    *value = (Int16)((((*mBuff)[0] & 0xff) << 8) | ((*mBuff)[1] & 0xff));

    return NOERROR;
}

ECode CRandomAccessFile::ReadUnsignedByte(
        /* out */ Int32* value)
{
    assert(value != NULL);
    
    FAIL_RETURN(Read(value));
    return *value < 0? E_EOF_EXCEPTION : NOERROR;
}

ECode CRandomAccessFile::ReadUnsignedInt16(
        /* out */ Int32* value)
{
    assert(value != NULL);

    Int32 number;
    FAIL_RETURN(ReadToBuff(2, &number));
    if (number < 0) return E_EOF_EXCEPTION;
    *value = (UInt16)((((*mBuff)[0] & 0xff) << 8) | ((*mBuff)[1] & 0xff));

    return NOERROR;
}

ECode CRandomAccessFile::SkipBytes(
        /* in */ Int32 count,
        /* out */ Int32* number)
{
    assert(number != NULL);

    Int32 skipped = 0;
    Int64 skip;

    //while (skipped < count && (Skip(count - skipped, &skip), skip != 0)) {

    //    skipped += skip;
    //}
    // BEGIN android-removed
    // if (skipped < 0) {
    //     return E_IO_EXCEPTION;//throw new EOFException();
    // }
    // END android-removed
    *number = skipped;

    return NOERROR;
}


ECode CRandomAccessFile::Write(
        /* in */ Int32 oneByte)
{
    FAIL_RETURN(OpenCheck());

    ArrayOf_<Byte, 1> byteArray;
    (byteArray)[0] = (Byte)oneByte;
    Int64 number;
    return Platform2IO(mFileSystem->Write(
            mFd->mDescriptor, byteArray, 0, 1, &number));
}


ECode CRandomAccessFile::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}


ECode CRandomAccessFile::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, made implicit null check explicit,
    // used (offset | count) < 0 instead of (offset < 0) || (count < 0)
    // to safe one operation
    if (buffer.GetPayload() == NULL) {
//        throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if ((offset | count) < 0 || count > buffer.GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed

    if (0 == count) {
        return NOERROR;
    }

    FAIL_RETURN(OpenCheck());
    Int64 number;
    return Platform2IO(mFileSystem->Write(
            mFd->mDescriptor, buffer, offset, count, &number));
}



ECode CRandomAccessFile::WriteBoolean(
        /* in */ Boolean value)
{
    FAIL_RETURN(Write(value? 1 : 0));
    mWritten++;
    return NOERROR;
}

ECode CRandomAccessFile::WriteByte(
        /* in */ Int32 value)
{
    FAIL_RETURN(Write(value));
    mWritten++;
    return NOERROR;
}

ECode CRandomAccessFile::WriteBytes(
        /* in */ const ArrayOf<Byte>& buffer)
{
    FAIL_RETURN(WriteBuffer(buffer));
    mWritten += buffer.GetLength();
    return NOERROR;
}


ECode CRandomAccessFile::WriteBytesEx(
        /* in */ Int32 count,
        /* in */ Int32 length,
        /* in */ const ArrayOf<byte>& buffer)
{
    return WriteBufferEx(length, count, buffer);
}

ECode CRandomAccessFile::WriteChar(
        /* in */ Int32 value)
{
    Int32 len = CountUTFBytes((Char32)value);
    WriteUTFBytesToBuffer(mWriteBuff->GetPayload(), (Char32)value, len);
    FAIL_RETURN(WriteBufferEx(0, len, *mWriteBuff));
    mWritten += len;
    return NOERROR;
}

ECode CRandomAccessFile::WriteDouble(
        /* in */ Double value)
{
    return WriteInt64(Math::DoubleToInt64Bits(value));
}

ECode CRandomAccessFile::WriteFloat(
        /* in */ Float value)
{
    return WriteInt32(Math::FloatToInt32Bits(value));
}

ECode CRandomAccessFile::WriteInt32(
        /* in */ Int32 value)
{
    (*mBuff)[0] = (Byte)(value >> 24);
    (*mBuff)[1] = (Byte)(value >> 16);
    (*mBuff)[2] = (Byte)(value >> 8);
    (*mBuff)[3] = (Byte)value;
    FAIL_RETURN(WriteBufferEx(0, 4, *mWriteBuff));
    mWritten += 4;
    return NOERROR;
}        

ECode CRandomAccessFile::WriteInt64( 
        /* in */ Int64 value)
{
    (*mBuff)[0] = (Byte)(value >> 56);
    (*mBuff)[1] = (Byte)(value >> 48);
    (*mBuff)[2] = (Byte)(value >> 40);
    (*mBuff)[3] = (Byte)(value >> 32);
    (*mBuff)[4] = (Byte)(value >> 24);
    (*mBuff)[5] = (Byte)(value >> 16);
    (*mBuff)[6] = (Byte)(value >> 8);
    (*mBuff)[7] = (Byte)value;
    FAIL_RETURN(WriteBufferEx(0, 8, *mWriteBuff));
    mWritten += 8;
    return NOERROR;
}

ECode CRandomAccessFile::WriteInt16(
        /* in */ Int32 value)
{
    (*mBuff)[0] = (Byte)(value >> 8);
    (*mBuff)[1] = (Byte)value;
    FAIL_RETURN(WriteBufferEx(0, 2, *mWriteBuff));
    mWritten += 2;
    return NOERROR;
}

Int32 CRandomAccessFile::CountUTFBytes(
        /* [in] */ Char32 srcChar)
{
    Int32 bytesToWrite;

    // Figure out how many bytes the result will require.
    if (srcChar < 0x00000080)
    {
        bytesToWrite = 1;
    }
    else if (srcChar < 0x00000800)
    {
        bytesToWrite = 2;
    }
    else if (srcChar < 0x00010000)
    {
        if ((srcChar < Character::MIN_HIGH_SURROGATE)
         || (srcChar > Character::MAX_LOW_SURROGATE))
        {
            bytesToWrite = 3;
        }
        else
        {
            // Surrogates are invalid UTF-32 characters.
            return 0;
        }
    }
    // Max code point for Unicode is 0x0010FFFF.
    else if (srcChar <= Character::MAX_CODE_POINT)
    {
        bytesToWrite = 4;
    }
    else
    {
        // Invalid UTF-32 character.
        return 0;
    }

    return bytesToWrite;
}


void CRandomAccessFile::WriteUTFBytesToBuffer(
        /* [in] */ Byte* dstP,
        /* [in] */ Char32 srcChar,
        /* [in] */ Int32 bytes)
{
    dstP += bytes;
    switch (bytes)
    {   /* note: everything falls through. */
        case 4: *--dstP = (Byte)((srcChar | kByteMark) & kByteMask); srcChar >>= 6;
        case 3: *--dstP = (Byte)((srcChar | kByteMark) & kByteMask); srcChar >>= 6;
        case 2: *--dstP = (Byte)((srcChar | kByteMark) & kByteMask); srcChar >>= 6;
        case 1: *--dstP = (Byte)(srcChar | kFirstByteMark[bytes]);
    }   
}

