#include "ZipFile.h"
#include <stdio.h>
#include <Elastos.IO.h>
#include <Elastos.Utility.h>
#include "CZipEntry.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

extern "C" const InterfaceID EIID_ZipFileRAFStream =
    { 0x83f2304, 0xfc73, 0x4727, { 0x86, 0x6a, 0x81, 0xd5, 0xeb, 0x82, 0xf3, 0x43 } };

ZipFile::RAFStream::RAFStream(
    /* [in] */ IRandomAccessFile* raf,
    /* [in] */ Int64 pos)
    : mSharedRaf(raf)
    , mOffset(pos)
{
    raf->GetLength(&mLength);
}

PInterface ZipFile::RAFStream::Probe(
/* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IInputStream) {
        return (IInputStream*)this;
    }
    return NULL;
}

UInt32 ZipFile::RAFStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ZipFile::RAFStream::Release()
{
    return ElRefBase::Release();
}

ECode ZipFile::RAFStream::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ZipFile::RAFStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = (mOffset < mLength ? 1 : 0);
    return NOERROR;
}

ECode ZipFile::RAFStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    ArrayOf_<Byte, 1> singleByteBuf;
    Int32 count;
    FAIL_RETURN(ReadBufferEx(0, 1, &singleByteBuf, &count));
    if (count == 1) {
        *value = singleByteBuf[0] & 0XFF;
        return NOERROR;
    }
    else {
        *value = -1;
        return NOERROR;
    }
}

ECode ZipFile::RAFStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    Mutex::Autolock lock(mSharedRafLock);

    ECode ec = NOERROR;
    mSharedRaf->Seek(mOffset);
    if (length > mLength - mOffset) {
        length = (int) (mLength - mOffset);
    }
    ec = mSharedRaf->ReadBufferEx(offset, length, buffer, number);
    if (*number > 0) {
        mOffset += *number;
    }
    else *number = -1;

    return ec;
}

ECode ZipFile::RAFStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    if (count > mLength - mOffset) {
        count = mLength - mOffset;
    }
    mOffset += count;
    *number = count;
    return NOERROR;
}

ECode ZipFile::RAFStream::Close()
{
    return NOERROR;
}

ECode ZipFile::RAFStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode ZipFile::RAFStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    *supported = FALSE;
    return NOERROR;
}

ECode ZipFile::RAFStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

ECode ZipFile::RAFStream::Reset()
{
    return E_IO_EXCEPTION;
}


ZipFile::ZipInflaterInputStream::ZipInflaterInputStream(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inf,
    /* [in] */ Int32 bsize,
    /* [in] */ CZipEntry* entry)
    : mEntry(entry)
    , mBytesRead(0)
{
    ASSERT_SUCCEEDED(InflaterInputStream::Init(is, inf, bsize));
}

