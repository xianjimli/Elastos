
#include "cmdef.h"
#include "ZipOutputStream.h"
#include "CZipFile.h"
#include <elastos/Algorithm.h>
#include <elastos/System.h>

using namespace Elastos::Core;


const Int32 ZipOutputStream::ZIPLocalHeaderVersionNeeded;

ZipOutputStream::ZipOutputStream()
    : mCompressMethod(IDeflater_DEFLATED)
    , mCompressLevel(IDeflater_DEFAULT_COMPRESSION)
    , mOffset(0)
    , mCurOffset(0)
    , mNameBytes(NULL)
{
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&mCDir);
    CCRC32::NewByFriend((CCRC32**)&mCrc);
}

ZipOutputStream::~ZipOutputStream()
{
    if (mNameBytes != NULL) ArrayOf<Byte>::Free(mNameBytes);
}

/**
 * Constructs a new {@code ZipOutputStream} with the specified output
 * stream.
 *
 * @param p1
 *            the {@code OutputStream} to write the data to.
 */
ECode ZipOutputStream::Init(
    /* [in] */ IOutputStream* p1)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend(IDeflater_DEFAULT_COMPRESSION, TRUE, (CDeflater**)&deflater);
    return DeflaterOutputStream::Init(p1, (IDeflater*)deflater.Get());
}

/**
 * Closes the current {@code ZipEntry}, if any, and the underlying output
 * stream. If the stream is already closed this method does nothing.
 *
 * @throws IOException
 *             If an error occurs closing the stream.
 */
//@Override
ECode ZipOutputStream::Close()
{
    if (mOut != NULL) {
        FAIL_RETURN(Finish());
        FAIL_RETURN(mOut->Close());
        mOut = NULL;
    }
    return NOERROR;
}

/**
 * Closes the current {@code ZipEntry}. Any entry terminal data is written
 * to the underlying stream.
 *
 * @throws IOException
 *             If an error occurs closing the entry.
 */
