
#include "os/FileUtils.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#if HAVE_ANDROID_OS
#include <sys/ioctl.h>
#include <linux/msdos_fs.h>
#endif

const Int32 FileUtils::IRWXU;
const Int32 FileUtils::IRUSR;
const Int32 FileUtils::IWUSR;
const Int32 FileUtils::IXUSR;

const Int32 FileUtils::IRWXG;
const Int32 FileUtils::IRGRP;
const Int32 FileUtils::IWGRP;
const Int32 FileUtils::IXGRP;

const Int32 FileUtils::IRWXO;
const Int32 FileUtils::IROTH;
const Int32 FileUtils::IWOTH;
const Int32 FileUtils::IXOTH;

Boolean FileUtils::GetFileStatus(
    /* [in] */ CString path,
    /* [in, out] */ FileStatus* fileStatus)
{
    const char* pathStr = (const char*)path;
    Boolean ret = FALSE;

    struct stat s;
    int res = stat(pathStr, &s);
    if (res == 0) {
        ret = TRUE;
        if (fileStatus != NULL) {
            fileStatus->mDev = s.st_dev;
            fileStatus->mIno = s.st_ino;
            fileStatus->mMode = s.st_mode;
            fileStatus->mNlink = s.st_nlink;
            fileStatus->mUid = s.st_uid;
            fileStatus->mGid = s.st_gid;
            fileStatus->mSize = s.st_size;
            fileStatus->mBlksize = s.st_blksize;
            fileStatus->mBlocks = s.st_blocks;
            fileStatus->mAtime = s.st_atime;
            fileStatus->mMtime = s.st_mtime;
            fileStatus->mCtime = s.st_ctime;
        }
    }

    return ret;
}

Int32 FileUtils::SetPermissions(
    /* [in] */ CString file,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
#if HAVE_ANDROID_OS
    if (file.GetLength() <= 0) {
        return ENOENT;
    }
    if (uid >= 0 || gid >= 0) {
        int res = chown((const char*)file, uid, gid);
        if (res != 0) {
            return errno;
        }
    }
    return chmod((const char*)file, mode) == 0 ? 0 : errno;
#else
    return ENOSYS;
#endif
}

Int32 FileUtils::GetPermissions(
    /* [in] */ CString file,
    /* [in, out] */ ArrayOf<Int32>* outPermissions)
{
#if HAVE_ANDROID_OS
    if (file.GetLength() <= 0) {
        return ENOENT;
    }
    struct stat st;
    if (stat((const char*)file, &st) != 0) {
        return errno;
    }
    if (outPermissions) {
        Int32 len = outPermissions->GetLength();
        if (len >= 1) {
            (*outPermissions)[0] = st.st_mode;
        }
        if (len >= 2) {
            (*outPermissions)[1] = st.st_uid;
        }
        if (len >= 3) {
            (*outPermissions)[2] = st.st_gid;
        }
    }
    return 0;
#else
    return ENOSYS;
#endif
}

Int32 FileUtils::GetFatVolumeId(
    /* [in] */ CString mountPoint)
{
#if HAVE_ANDROID_OS
    if (mountPoint.IsNull()) {
//        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return -1;
    }
    const char *pathStr = (const char*)mountPoint;
    Int32 result = -1;
    // only if our system supports this ioctl
#ifdef VFAT_IOCTL_GET_VOLUME_ID
    int fd = open(pathStr, O_RDONLY);
    if (fd >= 0) {
        result = ioctl(fd, VFAT_IOCTL_GET_VOLUME_ID);
        close(fd);
    }
#endif

    return result;
#else
    return -1;
#endif
}

