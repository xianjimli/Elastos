#include "os/MemoryFile.h"

const String MemoryFile::TAG = "MemoryFile";
const Int32 MemoryFile::PROT_READ;
const Int32 MemoryFile::PROT_WRITE;

MemoryFile::MemoryFile(
    /* [in] */ String name,
    /* [in] */ Int32 length)
    : mAddress(0)
    , mLength(0)
    , mAllowPurging(FALSE)
    , mOwnsRegion(FALSE)
{
}

MemoryFile::MemoryFile(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 length,
    /* [in] */ String mode)
    : mAddress(0)
    , mLength(0)
    , mAllowPurging(FALSE)
    , mOwnsRegion(FALSE)
{
}

void MemoryFile::Close()
{
}

void MemoryFile::Deactivate()
{
}

Boolean MemoryFile::IsDeactivated()
{
    return FALSE;
}

Boolean MemoryFile::IsClosed()
{
    return FALSE;
}

void MemoryFile::Finalize()
{
}

Int32 MemoryFile::Length()
{
    return -1;
}

Boolean MemoryFile::IsPurgingAllowed()
{
    return FALSE;
}

Boolean MemoryFile::AllowPurging(
    /* [in] */ Boolean allowPurging)
{
    return FALSE;
}

ECode MemoryFile::GetInputStream(
    /* [out] */ IInputStream** stream)
{
    return E_NOT_IMPLEMENTED;
}

ECode MemoryFile::GetOutputStream(
    /* [out] */ IOutputStream** stream)
{
    return E_NOT_IMPLEMENTED;
}

Int32 MemoryFile::ReadBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 destOffset,
    /* [in] */ Int32 count)
{
    return -1;
}

void MemoryFile::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 destOffset,
    /* [in] */ Int32 count)
{
}

ECode MemoryFile::GetParcelFileDescriptor(
    /* [out] */ IParcelFileDescriptor** des)
{
    return E_NOT_IMPLEMENTED;
}

ECode MemoryFile::GetFileDescriptor(
    /* [out] */ IFileDescriptor** des)
{
    return E_NOT_IMPLEMENTED;
}

Boolean MemoryFile::IsMemoryFile(
    /* [in] */ IFileDescriptor* fd)
{
    return FALSE;
}

Int32 MemoryFile::GetSize(
    /* [in] */ IFileDescriptor* fd)
{
    return -1;
}

Int32 MemoryFile::ModeToProt(
    /* [in] */ String mode)
{
    return -1;
}

