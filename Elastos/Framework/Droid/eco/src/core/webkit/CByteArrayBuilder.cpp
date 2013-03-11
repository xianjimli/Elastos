
#include "webkit/CByteArrayBuilder.h"

// Global pool of chunks to be used by other ByteArrayBuilders.
const Vector< AutoPtr<CByteArrayBuilder::Chunk> > CByteArrayBuilder::sPool;

// Reference queue for processing gc'd entries.
const Vector<AutoPtr<CByteArrayBuilder::Chunk> > CByteArrayBuilder::sQueue;

ECode CByteArrayBuilder::Append(
    /* [in] */ const ArrayOf<Byte> & array,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
#if 0
    while (length > 0)
    {
        Chunk* c = NULL;
        if (mChunks.IsEmpty())
        {
            c = ObtainChunk(length);
            mChunks.PushBack(c);
        }
        else
        {
            c = mChunks.getLast();
            if (c.mLength == c.mArray.length)
            {
                c = ObtainChunk(length);
                mChunks.PushBack(c);
            }
        }

        int amount = Math.min(length, c->mArray.length - c->mLength);
        System.arraycopy(array, offset, c->mArray, c->mLength, amount);
        c.mLength += amount;
        length -= amount;
        offset += amount;
    }
#endif
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CByteArrayBuilder::GetFirstChunk(
    /* [out] */ IByteArrayBuilderChunk ** ppChunk)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CByteArrayBuilder::IsEmpty(
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CByteArrayBuilder::GetByteSize(
    /* [out] */ Int32 * pSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CByteArrayBuilder::Clear()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CByteArrayBuilder::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}