Boolean FileUtils::Sync(
    /* [in] */ IFileOutputStream* stream)
{
    if (stream != NULL) {
        AutoPtr<IFileDescriptor> fd;
        stream->GetFD((IFileDescriptor**)&fd);
        if (SUCCEEDED(fd->Sync())) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean FileUtils::CopyFile(
    /* [in] */ IFile* srcFile,
    /* [in] */ IFile* destFile)
{
    Boolean result = FALSE;
    AutoPtr<IInputStream> in;
    if (SUCCEEDED(CFileInputStream::New(srcFile, (IFileInputStream**)&in))) {
        result = CopyToFile(in, destFile);
        in->Close();
    }
    return result;
}

Boolean FileUtils::CopyToFile(
    /* [in] */ IInputStream* inputStream,
    /* [in] */ IFile* destFile)
{
    Boolean result;
    destFile->Exists(&result);
    if (result) {
        destFile->Delete(&result);
    }
    AutoPtr<IFileOutputStream> out;
    if (SUCCEEDED(CFileOutputStream::New(destFile, (IFileOutputStream**)&out))) {
        AutoFree< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(4096);
        Int32 bytesRead;
        if (SUCCEEDED(inputStream->ReadBuffer(buffer, &bytesRead))) {
            while (bytesRead >= 0) {
                if (FAILED(out->WriteBufferEx(0, bytesRead, *buffer))) break;
                if (FAILED(inputStream->ReadBuffer(buffer.Get(), &bytesRead))) break;
            }
        }

        out->Flush();
        AutoPtr<IFileDescriptor> fd;
        out->GetFD((IFileDescriptor**)&fd);
        fd->Sync();
        out->Close();
        return TRUE;
    }
    return FALSE;
}

Boolean FileUtils::IsFilenameSafe(
    /* [in] */ IFile* file)
{
    // Note, we check whether it matches what's known to be safe,
    // rather than what's known to be unsafe.  Non-ASCII, control
    // characters, etc. are all unsafe by default.
//    return SAFE_FILENAME_PATTERN.matcher(file.getPath()).matches();
    return FALSE;
}

String FileUtils::ReadTextFile(
    /* [in] */ IFile* file,
    /* [in] */ Int32 max,
    /* [in] */ CString ellipsis)
{
    AutoPtr<IInputStream> input;
    CFileInputStream::New(file, (IFileInputStream**)&input);
    Int64 size;
    file->GetLength(&size);
    if (max > 0 || (size > 0 && max == 0)) {  // "head" mode: read the first N bytes
        if (size > 0 && (max == 0 || size < max)) max = (Int32)size;
        AutoFree< ArrayOf<Byte> > data = ArrayOf<Byte>::Alloc(max + 1);
        Int32 length;
        input->ReadBuffer(data.Get(), &length);
        input->Close();
        if (length <= 0) return String("");
        if (length <= max) return String((char*)data->GetPayload(), length);
        if (ellipsis.IsNull()) return String((char*)data->GetPayload(), max);
        return String((char*)data->GetPayload(), max) + ellipsis;
    }
    else if (max < 0) {  // "tail" mode: keep the last N
        Int32 len;
        Boolean rolled = FALSE;
        AutoFree< ArrayOf<Byte> > last = NULL, data = NULL;
        do {
            if (last != NULL) rolled = TRUE;
            ArrayOf<Byte>* tmp = last; last = data; data = tmp;
            if (data == NULL) data = ArrayOf<Byte>::Alloc(-max);
            input->ReadBuffer(data.Get(), &len);
        } while (len == data->GetLength());

        input->Close();
        if (last == NULL && len <= 0) return String("");
        if (last == NULL) return String((char*)data->GetPayload(), len);
        if (len > 0) {
            rolled = TRUE;
            memcpy(last, last + len, last->GetLength() - len);
            memcpy(last + last->GetLength() - len, data, len);
        }
        if (ellipsis.IsNull() || !rolled) return String((char*)last->GetPayload());
        return String(ellipsis) + String((char*)last->GetPayload());
    }
    else {  // "cat" mode: size unknown, read it all in streaming fashion
        AutoPtr<IByteArrayOutputStream> contents;
        CByteArrayOutputStream::New((IByteArrayOutputStream**)&contents);
        Int32 len;
        AutoFree< ArrayOf<Byte> > data = ArrayOf<Byte>::Alloc(1024);
        do {
            input->ReadBuffer(data, &len);
            if (len > 0) contents->WriteBufferEx(0, len, *data);
        } while (len == data->GetLength());
        input->Close();
        String str;
        contents->ToString(&str);
        return str;
    }
}
