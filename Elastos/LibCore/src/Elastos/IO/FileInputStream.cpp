#include "cmdef.h"
#include "FileInputStream.h"
#include "CFile.h"
#include <errno.h>
#include <unistd.h>


extern ECode Platform2IoECode(ECode ec);

FileInputStream::FileInputStream()
{
    AutoPtr<IPlatform> platform;
    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
    ASSERT_SUCCEEDED(platform->GetFileSystem((IFileSystem**)&mFileSystem));
}

FileInputStream::~FileInputStream()
{
}

ECode FileInputStream::Init(
    /* [in] */ IFile* file)
{
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        // For compatibility, nulls are passed to the manager.
//        String filePath = (null == file ? null : file.getPath());
//        security.checkRead(filePath);
//    }
    if (file == NULL) {
//        throw new NullPointerException("file == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd);
    mFd->mReadOnly = TRUE;
    String path;
    file->GetAbsolutePath(&path);
    mFileSystem->Open(path, IFileSystem_O_RDONLY, &mFd->mDescriptor);
    mInnerFD = TRUE;
    // BEGIN android-removed
    // channel = FileChannelFactory.getFileChannel(this, fd.descriptor,
    //         IFileSystem.O_RDONLY);
    // END android-removed
    return NOERROR;
}

ECode FileInputStream::Init(
    /* [in] */ IFileDescriptor* fd)
{
    if (fd == NULL) {
//        throw new NullPointerException("fd == null");
        return E_NULL_POINTER_EXCEPTION;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(fd);
//    }
    mFd = (CFileDescriptor*)fd;
    mInnerFD = FALSE;
    // BEGIN android-removed
    // channel = FileChannelFactory.getFileChannel(this, fd.descriptor,
    //         IFileSystem.O_RDONLY);
    // END android-removed
    return NOERROR;
}

ECode FileInputStream::Init(
    /* [in] */ const String& fileName)
{
    AutoPtr<IFile> file;
    if (!fileName.IsNull()) {
        CFile::New(fileName, (IFile**)&file);
    }
    return Init(file);
}

ECode FileInputStream::Available(
    /* [out] */ Int32* avail)
{
    FAIL_RETURN(OpenCheck());
    return Platform2IoECode(mFileSystem->IoctlAvailable(
            mFd->mDescriptor, avail));
}

ECode FileInputStream::Close()
{
    // BEGIN android-changed
    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return CloseLocked();
}

ECode FileInputStream::CloseLocked()
{
    // BEGIN android-changed

//    if (mChannel != NULL && mChannel->IsOpen()) {
//        mChannel->Close();
//        mChannel = NULL;
//    }
    Boolean isValid;
    if (mFd != NULL && (mFd->Valid(&isValid), isValid) && mInnerFD) {
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

ECode FileInputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    // BEGIN android-changed
    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return GetChannelLocked(channel);
}

ECode FileInputStream::GetChannelLocked(
    /* [out] */ IFileChannel** channel)
{
    // BEGIN android-changed

//    if (channel == null) {
//        channel = FileChannelFactory.getFileChannel(this, fd.descriptor,
//                IFileSystem.O_RDONLY);
//    }
//    return mChannel;
    // END android-changed
    return E_NOT_IMPLEMENTED;
}

ECode FileInputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    assert(fd != NULL);
    *fd = mFd;
    if (*fd != NULL) (*fd)->AddRef();
    return NOERROR;
}

ECode FileInputStream::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    ArrayOf_<Byte, 1> readed;
    Int32 result;
    FAIL_RETURN(ReadBufferEx(0, 1, &readed, &result));
    *value = result == -1 ? -1 : readed[0] & 0xff;
    return NOERROR;;
}

ECode FileInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    FAIL_RETURN(ReadBufferEx(0, buffer->GetLength(), buffer, number));
    return NOERROR;
}

ECode FileInputStream::ReadBufferEx(
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
    Mutex::Autolock lock(repositioningLock);

    return ReadBufferExLocked(offset, length, buffer, number);
}

ECode FileInputStream::ReadBufferExLocked(
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
    FAIL_RETURN(Platform2IoECode(mFileSystem->Read(
            mFd->mDescriptor, buffer, offset, length, &val)));
    *number = (Int32)val;
    return NOERROR;
    // END android-changed
}

ECode FileInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    FAIL_RETURN(OpenCheck());
    if (count == 0) {
        *number = 0;
        return NOERROR;
    }
    if (count < 0) {
//      throw new IOException("count < 0");
        return E_IO_EXCEPTION;
    }

    // The RI doesn't treat stdin as special. It throws IOException for
    // all non-seekable streams, so we do too. If you did want to support
    // non-seekable streams, the best way to do it would be to recognize
    // when lseek(2) fails with ESPIPE and call super.skip(count).
    Mutex::Autolock lock(repositioningLock);

    return SkipLocked(count, number);
}

ECode FileInputStream::SkipLocked(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    Int64 cur;
    FAIL_RETURN(OpenCheck());
    if (count == 0) {
        *number = 0;
        return NOERROR;
    }
    if (count < 0) {
//      throw new IOException("count < 0");
        return E_IO_EXCEPTION;
    }

    // Our seek returns the new offset, but we know it will throw an
    // exception if it couldn't perform exactly the seek we asked for.
    mFileSystem->Seek(mFd->mDescriptor, 0, IFileSystem_SEEK_CUR, &cur);
    mFileSystem->Seek(mFd->mDescriptor, count, IFileSystem_SEEK_CUR, number);
    return *number - cur;
}

ECode FileInputStream::OpenCheck()
{
    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return OpenCheckLocked();
}

ECode FileInputStream::OpenCheckLocked()
{
    if(mFd->mDescriptor < 0){
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}
