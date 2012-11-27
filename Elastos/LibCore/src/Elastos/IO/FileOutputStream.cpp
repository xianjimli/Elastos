#include "cmdef.h"
#include "FileOutputStream.h"
#include "CFile.h"
#include <errno.h>
#include <unistd.h>

ECode Platform2IO(ECode ec)
{
    switch(ec) {
    case NOERROR:
	return NOERROR;
    case E_PLATFORM_IO_EXCEPTION:
        return E_IO_EXCEPTION;
    case E_PLATFORM_FILE_NOT_FOUND_EXCEPTION:
        return E_FILE_NOT_FOUND_EXCEPTION;
    default:
        return E_FAIL;
    }
}

FileOutputStream::FileOutputStream()
{
    AutoPtr<IPlatform> platform;
    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
    ASSERT_SUCCEEDED(platform->GetFileSystem((IFileSystem**)&mFileSystem));
}

FileOutputStream::~FileOutputStream()
{
}

ECode FileOutputStream::Init(
    /* [in] */ IFile* file)
{
    return Init(file, FALSE);
}

ECode FileOutputStream::Init(
    /* [in] */ IFile* file,
    /* [in] */ Boolean append)
{
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(file.getPath());
//    }
    CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd);
    String path;
    file->GetAbsolutePath(&path);
    mFileSystem->Open(path,
            append ? IFileSystem_O_APPEND : IFileSystem_O_WRONLY,
            &mFd->mDescriptor);
    mInnerFD = TRUE;
//    channel = FileChannelFactory.getFileChannel(this, fd.descriptor,
//            append ? IFileSystem.O_APPEND : IFileSystem.O_WRONLY);
    return NOERROR;
}

ECode FileOutputStream::Init(
    /* [in] */ IFileDescriptor* fd)
{
    if (fd == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(fd);
//    }
    mFd = (CFileDescriptor*)fd;
    mInnerFD = FALSE;
//    channel = FileChannelFactory.getFileChannel(this, fd.descriptor,
//            IFileSystem.O_WRONLY);
    return NOERROR;
}

ECode FileOutputStream::Init(
    /* [in] */ const String& fileName)
{
    return Init(fileName, FALSE);
}

ECode FileOutputStream::Init(
    /* [in] */ const String& fileName,
    /* [in] */ Boolean append)
{
    AutoPtr<IFile> file;
    if (!fileName.IsNull()) {
        CFile::New(fileName, (IFile**)&file);
    }
    return Init(file, append);
}

ECode FileOutputStream::Close()
{
    if (mFd == NULL) {
        // if fd is null, then the underlying file is not opened, so nothing
        // to close
        return NOERROR;
    }

    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);
    return CloseLocked();
}

ECode FileOutputStream::CloseLocked()
{
    if (mFd == NULL) {
        // if fd is null, then the underlying file is not opened, so nothing
        // to close
        return NOERROR;
    }

//    if (channel != null) {
//            synchronized (channel) {
//                if (channel.isOpen() && fd.descriptor >= 0) {
//                    channel.close();
//                }
//            }
//        }
//

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
    return NOERROR;
}

ECode FileOutputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    assert(channel != NULL);
    *channel = mChannel;
    if (*channel != NULL) (*channel)->AddRef();
    return NOERROR;
}

ECode FileOutputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    assert(fd != NULL);
    *fd = mFd;
    if (*fd != NULL) (*fd)->AddRef();
    return NOERROR;
}

ECode FileOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    FAIL_RETURN(OpenCheck());

    ArrayOf_<Byte, 1> byteArray;
    (byteArray)[0] = (Byte)oneByte;
    Int64 number;
    return Platform2IO(mFileSystem->Write(
            mFd->mDescriptor, byteArray, 0, 1, &number));
}

ECode FileOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}

ECode FileOutputStream::WriteBufferEx(
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

ECode FileOutputStream::OpenCheck()
{
    Mutex* selfLock = GetSelfLock();
    Mutex::Autolock lock(*selfLock);

    return OpenCheckLocked();
}

ECode FileOutputStream::OpenCheckLocked()
{
    if(mFd->mDescriptor < 0){
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}
