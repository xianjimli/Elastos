
#include "ZipEntry.h"
#include <stdio.h>

/*
 * Read a two-byte short in little-endian order.
 */
ECode ZipEntry::LittleEndianReader::ReadInt16LE(
    /* [in] */ IInputStream* in,
    /* [out] */ Int32* value)
{
    Int32 count;
    in->ReadBufferEx(0, 2, &mB, &count);
    if (count == 2) {
        *value = (mB[0] & 0XFF) | ((mB[1] & 0XFF) << 8);
        return NOERROR;
    }
    else {
//        throw new EOFException("in ZipEntry.readShortLE(InputStream)");
        return E_EOF_EXCEPTION;
    }
}

/*
 * Read a four-byte int in little-endian order.
 */
ECode ZipEntry::LittleEndianReader::ReadInt32LE(
    /* [in] */ IInputStream* in,
    /* [out] */ Int64* value)
{
    assert(in != NULL);
    assert(value != NULL);
    Int32 count;
    
    in->Reset();
    in->ReadBufferEx(0, 4, &mB, &count);
    if (count == 4) {
        *value = (((mB[0] & 0XFF))
                 | ((mB[1] & 0XFF) << 8)
                 | ((mB[2] & 0XFF) << 16)
                 | ((mB[3] & 0XFF) << 24))
               & 0XFFFFFFFFll; // Here for sure NO sign extension is wanted.
        return NOERROR;
    }
    else {
//        throw new EOFException("in ZipEntry.readIntLE(InputStream)");
        return E_EOF_EXCEPTION;
    }
}



ZipEntry::ZipEntry()
    : mCompressedSize(-1)
    , mCrc(-1)
    , mSize(-1)
    , mCompressionMethod(-1)
    , mTime(-1)
    , mModDate(-1)
    , mExtra(NULL)
    , mNameLen(-1)
    , mLocalHeaderRelOffset(-1)
{}

ZipEntry::~ZipEntry()
{
    if (mExtra != NULL) ArrayOf<Byte>::Free(mExtra);
}

/**
 * Gets the comment for this {@code ZipEntry}.
 *
 * @return the comment for this {@code ZipEntry}, or {@code null} if there
 *         is no comment. If we're reading an archive with
 *         {@code ZipInputStream} the comment is not available.
 */
String ZipEntry::GetComment()
{
    return mComment;
}

/**
 * Gets the compressed size of this {@code ZipEntry}.
 *
 * @return the compressed size, or -1 if the compressed size has not been
 *         set.
 */
Int64 ZipEntry::GetCompressedSize()
{
    return mCompressedSize;
}

/**
 * Gets the checksum for this {@code ZipEntry}.
 *
 * @return the checksum, or -1 if the checksum has not been set.
 */
Int64 ZipEntry::GetCrc()
{
    return mCrc;
}

/**
 * Gets the extra information for this {@code ZipEntry}.
 *
 * @return a byte array containing the extra information, or {@code null} if
 *         there is none.
 */
ArrayOf<Byte>* ZipEntry::GetExtra()
{
    return mExtra;
}

/**
 * Gets the compression method for this {@code ZipEntry}.
 *
 * @return the compression method, either {@code DEFLATED}, {@code STORED}
 *         or -1 if the compression method has not been set.
 */
Int32 ZipEntry::GetMethod()
{
    return mCompressionMethod;
}

/**
 * Gets the name of this {@code ZipEntry}.
 *
 * @return the entry name.
 */
String ZipEntry::GetName()
{
    return mName;
}

/**
 * Gets the uncompressed size of this {@code ZipEntry}.
 *
 * @return the uncompressed size, or {@code -1} if the size has not been
 *         set.
 */
Int64 ZipEntry::GetSize()
{
    return mSize;
}

/**
 * Gets the last modification time of this {@code ZipEntry}.
 *
 * @return the last modification time as the number of milliseconds since
 *         Jan. 1, 1970.
 */
Int64 ZipEntry::GetTime()
{
//    if (time != -1) {
//        GregorianCalendar cal = new GregorianCalendar();
//        cal.set(Calendar.MILLISECOND, 0);
//        cal.set(1980 + ((modDate >> 9) & 0x7f), ((modDate >> 5) & 0xf) - 1,
//                modDate & 0x1f, (time >> 11) & 0x1f, (time >> 5) & 0x3f,
//                (time & 0x1f) << 1);
//        return cal.getTime().getTime();
//    }
    return -1;
}

/**
 * Determine whether or not this {@code ZipEntry} is a directory.
 *
 * @return {@code true} when this {@code ZipEntry} is a directory, {@code
 *         false} otherwise.
 */
Boolean ZipEntry::IsDirectory()
{
    return mName.GetChar(mName.GetCharCount() - 1) == '/';
}

