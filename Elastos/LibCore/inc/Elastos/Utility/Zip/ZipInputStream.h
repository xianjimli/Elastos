
#ifndef __ZIPINPUTSTREAM_H__
#define __ZIPINPUTSTREAM_H__

#include "cmdef.h"
#include "InflaterInputStream.h"
#include "CZipEntry.h"
#include "CCRC32.h"

/**
 * This class provides an implementation of {@code FilterInputStream} that
 * uncompresses data from a <i>ZIP-archive</i> input stream.
 * <p>
 * A <i>ZIP-archive</i> is a collection of compressed (or uncompressed) files -
 * the so called ZIP entries. Therefore when reading from a {@code
 * ZipInputStream} first the entry's attributes will be retrieved with {@code
 * getNextEntry} before its data is read.
 * <p>
 * While {@code InflaterInputStream} can read a compressed <i>ZIP-archive</i>
 * entry, this extension can read uncompressed entries as well.
 * <p>
 * Use {@code ZipFile} if you can access the archive as a file directly.
 *
 * @see ZipEntry
 * @see ZipFile
 */
class ZipInputStream : public InflaterInputStream
{
public:
    ZipInputStream();

    virtual ~ZipInputStream();

    /**
     * Closes this {@code ZipInputStream}.
     *
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    //@Override
    CARAPI Close();

    /**
     * Closes the current ZIP entry and positions to read the next entry.
     *
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    virtual CARAPI CloseEntry();

    /**
     * Reads the next entry from this {@code ZipInputStream} or {@code null} if
     * no more entries are present.
     *
     * @return the next {@code ZipEntry} contained in the input stream.
     * @throws IOException
     *             if an {@code IOException} occurs.
     * @see ZipEntry
     */
    virtual CARAPI GetNextEntry(
        /* [out] */ IZipEntry** entry);

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
    CARAPI ReadBufferEx(
        /* [in] */ Int32 start,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

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
    CARAPI Skip(
        /* [in] */ Int64 value,
        /* [out] */ Int64* number);

    //@Override
    CARAPI Available(
        /* [out] */ Int32* number);

protected:
    /**
     * Constructs a new {@code ZipInputStream} from the specified input stream.
     *
     * @param stream
     *            the input stream to representing a ZIP archive.
     */
    ECode Init(
        /* [in] */ IInputStream* stream);

    /**
     * creates a {@link ZipEntry } with the given name.
     *
     * @param name
     *            the name of the entry.
     * @return the created {@code ZipEntry}.
     */
    virtual CARAPI_(AutoPtr<CZipEntry>) CreateZipEntry(
        /* [in] */ const String& name);

private:
    CARAPI ReadAndVerifyDataDescriptor(
        /* [in] */ Int32 inB,
        /* [in] */ Int32 out);

    CARAPI_(Int32) GetInt16(
        /* [in] */ ArrayOf<Byte>& buffer,
        /* [in] */ Int32 off);

    CARAPI_(Int64) GetInt64(
        /* [in] */ ArrayOf<Byte>& buffer,
        /* [in] */ Int32 off);

    CARAPI CheckClosed();

public:
    static const Int32 ZIPLocalHeaderVersionNeeded = 20;

private:
    Boolean mEntriesEnd;

    Boolean mHasDD;

    Int32 mEntryIn;

    Int32 mInRead;
    Int32 mLastRead;

    AutoPtr<CZipEntry> mCurrentEntry;

    ArrayOf<Byte>* mHdrBuf;

    AutoPtr<CCRC32> mCrc;

    ArrayOf<Byte>* mNameBuf;

//    private char[] charBuf = new char[256];
};

#endif //__ZIPINPUTSTREAM_H__

