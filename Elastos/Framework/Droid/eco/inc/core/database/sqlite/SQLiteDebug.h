#ifndef __SQLITEDEBUG_H__
#define __SQLITEDEBUG_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/List.h>


using namespace Elastos::Core::Threading;

/**
 * Provides debugging info about all SQLite databases running in the current process.
 *
 * {@hide}
 */
class SQLiteDebug
{
public:
    class DbStats;

    /**
     * Contains statistics about the active pagers in the current process.
     *
     * @see #getPagerStats(PagerStats)
     */
    class PagerStats : public ElRefBase
    {
    public:
        PagerStats();

        ~PagerStats();

    public:
        /** The total number of bytes in all pagers in the current process
         * @deprecated not used any longer
         */
        Int64 mTotalBytes;
        /** The number of bytes in referenced pages in all pagers in the current process
         * @deprecated not used any longer
         * */
        Int64 mReferencedBytes;
        /** The number of bytes in all database files opened in the current process
         * @deprecated not used any longer
         */
        Int64 mDatabaseBytes;
        /** The number of pagers opened in the current process
         * @deprecated not used any longer
         */
        Int32 mNumPagers;

        /** the current amount of memory checked out by sqlite using sqlite3_malloc().
         * documented at http://www.sqlite.org/c3ref/c_status_malloc_size.html
         */
        Int32 mMemoryUsed;

        /** the number of bytes of page cache allocation which could not be sattisfied by the
         * SQLITE_CONFIG_PAGECACHE buffer and where forced to overflow to sqlite3_malloc().
         * The returned value includes allocations that overflowed because they where too large
         * (they were larger than the "sz" parameter to SQLITE_CONFIG_PAGECACHE) and allocations
         * that overflowed because no space was left in the page cache.
         * documented at http://www.sqlite.org/c3ref/c_status_malloc_size.html
         */
        Int32 mPageCacheOverflo;

        /** records the largest memory allocation request handed to sqlite3.
         * documented at http://www.sqlite.org/c3ref/c_status_malloc_size.html
         */
        Int32 mLargestMemAlloc;

        /** a list of {@link DbStats} - one for each main database opened by the applications
         * running on the android device
         */
        List< AutoPtr<DbStats> >* mDbStats;
    };

    /**
     * contains statistics about a database
     */
    class DbStats : public ElRefBase
    {
    public:
        DbStats(
            /*[in]*/ const String& dbName,
            /*[in]*/ Int64 pageCount,
            /*[in]*/ Int64 pageSize,
            /*[in]*/ Int32 lookaside);

    public:
        /** name of the database */
        String mDbName;

        /** the page size for the database */
        Int64 mPageSize;

        /** the database size */
        Int64 mDbSize;

        /** documented here http://www.sqlite.org/c3ref/c_dbstatus_lookaside_used.html */
        Int32 mLookaside;
    };

public:
    /**
     * return all pager and database stats for the current process.
     * @return {@link PagerStats}
     */
    static CARAPI_(AutoPtr<PagerStats>) GetDatabaseInfo();

    /**
     * Gathers statistics about all pagers in the current process.
     */
    static CARAPI_(void) GetPagerStats(
        /*[in]*/ PagerStats* stats);

    /**
     * Returns the size of the SQLite heap.
     * @return The size of the SQLite heap in bytes.
     */
    static CARAPI_(Int64) GetHeapSize();

    /**
     * Returns the amount of allocated memory in the SQLite heap.
     * @return The allocated size in bytes.
     */
    static CARAPI_(Int64) GetHeapAllocatedSize();

    /**
     * Returns the amount of free memory in the SQLite heap.
     * @return The freed size in bytes.
     */
    static CARAPI_(Int64) GetHeapFreeSize();

    /**
     * Determines the number of dirty belonging to the SQLite
     * heap segments of this process.  pages[0] returns the number of
     * shared pages, pages[1] returns the number of private pages
     */
    static CARAPI_(void) GetHeapDirtyPages(
        /* [in, out] */ ArrayOf<Int32>* pages);

    /**
     * Returns the number of active cursors that have been finalized. This depends on the GC having
     * run but is still useful for tests.
     */
    static CARAPI_(Int32) GetNumActiveCursorsFinalized();

    /* package */
    static CARAPI_(void) NotifyActiveCursorFinalized();

public:
    /**
     * Controls the printing of SQL statements as they are executed.
     */
    static const Boolean DEBUG_SQL_STATEMENTS;
            // = Log.isLoggable("SQLiteStatements", Log.VERBOSE);

    /**
     * Controls the printing of wall-clock time taken to execute SQL statements
     * as they are executed.
     */
    static const Boolean DEBUG_SQL_TIME;
            // = Log.isLoggable("SQLiteTime", Log.VERBOSE);

    /**
     * Controls the printing of compiled-sql-statement cache stats.
     */
    static const Boolean DEBUG_SQL_CACHE;
            // = Log.isLoggable("SQLiteCompiledSql", Log.VERBOSE);

    /*
     * Controls the stack trace reporting of active cursors being
     * finalized.
     */
    static const Boolean DEBUG_ACTIVE_CURSOR_FINALIZATION;
            // = Log.isLoggable("SQLiteCursorClosing", Log.VERBOSE);

    /**
     * Controls the tracking of time spent holding the database lock.
     */
    static const Boolean DEBUG_LOCK_TIME_TRACKING;
            // = Log.isLoggable("SQLiteLockTime", Log.VERBOSE);

    /**
     * Controls the printing of stack traces when tracking the time spent holding the database lock.
     */
    static const Boolean DEBUG_LOCK_TIME_TRACKING_STACK_TRACE;
            // = Log.isLoggable("SQLiteLockStackTrace", Log.VERBOSE);

private:
    static Int32 sNumActiveCursorsFinalized;
    static Mutex sLock;
};


#endif //__SQLITEDEBUG_H__
