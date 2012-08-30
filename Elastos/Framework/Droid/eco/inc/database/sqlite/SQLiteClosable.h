#ifndef __SQLITECLOSABLE_H__
#define __SQLITECLOSABLE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <StringBuffer.h>
#include <elastos/ElRefBase.h>

using namespace Elastos::Core::Threading;
/**
 * An object created from a SQLiteDatabase that can be closed.
 */
class SQLiteClosable : public ElRefBase, public ISQLiteClosable
{
protected:
    virtual CARAPI OnAllReferencesReleased() = 0;

    virtual CARAPI OnAllReferencesReleasedFromContainer();

public:
    SQLiteClosable();

    ~SQLiteClosable();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI AcquireReference();

    virtual CARAPI ReleaseReference();

    virtual CARAPI ReleaseReferenceFromContainer();

private:
    CARAPI GetObjInfo(
        /* [out] */ String* objInfo);

private:
    Int32 mReferenceCount;
//    private Object mLock = new Object();
    Mutex mLock;
};

#endif //__SQLITECLOSABLE_H__
