
#include "cmdef.h"
#include "MappedByteBuffer.h"

MappedByteBuffer::MappedByteBuffer(
    /* [in] */ IByteBuffer* directBuffer)
    : ByteBuffer(((ByteBuffer*)directBuffer->Probe(EIID_ByteBuffer))->mCapacity)
{
//    ByteBuffer* bufCls = ;
//    ByteBuffer(bufCls->mCapacity);
    Boolean isDirect;
    directBuffer->IsDirect(&isDirect);
    assert(isDirect);
//    if (!isDirect) {
//        throw new IllegalArgumentException();
//    }
    mWrapped = (DirectByteBuffer*)(ByteBuffer*)directBuffer->Probe(EIID_ByteBuffer);
//    mMapMode = null;
}

MappedByteBuffer::MappedByteBuffer(
    /* [in]  IPlatformAddress* addr,*/
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset,
    /* [in] */ FileChannelMapMode mapMode)
    : ByteBuffer(capacity)
    , mMapMode(mapMode)
{
//    if (mMapMode == FileChannelMapMode_READ_ONLY) {
//        mWrapped = (DirectByteBuffer*)new ReadOnlyDirectByteBuffer(addr, capacity, offset);
//    }
//    else {
//        mWrapped = (DirectByteBuffer*)new ReadWriteDirectByteBuffer(addr, capacity, offset);
//    }
//    addr.autoFree();
}

ECode MappedByteBuffer::IsLoaded(
    /* [out] */ Boolean* isLoaded)
{
//    return ((MappedPlatformAddress) ((DirectBuffer) wrapped).getBaseAddress()).mmapIsLoaded();
    return E_NOT_IMPLEMENTED;
}

ECode MappedByteBuffer::Load()
{
//    ((MappedPlatformAddress) ((DirectBuffer) wrapped).getBaseAddress()).mmapLoad();
    return E_NOT_IMPLEMENTED;
}

ECode MappedByteBuffer::Force()
{
    if (mMapMode == FileChannelMapMode_READ_WRITE) {
//        ((MappedPlatformAddress) ((DirectBuffer) wrapped).getBaseAddress()).mmapFlush();
        return E_NOT_IMPLEMENTED;
    }
    return NOERROR;
}
