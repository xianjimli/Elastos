#ifndef __SQLITECLOSABLE_H__
#define __SQLITECLOSABLE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <StringBuffer.h>

using namespace Elastos::Core::Threading;

extern "C" const InterfaceID EIID_SQLiteClosable;

/**
 * An object created from a SQLiteDatabase that can be closed.
 */
class SQLiteClosable
{
public:
    SQLiteClosable();

    virtual ~SQLiteClosable();

    virtual CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid) = 0;

    virtual CARAPI AcquireReference();

    virtual CARAPI ReleaseReference();

    virtual CARAPI ReleaseReferenceFromContainer();

//protected
public:
    virtual CARAPI OnAllReferencesReleased() = 0;

    virtual CARAPI OnAllReferencesReleasedFromContainer();

private:
    CARAPI_(String) GetObjInfo();

private:
    Int32 mReferenceCount;
    Mutex mLock;
};

#endif //__SQLITECLOSABLE_H__
