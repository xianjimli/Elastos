#include "app/backup/CBackupDataOutput.h"
#include "utils/log.h"


CBackupDataOutput::CBackupDataOutput()
	: mBackupWriter(NULL)
{
}

CBackupDataOutput::~CBackupDataOutput()
{
	assert(mBackupWriter == NULL);
}

ECode CBackupDataOutput::constructor(
	/* [in] */ IFileDescriptor* fd)
{
    //if (fd == NULL) throw new NullPointerException();
    assert(fd != NULL);
    mBackupWriter = ctor_native(fd);
    if (mBackupWriter == 0) {
    	assert(0);
        //throw new RuntimeException("Native initialization failed with fd=" + fd);
    }

    return NOERROR;
}

ECode CBackupDataOutput::WriteEntityHeader(
	/* [in] */ const String& key,
	/* [in] */ Int32 dataSize,
	/* [out] */ Int32* writtenSize)
{
	assert(writtenSize != NULL);
	*writtenSize = 0;

    Int32 result = writeEntityHeader_native(mBackupWriter, key, dataSize);
    if (result >= 0) {
    	*writtenSize = result;
        return NOERROR;
    } else {
        //throw new IOException("result=0x" + Integer.toHexString(result));
        assert(0);
    }

    return NOERROR;
}

ECode CBackupDataOutput::WriteEntityData(
	/* [in] */ ArrayOf<Byte>* data,
	/* [in] */ Int32 size,
	/* [out] */ Int32* writtenSize)
{
	assert(writtenSize != NULL);
	*writtenSize = 0;

    Int32 result = writeEntityData_native(mBackupWriter, data, size);
    if (result >= 0) {
    	*writtenSize = result;
        return NOERROR;
    } else {
        //throw new IOException("result=0x" + Integer.toHexString(result));
        assert(0);
    }

    return NOERROR;
}

ECode CBackupDataOutput::SetKeyPrefix(
	/* [in] */ const String& keyPrefix)
{
    setKeyPrefix_native(mBackupWriter, keyPrefix);
    return NOERROR;
}

void CBackupDataOutput::Finalize()
{
    dtor_native(mBackupWriter);
}

android::BackupDataWriter* CBackupDataOutput::ctor_native(
	/* [in] */ IFileDescriptor* fdes)
{
	assert(fdes != NULL);
    Int32 fd = 0;
    fdes->GetDescriptor(&fd);
    if (fd == -1) {
        return NULL;
    }

    return new android::BackupDataWriter(fd);
}

void CBackupDataOutput::dtor_native(
	/* [in] */ android::BackupDataWriter* writer)
{
	assert(mBackupWriter != NULL);
	delete mBackupWriter;
	mBackupWriter = NULL;
}

Int32 CBackupDataOutput::writeEntityHeader_native(
	/* [in] */ android::BackupDataWriter* writer,
	/* [in] */ const String& key,
	/* [in] */ Int32 dataSize)
{
	assert(writer != NULL);
	if (key.IsNull()) {
		return -1;
	}

    return writer->WriteEntityHeader(android::String8((const char*) key), dataSize);
}

Int32 CBackupDataOutput::writeEntityData_native(
	/* [in] */ android::BackupDataWriter* writer,
	/* [in] */ ArrayOf<Byte>* data,
	/* [in] */ Int32 size)
{
    if (data == NULL) {
        return -1;
    }

	assert(writer != NULL);

    if (data->GetLength() < size) {
        // size mismatch
        return -1;
    }

    return writer->WriteEntityData(data->GetPayload(), size);
}

void CBackupDataOutput::setKeyPrefix_native(
	/* [in] */ android::BackupDataWriter* writer,
	/* [in] */ const String& key)
{
	assert(writer != NULL);

    const char* keyPrefixUTF = (const char*) key;
    android::String8 keyPrefix(keyPrefixUTF ? keyPrefixUTF : "");
    writer->SetKeyPrefix(keyPrefix);
}
