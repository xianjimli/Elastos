
#include "cmdef.h"
#include "CRandomAccessFile.h"
#include "CFile.h"
#include "IoUtils.h"
#include <elastos/Math.h>
#include <elastos/Character.h>
#include <StringBuffer.h>

// #include <stdio.h>
// #include <errno.h>
// #include <unistd.h>

using namespace Elastos::Core;

extern ECode Platform2IoECode(ECode ec);

CRandomAccessFile::CRandomAccessFile()
    : mSyncMetadata(FALSE)
    , mIsReadOnly(FALSE)
{
   AutoPtr<IPlatform> platform;
   ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
   ASSERT_SUCCEEDED(platform->GetFileSystem((IFileSystem**)&mFileSystem));
}

CRandomAccessFile::~CRandomAccessFile()
{
    // try {
    Close();
    // } finally {
    //     super.finalize();
    // }
}

/**
 * Constructs a new {@code RandomAccessFile} based on {@code file} and opens
 * it according to the access string in {@code mode}.
 * <p><a id="accessmode"/>
 * {@code mode} may have one of following values:
 * <table border="0">
 * <tr>
 * <td>{@code "r"}</td>
 * <td>The file is opened in read-only mode. An {@code IOException} is
 * thrown if any of the {@code write} methods is called.</td>
 * </tr>
 * <tr>
 * <td>{@code "rw"}</td>
 * <td>The file is opened for reading and writing. If the file does not
 * exist, it will be created.</td>
 * </tr>
 * <tr>
 * <td>{@code "rws"}</td>
 * <td>The file is opened for reading and writing. Every change of the
 * file's content or metadata must be written synchronously to the target
 * device.</td>
 * </tr>
 * <tr>
 * <td>{@code "rwd"}</td>
 * <td>The file is opened for reading and writing. Every change of the
 * file's content must be written synchronously to the target device.</td>
 * </tr>
 * </table>
 *
 * @param file
 *            the file to open.
 * @param mode
 *            the file access <a href="#accessmode">mode</a>, either {@code
 *            "r"}, {@code "rw"}, {@code "rws"} or {@code "rwd"}.
 * @throws FileNotFoundException
 *             if the file cannot be opened or created according to {@code
 *             mode}.
 * @throws IllegalArgumentException
 *             if {@code mode} is not {@code "r"}, {@code "rw"}, {@code
 *             "rws"} or {@code "rwd"}.
 * @throws SecurityException
 *             if a {@code SecurityManager} is installed and it denies
 *             access request according to {@code mode}.
 * @see java.lang.SecurityManager#checkRead(FileDescriptor)
 * @see java.lang.SecurityManager#checkWrite(FileDescriptor)
 */
ECode CRandomAccessFile::constructor(
    /* in */ IFile* file,
    /* in */ const String& mode)
{
    mOptions = 0;
    FAIL_RETURN(CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd));

    if (mode.Equals("r")) {
        mIsReadOnly = TRUE;
        mFd->mReadOnly = TRUE;
        mOptions = IFileSystem_O_RDONLY;
    }
    else if (mode.Equals("rw") || mode.Equals("rws") || mode.Equals("rwd")) {
        mIsReadOnly = FALSE;
        mOptions = IFileSystem_O_RDWR;

        if (mode.Equals("rws")) {
            // Sync file and metadata with every write
            mSyncMetadata = TRUE;
        }
        else if (mode.Equals("rwd")) {
            // Sync file, but not necessarily metadata
            mOptions = IFileSystem_O_RDWRSYNC;
        }
    }
    else {
//        throw new IllegalArgumentException("Invalid mode: " + mode);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // SecurityManager security = System.getSecurityManager();
    // if (security != null) {
    //     security.checkRead(file.getPath());
    //     if (!isReadOnly) {
    //         security.checkWrite(file.getPath());
    //     }
    // }

    String path;
    file->GetAbsolutePath(&path);
    FAIL_RETURN(Platform2IoECode(mFileSystem->Open(path, mOptions, &mFd->mDescriptor)));
    // BEGIN android-removed
    // channel = FileChannelFactory.getFileChannel(this, fd.descriptor,
    //         options);
    // END android-removed

    // if we are in "rws" mode, attempt to sync file+metadata
    if (mSyncMetadata) {
        // try {
        mFd->Sync();
        // } catch (IOException e) {
        //     // Ignored
        // }
    }

    return NOERROR;
}

