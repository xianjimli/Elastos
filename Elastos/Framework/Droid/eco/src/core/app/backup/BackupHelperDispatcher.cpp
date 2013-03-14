
#include "app/backup/BackupHelperDispatcher.h"
#include "utils/log.h"
#include "app/backup/CBackupDataInputStream.h"
#include <unistd.h>

#define VERSION_1_HEADER 0x01706c48  // 'Hlp'1 little endian

CString BackupHelperDispatcher::TAG = "BackupHelperDispatcher";

BackupHelperDispatcher::BackupHelperDispatcher()
{

}

UInt32 BackupHelperDispatcher::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 BackupHelperDispatcher::Release()
{
	return ElRefBase::Release();
}

void BackupHelperDispatcher::AddHelper(
    /* [in] */ const String& keyPrefix,
    /* [in] */ IBackupHelper* helper)
{
    mHelpers[keyPrefix] = helper;
}

void BackupHelperDispatcher::PerformBackup(
    /* [in] */ IParcelFileDescriptor* oldState,
    /* [in] */ IBackupDataOutput* data,
    /* [in] */ IParcelFileDescriptor* newState)/* throws IOException */
{
    // First, do the helpers that we've already done, since they're already in the state
    // file.
    Int32 err;
    Header header;
    //TreeMap<String,BackupHelper> helpers = (TreeMap<String,BackupHelper>)mHelpers.clone();
    IFileDescriptor* oldStateFD = NULL;
    IFileDescriptor* newStateFD = NULL;
    newState->GetFileDescriptor(&newStateFD);

    if (oldState != NULL) {
        oldState->GetFileDescriptor(&oldStateFD);
        while ((err = readHeader_native(&header, oldStateFD)) >= 0) {
            if (err == 0) {
                AutoPtr<IBackupHelper> helper = mHelpers[header.mKeyPrefix];
                if (helper != NULL) {
                    DoOneBackup(oldState, data, newState, header, helper);
                    mHelpers.Erase(header.mKeyPrefix);
                } else {
                    skipChunk_native(oldStateFD, header.mChunkSize);
                }
            }
        }
    }

    // Then go through and do the rest that we haven't done.
    HashMap<String, AutoPtr<IBackupHelper> >::Iterator it = mHelpers.Begin();
    for (; it != mHelpers.End(); ++it) {
        header.mKeyPrefix = it->mFirst;
        //LOGD(TAG, "handling new helper '" + header.mKeyPrefix + "'");
        AutoPtr<IBackupHelper> helper = it->mSecond;
        DoOneBackup(oldState, data, newState, header, helper);
    }
}

void BackupHelperDispatcher::PerformRestore(
    /* [in] */ IBackupDataInput* input,
    /* [in] */ Int32 appVersionCode,
    /* [in] */ IParcelFileDescriptor* newState) /*throws IOException */
{
    Boolean alreadyComplained = FALSE;

    AutoPtr<IBackupDataInputStream> stream;
    CBackupDataInputStream::New(input, (IBackupDataInputStream**) &stream);

    assert(input != NULL);
    Boolean next = FALSE;
    input->ReadNextHeader(&next);
    while (next) {
        String rawKey;
        input->GetKey(&rawKey);
        Int32 pos = rawKey.IndexOf(':');
        if (pos > 0) {
            String prefix = rawKey.Substring(0, pos);
            AutoPtr<IBackupHelper> helper = mHelpers[prefix];
            if (helper != NULL) {
                input->GetDataSize(& ((CBackupDataInputStream*)stream.Get())->mDataSize);
                ((CBackupDataInputStream*)stream.Get())->mKey = rawKey.Substring(pos+1);
                helper->RestoreEntity(stream);
            } else {
                if (!alreadyComplained) {
                    //LOGW(TAG, "Couldn't find helper for: '" + (const char*)rawKey + "'");
                    alreadyComplained = TRUE;
                }
            }
        } else {
            if (!alreadyComplained) {
                //LOGW(TAG, "Entity with no prefix: '" + (const char*)rawKey + "'");
                alreadyComplained = TRUE;
            }
        }

        input->SkipEntityData(); // In case they didn't consume the data.
    }

    // Write out the state files -- mHelpers is a TreeMap, so the order is well defined.
    HashMap<String, AutoPtr<IBackupHelper> >::Iterator it = mHelpers.Begin();
    for (; it != mHelpers.End(); ++it) {
        AutoPtr<IBackupHelper> helper = it->mSecond;
        helper->WriteNewStateDescription(newState);
    }
}

void BackupHelperDispatcher::DoOneBackup(
    /* [in] */ IParcelFileDescriptor* oldState,
    /* [in] */ IBackupDataOutput* data,
    /* [in] */ IParcelFileDescriptor* newState,
    /* [in] */ const Header& header,
    /* [in] */ IBackupHelper* helper) /*throws IOException*/
{
    Int32 err;
    AutoPtr<IFileDescriptor> newStateFD;
    newState->GetFileDescriptor((IFileDescriptor**) &newStateFD);

    // allocate space for the header in the file
    Int32 pos = allocateHeader_native(header, newStateFD);
    if (pos < 0) {
        //throw new IOException("allocateHeader_native failed (error " + pos + ")");
        assert(0);
    }

    data->SetKeyPrefix(header.mKeyPrefix);

    // do the backup
    helper->PerformBackup(oldState, data, newState);

    // fill in the header (seeking back to pos).  The file pointer will be returned to
    // where it was at the end of performBackup.  Header.chunkSize will not be filled in.
    err = writeHeader_native(header, newStateFD, pos);
    if (err != 0) {
        //throw new IOException("writeHeader_native failed (error " + err + ")");
        assert(0);
    }
}

