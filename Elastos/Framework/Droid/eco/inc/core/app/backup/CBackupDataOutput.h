
#ifndef  __CBACKUPDATAOUTPUT_H__
#define  __CBACKUPDATAOUTPUT_H__

#include "_CBackupDataOutput.h"
#include <utils/BackupHelpers.h>

class FileBackupHelperBase;

CarClass(CBackupDataOutput)
{
public:
	CBackupDataOutput();

	~CBackupDataOutput();

    /** @hide */
    CARAPI constructor(
    	/* [in] */ IFileDescriptor* fd);

    /**
     * Mark the beginning of one record in the backup data stream. This must be called before
     * {@link #writeEntityData}.
     * @param key A string key that uniquely identifies the data record within the application
     * @param dataSize The size in bytes of this record's data.  Passing a dataSize
     *    of -1 indicates that the record under this key should be deleted.
     * @return The number of bytes written to the backup stream
     * @throws IOException if the write failed
     */
    CARAPI WriteEntityHeader(
    	/* [in] */ const String& key,
    	/* [in] */ Int32 dataSize,
    	/* [out] */ Int32* writtenSize);

    /**
     * Write a chunk of data under the current entity to the backup transport.
     * @param data A raw data buffer to send
     * @param size The number of bytes to be sent in this chunk
     * @return the number of bytes written
     * @throws IOException if the write failed
     */
    CARAPI WriteEntityData(
    	/* [in] */ ArrayOf<Byte>* data,
    	/* [in] */ Int32 size,
    	/* [out] */ Int32* writtenSize);

    /** @hide */
    CARAPI SetKeyPrefix(
    	/* [in] */ const String& keyPrefix);

protected:
    /** @hide */
    CARAPI_(void) Finalize();

private:
    static CARAPI_(android::BackupDataWriter*) ctor_native(
    	/* [in] */ IFileDescriptor* fd);

    CARAPI_(void) dtor_native(
    	/* [in] */ android::BackupDataWriter* writer);

    static CARAPI_(Int32) writeEntityHeader_native(
    	/* [in] */ android::BackupDataWriter* writer,
    	/* [in] */ const String& key,
    	/* [in] */ Int32 dataSize);

    static CARAPI_(Int32) writeEntityData_native(
    	/* [in] */ android::BackupDataWriter* writer,
    	/* [in] */ ArrayOf<Byte>* data,
    	/* [in] */ Int32 size);

    static CARAPI_(void) setKeyPrefix_native(
    	/* [in] */ android::BackupDataWriter* writer,
    	/* [in] */ const String& keyPrefix);

private:
	android::BackupDataWriter* mBackupWriter;

friend class FileBackupHelperBase;
};

#endif  //__CBACKUPDATAOUTPUT_H__
