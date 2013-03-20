#ifndef __FILE_H__
#define __FILE_H__

#include <elastos.h>
#include <Elastos.Core.h>
#include "Elastos.IO_server.h"
#include <sys/stat.h>
#ifdef _linux
#include <sys/statfs.h>
#endif

using namespace Elastos;

class File
{
public:
    File();

    virtual ~File();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Constructs a new file using the specified direc
     tory and name.
     *
     * @param dir
     *            the directory where the file is stored.
     * @param name
     *            the file's name.
     * @throws NullPointerException
     *             if {@code name} is {@code null}.
     */
    CARAPI Init(
        /* [in] */ IFile* dir,
        /* [in] */ const String& name);

    /**
     * Constructs a new file using the specified path.
     *
     * @param path
     *            the path to be used for the file.
     */
    CARAPI Init(
        /* [in] */ const String& path);

    /**
     * Constructs a new File using the specified directory path and file name,
     * placing a path separator between the two.
     *
     * @param dirPath
     *            the path to the directory where the file is stored.
     * @param name
     *            the file's name.
     * @throws NullPointerException
     *             if {@code name} is {@code null}.
     */
    CARAPI Init(
        /* [in] */ const String& dirPath,
        /* [in] */ const String& name);

    /**
     * Constructs a new File using the path of the specified URI. {@code uri}
     * needs to be an absolute and hierarchical Unified Resource Identifier with
     * file scheme and non-empty path component, but with undefined authority,
     * query or fragment components.
     *
     * @param uri
     *            the Unified Resource Identifier that is used to construct this
     *            file.
     * @throws IllegalArgumentException
     *             if {@code uri} does not comply with the conditions above.
     * @see #toURI
     * @see java.net.URI
     */
//    CARAPI Init(
//        /* [in] */ IURI* uri);

    /**
     * Lists the file system roots. The Java platform may support zero or more
     * file systems, each with its own platform-dependent root. Further, the
     * canonical pathname of any file on the system will always begin with one
     * of the returned file system roots.
     *
     * @return the array of file system roots.
     */
    static CARAPI ListRoots(
        /* [out] */ IObjectContainer** file);

    /**
     * Tests whether or not this process is allowed to execute this file.
     * Note that this is a best-effort result; the only way to be certain is
     * to actually attempt the operation.
     *
     * @return {@code true} if this file can be executed, {@code false} otherwise.
     * @throws SecurityException
     *             If a security manager exists and
     *             SecurityManager.checkExec(java.lang.String) disallows read
     *             permission to this file object
     * @see java.lang.SecurityManager#checkExec(String)
     *
     * @since 1.6
     */
    virtual CARAPI CanExecute(
        /* [out] */ Boolean* canExecute);

    /**
     * Indicates whether the current context is allowed to read from this file.
     *
     * @return {@code true} if this file can be read, {@code false} otherwise.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             read request.
     */
    virtual CARAPI CanRead(
        /* [out] */ Boolean* canRead);

    /**
     * Indicates whether the current context is allowed to write to this file.
     *
     * @return {@code true} if this file can be written, {@code false}
     *         otherwise.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             write request.
     */
    virtual CARAPI CanWrite(
    /* [out] */ Boolean* canWrite);

    /**
     * Returns the relative sort ordering of the paths for this file and the
     * file {@code another}. The ordering is platform dependent.
     *
     * @param another
     *            a file to compare this file to
     * @return an int determined by comparing the two paths. Possible values are
     *         described in the Comparable interface.
     * @see Comparable
     */
    virtual CARAPI CompareTo(
        /* [in] */ IFile* another,
        /* [out] */ Int32* result);

    /**
     * Deletes this file. Directories must be empty before they will be deleted.
     *
     * <p>Note that this method does <i>not</i> throw {@code IOException} on failure.
     * Callers must check the return value.
     *
     * @return {@code true} if this file was deleted, {@code false} otherwise.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies the
     *             request.
     * @see java.lang.SecurityManager#checkDelete
     */
    virtual CARAPI Delete(
        /* [out] */ Boolean* succeeded);

