
#ifndef  __CBACKUPDATAINPUTSTREAM_H__
#define  __CBACKUPDATAINPUTSTREAM_H__

#include "_CBackupDataInputStream.h"
#include <elastos/AutoPtr.h>

class BackupHelperDispatcher;

CarClass(CBackupDataInputStream)
{
public:
	CBackupDataInputStream();

	~CBackupDataInputStream();

    /** @hide */
    CARAPI constructor(
    	/* [in]  */ IBackupDataInput* data);

    /**
     * Read one byte of entity data from the stream, returning it as
     * an integer value.  If more than {@link #size()} bytes of data
     * are read from the stream, the output of this method is undefined.
     *
     * @return The byte read, or undefined if the end of the stream has been reached.
     */
    CARAPI Read(
    	/* [in]  */ Int32* readSize);

    /**
     * Read up to {@code size} bytes of data into a byte array, beginning at position
     * {@code offset} within the array.
     *
     * @param b Byte array into which the data will be read
     * @param offset The data will be stored in {@code b} beginning at this index
     *   within the array.
     * @param size The number of bytes to read in this operation.  If insufficient
     *   data exists within the entity to fulfill this request, only as much data
     *   will be read as is available.
     * @return The number of bytes of data read, or zero if all of the entity's
     *   data has already been read.
     */
	CARAPI ReadBufferEx(
	    /* [in] */ Int32 offset,
	    /* [in] */ Int32 size,
        /* [out] */ ArrayOf<Byte>* b,
	    /* [out] */ Int32* readSize);

    /**
     * Read enough entity data into a byte array to fill the array.
     *
     * @param b Byte array to fill with data from the stream.  If the stream does not
     *   have sufficient data to fill the array, then the contents of the remainder of
     *   the array will be undefined.
     * @return The number of bytes of data read, or zero if all of the entity's
     *   data has already been read.
     */

	CARAPI ReadBuffer(
	    /* [out] */ ArrayOf<Byte>* b,
	    /* [out] */ Int32* readSize);

    /**
     * Report the key string associated with this entity within the backup data set.
     *
     * @return The key string for this entity, equivalent to calling
     *   {@link BackupDataInput#getKey()} on the underlying {@link BackupDataInput}.
     */
    CARAPI GetKey(
    	/* [out] */ String* key);

    /**
     * Report the total number of bytes of data available for the current entity.
     *
     * @return The number of data bytes available, equivalent to calling
     *   {@link BackupDataInput#getDataSize()} on the underlying {@link BackupDataInput}.
     */
    CARAPI Size(
    	/* [out] */ Int32* size);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

public:
    AutoPtr<IBackupDataInput> mData;

private:
    String mKey;
    Int32 mDataSize;

    ArrayOf<Byte>* mOneByte;

friend class BackupHelperDispatcher;
};

#endif  //__CBACKUPDATAINPUTSTREAM_H__
