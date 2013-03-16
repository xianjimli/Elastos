
#ifndef __CBYTEARRAYBUILDER_H__
#define __CBYTEARRAYBUILDER_H__

#include "_CByteArrayBuilder.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>
#include <elastos/ElRefBase.h>

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
    class Chunk : //public ElRefBase, 
                  public IByteArrayBuilderChunk
    {
    public:
        Chunk(
            /* [in] */ Int32 length);

        /**
         * Release the chunk and make it available for reuse.
         */
        virtual CARAPI ChunkRelease();

    public:
        Byte*     mArray;
        Int32     mLength;
    };

private:
    // Must be called with lock held on sPool.
    CARAPI_(void) ProcessPoolLocked();

    CARAPI_(Chunk*) ObtainChunk(
        /* [in] */ Int32 length);

private:
    static const Int32 DEFAULT_CAPACITY = 8192;

    // Global pool of chunks to be used by other ByteArrayBuilders.
    static const Vector< AutoPtr<Chunk> > sPool;

    // Reference queue for processing gc'd entries.
    static const Vector<AutoPtr<Chunk> > sQueue;

    Vector<AutoPtr<Chunk> > mChunks;
};

#endif // __CBYTEARRAYBUILDER_H__
