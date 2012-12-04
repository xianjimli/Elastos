
#include "ZipInputStream.h"
#include "CZipFile.h"
#include <elastos/Math.h>

using namespace Elastos::Core;


const Int32 ZipInputStream::ZIPLocalHeaderVersionNeeded;

ZipInputStream::ZipInputStream()
    : mEntriesEnd(FALSE)
    , mHasDD(FALSE)
    , mEntryIn(0)
    , mLastRead(0)
{
    mHdrBuf = ArrayOf<Byte>::Alloc(ZipConstants_LOCHDR - ZipConstants_LOCVER);
    CCRC32::NewByFriend((CCRC32**)&mCrc);
    mNameBuf = ArrayOf<Byte>::Alloc(256);
}

ZipInputStream::~ZipInputStream()
{
    ArrayOf<Byte>::Free(mHdrBuf);
    ArrayOf<Byte>::Free(mNameBuf);
}

/**
 * Closes this {@code ZipInputStream}.
 *
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
//@Override
ECode ZipInputStream::Close()
{
    if (!mClosed) {
        FAIL_RETURN(CloseEntry()); // Close the current entry
        FAIL_RETURN(InflaterInputStream::Close());
    }
    return NOERROR;
}

/**
 * Closes the current ZIP entry and positions to read the next entry.
 *
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode ZipInputStream::CloseEntry()
{
    FAIL_RETURN(CheckClosed());
    if (mCurrentEntry == NULL) {
        return NOERROR;
    }
//    if (mCurrentEntry instanceof java.util.jar.JarEntry) {
//        Attributes temp = ((JarEntry) mCurrentEntry).getAttributes();
//        if (temp != null && temp.containsKey("hidden")) {
//            return;
//        }
//    }

    /*
     * The following code is careful to leave the ZipInputStream in a
     * consistent state, even when close() results in an exception. It does
     * so by:
     *  - pushing bytes back into the source stream
     *  - reading a data descriptor footer from the source stream
     *  - resetting fields that manage the entry being closed
     */

    // Ensure all entry bytes are read
    ECode ec;
    Int64 count;
    ec = Skip(Math::INT64_MAX_VALUE, &count);

    Int32 inB, out;
    if (mCurrentEntry->mCompressionMethod == IZipInputStream_DEFLATED) {
        mInf->GetTotalIn(&inB);
        mInf->GetTotalOut(&out);
    }
    else {
        inB = mInRead;
        out = mInRead;
    }
    Int32 diff = mEntryIn - inB;
    // Pushback any required bytes
    if (diff != 0) {
        IPushbackInputStream::Probe(mIn)->UnreadBufferEx(mLen - diff, diff, *mBuf);
    }

    FAIL_RETURN(ReadAndVerifyDataDescriptor(inB, out));

    mInf->Reset();
    mLastRead = mInRead = mEntryIn = mLen = 0;
    mCrc->Reset();
    mCurrentEntry = NULL;

    return ec;
}

