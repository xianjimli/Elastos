
#include "CFile.h"
#include <assert.h>
#include <sys/stat.h>

const Int64 CFile::mSerialVersionUID = 301077366599181567LL;
const Char8 CFile::mSeparatorChar = '/';
const String CFile::mSeparator = "/";
const Char8 CFile::mPathSeparatorChar = ':';
const String CFile::mPathSeparator = ":";

CFile::CFile()
{
}

CFile::~CFile()
{
    String::Free(mPath);
    String::Free(mAbsolutePath);
}

ECode CFile::CanExecute(
    /* [out] */ Boolean* isCanExecute)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::CanRead(
    /* [out] */ Boolean* isCanRead)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::CanWrite(
    /* [out] */ Boolean* isCanWrite)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::CompareTo(
    /* [in] */ IFile* another,
    /* [out] */ Int32* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::Delete(
    /* [out] */ Boolean* isDeleted)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::DeleteOnExit()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::Exists(
    /* [out] */ Boolean* isExist)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetAbsolutePath(
    /* [out] */ String* path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetAbsoluteFile(
    /* [out] */ IFile** file)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetCanonicalPath(
    /* [out] */ String* path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetCanonicalFile(
    /* [out] */ IFile** file)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetName(
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    Int32 separatorIndex = mPath.LastIndexOf(mSeparator);
    *name = (separatorIndex < 0) ? String::Duplicate(mPath)
                                 : String::Duplicate(mPath.Substring(separatorIndex + 1));
    return NOERROR;
}

ECode CFile::GetParent(
    /* [out] */ String* parent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetParentFile(
    /* [out] */ IFile** file)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetPath(
    /* [out] */ String* path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetHashCode(
    /* [out] */ Int32* code)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::IsDirectory(
    /* [out] */ Boolean* isDirectory)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::IsFile(
    /* [out] */ Boolean* isFile)
{
    if (!isFile) {
        return E_INVALID_ARGUMENT;
    }

    if (mPath.IsEmpty()) {
        *isFile = FALSE;
        return NOERROR;
    }
//	    SecurityManager security = System.getSecurityManager();
//	    if (security != null) {
//	        security.checkRead(path);
//	    }
    *isFile = IsFileImpl(mAbsolutePath);
    return NOERROR;
}

Boolean CFile::IsFileImpl(
    /* [in] */ const String& path)
{
    struct stat sb;
    return stat((const char*)path, &sb) == 0 && S_ISREG(sb.st_mode);
}

ECode CFile::IsHidden(
    /* [out] */ Boolean* isHidden)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::LastModified(
    /* [out] */ Int64* time)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::SetLastModified(
    /* [in] */ Int64 time,
    /* [out] */ Boolean* isSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::SetReadOnly(
    /* [out] */ Boolean* isSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::SetExecutable(
    /* [in] */ Boolean executable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* isSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::SetExecutableEx(
    /* [in] */ Boolean executable,
    /* [out] */ Boolean* isSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::SetReadable(
    /* [in] */ Boolean readable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* isSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::SetReadableEx(
    /* [in] */ Boolean readable,
    /* [out] */ Boolean* isSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::SetWritable(
    /* [in] */ Boolean writable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* isSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::SetWritableEx(
    /* [in] */ Boolean writable,
    /* [out] */ Boolean* isSet)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetLength(
    /* [out] */ Int64* length)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::List(
    /* [out, callee] */ ArrayOf<String>** files)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::ListEx(
    /* [in] */ IFilenameFilter* filter,
    /* [out, callee] */ ArrayOf<String>** files)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::ListFiles(
    /* [out] */ IObjectContainer** files)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::ListFilesEx(
    /* [in] */ IFilenameFilter* filter,
    /* [out] */ IObjectContainer** files)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::ListFilesEx2(
    /* [in] */ IFileFilter* filter,
    /* [out] */ IObjectContainer** files)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::Mkdir(
    /* [out] */ Boolean* isCreated)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::Mkdirs(
    /* [out] */ Boolean* isCreated)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::CreateNewFile(
    /* [out] */ Boolean* isCreated)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::RenameTo(
    /* [in] */ IFile* newPath,
    /* [out] */ Boolean* isSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetDescription(
    /* [out] */ String* des)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::ToURI(
    /* [out] */ IURI** uri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::ToURL(
    /* [out] */ IURL** uri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetTotalSpace(
    /* [out] */ Int64* space)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetUsableSpace(
    /* [out] */ Int64* space)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::GetFreeSpace(
    /* [out] */ Int64* space)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::constructor(
    /* [in] */ IFile* dir,
    /* [in] */ String name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::constructor(
    /* [in] */ String path)
{
    Init(path);
    return NOERROR;
}

ECode CFile::constructor(
    /* [in] */ String dirPath,
    /* [in] */ String name)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFile::constructor(
    /* [in] */ IURI* uri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

void CFile::Init(
    /* [in] */ String dirtyPath)
{
    // Cache the path and the absolute path.
    // We can't call isAbsolute() here (http://b/2486943).
    String cleanPath;
    FixSlashes(dirtyPath, &cleanPath);
    Boolean isAbsolute = cleanPath.GetLength() > 0 && cleanPath[0] == mSeparatorChar;
    if (isAbsolute) {
        mAbsolutePath = String::Duplicate(cleanPath);
        mPath = String::Duplicate(cleanPath);
    } else {
//	        String userDir = AccessController.doPrivileged(new PriviAction<String>("user.dir"));
//	        this.absolutePath = cleanPath.isEmpty() ? userDir : join(userDir, cleanPath);
//	        // We want path to be equal to cleanPath, but we'd like to reuse absolutePath's char[].
//	        this.path = absolutePath.substring(absolutePath.length() - cleanPath.length());
        assert(FALSE && "not implemented");
    }
}

// Removes duplicate adjacent slashes and any trailing slash.
void CFile::FixSlashes(
    /* [in] */ String origPath,
    /* [out] */ String* newPath)
{
    // Remove duplicate adjacent slashes.
    Boolean lastWasSlash = FALSE;
    *newPath = String::Duplicate(origPath);
    char* newPathStr = (char*)(const char*)*newPath;
    assert(!newPath->IsNull());
    Int32 length = newPath->GetLength();
    Int32 newLength = 0;
    for (Int32 i = 0; i < length; ++i) {
        Char8 ch = origPath[i];
        if (ch == '/') {
            if (!lastWasSlash) {
                newPathStr[newLength++] = mSeparatorChar;
                lastWasSlash = TRUE;
            }
        } else {
            newPathStr[newLength++] = ch;
            lastWasSlash = FALSE;
        }
    }
    // Remove any trailing slash (unless this is the root of the file system).
    if (lastWasSlash && newLength > 1) {
        newLength--;
    }
    newPathStr[newLength] = '\0';
}

