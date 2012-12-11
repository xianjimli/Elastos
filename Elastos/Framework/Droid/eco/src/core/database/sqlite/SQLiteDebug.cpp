
#include "database/sqlite/SQLiteDebug.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cutils/mspace.h>
#include <sqlite3.h>


SQLiteDebug::PagerStats::PagerStats()
    : mDbStats(NULL)
{}

SQLiteDebug::PagerStats::~PagerStats()
{
    if (mDbStats != NULL) {
        delete mDbStats;
    }
}


SQLiteDebug::DbStats::DbStats(
    /*[in]*/ const String& dbName,
    /*[in]*/ Int64 pageCount,
    /*[in]*/ Int64 pageSize,
    /*[in]*/ Int32 lookaside)
    : mDbName(dbName)
    , mPageSize(pageSize / 1024)
    , mDbSize((pageCount * pageSize) / 1024)
    , mLookaside(lookaside)
{}


Int32 SQLiteDebug::sNumActiveCursorsFinalized = 0;
Mutex SQLiteDebug::sLock;

AutoPtr<SQLiteDebug::PagerStats> SQLiteDebug::GetDatabaseInfo()
{
    AutoPtr<PagerStats> stats = new PagerStats();
    GetPagerStats(stats);
    // stats->dbStats = SQLiteDatabase::GetDbStats();
    return stats;
}

void SQLiteDebug::GetPagerStats(
    /*[in]*/ PagerStats* stats)
{
    Int32 memoryUsed;
    Int32 pageCacheOverflo;
    Int32 largestMemAlloc;
    Int32 unused;

    assert(stats != NULL);
    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &memoryUsed, &unused, 0);
    sqlite3_status(SQLITE_STATUS_MALLOC_SIZE, &unused, &largestMemAlloc, 0);
    sqlite3_status(SQLITE_STATUS_PAGECACHE_OVERFLOW, &pageCacheOverflo, &unused, 0);
    stats->mMemoryUsed = memoryUsed;
    stats->mPageCacheOverflo = pageCacheOverflo;
    stats->mLargestMemAlloc = largestMemAlloc;
}

Int64 SQLiteDebug::GetHeapSize()
{
#if !NO_MALLINFO
    struct mallinfo info = mspace_mallinfo(sqlite3_get_mspace());
    struct mallinfo info = dlmallinfo();
    return (Int64) info.usmblks;
#elif USE_MSPACE
    mspace space = sqlite3_get_mspace();
    if (space != 0) {
        return mspace_footprint(space);
    } else {
        return 0;
    }
#else
    return 0;
#endif
}

Int64 SQLiteDebug::GetHeapAllocatedSize()
{
#if !NO_MALLINFO
    struct mallinfo info = mspace_mallinfo(sqlite3_get_mspace());
    return (Int64) info.uordblks;
#else
    return sqlite3_memory_used();
#endif
}

Int64 SQLiteDebug::GetHeapFreeSize()
{
#if !NO_MALLINFO
    struct mallinfo info = mspace_mallinfo(sqlite3_get_mspace());
    return (Int64) info.fordblks;
#else
    return GetHeapSize() - sqlite3_memory_used();
#endif
}

static int read_mapinfo(FILE *fp, int *sharedPages, int *privatePages)
{
    char line[1024];
    int len;
    int skip;

    unsigned start = 0, size = 0, resident = 0;
    unsigned shared_clean = 0, shared_dirty = 0;
    unsigned private_clean = 0, private_dirty = 0;
    unsigned referenced = 0;

    int isAnon = 0;
    int isHeap = 0;

again:
    skip = 0;

    if(fgets(line, 1024, fp) == 0) return 0;

    len = strlen(line);
    if (len < 1) return 0;
    line[--len] = 0;

    /* ignore guard pages */
    if (line[18] == '-') skip = 1;

    start = strtoul(line, 0, 16);

    if (len > 50 && !strncmp(line + 49, "/tmp/sqlite-heap", strlen("/tmp/sqlite-heap"))) {
        isHeap = 1;
    }

    if (fgets(line, 1024, fp) == 0) return 0;
    if (sscanf(line, "Size: %d kB", &size) != 1) return 0;
    if (fgets(line, 1024, fp) == 0) return 0;
    if (sscanf(line, "Rss: %d kB", &resident) != 1) return 0;
    if (fgets(line, 1024, fp) == 0) return 0;
    if (sscanf(line, "Shared_Clean: %d kB", &shared_clean) != 1) return 0;
    if (fgets(line, 1024, fp) == 0) return 0;
    if (sscanf(line, "Shared_Dirty: %d kB", &shared_dirty) != 1) return 0;
    if (fgets(line, 1024, fp) == 0) return 0;
    if (sscanf(line, "Private_Clean: %d kB", &private_clean) != 1) return 0;
    if (fgets(line, 1024, fp) == 0) return 0;
    if (sscanf(line, "Private_Dirty: %d kB", &private_dirty) != 1) return 0;
    if (fgets(line, 1024, fp) == 0) return 0;
    if (sscanf(line, "Referenced: %d kB", &referenced) != 1) return 0;

    if (skip) {
        goto again;
    }

    if (isHeap) {
        *sharedPages += shared_dirty;
        *privatePages += private_dirty;
    }
    return 1;
}

static void load_maps(Int32 pid, Int32 *sharedPages, Int32 *privatePages)
{
    char tmp[128];
    FILE *fp;

    sprintf(tmp, "/proc/%d/smaps", pid);
    fp = fopen(tmp, "r");
    if (fp == 0) return;

    while (read_mapinfo(fp, sharedPages, privatePages) != 0) {
        // Do nothing
    }
    fclose(fp);
}

void SQLiteDebug::GetHeapDirtyPages(
    /* [in, out] */ ArrayOf<Int32>* pages)
{
    assert(pages != NULL && pages->GetLength() >= 2);

    Int32 _pages[2];

    _pages[0] = 0;
    _pages[1] = 0;

    load_maps(getpid(), &_pages[0], &_pages[1]);

    // Convert from kbytes to 4K pages
    _pages[0] /= 4;
    _pages[1] /= 4;

    (*pages)[0] = _pages[0];
    (*pages)[1] = _pages[1];
}

Int32 SQLiteDebug::GetNumActiveCursorsFinalized()
{
    return sNumActiveCursorsFinalized;
}

void SQLiteDebug::NotifyActiveCursorFinalized()
{
    Mutex::Autolock lock(sLock);

    sNumActiveCursorsFinalized++;
}
