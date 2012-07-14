
#ifndef __HH_INSTALLER_H
#define __HH_INSTALLER_H

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class Installer
{
public:
    Installer();

    ~Installer();

    CARAPI_(Int32) Install(
        /* [in] */ CString name,
        /* [in] */ Boolean useEncryptedFilesystem,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI_(Int32) DexOpt(
        /* [in] */ CString apkPath,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean isPublic);

    CARAPI_(Int32) MoveDex(
        /* [in] */ CString srcPath,
        /* [in] */ CString dstPath);

    //todo: should rename to RmEco
    CARAPI_(Int32) RmDex(
        /* [in] */ CString codePath);

    CARAPI_(Int32) Remove(
        /* [in] */ CString name,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Int32) Rename(
        /* [in] */ CString oldname,
        /* [in] */ CString newname,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Int32) DeleteCacheFiles(
        /* [in] */ CString name,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Int32) ClearUserData(
        /* [in] */ CString name,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Boolean) Ping();

    CARAPI_(Int32) FreeCache(
        /* [in] */ Int64 freeStorageSize);

    /*
     * @param packagePathSuffix The name of the path relative to install
     * directory. Say if the path name is /data/app/com.test-1.apk,
     * the package suffix path will be com.test-1
     */
    CARAPI_(Int32) SetForwardLockPerm(
        /* [in] */ CString capsulePathSuffix,
        /* [in] */ Int32 gid);

    CARAPI_(Int32) GetSizeInfo(
        /* [in] */ CString capName,
        /* [in] */ CString capPath,
        /* [in] */ CString fwdLockCapPath,
        /* [in] */ ICapsuleStats* stats,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Int32) MoveFiles();

    CARAPI_(Int32) LinkNativeLibraryDirectory(
        /* [in] */ CString dataPath,
        /* [in] */ CString nativeLibPath);

    CARAPI_(Int32) UnlinkNativeLibraryDirectory(
        /* [in] */ CString dataPath);

private:
    CARAPI_(Boolean) Connect();

    CARAPI_(void) Disconnect();

    CARAPI_(Boolean) ReadBytes(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 len);

    CARAPI_(Boolean) ReadReply();

    CARAPI_(Boolean) WriteCommand(
        /* [in] */ CString _cmd);

    CARAPI_(String) Transaction(
        /* [in] */ CString cmd);

    CARAPI_(Int32) Execute(
        /* [in] */ CString cmd);

public:
    AutoPtr<IInputStream> mIn;
    AutoPtr<IOutputStream> mOut;
    AutoPtr<ILocalSocket> mSocket;

    ArrayOf<Byte>* mBuf;
    Int32 mBuflen;

private:
    static CString TAG;

    Mutex mLock;
};

#endif // __HH_INSTALLER_H