    /**
     * Compares {@code obj} to this file and returns {@code true} if they
     * represent the <em>same</em> object using a path specific comparison.
     *
     * @param obj
     *            the object to compare this file with.
     * @return {@code true} if {@code obj} is the same as this object,
     *         {@code false} otherwise.
     */
    virtual CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    /**
     * Returns a boolean indicating whether this file can be found on the
     * underlying file system.
     *
     * @return {@code true} if this file exists, {@code false} otherwise.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     * @see #getPath
     * @see java.lang.SecurityManager#checkRead(FileDescriptor)
     */
     virtual CARAPI Exists(
        /* [out] */ Boolean* isExist);

    /**
     * Returns the absolute path of this file.
     *
     * @return the absolute file path.
     */
    virtual CARAPI GetAbsolutePath(
        /* [out] */ String* path);

    /**
     * Returns a new file constructed using the absolute path of this file.
     *
     * @return a new file from this file's absolute path.
     * @see java.lang.SecurityManager#checkPropertyAccess
     */
    virtual CARAPI GetAbsoluteFile(
        /* [out] */ IFile** file);

    /**
     * Returns the absolute path of this file with all references resolved. An
     * <em>absolute</em> path is one that begins at the root of the file
     * system. The canonical path is one in which all references have been
     * resolved. For the cases of '..' and '.', where the file system supports
     * parent and working directory respectively, these are removed and replaced
     * with a direct directory reference. If the file does not exist,
     * getCanonicalPath() may not resolve any references and simply returns an
     * absolute path name or throws an IOException.
     *
     * @return the canonical path of this file.
     * @throws IOException
     *             if an I/O error occurs.
     */
    virtual CARAPI GetCanonicalPath(
        /* [out] */ String* path);

    /**
     * Returns a new file created using the canonical path of this file.
     * Equivalent to {@code new File(this.getCanonicalPath())}.
     *
     * @return the new file constructed from this file's canonical path.
     * @throws IOException
     *             if an I/O error occurs.
     * @see java.lang.SecurityManager#checkPropertyAccess
     */
    virtual CARAPI GetCanonicalFile(
        /* [out] */ IFile** file);

    /**
     * Returns the name of the file or directory represented by this file.
     *
     * @return this file's name or an empty string if there is no name part in
     *         the file's path.
     */
    virtual CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Returns the pathname of the parent of this file. This is the path up to
     * but not including the last name. {@code null} is returned if there is no
     * parent.
     *
     * @return this file's parent pathname or {@code null}.
     */
    virtual CARAPI GetParent(
        /* [out] */ String* parent);

    /**
     * Returns a new file made from the pathname of the parent of this file.
     * This is the path up to but not including the last name. {@code null} is
     * returned when there is no parent.
     *
     * @return a new file representing this file's parent or {@code null}.
     */
    virtual CARAPI GetParentFile(
        /* [out] */ IFile ** file);

    /**
     * Returns the path of this file.
     *
     * @return this file's path.
     */
    virtual CARAPI GetPath(
        /* [out] */ String* path);

    /**
     * Returns an integer hash code for the receiver. Any two objects for which
     * {@code equals} returns {@code true} must return the same hash code.
     *
     * @return this files's hash value.
     * @see #equals
     */
    virtual CARAPI GetHashCode(
        /* [out] */ Int32* code);

    /**
     * Indicates if this file's pathname is absolute. Whether a pathname is
     * absolute is platform specific. On Android, absolute paths start with
     * the character '/'.
     *
     * @return {@code true} if this file's pathname is absolute, {@code false}
     *         otherwise.
     * @see #getPath
     */
    virtual CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    /**
     * Indicates if this file represents a <em>directory</em> on the
     * underlying file system.
     *
     * @return {@code true} if this file is a directory, {@code false}
     *         otherwise.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     */
    virtual CARAPI IsDirectory(
        /* [out] */ Boolean* isDirectory);

    /**
     * Indicates if this file represents a <em>file</em> on the underlying
     * file system.
     *
     * @return {@code true} if this file is a file, {@code false} otherwise.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     */
    virtual CARAPI IsFile(
        /* [out] */ Boolean* isFile);

    /**
     * Returns whether or not this file is a hidden file as defined by the
     * operating system. The notion of "hidden" is system-dependent. For Unix
     * systems a file is considered hidden if its name starts with a ".". For
     * Windows systems there is an explicit flag in the file system for this
     * purpose.
     *
     * @return {@code true} if the file is hidden, {@code false} otherwise.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     */
    virtual CARAPI IsHidden(
        /* [out] */ Boolean* isHidden);

