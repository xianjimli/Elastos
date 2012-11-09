#include "database/sqlite/SQLiteOpenHelper.h"
#include "database/sqlite/SQLiteDatabase.h"


SQLiteOpenHelper::SQLiteOpenHelper(
    /*[in]*/ IContext* context,
    /*[in]*/ String name,
    /*[in]*/ ICursorFactory* factory,
    /*[in]*/ Int32 version)
{
    mContext = context;
    mName = name;
    mFactory = factory;
    mNewVersion = version;
}

ECode SQLiteOpenHelper::GetWritableDatabase(
    /*[out]*/ ISQLiteDatabase* database)
{
	Boolean isopen, isreadonly;
	mDatabase->IsOpen(&isopen);
	mDatabase->IsReadOnly(&isreadonly);

    if (mDatabase != NULL && isopen && !isreadonly) {
    	database = mDatabase;
        return NOERROR;  // The database is already open for business
    }

    if (mIsInitializing) {
//        throw new IllegalStateException("getWritableDatabase called recursively");
    	return !NOERROR;
    }

    // If we have a read-only database open, someone could be using it
    // (though they shouldn't), which would cause a lock to be held on
    // the file, and our attempts to open the database read-write would
    // fail waiting for the file lock.  To prevent that, we acquire the
    // lock on the read-only database, which shuts out other users.

    Boolean success = false;
    ISQLiteDatabase* db = NULL;
    if (mDatabase != NULL) mDatabase->Lock();
//    try {
        mIsInitializing = true;
        if (mName == NULL) {
            SQLiteDatabase::Create(NULL,&db);
        } else {
//            mContext->OpenOrCreateDatabase(mName, 0, mFactory,&db); ///////////////////////
        }

        Int32 version;
        db->GetVersion(&version);
        if (version != mNewVersion) {
            db->BeginTransaction();
//            try {
                if (version == 0) {
                    OnCreate(db);
                } else {
                    if (version > mNewVersion) {
              /*          Log.wtf(TAG, "Can't downgrade read-only database from version " +
                                version + " to " + mNewVersion + ": " + db.getPath());   */
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
        success = true;
        database = db;
 //       return NOERROR;
//    } finally {
        mIsInitializing = false;
        if (success) {
            if (mDatabase != NULL) {
//                try { mDatabase->close(); } catch (Exception e) { }
                mDatabase->Close();
//              mDatabase->Unlock();           ////////////////////
            }
            mDatabase = db;
        } else {
            if (mDatabase != NULL)   // mDatabase->Unlock();  ////////////////////
            if (db != NULL) db->Close();
        }
//    }
    return NOERROR;
}

ECode SQLiteOpenHelper::GetReadableDatabase(
    	/*[out]*/ ISQLiteDatabase** database)
{/*
	if (mDatabase != null && mDatabase.isOpen()) {
        return mDatabase;  // The database is already open for business
    }

    if (mIsInitializing) {
        throw new IllegalStateException("getReadableDatabase called recursively");
    }

    try {
        return getWritableDatabase();
    } catch (SQLiteException e) {
        if (mName == null) throw e;  // Can't open a temp database read-only!
        Log.e(TAG, "Couldn't open " + mName + " for writing (will try read-only):", e);
    }

    SQLiteDatabase db = null;
    try {
        mIsInitializing = true;
        String path = mContext.getDatabasePath(mName).getPath();
        db = SQLiteDatabase.openDatabase(path, mFactory, SQLiteDatabase.OPEN_READONLY);
        if (db.getVersion() != mNewVersion) {
            throw new SQLiteException("Can't upgrade read-only database from version " +
                    db.getVersion() + " to " + mNewVersion + ": " + path);
        }

        onOpen(db);
        Log.w(TAG, "Opened " + mName + " in read-only mode");
        mDatabase = db;
        return mDatabase;
    } finally {
        mIsInitializing = false;
        if (db != null && db != mDatabase) db.close();
    }
    */
    return NOERROR;
}

ECode SQLiteOpenHelper::Close()
{
	if (mIsInitializing) 
//      throw new IllegalStateException("Closed during initialization");
		return !NOERROR;
    
    Boolean isopen;
    mDatabase->IsOpen(&isopen);
    if (mDatabase != NULL && isopen) {
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