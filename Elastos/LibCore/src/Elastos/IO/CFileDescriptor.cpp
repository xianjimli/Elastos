#include "cmdef.h"
#include "CFileDescriptor.h"
#include <errno.h>
#include <unistd.h>

AutoPtr<IFileDescriptor> CreateFD(
    /* [in] */ Int32 descriptor)
{
    AutoPtr<CFileDescriptor> fd;
    CFileDescriptor::NewByFriend((CFileDescriptor**)&fd);
    fd->mDescriptor = descriptor;

    return (IFileDescriptor*)fd.Get();
}

const AutoPtr<IFileDescriptor> CFileDescriptor::in = CreateFD(0);
const AutoPtr<IFileDescriptor> CFileDescriptor::out = CreateFD(1);
const AutoPtr<IFileDescriptor> CFileDescriptor::err = CreateFD(2);

CFileDescriptor::CFileDescriptor()
    : mDescriptor(-1)
    , mReadOnly(FALSE)
{
}

ECode CFileDescriptor::Sync()
{
    // if the descriptor is a read-only one, do nothing
    if (!mReadOnly) {
        SyncImpl();
    }
    return NOERROR;
}

ECode CFileDescriptor::SyncImpl()
{
    Int32 fd = mDescriptor;
    Int32 rc = fsync(fd);
    if (rc == -1) {
        /*
         * If fd is a socket, then fsync(fd) is defined to fail with
         * errno EINVAL. This isn't actually cause for concern.
         * TODO: Look into not bothering to call fsync() at all if
         * we know we are dealing with a socket.
         */
        if (errno != EINVAL) {
//            char buf[BUFSIZ];
//            jniThrowException(env, "java/io/SyncFailedException",
//                    jniStrError(errno, buf, sizeof(buf)));
            return E_SYNC_FAILED_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CFileDescriptor::Valid(
    /* [out] */ Boolean* isValid)
{
    VALIDATE_NOT_NULL(isValid);

    *isValid = mDescriptor != -1;
    return NOERROR;
}

ECode CFileDescriptor::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFileDescriptor::GetDescriptor(
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);
    *fd = mDescriptor;
    return NOERROR;
}

ECode CFileDescriptor::SetDescriptor(
        /* [in] */ Int32 fd)
{
    mDescriptor = fd;
    return NOERROR;
}
