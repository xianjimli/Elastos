
#ifndef __HH_INSTALLER_H
#define __HH_INSTALLER_H

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos.h>

using namespace Elastos;

class Installer
{
public:
    Installer();

    ~Installer();

    CARAPI_(Int32) Install(
        /* [in] */ String name,
        /* [in] */ Boolean useEncryptedFilesystem,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI_(Int32) Dexopt(
        /* [in] */ String apkPath,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean isPublic);

    CARAPI_(Int32) Movedex(
        /* [in] */ String srcPath,
        /* [in] */ String dstPath);

    CARAPI_(Int32) Rmdex(
        /* [in] */ String codePath);

    CARAPI_(Int32) Remove(
        /* [in] */ String name,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Int32) Rename(
        /* [in] */ String oldname,
        /* [in] */ String newname,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Int32) DeleteCacheFiles(
        /* [in] */ String name,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Int32) ClearUserData(
        /* [in] */ String name,
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
        /* [in] */ String capsulePathSuffix,
        /* [in] */ Int32 gid);

    CARAPI_(Int32) GetSizeInfo(
        /* [in] */ String capName,
        /* [in] */ String apkPath,
        /* [in] */ String fwdLockApkPath,
        /* [in] */ ICapsuleStats* stats,
        /* [in] */ Boolean useEncryptedFilesystem);

    CARAPI_(Int32) MoveFiles();

    CARAPI_(Int32) LinkNativeLibraryDirectory(
        /* [in] */ String dataPath,
        /* [in] */ String nativeLibPath);

    CARAPI_(Int32) UnlinkNativeLibraryDirectory(
        /* [in] */ String dataPath);

private:
    static const String TAG;

public:
	AutoPtr<IInputStream> mIn;
	AutoPtr<IOutputStream> mOut;
//		LocalSocket mSocket;

//		byte buf[] = new byte[1024];
//		int buflen = 0;
//	    AutoFree< ArrayOf<Byte> > mBuf;
    Byte* mBuf;
    Int32 mBufLen;
};

#endif // __HH_INSTALLER_H

