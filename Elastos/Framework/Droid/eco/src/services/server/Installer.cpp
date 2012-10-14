
#include "server/Installer.h"
#include <Slogger.h>
#include <StringBuffer.h>

using namespace Elastos::Utility::Logging;
using namespace Elastos::Core;

CString Installer::TAG = "Installer";

Installer::Installer()
{
    mBuf = ArrayOf<Byte>::Alloc(1024);
}

Installer::~Installer()
{
    ArrayOf<Byte>::Free(mBuf);
}

Boolean Installer::Connect()
{
    if (mSocket != NULL) {
        return TRUE;
    }
    Slogger::I(TAG, "connecting...");
    ECode ec = CLocalSocket::New((ILocalSocket**)&mSocket);
    if (FAILED(ec)) {
        Disconnect();
        return FALSE;
    }

    AutoPtr<ILocalSocketAddress> address;
    CLocalSocketAddress::New(
        String("installd"), LocalSocketAddressNamespace_RESERVED,
        (ILocalSocketAddress**)&address);

    ec = mSocket->Connect(address);
    if (FAILED(ec)) {
        Disconnect();
        return FALSE;
    }

    ec = mSocket->GetInputStream((IInputStream**)&mIn);
    if (FAILED(ec)) {
        Disconnect();
        return FALSE;
    }
    ec = mSocket->GetOutputStream((IOutputStream**)&mOut);
    if (FAILED(ec)) {
        Disconnect();
        return FALSE;
    }
    return TRUE;
}

void Installer::Disconnect()
{
    Slogger::I(TAG, "disconnecting...");
    if (mSocket != NULL) mSocket->Close();
    if (mIn != NULL) mIn->Close();
    if (mOut != NULL) mOut->Close();
    mSocket = NULL;
    mIn = NULL;
    mOut = NULL;
}

Boolean Installer::ReadBytes(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 len)
{
    Int32 off = 0, count;
    if (len < 0) return FALSE;
    while (off != len) {
        if (SUCCEEDED(mIn->ReadBufferEx(off, len - off, buffer, &count))) {
            if (count <= 0) {
                Slogger::E(TAG, StringBuffer("read error ") + count);
                break;
            }
            off += count;
        }
        else {
            Slogger::E(TAG, "read exception");
            break;
        }
    }
//        Slog.i(TAG, "read "+len+" bytes");
    if (off == len) return TRUE;
    Disconnect();
    return FALSE;
}

Boolean Installer::ReadReply()
{
    Int32 len;
    mBuflen = 0;
    if (!ReadBytes(mBuf, 2)) return FALSE;
    len = (((Int32)(*mBuf)[0]) & 0xff) | ((((Int32)(*mBuf)[1]) & 0xff) << 8);
    if ((len < 1) || (len > 1024)) {
        Slogger::E(TAG, StringBuffer("invalid reply length (") + len + ")");
        Disconnect();
        return FALSE;
    }
    if (!ReadBytes(mBuf, len)) return FALSE;
    mBuflen = len;
    return TRUE;
}

Boolean Installer::WriteCommand(
    /* [in] */ CString _cmd)
{
    ArrayOf<Byte> cmd((Byte*)(const char*)_cmd, _cmd.GetLength());
    Int32 len = cmd.GetLength();
    if ((len < 1) || (len > 1024)) return FALSE;
    (*mBuf)[0] = (Byte)(len & 0xff);
    (*mBuf)[1] = (byte)((len >> 8) & 0xff);
    if (SUCCEEDED(mOut->WriteBufferEx(0, 2, *mBuf))) {
        if (SUCCEEDED(mOut->WriteBufferEx(0, len, cmd))) {
            return TRUE;
        }
    }
    Slogger::E(TAG, "write error");
    Disconnect();
    return FALSE;
}

String Installer::Transaction(
    /* [in] */ CString cmd)
{
    Mutex::Autolock lock(&mLock);

    if (!Connect()) {
        Slogger::E(TAG, "connection failed");
        return String("-1");
    }

    if (!WriteCommand(cmd)) {
            /* If installd died and restarted in the background
             * (unlikely but possible) we'll fail on the next
             * write (this one).  Try to reconnect and write
             * the command one more time before giving up.
             */
        Slogger::E(TAG, "write command failed? reconnect!");
        if (!Connect() || !WriteCommand(cmd)) {
            return String("-1");
        }
    }
//    Slog.i(TAG,"send: '"+cmd+"'");
    if (ReadReply()) {
//        Slog.i(TAG,"recv: '"+s+"'");
        return String((const char*)mBuf->GetPayload(), mBuflen);
    } else {
//        Slog.i(TAG,"fail");
        return String("-1");
    }
}

Int32 Installer::Execute(
    /* [in] */ CString cmd)
{
    String res = Transaction(cmd);
//	try {
    return res.ToInt32();
//	} catch (NumberFormatException ex) {
//		return -1;
//	}
}

Int32 Installer::Install(
    /* [in] */ CString name,
    /* [in] */ Boolean useEncryptedFilesystem,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    StringBuffer builder("install");
    builder += ' ';
    builder += name;
    builder += ' ';
    if (useEncryptedFilesystem) {
        builder += '1';
    }
    else {
        builder += '0';
    }
    builder += ' ';
    builder += uid;
    builder += ' ';
    builder += gid;
    return Execute(builder);
}

