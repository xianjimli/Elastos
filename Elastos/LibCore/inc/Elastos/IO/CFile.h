
#ifndef __CFILE_H__
#define __CFILE_H__

#include "_CFile.h"

CarClass(CFile)
{
public:
    CFile();

    ~CFile();

public:
    CARAPI CanExecute(
        /* [out] */ Boolean* canExecute);

    CARAPI CanRead(
        /* [out] */ Boolean* canRead);

    CARAPI CanWrite(
        /* [out] */ Boolean* canWrite);

    CARAPI CompareTo(
        /* [in] */ IFile* another,
        /* [out] */ Int32* result);

    CARAPI Delete(
        /* [out] */ Boolean* isDeleted);

    CARAPI DeleteOnExit();

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI Exists(
        /* [out] */ Boolean* isExist);

    CARAPI GetAbsolutePath(
        /* [out] */ String* path);

    CARAPI GetAbsoluteFile(
        /* [out] */ IFile** file);

    CARAPI GetCanonicalPath(
        /* [out] */ String* path);

    CARAPI GetCanonicalFile(
        /* [out] */ IFile** file);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetParent(
        /* [out] */ String* parent);

    CARAPI GetParentFile(
        /* [out] */ IFile** file);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    CARAPI IsDirectory(
        /* [out] */ Boolean* isDirectory);

    CARAPI IsFile(
        /* [out] */ Boolean* isFile);

    CARAPI IsHidden(
        /* [out] */ Boolean* isHidden);

    CARAPI LastModified(
        /* [out] */ Int64* time);

    CARAPI SetLastModified(
        /* [in] */ Int64 time,
        /* [out] */ Boolean* succeeded);

    CARAPI SetReadOnly(
        /* [out] */ Boolean* succeeded);

    CARAPI SetExecutable(
        /* [in] */ Boolean executable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    CARAPI SetExecutableEx(
        /* [in] */ Boolean executable,
        /* [out] */ Boolean* succeeded);

    CARAPI SetReadable(
        /* [in] */ Boolean readable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    CARAPI SetReadableEx(
        /* [in] */ Boolean readable,
        /* [out] */ Boolean* succeeded);

    CARAPI SetWritable(
        /* [in] */ Boolean writable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    CARAPI SetWritableEx(
        /* [in] */ Boolean writable,
        /* [out] */ Boolean* succeeded);

    CARAPI GetLength(
        /* [out] */ Int64* length);

    CARAPI List(
        /* [out, callee] */ ArrayOf<String>** files);

    CARAPI ListEx(
        /* [in] */ IFilenameFilter* filter,
        /* [out, callee] */ ArrayOf<String>** files);

    CARAPI ListFiles(
        /* [out] */ IObjectContainer** files);

    CARAPI ListFilesEx(
        /* [in] */ IFilenameFilter* filter,
        /* [out] */ IObjectContainer** files);

    CARAPI ListFilesEx2(
        /* [in] */ IFileFilter* filter,
        /* [out] */ IObjectContainer** files);

    CARAPI Mkdir(
        /* [out] */ Boolean* succeeded);

    CARAPI Mkdirs(
        /* [out] */ Boolean* succeeded);

    CARAPI CreateNewFile(
        /* [out] */ Boolean* succeeded);

    CARAPI RenameTo(
        /* [in] */ IFile* newPath,
        /* [out] */ Boolean* succeeded);

    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI ToURI(
        /* [out] */ IURI** uri);

    CARAPI ToURL(
        /* [out] */ IURL** uri);

    CARAPI GetTotalSpace(
        /* [out] */ Int64* space);

    CARAPI GetUsableSpace(
        /* [out] */ Int64* space);

    CARAPI GetFreeSpace(
        /* [out] */ Int64* space);

    CARAPI constructor(
        /* [in] */ IFile* dir,
        /* [in] */ String name);

    CARAPI constructor(
        /* [in] */ String path);

    CARAPI constructor(
        /* [in] */ String dirPath,
        /* [in] */ String name);

    CARAPI constructor(
        /* [in] */ IURI* uri);

private:
    CARAPI_(Boolean) IsFileImpl(
        /* [in] */ const String& path);

    CARAPI_(void) Init(
        /* [in] */ String dirtyPath);

    CARAPI_(void) FixSlashes(
        /* [in] */ String origPath,
        /* [out] */ String* newPath);

private:
    static const Int64 mSerialVersionUID;

public:
    /**
     * The system-dependent character used to separate components in filenames ('/').
     * Use of this (rather than hard-coding '/') helps portability to other operating systems.
     *
     * <p>This field is initialized from the system property "file.separator".
     * Later changes to that property will have no effect on this field or this class.
     */
    static const Char8 mSeparatorChar;

    /**
     * The system-dependent string used to separate components in filenames ('/').
     * See {@link #separatorChar}.
     */
    static const String mSeparator;

    /**
     * The system-dependent character used to separate components in search paths (':').
     * This is used to split such things as the PATH environment variable and classpath
     * system properties into lists of directories to be searched.
     *
     * <p>This field is initialized from the system property "path.separator".
     * Later changes to that property will have no effect on this field or this class.
     */
    static const Char8 mPathSeparatorChar;

    /**
     * The system-dependent string used to separate components in search paths (":").
     * See {@link #pathSeparatorChar}.
     */
    static const String mPathSeparator;

private:
    /**
     * The path we return from getPath. This is almost the path we were
     * given, but without duplicate adjacent slashes and without trailing
     * slashes (except for the special case of the root directory). This
     * path may be the empty string.
     */
    String mPath;

    /**
     * The path we return from getAbsolutePath, and pass down to native code.
     */
    String mAbsolutePath;
};

#endif // __CFILE_H__
