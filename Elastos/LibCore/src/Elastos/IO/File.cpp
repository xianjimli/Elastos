
#include "cmdef.h"
#include "File.h"
#include "CFile.h"
#include <elastos/Character.h>
#include <StringBuffer.h>
#include <elastos/AutoPtr.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>

using namespace Elastos::Core;

struct DirEntry
{
    DirEntry(const char* filename)
    {
        name = filename;
        next = NULL;
    }
    // On Linux, the ext family all limit the length of a directory entry to
    // less than 256 characters.
    String name;
    DirEntry* next;
};

class DirEntries
{
public:
    DirEntries()
        : mSize(0)
        , mHead(NULL)
    {}

    ~DirEntries()
    {
        while (mHead) {
            pop_front();
        }
    }

    bool push_front(const char* name)
    {
        DirEntry* oldHead = mHead;
        mHead = new DirEntry(name);
        if (mHead == NULL) {
            return false;
        }
        mHead->next = oldHead;
        ++mSize;
        return true;
    }

    const char* front() const
    {
        return (const char*)mHead->name;
    }

    void pop_front()
    {
        DirEntry* popped = mHead;
        if (popped != NULL) {
            mHead = popped->next;
            --mSize;
            delete popped;
        }
    }

    size_t size() const
    {
        return mSize;
    }

private:
    size_t mSize;
    DirEntry* mHead;

    // Disallow copy and assignment.
    DirEntries(DirEntries&);
    void operator=(const DirEntries&);
};

// Iterates over the filenames in the given directory.
class ScopedReaddir
{
public:
    ScopedReaddir(const char* path)
    {
        mDirStream = opendir(path);
        mIsBad = (mDirStream == NULL);
    }

    ~ScopedReaddir()
    {
        if (mDirStream != NULL) {
            closedir(mDirStream);
        }
    }

    // Returns the next filename, or NULL.
    const char* next()
    {
#ifdef _linux
        dirent* result = NULL;
        int rc = readdir_r(mDirStream, &mEntry, &result);
        if (rc != 0) {
            mIsBad = true;
            return NULL;
        }
        return (result != NULL) ? result->d_name : NULL;
#else
        return NULL;
#endif
    }

    // Has an error occurred on this stream?
    bool isBad() const
    {
        return mIsBad;
    }

private:
    DIR* mDirStream;
    dirent mEntry;
    bool mIsBad;

    // Disallow copy and assignment.
    ScopedReaddir(const ScopedReaddir&);
    void operator=(const ScopedReaddir&);
};

// Reads the directory referred to by 'pathBytes', adding each directory entry
// to 'entries'.
static Boolean ReadDirectory(
    /* [in] */ const String& path,
    /* [in] */ DirEntries& entries)
{
    if (path.IsNull()) {
        return FALSE;
    }

    ScopedReaddir dir(path);
    if (dir.isBad()) {
        return FALSE;
    }
    const char* filename;
    while ((filename = dir.next()) != NULL) {
        if (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0) {
            if (!entries.push_front(filename)) {
//                jniThrowException(env, "java/lang/OutOfMemoryError", NULL);
                return FALSE;
            }
        }
    }
    return TRUE;
}


const Int64 File::serialVersionUID = 301077366599181567LL;
const Char32 File::separatorChar = '/';
const Char32 File::pathSeparatorChar = ':';
const String File::separator = String("/");
const String File::pathSeparator = String(":");

//static {
//    // The default protection domain grants access to these properties.
//    separatorChar = System.getProperty("file.separator", "/").charAt(0);
//    pathSeparatorChar = System.getProperty("path.separator", ":").charAt(0);
//    separator = String.valueOf(separatorChar);
//    pathSeparator = String.valueOf(pathSeparatorChar);
//}

File::File()
{}

File::~File()
{}

ECode File::Init(
    /* [in] */ IFile* dir,
    /* [in] */ const String& name)
{
    if (dir == NULL) {
        return Init(String(NULL), name);
    }
    else {
        String dirPath;
        dir->GetPath(&dirPath);
        return Init(dirPath, name);
    }
}

ECode File::Init(
    /* [in] */ const String& path)
{
    return _Init(path);
}

ECode File::Init(
    /* [in] */ const String& dirPath,
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (dirPath.IsNullOrEmpty()) {
        return _Init(name);
    }
    else if (name.IsEmpty()) {
        return _Init(dirPath);
    }
    else {
        String dest;
        FAIL_RETURN(Join(dirPath, name, &dest));
        return _Init(dest);
    }
}

ECode File::Init(
    /* [in] */ IURI* uri)
{
    // check pre-conditions
    FAIL_RETURN(CheckURI(uri));
    String path;
//    uri->GetPath(&path);
    return _Init(path);
}

