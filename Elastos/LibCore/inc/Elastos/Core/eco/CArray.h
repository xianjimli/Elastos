
#ifndef __CARRAY_H__
#define __CARRAY_H__

#include "_CArray.h"
#include <elastos/AutoPtr.h>

CarClass(CArray)
{
public:
    CArray();

    ~CArray();

    CARAPI constructor(
        /* [in] */ Int32 size);

    CARAPI GetLength(
        /* [out] */ Int32* size);

    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ IInterface** element);

    CARAPI Put(
        /* [in] */ Int32 index,
        /* [in] */ IInterface* element);

private:
    ArrayOf< AutoPtr<IInterface> >* mElements;
};


#endif //__CARRAY_H__
