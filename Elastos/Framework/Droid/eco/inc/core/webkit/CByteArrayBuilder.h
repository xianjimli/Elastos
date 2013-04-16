
#ifndef __CBYTEARRAYBUILDER_H__
#define __CBYTEARRAYBUILDER_H__

#include "_CByteArrayBuilder.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoFree.h>
#include <elastos/List.h>

CarClass(CByteArrayBuilder)
{
public:
    CARAPI Append(
        /* [in] */ const ArrayOf<Byte>& array,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI GetFirstChunk(
        /* [out] */ IByteArrayBuilderChunk** chunk);

    CARAPI IsEmpty(
        /* [out] */ Boolean* flag);

    CARAPI GetByteSize(
        /* [out] */ Int32* size);

    CARAPI Clear();

    CARAPI constructor();

public:
    class Chunk : public IByteArrayBuilderChunk, ElRefBase
    {
    public:
        Chunk(
            /* [in] */ Int32 length);

        /**
         * Release the chunk and make it available for reuse.
         */
        virtual CARAPI ChunkRelease();

        CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        ECode GetInterfaceID(
            /* [in] */ IInterface* Object,
            /* [out] */ InterfaceID* IID);

    public:
        AutoFree<ArrayOf<Byte> >     mArray;
        Int32     mLength;
    };

private:
    // Must be called with lock held on sPool.
    CARAPI_(void) ProcessPoolLocked();

    CARAPI_(AutoPtr<Chunk>) ObtainChunk(
        /* [in] */ Int32 length);

private:
    static const Int32 DEFAULT_CAPACITY = 8192;

    // Global pool of chunks to be used by other ByteArrayBuilders.
    static /*const*/ List<AutoPtr<Chunk> > sPool;

    // Reference queue for processing gc'd entries.
//    static const Vector<AutoPtr<Chunk> > sQueue;

    List<AutoPtr<Chunk> > mChunks;
};

#endif // __CBYTEARRAYBUILDER_H__