PInterface ZipFile::ZipInflaterInputStream::Probe(
/* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IInflaterInputStream) {
        return (IInflaterInputStream*)this;
    }
    else if (riid == EIID_IInputStream) {
        return (IInputStream *)this;
    }
    return NULL;
}

UInt32 ZipFile::ZipInflaterInputStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ZipFile::ZipInflaterInputStream::Release()
{
    return ElRefBase::Release();
}

ECode ZipFile::ZipInflaterInputStream::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

//@Override
ECode ZipFile::ZipInflaterInputStream::ReadBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    FAIL_RETURN(InflaterInputStream::ReadBufferEx(off, nbytes, buffer, number));
    if (*number != -1) {
        mBytesRead += *number;
    }
    return NOERROR;
}

//@Override
ECode ZipFile::ZipInflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    if (mClosed) {
        // Our superclass will throw an exception, but there's a jtreg test that
        // explicitly checks that the InputStream returned from ZipFile.getInputStream
        // returns 0 even when closed.
        *number = 0;
        return NOERROR;
    }
    InflaterInputStream::Available(number);
    if (*number != 0) {
        Int64 size;
        mEntry->GetSize(&size);
        *number = size - mBytesRead;
    }
    return NOERROR;
}

// ------------------------gaojianfeng add 2012-11-29------------------
ECode ZipFile::ZipInflaterInputStream::Close()
{
    return InflaterInputStream::Close();
}

ECode ZipFile::ZipInflaterInputStream::Mark(Int32 mark)
{
    return InflaterInputStream::Mark(mark);
}

ECode ZipFile::ZipInflaterInputStream::IsMarkSupported(Boolean *isMark)
{
    return InflaterInputStream::IsMarkSupported(isMark);
}

ECode ZipFile::ZipInflaterInputStream::Reset()
{
    return InflaterInputStream::Reset();
}

ECode ZipFile::ZipInflaterInputStream::Read(Int32 *value)
{
    VALIDATE_NOT_NULL(value);
    return InflaterInputStream::Read(value);
}

ECode ZipFile::ZipInflaterInputStream::ReadBuffer(ArrayOf<Byte> *buffer, Int32 *number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

ECode ZipFile::ZipInflaterInputStream::Skip(Int64 offset, Int64*number)
{
    VALIDATE_NOT_NULL(number);
    return InflaterInputStream::Skip(offset, number);
}

ZipFile::ZipFile()
{
    mLer = new CZipEntry::LittleEndianReader;
}

ZipFile::~ZipFile()
{
    Close();
    delete mLer;
}

/**
 * Closes this ZIP file. This method is idempotent.
 *
 * @throws IOException
 *             if an IOException occurs.
 */
ECode ZipFile::Close()
{
  //  IRandomAccessFile raf = mRaf;

    if (mRaf != NULL) { // Only close initialized instances
//        synchronized(raf) {
            //mRaf = null;
            mRaf->Close();
            mRaf = NULL;
//        }
//        if (fileToDeleteOnClose != null) {
//            AccessController.doPrivileged(new PrivilegedAction<Object>() {
//                public Object run() {
//                    new File(fileName).delete();
//                    return null;
//                }
//            });
//            // fileToDeleteOnClose.delete();
//            fileToDeleteOnClose = null;
//        }
//    }
    }
    return NOERROR;
}

ECode ZipFile::CheckNotClosed()
{
   if (mRaf == NULL) {
       // throw new IllegalStateException("Zip file closed");
       return E_ILLEGAL_STATE_EXCEPTION;
   }
   return NOERROR;
}

/**
 * Returns an enumeration of the entries. The entries are listed in the
 * order in which they appear in the ZIP archive.
 *
 * @return the enumeration of the entries.
 * @throws IllegalStateException if this ZIP file has been closed.
 */
ECode ZipFile::GetEntries(
    /* [out] */ IObjectContainer** entries)
{
    FAIL_RETURN(CheckNotClosed());

    FAIL_RETURN(CObjectContainer::New(entries));

    HashMap<String, AutoPtr<IZipEntry> >::Iterator it = mEntries.Begin();
    for (; it != mEntries.End(); ++it) {
        AutoPtr<IZipEntry> p = it->mSecond;
        (*entries)->Add(p);
    }
    return NOERROR;
}

/**
 * Gets the ZIP entry with the specified name from this {@code ZipFile}.
 *
 * @param entryName
 *            the name of the entry in the ZIP file.
 * @return a {@code ZipEntry} or {@code null} if the entry name does not
 *         exist in the ZIP file.
 * @throws IllegalStateException if this ZIP file has been closed.
 */
ECode ZipFile::GetEntry(
    /* [in] */ const String& entryName,
    /* [out] */ IZipEntry** entry)
{
    if (entryName.IsNull()) {
         return E_NULL_POINTER_EXCEPTION;
    }

    FAIL_RETURN(CheckNotClosed());

    *entry = NULL;
    HashMap<String, AutoPtr<IZipEntry> >::Iterator it = mEntries.Find(entryName);
    if (it == mEntries.End()) {
        it = mEntries.Find(entryName + "/");
    }
    if (it != mEntries.End()) {
        *entry = it->mSecond;
        if (*entry != NULL) (*entry)->AddRef();
    }
    return NOERROR;
}

/**
 * Returns an input stream on the data of the specified {@code ZipEntry}.
 *
 * @param entry
 *            the ZipEntry.
 * @return an input stream of the data contained in the {@code ZipEntry}.
 * @throws IOException
 *             if an {@code IOException} occurs.
 * @throws IllegalStateException if this ZIP file has been closed.
 */
ECode ZipFile::GetInputStream(
    /* [in] */ IZipEntry* entry,
    /* [out] */ IInputStream** is)
{
//    /*
//     * Make sure this ZipEntry is in this Zip file.  We run it through
//     * the name lookup.
//     */
    IZipEntry *pEntry = NULL;
    String name;
    entry->GetName(&name);
    GetEntry(name, &pEntry);
    if (entry == NULL) {
        *is = NULL;
        return NOERROR;
    }

//    /*
//     * Create a ZipInputStream at the right part of the file.
//     */
    IRandomAccessFile *raf = mRaf;
//    synchronized (raf) {
//        // We don't know the entry data's start position. All we have is the
//        // position of the entry's local header. At position 28 we find the
//        // length of the extra data. In some cases this length differs from
//        // the one coming in the central header.
        AutoPtr<RAFStream> rafstrm = new RAFStream(raf, ((CZipEntry*)entry)->mLocalHeaderRelOffset + 28);
        Int32 localExtraLenOrWhatever;
        mLer->ReadInt16LE((IInputStream *) rafstrm->Probe(EIID_IInputStream), &localExtraLenOrWhatever);
        //mLer->ReadShortLE(rafstrm);
//        // Skip the name and this "extra" data or whatever it is:
        Int64 number;
        rafstrm->Skip(((CZipEntry*)entry)->mNameLen + localExtraLenOrWhatever, &number);
        Int64 compressedSize;
        entry->GetCompressedSize(&compressedSize);
        Int32 compressionMethod;
        entry->GetMethod(&compressionMethod);
        rafstrm->mLength = rafstrm->mOffset + compressedSize;
        if (compressionMethod == IDeflater_DEFLATED) {
            Int64 size;
            entry->GetSize(&size);
            Int32 bufSize = Math::Max(1024, Math::Min((Int32)size, 65535L));
            AutoPtr<IInflater> inf;
            ECode ec = CInflater::New(TRUE,(IInflater**)&inf);
            if (FAILED(ec)) return ec;
            AutoPtr<ZipInflaterInputStream> pInflaterStream = new ZipInflaterInputStream(rafstrm, (IInflater*)inf, bufSize, (CZipEntry *)entry);
            *is = (IInputStream *)pInflaterStream->Probe(EIID_IInputStream);
            (*is)->AddRef();
        } else {
            *is = (IInputStream *) rafstrm->Probe(EIID_IInputStream);
            (*is)->AddRef();
        }
//    }
    return NOERROR;
}

/**
 * Gets the file name of this {@code ZipFile}.
 *
 * @return the file name of this {@code ZipFile}.
 */
ECode ZipFile::GetName(
    /* [out] */ String* name)
{
    *name = mFileName;
    return NOERROR;
}

/**
 * Returns the number of {@code ZipEntries} in this {@code ZipFile}.
 *
 * @return the number of entries in this file.
 * @throws IllegalStateException if this ZIP file has been closed.
 */
ECode ZipFile::GetSize(
    /* [out] */ Int32* size)
{
    FAIL_RETURN(CheckNotClosed());
    *size = mEntries.GetSize();
    return NOERROR;
}

/**
 * Find the central directory and read the contents.
 *
 * <p>The central directory can be followed by a variable-length comment
 * field, so we have to scan through it backwards.  The comment is at
 * most 64K, plus we have 18 bytes for the end-of-central-dir stuff
 * itself, plus apparently sometimes people throw random junk on the end
 * just for the fun of it.
 *
 * <p>This is all a little wobbly.  If the wrong value ends up in the EOCD
 * area, we're hosed. This appears to be the way that everybody handles
 * it though, so we're in good company if this fails.
 */
ECode ZipFile::ReadCentralDir()
{
//    /*
//     * Scan back, looking for the End Of Central Directory field.  If
//     * the archive doesn't have a comment, we'll hit it on the first
//     * try.
//     *
//     * No need to synchronize mRaf here -- we only do this when we
//     * first open the Zip file.
//     */
      Int64 len = 0;
      ECode ec = NOERROR;
      ec = mRaf->GetLength(&len);
      if (FAILED(ec)) {
          return ec;
      }

      Int64 scanOffset = len - ZipConstants_ENDHDR;
      if (scanOffset < 0) {
          return E_ZIP_EXCEPTION;
      }
      Int64 stopOffset = scanOffset - 65536;
      if (stopOffset < 0) {
          stopOffset = 0;
      }

      while(1) {
          mRaf->Seek(scanOffset);
          Int64 curptr;
          mRaf->GetFilePointer(&curptr);
          Int64 flag;
          ec = ZipEntry::ReadInt32LE((IRandomAccessFile*)mRaf, &flag);
          if (flag == 101010256L) {
              break;
          }
          scanOffset--;
          if (scanOffset < stopOffset) {
               return E_ZIP_EXCEPTION;
          }
      }

      Int64 offset;
      mRaf->GetFilePointer(&offset);
      AutoPtr<RAFStream> rafs = new RAFStream(mRaf, offset);
      AutoPtr<IBufferedInputStream> bin;
      ec = CBufferedInputStream::New((IInputStream *)rafs->Probe(EIID_IInputStream), ZipConstants_ENDHDR, (IBufferedInputStream **)&bin);
      Int32 diskNumber;
      Int32 diskWithCentralDir;
      Int32 numEntries;
      Int32 totalNumEntries;
      Int64 centralDirOffset;

      //mLe-->ReadInt16LE((IInputStream*)rafs->Probe(EIID_IInputStream), &diskNumber);
      mLer->ReadInt16LE(bin, &diskNumber);
      mLer->ReadInt16LE(bin, &diskWithCentralDir);
      mLer->ReadInt16LE(bin, &numEntries);
      mLer->ReadInt16LE(bin, &totalNumEntries);
      Int64 dirsize;
      mLer->ReadInt32LE(bin, &dirsize);
      Int32 xsize = dirsize;
      mLer->ReadInt32LE(bin, &centralDirOffset);
      Int32 off = centralDirOffset;

      if (numEntries != totalNumEntries || diskNumber != 0 || diskWithCentralDir != 0) {
           return E_ZIP_EXCEPTION;
      }

      //rafs = new RAFStream(mRaf, centralDirOffset);
      rafs = new RAFStream(mRaf, off);
      AutoPtr<IBufferedInputStream> bin1;
      ec = CBufferedInputStream::New((IInputStream *)rafs->Probe(EIID_IInputStream), 4096, (IBufferedInputStream **)&bin1);
      for (int i = 0; i < numEntries; i++) {
          AutoPtr<IZipEntry> newEntry;
          ec = CZipEntry::New((Int32)mLer, (IInputStream *)bin1->Probe(EIID_IInputStream), (IZipEntry**)&newEntry);
          if (FAILED(ec)) break;
          String name;
          newEntry->GetName(&name);
          mEntries[name] = newEntry;
      }

      return ec;
}

/**
 * Constructs a new {@code ZipFile} with the specified file.
 *
 * @param file
 *            the file to read from.
 * @throws ZipException
 *             if a ZIP error occurs.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode ZipFile::Init(
    /* [in] */ IFile* file)
{
    return Init(file, IZipFile_OPEN_READ);
}

