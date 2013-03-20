#include "ZoneInfoDB.h"
#include "cmdef.h"
#include <elastos/System.h>
#include <elastos/List.h>
#include "TimeZone.h"
#include "ZoneInfo.h"


const String ZoneInfoDB::ZONE_DIRECTORY_NAME = String("/usr/share/zoneinfo/");
            //System:::GetEnv("ANDROID_ROOT") + "/usr/share/zoneinfo/";

const String ZoneInfoDB::ZONE_FILE_NAME = ZONE_DIRECTORY_NAME + "zoneinfo.dat";

const String ZoneInfoDB::INDEX_FILE_NAME = ZONE_DIRECTORY_NAME + "zoneinfo.idx";

//    private static final Object LOCK = new Object();

const String ZoneInfoDB::VERSION = ReadVersion();

ArrayOf<String>*  ZoneInfoDB::mIDs = NULL;
ArrayOf<Int32>*  ZoneInfoDB::mRawUtcOffsets = NULL;
ArrayOf<Int32>*  ZoneInfoDB::mByteOffsets = NULL;
IByteBuffer*  ZoneInfoDB::mMappedData = MapData();

String ZoneInfoDB::ReadVersion()
{
    AutoPtr<IRandomAccessFile> versionFile;
    ECode ec = NOERROR;
    ec = CRandomAccessFile::New(String("zoneinfo.versio"), String("r"), (IRandomAccessFile **)&versionFile);

    Int32 len;
    versionFile->GetLength((Int64 *)&len);
    AutoPtr<IDataInput> input;
    input = IDataInput::Probe(versionFile);

    ArrayOf<Byte> *buf = ArrayOf<Byte>::Alloc(len);
    input->ReadFully(buf);
    versionFile->Close();
    return String((const char *)buf, buf->GetLength()).Trim();
}

void ZoneInfoDB::ReadIndex()
{
    AutoPtr<IRandomAccessFile> indexFile;
    ECode ec = NOERROR;
    ec = CRandomAccessFile::New(INDEX_FILE_NAME, String("r"), (IRandomAccessFile **)&indexFile);
    AutoPtr<IDataInput> input;
    input = IDataInput::Probe(indexFile);

    // The database reserves 40 bytes for each id.
    const Int32 SIZEOF_TZNAME = 40;
    // The database uses 32-bit (4 byte) integers.
    const Int32 SIZEOF_TZINT = 4;

    ArrayOf<Byte> *idBytes = ArrayOf<Byte>::Alloc(SIZEOF_TZNAME);
    Int64 len;
    indexFile->GetLength(&len);

    Int32 numEntries = (Int32) (len / (SIZEOF_TZNAME + 3*SIZEOF_TZINT));

    ArrayOf<Char>* idChars = ArrayOf<Char>::Alloc(numEntries * SIZEOF_TZNAME);
    ArrayOf<Int32>* idEnd =  ArrayOf<Int32>::Alloc(numEntries);
    Int32 idOffset = 0;

    mByteOffsets = ArrayOf<Int32>::Alloc(numEntries);
    mRawUtcOffsets = ArrayOf<Int32>::Alloc(numEntries);

    Int32 length;
    for (Int32 i = 0; i < numEntries; i++) {
        input->ReadFully(idBytes);
        Int32 offset;
        input->ReadInt32(&offset);
        (*mByteOffsets)[i] = offset;

        input->ReadInt32(&length);
        if (length < 44) {
            indexFile->Close();
            return;
        }

        input->ReadInt32(&offset);
        (*mRawUtcOffsets)[i] = offset;

        // Don't include null chars in the String
        Int32 len = idBytes->GetLength();
        Int32 value;
        for (Int32 j = 0; j < len; j++) {
            if ((*idBytes)[j] == 0) {
                break;
            }
            value = (*idBytes)[j] & 0xFF;
            (*idChars)[idOffset++] = *((Char *)&value);
        }

        (*idEnd)[i] = idOffset;
    }

        // We create one string containing all the ids, and then break that into substrings.
        // This way, all ids share a single char[] on the heap.
    String allIds = String((const char *)idChars, idOffset);
    mIDs = ArrayOf<String>::Alloc(numEntries);
    for (Int32 i = 0; i < numEntries; i++) {
        (*mIDs)[i] = allIds.Substring(i == 0 ? 0 : (*idEnd)[i - 1], (*idEnd)[i]);
    }

    indexFile->Close();
}

IByteBuffer* ZoneInfoDB::MapData()
{
    AutoPtr<IRandomAccessFile> file;
    ECode ec = NOERROR;
    ec = CRandomAccessFile::New(INDEX_FILE_NAME, String("r"), (IRandomAccessFile **)&file);
    AutoPtr<IDataInput> input;
    input = IDataInput::Probe(file);

    AutoPtr<IFileChannel> channel;
    file->GetChannel((IFileChannel**) &channel);

    IByteBuffer* buf;
    Int64 size;
    channel->Size(&size);
    channel->Map(FileChannelMapMode_READ_ONLY, 0, size, (IMappedByteBuffer **) &buf);
    file->Close();
    return buf;
}

