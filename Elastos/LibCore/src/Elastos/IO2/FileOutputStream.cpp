#include "cmdef.h"
#include "FileOutputStream.h"


FileOutputStream::FileOutputStream()
{
}

FileOutputStream::~FileOutputStream()
{
    Close();
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
    return E_NOT_IMPLEMENTED;
}

ECode FileOutputStream::Init(
    /* [in] */ IFileDescriptor* fd)
{
    return E_NOT_IMPLEMENTED;
}

ECode FileOutputStream::Init(
    /* [in] */ String fileName)
{
    return Init(fileName, FALSE);
}

ECode FileOutputStream::Init(
    /* [in] */ String fileName,
    /* [in] */ Boolean append)
{
    return E_NOT_IMPLEMENTED;
}

ECode FileOutputStream::Close()
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
//        synchronized (this) {
//            if (fd.valid() && innerFD) {
//                IoUtils.close(fd);
//            }
//        }
    return E_NOT_IMPLEMENTED;
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

    BufferOf<Byte>* byteArray = BufferOf<Byte>::Alloc(1);
    (*byteArray)[0] = (Byte)oneByte;

    ///////////////////////fileSystem.write(fd.descriptor, byteArray, 0, 1);   E_NOT_IMPLEMENTED
    //todo: free byteArray

    return NOERROR;
}

ECode FileOutputStream::WriteBuffer(
    /* [in] */ const BufferOf<Byte>& buffer)
{
    return WriteBufferEx(0, buffer.GetCapacity(), buffer);
}

ECode FileOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const BufferOf<Byte>& buffer)
{
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, made implicit null check explicit,
    // used (offset | count) < 0 instead of (offset < 0) || (count < 0)
    // to safe one operation
    if ((offset | count) < 0 || count > buffer.GetCapacity() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed

    if (0 == count) {
        return NOERROR;
    }

    FAIL_RETURN(OpenCheck());
    //fileSystem.write(fd.descriptor, buffer, offset, count);
    return NOERROR;
}

///////////////synchronized
ECode FileOutputStream::OpenCheck()
{
//    if(Fd->mDescriptor < 0){   /////////////////////E_NOT_IMPLEMENTED
//        return E_IO_EXCEPTION;
//    }
    return E_NOT_IMPLEMENTED;
}
