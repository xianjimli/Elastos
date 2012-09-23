
#include "cmdef.h"
#include "OSFileSystem.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

#define LOG_TAG "OSFileSystem"

/* Values for HyFileOpen */
#define HyOpenRead    1
#define HyOpenWrite   2
#define HyOpenCreate  4
#define HyOpenTruncate  8
#define HyOpenAppend  16
/* Use this flag with HyOpenCreate, if this flag is specified then
 * trying to create an existing file will fail
 */
#define HyOpenCreateNew 64
#define HyOpenSync      128
#define SHARED_LOCK_TYPE 1L

#if HAVE_SYS_SENDFILE_H
#include <sys/sendfile.h>
#else
/*
 * Define a small adapter function: sendfile() isn't part of a standard,
 * and its definition differs between Linux, BSD, and OS X. This version
 * works for OS X but will probably not work on other BSDish systems.
 * Note: We rely on function overloading here to define a same-named
 * function with different arguments.
 */
#include <sys/socket.h>
#include <sys/types.h>
static inline ssize_t sendfile(int out_fd, int in_fd, off_t* offset, size_t count) {
    off_t len = count;
    int result = sendfile(in_fd, out_fd, *offset, &len, NULL, 0);
    if (result < 0) {
        return -1;
    }
    return len;
}
#endif

static Int32 EsTranslateOpenFlags(Int32 flags)
{
    Int32 realFlags = 0;

    if (flags & HyOpenAppend) {
        realFlags |= O_APPEND;
    }
    if (flags & HyOpenTruncate) {
        realFlags |= O_TRUNC;
    }
    if (flags & HyOpenCreate) {
        realFlags |= O_CREAT;
    }
    if (flags & HyOpenCreateNew) {
        realFlags |= O_EXCL | O_CREAT;
    }
#ifdef O_SYNC
    if (flags & HyOpenSync) {
        realFlags |= O_SYNC;
    }
#endif
    if (flags & HyOpenRead) {
        if (flags & HyOpenWrite) {
            return (O_RDWR | realFlags);
        }
        return (O_RDONLY | realFlags);
    }
    if (flags & HyOpenWrite) {
        return (O_WRONLY | realFlags);
    }
    return -1;
}

// Checks whether we can safely treat the given jlong as an off_t without
// accidental loss of precision.
// TODO: this is bogus; we should use _FILE_OFFSET_BITS=64.
static Boolean offsetTooLarge(Int64 offset)
{
    if (sizeof(off_t) >= sizeof(Int64)) {
        // We're only concerned about the possibility that off_t is
        // smaller than jlong. off_t is signed, so we don't need to
        // worry about signed/unsigned.
        return FALSE;
    }

    // TODO: use std::numeric_limits<off_t>::max() and min() when we have them.
    assert(sizeof(off_t) == sizeof(Int32));
    static const off_t off_t_max = INT_MAX;
    static const off_t off_t_min = INT_MIN;

    if (offset > off_t_max || offset < off_t_min) {
        // "Value too large for defined data type".
//        jniThrowIOException(env, EOVERFLOW);
        return TRUE;
    }
    return FALSE;
}

static Int64 translateLockLength(Int64 length)
{
    // FileChannel.tryLock uses Long.MAX_VALUE to mean "lock the whole
    // file", where POSIX would use 0. We can support that special case,
    // even for files whose actual length we can't represent. For other
    // out of range lengths, though, we want our range checking to fire.
    return (length == 0x7fffffffffffffffLL) ? 0 : length;
}

static struct flock flockFromStartAndLength(Int64 start, Int64 length)
{
    struct flock lock;
    memset(&lock, 0, sizeof(lock));

    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = length;

    return lock;
}

// Translate three Java int[]s to a native iovec[] for readv and writev.
static iovec* initIoVec(
    const ArrayOf<Int32>& addresses,
    const ArrayOf<Int32>& offsets,
    const ArrayOf<Int32>& lengths,
    Int32 size)
{
    iovec* vectors = new iovec[size];
    if (vectors == NULL) {
//        jniThrowException(env, "java/lang/OutOfMemoryError", "native heap");
        return NULL;
    }
    if (addresses.GetPayload() == NULL) {
        return NULL;
    }
    if (offsets.GetPayload() == NULL) {
        return NULL;
    }
    if (lengths.GetPayload() == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; ++i) {
        vectors[i].iov_base = reinterpret_cast<void*>(addresses[i] + offsets[i]);
        vectors[i].iov_len = lengths[i];
    }
    return vectors;
}