/**
 * Constructs a new {@code RandomAccessFile} based on the file named {@code
 * fileName} and opens it according to the access string in {@code mode}.
 * The file path may be specified absolutely or relative to the system
 * property {@code "user.dir"}.
 *
 * @param fileName
 *            the name of the file to open.
 * @param mode
 *            the file access <a href="#accessmode">mode</a>, either {@code
 *            "r"}, {@code "rw"}, {@code "rws"} or {@code "rwd"}.
 * @throws FileNotFoundException
 *             if the file cannot be opened or created according to {@code
 *             mode}.
 * @throws IllegalArgumentException
 *             if {@code mode} is not {@code "r"}, {@code "rw"}, {@code
 *             "rws"} or {@code "rwd"}.
 * @throws SecurityException
 *             if a {@code SecurityManager} is installed and it denies
 *             access request according to {@code mode}.
 * @see java.lang.SecurityManager#checkRead(FileDescriptor)
 * @see java.lang.SecurityManager#checkWrite(FileDescriptor)
 */
ECode CRandomAccessFile::constructor(
    /* [in] */ const String& fileName,
    /* [in] */ const String& mode)
{
    AutoPtr<IFile> file;
    FAIL_RETURN(CFile::New(fileName, (IFile**)&file));
    return constructor(file, mode);
}

/**
 * Closes this file.
 *
 * @throws IOException
 *             if an error occurs while closing this file.
 */
ECode CRandomAccessFile::Close()
{
    Mutex::Autolock lock(_m_syncLock);

    // if (channel != null && channel.isOpen()) {
    //     channel.close();
    //     channel = null;
    // }
    Boolean isValid;
    if (mFd != NULL && (mFd->Valid(&isValid), isValid)) {
        return IoUtils::Close(mFd);
    }
    return NOERROR;
}

/**
 * Gets this file's {@link FileChannel} object.
 * <p>
 * The file channel's {@link FileChannel#position() position} is the same
 * as this file's file pointer offset (see {@link #getFilePointer()}). Any
 * changes made to this file's file pointer offset are also visible in the
 * file channel's position and vice versa.
 *
 * @return this file's file channel instance.
 */
ECode CRandomAccessFile::GetChannel(
    /* out */ IFileChannel **channel)
{
    // Mutex::Autolock lock(_m_syncLock);

    // // BEGIN android-added
    // if(channel == null) {
    //     channel = FileChannelFactory.getFileChannel(this, fd.descriptor,
    //             options);
    // }
    // // END android-added
    // return channel;
    return E_NOT_IMPLEMENTED;
}

ECode CRandomAccessFile::GetFD(
    /* out */ IFileDescriptor ** fd)
{
    VALIDATE_NOT_NULL(fd);

    *fd = mFd;
    if (*fd != NULL) (*fd)->AddRef();
    return NOERROR;
}

ECode CRandomAccessFile::GetFilePointer(
    /* out */ Int64* offset)
{
    VALIDATE_NOT_NULL(offset);

    FAIL_RETURN(OpenCheck())
    return Platform2IoECode(mFileSystem->Seek(mFd->mDescriptor, 0ll, IFileSystem_SEEK_CUR, offset));
}

/**
 * Checks to see if the file is currently open. Returns silently if it is,
 * and throws an exception if it is not.
 *
 * @throws IOException
 *             the receiver is closed.
 */
