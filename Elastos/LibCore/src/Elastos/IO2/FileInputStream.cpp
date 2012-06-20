#include "cmdef.h"
#include "FileInputStream.h"


FileInputStream::FileInputStream()
{
}

FileInputStream::~FileInputStream()
{
    Close();
}

ECode FileInputStream::Init(
    /* [in] */ IFile* file)
{
    return E_NOT_IMPLEMENTED;
}

ECode FileInputStream::Init(
    /* [in] */ IFileDescriptor* fd)
{
    return E_NOT_IMPLEMENTED;
}

ECode FileInputStream::Init(
    /* [in] */ String fileName)
{
    return E_NOT_IMPLEMENTED;
}

ECode FileInputStream::Available(
    /* [out] */ Int32* isAvailable)
{
    FAIL_RETURN(OpenCheck());
    return E_NOT_IMPLEMENTED;
}

ECode FileInputStream::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode FileInputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
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
    BufferOf<Byte>* readed = BufferOf<Byte>::Alloc(1);
    FAIL_RETURN(ReadBufferEx(0, 1, readed, value));
    *value = *value == -1 ? -1 : (*readed)[0] & 0xff;
    BufferOf<Byte>::Free(readed);
    return NOERROR;;
}

ECode FileInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    FAIL_RETURN(ReadBufferEx(0, buffer->GetCapacity(), buffer, number));
    return NOERROR;
}

ECode FileInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte>* buffer,
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
    if ((offset | length) < 0 || length > buffer->GetCapacity() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed

    if (0 == length) {
        *number = 0;
        return NOERROR;
    }

    FAIL_RETURN(OpenCheck());
//    synchronized (repositioningLock) {
//            // BEGIN android-changed
//            // If you only support Linux, there's nothing special about stdin.
//            return (int) fileSystem.read(fd.descriptor, buffer, offset, count);
//            // END android-changed
//        }
    return E_NOT_IMPLEMENTED;
}

ECode FileInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * number)
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

//    // The RI doesn't treat stdin as special. It throws IOException for
//        // all non-seekable streams, so we do too. If you did want to support
//        // non-seekable streams, the best way to do it would be to recognize
//        // when lseek(2) fails with ESPIPE and call super.skip(count).
//        synchronized (repositioningLock) {
//            // Our seek returns the new offset, but we know it will throw an
//            // exception if it couldn't perform exactly the seek we asked for.
//            fileSystem.seek(fd.descriptor, count, IFileSystem.SEEK_CUR);
//            return count;
//        }
    return E_NOT_IMPLEMENTED;
}

///////////////synchronized
ECode FileInputStream::OpenCheck()
{
//    if(Fd->mDescriptor < 0){   /////////////////////E_NOT_IMPLEMENTED
//        return E_IO_EXCEPTION;
//    }
    return NOERROR;
}
