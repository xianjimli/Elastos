
#include "server/Installer.h"

const String Installer::TAG = "Installer";

Installer::Installer()
    : mBuf(new Byte[1024])
    , mBufLen(0)
{
}

Installer::~Installer()
{
}

Int32 Installer::Install(
    /* [in] */ String name,
    /* [in] */ Boolean useEncryptedFilesystem,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    return -1;
}

Int32 Installer::Dexopt(
    /* [in] */ String apkPath,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isPublic)
{
    return -1;
}

Int32 Installer::Movedex(
    /* [in] */ String srcPath,
    /* [in] */ String dstPath)
{
    return -1;
}

Int32 Installer::Rmdex(
    /* [in] */ String codePath)
{
    return -1;
}

Int32 Installer::Remove(
    /* [in] */ String name,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    return -1;
}

Int32 Installer::Rename(
    /* [in] */ String oldname,
    /* [in] */ String newname,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    return -1;
}

Int32 Installer::DeleteCacheFiles(
    /* [in] */ String name,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    return -1;
}

Int32 Installer::ClearUserData(
    /* [in] */ String name,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    return -1;
}

Boolean Installer::Ping()
{
    return FALSE;
}

Int32 Installer::FreeCache(
    /* [in] */ Int64 freeStorageSize)
{
    return -1;
}

Int32 Installer::SetForwardLockPerm(
    /* [in] */ String capsulePathSuffix,
    /* [in] */ Int32 gid)
{
    return -1;
}

Int32 Installer::GetSizeInfo(
    /* [in] */ String capName,
    /* [in] */ String apkPath,
    /* [in] */ String fwdLockApkPath,
    /* [in] */ ICapsuleStats* stats,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    return -1;
}

Int32 Installer::MoveFiles()
{
    return -1;
}

Int32 Installer::LinkNativeLibraryDirectory(
    /* [in] */ String dataPath,
    /* [in] */ String nativeLibPath)
{
    return -1;
}

Int32 Installer::UnlinkNativeLibraryDirectory(
    /* [in] */ String dataPath)
{
    return -1;
}
