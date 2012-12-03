#include "database/sqlite/SQLiteContentHelper.h"

SQLiteContentHelper::GetBlobColumnAsAssetFile(
    /*[in]*/ ISQLiteDatabase* db, 
    /*[in]*/ String sql,
    /*[in]*/ ArrayOf<String> *selectionArgs,
    /*[out]*/ IAssetFileDescriptor** fd)
{/*
    try {
        MemoryFile file = simpleQueryForBlobMemoryFile(db, sql, selectionArgs);
        if (file == null) {
            throw new FileNotFoundException("No results.");
        }
        return AssetFileDescriptor.fromMemoryFile(file);
    } catch (IOException ex) {
        throw new FileNotFoundException(ex.toString());
    }
*/
    return NOERROR;
}

SQLiteContentHelper::SimpleQueryForBlobMemoryFile(
    	/*[in]*/ ISQLiteDatabase* db, 
    	/*[in]*/ String sql,
        /*[in]*/ ArrayOf<String> selectionArgs)
 //       /*[out]*/ IMemoryFile** mf)
{/*
    Cursor cursor = db.rawQuery(sql, selectionArgs);
    if (cursor == null) {
        return null;
    }
    try {
        if (!cursor.moveToFirst()) {
            return null;
        }
        byte[] bytes = cursor.getBlob(0);
        if (bytes == null) {
            return null;
        }
        MemoryFile file = new MemoryFile(null, bytes.length);
        file.writeBytes(bytes, 0, 0, bytes.length);
        file.deactivate();
        return file;
    } finally {
        cursor.close();
    }
*/
    return NOERROR;
}