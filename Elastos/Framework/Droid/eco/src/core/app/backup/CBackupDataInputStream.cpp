
#include "app/backup/CBackupDataInputStream.h"

CBackupDataInputStream::CBackupDataInputStream()
	: mDataSize(0)
	, mOneByte(NULL)
{
}

CBackupDataInputStream::~CBackupDataInputStream()
{
	if (mOneByte != NULL) {
		ArrayOf<Byte>::Free(mOneByte);
		mOneByte = NULL;
	}
}

ECode CBackupDataInputStream::constructor(
	/* [in]  */ IBackupDataInput* data)
{
    mData = data;
    return NOERROR;
}

ECode CBackupDataInputStream::Read(
	/* [in]  */ Int32* readSize)
{
	assert(readSize != NULL);
    if (mOneByte == NULL) {
        mOneByte = ArrayOf<Byte>::Alloc(1);
    }

    return mData->ReadEntityData(mOneByte, 0, 1, readSize);
}

ECode CBackupDataInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size,
    /* [out] */ ArrayOf<Byte>* b,
    /* [out] */ Int32* readSize)
{
	assert(readSize != NULL);
	return mData->ReadEntityData(b, offset, size, readSize);
}

ECode CBackupDataInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* b,
    /* [out] */ Int32* readSize)
{
	assert(b != NULL);
    return mData->ReadEntityData(b, 0, b->GetLength(), readSize);
}

ECode CBackupDataInputStream::GetKey(
	/* [out] */ String* key)
{
	assert(key != NULL);
	*key = mKey;
    return NOERROR;
}

ECode CBackupDataInputStream::Size(
	/* [out] */ Int32* size)
{
	assert(size != NULL);
	*size = mDataSize;
    return NOERROR;
}

ECode CBackupDataInputStream::Available(
    /* [out] */ Int32* number)
{
	assert(number != NULL);
	*number = 0;
	return NOERROR;
}

ECode CBackupDataInputStream::Close()
{
	return NOERROR;
}

ECode CBackupDataInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
	return NOERROR;
}

ECode CBackupDataInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
	assert(supported != NULL);
	*supported = FALSE;
	return NOERROR;
}

ECode CBackupDataInputStream::Reset()
{
	return NOERROR;
}

ECode CBackupDataInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
//  return Streams.skipByReading(this, byteCount);
    return E_NOT_IMPLEMENTED;
}
