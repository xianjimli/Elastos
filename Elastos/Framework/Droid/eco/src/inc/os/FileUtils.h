
#ifndef __HH_FILEUTILS_H
#define __HH_FILEUTILS_H

#include "ext/frameworkext.h"

class FileUtils
{
public:
    static const Int32 IRWXU = 00700;
    static const Int32 IRUSR = 00400;
    static const Int32 IWUSR = 00200;
    static const Int32 IXUSR = 00100;

    static const Int32 IRWXG = 00070;
    static const Int32 IRGRP = 00040;
    static const Int32 IWGRP = 00020;
    static const Int32 IXGRP = 00010;

    static const Int32 IRWXO = 00007;
    static const Int32 IROTH = 00004;
    static const Int32 IWOTH = 00002;
    static const Int32 IXOTH = 00001;

public:
    /**
     * Get the status for the given path. This is equivalent to the POSIX stat(2) system call.
     * @param path The path of the file to be stat'd.
     * @param status Optional argument to fill in. It will only fill in the status if the file
     * exists.
     * @return true if the file exists and false if it does not exist. If you do not have
     * permission to stat the file, then this method will return false.
     */
    static CARAPI_(Boolean) GetFileStatus(
        /* [in] */ CString path,
        /* [in, out] */ FileStatus* status);

    static CARAPI_(Int32) SetPermissions(
        /* [in] */ CString file,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    static CARAPI_(Int32) GetPermissions(
        /* [in] */ CString file,
        /* [in, out] */ ArrayOf<Int32>* outPermissions);

    /** returns the FAT file system volume ID for the volume mounted
     * at the given mount point, or -1 for failure
     * @param mount point for FAT volume
     * @return volume ID or -1
     */
    static CARAPI_(Int32) GetFatVolumeId(
        /* [in] */ CString mountPoint);

    /**
     * Perform an fsync on the given FileOutputStream.  The stream at this
     * point must be flushed but not yet closed.
     */
    static CARAPI_(Boolean) Sync(
        /* [in] */ IFileOutputStream* stream);

    // copy a file from srcFile to destFile, return true if succeed, return
    // false if fail
    static CARAPI_(Boolean) CopyFile(
        /* [in] */ IFile* srcFile,
        /* [in] */ IFile* destFile);

    /**
     * Copy data from a source stream to destFile.
     * Return true if succeed, return false if failed.
     */
    static CARAPI_(Boolean) CopyToFile(
        /* [in] */ IInputStream* inputStream,
        /* [in] */ IFile* destFile);

    /**
     * Check if a filename is "safe" (no metacharacters or spaces).
     * @param file  The file to check
     */
    static CARAPI_(Boolean) IsFilenameSafe(
        /* [in] */ IFile* file);

    /**
     * Read a text file into a String, optionally limiting the length.
     * @param file to read (will not seek, so things like /proc files are OK)
     * @param max length (positive for head, negative of tail, 0 for no limit)
     * @param ellipsis to add of the file was truncated (can be null)
     * @return the contents of the file, possibly truncated
     * @throws IOException if something goes wrong reading the file
     */
    static CARAPI_(String) ReadTextFile(
        /* [in] */ IFile* file,
        /* [in] */ Int32 max,
        /* [in] */ CString ellipsis);

private:
//    /** Regular expression for safe filenames: no spaces or metacharacters */
//    private static final Pattern SAFE_FILENAME_PATTERN = Pattern.compile("[\\w%+,./=_-]+");
};

#endif // __HH_FILEUTILS_H