/**
 * Opens a file as <i>ZIP-archive</i>. "mode" must be {@code OPEN_READ} or
 * {@code OPEN_DELETE} . The latter sets the "delete on exit" flag through a
 * file.
 *
 * @param file
 *            the ZIP file to read.
 * @param mode
 *            the mode of the file open operation.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode ZipFile::Init(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode)
{
    file->GetPath(&mFileName);
    if (mode != IZipFile_OPEN_READ &&
        mode != (IZipFile_OPEN_READ | IZipFile_OPEN_DELETE)) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkRead(fileName);
//    }
    if ((mode & IZipFile_OPEN_DELETE) != 0) {
//        if (security != null) {
//            security.checkDelete(fileName);
//        }
        mFileToDeleteOnClose = file; // file.deleteOnExit();
    }
    else {
        mFileToDeleteOnClose = NULL;
    }

    FAIL_RETURN(CRandomAccessFile::New(file, "r", (IRandomAccessFile **)&mRaf));

    return ReadCentralDir();
}

/**
 * Opens a ZIP archived file.
 *
 * @param name
 *            the name of the ZIP file.
 * @throws IOException
 *             if an IOException occurs.
 */
ECode ZipFile::Init(
    /* [in] */ const String& name)
{
    AutoPtr<IFile> file;
    CFile::New(name, (IFile**)&file);
    return Init(file, IZipFile_OPEN_READ);
}
