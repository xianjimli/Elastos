#include "cmdef.h"
#include "CFile.h"

PInterface CFile::Probe(
    /* [in] */ REIID riid)
{
    return _CFile::Probe(riid);
}

ECode CFile::CanExecute(
    /* [out] */ Boolean* canExecute)
{
    VALIDATE_NOT_NULL(canExecute);

    return File::CanExecute(canExecute);
}

ECode CFile::CanRead(
    /* [out] */ Boolean* canRead)
{
    VALIDATE_NOT_NULL(canRead);

    return File::CanRead(canRead);
}

ECode CFile::CanWrite(
    /* [out] */ Boolean* canWrite)
{
    VALIDATE_NOT_NULL(canWrite);

    return File::CanWrite(canWrite);
}

ECode CFile::CompareTo(
    /* [in] */ IFile* another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(another);
    VALIDATE_NOT_NULL(result);

    return File::CompareTo(another, result);
}

ECode CFile::Delete(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::Delete(succeeded);
}

ECode CFile::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(obj);
    VALIDATE_NOT_NULL(isEqual);

    return File::Equals(obj, isEqual);
}

ECode CFile::Exists(
    /* [out] */ Boolean* isExist)
{
    VALIDATE_NOT_NULL(isExist);

    return File::Exists(isExist);
}

ECode CFile::GetAbsolutePath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    return File::GetAbsolutePath(path);
}

ECode CFile::GetAbsoluteFile(
    /* [out] */ IFile** file)
{
    VALIDATE_NOT_NULL(file);

    return File::GetAbsoluteFile(file);
}

ECode CFile::GetCanonicalPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);

    return File::GetCanonicalPath(path);
}

ECode CFile::GetCanonicalFile(
    /* [out] */ IFile** file)
{
    VALIDATE_NOT_NULL(file);

    return File::GetCanonicalFile(file);
}

ECode CFile::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    return File::GetName(name);
}

ECode CFile::GetParent(
    /* [out] */ String* parent)
{
    VALIDATE_NOT_NULL(parent);

    return File::GetParent(parent);
}

ECode CFile::GetParentFile(
    /* [out] */ IFile** file)
{
    VALIDATE_NOT_NULL(file);

    return File::GetParentFile(file);
}

ECode CFile::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);

    return File::GetPath(path);
}

ECode CFile::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);

    return File::GetHashCode(code);
}

ECode CFile::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    VALIDATE_NOT_NULL(isAbsolute);

    return File::IsAbsolute(isAbsolute);
}

ECode CFile::IsDirectory(
    /* [out] */ Boolean* isDirectory)
{
    VALIDATE_NOT_NULL(isDirectory);

    return File::IsDirectory(isDirectory);
}

ECode CFile::IsFile(
    /* [out] */ Boolean* isFile)
{
    VALIDATE_NOT_NULL(isFile);

    return File::IsFile(isFile);
}

ECode CFile::IsHidden(
    /* [out] */ Boolean* isHidden)
{
    VALIDATE_NOT_NULL(isHidden);

    return File::IsHidden(isHidden);
}

ECode CFile::LastModified(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);

    return File::LastModified(time);
}

ECode CFile::SetLastModified(
    /* [in] */ Int64 time,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::SetLastModified(time, succeeded);
}

ECode CFile::SetReadOnly(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::SetReadOnly(succeeded);
}

ECode CFile::SetExecutable(
    /* [in] */ Boolean executable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::SetExecutable(executable, ownerOnly, succeeded);
}

ECode CFile::SetExecutableEx(
    /* [in] */ Boolean executable,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::SetExecutableEx(executable, succeeded);
}

ECode CFile::SetReadable(
    /* [in] */ Boolean readable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::SetReadable(readable, ownerOnly, succeeded);
}

ECode CFile::SetReadableEx(
    /* [in] */ Boolean readable,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::SetReadableEx(readable, succeeded);
}

ECode CFile::SetWritable(
    /* [in] */ Boolean writable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::SetWritable(writable, ownerOnly, succeeded);
}

ECode CFile::SetWritableEx(
    /* [in] */ Boolean writable,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::SetWritableEx(writable, succeeded);
}

ECode CFile::GetLength(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length);

    return File::GetLength(length);
}

ECode CFile::List(
    /* [out, callee] */ ArrayOf<String>** files)
{
    VALIDATE_NOT_NULL(files);

    return File::List(files);
}

ECode CFile::ListWithFilenameFilter(
    /* [in] */ IFilenameFilter* filter,
    /* [out, callee] */ ArrayOf<String>** files)
{
    VALIDATE_NOT_NULL(filter);
    VALIDATE_NOT_NULL(files);

    return File::ListWithFilenameFilter(filter, files);
}

ECode CFile::ListFiles(
    /* [out] */ IObjectContainer** files)
{
    VALIDATE_NOT_NULL(files);

    return File::ListFiles(files);
}

ECode CFile::ListFilesWithFilenameFilter(
    /* [in] */ IFilenameFilter* filter,
    /* [out] */ IObjectContainer** files)
{
    VALIDATE_NOT_NULL(filter);
    VALIDATE_NOT_NULL(files);

    return File::ListFilesWithFilenameFilter(filter, files);
}

ECode CFile::ListFilesWithFileFilter(
    /* [in] */ IFileFilter* filter,
    /* [out] */ IObjectContainer** files)
{
    VALIDATE_NOT_NULL(filter);
    VALIDATE_NOT_NULL(files);

    return File::ListFilesWithFileFilter(filter, files);
}

ECode CFile::Mkdir(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::Mkdir(succeeded);
}

ECode CFile::Mkdirs(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::Mkdirs(succeeded);
}

ECode CFile::CreateNewFile(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return File::CreateNewFile(succeeded);
}

ECode CFile::RenameTo(
    /* [in] */ IFile* newPath,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(newPath);
    VALIDATE_NOT_NULL(succeeded);

    return File::RenameTo(newPath, succeeded);
}

ECode CFile::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return File::ToString(str);
}

//ECode CFile::ToURI(
//    /* [out] */ IURI** uri)
//{
//    VALIDATE_NOT_NULL(uri);
//
//    return File::ToURI(uri);
//}

//ECode CFile::ToURL(
//    /* [out] */ IURL** uri)
//{
//    VALIDATE_NOT_NULL(uri);
//
//    return File::ToURL(uri);
//}

ECode CFile::GetTotalSpace(
    /* [out] */ Int64* space)
{
    VALIDATE_NOT_NULL(space);

    return File::GetTotalSpace(space);
}

ECode CFile::GetUsableSpace(
    /* [out] */ Int64* space)
{
    VALIDATE_NOT_NULL(space);

    return File::GetUsableSpace(space);
}

ECode CFile::GetFreeSpace(
    /* [out] */ Int64* space)
{
    VALIDATE_NOT_NULL(space);

    return File::GetFreeSpace(space);
}

ECode CFile::constructor(
    /* [in] */ IFile* dir,
    /* [in] */ const String& name)
{
    VALIDATE_NOT_NULL(dir);

    return File::Init(dir, name);
}

ECode CFile::constructor(
    /* [in] */ const String& path)
{
    return File::Init(path);
}

ECode CFile::constructor(
    /* [in] */ const String& dirPath,
    /* [in] */ const String& name)
{
    return File::Init(dirPath, name);
}

//ECode CFile::constructor(
//    /* [in] */ IURI* uri)
//{
//    return File::Init(uri);
//}

