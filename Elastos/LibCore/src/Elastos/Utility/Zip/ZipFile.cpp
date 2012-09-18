
#include "ZipFile.h"

extern "C" const InterfaceID EIID_ZipFileRAFStream =
    { 0x83f2304, 0xfc73, 0x4727, { 0x86, 0x6a, 0x81, 0xd5, 0xeb, 0x82, 0xf3, 0x43 } };

ZipFile::RAFStream::RAFStream(
    /* [in] */ IRandomAccessFile* raf,
    /* [in] */ Int64 pos)
    : mSharedRaf(raf)
    , mOffset(pos)
{
//    mLength = raf.length();
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
//    synchronized (mSharedRaf) {
//        mSharedRaf.seek(mOffset);
//        if (len > mLength - mOffset) {
//            len = (int) (mLength - mOffset);
//        }
//        int count = mSharedRaf.read(b, off, len);
//        if (count > 0) {
//            mOffset += count;
//            return count;
//        } else {
//            return -1;
//        }
//    }
    return E_NOT_IMPLEMENTED;
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
//    RandomAccessFile raf = mRaf;
//
//    if (raf != null) { // Only close initialized instances
//        synchronized(raf) {
//            mRaf = null;
//            raf.close();
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
    return E_NOT_IMPLEMENTED;
}

ECode ZipFile::CheckNotClosed()
{
    if (mRaf == NULL) {
//        throw new IllegalStateException("Zip file closed");
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
//    checkNotClosed();
//    final Iterator<ZipEntry> iterator = mEntries.values().iterator();
//
//    return new Enumeration<ZipEntry>() {
//        public boolean hasMoreElements() {
//            checkNotClosed();
//            return iterator.hasNext();
//        }
//
//        public ZipEntry nextElement() {
//            checkNotClosed();
//            return iterator.next();
//        }
//    };
    return E_NOT_IMPLEMENTED;
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
//    checkNotClosed();
//    if (entryName == null) {
//        throw new NullPointerException();
//    }
//
//    ZipEntry ze = mEntries.get(entryName);
//    if (ze == null) {
//        ze = mEntries.get(entryName + "/");
//    }
//    return ze;
    return E_NOT_IMPLEMENTED;
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
//    entry = getEntry(entry.getName());
//    if (entry == null) {
//        return null;
//    }
//
//    /*
//     * Create a ZipInputStream at the right part of the file.
//     */
//    RandomAccessFile raf = mRaf;
//    synchronized (raf) {
//        // We don't know the entry data's start position. All we have is the
//        // position of the entry's local header. At position 28 we find the
//        // length of the extra data. In some cases this length differs from
//        // the one coming in the central header.
//        RAFStream rafstrm = new RAFStream(raf,
//                entry.mLocalHeaderRelOffset + 28);
//        int localExtraLenOrWhatever = ler.readShortLE(rafstrm);
//        // Skip the name and this "extra" data or whatever it is:
//        rafstrm.skip(entry.nameLen + localExtraLenOrWhatever);
//        rafstrm.mLength = rafstrm.mOffset + entry.compressedSize;
//        if (entry.compressionMethod == ZipEntry.DEFLATED) {
//            int bufSize = Math.max(1024, (int)Math.min(entry.getSize(), 65535L));
//            return new ZipInflaterInputStream(rafstrm, new Inflater(true), bufSize, entry);
//        } else {
//            return rafstrm;
//        }
//    }
    return E_NOT_IMPLEMENTED;
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
//    checkNotClosed();
//    return mEntries.size();
    return E_NOT_IMPLEMENTED;
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
//    long scanOffset = mRaf.length() - ENDHDR;
//    if (scanOffset < 0) {
//        throw new ZipException("too short to be Zip");
//    }
//
//    long stopOffset = scanOffset - 65536;
//    if (stopOffset < 0) {
//        stopOffset = 0;
//    }
//
//    while (true) {
//        mRaf.seek(scanOffset);
//        if (ZipEntry.readIntLE(mRaf) == 101010256L) {
//            break;
//        }
//
//        scanOffset--;
//        if (scanOffset < stopOffset) {
//            throw new ZipException("EOCD not found; not a Zip archive?");
//        }
//    }
//
//    /*
//     * Found it, read the EOCD.
//     *
//     * For performance we want to use buffered I/O when reading the
//     * file.  We wrap a buffered stream around the random-access file
//     * object.  If we just read from the RandomAccessFile we'll be
//     * doing a read() system call every time.
//     */
//    RAFStream rafs = new RAFStream(mRaf, mRaf.getFilePointer());
//    BufferedInputStream bin = new BufferedInputStream(rafs, ENDHDR);
//
//    int diskNumber = ler.readShortLE(bin);
//    int diskWithCentralDir = ler.readShortLE(bin);
//    int numEntries = ler.readShortLE(bin);
//    int totalNumEntries = ler.readShortLE(bin);
//    /*centralDirSize =*/ ler.readIntLE(bin);
//    long centralDirOffset = ler.readIntLE(bin);
//    /*commentLen =*/ ler.readShortLE(bin);
//
//    if (numEntries != totalNumEntries ||
//        diskNumber != 0 ||
//        diskWithCentralDir != 0) {
//        throw new ZipException("spanned archives not supported");
//    }
//
//    /*
//     * Seek to the first CDE and read all entries.
//     */
//    rafs = new RAFStream(mRaf, centralDirOffset);
//    bin = new BufferedInputStream(rafs, 4096);
//    for (int i = 0; i < numEntries; i++) {
//        ZipEntry newEntry = new ZipEntry(ler, bin);
//        mEntries.put(newEntry.getName(), newEntry);
//    }
    return E_NOT_IMPLEMENTED;
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

//    mRaf = new RandomAccessFile(fileName, "r");

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