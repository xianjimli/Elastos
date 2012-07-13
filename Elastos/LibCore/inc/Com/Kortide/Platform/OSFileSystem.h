
#ifndef __OSFILESYSTEM_H__
#define __OSFILESYSTEM_H__

#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "Com.Kortide.Platform_server.h"

class OSFileSystem : public ElRefBase, public IFileSystem
{
public:
    static CARAPI_(AutoPtr<IFileSystem>) GetOSFileSystem();

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Returns the granularity for virtual memory allocation.
     * Note that this value for Windows differs from the one for the
     * page size (64K and 4K respectively).
     */
    CARAPI GetAllocGranularity(
        /* [out] */ Int32* granularity);

    CARAPI GetLength(
        /* [in] */ Int32 fd,
        /* [out] */ Int64* size);

    CARAPI Lock(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 start,
        /* [in] */ Int64 length,
        /* [in] */ Int32 type,
        /* [in] */ Boolean waitFlag,
        /* [out] */ Boolean* succeeded);

    CARAPI Unlock(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 start,
        /* [in] */ Int64 length);

    CARAPI Fsync(
        /* [in] */ Int32 fd,
        /* [in] */ Boolean metadata);

    CARAPI Seek(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int32 whence,
        /* [out] */ Int64* number);

    CARAPI ReadDirect(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* number);

    CARAPI WriteDirect(
        /* [in] */ Int32 fd,
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* number);

    CARAPI Read(
        /* [in] */ Int32 fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* number);

    CARAPI Write(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* number);

    CARAPI Readv(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Int32>& addresses,
        /* [in] */ const ArrayOf<Int32>& offsets,
        /* [in] */ const ArrayOf<Int32>& lengths,
        /* [in] */ Int32 size,
        /* [out] */ Int64* number);

    CARAPI Writev(
        /* [in] */ Int32 fd,
        /* [in] */ const ArrayOf<Int32>& addresses,
        /* [in] */ const ArrayOf<Int32>& offsets,
        /* [in] */ const ArrayOf<Int32>& lengths,
        /* [in] */ Int32 size,
        /* [out] */ Int64* number);

    CARAPI Truncate(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 length);

    CARAPI Open(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode,
        /* [out] */ Int32* fd);

    CARAPI Transfer(
        /* [in] */ Int32 fileDescriptor,
        /* [in] */ Int32 socketDescriptor,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI IoctlAvailable(
        /* [in] */ Int32 fileDescriptor,
        /* [out] */ Int32* avail);

private:
    OSFileSystem() {}

    CARAPI ValidateLockArgs(
        /* [in] */ Int32 type,
        /* [in] */ Int64 start,
        /* [in] */ Int64 length);

    CARAPI_(Int32) LockImpl(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 start,
        /* [in] */ Int64 length,
        /* [in] */ Int32 type,
        /* [in] */ Boolean wait);

    CARAPI UnlockImpl(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 start,
        /* [in] */ Int64 length);

private:
    static const AutoPtr<OSFileSystem> sSingleton;
};

#endif //__OSFILESYSTEM_H__
