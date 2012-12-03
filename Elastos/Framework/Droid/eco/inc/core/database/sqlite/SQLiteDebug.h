#ifndef __SQLITEDEBUG_H__
#define __SQLITEDEBUG_H__

#include "ext/frameworkext.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

class SQLiteDebug
{
    class PagerStats
    {
    public:
        /** The total number of bytes in all pagers in the current process
         * @deprecated not used any longer
         */
        Int64 totalBytes;
        /** The number of bytes in referenced pages in all pagers in the current process
         * @deprecated not used any longer
         * */
        Int64 referencedBytes;
        /** The number of bytes in all database files opened in the current process
         * @deprecated not used any longer
         */
        Int64 databaseBytes;
        /** The number of pagers opened in the current process
         * @deprecated not used any longer
         */
        Int32 numPagers;

        /** the current amount of memory checked out by sqlite using sqlite3_malloc().
         * documented at http://www.sqlite.org/c3ref/c_status_malloc_size.html
         */
        Int32 memoryUsed;

        /** the number of bytes of page cache allocation which could not be sattisfied by the
         * SQLITE_CONFIG_PAGECACHE buffer and where forced to overflow to sqlite3_malloc().
         * The returned value includes allocations that overflowed because they where too large
         * (they were larger than the "sz" parameter to SQLITE_CONFIG_PAGECACHE) and allocations
         * that overflowed because no space was left in the page cache.
         * documented at http://www.sqlite.org/c3ref/c_status_malloc_size.html
         */
        Int32 pageCacheOverflo;

        /** records the largest memory allocation request handed to sqlite3.
         * documented at http://www.sqlite.org/c3ref/c_status_malloc_size.html
         */
        Int32 largestMemAlloc;

        /** a list of {@link DbStats} - one for each main database opened by the applications
         * running on the android device
         */
 //       ArrayList<DbStats> dbStats;
    };

    class DbStats
    {
    public:
        /** name of the database */
        String dbName;

        /** the page size for the database */
        Int64 pageSize;

        /** the database size */
        Int64 dbSize;

        /** documented here http://www.sqlite.org/c3ref/c_dbstatus_lookaside_used.html */
        Int32 lookaside;

        DbStats(
            /*[in]*/ String dbName, 
            /*[in]*/ Int64 pageCount, 
            /*[in]*/ Int64 pageSize, 
            /*[in]*/ Int32 lookaside);
    };
public:
    /**
     * return all pager and database stats for the current process.
     * @return {@link PagerStats}
     */
    CARAPI GetDatabaseInfo(
    	/*[out]*/ PagerStats stat);

    /**
     * Returns the number of active cursors that have been finalized. This depends on the GC having
     * run but is still useful for tests.
     */
    CARAPI GetNumActiveCursorsFinalized(
        /* [out */ Int32* num);

    CARAPI NotifyActiveCursorFinalized();
private:
    /**
     * Gathers statistics about all pagers in the current process.
     */
    CARAPI_(void) GetPagerStats(
        /*[in]*/ PagerStats stats);

    /**
     * Returns the size of the SQLite heap.
     * @return The size of the SQLite heap in bytes.
     */
    CARAPI_(Int64) GetHeapSize();

    /**
     * Returns the amount of allocated memory in the SQLite heap.
     * @return The allocated size in bytes.
     */
    CARAPI_(Int64) GetHeapAllocatedSize();

    /**
     * Returns the amount of free memory in the SQLite heap.
     * @return The freed size in bytes.
     */
    CARAPI_(Int64) GetHeapFreeSize();

    /**
     * Determines the number of dirty belonging to the SQLite
     * heap segments of this process.  pages[0] returns the number of
     * shared pages, pages[1] returns the number of private pages
     */
    CARAPI_(void) getHeapDirtyPages(
        /* [out] */ ArrayOf<Int32>** pages);



    static Int32 sNumActiveCursorsFinalized;
};


#endif //__SQLITEDEBUG_H__
