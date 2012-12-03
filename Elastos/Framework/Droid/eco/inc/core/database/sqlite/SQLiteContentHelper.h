#ifndef __SQLITECONTENTHELPER_H__
#define __SQLITECONTENTHELPER_H__

#include "ext/frameworkext.h"
#include "os/MemoryFile.h"

class SQLiteContentHelper
{
public:
    /**
     * Runs an SQLite query and returns an AssetFileDescriptor for the
     * blob in column 0 of the first row. If the first column does
     * not contain a blob, an unspecified exception is thrown.
     *
     * @param db Handle to a readable database.
     * @param sql SQL query, possibly with query arguments.
     * @param selectionArgs Query argument values, or {@code null} for no argument.
     * @return If no exception is thrown, a non-null AssetFileDescriptor is returned.
     * @throws FileNotFoundException If the query returns no results or the
     *         value of column 0 is NULL, or if there is an error creating the
     *         asset file descriptor.
     */
    static CARAPI GetBlobColumnAsAssetFile(
        /*[in]*/ ISQLiteDatabase* db, 
        /*[in]*/ String sql,
        /*[in]*/ ArrayOf<String> *selectionArgs,
        /*[out]*/ IAssetFileDescriptor** fd);
private:
	/**
     * Runs an SQLite query and returns a MemoryFile for the
     * blob in column 0 of the first row. If the first column does
     * not contain a blob, an unspecified exception is thrown.
     *
     * @return A memory file, or {@code null} if the query returns no results
     *         or the value column 0 is NULL.
     * @throws IOException If there is an error creating the memory file.
     */
    // TODO: make this native and use the SQLite blob API to reduce copying
    static CARAPI SimpleQueryForBlobMemoryFile(
        /*[in]*/ ISQLiteDatabase* db, 
        /*[in]*/ String sql,
        /*[in]*/ ArrayOf<String> selectionArgs);
//        /*[out]*/ IMemoryFile** mf);
};


#endif //__SQLITECONTENTHELPER_H__