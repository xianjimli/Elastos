
#ifndef __ZIPENTRY_H__
#define __ZIPENTRY_H__

#include "Elastos.Utility.Zip_server.h"
#include <elastos.h>

using namespace Elastos;

class ZipEntry
{
public:
    class LittleEndianReader
    {
    public:
        /*
         * Read a two-byte short in little-endian order.
         */
        CARAPI ReadInt16LE(
            /* [in] */ IInputStream* in,
            /* [out] */ Int32* value);

        /*
         * Read a four-byte int in little-endian order.
         */
        CARAPI ReadInt32LE(
            /* [in] */ IInputStream* in,
            /* [out] */ Int64* value);

    private:
        ArrayOf_<Byte, 4> mB;
        ArrayOf_<Byte, ZipConstants_CENHDR> mHdrBuf;
    };

public:
    ZipEntry();

    virtual ~ZipEntry();

    /**
     * Gets the comment for this {@code ZipEntry}.
     *
     * @return the comment for this {@code ZipEntry}, or {@code null} if there
     *         is no comment. If we're reading an archive with
     *         {@code ZipInputStream} the comment is not available.
     */
    virtual CARAPI_(String) GetComment();

    /**
     * Gets the compressed size of this {@code ZipEntry}.
     *
     * @return the compressed size, or -1 if the compressed size has not been
     *         set.
     */
    virtual CARAPI_(Int64) GetCompressedSize();

    /**
     * Gets the checksum for this {@code ZipEntry}.
     *
     * @return the checksum, or -1 if the checksum has not been set.
     */
    virtual CARAPI_(Int64) GetCrc();

    /**
     * Gets the extra information for this {@code ZipEntry}.
     *
     * @return a byte array containing the extra information, or {@code null} if
     *         there is none.
     */
    virtual CARAPI_(ArrayOf<Byte>*) GetExtra();

    /**
     * Gets the compression method for this {@code ZipEntry}.
     *
     * @return the compression method, either {@code DEFLATED}, {@code STORED}
     *         or -1 if the compression method has not been set.
     */
    virtual CARAPI_(Int32) GetMethod();

    /**
     * Gets the name of this {@code ZipEntry}.
     *
     * @return the entry name.
     */
    virtual CARAPI_(String) GetName();

    /**
     * Gets the uncompressed size of this {@code ZipEntry}.
     *
     * @return the uncompressed size, or {@code -1} if the size has not been
     *         set.
     */
    virtual CARAPI_(Int64) GetSize();

    /**
     * Gets the last modification time of this {@code ZipEntry}.
     *
     * @return the last modification time as the number of milliseconds since
     *         Jan. 1, 1970.
     */
    virtual CARAPI_(Int64) GetTime();

    /**
     * Determine whether or not this {@code ZipEntry} is a directory.
     *
     * @return {@code true} when this {@code ZipEntry} is a directory, {@code
     *         false} otherwise.
     */
    virtual CARAPI_(Boolean) IsDirectory();

    /**
     * Sets the comment for this {@code ZipEntry}.
     *
     * @param string
     *            the comment for this entry.
     */
    virtual CARAPI SetComment(
        /* [in] */ const String& string);

    /**
     * Sets the compressed size for this {@code ZipEntry}.
     *
     * @param value
     *            the compressed size (in bytes).
     */
    virtual CARAPI SetCompressedSize(
        /* [in] */ Int64 value);

    /**
     * Sets the checksum for this {@code ZipEntry}.
     *
     * @param value
     *            the checksum for this entry.
     * @throws IllegalArgumentException
     *             if {@code value} is < 0 or > 0xFFFFFFFFL.
     */
    virtual CARAPI SetCrc(
        /* [in] */ Int64 value);

    /**
     * Sets the extra information for this {@code ZipEntry}.
     *
     * @param data
     *            a byte array containing the extra information.
     * @throws IllegalArgumentException
     *             when the length of data is greater than 0xFFFF bytes.
     */
    virtual CARAPI SetExtra(
        /* [in] */ ArrayOf<Byte>* data);

    /**
     * Sets the compression method for this {@code ZipEntry}.
     *
     * @param value
     *            the compression method, either {@code DEFLATED} or {@code
     *            STORED}.
     * @throws IllegalArgumentException
     *             when value is not {@code DEFLATED} or {@code STORED}.
     */
    virtual CARAPI SetMethod(
        /* [in] */ Int32 value);

    /**
     * Sets the uncompressed size of this {@code ZipEntry}.
     *
     * @param value
     *            the uncompressed size for this entry.
     * @throws IllegalArgumentException
     *             if {@code value} < 0 or {@code value} > 0xFFFFFFFFL.
     */
    virtual CARAPI SetSize(
        /* [in] */ Int64 value);

    /**
     * Sets the modification time of this {@code ZipEntry}.
     *
     * @param value
     *            the modification time as the number of milliseconds since Jan.
     *            1, 1970.
     */
    virtual CARAPI SetTime(
        /* [in] */ Int64 value);

    /*
     * Read a four-byte int in little-endian order.
     */
//    static CARAPI_(Int64) ReadInt32LE(
//        /* [in] */ IRandomAccessFile* raf);

protected:
    /**
     * Constructs a new {@code ZipEntry} with the specified name.
     *
     * @param name
     *            the name of the ZIP entry.
     * @throws IllegalArgumentException
     *             if the name length is outside the range (> 0xFFFF).
     */
    CARAPI Init(
        /* [in] */ const String& name);

    /**
     * Constructs a new {@code ZipEntry} using the values obtained from {@code
     * ze}.
     *
     * @param ze
     *            the {@code ZipEntry} from which to obtain values.
     */
    CARAPI Init(
        /* [in]*/ ZipEntry* ze);

private:
    CARAPI MyReadFully(
        /* [in] */ IInputStream* in,
        /* [in] */ ArrayOf<Byte>* b);

public:
    String mName;
    String mComment;

    Int64 mCompressedSize;
    Int64 mCrc;
    Int64 mSize;

    Int32 mCompressionMethod;
    Int32 mTime;
    Int32 mModDate;

    ArrayOf<Byte>* mExtra;

    Int32 mNameLen;
    Int64 mLocalHeaderRelOffset;
};


#endif //__ZIPENTRY_H__