ITimeZone* ZoneInfoDB::MakeTimeZone(String id)
{
// Work out where in the big data file this time zone is.
//    int index = Arrays.binarySearch(ids, id);
    Int32 index = -1;
    for (Int32 i = 0; i < mIDs->GetLength(); i++) {
        if ((*mIDs)[i] == id) {
            index = i;
            break;
        }
    }

    if (index < 0) {
        return NULL;
    }
    Int32 start = (*mByteOffsets)[index];

    // We duplicate the ByteBuffer to allow unsynchronized access to this shared data,
    // despite Buffer's implicit position.
    AutoPtr<IByteBuffer> data;
    mMappedData->Duplicate((IByteBuffer **) &data);

    data->SetPosition(start);
    // Variable names beginning tzh_ correspond to those in "tzfile.h".
    // Check tzh_magic.
    Int32 value;
    Byte bValue;
    data->GetInt32(&value);
    if (value != 0x545a6966) { // "TZif"
        return NULL;
    }

    // Skip the uninteresting part of the header.
    data->SetPosition(start + 32);

    // Read the sizes of the arrays we're about to read.
    Int32 tzh_timecnt;
    Int32 tzh_typecnt;
    Int32 tzh_charcnt;

    data->GetInt32(&tzh_timecnt);
    data->GetInt32(&tzh_typecnt);
    data->GetInt32(&tzh_charcnt);

    ArrayOf<Int32>* transitions = ArrayOf<Int32>::Alloc(tzh_timecnt);
    for (Int32 i = 0; i < tzh_timecnt; ++i) {
        data->GetInt32(&value);
        (*transitions)[i] = value;
    }

    ArrayOf<Byte>* type = ArrayOf<Byte>::Alloc(tzh_timecnt);
    data->GetBytes(type);

    ArrayOf<Int32>* gmtOffsets = ArrayOf<Int32>::Alloc(tzh_typecnt);
    ArrayOf<Byte>*  isDsts = ArrayOf<Byte>::Alloc(tzh_typecnt);
    ArrayOf<Byte>*  abbreviationIndexes =  ArrayOf<Byte>::Alloc(tzh_typecnt);
    for (Int32 i = 0; i < tzh_typecnt; ++i) {
        data->GetInt32(&value);
        (*gmtOffsets)[i] = value;
        data->GetByte(&bValue);
        (*isDsts)[i] = bValue;
        data->GetByte(&bValue);
        (*abbreviationIndexes)[i] = bValue;
    }

    ArrayOf<Byte>* abbreviationList = ArrayOf<Byte>::Alloc(tzh_charcnt);
    data->GetBytes(abbreviationList);
    ITimeZone *pTimeZone =  (ITimeZone *)new ZoneInfo(id,  \
        *transitions, *type, *gmtOffsets, *isDsts, *abbreviationIndexes, *abbreviationList);
    return pTimeZone;
}


ArrayOf<String>* ZoneInfoDB::GetAvailableIDs()
{
    return mIDs->Clone();
}

ArrayOf<String>* ZoneInfoDB::GetAvailableIDs(
    /* [in] */ Int32 rawOffset)
{
    List<String>* matches = new List<String>();

    for (Int32 i = 0, end = mRawUtcOffsets->GetLength(); i < end; i++) {
        if ((*mRawUtcOffsets)[i] == rawOffset) {
            matches->PushBack((*mIDs)[i]);
        }
    }

    ArrayOf<String>* arrayOfStr = ArrayOf<String>::Alloc(matches->GetSize());
    List<String>::Iterator it = matches->Begin();
    for (Int32 i = 0; it != matches->End(); ++i, ++it) {
        (*arrayOfStr)[i] = *it;
    }
    return arrayOfStr;
}

ITimeZone* ZoneInfoDB::GetSystemDefault()
{
//    TimezoneGetter tzGetter = TimezoneGetter.getInstance();
//    String zoneName = tzGetter != null ? tzGetter.getId() : null;
//    if (zoneName != null) {
//        zoneName = zoneName.trim();
//    }
//    if (zoneName == null || zoneName.isEmpty()) {
//        // use localtime for the simulator
//        // TODO: what does that correspond to?
//        zoneName = "localtime";
//    }
    ITimeZone* tz;
    TimeZone::GetTimeZone(String(""), &tz);
    return tz;
}

ITimeZone* ZoneInfoDB::GetTimeZone(
    /* [in] */ String id)
{
    if (id.IsNull()) {
        return NULL;
    }
    return MakeTimeZone(id);
}

String ZoneInfoDB::GetVersion()
{
    return VERSION;
}