Int32 Installer::DexOpt(
    /* [in] */ CString apkPath,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isPublic)
{
//    StringBuilder builder = new StringBuilder("dexopt");
//    builder.append(' ');
//    builder.append(apkPath);
//    builder.append(' ');
//    builder.append(uid);
//    builder.append(isPublic ? " 1" : " 0");
//    return execute(builder.toString());
    return -1;
}

Int32 Installer::MoveDex(
    /* [in] */ CString srcPath,
    /* [in] */ CString dstPath)
{
//    StringBuilder builder = new StringBuilder("movedex");
//    builder.append(' ');
//    builder.append(srcPath);
//    builder.append(' ');
//    builder.append(dstPath);
//    return execute(builder.toString());
    return -1;
}

Int32 Installer::RmDex(
    /* [in] */ CString codePath)
{
//    StringBuilder builder = new StringBuilder("rmdex");
//    builder.append(' ');
//    builder.append(codePath);
//    return execute(builder.toString());
    return -1;
}

Int32 Installer::Remove(
    /* [in] */ CString name,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    StringBuffer builder("remove");
    builder += ' ';
    builder += name;
    builder += ' ';
    if (useEncryptedFilesystem) {
        builder += '1';
    }
    else {
        builder += '0';
    }
    return Execute(builder);
}

Int32 Installer::Rename(
    /* [in] */ CString oldname,
    /* [in] */ CString newname,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    StringBuffer builder("rename");
    builder += ' ';
    builder += oldname;
    builder += ' ';
    builder += newname;
    builder += ' ';
    if (useEncryptedFilesystem) {
        builder += '1';
    }
    else {
        builder += '0';
    }
    return Execute(builder);
}

Int32 Installer::DeleteCacheFiles(
    /* [in] */ CString name,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    StringBuffer builder("rmcache");
    builder += ' ';
    builder += name;
    builder += ' ';
    if (useEncryptedFilesystem) {
        builder += '1';
    }
    else {
        builder += '0';
    }
    return Execute(builder);
}

Int32 Installer::ClearUserData(
    /* [in] */ CString name,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    StringBuffer builder("rmuserdata");
    builder += ' ';
    builder += name;
    builder += ' ';
    if (useEncryptedFilesystem) {
        builder += '1';
    }
    else {
        builder += '0';
    }
    return Execute(builder);
}

Boolean Installer::Ping()
{
    if (Execute("ping") < 0) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}

Int32 Installer::FreeCache(
    /* [in] */ Int64 freeStorageSize)
{
    StringBuffer builder("freecache");
    builder += ' ';
    builder += String::FromInt64(freeStorageSize);
    return Execute(builder);
}

Int32 Installer::SetForwardLockPerm(
    /* [in] */ CString capsulePathSuffix,
    /* [in] */ Int32 gid)
{
    StringBuffer builder("protect");
    builder += ' ';
    builder += capsulePathSuffix;
    builder += ' ';
    builder += gid;
    return Execute(builder);
}

Int32 Installer::GetSizeInfo(
    /* [in] */ CString capName,
    /* [in] */ CString capPath,
    /* [in] */ CString fwdLockCapPath,
    /* [in] */ ICapsuleStats* stats,
    /* [in] */ Boolean useEncryptedFilesystem)
{
    StringBuffer builder("getsize");
    builder += ' ';
    builder += capName;
    builder += ' ';
    builder += capPath;
    builder += ' ';
    builder += !fwdLockCapPath.IsNull() ? fwdLockCapPath : "!";
    builder += ' ';
    if (useEncryptedFilesystem) {
        builder += '1';
    }
    else {
        builder += '0';
    }

    String s = Transaction(builder);
//    String res[] = s.split(" ");
//
//    if((res == null) || (res.length != 4)) {
//        return -1;
//    }
//    try {
//        pStats.codeSize = Long.parseLong(res[1]);
//        pStats.dataSize = Long.parseLong(res[2]);
//        pStats.cacheSize = Long.parseLong(res[3]);
//        return Integer.parseInt(res[0]);
//    } catch (NumberFormatException e) {
//        return -1;
//    }
    return -1;
}

Int32 Installer::MoveFiles()
{
    return Execute("movefiles");
}

Int32 Installer::LinkNativeLibraryDirectory(
    /* [in] */ CString dataPath,
    /* [in] */ CString nativeLibPath)
{
    if (dataPath.IsNull()) {
        Slogger::E(TAG, "unlinkNativeLibraryDirectory dataPath is null");
        return -1;
    }
    else if (nativeLibPath.IsNull()) {
        Slogger::E(TAG, "unlinkNativeLibraryDirectory nativeLibPath is null");
        return -1;
    }

    StringBuffer builder("linklib ");
    builder += dataPath;
    builder += ' ';
    builder += nativeLibPath;

    return Execute(builder);
}

Int32 Installer::UnlinkNativeLibraryDirectory(
    /* [in] */ CString dataPath)
{
    if (dataPath.IsNull()) {
        Slogger::E(TAG, "unlinkNativeLibraryDirectory dataPath is null");
        return -1;
    }

    StringBuffer builder("unlinklib ");
    builder += dataPath;

    return Execute(builder);
}