ECode ZipOutputStream::CloseEntry()
{
    FAIL_RETURN(CheckClosed());
    if (mCurrentEntry == NULL) {
        return NOERROR;
    }

    Int32 method;
    if (mCurrentEntry->GetMethod(&method), method == IZipOutputStream_DEFLATED) {
        FAIL_RETURN(DeflaterOutputStream::Finish());
    }

    // Verify values for STORED types
    if (mCurrentEntry->GetMethod(&method), method == IZipOutputStream_STORED) {
        Int64 value;
        mCrc->GetValue(&value);
        if (value != mCurrentEntry->mCrc) {
//            throw new ZipException("CRC mismatch");
            return E_ZIP_EXCEPTION;
        }
        if (mCurrentEntry->mSize != mCrc->mTbytes) {
//           throw new ZipException("Size mismatch");
            return E_ZIP_EXCEPTION;
        }
    }
    mCurOffset = ZipConstants_LOCHDR;

    // Write the DataDescriptor
    if (mCurrentEntry->GetMethod(&method), method != IZipOutputStream_STORED) {
        Int32 value;
        mCurOffset += ZipConstants_EXTHDR;
        WriteInt64(mOut, ZipConstants_EXTSIG);
        mCrc->GetValue(&mCurrentEntry->mCrc);
        WriteInt64(mOut, mCurrentEntry->mCrc);
        mDef->GetTotalOut(&value);
        mCurrentEntry->mCompressedSize = value;
        WriteInt64(mOut, mCurrentEntry->mCompressedSize);
        mDef->GetTotalIn(&value);
        mCurrentEntry->mSize = value;
        WriteInt64(mOut, mCurrentEntry->mSize);
    }
    // Update the CentralDirectory
    // http://www.pkware.com/documents/casestudies/APPNOTE.TXT
    Int32 flags = (mCurrentEntry->GetMethod(&method), method) == IZipOutputStream_STORED ?
            0 : CZipFile::GPBF_DATA_DESCRIPTOR_FLAG;
    WriteInt64(mCDir, ZipConstants_CENSIG);
    WriteInt16(mCDir, ZIPLocalHeaderVersionNeeded); // Version created
    WriteInt16(mCDir, ZIPLocalHeaderVersionNeeded); // Version to extract
    WriteInt16(mCDir, flags);
    mCurrentEntry->GetMethod(&method);
    WriteInt16(mCDir, method);
    WriteInt16(mCDir, mCurrentEntry->mTime);
    WriteInt16(mCDir, mCurrentEntry->mModDate);
    Int64 checksum;
    mCrc->GetValue(&checksum);
    WriteInt64(mCDir, checksum);
    if (mCurrentEntry->GetMethod(&method), method == IZipOutputStream_DEFLATED) {
        Int32 value;
        mDef->GetTotalOut(&value);
        mCurOffset += WriteInt64(mCDir, value);
        mDef->GetTotalIn(&value);
        WriteInt64(mCDir, value);
    }
    else {
        mCurOffset += WriteInt64(mCDir, mCrc->mTbytes);
        WriteInt64(mCDir, mCrc->mTbytes);
    }
    mCurOffset += WriteInt16(mCDir, mNameLength);
    if (mCurrentEntry->mExtra != NULL) {
        mCurOffset += WriteInt16(mCDir, mCurrentEntry->mExtra->GetLength());
    }
    else {
        WriteInt16(mCDir, 0);
    }
    String c;
    mCurrentEntry->GetComment(&c);
    if (!c.IsNull()) {
        WriteInt16(mCDir, c.GetCharCount());
    }
    else {
        WriteInt16(mCDir, 0);
    }
    WriteInt16(mCDir, 0); // Disk Start
    WriteInt16(mCDir, 0); // Internal File Attributes
    WriteInt64(mCDir, 0); // External File Attributes
    WriteInt64(mCDir, mOffset);
    mCDir->WriteBuffer(*mNameBytes);
    ArrayOf<Byte>::Free(mNameBytes);
    mNameBytes = NULL;
    if (mCurrentEntry->mExtra != NULL) {
        mCDir->WriteBuffer(*mCurrentEntry->mExtra);
    }
    mOffset +=mCurOffset;
    if (!c.IsNull()) {
        mCDir->WriteBuffer(ArrayOf<Byte>(
            reinterpret_cast<Byte*>(const_cast<char*>((const char*)c)), c.GetLength()));
    }
    mCurrentEntry = NULL;
    mCrc->Reset();
    mDef->Reset();
    mDone = FALSE;
    return NOERROR;
}

/**
 * Indicates that all entries have been written to the stream. Any terminal
 * information is written to the underlying stream.
 *
 * @throws IOException
 *             if an error occurs while terminating the stream.
 */
//@Override
ECode ZipOutputStream::Finish()
{
    // TODO: is there a bug here? why not checkClosed?
    if (mOut == NULL) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    if (mCDir == NULL) {
        return NOERROR;
    }
    if (mEntries.GetSize() == 0) {
//        throw new ZipException("No entries");
        return E_ZIP_EXCEPTION;
    }
    if (mCurrentEntry != NULL) {
        FAIL_RETURN(CloseEntry());
    }
    Int32 cdirSize;
    mCDir->GetSize(&cdirSize);
    // Write Central Dir End
    WriteInt64(mCDir, ZipConstants_ENDSIG);
    WriteInt16(mCDir, 0); // Disk Number
    WriteInt16(mCDir, 0); // Start Disk
    WriteInt16(mCDir, mEntries.GetSize()); // Number of entries
    WriteInt16(mCDir, mEntries.GetSize()); // Number of entries
    WriteInt64(mCDir, cdirSize); // Size of central dir
    WriteInt64(mCDir, mOffset); // Offset of central dir
    if (!mComment.IsNull()) {
        WriteInt16(mCDir, mComment.GetCharCount());
        mCDir->WriteBuffer(ArrayOf<Byte>(
                reinterpret_cast<Byte*>(const_cast<char*>((const char*)mComment)),
                mComment.GetLength()));
    }
    else {
        WriteInt16(mCDir, 0);
    }
    // Write the central dir
    ArrayOf<Byte> *bytes;
    mCDir->ToByteArray(&bytes);
    mOut->WriteBuffer(*bytes);
    mCDir = NULL;
    ArrayOf<Byte>::Free(bytes);
    return NOERROR;
}

