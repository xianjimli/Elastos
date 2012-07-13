
#include "cmdef.h"
#include "WrappedFileSystem.h"

WrappedFileSystem::WrappedFileSystem(
    /* [in] */ IFileSystem* fileSystem)
{
    mFileSystem = fileSystem;
    CBlockGuard::AcquireSingleton((IBlockGuard**)&mBlockGuard);
}

PInterface WrappedFileSystem::Probe(
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

UInt32 WrappedFileSystem::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WrappedFileSystem::Release()
{
    return ElRefBase::Release();
}

ECode WrappedFileSystem::GetInterfaceID(
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

/**
 * Returns the granularity for virtual memory allocation.
 * Note that this value for Windows differs from the one for the
 * page size (64K and 4K respectively).
 */
ECode WrappedFileSystem::GetAllocGranularity(
    /* [out] */ Int32* granularity)
{
    return mFileSystem->GetAllocGranularity(granularity);
}

ECode WrappedFileSystem::GetLength(
    /* [in] */ Int32 fd,
    /* [out] */ Int64* size)
{
    return mFileSystem->GetLength(fd, size);
}

ECode WrappedFileSystem::Lock(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 start,
    /* [in] */ Int64 length,
    /* [in] */ Int32 type,
    /* [in] */ Boolean waitFlag,
    /* [out] */ Boolean* succeeded)
{
    return mFileSystem->Lock(fd, start, length, type, waitFlag, succeeded);
}

ECode WrappedFileSystem::Unlock(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 start,
    /* [in] */ Int64 length)
{
    return mFileSystem->Unlock(fd, start, length);
}

ECode WrappedFileSystem::Fsync(
    /* [in] */ Int32 fd,
    /* [in] */ Boolean metadata)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnWriteToDisk());
    return mFileSystem->Fsync(fd, metadata);
}

ECode WrappedFileSystem::Seek(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 whence,
    /* [out] */ Int64* number)
{
    return mFileSystem->Seek(fd, offset, whence, number);
}

ECode WrappedFileSystem::ReadDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnReadFromDisk());
    return mFileSystem->ReadDirect(fd, address, offset, length, number);
}

ECode WrappedFileSystem::WriteDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnWriteToDisk());
    return mFileSystem->WriteDirect(fd, address, offset, length, number);
}

ECode WrappedFileSystem::Read(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnReadFromDisk());
    return mFileSystem->Read(fd, bytes, offset, length, number);
}

ECode WrappedFileSystem::Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnWriteToDisk());
    return mFileSystem->Write(fd, bytes, offset, length, number);
}

ECode WrappedFileSystem::Readv(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Int32>& addresses,
    /* [in] */ const ArrayOf<Int32>& offsets,
    /* [in] */ const ArrayOf<Int32>& lengths,
    /* [in] */ Int32 size,
    /* [out] */ Int64* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnReadFromDisk());
    return mFileSystem->Readv(fd, addresses, offsets, lengths, size, number);
}

ECode WrappedFileSystem::Writev(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Int32>& addresses,
    /* [in] */ const ArrayOf<Int32>& offsets,
    /* [in] */ const ArrayOf<Int32>& lengths,
    /* [in] */ Int32 size,
    /* [out] */ Int64* number)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnWriteToDisk());
    return mFileSystem->Writev(fd, addresses, offsets, lengths, size, number);
}

ECode WrappedFileSystem::Truncate(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 length)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnWriteToDisk());
    return mFileSystem->Truncate(fd, length);
}

ECode WrappedFileSystem::Open(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode,
    /* [out] */ Int32* fd)
{
    AutoPtr<IPolicy> policy;
    mBlockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnReadFromDisk());
    if (mode != 0) {  // 0 is read-only
        policy->OnWriteToDisk();
    }
    return mFileSystem->Open(path, mode, fd);
}

ECode WrappedFileSystem::Transfer(
    /* [in] */ Int32 fileDescriptor,
    /* [in] */ Int32 socketDescriptor,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    return mFileSystem->Transfer(fileDescriptor, socketDescriptor, offset, count, number);
}

ECode WrappedFileSystem::IoctlAvailable(
    /* [in] */ Int32 fileDescriptor,
    /* [out] */ Int32* avail)
{
    return mFileSystem->IoctlAvailable(fileDescriptor, avail);
}