const AutoPtr<OSFileSystem> OSFileSystem::sSingleton = new OSFileSystem();

AutoPtr<IFileSystem> OSFileSystem::GetOSFileSystem()
{
    AutoPtr<IFileSystem> fs = (IFileSystem*)sSingleton.Get();
    return fs;
}

PInterface OSFileSystem::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IFileSystem*)this;
    }
    else if (riid == EIID_IFileSystem) {
        return (IFileSystem*)this;
    }
    return NULL;
}

UInt32 OSFileSystem::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 OSFileSystem::Release()
{
    return ElRefBase::Release();
}

ECode OSFileSystem::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IFileSystem*)this) {
        *pIID = EIID_IFileSystem;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode OSFileSystem::ValidateLockArgs(
    /* [in] */ Int32 type,
    /* [in] */ Int64 start,
    /* [in] */ Int64 length)
{
    if ((type != IFileSystem_SHARED_LOCK_TYPE)
            && (type != IFileSystem_EXCLUSIVE_LOCK_TYPE)) {
//        throw new IllegalArgumentException("Illegal lock type requested.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Start position
    if (start < 0) {
//        throw new IllegalArgumentException("start < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Length of lock stretch
    if (length < 0) {
//        throw new IllegalArgumentException("length < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

Int32 OSFileSystem::LockImpl(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 start,
    /* [in] */ Int64 length,
    /* [in] */ Int32 type,
    /* [in] */ Boolean wait)
{
    length = translateLockLength(length);
    if (offsetTooLarge(start) || offsetTooLarge(length)) {
        return -1;
    }

    struct flock lock(flockFromStartAndLength(start, length));

    if ((type & SHARED_LOCK_TYPE) == SHARED_LOCK_TYPE) {
        lock.l_type = F_RDLCK;
    }
    else {
        lock.l_type = F_WRLCK;
    }

    Int32 waitMode = (wait) ? F_SETLKW : F_SETLK;
    return TEMP_FAILURE_RETRY(fcntl(fd, waitMode, &lock));
}

ECode OSFileSystem::GetAllocGranularity(
    /* [out] */ Int32* granularity)
{
    VALIDATE_NOT_NULL(granularity);
    static Int32 allocGranularity = getpagesize();
    *granularity = allocGranularity;
    return NOERROR;
}

ECode OSFileSystem::GetLength(
    /* [in] */ Int32 fd,
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size);
    struct stat sb;
    Int32 rc = TEMP_FAILURE_RETRY(fstat(fd, &sb));
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return E_PLATFORM_IO_EXCEPTION;
    }
    *size = sb.st_size;
    return NOERROR;
}

ECode OSFileSystem::Lock(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 start,
    /* [in] */ Int64 length,
    /* [in] */ Int32 type,
    /* [in] */ Boolean waitFlag,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // Validate arguments
    FAIL_RETURN(ValidateLockArgs(type, start, length));
    Int32 result = LockImpl(fd, start, length, type, waitFlag);
    *succeeded = result != -1;
    return NOERROR;
}

ECode OSFileSystem::UnlockImpl(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 start,
    /* [in] */ Int64 length)
{
    length = translateLockLength(length);
    if (offsetTooLarge(start) || offsetTooLarge(length)) {
        return NOERROR;
    }

    struct flock lock(flockFromStartAndLength(start, length));
    lock.l_type = F_UNLCK;

    Int32 rc = TEMP_FAILURE_RETRY(fcntl(fd, F_SETLKW, &lock));
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return E_PLATFORM_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode OSFileSystem::Unlock(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 start,
    /* [in] */ Int64 length)
{
    // Validate arguments
    FAIL_RETURN(ValidateLockArgs(IFileSystem_SHARED_LOCK_TYPE, start, length));
    return UnlockImpl(fd, start, length);
}

ECode OSFileSystem::Fsync(
    /* [in] */ Int32 fd,
    /* [in] */ Boolean metadata)
{
    if (!metadata) {
//        LOGW("fdatasync(2) unimplemented on Android - doing fsync(2)"); // http://b/2667481
    }
    Int32 rc = fsync(fd);
    // int rc = metadata ? fsync(fd) : fdatasync(fd);
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return E_PLATFORM_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode OSFileSystem::Seek(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 whence,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    /* Convert whence argument */
    Int32 nativeWhence = 0;
    switch (whence) {
    case 1:
        nativeWhence = SEEK_SET;
        break;
    case 2:
        nativeWhence = SEEK_CUR;
        break;
    case 4:
        nativeWhence = SEEK_END;
        break;
    default:
        *number = -1;
        return NOERROR;
    }

    // If the offset is relative, lseek(2) will tell us whether it's too large.
    // We're just worried about too large an absolute offset, which would cause
    // us to lie to lseek(2).
    if (offsetTooLarge(offset)) {
        *number = -1;
        return NOERROR;
    }

    Int64 result = lseek(fd, offset, nativeWhence);
    if (result == -1) {
//        jniThrowIOException(env, errno);
        *number = -1;
        return E_PLATFORM_IO_EXCEPTION;
    }
    *number = result;
    return NOERROR;
}

ECode OSFileSystem::ReadDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    if (length == 0) {
        *number = 0;
        return NOERROR;
    }
    Byte* dst = reinterpret_cast<Byte*>(address + offset);
    Int64 rc = TEMP_FAILURE_RETRY(read(fd, dst, length));
    if (rc == 0) {
        *number = -1;
        return NOERROR;
    }
    if (rc == -1) {
        // We return 0 rather than throw if we try to read from an empty non-blocking pipe.
        if (errno == EAGAIN) {
            *number = 0;
            return NOERROR;
        }
//        jniThrowIOException(env, errno);
        return E_PLATFORM_IO_EXCEPTION;
    }
    *number = rc;
    return NOERROR;
}

ECode OSFileSystem::WriteDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    if (length == 0) {
        return 0;
    }
    Byte* src = reinterpret_cast<Byte*>(address + offset);
    Int64 rc = TEMP_FAILURE_RETRY(write(fd, src, length));
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return E_PLATFORM_IO_EXCEPTION;
    }
    *number = rc;
    return NOERROR;
}

ECode OSFileSystem::Read(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    if (bytes == NULL || bytes->GetPayload() == NULL) {
        *number = 0;
        return NOERROR;
    }
    Int32 buf = static_cast<Int32>(reinterpret_cast<uintptr_t>(bytes->GetPayload()));
    return ReadDirect(fd, buf, offset, length, number);
}

ECode OSFileSystem::Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    if (bytes.GetPayload() == NULL) {
        *number = 0;
        return NOERROR;
    }
    Int32 buf = static_cast<Int32>(reinterpret_cast<uintptr_t>(bytes.GetPayload()));
    return WriteDirect(fd, buf, offset, length, number);
}

ECode OSFileSystem::Readv(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Int32>& addresses,
    /* [in] */ const ArrayOf<Int32>& offsets,
    /* [in] */ const ArrayOf<Int32>& lengths,
    /* [in] */ Int32 size,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    iovec* vectors = initIoVec(addresses, offsets, lengths, size);
    if (vectors == NULL) {
        *number = -1;
        return NOERROR;
    }
    Int64 result = readv(fd, vectors, size);
    if (result == 0) {
        *number = -1;
        return NOERROR;
    }
    if (result == -1) {
//        jniThrowIOException(env, errno);
        *number = result;
        return E_PLATFORM_IO_EXCEPTION;
    }
    *number = result;
    return NOERROR;
}

ECode OSFileSystem::Writev(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Int32>& addresses,
    /* [in] */ const ArrayOf<Int32>& offsets,
    /* [in] */ const ArrayOf<Int32>& lengths,
    /* [in] */ Int32 size,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    iovec* vectors = initIoVec(addresses, offsets, lengths, size);
    if (vectors == NULL) {
        *number = -1;
        return NOERROR;
    }
    Int64 result = writev(fd, vectors, size);
    if (result == -1) {
//        jniThrowIOException(env, errno);
        *number = result;
        return E_PLATFORM_IO_EXCEPTION;
    }
    *number = result;
    return NOERROR;
}

ECode OSFileSystem::Truncate(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 length)
{
    if (offsetTooLarge(length)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 rc = ftruncate(fd, length);
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return E_PLATFORM_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode OSFileSystem::Open(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);
    Int32 flags = 0;
    Int32 nMode = 0;

    // On Android, we don't want default permissions to allow global access.
    switch (mode) {
    case 0:
        flags = HyOpenRead;
        nMode = 0;
        break;
    case 1:
        flags = HyOpenCreate | HyOpenWrite | HyOpenTruncate;
        nMode = 0600;
        break;
    case 16:
        flags = HyOpenRead | HyOpenWrite | HyOpenCreate;
        nMode = 0600;
        break;
    case 32:
        flags = HyOpenRead | HyOpenWrite | HyOpenCreate | HyOpenSync;
        nMode = 0600;
        break;
    case 256:
        flags = HyOpenWrite | HyOpenCreate | HyOpenAppend;
        nMode = 0600;
        break;
    }

    flags = EsTranslateOpenFlags(flags);

    if (path.IsNull()) {
        *fd = -1;
        return NOERROR;
    }
    Int32 rc = TEMP_FAILURE_RETRY(open(path.string(), flags, nMode));
    if (rc == -1) {
//        // Get the human-readable form of errno.
//        char buffer[80];
//        const char* reason = jniStrError(errno, &buffer[0], sizeof(buffer));
//
//        // Construct a message that includes the path and the reason.
//        LocalArray<128> message(path.size() + 2 + strlen(reason) + 1 + 1);
//        snprintf(&message[0], message.size(), "%s (%s)", path.c_str(), reason);
//
//        // We always throw FileNotFoundException, regardless of the specific
//        // failure. (This appears to be true of the RI too.)
//        jniThrowException(env, "java/io/FileNotFoundException", &message[0]);
        return E_PLATFORM_FILE_NOT_FOUND_EXCEPTION;
    }
    *fd = rc;
    return NOERROR;
}

ECode OSFileSystem::Transfer(
    /* [in] */ Int32 fileDescriptor,
    /* [in] */ Int32 socketDescriptor,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    Int32 fd = fileDescriptor;
    Int32 socket = socketDescriptor;
    if (fd == -1 || socket == -1) {
        *number = -1;
        return NOERROR;
    }

    /* Value of offset is checked in jint scope (checked in java layer)
       The conversion here is to guarantee no value lost when converting offset to off_t
     */
    off_t off = offset;

    ssize_t rc = sendfile(socket, fd, &off, count);
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return E_PLATFORM_IO_EXCEPTION;
    }
    *number = rc;
    return NOERROR;
}

ECode OSFileSystem::IoctlAvailable(
    /* [in] */ Int32 fileDescriptor,
    /* [out] */ Int32* avail)
{
    /*
     * On underlying platforms Android cares about (read "Linux"),
     * ioctl(fd, FIONREAD, &avail) is supposed to do the following:
     *
     * If the fd refers to a regular file, avail is set to
     * the difference between the file size and the current cursor.
     * This may be negative if the cursor is past the end of the file.
     *
     * If the fd refers to an open socket or the read end of a
     * pipe, then avail will be set to a number of bytes that are
     * available to be read without blocking.
     *
     * If the fd refers to a special file/device that has some concept
     * of buffering, then avail will be set in a corresponding way.
     *
     * If the fd refers to a special device that does not have any
     * concept of buffering, then the ioctl call will return a negative
     * number, and errno will be set to ENOTTY.
     *
     * If the fd refers to a special file masquerading as a regular file,
     * then avail may be returned as negative, in that the special file
     * may appear to have zero size and yet a previous read call may have
     * actually read some amount of data and caused the cursor to be
     * advanced.
     */
    VALIDATE_NOT_NULL(avail);
    Int32 fd = fileDescriptor;
    if (fd == -1) {
        *avail = -1;
        return NOERROR;
    }
    Int32 nAvail = 0;
    Int32 rc = ioctl(fd, FIONREAD, &nAvail);
    if (rc >= 0) {
        /*
         * Success, but make sure not to return a negative number (see
         * above).
         */
        if (nAvail < 0) {
            nAvail = 0;
        }
    }
    else if (errno == ENOTTY) {
        /* The fd is unwilling to opine about its read buffer. */
        nAvail = 0;
    }
    else {
        /* Something strange is happening. */
//        jniThrowIOException(env, errno);
        return E_PLATFORM_IO_EXCEPTION;
    }

    *avail = nAvail;
    return NOERROR;
}