ECode File::_Init(
    /* [in] */ const String& dirtyPath)
{
    // Cache the path and the absolute path.
    // We can't call isAbsolute() here (http://b/2486943).
    String cleanPath;
    FixSlashes(dirtyPath, &cleanPath);
    Boolean isAbsolute = cleanPath.GetLength() > 0 && cleanPath.GetChar(0) == (Int32)separatorChar;
    if (isAbsolute) {
        mPath = cleanPath;
        mAbsolutePath = cleanPath;
    }
    else {
//        String userDir = AccessController.doPrivileged(new PriviAction<String>("user.dir"));
        String userDir;
        if (cleanPath.IsEmpty()) {
            mAbsolutePath = userDir;
        }
        else {
            String temp;
            Join(userDir, cleanPath, &temp);
            mAbsolutePath = temp;
        }
        // We want path to be equal to cleanPath, but we'd like to reuse m_absolutePath's char[].
        mPath = mAbsolutePath.Substring(
                mAbsolutePath.GetLength() - cleanPath.GetLength());
    }
    return NOERROR;
}

void File::FixSlashes(
    /* [in] */ const String& origPath,
    /* [out] */ String* destPath)
{
    assert(destPath != NULL);
    // Remove duplicate adjacent slashes.
    Boolean lastWasSlash = FALSE;

    if (Character::GetByteCount(separatorChar)
        == Character::GetByteCount('/')) {
        Int32 size = origPath.GetLength();
        StringBuf* newPath = StringBuf::Alloc(size);
        newPath->Copy(origPath);

        Int32 newLength = 0;
        for (Int32 i = 0; i < size; ++i) {
            Char8 ch = (*newPath)[i];
            if (ch == '/') {
                if (!lastWasSlash) {
                    (*newPath)[newLength++] = (Char8)separatorChar;
                    lastWasSlash = TRUE;
                }
            }
            else {
                (*newPath)[newLength++] = ch;
                lastWasSlash = FALSE;
            }
        }
        // Remove any trailing slash (unless this is the root of the file system).
        if (lastWasSlash && newLength > 1) {
            newLength--;
        }
        // Reuse the original string if possible.
        if (newLength != size) {
            (*newPath)[newLength] = '\0';
            *destPath = newPath->GetPayload();
        }
        else {
            *destPath = origPath;
        }

        StringBuf::Free(newPath);
    }
    else {
        Int32 size = origPath.GetLength();
        StringBuf* newPath = StringBuf::Alloc(size * 4);

        Int32 newLength = 0;
        for (Int32 i = 0; i < size; ++i) {
            Char8 ch = origPath[i];
            if (ch == '/') {
                if (!lastWasSlash) {
                    Int32 number;
                    ArrayOf<Char8> buf(newPath->GetPayload(),
                            newPath->GetLength());
                    Character::ToChars(separatorChar, buf, newLength, &number);
                    newLength += number;
                    lastWasSlash = TRUE;
                }
            }
            else {
                (*newPath)[newLength++] = ch;
                lastWasSlash = FALSE;
            }
        }
        // Remove any trailing slash (unless this is the root of the file system).
        if (lastWasSlash && newLength > 1) {
            newLength -= Character::GetByteCount(separatorChar);
        }
        // Reuse the original string if possible.
        if (newLength != size) {
            (*newPath)[newLength] = '\0';
            *destPath = newPath->GetPayload();
        }
        else {
            *destPath = origPath;
        }

        StringBuf::Free(newPath);
    }
}

ECode File::Join(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [out] */ String* dest)
{
    assert(dest != NULL);
    Int32 prefixLength = prefix.GetCharCount();
    Boolean haveSlash = (prefixLength > 0 && prefix.GetChar(prefixLength - 1) == (Int32)separatorChar);
    if (!haveSlash) {
        haveSlash = (suffix.GetCharCount() > 0 && suffix.GetChar(0) == (Int32)separatorChar);
    }
    StringBuffer strbuf;
    if (haveSlash) {
        strbuf += prefix;
        strbuf += suffix;
    }
    else {
        strbuf += prefix;
        strbuf += separatorChar;
        strbuf += suffix;
    }
    *dest = (const char*)strbuf;

    return NOERROR;
}

ECode File::CheckURI(
    /* [in] */ IURI* uri)
{
//    if (!uri->IsAbsolute()) {
//        //throw new IllegalArgumentException("URI is not absolute: " + uri);
//        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//    } else if (!uri->getRawSchemeSpecificPart().startsWith("/")) {
//        //throw new IllegalArgumentException("URI is not hierarchical: " + uri);
//        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//    }
//    if (!"file".equals(uri->getScheme())) {
//        //throw new IllegalArgumentException("Expected file scheme in URI: " + uri);
//        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//    }
//    String rawPath = uri->getRawPath();
//    if (rawPath == NULL || rawPath.isEmpty()) {
//        //throw new IllegalArgumentException("Expected non-empty path in URI: " + uri);
//        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//    }
//    if (uri->getRawAuthority() != NULL) {
//        //throw new IllegalArgumentException("Found authority in URI: " + uri);
//        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//    }
//    if (uri->getRawQuery() != NULL) {
//        //throw new IllegalArgumentException("Found query in URI: " + uri);
//        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//    }
//    if (uri->getRawFragment() != NULL) {
//        //throw new IllegalArgumentException("Found fragment in URI: " + uri);
//        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//    }
    return NOERROR;
}

ECode File::ListRoots(
    /* [out] */ IObjectContainer** file)
{
    assert(file != NULL);
    //return new File[] { new File("/") };

    return E_NOT_IMPLEMENTED;
}

