#ifndef __MAPPEDBYTEBUFFER_H__
#define __MAPPEDBYTEBUFFER_H__

#include "ByteBuffer.h"
#include "DirectByteBuffer.h"

/**
 * {@code MappedByteBuffer} is a special kind of direct byte buffer which maps a
 * region of file to memory.
 * <p>
 * {@code MappedByteBuffer} can be created by calling
 * {@link java.nio.channels.FileChannel#map(java.nio.channels.FileChannel.MapMode, long, long) FileChannel.map}.
 * Once created, the mapping between the byte buffer and the file region remains
 * valid until the byte buffer is garbage collected.
 * <p>
 * All or part of a {@code MappedByteBuffer}'s content may change or become
 * inaccessible at any time, since the mapped file region can be modified by
 * another thread or process at any time. If this happens, the behavior of the
 * {@code MappedByteBuffer} is undefined.
 */
class MappedByteBuffer : public ByteBuffer//implements DirectBuffer
{
public:
    MappedByteBuffer(
        /* [in] */ IByteBuffer* directBuffer);

    MappedByteBuffer(
        /* [in]  IPlatformAddress* addr,*/
        /* [in] */ Int32 capacity,
        /* [in] */ Int32 offset,
        /* [in] */ FileChannelMapMode mapMode);

    CARAPI IsLoaded(
        /* [out] */ Boolean* isLoaded);

    CARAPI Load();

    CARAPI Force();

    virtual CARAPI Equals(
        /* [in]  */ IInterface* other,
        /* [out] */ Boolean* isEquals);

public:
    DirectByteBuffer* mWrapped;

private:
    FileChannelMapMode mMapMode;
};

#endif // __MAPPEDBYTEBUFFER_H__