Int32 BackupHelperDispatcher::readHeader_native(
    /* [in] */ Header* h,
    /* [in] */ IFileDescriptor* fdes)
{
	assert(fdes != NULL);

    chunk_header_v1 flattenedHeader;
    Int32 fd = 0;
    fdes->GetDescriptor(&fd);
    ssize_t amt;
    String keyPrefix;
    char* buf;

    amt = read(fd, &flattenedHeader.mHeaderSize, sizeof(flattenedHeader.mHeaderSize));
    if (amt != sizeof(flattenedHeader.mHeaderSize)) {
        return -1;
    }

    Int32 remainingHeader = flattenedHeader.mHeaderSize - sizeof(flattenedHeader.mHeaderSize);

    if (flattenedHeader.mHeaderSize < (Int32)sizeof(chunk_header_v1)) {
        LOGW("Skipping unknown header: %d bytes", flattenedHeader.mHeaderSize);
        if (remainingHeader > 0) {
            lseek(fd, remainingHeader, SEEK_CUR);
            // >0 means skip this chunk
            return 1;
        }
    }

    amt = read(fd, &flattenedHeader.mVersion,
            sizeof(chunk_header_v1)-sizeof(flattenedHeader.mHeaderSize));
    if (amt <= 0) {
        LOGW("Failed reading chunk header");
        return -1;
    }
    remainingHeader -= sizeof(chunk_header_v1)-sizeof(flattenedHeader.mHeaderSize);

    if (flattenedHeader.mVersion != VERSION_1_HEADER) {
        LOGW("Skipping unknown header mVersion: 0x%08x, %d bytes", flattenedHeader.mVersion,
                flattenedHeader.mHeaderSize);
        if (remainingHeader > 0) {
            lseek(fd, remainingHeader, SEEK_CUR);
            // >0 means skip this chunk
            return 1;
        }
    }

    if (flattenedHeader.mDataSize < 0 || flattenedHeader.mNameLength < 0 ||
            remainingHeader < flattenedHeader.mNameLength) {
        LOGW("Malformed V1 header remainingHeader=%d mDataSize=%d mNameLength=%d", remainingHeader,
                flattenedHeader.mDataSize, flattenedHeader.mNameLength);
        return -1;
    }

    buf = keyPrefix.LockBuffer(flattenedHeader.mNameLength);
    if (buf == NULL) {
        LOGW("unable to allocate %d bytes", flattenedHeader.mNameLength);
        return -1;
    }

    amt = read(fd, buf, flattenedHeader.mNameLength);
    buf[flattenedHeader.mNameLength] = 0;

    keyPrefix.UnlockBuffer(flattenedHeader.mNameLength);

    remainingHeader -= flattenedHeader.mNameLength;

    if (remainingHeader > 0) {
        lseek(fd, remainingHeader, SEEK_CUR);
    }

    h->mChunkSize = flattenedHeader.mDataSize;
    h->mKeyPrefix = keyPrefix;

    return 0;
}

Int32 BackupHelperDispatcher::skipChunk_native(
    /* [in] */ IFileDescriptor* fdes,
    /* [in] */ Int32 bytesToSkip)
{
	assert(fdes != NULL);
    Int32 fd = 0;
    fdes->GetDescriptor(&fd);

    lseek(fd, bytesToSkip, SEEK_CUR);

    return 0;
}

Int32 BackupHelperDispatcher::allocateHeader_native(
    /* [in] */ const Header& h,
    /* [in] */ IFileDescriptor* fdes)
{
	assert(fdes != NULL);
    Int32 fd = 0;
    fdes->GetDescriptor(&fd);

    Int32 pos;
    Int32 nameLength = 0;
    Int32 namePadding = 0;
    Int32 headerSize = 0;

    nameLength = h.mKeyPrefix.GetLength();
    namePadding = padding_len(nameLength);

    headerSize = sizeof(chunk_header_v1) + nameLength + namePadding;

    pos = lseek(fd, 0, SEEK_CUR);

    lseek(fd, headerSize, SEEK_CUR);

    return pos;
}

Int32 BackupHelperDispatcher::writeHeader_native(
    /* [in] */ const Header& h,
    /* [in] */ IFileDescriptor* fdes,
    /* [in] */ Int32 pos)
{
	assert(fdes != NULL);
    Int32 fd = 0;
    fdes->GetDescriptor(&fd);

    Int32 err;
    chunk_header_v1 header;

    Int32 namePadding;
    Int32 prevPos;
    const char* buf;

    prevPos = lseek(fd, 0, SEEK_CUR);

    header.mNameLength = h.mKeyPrefix.GetLength();
    namePadding = padding_len(header.mNameLength);

    header.mHeaderSize = sizeof(chunk_header_v1) + header.mNameLength + namePadding;
    header.mVersion = VERSION_1_HEADER;
    header.mDataSize = prevPos - (pos + header.mHeaderSize);

    lseek(fd, pos, SEEK_SET);
    err = write(fd, &header, sizeof(chunk_header_v1));
    if (err != sizeof(chunk_header_v1)) {
        return err;
    }

    buf = (const char*)h.mKeyPrefix;
    err = write(fd, buf, header.mNameLength);
    if (err != header.mNameLength) {
        return err;
    }

    if (namePadding != 0) {
        Int32 zero = 0;
        err = write(fd, &zero, namePadding);
        if (err != namePadding) {
            return err;
        }
    }

    lseek(fd, prevPos, SEEK_SET);
    return 0;
}

Int32 BackupHelperDispatcher::padding_len(
	/* [in] */ Int32 len)
{
    len = len % 4;
    return len == 0 ? len : 4 - len;
}
