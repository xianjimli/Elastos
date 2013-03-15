
#include "app/backup/CBackupDataInput.h"
#include "utils/log.h"
#include <stdio.h>

CBackupDataInput::EntityHeader::EntityHeader()
    : mDataSize(0)
{

}

CBackupDataInput::CBackupDataInput()
	: mBackupReader(NULL)
	, mHeaderReady(FALSE)
{
	mHeader = new EntityHeader();
}

CBackupDataInput::~CBackupDataInput()
{
	if (mHeader != NULL) {
		delete mHeader;
	}

	assert(mBackupReader == NULL);
}

ECode CBackupDataInput::constructor(
	/* [in] */ IFileDescriptor* fDes)
{
    // if (fDes == NULL) throw new NullPointerException();
    assert(fDes != NULL);
    mBackupReader = ctor_native(fDes);
    if (mBackupReader == 0) {
        //throw new RuntimeException("Native initialization failed with fd=" + fd);
        assert(0);
    }

    return NOERROR;
}

ECode CBackupDataInput::ReadNextHeader(
	/* [out] */ Boolean* next)
{
	assert(next != NULL);
	*next = FALSE;

    Int32 result = readNextHeader_native(mBackupReader, mHeader);
    if (result == 0) {
        // read successfully
        mHeaderReady = TRUE;
        *next = TRUE;
    } else if (result > 0) {
        // done
        mHeaderReady = FALSE;
        *next = FALSE;
    } else {
        // error
        mHeaderReady = FALSE;
        //throw new IOException("failed: 0x" + Integer.toHexString(result));
        assert(0);
    }

    return NOERROR;
}

ECode CBackupDataInput::GetKey(
	/* [out] */ String* key)
{
	assert(key != NULL);

    if (mHeaderReady) {
    	*key = mHeader->mKey;
    } else {
        //throw new IllegalStateException("Entity header not read");
        assert(0);
    }

    return NOERROR;
}

ECode CBackupDataInput::GetDataSize(
    /* [out] */ Int32* size)
{
	assert(size != NULL);
	*size = 0;

    if (mHeaderReady) {
    	*size = mHeader->mDataSize;
    } else {
        //throw new IllegalStateException("Entity header not read");
    	assert(0);
    }

    return NOERROR;
}

ECode CBackupDataInput::ReadEntityData(
	/* [in, out] */ ArrayOf<Byte>* data,
	/* [in] */ Int32 offset,
	/* [in] */ Int32 size,
	/* [out] */ Int32* readSize)
{
	assert(readSize != NULL && data != NULL);
	*readSize = 0;

    if (mHeaderReady) {
        Int32 result = readEntityData_native(mBackupReader, data, offset, size);
        if (result >= 0) {
        	*readSize = result;
        } else {
            //throw new IOException("result=0x" + Integer.toHexString(result));
            assert(0);
        }
    } else {
        //throw new IllegalStateException("Entity header not read");
        assert(0);
    }

    return NOERROR;
}

ECode CBackupDataInput::SkipEntityData()
{
    if (mHeaderReady) {
        skipEntityData_native(mBackupReader);
    } else {
        //throw new IllegalStateException("Entity header not read");
        assert(0);
    }

    return NOERROR;
}

void CBackupDataInput::Finalize() {
	dtor_native();
    // try {
    //     dtor_native(mBackupReader);
    // } finally {
    // 	//TODO: has parent?
    //     //super.finalize();
    // }
}

android::BackupDataReader* CBackupDataInput::ctor_native(
	/* [in] */ IFileDescriptor* fDes)
{
    assert(fDes != NULL);
    Int32 fd = 0;
    fDes->GetDescriptor(&fd);
    if (fd == -1) {
        return NULL;
    }

    return new android::BackupDataReader(fd);
}

void CBackupDataInput::dtor_native()
{
	delete mBackupReader;
	mBackupReader = NULL;
}

Int32 CBackupDataInput::readNextHeader_native(
	/* [in] */ android::BackupDataReader* reader,
	/* [in] */ EntityHeader* entity)
{
    Int32 err = 0;
    Boolean done = FALSE;
    Int32 type = 0;

    bool temp = false;
    err = reader->ReadNextHeader(&temp, &type);
printf("File=[%s], Line = [%d], FUNC = [%s], temp = [%d]\n", __FILE__, __LINE__, __FUNCTION__, temp);
    done = temp;
printf("File=[%s], Line = [%d], FUNC = [%s], done = [%d]\n", __FILE__, __LINE__, __FUNCTION__, done);
    if (done) {
        return 1;
    }

    if (err != 0) {
        return err < 0 ? err : -1;
    }

    switch (type) {
    case android::BACKUP_HEADER_ENTITY_V1:
    {
        android::String8 key;
        size_t dataSize;
        err = reader->ReadEntityHeader(&key, &dataSize);
        if (err != 0) {
            return err < 0 ? err : -1;
        }

        // TODO: Set the fields in the entity object
        entity->mKey = String((const char*) key);
        entity->mDataSize = dataSize;
        return 0;
    }
    default:
        LOGD("Unknown header type: 0x%08x\n", type);
        return -1;
    }

    // done
    return 1;
}

Int32 CBackupDataInput::readEntityData_native(
	/* [in] */ android::BackupDataReader* reader,
	/* [in] */ ArrayOf<Byte>* data,
	/* [in] */ Int32 offset,
	/* [in] */ Int32 size)
{
    if (data == NULL) {
        return -2;
    }

    if (data->GetLength() < (size + offset)) {
        // size mismatch
        return -1;
    }

    assert(reader != NULL);
    return reader->ReadEntityData(data->GetPayload() + offset, size);
}

Int32 CBackupDataInput::skipEntityData_native(
	/* [in] */ android::BackupDataReader* reader)
{
	assert(reader != NULL);
    return reader->SkipEntityData();
}
