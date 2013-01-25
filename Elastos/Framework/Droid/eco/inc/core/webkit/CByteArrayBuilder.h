
#ifndef __CBYTEARRAYBUILDER_H__
#define __CBYTEARRAYBUILDER_H__

#include "_CByteArrayBuilder.h"

CarClass(CByteArrayBuilder)
{
public:
    CARAPI Append(
        /* [in] */ const ArrayOf<Byte> & array,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI GetFirstChunk(
        /* [out] */ IByteArrayBuilderChunk ** ppChunk);

    CARAPI IsEmpty(
        /* [out] */ Boolean * pFlag);

    CARAPI GetByteSize(
        /* [out] */ Int32 * pSize);

    CARAPI Clear();

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CBYTEARRAYBUILDER_H__