ECode ZipInputStream::ReadAndVerifyDataDescriptor(
    /* [in] */ Int32 inB,
    /* [in] */ Int32 out)
{
    if (mHasDD) {
        Int32 count;
        FAIL_RETURN(mIn->ReadBufferEx(0, ZipConstants_EXTHDR, mHdrBuf, &count));
        Int64 sig = GetInt64(*mHdrBuf, 0);
        if (sig != ZipConstants_EXTSIG) {
//            throw new ZipException(String.format("unknown format (EXTSIG=%x)", sig));
            return E_ZIP_EXCEPTION;
        }
        mCurrentEntry->mCrc = GetInt64(*mHdrBuf, ZipConstants_EXTCRC);
        mCurrentEntry->mCompressedSize = GetInt64(*mHdrBuf, ZipConstants_EXTSIZ);
        mCurrentEntry->mSize = GetInt64(*mHdrBuf, ZipConstants_EXTLEN);
    }
    Int64 checksum;
    mCrc->GetValue(&checksum);
    if (mCurrentEntry->mCrc != checksum) {
//        throw new ZipException("CRC mismatch");
        return E_ZIP_EXCEPTION;
    }
    if (mCurrentEntry->mCompressedSize != inB || mCurrentEntry->mSize != out) {
//        throw new ZipException("Size mismatch");
        return E_ZIP_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Reads the next entry from this {@code ZipInputStream} or {@code null} if
 * no more entries are present.
 *
 * @return the next {@code ZipEntry} contained in the input stream.
 * @throws IOException
 *             if an {@code IOException} occurs.
 * @see ZipEntry
 */
ECode ZipInputStream::GetNextEntry(
    /* [out] */ IZipEntry** entry)
{
    FAIL_RETURN(CloseEntry());
    if (mEntriesEnd) {
        *entry = NULL;
        return NOERROR;
    }

    Int32 x = 0, count = 0;
    while (count != 4) {
        FAIL_RETURN(mIn->ReadBufferEx(count, 4 - count, mHdrBuf, &x));
        count += x;
        if (x == -1) {
            *entry = NULL;
            return NOERROR;
        }
    }
    Int64 hdr = GetInt64(*mHdrBuf, 0);
    if (hdr == ZipConstants_CENSIG) {
        mEntriesEnd = TRUE;
        *entry = NULL;
        return NOERROR;
    }
    if (hdr != ZipConstants_LOCSIG) {
        *entry = NULL;
        return NOERROR;
    }

    // Read the local header
    count = 0;
    while (count != (ZipConstants_LOCHDR - ZipConstants_LOCVER)) {
        FAIL_RETURN(mIn->ReadBufferEx(count,
                (ZipConstants_LOCHDR - ZipConstants_LOCVER) - count, mHdrBuf, &x));
        count += x;
        if (x == -1) {
//            throw new EOFException();
            return E_EOF_EXCEPTION;
        }
    }
    Int32 version = GetInt16(*mHdrBuf, 0) & 0xff;
    if (version > ZIPLocalHeaderVersionNeeded) {
//        throw new ZipException("Cannot read local header version " + version);
        return E_ZIP_EXCEPTION;
    }
    Int32 flags = GetInt16(*mHdrBuf, ZipConstants_LOCFLG - ZipConstants_LOCVER);
    mHasDD = ((flags & CZipFile::GPBF_DATA_DESCRIPTOR_FLAG) != 0);
    Int32 cetime = GetInt16(*mHdrBuf, ZipConstants_LOCTIM - ZipConstants_LOCVER);
    Int32 cemodDate = GetInt16(*mHdrBuf, ZipConstants_LOCTIM - ZipConstants_LOCVER + 2);
    Int32 cecompressionMethod = GetInt16(*mHdrBuf, ZipConstants_LOCHOW - ZipConstants_LOCVER);
    Int64 cecrc = 0, cecompressedSize = 0, cesize = -1;
    if (!mHasDD) {
        cecrc = GetInt64(*mHdrBuf, ZipConstants_LOCCRC - ZipConstants_LOCVER);
        cecompressedSize = GetInt64(*mHdrBuf, ZipConstants_LOCSIZ - ZipConstants_LOCVER);
        cesize = GetInt64(*mHdrBuf, ZipConstants_LOCLEN - ZipConstants_LOCVER);
    }
    Int32 flen = GetInt16(*mHdrBuf, ZipConstants_LOCNAM - ZipConstants_LOCVER);
    if (flen == 0) {
//        throw new ZipException("Entry is not named");
        return E_ZIP_EXCEPTION;
    }
    Int32 elen = (Int32)GetInt16(*mHdrBuf, ZipConstants_LOCEXT - ZipConstants_LOCVER);

    count = 0;
    if (flen > mNameBuf->GetLength()) {
        ArrayOf<Byte>::Free(mNameBuf);
        mNameBuf = ArrayOf<Byte>::Alloc(flen + 1);
//        charBuf = new char[flen];
    }
    while (count != flen) {
        FAIL_RETURN(mIn->ReadBufferEx(count, flen - count, mNameBuf, &x));
        count += x;

        if (x == -1) {
//            throw new EOFException();
            return E_EOF_EXCEPTION;
        }
    }
    //(*mNameBuf)[flen] = '\0';
    mCurrentEntry = CreateZipEntry(String((const char *)mNameBuf->GetPayload(), mNameBuf->GetLength()));
    mCurrentEntry->mTime = cetime;
    mCurrentEntry->mModDate = cemodDate;
    mCurrentEntry->SetMethod(cecompressionMethod);
    if (cesize != -1) {
        mCurrentEntry->SetCrc(cecrc);
        mCurrentEntry->SetSize(cesize);
        mCurrentEntry->SetCompressedSize(cecompressedSize);
    }
    if (elen > 0) {
        count = 0;
        ArrayOf<Byte>* e = ArrayOf<Byte>::Alloc(elen);
        while (count != elen) {
            FAIL_RETURN(mIn->ReadBufferEx(count, elen - count, e, &x));
            count += x;
            if (x == -1) {
//                throw new EOFException();
                return E_EOF_EXCEPTION;
            }
        }
        mCurrentEntry->SetExtra(e);
    }
    *entry = (IZipEntry*)mCurrentEntry.Get();
    (*entry)->AddRef();
    return NOERROR;
}

/* Read 4 bytes from the buffer and store it as an int */

/**
 * Reads up to the specified number of uncompressed bytes into the buffer
 * starting at the offset.
 *
 * @param buffer
 *            a byte array
 * @param start
 *            the starting offset into the buffer
 * @param length
 *            the number of bytes to read
 * @return the number of bytes read
 */
//@Override
ECode ZipInputStream::ReadBufferEx(
    /* [in] */ Int32 start,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
    Boolean finished;
    mInf->Finished(&finished);
    if (finished || mCurrentEntry == NULL) {
        *number = -1;
        return NOERROR;
    }
    // avoid int overflow, check null buffer
    if (start > buffer->GetLength() || length < 0 || start < 0
            || buffer->GetLength() - start < length) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (mCurrentEntry->mCompressionMethod == IZipInputStream_STORED) {
        Int32 csize = (Int32)mCurrentEntry->mSize;
        if (mInRead >= csize) {
            *number = -1;
            return NOERROR;
        }
        if (mLastRead >= mLen) {
            mLastRead = 0;
            FAIL_RETURN(mIn->ReadBuffer(mBuf, &mLen));
            if (mLen == -1) {
                mEof = TRUE;
                *number = -1;
                return NOERROR;
            }
            mEntryIn += mLen;
        }
        Int32 toRead = length > (mLen - mLastRead) ? mLen - mLastRead : length;
        if ((csize - mInRead) < toRead) {
            toRead = csize - mInRead;
        }
        memcpy(buffer->GetPayload() + start, mBuf->GetPayload() + mLastRead, toRead);
        mLastRead += toRead;
        mInRead += toRead;
        mCrc->UpdateEx2(*buffer, start, toRead);
        *number = toRead;
        return NOERROR;
    }
    Boolean value;
    mInf->NeedsInput(&value);
    if (value) {
        FAIL_RETURN(Fill());
        if (mLen > 0) {
            mEntryIn += mLen;
        }
    }
    Int32 read;
    if (FAILED(mInf->InflateEx(start, length, buffer, &read))) {
        return E_ZIP_EXCEPTION;
    }
    mInf->Finished(&value);
    if (read == 0 && value) {
        *number = -1;
        return NOERROR;
    }
    mCrc->UpdateEx2(*buffer, start, read);
    *number = read;
    return NOERROR;
}

/**
 * Skips up to the specified number of bytes in the current ZIP entry.
 *
 * @param value
 *            the number of bytes to skip.
 * @return the number of bytes skipped.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
//@Override
ECode ZipInputStream::Skip(
    /* [in] */ Int64 value,
    /* [out] */ Int64* number)
{
    if (value < 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 skipped = 0;
    ArrayOf<Byte>* b = ArrayOf<Byte>::Alloc((Int32)Math::Min(value, 2048ll));
    while (skipped != value) {
        Int64 rem = value - skipped;
        Int32 x;
        FAIL_RETURN(ReadBufferEx(0,
                (Int32)(b->GetLength() > rem ? rem : b->GetLength()), b, &x));
        if (x == -1) {
            *number = skipped;
            return NOERROR;
        }
        skipped += x;
    }
    *number = skipped;
    return NOERROR;
}

//@Override
ECode ZipInputStream::Available(
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
    // The InflaterInputStream contract says we must only return 0 or 1.
    *number = (mCurrentEntry == NULL || mInRead < mCurrentEntry->mSize) ? 1 : 0;
    return NOERROR;
}

/**
 * creates a {@link ZipEntry } with the given name.
 *
 * @param name
 *            the name of the entry.
 * @return the created {@code ZipEntry}.
 */
AutoPtr<CZipEntry> ZipInputStream::CreateZipEntry(
    /* [in] */ const String& name)
{
    AutoPtr<CZipEntry> entry;
    CZipEntry::NewByFriend(name, (CZipEntry**)&entry);
    return entry;
}

Int32 ZipInputStream::GetInt16(
    /* [in] */ ArrayOf<Byte>& buffer,
    /* [in] */ Int32 off)
{
    return (buffer[off] & 0xFF) | ((buffer[off + 1] & 0xFF) << 8);
}

Int64 ZipInputStream::GetInt64(
    /* [in] */ ArrayOf<Byte>& buffer,
    /* [in] */ Int32 off)
{
    Int64 l = 0;
    l |= (buffer[off] & 0xFF);
    l |= (buffer[off + 1] & 0xFF) << 8;
    l |= (buffer[off + 2] & 0xFF) << 16;
    l |= ((Int64)(buffer[off + 3] & 0xFF)) << 24;
    return l;
}

ECode ZipInputStream::CheckClosed()
{
    if (mClosed) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Constructs a new {@code ZipInputStream} from the specified input stream.
 *
 * @param stream
 *            the input stream to representing a ZIP archive.
 */
ECode ZipInputStream::Init(
    /* [in] */ IInputStream* stream)
{
    if (stream == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<IPushbackInputStream> is;
    FAIL_RETURN(CPushbackInputStream::New(stream, BUF_SIZE, (IPushbackInputStream**)&is));
    AutoPtr<CInflater> inflater;
    CInflater::NewByFriend(TRUE, (CInflater**)&inflater);
    return InflaterInputStream::Init(is.Get(), (IInflater*)inflater.Get());
}
