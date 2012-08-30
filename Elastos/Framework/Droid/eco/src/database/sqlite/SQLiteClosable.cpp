
#include "database/sqlite/SQLiteClosable.h"

PInterface SQLiteClosable::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IObservable) {
        return (ISQLiteClosable*)this;
    }

    return NULL;
}

UInt32 SQLiteClosable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SQLiteClosable::Release()
{
    return ElRefBase::Release();
}

ECode SQLiteClosable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ISQLiteClosable*)this) {
        *pIID = EIID_ISQLiteClosable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

SQLiteClosable::SQLiteClosable() : mReferenceCount(1)
{
}

SQLiteClosable::~SQLiteClosable()
{
}

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

ECode SQLiteClosable::GetObjInfo(
        /* [out] */ String* objInfo)
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
    assert(objInfo != NULL);
    StringBuffer *buff = new StringBuffer();
//    *buff += this->GetClass();
    *buff += " (";
    if ((ISQLiteClosable*)this->Probe(EIID_ISQLiteClosable)) {
        *buff += "database = ";
    } else if ((ICursorWindow*)this->Probe(EIID_ICursorWindow)) {
        *buff += "mStartPos = ";
    }
    *buff += ") ";
    *objInfo = buff->Substring(0,buff->GetLength() - 1);
    return E_NOT_IMPLEMENTED;
}