ECode File::CanExecute(
    /* [out] */ Boolean* canExecute)
{
    assert(canExecute != NULL);
    if (mPath.IsEmpty()) {
        *canExecute = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkExec(path); // Seems bogus, but this is what the RI does.
//    }
    *canExecute = DoAccess(mAbsolutePath, X_OK);
    return NOERROR;
}

ECode File::CanRead(
    /* [out] */ Boolean* canRead)
{
    assert(canRead != NULL);
    if (mPath.IsEmpty()) {
        *canRead = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(path);
//    }
    *canRead = DoAccess(mAbsolutePath, R_OK);
    return NOERROR;
}

ECode File::CanWrite(
    /* [out] */ Boolean* canWrite)
{
    assert(canWrite != NULL);
    if (mPath.IsEmpty()) {
        *canWrite = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(path);
//    }
    *canWrite = DoAccess(mAbsolutePath, W_OK);
    return NOERROR;
}

ECode File::CompareTo(
    /* [in] */ IFile* another,
    /* [out] */ Int32* result)
{
    assert(result != NULL);
    String anotherPath;
    another->GetPath(&anotherPath);
    //todo: the result of String.Compare() maybe not correct;
    *result = mPath.Compare(anotherPath);
    return NOERROR;
}

ECode File::Delete(
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    if (mPath.IsEmpty()) {
        *succeeded = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkDelete(path);
//    }
    *succeeded = (remove((const char*)mPath) == 0);
    return NOERROR;
}

ECode File::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    assert(isEqual != NULL);
    if (IFile::Probe(obj) == NULL) {
        *isEqual = FALSE;
        return NOERROR;
    }
    String path;
    IFile::Probe(obj)->GetPath(&path);
    *isEqual = mPath.Equals(path);
    return NOERROR;
}

ECode File::Exists(
    /* [out] */ Boolean* isExist)
{
    assert(isExist != NULL);
    if (mPath.IsEmpty()) {
        *isExist = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(path);
//    }
    *isExist = DoAccess(mAbsolutePath, F_OK);
    return NOERROR;
}

ECode File::GetAbsolutePath(
    /* [out] */ String* path)
{
    assert(path != NULL);
    *path = mAbsolutePath;
    return NOERROR;
}

ECode File::GetAbsoluteFile(
    /* [out] */ IFile** file)
{
    assert(file != NULL);
    return CFile::New(mAbsolutePath, file);
}

ECode File::GetCanonicalPath(
    /* [out] */ String* path)
{
    assert(path != NULL);
    // BEGIN android-removed
    //     Caching the canonical path is bogus. Users facing specific
    //     performance problems can perform their own caching, with
    //     eviction strategies that are appropriate for their application.
    //     A VM-wide cache with no mechanism to evict stale elements is a
    //     disservice to applications that need up-to-date data.
    // String canonPath = FileCanonPathCache.get(absPath);
    // if (canonPath != null) {
    //     return canonPath;
    // }
    // END android-removed

    // TODO: rewrite getCanonicalPath, resolve, and resolveLink.

    String result = mAbsolutePath;
    if (separatorChar == '/') {
        // resolve the full path first
        String fullPath;
        ResolveLink(result, result.GetLength(), FALSE, &fullPath);
        // resolve the parent directories
        Resolve(fullPath, &result);
    }
    Int32 numSeparators = 1;
    Int32 count = result.GetCharCount();
    for (Int32 i = 0; i < count; ++i) {
        if (result.GetChar(i) == (Int32)separatorChar) {
            numSeparators++;
        }
    }
    Int32* sepLocations = new Int32[numSeparators];
    Int32 rootLoc = 0;
    if (separatorChar != '/') {
        if (result.GetChar(0) == '\\') {
            rootLoc = (count > 1 && result.GetChar(1) == '\\') ? 1 : 0;
        }
        else {
            rootLoc = 2; // skip drive i.e. c:
        }
    }

    ArrayOf<Char8>* newResult = ArrayOf<Char8>::Alloc(result.GetCharCount() * 4 + 1);
    Int32 newLength = 0, lastSlash = 0, foundDots = 0;
    sepLocations[lastSlash] = rootLoc;
    Int32 N = result.GetCharCount();
    for (Int32 i = 0; i <= N; ++i) {
        if (i < rootLoc) {
            Char32 c = result.GetChar(i);
            Int32 number;
            Character::ToChars(c, *newResult, newLength, &number);
            newLength += number;
        }
        else {
            if (i == N || result.GetChar(i) == (Int32)separatorChar) {
                if (i == N && foundDots == 0) {
                    break;
                }
                if (foundDots == 1) {
                    /* Don't write anything, just reset and continue */
                    foundDots = 0;
                    continue;
                }
                if (foundDots > 1) {
                    /* Go back N levels */
                    lastSlash = lastSlash > (foundDots - 1) ? lastSlash - (foundDots - 1) : 0;
                    newLength = sepLocations[lastSlash] + 1;
                    foundDots = 0;
                    continue;
                }
                sepLocations[++lastSlash] = newLength;
                Int32 number;
                Character::ToChars(separatorChar, *newResult, newLength, &number);
                newLength += number;
                continue;
            }
            if (result.GetChar(i) == '.') {
                foundDots++;
                continue;
            }
            /* Found some dots within text, write them out */
            if (foundDots > 0) {
                for (Int32 j = 0; j < foundDots; j++) {
                    Int32 number;
                    Character::ToChars('.', *newResult, newLength, &number);
                    newLength += number;
                }
            }
            Char32 c = result.GetChar(i);
            Int32 number;
            Character::ToChars(c, *newResult, newLength, &number);
            newLength += number;
            foundDots = 0;
        }
    }
    // remove trailing slash
    if (newLength > (rootLoc + 1)) {
        Char32 c;
        Character::GetCharAt(*newResult, newLength - 1, &c, NULL);
        if (c == separatorChar) {
            newLength--;
        }
    }
    (*newResult)[newLength + 1] = '\0';
    *path = (char*)newResult->GetPayload();
    delete sepLocations;
    ArrayOf<Char8>::Free(newResult);
    return NOERROR;
}

ECode File::Resolve(
    /* [in] */ String& path,
    /* [out] */ String* destpath)
{
    assert(destpath != NULL);
    Int32 last = 1;
    Boolean done;
    for (Int32 i = 1; (UInt32)i <= path.GetCharCount(); i++) {
        String linkPath = path;
        String bytes;
        if ((UInt32)i == path.GetCharCount() || path.GetChar(i) == (Int32)separatorChar) {
            done = (UInt32)i >= path.GetCharCount() - 1;
            // if there is only one segment, do nothing
            if (done && linkPath.GetCharCount() == 1) {
                *destpath = path;
                return NOERROR;
            }
            Boolean inPlace = FALSE;
            if (linkPath.Equals(path)) {
                bytes = path;
                // if there are no symbolic links, truncate the path instead of copying
                if (!done) {
                    inPlace = TRUE;
                    //todo: check memory leak
                    path = path.Substring(0, i);
                }
            }
            else {
                Int32 nextSize = i - last + 1;
                Int32 linkSize = linkPath.GetCharCount();
                if (linkPath.GetChar(linkSize - 1) == (Int32)separatorChar) {
                    linkSize--;
                }
                Int32 offset1, offset2, offset3;
                Character::GetOffsetByChars(linkPath, 0, linkSize, &offset1);
                Character::GetOffsetByChars(path, 0, last - 1, &offset2);
                Character::GetOffsetByChars(path, 0, last - 1 + nextSize, &offset3);
                String tmp = linkPath.Substring(0, offset1);
                //todo: check memory leak
                tmp += path.Substring(offset2, offset3);
                //todo: correct?
                bytes = linkPath;
                // the full path has already been resolved
            }
            if (done) {
                *destpath = path;
                return NOERROR;
            }
            ResolveLink(bytes, inPlace ? i : bytes.GetCharCount(), TRUE, &linkPath);
            if (inPlace) {
                // path[i] = '/';
                if ((UInt32)i + 1 < path.GetCharCount()) {
                    Int32 offset1;
                    Character::GetOffsetByChars(path, 0, i + 1, &offset1);
                    String tmp = path.Substring(0, i);
                    tmp += "/";
                    tmp += path.Substring(offset1);
                    //todo: memory leak
                    path = tmp;
                }
                else {
                    //todo: memory leak
                    path = path.Substring(0, i);
                    path += "/";
                }
            }
            last = i + 1;
        }
    }
//    throw new InternalError();
    return E_RUNTIME_EXCEPTION;
}

ECode File::ResolveLink(
    /* [in] */ String& path,
    /* [in] */ Int32 length,
    /* [in] */ Boolean resolveAbsolute,
    /* [out] */ String* destlink)
{
    assert(destlink != NULL);
    Boolean restart = FALSE;
    do {
        String fragment, target;
        Int32 offset;
        Character::GetOffsetByChars(path, 0, length, &offset);
        //todo: memory leak
        fragment = path.Substring(0, offset);
        Readlink(fragment, &target);
        if (target.Equals(fragment)) {
            break;
        }
        if (target.GetChar(0) == (Int32)separatorChar) {
            // The link target was an absolute path, so we may need to start again.
            restart = resolveAbsolute;
            Int32 offset;
            Character::GetOffsetByChars(path, 0, length, &offset);
            //todo: memory leak
            path = target + path.Substring(offset);
        }
        else {
            Int32 offset = path.LastIndexOf(separatorChar) + 1;
            //todo: memory leak
            path = path.Substring(0, offset) + target;
        }
        length = path.GetCharCount();
    } while (DoAccess(path, F_OK));
    // resolve the parent directories
    if (restart) {
        return Resolve(path, destlink);
    }
    *destlink = path;
    return NOERROR;
}

void File::Readlink(
    /* [in] */ const String& filePath,
    /* [out] */ String* destLink)
{
#ifdef _linux
    if (filePath.IsNull()) {
        *destLink = NULL;
        return;
    }

    // We can't know how big a buffer readlink(2) will need, so we need to
    // loop until it says "that fit".
    size_t bufSize = 512;
    ArrayOf<Char8>* buf = ArrayOf<Char8>::Alloc(bufSize);
    while (TRUE) {
        ssize_t len = readlink(filePath, buf->GetPayload(), buf->GetLength() - 1);
        if (len == -1) {
            // An error occurred.
            *destLink = filePath;
            ArrayOf<Char8>::Free(buf);
            return;
        }
        if (static_cast<size_t>(len) < static_cast<size_t>(buf->GetLength() - 1)) {
            // The buffer was big enough.
            (*buf)[len] = '\0'; // readlink(2) doesn't NUL-terminate.
            *destLink = buf->GetPayload();
            ArrayOf<Char8>::Free(buf);
            return;
        }
        // Try again with a bigger buffer.
        bufSize *= 2;
        ArrayOf<Char8>::Free(buf);
        buf = ArrayOf<Char8>::Alloc(bufSize);
    }
#endif
}

ECode File::GetCanonicalFile(
    /* [out] */ IFile** file)
{
    assert(file != NULL);
    String path;
    GetCanonicalPath(&path);
    return CFile::New(path, file);
}

ECode File::GetName(
    /* [out] */ String* name)
{
    assert(name != NULL);
    Int32 separatorIndex = mPath.LastIndexOf(separator);
    if (separatorIndex < 0) {
        *name = mPath;
    }
    else {
        //todo: memory leak
        *name = mPath.Substring(separatorIndex + 1, mPath.GetLength());
    }
    return NOERROR;
}

ECode File::GetParent(
    /* [out] */ String* parent)
{
    assert(parent != NULL);
    Int32 charCount = mPath.GetCharCount(), firstInPath = 0;
    if (separatorChar == '\\' && charCount > 2 && mPath.GetChar(1) == ':') {
        firstInPath = 2;
    }
    Int32 index = mPath.LastIndexOf(separatorChar);
    if (index == -1 && firstInPath > 0) {
        index = 2;
    }
    if (index == -1 || mPath.GetChar(charCount - 1) == (Int32)separatorChar) {
        *parent = NULL;
        return NOERROR;
    }
    if (mPath.IndexOf(separatorChar) == index
            && mPath.GetChar(firstInPath) == (Int32)separatorChar) {
        Int32 offset;
        Character::GetOffsetByChars(mPath, 0, index + 1, &offset);
        //todo: memory leak
        *parent = mPath.Substring(0, offset);
        return NOERROR;
    }
    Int32 offset;
    Character::GetOffsetByChars(mPath, 0, index, &offset);
    //todo: memory leak
    *parent = mPath.Substring(0, offset);
    return NOERROR;
}

ECode File::GetParentFile(
    /* [out] */ IFile** file)
{
    assert(file != NULL);
    String tempParent;
    GetParent(&tempParent);
    if (tempParent.IsNull()) {
        *file = NULL;
        return NOERROR;
    }
    return CFile::New(tempParent, file);
}

ECode File::GetPath(
    /* [out] */ String* path)
{
    assert(path != NULL);
    *path = mPath;
    return NOERROR;
}

ECode File::GetHashCode(
    /* [out] */ Int32* code)
{
    assert(code != NULL);
////  return getPath().hashCode() ^ 1234321;
    String temp;
    GetPath(&temp);
    *code = temp.GetHashCode();
    return NOERROR;
}

ECode File::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    assert(isAbsolute != NULL);
    *isAbsolute = mPath.GetCharCount() > 0 && mPath.GetChar(0) == (Int32)separatorChar;
    return NOERROR;
}

ECode File::IsDirectory(
    /* [out] */ Boolean* isDirectory)
{
    assert(isDirectory != NULL);
    if (mPath.IsEmpty()) {
        *isDirectory = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(path);
//    }
    struct stat sb;
    *isDirectory = (DoStat(mAbsolutePath, sb) && S_ISDIR(sb.st_mode));
    return NOERROR;
}

ECode File::IsFile(
    /* [out] */ Boolean* isFile)
{
    assert(isFile != NULL);
    if (mPath.IsEmpty()) {
        *isFile = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(path);
//    }
    struct stat sb;
    *isFile = (DoStat(mAbsolutePath, sb) && S_ISREG(sb.st_mode));
    return NOERROR;
}

ECode File::IsHidden(
    /* [out] */ Boolean* isHidden)
{
    assert(isHidden != NULL);
    if (mPath.IsEmpty()) {
        *isHidden = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(path);
//    }
    String name;
    GetName(&name);
    *isHidden = name.StartWith(".");
    return NOERROR;
}

ECode File::LastModified(
    /* [out] */ Int64* time)
{
    assert(time != NULL);
    if (mPath.IsEmpty()) {
        *time = 0;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(path);
//    }
    struct stat sb;
    if (!DoStat(mAbsolutePath, sb)) {
        *time = 0;
        return NOERROR;
    }
    *time = static_cast<Int64>(sb.st_mtime) * 1000LL;
    return NOERROR;
}

ECode File::SetLastModified(
    /* [in] */ Int64 time,
    /* [out] */ Boolean* succeeded)
{
#ifdef _linux
    assert(succeeded != NULL);
    if (mPath.IsEmpty()) {
        *succeeded = FALSE;
        return NOERROR;
    }
    if (time < 0) {
////        throw new IllegalArgumentException("time < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(path);
//    }

    // We want to preserve the access time.
    struct stat sb;
    if (stat((const char*)mAbsolutePath, &sb) == -1) {
        *succeeded = FALSE;
        return NOERROR;
    }

    // TODO: we could get microsecond resolution with utimes(3), "legacy" though it is.
    utimbuf times;
    times.actime = sb.st_atime;
    times.modtime = static_cast<time_t>(time / 1000);
    *succeeded = (utime((const char*)mAbsolutePath, &times) == 0);
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode File::SetReadOnly(
    /* [out] */ Boolean* succeeded)
{
    return SetWritable(FALSE, FALSE, succeeded);
}

ECode File::SetExecutable(
    /* [in] */ Boolean executable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    if (mPath.IsEmpty()) {
        *succeeded = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(path);
//    }
    *succeeded = DoChmod(mAbsolutePath,
            ownerOnly ? S_IXUSR : (S_IXUSR | S_IXGRP | S_IXOTH), executable);
    return NOERROR;
}

ECode File::SetExecutableEx(
    /* [in] */ Boolean executable,
    /* [out] */ Boolean* succeeded)
{
    return SetExecutable(executable, TRUE, succeeded);
}

ECode File::SetReadable(
    /* [in] */ Boolean readable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    if (mPath.IsEmpty()) {
        *succeeded = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(path);
//    }
    *succeeded = DoChmod(mAbsolutePath,
            ownerOnly ? S_IRUSR : (S_IRUSR | S_IRGRP | S_IROTH), readable);
    return NOERROR;
}

ECode File::SetReadableEx(
    /* [in] */ Boolean readable,
    /* [out] */ Boolean* succeeded)
{
    return SetReadable(readable, TRUE, succeeded);
}

ECode File::SetWritable(
    /* [in] */ Boolean writable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    if (mPath.IsEmpty()) {
        *succeeded = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(path);
//    }
    *succeeded = DoChmod(mAbsolutePath,
            ownerOnly ? S_IWUSR : (S_IWUSR | S_IWGRP | S_IWOTH), writable);
    return NOERROR;
}

ECode File::SetWritableEx(
    /* [in] */ Boolean writable,
    /* [out] */ Boolean* succeeded)
{
    return SetWritable(writable, TRUE, succeeded);
}

ECode File::GetLength(
    /* [out] */ Int64* length)
{
    assert(length != NULL);
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(path);
//    }

    struct stat sb;
    if (!DoStat(mAbsolutePath, sb)) {
        // We must return 0 for files that don't exist.
        // TODO: shouldn't we throw an IOException for ELOOP or EACCES?
        *length = 0;
        return NOERROR;
    }

    /*
     * This android-changed code explicitly treats non-regular files (e.g.,
     * sockets and block-special devices) as having size zero. Some synthetic
     * "regular" files may report an arbitrary non-zero size, but
     * in these cases they generally report a block count of zero.
     * So, use a zero block count to trump any other concept of
     * size.
     *
     * TODO: why do we do this?
     */
    if (!S_ISREG(sb.st_mode) || sb.st_blocks == 0) {
        *length = 0;
        return NOERROR;
    }
    *length = sb.st_size;
    return NOERROR;
}

ECode File::List(
    /* [out, callee] */ ArrayOf<String>** files)
{
    assert(files != NULL);
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(path);
//    }
    if (mPath.IsEmpty()) {
        *files = NULL;
        return NOERROR;
    }

    // Read the directory entries into an intermediate form.
    DirEntries _files;
    if (!ReadDirectory(mAbsolutePath, _files)) {
        *files = NULL;
        return NOERROR;
    }

    *files = ArrayOf<String>::Alloc(_files.size());
    for (Int32 i = 0; _files.size() != 0; _files.pop_front(), ++i) {
        String fname(_files.front());
        (**files)[i] = fname;
    }
    return NOERROR;
}

ECode File::ListWithFilenameFilter(
    /* [in] */ IFilenameFilter* filter,
    /* [out, callee] */ ArrayOf<String>** files)
{
    //todo: memory leak
    ArrayOf<String>* _files;
    FAIL_RETURN(List(&_files));
    if (filter == NULL || _files == NULL) {
        *files = _files;
        return NOERROR;
    }
    ArrayOf<Boolean>* result = ArrayOf<Boolean>::Alloc(_files->GetLength());
    Int32 size = 0;
    for (Int32 i = 0; i < _files->GetLength(); i++) {
        String fname = (*_files)[i];
        filter->Accept((IFile*)this->Probe(EIID_IFile), fname, &(*result)[i]);
        if ((*result)[i]) size++;
    }

    *files = ArrayOf<String>::Alloc(size);
    Int32 index = 0;
    for (Int32 i = 0; i < result->GetLength(); i++) {
        if ((*result)[i]) {
            (**files)[index++] = (*_files)[i];
        }
    }
    return NOERROR;
}

ECode File::ListFiles(
    /* [out] */ IObjectContainer** files)
{
    assert(files != NULL);
    ArrayOf<String>* fnames;
    FAIL_RETURN(List(&fnames));

    ECode ec = FilenamesToFiles(fnames, files);
    //todo: memory leak
    ArrayOf<String>::Free(fnames);
    return ec;
}

ECode File::ListFilesWithFilenameFilter(
    /* [in] */ IFilenameFilter* filter,
    /* [out] */ IObjectContainer** files)
{
    assert(files != NULL);
    ArrayOf<String>* fnames;
    FAIL_RETURN(ListWithFilenameFilter(filter, &fnames));

    ECode ec = FilenamesToFiles(fnames, files);
    //todo: memory leak
    ArrayOf<String>::Free(fnames);
    return ec;
}

ECode File::ListFilesWithFileFilter(
    /* [in] */ IFileFilter* filter,
    /* [out] */ IObjectContainer** files)
{
//    File[] files = listFiles();
//    if (filter == null || files == null) {
//        return files;
//    }
//    List<File> result = new ArrayList<File>(files.length);
//    for (File file : files) {
//        if (filter.accept(file)) {
//            result.add(file);
//        }
//    }
//    return result.toArray(new File[result.size()]);
    return E_NOT_IMPLEMENTED;
}

ECode File::FilenamesToFiles(
    /* [in] */ ArrayOf<String>* filenames,
    /* [out] */ IObjectContainer** files)
{
    assert(files != NULL);
    if (filenames == NULL) {
        *files = NULL;
        return NOERROR;
    }
//    CObjectContainer::New(files);
    Int32 count = filenames->GetLength();
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IFile> file;
        CFile::New((IFile*)this->Probe(EIID_IFile),
                (*filenames)[i], (IFile**)&file);
        (*files)->Add(file.Get());
    }
    return NOERROR;
}

ECode File::Mkdir(
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(path);
//    }
    if (mAbsolutePath.IsNull()) {
        *succeeded = FALSE;
        return NOERROR;
    }

    // On Android, we don't want default permissions to allow global access.
    *succeeded = (mkdir((const char*)mAbsolutePath, S_IRWXU) == 0);
    return NOERROR;
}

ECode File::Mkdirs(
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    /* If the terminal directory already exists, answer FALSE */
    Boolean IsExists = FALSE;
    Exists(&IsExists);
    if (IsExists) {
        *succeeded = FALSE;
        return NOERROR;
    }

    /* If the receiver can be created, answer true */
    Mkdir(succeeded);
    if (*succeeded) {
        return NOERROR;
    }

    String parentDir;
    GetParent(&parentDir);
    /* If there is no parent and we were not created, answer false */
    if (parentDir.IsNull()) {
        *succeeded = FALSE;
        return NOERROR;
    }

    /* Otherwise, try to create a parent directory and then this directory */
    AutoPtr<IFile> parent;
    CFile::New(parentDir, (IFile**)&parent);
    Boolean b1, b2;
    parent->Mkdirs(&b1);
    Mkdir(&b2);
    *succeeded = b1 && b2;
    return NOERROR;
}

ECode File::CreateNewFile(
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(path);
//    }
    if (mPath.IsEmpty()) {
//        throw new IOException("No such file or directory");
        return E_IO_EXCEPTION;
    }
    if (mAbsolutePath.IsNull()) {
        *succeeded = FALSE;
        return NOERROR;
    }

    // On Android, we don't want default permissions to allow global access.
    Int32 fd = open((const char*)mAbsolutePath, O_CREAT | O_EXCL, 0600);
    if (fd != -1) {
        // We created a new file. Success!
        *succeeded = TRUE;
        return NOERROR;
    }
    if (errno == EEXIST) {
        // The file already exists.
        *succeeded = FALSE;
        return NOERROR;
    }
//    jniThrowIOException(env, errno);
    return E_IO_EXCEPTION;
}

ECode File::CreateTempFile(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [out] */ IFile* file)
{
    assert(file != NULL);
    return CreateTempFileEx(prefix, suffix, NULL, file);
}

ECode File::CreateTempFileEx(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [in] */ IFile* directory,
    /* [out] */ IFile* file)
{
//    // Force a prefix null check first
//    if (prefix.length() < 3) {
//        throw new IllegalArgumentException("prefix must be at least 3 characters");
//    }
//    if (suffix == null) {
//        suffix = ".tmp";
//    }
//    File tmpDirFile = directory;
//    if (tmpDirFile == null) {
//        String tmpDir = AccessController.doPrivileged(
//            new PriviAction<String>("java.io.tmpdir", "."));
//        tmpDirFile = new File(tmpDir);
//    }
//    File result;
//    do {
//        result = new File(tmpDirFile, prefix + new Random().nextInt() + suffix);
//    } while (!result.createNewFile());
//    return result;
    return E_NOT_IMPLEMENTED;
}

ECode File::RenameTo(
    /* [in] */ IFile* newPath,
    /* [out] */ Boolean* succeeded)
{
    assert(newPath != NULL);
    assert(succeeded != NULL);
    String path;
    newPath->GetPath(&path);
    if (mPath.IsEmpty() || path.IsEmpty()) {
        *succeeded = FALSE;
        return NOERROR;
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkWrite(path);
//        security.checkWrite(newPath.path);
//    }
    String abPath;
    newPath->GetAbsolutePath(&abPath);
    if (mAbsolutePath.IsNull() || abPath.IsNull()) {
        *succeeded = FALSE;
        return NOERROR;
    }

    *succeeded = (rename((const char*)mAbsolutePath, (const char*)abPath) == 0);
    return NOERROR;
}

ECode File::ToString(
    /* [out] */ String* path)
{
    assert(path != NULL);
    *path = mPath;
    return NOERROR;
}

ECode File::ToURI(
    /* [out] */ IURI** uri)
{
    assert(uri != NULL);
//    String name = getAbsoluteName();
//    try {
//        if (!name.startsWith("/")) {
//            // start with sep.
//            return new URI("file", null, new StringBuilder(
//                    name.length() + 1).append('/').append(name).toString(),
//                    null, null);
//        } else if (name.startsWith("//")) {
//            return new URI("file", "", name, null); // UNC path
//        }
//        return new URI("file", null, name, null, null);
//    } catch (URISyntaxException e) {
//        // this should never happen
//        return null;
//    }
    return E_NOT_IMPLEMENTED;
}
//..URI URL not implement
ECode File::ToURL(
    /* [out] */ IURL** uri)
{
    assert(uri != NULL);
//    String name = getAbsoluteName();
//    if (!name.startsWith("/")) {
//        // start with sep.
//        return new URL("file", "", -1,
//                new StringBuilder(name.length() + 1).append('/').append(name).toString(), null);
//    } else if (name.startsWith("//")) {
//        return new URL("file:" + name); // UNC path
//    }
//    return new URL("file", "", -1, name, null);
    return E_NOT_IMPLEMENTED;
}

ECode File::GetAbsoluteName(
    /* [out] */ String* absoluteName)
{
    //todo: memory leak
    assert(absoluteName != NULL);
    AutoPtr<IFile> file;
    GetAbsoluteFile((IFile**)&file);

    String name;
    file->GetPath(&name);
    Boolean isDir = FALSE;
    file->IsDirectory(&isDir);
    if (isDir && name.GetChar(name.GetCharCount() - 1) != (Int32)separatorChar) {
        // Directories must end with a slash
        name += "/";
    }
    if (separatorChar != '/') { // Must convert slashes.
//        name = name.replace(separatorChar, '/');
    }
    *absoluteName = name;
    return NOERROR;
}

//ECode File::WriteObject(
//    /* [in] */ IObjectOutputStream* stream)
//{
//    stream.defaultWriteObject();
//    stream.writeChar(separatorChar);
//}
//
//ECode File::ReadObject(
//    /* [in] */ IObjectInputStream* stream)
//{
//    stream.defaultReadObject();
//    char inSeparator = stream.readChar();
//    init(path.replace(inSeparator, separatorChar));
//}

ECode File::GetTotalSpace(
    /* [out] */ Int64* space)
{
    assert(space != NULL);
#ifdef _linux
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkPermission(new RuntimePermission("getFileSystemAttributes"));
//    }

    struct statfs sb;
    if (!DoStatFs(mAbsolutePath, sb)) {
        *space = 0;
        return NOERROR;
    }
    *space = sb.f_blocks * sb.f_bsize; // total block count * block size in bytes.
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode File::GetUsableSpace(
    /* [out] */ Int64* space)
{
    assert(space != NULL);
#ifdef _linux
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkPermission(new RuntimePermission("getFileSystemAttributes"));
//    }

    struct statfs sb;
    if (!DoStatFs(mAbsolutePath, sb)) {
        *space = 0;
        return NOERROR;
    }
    *space = sb.f_bavail * sb.f_bsize; // non-root free block count * block size in bytes.
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode File::GetFreeSpace(
    /* [out] */ Int64* space)
{
    assert(space != NULL);
#ifdef _linux
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkPermission(new RuntimePermission("getFileSystemAttributes"));
//    }

    struct statfs sb;
    if (!DoStatFs(mAbsolutePath, sb)) {
        *space = 0;
        return NOERROR;
    }
    *space = sb.f_bfree * sb.f_bsize; // free block count * block size in bytes.
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

Boolean File::DoStat(
    /* [in] */ const String& path,
    /* [in] */ struct stat& sb)
{
    if (path.IsNull()) {
        return FALSE;
    }
    return (stat((const char*)path, &sb) == 0);
}

Boolean File::DoAccess(
    /* [in] */ const String& path,
    /* [in] */ Int32 mode)
{
    if (path.IsNull()) {
        return FALSE;
    }
    return (access((const char*)path, mode) == 0);
}

Boolean File::DoChmod(
    /* [in] */ const String& path,
    /* [in] */ mode_t mask,
    /* [in] */ Boolean set)
{
#ifdef _linux
    if (path.IsNull()) {
        return FALSE;
    }

    struct stat sb;
    if (stat((const char*)path, &sb) == -1) {
        return FALSE;
    }
    mode_t newMode = set ? (sb.st_mode | mask) : (sb.st_mode & ~mask);
    return (chmod((const char*)path, newMode) == 0);
#else
    return FALSE;
#endif
}

#ifdef _linux
Boolean File::DoStatFs(
    /* [in] */ const String& path,
    /* [in] */ struct statfs& sb)
{
    if (path.IsNull()) {
        return FALSE;
    }

    Int32 rc = statfs((const char*)path, &sb);
    return (rc != -1);
}
#endif
