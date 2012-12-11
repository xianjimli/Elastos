
#include "database/sqlite/SQLiteClosable.h"


// {4AC5A917-3D9F-4FBF-9966-04C093AE8A8E}
extern "C" const InterfaceID EIID_SQLiteClosable =
        { 0x4ac5a917, 0x3d9f, 0x4fbf, { 0x99, 0x66, 0x4, 0xc0, 0x93, 0xae, 0x8a, 0x8e } };

SQLiteClosable::SQLiteClosable()
    : mReferenceCount(1)
{}

SQLiteClosable::~SQLiteClosable()
{}

ECode SQLiteClosable::OnAllReferencesReleasedFromContainer()
{
    return NOERROR;
}

ECode SQLiteClosable::AcquireReference()
{
    Mutex::Autolock lock(mLock);

    if (mReferenceCount <= 0) {
//        throw new IllegalStateException(
//                        "attempt to re-open an already-closed object: " + getObjInfo());
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mReferenceCount++;
    return NOERROR;
}

ECode SQLiteClosable::ReleaseReference()
{
    Mutex::Autolock lock(mLock);

    mReferenceCount--;
    if (mReferenceCount == 0) {
        OnAllReferencesReleased();
    }
    return NOERROR;
}

ECode SQLiteClosable::ReleaseReferenceFromContainer()
{
    Mutex::Autolock lock(mLock);

    mReferenceCount--;
    if (mReferenceCount == 0) {
        OnAllReferencesReleasedFromContainer();
    }
    return NOERROR;
}

String SQLiteClosable::GetObjInfo()
{
/*         StringBuilder buff = new StringBuilder();
        buff.append(this.getClass().getName());
        buff.append(" (");
        if (this instanceof SQLiteDatabase) {
            buff.append("database = ");
            buff.append(((SQLiteDatabase)this).getPath());
        } else if (this instanceof SQLiteProgram || this instanceof SQLiteStatement ||
                this instanceof SQLiteQuery) {
            buff.append("mSql = ");
            buff.append(((SQLiteProgram)this).mSql);
        } else if (this instanceof CursorWindow) {
            buff.append("mStartPos = ");
            buff.append(((CursorWindow)this).getStartPosition());
        }
        buff.append(") ");
        return buff.toString();
    } */

    StringBuffer buff;
//    buff.append(this.getClass().getName());
    buff += " (";
    if (this->Probe(EIID_ISQLiteDatabase) != NULL) {
        buff += "database = ";
//        buff.append(((SQLiteDatabase)this).getPath());
    }
    else if (this->Probe(EIID_ISQLiteProgram) != NULL ||
             this->Probe(EIID_ISQLiteStatement) != NULL ||
             this->Probe(EIID_ISQLiteQuery) != NULL) {
        buff += "mSql = ";
//        buff.append(((SQLiteProgram)this).mSql);
    }
    else if (this->Probe(EIID_ICursorWindow) != NULL) {
        buff += "mStartPos = ";
        Int32 position;
        ((ICursorWindow*)this->Probe(EIID_ICursorWindow))->GetStartPosition(&position);
        buff += position;
    }
    buff += ") ";

    return String(buff);
}