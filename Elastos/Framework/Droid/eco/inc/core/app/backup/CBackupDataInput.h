
#ifndef  __CBACKUPDATAINPUT_H__
#define  __CBACKUPDATAINPUT_H__

#include "_CBackupDataInput.h"
#include <utils/BackupHelpers.h>

CarClass(CBackupDataInput)
{
private:
    class EntityHeader {
    public:
        EntityHeader();

    public:
        String mKey;
        Int32 mDataSize;
    };

public:
	CBackupDataInput();

	~CBackupDataInput();

    /** @hide */
    CARAPI constructor(
    	/* [in] */ IFileDescriptor* fd);

    /**
     * Extract the next entity header from the restore stream.  After this method
     * return success, the {@link #getKey()} and {@link #getDataSize()} methods can
     * be used to inspect the entity that is now available for processing.
     *
     * @return <code>TRUE</code> when there is an entity ready for consumption from the
     *    restore stream, <code>FALSE</code> if the restore stream has been fully consumed.
     * @throws IOException if an error occurred while reading the restore stream
     */
    CARAPI ReadNextHeader(
    	/* [out] */ Boolean* next);

    /**
     * Report the key associated with the current entity in the restore stream
     * @return the current entity's key string
     * @throws IllegalStateException if the next record header has not yet been read
     */
    CARAPI GetKey(
    	/* [out] */ String* key);

    /**
     * Report the size in bytes of the data associated with the current entity in the
     * restore stream.
     *
     * @return The size of the record's raw data, in bytes
     * @throws IllegalStateException if the next record header has not yet been read
     */
	CARAPI GetDataSize(
	    /* [out] */ Int32* size);

    /**
     * Read a record's raw data from the restore stream.  The record's header must first
     * have been processed by the {@link #readNextHeader()} method.  Multiple calls to
     * this method may be made in order to process the data in chunks; not all of it
     * must be read in a single call.  Once all of the raw data for the current entity
     * has been read, further calls to this method will simply return zero.
     *
     * @param data An allocated byte array of at least 'size' bytes
     * @param offset Offset within the 'data' array at which the data will be placed
     *    when read from the stream
     * @param size The number of bytes to read in this pass
     * @return The number of bytes of data read.  Once all of the data for this entity
     *    has been read, further calls to this method will return zero.
     * @throws IOException if an error occurred when trying to read the restore data stream
     */

	CARAPI ReadEntityData(
		/* [in, out] */ ArrayOf<Byte>* data,
		/* [in] */ Int32 offset,
		/* [in] */ Int32 size,
		/* [out] */ Int32* readSize);

    /**
     * Consume the current entity's data without extracting it into a buffer
     * for further processing.  This allows a {@link android.app.backup.BackupAgent} to
     * efficiently discard obsolete or otherwise uninteresting records during the
     * restore operation.
     *
     * @throws IOException if an error occurred when trying to read the restore data stream
     */
    CARAPI SkipEntityData();

protected:
    /** @hide */
    CARAPI_(void) Finalize();

private:
    CARAPI_(android::BackupDataReader*) ctor_native(
    	/* [in] */ IFileDescriptor* fd);

    CARAPI_(void) dtor_native();

    CARAPI_(Int32) readNextHeader_native(
    	/* [in] */ android::BackupDataReader* reader,
    	/* [in] */ EntityHeader* entity);

    CARAPI_(Int32) readEntityData_native(
    	/* [in] */ android::BackupDataReader* reader,
    	/* [in] */ ArrayOf<Byte>* data,
    	/* [in] */ Int32 offset,
    	/* [in] */ Int32 size);

    CARAPI_(Int32) skipEntityData_native(
    	/* [in] */ android::BackupDataReader* reader);

public:
    android::BackupDataReader* mBackupReader;

private:
    EntityHeader* mHeader;// = new EntityHeader();
    Boolean mHeaderReady;

};

#endif  //__CBACKUPDATAINPUT_H__
