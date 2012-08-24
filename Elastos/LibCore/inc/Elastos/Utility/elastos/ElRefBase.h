
#ifndef __ELREFBASE_H__
#define __ELREFBASE_H__

#include <elastos.h>
#include <assert.h>

using namespace Elastos;

class ElRefBase
{
public:
    ElRefBase() : mRef(0) {}

    virtual ~ElRefBase() {}

    CARAPI_(UInt32) AddRef()
    {
        // atomic_inc of android bionic C library will return
        // the old value of mRef before it is increased
        Int32 ref = atomic_inc(&mRef);
        // so we should increase ref before return
        return ++ref;
    }

    CARAPI_(UInt32) Release()
    {
        // atomic_inc of android bionic C library will return
        // the old value of mRef before it is decreased
        Int32 ref = atomic_dec(&mRef);
        // so we should decrease ref
        if (--ref == 0) {
            delete this;
        }
        assert(ref >= 0);
        return ref;
    }

protected:
    Int32 mRef;
};

#endif //__ELREFBASE_H__