ECode CRandomAccessFile::OpenCheck()
{
    Mutex::Autolock lock(_m_syncLock);

    if (mFd->mDescriptor < 0) {
        // throw new IOException();
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Returns the length of this file in bytes.
 *
 * @return the file's length in bytes.
 * @throws IOException
 *             if this file is closed or some other I/O error occurs.
 */
ECode CRandomAccessFile::GetLength(
    /* out */ Int64* len)
{
    VALIDATE_NOT_NULL(len);

    FAIL_RETURN(OpenCheck());
    return Platform2IoECode(mFileSystem->GetLength(mFd->mDescriptor, len));
}

/**
 * Reads a single byte from the current position in this file and returns it
 * as an integer in the range from 0 to 255. Returns -1 if the end of the
 * file has been reached. Blocks until one byte has been read, the end of
 * the file is detected or an exception is thrown.
 *
 * @return the byte read or -1 if the end of the file has been reached.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 */
ECode CRandomAccessFile::Read(
    /* out */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(OpenCheck());
    ArrayOf_<Byte, 1> bytes;
    Int64 byteCount;
    FAIL_RETURN(Platform2IoECode(mFileSystem->Read(mFd->mDescriptor, &bytes, 0, 1, &byteCount)));
    *value = byteCount == -1 ? -1 : bytes[0] & 0xff;
    return NOERROR;
}

/**
 * Reads bytes from the current position in this file and stores them in the
 * byte array {@code buffer}. The maximum number of bytes read corresponds
 * to the size of {@code buffer}. Blocks until at least one byte has been
 * read.
 *
 * @param buffer
 *            the byte array in which to store the bytes read.
 * @return the number of bytes actually read or -1 if the end of the file
 *         has been reached.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 */
ECode CRandomAccessFile::ReadBuffer(
    /* out */ ArrayOf<Byte>* buffer,
    /* out */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

/**
 * Reads at most {@code count} bytes from the current position in this file
 * and stores them in the byte array {@code buffer} starting at {@code
 * offset}. Blocks until {@code count} bytes have been read, the end of the
 * file is reached or an exception is thrown.
 *
 * @param buffer
 *            the array in which to store the bytes read from this file.
 * @param offset
 *            the initial position in {@code buffer} to store the bytes read
 *            from this file.
 * @param count
 *            the maximum number of bytes to store in {@code buffer}.
 * @return the number of bytes actually read or -1 if the end of the stream
 *         has been reached.
 * @throws IndexOutOfBoundsException
 *             if {@code offset < 0} or {@code count < 0}, or if {@code
 *             offset + count} is greater than the size of {@code buffer}.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 */
ECode CRandomAccessFile::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

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
    Int64 val;
    ECode ec = Platform2IoECode(mFileSystem->Read(
            mFd->mDescriptor, buffer, offset, length, &val));
    *number = (Int32)val;
    return ec;
}

/**
 * Reads a boolean from the current position in this file. Blocks until one
 * byte has been read, the end of the file is reached or an exception is
 * thrown.
 *
 * @return the next boolean value from this file.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeBoolean(boolean)
 */
ECode CRandomAccessFile::ReadBoolean(
    /* out */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 temp;
    FAIL_RETURN(Read(&temp));
    if (temp < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = temp != 0;
    return NOERROR;
}

/**
 * Reads an 8-bit byte from the current position in this file. Blocks until
 * one byte has been read, the end of the file is reached or an exception is
 * thrown.
 *
 * @return the next signed 8-bit byte value from this file.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeBoolean(boolean)
 */
ECode CRandomAccessFile::ReadByte(
    /* out */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 temp;
    FAIL_RETURN(Read(&temp));
    if (temp < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = (Byte)temp;
    return NOERROR;
}

/**
 * Reads a 16-bit character from the current position in this file. Blocks until
 * two bytes have been read, the end of the file is reached or an exception is
 * thrown.
 *
 * @return the next char value from this file.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeChar(int)
 */
ECode CRandomAccessFile::ReadChar(
    /* out */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    ArrayOf_<Byte, 4> buffer;
    Int32 count;
    FAIL_RETURN(ReadBufferEx(0, buffer.GetLength(), &buffer, &count));
    if (count < 0) {
        return E_EOF_EXCEPTION;
    }
    return Character::GetCharAt(*(ArrayOf<Char8>*)&buffer, 0, value, NULL);
}

/**
 * Reads a 64-bit double from the current position in this file. Blocks
 * until eight bytes have been read, the end of the file is reached or an
 * exception is thrown.
 *
 * @return the next double value from this file.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeDouble(double)
 */
ECode CRandomAccessFile::ReadDouble(
    /* out */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    Int64 i64Value;
    FAIL_RETURN(ReadInt64(&i64Value));
    *value = Math::Int64BitsToDouble(i64Value);
    return NOERROR;
}

/**
 * Reads a 32-bit float from the current position in this file. Blocks
 * until four bytes have been read, the end of the file is reached or an
 * exception is thrown.
 *
 * @return the next float value from this file.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeFloat(float)
 */
ECode CRandomAccessFile::ReadFloat(
    /* out */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 i32Value;
    FAIL_RETURN(ReadInt32(&i32Value));
    *value = Math::Int32BitsToFloat(i32Value);
    return NOERROR;
}

/**
 * Reads bytes from this file into {@code buffer}. Blocks until {@code
 * buffer.length} number of bytes have been read, the end of the file is
 * reached or an exception is thrown.
 *
 * @param buffer
 *            the buffer to read bytes into.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @throws NullPointerException
 *             if {@code buffer} is {@code null}.
 */
ECode CRandomAccessFile::ReadFully(
    /* out */ ArrayOf<Byte>* buffer)
{
    return ReadFullyEx(0, buffer->GetLength(), buffer);
}

/**
 * Read bytes from this file into {@code buffer} starting at offset {@code
 * offset}. This method blocks until {@code count} number of bytes have been
 * read.
 *
 * @param buffer
 *            the buffer to read bytes into.
 * @param offset
 *            the initial position in {@code buffer} to store the bytes read
 *            from this file.
 * @param count
 *            the maximum number of bytes to store in {@code buffer}.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IndexOutOfBoundsException
 *             if {@code offset < 0} or {@code count < 0}, or if {@code
 *             offset + count} is greater than the length of {@code buffer}.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @throws NullPointerException
 *             if {@code buffer} is {@code null}.
 */
ECode CRandomAccessFile::ReadFullyEx(
        /* in */ Int32 offset,
        /* in */ Int32 length,
        /* out */ ArrayOf<byte>* buffer)
{
    if (buffer == NULL) {
        // throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // used (offset | length) < 0 instead of separate (offset < 0) and
    // (length < 0) check to safe one operation
    if ((offset | length) < 0 || offset > buffer->GetLength() - length) {
        // throw new IndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    Int32 number;
    while (length > 0) {
        FAIL_RETURN(ReadBufferEx(offset, length, buffer, &number));
        if (number < 0) {
            // throw new EOFException();
            return E_EOF_EXCEPTION;
        }
        offset += number;
        length -= number;
    }
    return NOERROR;
}

/**
 * Reads a 32-bit integer from the current position in this file. Blocks
 * until four bytes have been read, the end of the file is reached or an
 * exception is thrown.
 *
 * @return the next int value from this file.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeInt(int)
 */
ECode CRandomAccessFile::ReadInt32(
        /* out */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    ArrayOf_<Byte, 4> buffer;
    Int32 count;
    FAIL_RETURN(ReadBufferEx(0, buffer.GetLength(), &buffer, &count));
    if (count != buffer.GetLength()) {
        // throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = ((buffer[0] & 0xff) << 24) + ((buffer[1] & 0xff) << 16)
            + ((buffer[2] & 0xff) << 8) + (buffer[3] & 0xff);
    return NOERROR;
}

/**
 * Reads a line of text form the current position in this file. A line is
 * represented by zero or more characters followed by {@code '\n'}, {@code
 * '\r'}, {@code "\r\n"} or the end of file marker. The string does not
 * include the line terminating sequence.
 * <p>
 * Blocks until a line terminating sequence has been read, the end of the
 * file is reached or an exception is thrown.
 *
 * @return the contents of the line or {@code null} if no characters have
 *         been read before the end of the file has been reached.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 */
ECode CRandomAccessFile::ReadLine(
        /* out */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer line;
    Boolean foundTerminator = FALSE;
    Int64 unreadPosition = 0;
    while (TRUE) {
        Int32 nextByte;
        FAIL_RETURN(Read(&nextByte));
        switch (nextByte) {
        case -1:
            if (line.GetLength() != 0) {
                *str = String((const char*)line);
            }
            else {
                *str = NULL;
            }
            return NOERROR;
        case (Byte)'\r':
            if (foundTerminator) {
                Seek(unreadPosition);
                *str = String((const char*)line);
                return NOERROR;
            }
            foundTerminator = TRUE;
            /* Have to be able to peek ahead one byte */
            GetFilePointer(&unreadPosition);
            break;
        case (Byte)'\n':
            *str = String((const char*)line);
            return NOERROR;
        default:
            if (foundTerminator) {
                Seek(unreadPosition);
                *str = String((const char*)line);
                return NOERROR;
            }
            line += (Char32)nextByte;
        }
    }
    return NOERROR;
}

/**
 * Reads a 64-bit long from the current position in this file. Blocks until
 * eight bytes have been read, the end of the file is reached or an
 * exception is thrown.
 *
 * @return the next long value from this file.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeLong(long)
 */
ECode CRandomAccessFile::ReadInt64(
        /* out */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    ArrayOf_<Byte, 8> buffer;
    Int32 count;
    FAIL_RETURN(ReadBufferEx(0, buffer.GetLength(), &buffer, &count));
    if (count != buffer.GetLength()) {
        // throw new EOFException();
        return E_EOF_EXCEPTION;
    }

    *value = ((Int64)(((buffer[0] & 0xff) << 24) + ((buffer[1] & 0xff) << 16)
            + ((buffer[2] & 0xff) << 8) + (buffer[3] & 0xff)) << 32)
            + ((Int64)(buffer[4] & 0xff) << 24)
            + ((buffer[5] & 0xff) << 16)
            + ((buffer[6] & 0xff) << 8)
            + (buffer[7] & 0xff);
    return NOERROR;
}

/**
 * Reads a 16-bit short from the current position in this file. Blocks until
 * two bytes have been read, the end of the file is reached or an exception
 * is thrown.
 *
 * @return the next short value from this file.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeShort(int)
 */
ECode CRandomAccessFile::ReadInt16(
        /* out */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    ArrayOf_<Byte, 2> buffer;
    Int32 count;
    FAIL_RETURN(ReadBufferEx(0, buffer.GetLength(), &buffer, &count));
    if (count != buffer.GetLength()) {
        // throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = (Int16)(((buffer[0] & 0xff) << 8) + (buffer[1] & 0xff));
    return NOERROR;
}

/**
 * Reads an unsigned 8-bit byte from the current position in this file and
 * returns it as an integer. Blocks until one byte has been read, the end of
 * the file is reached or an exception is thrown.
 *
 * @return the next unsigned byte value from this file as an int.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeByte(int)
 */
ECode CRandomAccessFile::ReadUnsignedByte(
        /* out */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(Read(value));
    return *value < 0 ? E_EOF_EXCEPTION : NOERROR;
}

/**
 * Reads an unsigned 16-bit short from the current position in this file and
 * returns it as an integer. Blocks until two bytes have been read, the end of
 * the file is reached or an exception is thrown.
 *
 * @return the next unsigned short value from this file as an int.
 * @throws EOFException
 *             if the end of this file is detected.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #writeShort(int)
 */
ECode CRandomAccessFile::ReadUnsignedInt16(
        /* out */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    ArrayOf_<Byte, 2> buffer;
    Int32 count;
    FAIL_RETURN(ReadBufferEx(0, buffer.GetLength(), &buffer, &count));
    if (count != buffer.GetLength()) {
        // throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = (((buffer[0] & 0xff) << 8) + (buffer[1] & 0xff));
    return NOERROR;
}

/**
 * Moves this file's file pointer to a new position, from where following
 * {@code read}, {@code write} or {@code skip} operations are done. The
 * position may be greater than the current length of the file, but the
 * file's length will only change if the moving of the pointer is followed
 * by a {@code write} operation.
 *
 * @param offset
 *            the new file pointer position.
 * @throws IOException
 *             if this file is closed, {@code pos < 0} or another I/O error
 *             occurs.
 */
ECode CRandomAccessFile::Seek(
    /* in */ Int64 offset)
{
    if (offset < 0) {
        // seek position is negative
        // throw new IOException("offset < 0");
        return E_IO_EXCEPTION;
    }
    FAIL_RETURN(OpenCheck());
    Int64 result;
    return Platform2IoECode(mFileSystem->Seek(mFd->mDescriptor, offset, IFileSystem_SEEK_SET, &result));
}

/**
 * Sets the length of this file to {@code newLength}. If the current file is
 * smaller, it is expanded but the contents from the previous end of the
 * file to the new end are undefined. The file is truncated if its current
 * size is bigger than {@code newLength}. If the current file pointer
 * position is in the truncated part, it is set to the end of the file.
 *
 * @param newLength
 *            the new file length in bytes.
 * @throws IllegalArgumentException
 *             if {@code newLength < 0}.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 */
ECode CRandomAccessFile::SetLength(
   /* in */ Int64 newLength)
{
    FAIL_RETURN(OpenCheck());
    if (newLength < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(Platform2IoECode(mFileSystem->Truncate(mFd->mDescriptor, newLength)));

    // if we are in "rws" mode, attempt to sync file+metadata
    if (mSyncMetadata) {
        mFd->Sync();
    }
    return NOERROR;
}

/**
 * Skips over {@code count} bytes in this file. Less than {@code count}
 * bytes are skipped if the end of the file is reached or an exception is
 * thrown during the operation. Nothing is done if {@code count} is
 * negative.
 *
 * @param count
 *            the number of bytes to skip.
 * @return the number of bytes actually skipped.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 */
ECode CRandomAccessFile::SkipBytes(
        /* in */ Int32 count,
        /* out */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (count > 0) {
        Int64 currentPos, eof;
        GetFilePointer(&currentPos);
        GetLength(&eof);
        Int32 newCount = (Int32)((currentPos + count > eof) ? eof - currentPos
                : count);
        FAIL_RETURN(Seek(currentPos + newCount));
        *number = newCount;
        return NOERROR;
    }
    *number = 0;
    return NOERROR;
}

/**
 * Writes the entire contents of the byte array {@code buffer} to this file,
 * starting at the current file pointer.
 *
 * @param buffer
 *            the buffer to write.
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #read(byte[])
 * @see #read(byte[],int,int)
 * @see #readFully(byte[])
 * @see #readFully(byte[],int,int)
 */
ECode CRandomAccessFile::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}

/**
 * Writes {@code count} bytes from the byte array {@code buffer} to this
 * file, starting at the current file pointer and using {@code offset} as
 * the first position within {@code buffer} to get bytes.
 *
 * @param buffer
 *            the buffer to write to this file.
 * @param offset
 *            the index of the first byte in {@code buffer} to write.
 * @param count
 *            the number of bytes from {@code buffer} to write.
 * @throws IndexOutOfBoundsException
 *             if {@code count < 0}, {@code offset < 0} or {@code count +
 *             offset} is greater than the size of {@code buffer}.
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #read(byte[], int, int)
 * @see #readFully(byte[], int, int)
 */
ECode CRandomAccessFile::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // made implicit null check explicit,
    // removed redundant check, used (offset | count) < 0
    // instead of (offset < 0) || (count < 0) to save one operation
    // if (buffer == null) {
    //     throw new NullPointerException("buffer == null");
    // }
    if ((offset | count) < 0 || count > buffer.GetLength() - offset) {
        // throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    if (count == 0) {
        return NOERROR;
    }
    FAIL_RETURN(OpenCheck());
    Int64 number;
    FAIL_RETURN(Platform2IoECode(mFileSystem->Write(
            mFd->mDescriptor, buffer, offset, count, &number)));

    // if we are in "rws" mode, attempt to sync file+metadata
    if (mSyncMetadata) {
        mFd->Sync();
    }
    return NOERROR;
}

/**
 * Writes a byte to this file, starting at the current file pointer. Only
 * the least significant byte of the integer {@code oneByte} is written.
 *
 * @param oneByte
 *            the byte to write to this file.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #read()
 */
ECode CRandomAccessFile::Write(
        /* in */ Int32 oneByte)
{
    FAIL_RETURN(OpenCheck());

    ArrayOf_<Byte, 1> bytes;
    (bytes)[0] = (Byte)(oneByte & 0xff);
    Int64 number;
    FAIL_RETURN(Platform2IoECode(mFileSystem->Write(
            mFd->mDescriptor, bytes, 0, 1, &number)));

    // if we are in "rws" mode, attempt to sync file+metadata
    if (mSyncMetadata) {
        mFd->Sync();
    }
    return NOERROR;
}

/**
 * Writes a boolean to this file, starting at the current file pointer.
 *
 * @param val
 *            the boolean to write to this file.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #readBoolean()
 */
ECode CRandomAccessFile::WriteBoolean(
        /* in */ Boolean value)
{
    return Write(value? 1 : 0);
}

/**
 * Writes an 8-bit byte to this file, starting at the current file pointer.
 * Only the least significant byte of the integer {@code val} is written.
 *
 * @param val
 *            the byte to write to this file.
 * @throws IOException
 *             if this file is closed or another I/O error occurs.
 * @see #readByte()
 * @see #readUnsignedByte()
 */
ECode CRandomAccessFile::WriteByte(
        /* in */ Int32 value)
{
    return Write(value & 0xff);
}

/**
 * Writes the low order 8-bit bytes from a string to this file, starting at
 * the current file pointer.
 *
 * @param str
 *            the string containing the bytes to write to this file
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #read(byte[])
 * @see #read(byte[],int,int)
 * @see #readFully(byte[])
 * @see #readFully(byte[],int,int)
 */
ECode CRandomAccessFile::WriteBytes(
        /* in */ const ArrayOf<Byte>& buffer)
{
    return WriteBuffer(buffer);
}


ECode CRandomAccessFile::WriteBytesEx(
        /* in */ Int32 count,
        /* in */ Int32 length,
        /* in */ const ArrayOf<byte>& buffer)
{
    return WriteBufferEx(length, count, buffer);
}

/**
 * Writes a 16-bit character to this file, starting at the current file
 * pointer. Only the two least significant bytes of the integer {@code val}
 * are written, with the high byte first.
 *
 * @param val
 *            the char to write to this file.
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #readChar()
 */
ECode CRandomAccessFile::WriteChar(
        /* in */ Int32 value)
{
    ArrayOf_<Byte, 4> buffer;
    Int32 len;
    Character::ToChars((Char32)value, *(ArrayOf<Char8>*)&buffer, 0, &len);
    return WriteBufferEx(0, len, buffer);
}

/**
 * Writes a 64-bit double to this file, starting at the current file
 * pointer. The eight bytes returned by
 * {@link Double#doubleToLongBits(double)} are written to this file.
 *
 * @param val
 *            the double to write to this file.
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #readDouble()
 */
ECode CRandomAccessFile::WriteDouble(
        /* in */ Double value)
{
    return WriteInt64(Math::DoubleToInt64Bits(value));
}

/**
 * Writes a 32-bit float to this file, starting at the current file pointer.
 * The four bytes returned by {@link Float#floatToIntBits(float)} are
 * written to this file.
 *
 * @param val
 *            the float to write to this file.
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #readFloat()
 */
ECode CRandomAccessFile::WriteFloat(
        /* in */ Float value)
{
    return WriteInt32(Math::FloatToInt32Bits(value));
}

/**
 * Writes a 32-bit integer to this file, starting at the current file
 * pointer. The four bytes of the integer are written with the highest byte
 * first.
 *
 * @param val
 *            the int to write to this file.
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #readInt()
 */
ECode CRandomAccessFile::WriteInt32(
        /* in */ Int32 value)
{
    ArrayOf_<Byte, 4> buffer;
    buffer[0] = (Byte)(value >> 24);
    buffer[1] = (Byte)(value >> 16);
    buffer[2] = (Byte)(value >> 8);
    buffer[3] = (Byte)value;
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}

/**
 * Writes a 64-bit long to this file, starting at the current file
 * pointer. The eight bytes of the long are written with the highest byte
 * first.
 *
 * @param val
 *            the long to write to this file.
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #readLong()
 */
ECode CRandomAccessFile::WriteInt64(
        /* in */ Int64 value)
{
    ArrayOf_<Byte, 8> buffer;
    Int32 t = (Int32)(value >> 32);
    buffer[0] = (Byte)(t >> 24);
    buffer[1] = (Byte)(t >> 16);
    buffer[2] = (Byte)(t >> 8);
    buffer[3] = (Byte)t;
    buffer[4] = (Byte)(value >> 24);
    buffer[5] = (Byte)(value >> 16);
    buffer[6] = (Byte)(value >> 8);
    buffer[7] = (Byte)value;
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}

/**
 * Writes a 16-bit short to this file, starting at the current file
 * pointer. Only the two least significant bytes of the integer {@code val}
 * are written, with the high byte first.
 *
 * @param val
 *            the short to write to this file.
 * @throws IOException
 *             if an I/O error occurs while writing to this file.
 * @see #readShort()
 * @see DataInput#readUnsignedShort()
 */
ECode CRandomAccessFile::WriteInt16(
        /* in */ Int32 value)
{
    ArrayOf_<Byte, 2> buffer;
    buffer[0] = (Byte)(value >> 8);
    buffer[1] = (Byte)value;
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}
