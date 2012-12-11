
#include "database/sqlite/SQLiteOpenHelper.h"


const CString SQLiteOpenHelper::TAG = "SQLiteOpenHelper";

SQLiteOpenHelper::SQLiteOpenHelper(
    /*[in]*/ IContext* context,
    /*[in]*/ const String& name,
    /*[in]*/ ICursorFactory* factory,
    /*[in]*/ Int32 version)
    : mIsInitializing(FALSE)
{
    if(version < 1) {
        assert(0);
        // throw new IllegalArgumentException("Version must be >= 1, was " + version);
    }

    mContext = context;
    mName = name;
    mFactory = factory;
    mNewVersion = version;
}

ECode SQLiteOpenHelper::GetWritableDatabase(
    /*[out]*/ ISQLiteDatabase** database)
{
    Mutex::Autolock lock(mLock);

    Boolean isOpen, isReadonly;
    if (mDatabase != NULL && (mDatabase->IsOpen(&isOpen), isOpen) &&
        (mDatabase->IsReadOnly(&isReadonly), !isReadonly)) {
        *database = (ISQLiteDatabase*)mDatabase.Get();
        (*database)->AddRef();
        return NOERROR;  // The database is already open for business
    }

    if (mIsInitializing) {
    //        throw new IllegalStateException("getWritableDatabase called recursively");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // If we have a read-only database open, someone could be using it
    // (though they shouldn't), which would cause a lock to be held on
    // the file, and our attempts to open the database read-write would
    // fail waiting for the file lock.  To prevent that, we acquire the
    // lock on the read-only database, which shuts out other users.

    Boolean success = FALSE;
    AutoPtr<ISQLiteDatabase> db;
    if (mDatabase != NULL) mDatabase->Lock();
//    try {
    mIsInitializing = true;
    if (mName == NULL) {
        SQLiteDatabase::Create(NULL, (ISQLiteDatabase**)&db);
    }
    else {
//        mContext->OpenOrCreateDatabase(mName, 0, mFactory, (ISQLiteDatabase**)&db);
    }

    Int32 version;
    db->GetVersion(&version);
    if (version != mNewVersion) {
        db->BeginTransaction();
//            try {
        if (version == 0) {
            OnCreate(db);
        }
        else {
            if (version > mNewVersion) {
                // Log.wtf(TAG, "Can't downgrade read-only database from version " +
                //         version + " to " + mNewVersion + ": " + db.getPath());
            }
            OnUpgrade(db, version, mNewVersion);
        }
        db->SetVersion(mNewVersion);
        db->SetTransactionSuccessful();
//            } finally {
        db->EndTransaction();
        }
//        }

    OnOpen(db);
    success = TRUE;
    *database = db;
    if (*database != NULL) (*database)->AddRef();
 //       return NOERROR;
//    } finally {
    mIsInitializing = FALSE;
    if (success) {
        if (mDatabase != NULL) {
//                try { mDatabase->close(); } catch (Exception e) { }
            mDatabase->Close();
            mDatabase->Unlock();
        }
        mDatabase = (SQLiteDatabase*)db.Get();
    }
    else {
        if (mDatabase != NULL) mDatabase->Unlock();
        if (db != NULL) db->Close();
    }
//    }
    return NOERROR;
}

ECode SQLiteOpenHelper::GetReadableDatabase(
    	/*[out]*/ ISQLiteDatabase** database)
{
    Mutex::Autolock lock(mLock);

    Boolean isOpen;
    if (mDatabase != NULL && (mDatabase->IsOpen(&isOpen), isOpen)) {
        *database = (ISQLiteDatabase*)mDatabase.Get();  // The database is already open for business
        (*database)->AddRef();
        return NOERROR;
    }

    if (mIsInitializing) {
        // throw new IllegalStateException("getReadableDatabase called recursively");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // try {
    ECode ec = GetWritableDatabase(database);
    if (SUCCEEDED(ec)) return NOERROR;
    // } catch (SQLiteException e) {
    //     if (mName == null) throw e;  // Can't open a temp database read-only!
    //     Log.e(TAG, "Couldn't open " + mName + " for writing (will try read-only):", e);
    // }

    AutoPtr<ISQLiteDatabase> db;
    // try {
    mIsInitializing = TRUE;
    String path;// = mContext->GetDatabasePath(mName).getPath();
    SQLiteDatabase::OpenDatabase(path, mFactory, SQLiteDatabase_OPEN_READONLY, (ISQLiteDatabase**)&db);
    Int32 version;
    db->GetVersion(&version);
    if (version != mNewVersion) {
        // throw new SQLiteException("Can't upgrade read-only database from version " +
        //         db.getVersion() + " to " + mNewVersion + ": " + path);
        mIsInitializing = FALSE;
        if (db != NULL && db.Get() != (ISQLiteDatabase*)mDatabase.Get()) db->Close();
        return E_SQLITE_EXCEPTION;
    }

    OnOpen(db);
    // Log.w(TAG, "Opened " + mName + " in read-only mode");
    mDatabase = (SQLiteDatabase*)db.Get();
    *database = (ISQLiteDatabase*)mDatabase.Get();
    (*database)->AddRef();
    // } finally {
    mIsInitializing = FALSE;
    if (db != NULL && db.Get() != (ISQLiteDatabase*)mDatabase.Get()) db->Close();
    // }

    return NOERROR;
}

ECode SQLiteOpenHelper::Close()
{
    Mutex::Autolock lock(mLock);

    if (mIsInitializing) {
//      throw new IllegalStateException("Closed during initialization");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Boolean isOpen;
    if (mDatabase != NULL && (mDatabase->IsOpen(&isOpen), isOpen)) {
        mDatabase->Close();
        mDatabase = NULL;
    }
    return NOERROR;
}

ECode SQLiteOpenHelper::OnOpen(
    /*[in]*/ ISQLiteDatabase* db)
{
    return NOERROR;
}