/**
 * Writes entry information to the underlying stream. Data associated with
 * the entry can then be written using {@code write()}. After data is
 * written {@code closeEntry()} must be called to complete the writing of
 * the entry to the underlying stream.
 *
 * @param ze
 *            the {@code ZipEntry} to store.
 * @throws IOException
 *             If an error occurs storing the entry.
 * @see #write
 */
ECode ZipOutputStream::PutNextEntry(
    /* [in] */ IZipEntry* _ze)
{
    CZipEntry* ze = (CZipEntry*)_ze;
    if (mCurrentEntry != NULL) {
        FAIL_RETURN(CloseEntry());
    }
    Int32 method;
    ze->GetMethod(&method);
    if (method == IZipOutputStream_STORED ||
        (mCompressMethod == IZipOutputStream_STORED && method == -1)) {
        if (ze->mCrc == -1) {
//            throw new ZipException("CRC mismatch");
            return E_ZIP_EXCEPTION;
        }
        if (ze->mSize == -1 && ze->mCompressedSize == -1) {
//            throw new ZipException("Size mismatch");
            return E_ZIP_EXCEPTION;
        }
        if (ze->mSize != ze->mCompressedSize && ze->mCompressedSize != -1 && ze->mSize != -1) {
//            throw new ZipException("Size mismatch");
            return E_ZIP_EXCEPTION;
        }
    }
    FAIL_RETURN(CheckClosed());
    Vector<String>::Iterator it = Find(mEntries.Begin(), mEntries.End(), ze->mName);
    if (it != mEntries.End()) {
//        throw new ZipException("Entry already exists: " + ze.name);
        return E_ZIP_EXCEPTION;
    }
    mNameLength = GetUtf8Count(ze->mName);
    if (mNameLength > 0xffff) {
//        throw new IllegalArgumentException("Name too long: " + nameLength + " UTF-8 bytes");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mDef->SetLevel(mCompressLevel);
    mCurrentEntry = ze;
    mEntries.PushBack(mCurrentEntry->mName);
    mCurrentEntry->GetMethod(&method);
    if (method == -1) {
        mCurrentEntry->SetMethod(mCompressMethod);
    }
    // BEGIN android-changed
    // Local file header.
    // http://www.pkware.com/documents/casestudies/APPNOTE.TXT
    mCurrentEntry->GetMethod(&method);
    Int32 flags = method == IZipOutputStream_STORED ? 0 : CZipFile::GPBF_DATA_DESCRIPTOR_FLAG;
    // Java always outputs UTF-8 filenames. (Before Java 7, the RI didn't set this flag and used
    // modified UTF-8. From Java 7, it sets this flag and uses normal UTF-8.)
    flags |= CZipFile::GPBF_UTF8_FLAG;
    WriteInt64(mOut, ZipConstants_LOCSIG); // Entry header
    WriteInt16(mOut, ZIPLocalHeaderVersionNeeded); // Extraction version
    WriteInt16(mOut, flags);
    mCurrentEntry->GetMethod(&method);
    WriteInt16(mOut, method);
    Int64 time;
    mCurrentEntry->GetTime(&time);
    if (time == -1) {
        mCurrentEntry->SetTime(System::GetCurrentTimeMillis());
    }
    WriteInt16(mOut, mCurrentEntry->mTime);
    WriteInt16(mOut, mCurrentEntry->mModDate);
    // END android-changed

    if (mCurrentEntry->GetMethod(&method), method == IZipOutputStream_STORED) {
        if (mCurrentEntry->mSize == -1) {
            mCurrentEntry->mSize = mCurrentEntry->mCompressedSize;
        }
        else if (mCurrentEntry->mCompressedSize == -1) {
            mCurrentEntry->mCompressedSize = mCurrentEntry->mSize;
        }
        WriteInt64(mOut, mCurrentEntry->mCrc);
        WriteInt64(mOut, mCurrentEntry->mSize);
        WriteInt64(mOut, mCurrentEntry->mSize);
    }
    else {
        WriteInt64(mOut, 0);
        WriteInt64(mOut, 0);
        WriteInt64(mOut, 0);
    }
    WriteInt16(mOut, mNameLength);
    if (mCurrentEntry->mExtra != NULL) {
        WriteInt16(mOut, mCurrentEntry->mExtra->GetLength());
    }
    else {
        WriteInt16(mOut, 0);
    }
    mNameBytes = ToUTF8Bytes(mCurrentEntry->mName, mNameLength);
    mOut->WriteBuffer(*mNameBytes);
    if (mCurrentEntry->mExtra != NULL) {
        mOut->WriteBuffer(*mCurrentEntry->mExtra);
    }
    return NOERROR;
}

/**
 * Sets the {@code ZipFile} comment associated with the file being written.
 *
 * @param comment
 *            the comment associated with the file.
 */
ECode ZipOutputStream::SetComment(
    /* [in] */ const String& comment)
{
    if (comment.GetCharCount() > 0xFFFF) {
//        throw new IllegalArgumentException("Comment too long: " + comment.length() + " characters");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mComment = comment;
    return NOERROR;
}

/**
 * Sets the compression level to be used for writing entry data. This level
 * may be set on a per entry basis. The level must have a value between -1
 * and 8 according to the {@code Deflater} compression level bounds.
 *
 * @param level
 *            the compression level (ranging from -1 to 8).
 * @see Deflater
 */
ECode ZipOutputStream::SetLevel(
    /* [in] */ Int32 level)
{
    if (level < IDeflater_DEFAULT_COMPRESSION
            || level > IDeflater_BEST_COMPRESSION) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCompressLevel = level;
    return NOERROR;
}

/**
 * Sets the compression method to be used when compressing entry data.
 * method must be one of {@code STORED} (for no compression) or {@code
 * DEFLATED}.
 *
 * @param method
 *            the compression method to use.
 */
ECode ZipOutputStream::SetMethod(
    /* [in] */ Int32 method)
{
    if (method != IZipOutputStream_STORED && method != IZipOutputStream_DEFLATED) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCompressMethod = method;
    return NOERROR;
}

Int64 ZipOutputStream::WriteInt64(
    /* [in] */ IOutputStream* os,
    /* [in] */ Int64 i)
{
    // Write out the long value as an unsigned int
    os->Write((Int32)(i & 0xFF));
    os->Write((Int32)(i >> 8) & 0xFF);
    os->Write((Int32)(i >> 16) & 0xFF);
    os->Write((Int32)(i >> 24) & 0xFF);
    return i;
}

Int32 ZipOutputStream::WriteInt16(
    /* [in] */ IOutputStream* os,
    /* [in] */ Int32 i)
{
    os->Write(i & 0xFF);
    os->Write((i >> 8) & 0xFF);
    return i;

}

/**
 * Writes data for the current entry to the underlying stream.
 *
 * @exception IOException
 *                If an error occurs writing to the stream
 */
//@Override
ECode ZipOutputStream::WriteBufferEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    // avoid int overflow, check null buf
    if ((off < 0 || (nbytes < 0) || off > buffer.GetLength())
            || (buffer.GetLength() - off < nbytes)) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (mCurrentEntry == NULL) {
//        throw new ZipException("No active entry");
        return E_ZIP_EXCEPTION;
    }

    Int32 method;
    mCurrentEntry->GetMethod(&method);
    if (method == IZipOutputStream_STORED) {
        FAIL_RETURN(mOut->WriteBufferEx(off, nbytes, buffer));
    }
    else {
        FAIL_RETURN(DeflaterOutputStream::WriteBufferEx(off, nbytes, buffer));
    }
    return mCrc->UpdateEx2(buffer, off, nbytes);
}

Int32 ZipOutputStream::GetUtf8Count(
    /* [in] */ const String& value)
{
    return value.GetLength();
}

ArrayOf<Byte>* ZipOutputStream::ToUTF8Bytes(
    /* [in] */ const String& value,
    /* [in] */ Int32 length)
{
    assert(value.GetLength() == length);
    ArrayOf<Byte>* result = ArrayOf<Byte>::Alloc(length);
    memcpy(result->GetPayload(), (const char*)value, length);
    return result;
}

ECode ZipOutputStream::CheckClosed()
{
    if (mCDir == NULL) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}
