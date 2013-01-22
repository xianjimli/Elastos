#ifndef __ZONEINFODB_H__
#define __ZONEINFODB_H__

#include "CTimeZone.h"
#include "Elastos.IO.h"

using namespace Elastos;

class ZoneInfoDB
{
public:
    static ArrayOf<String>* GetAvailableIDs();
    static ArrayOf<String>* GetAvailableIDs(
        /* [in] */ Int32 rawOffset);
    static ITimeZone* GetSystemDefault();
    static ITimeZone* GetTimeZone(
        /* [in] */ String id);
    static String GetVersion();

private:
    static String ReadVersion();
    static void ReadIndex();
    static IByteBuffer* MapData();
    static ITimeZone* MakeTimeZone(
        /* [in] */ String id);

private:
    static const String ZONE_DIRECTORY_NAME;

    /**
     * The name of the file containing the concatenated time zone records.
     */
    static const String ZONE_FILE_NAME;

    /**
     * The name of the file containing the index to each time zone record within
     * the zoneinfo.dat file.
     */
    static const String INDEX_FILE_NAME;

//    private static final Object LOCK = new Object();

    static const String VERSION;

    static ArrayOf<String>*  mIDs;
    static ArrayOf<Int32>* mRawUtcOffsets;
    static ArrayOf<Int32>* mByteOffsets;
    static IByteBuffer* mMappedData;
};

#endif //__ZONEINFODB_H__
