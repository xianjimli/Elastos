
#ifndef  __BACKUPHELPERDISPATCHER_H__
#define  __BACKUPHELPERDISPATCHER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>


/** @hide */
class BackupHelperDispatcher:
    public ElRefBase
{
private:
    class Header {
    public:
        Int32 mChunkSize; // not including the header
        String mKeyPrefix;
    };

    struct chunk_header_v1 {
        Int32 mHeaderSize;
        Int32 mVersion;
        Int32 mDataSize; // corresponds to Header.chunkSize
        Int32 mNameLength; // not including the NULL terminator, which is not written to the file
    };

public:
    BackupHelperDispatcher();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(void) AddHelper(
        /* [in] */ const String& keyPrefix,
        /* [in] */ IBackupHelper* helper);

    CARAPI_(void) PerformBackup(
        /* [in] */ IParcelFileDescriptor* oldState,
        /* [in] */ IBackupDataOutput* data,
        /* [in] */ IParcelFileDescriptor* newState)/* throws IOException */;

    CARAPI_(void) PerformRestore(
        /* [in] */ IBackupDataInput* input,
        /* [in] */ Int32 appVersionCode,
        /* [in] */ IParcelFileDescriptor* newState) /*throws IOException */;

private:
    CARAPI_(void) DoOneBackup(
        /* [in] */ IParcelFileDescriptor* oldState,
        /* [in] */ IBackupDataOutput* data,
        /* [in] */ IParcelFileDescriptor* newState,
        /* [in] */ const Header& header,
        /* [in] */ IBackupHelper* helper) /*throws IOException*/;

private:
    static /*native*/ Int32 readHeader_native(
        /* [in] */ Header* h,
        /* [in] */ IFileDescriptor* fd);

    static /*native*/ Int32 skipChunk_native(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 bytesToSkip);

    static /*native*/ Int32 allocateHeader_native(
        /* [in] */ const Header& h,
        /* [in] */ IFileDescriptor* fd);

    static /*native*/ Int32 writeHeader_native(
        /* [in] */ const Header& h,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 pos);

    static Int32 padding_len(
        /* [in] */ Int32 len);

private:
    static CString TAG;
    HashMap<String, AutoPtr<IBackupHelper> > mHelpers;// = new TreeMap<String, AutoPtr<IBackupHelper> >();
};

#endif  //__BACKUPHELPERDISPATCHER_H__
