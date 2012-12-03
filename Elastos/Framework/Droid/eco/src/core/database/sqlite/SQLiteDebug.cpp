#include "database/sqlite/SQLiteDebug.h"

Int32 SQLiteDebug::sNumActiveCursorsFinalized = 0;

SQLiteDebug::DbStats::DbStats(
	/*[in]*/ String dbName, 
    /*[in]*/ Int64 pageCount, 
    /*[in]*/ Int64 pageSize, 
    /*[in]*/ Int32 lookaside)
{
    this->dbName = dbName;
    this->pageSize = pageSize / 1024;
    dbSize = (pageCount * pageSize) / 1024;
    this->lookaside = lookaside;
}

ECode SQLiteDebug::GetDatabaseInfo(
	/*[out]*/ PagerStats stat)
{
    PagerStats *stats = new PagerStats();
    GetPagerStats(*stats);
 //   stats->dbStats = SQLiteDatabase.getDbStats();
    return NOERROR;
}

ECode SQLiteDebug::GetNumActiveCursorsFinalized(
    /* [out] */ Int32* num)
{
	*num = sNumActiveCursorsFinalized;
    return NOERROR;
}

ECode SQLiteDebug::NotifyActiveCursorFinalized()
{
	sNumActiveCursorsFinalized++;
    return NOERROR;
}

void SQLiteDebug::GetPagerStats(
    /*[in]*/ PagerStats stats)
{
    Int32 memoryUsed;
    Int32 pageCacheOverflo;
    Int32 largestMemAlloc;
    Int32 unused;

    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &memoryUsed, &unused, 0);
    sqlite3_status(SQLITE_STATUS_MALLOC_SIZE, &unused, &largestMemAlloc, 0);
    sqlite3_status(SQLITE_STATUS_PAGECACHE_OVERFLOW, &pageCacheOverflo, &unused, 0);
/*  env->SetIntField(statsObj, gMemoryUsedField, memoryUsed);
    env->SetIntField(statsObj, gPageCacheOverfloField, pageCacheOverflo);
    env->SetIntField(statsObj, gLargestMemAllocField, largestMemAlloc); */
}


Int64 SQLiteDebug::GetHeapSize()
{/*
#if !NO_MALLINFO
    struct mallinfo info = mspace_mallinfo(sqlite3_get_mspace());
    struct mallinfo info = dlmallinfo();
//  return (jlong) info.usmblks;
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
#endif  */
    return E_NOT_IMPLEMENTED;
}


Int64 SQLiteDebug::GetHeapAllocatedSize()
{/*
#if !NO_MALLINFO
    struct mallinfo info = mspace_mallinfo(sqlite3_get_mspace());
//  return (jlong) info.uordblks;
    return (Int64) info.uordblks;
#else
    return sqlite3_memory_used();
#endif  */

    return E_NOT_IMPLEMENTED;
}

Int64 SQLiteDebug::GetHeapFreeSize()
{/*
#if !NO_MALLINFO
    struct mallinfo info = mspace_mallinfo(sqlite3_get_mspace());
//  return (jlong) info.fordblks;
    return (Int64) info.fordblks;
#else
    return getHeapSize(env, clazz) - sqlite3_memory_used();
#endif  */

    return E_NOT_IMPLEMENTED;
}


void SQLiteDebug::getHeapDirtyPages(
    /* [out] */ ArrayOf<Int32>** pages)
{/*
    int _pages[2];

    _pages[0] = 0;
    _pages[1] = 0;

    load_maps(getpid(), &_pages[0], &_pages[1]);

    // Convert from kbytes to 4K pages
    _pages[0] /= 4;
    _pages[1] /= 4;

  env->SetIntArrayRegion(pages, 0, 2, _pages);   */

}