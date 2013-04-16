
#include "webkit/CByteArrayBuilder.h"

// Global pool of chunks to be used by other ByteArrayBuilders.
/*const*/ List<AutoPtr<CByteArrayBuilder::Chunk> > CByteArrayBuilder::sPool;

// Reference queue for processing gc'd entries.
//const Vector<AutoPtr<CByteArrayBuilder::Chunk> > CByteArrayBuilder::sQueue;

ECode CByteArrayBuilder::Append(
    /* [in] */ const ArrayOf<Byte>& array,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    while (length > 0) {
        Chunk* c;
        if (mChunks.IsEmpty()) {
            c = ObtainChunk(length);
            mChunks.PushBack(c);
        } else {
            c = mChunks.GetBack();
            mChunks.PopBack();
            if (c->mLength == c->mArray->GetLength()) {
                c = ObtainChunk(length);
                mChunks.PushBack(c);
            }
        }

        Int32 len = c->mArray->GetLength() - c->mLength;
        Int32 amount = length < len ? length : len;//Math.min(length, );
//        System.arraycopy(array, offset, c.mArray, c.mLength, amount);
        c->mLength += amount;
        length -= amount;
        offset += amount;
    }

    return NOERROR;
}

ECode CByteArrayBuilder::GetFirstChunk(
    /* [out] */ IByteArrayBuilderChunk** chunk)
{
    VALIDATE_NOT_NULL(chunk);

    if (mChunks.IsEmpty()) {
        *chunk = NULL;
        return NOERROR;
    }

    *chunk = mChunks.GetFront();
    mChunks.PopFront();

    return NOERROR;
}

ECode CByteArrayBuilder::IsEmpty(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    *flag = mChunks.IsEmpty();

    return NOERROR;
}

ECode CByteArrayBuilder::GetByteSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    Int32 length = mChunks.GetSize();
    Int32 total = 0;
    for (Int32 i = 0; i < length; i++) {
        total += mChunks[i]->mLength;
    }

    *size = total;

    return NOERROR;
}

ECode CByteArrayBuilder::Clear()
{
    Chunk* c = NULL;
    GetFirstChunk((IByteArrayBuilderChunk**)&c);
    while (c != NULL) {
        c->ChunkRelease();
        GetFirstChunk((IByteArrayBuilderChunk**)&c);
    }

    return NOERROR;
}

ECode CByteArrayBuilder::constructor()
{
    return NOERROR;
}

// Must be called with lock held on sPool.
CARAPI_(void) CByteArrayBuilder::ProcessPoolLocked()
{}

CARAPI_(AutoPtr<CByteArrayBuilder::Chunk>) CByteArrayBuilder::ObtainChunk(
    /* [in] */ Int32 length)
{
    // Correct a small length.
    if (length < DEFAULT_CAPACITY) {
        length = DEFAULT_CAPACITY;
    }

    //synchronized (sPool) {
    Mutex::Autolock lock(_m_syncLock);
    // Process any queued references and remove them from the pool.
    ProcessPoolLocked();
    if (!sPool.IsEmpty()) {
        AutoPtr<Chunk> c = sPool.GetFront();
        sPool.PopFront();
        // The first item may have been queued after processPoolLocked
        // so check for null.
        if (c != NULL) {
            return c;
        }
    }

    return new Chunk(length);
}

/*********************************Chunk***********************************/
CByteArrayBuilder::Chunk::Chunk(
    /* [in] */ Int32 length)
{
//    mArray = ArrayOf<Byte>::Alloc(length);
    mLength = 0;
}

/**
 * Release the chunk and make it available for reuse.
 */
CARAPI CByteArrayBuilder::Chunk::ChunkRelease()
{
    return NOERROR;
}

PInterface CByteArrayBuilder::Chunk::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 CByteArrayBuilder::Chunk::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CByteArrayBuilder::Chunk::Release()
{
    return ElRefBase::Release();
}

ECode CByteArrayBuilder::Chunk::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* IID)
{
    return NOERROR;
}