    /**
     * Returns the time when this file was last modified, measured in
     * milliseconds since January 1st, 1970, midnight.
     * Returns 0 if the file does not exist.
     *
     * @return the time when this file was last modified.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     */
    virtual CARAPI LastModified(
        /* [out] */ Int64* time);

    /**
     * Sets the time this file was last modified, measured in milliseconds since
     * January 1st, 1970, midnight.
     *
     * <p>Note that this method does <i>not</i> throw {@code IOException} on failure.
     * Callers must check the return value.
     *
     * @param time
     *            the last modification time for this file.
     * @return {@code true} if the operation is successful, {@code false}
     *         otherwise.
     * @throws IllegalArgumentException
     *             if {@code time < 0}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies write
     *             access to this file.
     */
    virtual CARAPI SetLastModified(
        /* [in] */ Int64 time,
        /* [out] */ Boolean* succeeded);

    /**
     * Equivalent to setWritable(false, false).
     *
     * @see #setWritable(boolean, boolean)
     */
    virtual CARAPI SetReadOnly(
        /* [out] */ Boolean* succeeded);

    /**
     * Manipulates the execute permissions for the abstract path designated by
     * this file.
     *
     * <p>Note that this method does <i>not</i> throw {@code IOException} on failure.
     * Callers must check the return value.
     *
     * @param executable
     *            To allow execute permission if true, otherwise disallow
     * @param ownerOnly
     *            To manipulate execute permission only for owner if true,
     *            otherwise for everyone. The manipulation will apply to
     *            everyone regardless of this value if the underlying system
     *            does not distinguish owner and other users.
     * @return true if and only if the operation succeeded. If the user does not
     *         have permission to change the access permissions of this abstract
     *         pathname the operation will fail. If the underlying file system
     *         does not support execute permission and the value of executable
     *         is false, this operation will fail.
     * @throws SecurityException -
     *             If a security manager exists and
     *             SecurityManager.checkWrite(java.lang.String) disallows write
     *             permission to this file object
     * @since 1.6
     */
    virtual CARAPI SetExecutable(
        /* [in] */ Boolean executable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    /**
     * Equivalent to setExecutable(executable, true).
     * @see #setExecutable(boolean, boolean)
     * @since 1.6
     */
    virtual CARAPI SetExecutableEx(
        /* [in] */ Boolean executable,
        /* [out] */ Boolean* succeeded);

    /**
     * Manipulates the read permissions for the abstract path designated by this
     * file.
     *
     * @param readable
     *            To allow read permission if true, otherwise disallow
     * @param ownerOnly
     *            To manipulate read permission only for owner if true,
     *            otherwise for everyone. The manipulation will apply to
     *            everyone regardless of this value if the underlying system
     *            does not distinguish owner and other users.
     * @return true if and only if the operation succeeded. If the user does not
     *         have permission to change the access permissions of this abstract
     *         pathname the operation will fail. If the underlying file system
     *         does not support read permission and the value of readable is
     *         false, this operation will fail.
     * @throws SecurityException -
     *             If a security manager exists and
     *             SecurityManager.checkWrite(java.lang.String) disallows write
     *             permission to this file object
     * @since 1.6
     */
    virtual CARAPI SetReadable(
        /* [in] */ Boolean readable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    /**
     * Equivalent to setReadable(readable, true).
     * @see #setReadable(boolean, boolean)
     * @since 1.6
     */
    virtual CARAPI SetReadableEx(
        /* [in] */ Boolean readable,
        /* [out] */ Boolean* succeeded);

    /**
     * Manipulates the write permissions for the abstract path designated by this
     * file.
     *
     * @param writable
     *            To allow write permission if true, otherwise disallow
     * @param ownerOnly
     *            To manipulate write permission only for owner if true,
     *            otherwise for everyone. The manipulation will apply to
     *            everyone regardless of this value if the underlying system
     *            does not distinguish owner and other users.
     * @return true if and only if the operation succeeded. If the user does not
     *         have permission to change the access permissions of this abstract
     *         pathname the operation will fail.
     * @throws SecurityException -
     *             If a security manager exists and
     *             SecurityManager.checkWrite(java.lang.String) disallows write
     *             permission to this file object
     * @since 1.6
     */
    virtual CARAPI SetWritable(
        /* [in] */ Boolean writable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    /**
     * Equivalent to setWritable(writable, true).
     * @see #setWritable(boolean, boolean)
     * @since 1.6
     */
    virtual CARAPI SetWritableEx(
        /* [in] */ Boolean writable,
        /* [out] */ Boolean* succeeded);

    /**
     * Returns the length of this file in bytes.
     * Returns 0 if the file does not exist.
     * The result for a directory is not defined.
     *
     * @return the number of bytes in this file.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     */
    virtual CARAPI GetLength(
        /* [out] */ Int64* length);

    /**
     * Returns an array of strings with the file names in the directory
     * represented by this file. The result is {@code null} if this file is not
     * a directory.
     * <p>
     * The entries {@code .} and {@code ..} representing the current and parent
     * directory are not returned as part of the list.
     *
     * @return an array of strings with file names or {@code null}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     * @see #isDirectory
     * @see java.lang.SecurityManager#checkRead(FileDescriptor)
     */
    virtual CARAPI List(
        /* [out, callee] */ ArrayOf<String>** files);

    /**
     * Gets a list of the files in the directory represented by this file. This
     * list is then filtered through a FilenameFilter and the names of files
     * with matching names are returned as an array of strings. Returns
     * {@code null} if this file is not a directory. If {@code filter} is
     * {@code null} then all filenames match.
     * <p>
     * The entries {@code .} and {@code ..} representing the current and parent
     * directories are not returned as part of the list.
     *
     * @param filter
     *            the filter to match names against, may be {@code null}.
     * @return an array of files or {@code null}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     * @see #getPath
     * @see #isDirectory
     * @see java.lang.SecurityManager#checkRead(FileDescriptor)
     */
    virtual CARAPI ListWithFilenameFilter(
        /* [in] */ IFilenameFilter* filter,
        /* [out, callee] */ ArrayOf<String>** files);

    /**
     * Returns an array of files contained in the directory represented by this
     * file. The result is {@code null} if this file is not a directory. The
     * paths of the files in the array are absolute if the path of this file is
     * absolute, they are relative otherwise.
     *
     * @return an array of files or {@code null}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     * @see #list
     * @see #isDirectory
     */
    virtual CARAPI ListFiles(
        /* [out] */ IObjectContainer** files);

    /**
     * Gets a list of the files in the directory represented by this file. This
     * list is then filtered through a FilenameFilter and files with matching
     * names are returned as an array of files. Returns {@code null} if this
     * file is not a directory. If {@code filter} is {@code null} then all
     * filenames match.
     * <p>
     * The entries {@code .} and {@code ..} representing the current and parent
     * directories are not returned as part of the list.
     *
     * @param filter
     *            the filter to match names against, may be {@code null}.
     * @return an array of files or {@code null}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     * @see #list(FilenameFilter filter)
     * @see #getPath
     * @see #isDirectory
     * @see java.lang.SecurityManager#checkRead(FileDescriptor)
     */
    virtual CARAPI ListFilesWithFilenameFilter(
        /* [in] */ IFilenameFilter* filter,
        /* [out] */ IObjectContainer** files);

    /**
     * Gets a list of the files in the directory represented by this file. This
     * list is then filtered through a FileFilter and matching files are
     * returned as an array of files. Returns {@code null} if this file is not a
     * directory. If {@code filter} is {@code null} then all files match.
     * <p>
     * The entries {@code .} and {@code ..} representing the current and parent
     * directories are not returned as part of the list.
     *
     * @param filter
     *            the filter to match names against, may be {@code null}.
     * @return an array of files or {@code null}.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies read
     *             access to this file.
     * @see #getPath
     * @see #isDirectory
     * @see java.lang.SecurityManager#checkRead(FileDescriptor)
     */
    virtual CARAPI ListFilesWithFileFilter(
        /* [in] */ IFileFilter* filter,
        /* [out] */ IObjectContainer** files);

    /**
     * Creates the directory named by the trailing filename of this file. Does
     * not create the complete path required to create this directory.
     *
     * <p>Note that this method does <i>not</i> throw {@code IOException} on failure.
     * Callers must check the return value.
     *
     * @return {@code true} if the directory has been created, {@code false}
     *         otherwise.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies write
     *             access for this file.
     * @see #mkdirs
     */
    virtual CARAPI Mkdir(
        /* [out] */ Boolean* succeeded);

    /**
     * Creates the directory named by the trailing filename of this file,
     * including the complete directory path required to create this directory.
     *
     * <p>Note that this method does <i>not</i> throw {@code IOException} on failure.
     * Callers must check the return value.
     *
     * @return {@code true} if the necessary directories have been created,
     *         {@code false} if the target directory already exists or one of
     *         the directories can not be created.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies write
     *             access for this file.
     * @see #mkdir
     */
    virtual CARAPI Mkdirs(
        /* [out] */ Boolean* succeeded);

    /**
     * Creates a new, empty file on the file system according to the path
     * information stored in this file.
     *
     * <p>Note that this method does <i>not</i> throw {@code IOException} on failure.
     * Callers must check the return value.
     *
     * @return {@code true} if the file has been created, {@code false} if it
     *         already exists.
     * @throws IOException if it's not possible to create the file.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies write
     *             access for this file.
     */
    virtual CARAPI CreateNewFile(
        /* [out] */ Boolean* succeeded);

    /**
     * Creates an empty temporary file using the given prefix and suffix as part
     * of the file name. If {@code suffix} is null, {@code .tmp} is used. This
     * method is a convenience method that calls
     * {@link #createTempFile(String, String, File)} with the third argument
     * being {@code null}.
     *
     * @param prefix
     *            the prefix to the temp file name.
     * @param suffix
     *            the suffix to the temp file name.
     * @return the temporary file.
     * @throws IOException
     *             if an error occurs when writing the file.
     */
    static CARAPI CreateTempFile(
        /* [in] */ const String& prefix,
        /* [in] */ const String& suffix,
        /* [out] */ IFile** file);

    /**
     * Creates an empty temporary file in the given directory using the given
     * prefix and suffix as part of the file name. If {@code suffix} is null, {@code .tmp} is used.
     *
     * <p>Note that this method does <i>not</i> call {@link #deleteOnExit}.
     *
     * @param prefix
     *            the prefix to the temp file name.
     * @param suffix
     *            the suffix to the temp file name.
     * @param directory
     *            the location to which the temp file is to be written, or
     *            {@code null} for the default location for temporary files,
     *            which is taken from the "java.io.tmpdir" system property. It
     *            may be necessary to set this property to an existing, writable
     *            directory for this method to work properly.
     * @return the temporary file.
     * @throws IllegalArgumentException
     *             if the length of {@code prefix} is less than 3.
     * @throws IOException
     *             if an error occurs when writing the file.
     */
    static CARAPI CreateTempFileEx(
        /* [in] */ const String& prefix,
        /* [in] */ const String& suffix,
        /* [in] */ IFile* directory,
        /* [out] */ IFile** file);

    /**
     * Renames this file to {@code newPath}. This operation is supported for both
     * files and directories.
     *
     * <p>Many failures are possible. Some of the more likely failures include:
     * <ul>
     * <li>Write permission is required on the directories containing both the source and
     * destination paths.
     * <li>Search permission is required for all parents of both paths.
     * <li>Both paths be on the same mount point. On Android, applications are most likely to hit
     * this restriction when attempting to copy between internal storage and an SD card.
     * </ul>
     *
     * <p>Note that this method does <i>not</i> throw {@code IOException} on failure.
     * Callers must check the return value.
     *
     * @param newPath the new path.
     * @return true on success.
     * @throws SecurityException
     *             if a {@code SecurityManager} is installed and it denies write
     *             access for this file or {@code newPath}.
     */
    virtual CARAPI RenameTo(
        /* [in] */ IFile* newPath,
        /* [out] */ Boolean* succeeded);

    /**
     * Returns a string containing a concise, human-readable description of this
     * file.
     *
     * @return a printable representation of this file.
     */
    virtual CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Returns a Uniform Resource Identifier for this file. The URI is system
     * dependent and may not be transferable between different operating / file
     * systems.
     *
     * @return an URI for this file.
     */

//    virtual CARAPI ToURI(
//        /* [out] */ IURI** uri);

    /**
     * Returns a Uniform Resource Locator for this file. The URL is system
     * dependent and may not be transferable between different operating / file
     * systems.
     *
     * @return a URL for this file.
     * @throws java.net.MalformedURLException
     *             if the path cannot be transformed into a URL.
     * @deprecated use {@link #toURI} and {@link java.net.URI#toURL} to get
     * correct escaping of illegal characters.
     */
//    virtual CARAPI ToURL(
//        /* [out] */ IURL** uri);

    /**
     * Returns the total size in bytes of the partition containing this path.
     * Returns 0 if this path does not exist.
     *
     * @since 1.6
     */
    virtual CARAPI GetTotalSpace(
        /* [out] */ Int64* space);

    /**
     * Returns the number of usable free bytes on the partition containing this path.
     * Returns 0 if this path does not exist.
     *
     * <p>Note that this is likely to be an optimistic over-estimate and should not
     * be taken as a guarantee your application can actually write this many bytes.
     * On Android (and other Unix-based systems), this method returns the number of free bytes
     * available to non-root users, regardless of whether you're actually running as root,
     * and regardless of any quota or other restrictions that might apply to the user.
     * (The {@code getFreeSpace} method returns the number of bytes potentially available to root.)
     *
     * @since 1.6
     */
    virtual CARAPI GetUsableSpace(
        /* [out] */ Int64* space);

    /**
     * Returns the number of free bytes on the partition containing this path.
     * Returns 0 if this path does not exist.
     *
     * <p>Note that this is likely to be an optimistic over-estimate and should not
     * be taken as a guarantee your application can actually write this many bytes.
     *
     * @since 1.6
     */
    virtual CARAPI GetFreeSpace(
        /* [out] */ Int64* space);

private:
    CARAPI _Init(
        /* [in] */ const String& dirtyPath);

    // Removes duplicate adjacent slashes and any trailing slash.
    CARAPI_(void) FixSlashes(
        /* [in] */ const String& origPath,
         /* [out] */ String* destPath);

    // Joins two path components, adding a separator only if necessary.
    CARAPI Join(
        /* [in] */ const String& prefix,
        /* [in] */ const String& suffix,
         /* [out] */ String* dest);

//    CARAPI CheckURI(
//        /* [in] */ IURI* uri);

    /*
     * Resolve symbolic links in the parent directories.
     */
    static CARAPI Resolve(
        /* [in] */ String& path,
         /* [out] */ String* destPath);

    /*
     * Resolve a symbolic link. While the path resolves to an existing path,
     * keep resolving. If an absolute link is found, resolve the parent
     * directories if resolveAbsolute is true.
     */
    static CARAPI ResolveLink(
        /* [in] */ String& path,
        /* [in] */ Int32 length,
        /* [in] */ Boolean resolveAbsolute,
         /* [out] */ String* destLink);

    static CARAPI_(void) Readlink(
        /* [in] */ const String& filePath,
         /* [out] */ String* destLink);

    /**
     * Converts a String[] containing filenames to a File[].
     * Note that the filenames must not contain slashes.
     * This method is to remove duplication in the implementation
     * of File.list's overloads.
     */
    CARAPI FilenamesToFiles(
        /* [in] */ ArrayOf<String>* filenames,
        /* [out] */ IObjectContainer** files);

    CARAPI GetAbsoluteName(
         /* [out] */ String* name);

//    CARAPI WriteObject(
//        /* [in] */ IObjectOutputStream* stream);
//
//    CARAPI ReadObject(
//        /* [in] */ IObjectInputStream* stream);

    static CARAPI_(Boolean) DoStat(
        /* [in] */ const String& path,
        /* [in] */ struct stat& sb);

    static CARAPI_(Boolean) DoAccess(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode);

    static CARAPI_(Boolean) DoChmod(
        /* [in] */ const String& path,
        /* [in] */ mode_t mask,
        /* [in] */ Boolean set);

#ifdef _linux
    static CARAPI_(Boolean) DoStatFs(
        /* [in] */ const String& path,
        /* [in] */ struct statfs& sb);
#endif

public:
    static const Char32 separatorChar;
    static const String separator;
    static const Char32 pathSeparatorChar;
    static const String pathSeparator;

private:
    const static Int64 serialVersionUID;
    String mPath;
    String mAbsolutePath;
};

#endif //__FILE_H__