/**
 * Sets the comment for this {@code ZipEntry}.
 *
 * @param string
 *            the comment for this entry.
 */
ECode ZipEntry::SetComment(
    /* [in] */ const String& string)
{
    if (string.IsNull() || string.GetCharCount() <= 0xFFFF) {
        mComment = string;
        return NOERROR;
    }
    else {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

/**
 * Sets the compressed size for this {@code ZipEntry}.
 *
 * @param value
 *            the compressed size (in bytes).
 */
ECode ZipEntry::SetCompressedSize(
    /* [in] */ Int64 value)
{
    mCompressedSize = value;
    return NOERROR;
}

/**
 * Sets the checksum for this {@code ZipEntry}.
 *
 * @param value
 *            the checksum for this entry.
 * @throws IllegalArgumentException
 *             if {@code value} is < 0 or > 0xFFFFFFFFL.
 */
ECode ZipEntry::SetCrc(
    /* [in] */ Int64 value)
{
    if (value >= 0 && value <= 0xFFFFFFFFll) {
        mCrc = value;
        return NOERROR;
    }
    else {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

/**
 * Sets the extra information for this {@code ZipEntry}.
 *
 * @param data
 *            a byte array containing the extra information.
 * @throws IllegalArgumentException
 *             when the length of data is greater than 0xFFFF bytes.
 */
ECode ZipEntry::SetExtra(
    /* [in] */ ArrayOf<Byte>* data)
{
    if (data == NULL || data->GetLength() <= 0xFFFF) {
        mExtra = data;
        return NOERROR;
    }
    else {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

/**
 * Sets the compression method for this {@code ZipEntry}.
 *
 * @param value
 *            the compression method, either {@code DEFLATED} or {@code
 *            STORED}.
 * @throws IllegalArgumentException
 *             when value is not {@code DEFLATED} or {@code STORED}.
 */
ECode ZipEntry::SetMethod(
    /* [in] */ Int32 value)
{
    if (value != IZipEntry_STORED && value != IZipEntry_DEFLATED) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCompressionMethod = value;
    return NOERROR;
}

/**
 * Sets the uncompressed size of this {@code ZipEntry}.
 *
 * @param value
 *            the uncompressed size for this entry.
 * @throws IllegalArgumentException
 *             if {@code value} < 0 or {@code value} > 0xFFFFFFFFL.
 */
ECode ZipEntry::SetSize(
    /* [in] */ Int64 value)
{
    if (value >= 0 && value <= 0xFFFFFFFFll) {
        mSize = value;
        return NOERROR;
    }
    else {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

/**
 * Sets the modification time of this {@code ZipEntry}.
 *
 * @param value
 *            the modification time as the number of milliseconds since Jan.
 *            1, 1970.
 */
ECode ZipEntry::SetTime(
    /* [in] */ Int64 value)
{
//    GregorianCalendar cal = new GregorianCalendar();
//    cal.setTime(new Date(value));
//    int year = cal.get(Calendar.YEAR);
//    if (year < 1980) {
//        modDate = 0x21;
//        time = 0;
//    } else {
//        modDate = cal.get(Calendar.DATE);
//        modDate = (cal.get(Calendar.MONTH) + 1 << 5) | modDate;
//        modDate = ((cal.get(Calendar.YEAR) - 1980) << 9) | modDate;
//        time = cal.get(Calendar.SECOND) >> 1;
//        time = (cal.get(Calendar.MINUTE) << 5) | time;
//        time = (cal.get(Calendar.HOUR_OF_DAY) << 11) | time;
//    }
    return E_NOT_IMPLEMENTED;
}

/*
//  * Internal constructor.  Creates a new ZipEntry by reading the
//  * Central Directory Entry from "in", which must be positioned at
//  * the CDE signature.
//  *
//  * On exit, "in" will be positioned at the start of the next entry.
//  */
//*ECode ZipEntry::ZipEntry(
//    /* in */ InputStream *in)
//{
//    return NOERROR;
//     /*
//      * We're seeing performance issues when we call readShortLE and
//      * readIntLE, so we're going to read the entire header at once
//      * and then parse the results out without using any function calls.
//      * Uglier, but should be much faster.
//      *
//      * Note that some lines look a bit different, because the corresponding
//      * fields or locals are long and so we need to do & 0xffffffffl to avoid
//      * problems induced by sign extension.
//      */

//     byte[] hdrBuf = ler.hdrBuf;
//     myReadFully(in, hdrBuf);

//     long sig = (hdrBuf[0] & 0xff) | ((hdrBuf[1] & 0xff) << 8) |
//         ((hdrBuf[2] & 0xff) << 16) | ((hdrBuf[3] << 24) & 0xffffffffL);
//     if (sig != CENSIG) {
//          throw new ZipException("Central Directory Entry not found");
//     }

//     compressionMethod = (hdrBuf[10] & 0xff) | ((hdrBuf[11] & 0xff) << 8);
//     time = (hdrBuf[12] & 0xff) | ((hdrBuf[13] & 0xff) << 8);
//     modDate = (hdrBuf[14] & 0xff) | ((hdrBuf[15] & 0xff) << 8);
//     crc = (hdrBuf[16] & 0xff) | ((hdrBuf[17] & 0xff) << 8)
//             | ((hdrBuf[18] & 0xff) << 16)
//             | ((hdrBuf[19] << 24) & 0xffffffffL);
//     compressedSize = (hdrBuf[20] & 0xff) | ((hdrBuf[21] & 0xff) << 8)
//             | ((hdrBuf[22] & 0xff) << 16)
//             | ((hdrBuf[23] << 24) & 0xffffffffL);
//     size = (hdrBuf[24] & 0xff) | ((hdrBuf[25] & 0xff) << 8)
//             | ((hdrBuf[26] & 0xff) << 16)
//             | ((hdrBuf[27] << 24) & 0xffffffffL);
//     nameLen = (hdrBuf[28] & 0xff) | ((hdrBuf[29] & 0xff) << 8);
//     int extraLen = (hdrBuf[30] & 0xff) | ((hdrBuf[31] & 0xff) << 8);
//     int commentLen = (hdrBuf[32] & 0xff) | ((hdrBuf[33] & 0xff) << 8);
//     mLocalHeaderRelOffset = (hdrBuf[42] & 0xff) | ((hdrBuf[43] & 0xff) << 8)
//             | ((hdrBuf[44] & 0xff) << 16)
//             | ((hdrBuf[45] << 24) & 0xffffffffL);

//     byte[] nameBytes = new byte[nameLen];
//     myReadFully(in, nameBytes);

//     byte[] commentBytes = null;
//     if (commentLen > 0) {
//         commentBytes = new byte[commentLen];
//         myReadFully(in, commentBytes);
//     }

//     if (extraLen > 0) {
//         extra = new byte[extraLen];
//         myReadFully(in, extra);
//     }

//     // The RI has always assumed UTF-8. (If GPBF_UTF8_FLAG isn't set, the encoding is
//     // actually IBM-437.)
//     name = new String(nameBytes, 0, nameBytes.length, Charsets.UTF_8);
//     if (commentBytes != null) {
//         comment = new String(commentBytes, 0, commentBytes.length, Charsets.UTF_8);
//     } else {
//         comment = null;
//     }
//}

ECode ZipEntry::MyReadFully(
    /* [in] */ IInputStream* in,
    /* [in] */ ArrayOf<Byte>* b)
{
    Int32 len = b->GetLength();
    Int32 off = 0;

    while (len > 0) {
        Int32 count;
        in->ReadBufferEx(off, len, b, &count);
        if (count <= 0) {
//            throw new EOFException();
            return E_EOF_EXCEPTION;
        }
        off += count;
        len -= count;
    }
    return NOERROR;
}

/*
 * Read a four-byte int in little-endian order.
 */
// static long readIntLE(RandomAccessFile raf) throws IOException {
//     int b0 = raf.read();
//     int b1 = raf.read();
//     int b2 = raf.read();
//     int b3 = raf.read();

//     if (b3 < 0) {
//         throw new EOFException("in ZipEntry.readIntLE(RandomAccessFile)");
//     }
//     return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24); // ATTENTION: DOES SIGN EXTENSION: IS THIS WANTED?
// }

/**
 * Constructs a new {@code ZipEntry} with the specified name.
 *
 * @param name
 *            the name of the ZIP entry.
 * @throws IllegalArgumentException
 *             if the name length is outside the range (> 0xFFFF).
 */
ECode ZipEntry::Init(
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (name.GetCharCount() > 0xFFFF) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mName = name;
    return NOERROR;
}

/**
 * Constructs a new {@code ZipEntry} using the values obtained from {@code
 * ze}.
 *
 * @param ze
 *            the {@code ZipEntry} from which to obtain values.
 */
ECode ZipEntry::Init(
    /* [in] */ ZipEntry* ze)
{
    mName = ze->mName;
    mComment = ze->mComment;
    mTime = ze->mTime;
    mSize = ze->mSize;
    mCompressedSize = ze->mCompressedSize;
    mCrc = ze->mCrc;
    mCompressionMethod = ze->mCompressionMethod;
    mModDate = ze->mModDate;
    mExtra = ze->mExtra == NULL ? NULL : ze->mExtra->Clone();
    mNameLen = ze->mNameLen;
    mLocalHeaderRelOffset = ze->mLocalHeaderRelOffset;
    return NOERROR;
}

//--------jian feng add 2012-11-29-------------


ECode ZipEntry::Init(
    /* in */ Int32 little,
    /* in */ IInputStream* in)
{
        LittleEndianReader *ler = (LittleEndianReader *) little;
        ArrayOf_<Byte, ZipConstants_CENHDR> hdrBuf;
        MyReadFully(in, &hdrBuf);

        Int64 sig = ((hdrBuf)[0] & 0xff) | (((hdrBuf)[1] & 0xff) << 8) |
            (((hdrBuf)[2] & 0xff) << 16) | (((hdrBuf)[3] << 24) & 0xffffffffL);
        //if (sig != CENSIG) {
        //     throw new ZipException("Central Directory Entry not found");
        //}
        //
        mCompressionMethod = ((hdrBuf)[10] & 0xff) | (((hdrBuf)[11] & 0xff) << 8);
        mTime = ((hdrBuf)[12] & 0xff) | (((hdrBuf)[13] & 0xff) << 8);
        mModDate = ((hdrBuf)[14] & 0xff) | (((hdrBuf)[15] & 0xff) << 8);
        mCrc = ((hdrBuf)[16] & 0xff) | (((hdrBuf)[17] & 0xff) << 8)
                | (((hdrBuf)[18] & 0xff) << 16)
                | (((hdrBuf)[19] << 24) & 0xffffffffL);
        mCompressedSize = ((hdrBuf)[20] & 0xff) | (((hdrBuf)[21] & 0xff) << 8)
                | (((hdrBuf)[22] & 0xff) << 16)
                | (((hdrBuf)[23] << 24) & 0xffffffffL);
        mSize = ((hdrBuf)[24] & 0xff) | (((hdrBuf)[25] & 0xff) << 8)
                | (((hdrBuf)[26] & 0xff) << 16)
                | (((hdrBuf)[27] << 24) & 0xffffffffL);
        mNameLen = ((hdrBuf)[28] & 0xff) | (((hdrBuf)[29] & 0xff) << 8);
        Int32 extraLen = ((hdrBuf)[30] & 0xff) | (((hdrBuf)[31] & 0xff) << 8);
        Int32 commentLen = ((hdrBuf)[32] & 0xff) | (((hdrBuf)[33] & 0xff) << 8);
        mLocalHeaderRelOffset = ((hdrBuf)[42] & 0xff) | (((hdrBuf)[43] & 0xff) << 8)
                | (((hdrBuf)[44] & 0xff) << 16)
                | (((hdrBuf)[45] << 24) & 0xffffffffL);

        ArrayOf<Byte> *nameBytes = ArrayOf<Byte>::Alloc(mNameLen);
        MyReadFully(in, nameBytes);

        ArrayOf<Byte> *commentBytes = NULL;
        if (commentLen > 0) {
            commentBytes = ArrayOf<Byte>::Alloc(commentLen);
            MyReadFully(in, commentBytes);
        }

        ArrayOf<Byte> *extra = NULL;
        if (extraLen > 0) {
            extra = ArrayOf<Byte>::Alloc(extraLen);
            MyReadFully(in, extra);
        }

        // The RI has always assumed UTF-8. (If GPBF_UTF8_FLAG isn't set, the encoding is
        // actually IBM-437.)
        mName = String((const char *)nameBytes->GetPayload(), nameBytes->GetLength());
        if (commentBytes != NULL) {
            mComment = String((const char *)commentBytes->GetPayload(), commentBytes->GetLength()); 
        } else {
            //mComment = NULL;
        }
        if(nameBytes) {
            ArrayOf<Byte>::Free(nameBytes);
            nameBytes = NULL;
        }
        if(extra) {
            ArrayOf<Byte>::Free(extra);
            extra = NULL;
        }
        if(commentBytes) {
            ArrayOf<Byte>::Free(commentBytes);
            commentBytes = NULL;
        }
        return NOERROR;
}

ECode ZipEntry::ReadInt32LE(
    /* in */  IRandomAccessFile* rafs,
    /* out */ Int64 *value)
{

    Int32 b0; 
    Int32 b1;
    Int32 b2;
    Int32 b3;
    ECode ec = NOERROR;
    
    IDataInput *rafInput = (IDataInput*)rafs->Probe(EIID_IDataInput); 

    //rafInput->Read(&b0);
    //rafInput->Read(&b1);
    //rafInput->Read(&b2);
    //rafInput->Read(&b3);
    rafs->Read(&b0);
    rafs->Read(&b1);
    rafs->Read(&b2);
    rafs->Read(&b3);

    if (b3 < 0) {
        return E_EOF_EXCEPTION;
    }
    *value = (Int64) b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
    return ec;
}
