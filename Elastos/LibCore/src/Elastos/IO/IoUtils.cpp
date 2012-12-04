
#include "cmdef.h"
#include "IoUtils.h"
#include "CFileDescriptor.h"
#include "Com.Kortide.Platform.h"
#include <fcntl.h>
#include <unistd.h>


ECode Platform2IoECode(ECode ec)
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

IoUtils::IoUtils()
{}

/**
 * Calls close(2) on 'fd'. Also resets the internal int to -1.
 */
ECode IoUtils::Close(
    /* [in] */ IFileDescriptor* fd)
{
    CFileDescriptor* fdObj =  (CFileDescriptor*)fd;
    Int32 nativeFd = fdObj->mDescriptor;
    Int32 rc = TEMP_FAILURE_RETRY(close(nativeFd));
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return E_IO_EXCEPTION;
    }
    fdObj->mDescriptor = -1;
    return NOERROR;
}

/**
 * Closes 'closeable', ignoring any exceptions. Does nothing if 'closeable' is null.
 */
ECode IoUtils::CloseQuietly(
    /* [in] */ ICloseable* closeable)
{
    if (closeable != NULL) {
        // try {
            closeable->Close();
        // } catch (IOException ignored) {
        // }
    }
    return NOERROR;
}

/**
 * Returns the int file descriptor from within the given FileDescriptor 'fd'.
 */
Int32 IoUtils::GetFd(
    /* [in] */ IFileDescriptor* fd)
{
    return ((CFileDescriptor*)fd)->mDescriptor;
}

/**
 * Returns a new FileDescriptor whose internal integer is set to 'fd'.
 */
AutoPtr<IFileDescriptor> IoUtils::NewFileDescriptor(
    /* [in] */ Int32 fd)
{
    AutoPtr<IFileDescriptor> result;
    ASSERT_SUCCEEDED(CFileDescriptor::New((IFileDescriptor**)&result));
    SetFd(result, fd);
    return result;
}

 /**
 * Creates a pipe by calling pipe(2), returning the two file descriptors in
 * elements 0 and 1 of the array 'fds'. fds[0] is the read end of the pipe.
 * fds[1] is the write end of the pipe. Throws an appropriate IOException on failure.
 */
ECode IoUtils::Pipe(
    /* [in] */ const ArrayOf<Int32>& fds)
{
    Int32 rc = pipe(const_cast<Int32*>(&fds[0]));
    if (rc == -1) {
        // jniThrowIOException(env, errno);
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Sets the int file descriptor within the given FileDescriptor 'fd' to 'newValue'.
 */
ECode IoUtils::SetFd(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 newValue)
{
    ((CFileDescriptor*)fd)->mDescriptor = newValue;
    return NOERROR;
}

/**
 * Sets 'fd' to be blocking or non-blocking, according to the state of 'blocking'.
 */
ECode IoUtils::SetBlocking(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean blocking)
{
    Int32 nativeFd = ((CFileDescriptor*)fd)->mDescriptor;
    if (nativeFd == -1) {
        return NOERROR;
    }
    if (!SetBlocking(nativeFd, blocking)) {
        // jniThrowIOException(env, errno);
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

Boolean IoUtils::SetBlocking(
    /* [in] */ Int32 fd,
    /* [in] */ Boolean blocking)
{
    Int32 flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        return FALSE;
    }

    if (!blocking) {
        flags |= O_NONBLOCK;
    }
    else {
        flags &= ~O_NONBLOCK;
    }

    Int32 rc = fcntl(fd, F_SETFL, flags);
    return (rc != -1);
}
