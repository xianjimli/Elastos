#include "database/sqlite/SQLiteContentHelper.h"

ECode SQLiteContentHelper::GetBlobColumnAsAssetFile(
    /*[in]*/ ISQLiteDatabase* db,
    /*[in]*/ const String& sql,
    /*[in]*/ ArrayOf<String>* selectionArgs,
    /*[out]*/ IAssetFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);

//    try {
    MemoryFile* file = SimpleQueryForBlobMemoryFile(db, sql, selectionArgs);
    if (file == NULL) {
        // throw new FileNotFoundException("No results.");
        *fd = NULL;
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    ECode ec; // = AssetFileDescriptor.fromMemoryFile(file, fd);
    if (ec == (ECode)E_IO_EXCEPTION) {
        ec = E_FILE_NOT_FOUND_EXCEPTION;
    }
    return ec;

//    } catch (IOException ex) {
//        throw new FileNotFoundException(ex.toString());
//    }
}

MemoryFile* SQLiteContentHelper::SimpleQueryForBlobMemoryFile(
    /*[in]*/ ISQLiteDatabase* db,
    /*[in]*/ const String& sql,
    /*[in]*/ ArrayOf<String>* selectionArgs)
{
    AutoPtr<ICursor> cursor;
    db->RawQuery(sql, selectionArgs, (ICursor**)&cursor);
    if (cursor == NULL) {
        return NULL;
    }
//    try {
    Boolean succeeded;
    if (cursor->MoveToFirst(&succeeded), !succeeded) {
        cursor->Close();
        return NULL;
    }
    ArrayOf<Byte>* bytes = NULL;
    cursor->GetBlob(0, &bytes);
    if (bytes == NULL) {
        cursor->Close();
        return NULL;
    }
    MemoryFile* file = new MemoryFile(NULL, bytes->GetLength());
    file->WriteBytes(*bytes, 0, 0, bytes->GetLength());
    file->Deactivate();
    cursor->Close();
    ArrayOf<Byte>::Free(bytes);
    return file;
//    } finally {
//        cursor.close();
//    }
}
