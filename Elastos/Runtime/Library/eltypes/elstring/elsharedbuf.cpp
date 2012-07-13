
#include <elsharedbuf.h>
#include <string.h>
#include <malloc.h>
#include <sys/atomics.h>

_ELASTOS_NAMESPACE_BEGIN


SharedBuffer* SharedBuffer::Alloc(UInt32 size)
{
    SharedBuffer* sb = static_cast<SharedBuffer *>(malloc(sizeof(SharedBuffer) + size));
    if (sb) {
        sb->mRefs = 1;
        sb->mSize = size;
    }
    return sb;
}

Int32 SharedBuffer::Dealloc(const SharedBuffer* released)
{
    if (released->mRefs != 0) return -1; // XXX: invalid operation
    free(const_cast<SharedBuffer*>(released));
    return 0;
}

SharedBuffer* SharedBuffer::Edit() const
{
    if (IsOnlyOwner()) {
        return const_cast<SharedBuffer*>(this);
    }
    SharedBuffer* sb = Alloc(mSize);
    if (sb) {
        memcpy(sb->GetData(), GetData(), GetSize());
        Release();
    }
    return sb;
}

SharedBuffer* SharedBuffer::EditResize(UInt32 newSize) const
{
    if (IsOnlyOwner()) {
        SharedBuffer* buf = const_cast<SharedBuffer*>(this);
        if (buf->mSize == newSize) return buf;
        buf = (SharedBuffer*)realloc(buf, sizeof(SharedBuffer) + newSize);
        if (buf != NULL) {
            buf->mSize = newSize;
            return buf;
        }
    }
    SharedBuffer* sb = Alloc(newSize);
    if (sb) {
        const UInt32 mySize = mSize;
        memcpy(sb->GetData(), GetData(), newSize < mySize ? newSize : mySize);
        Release();
    }
    return sb;
}

SharedBuffer* SharedBuffer::AttemptEdit() const
{
    if (IsOnlyOwner()) {
        return const_cast<SharedBuffer*>(this);
    }
    return 0;
}

SharedBuffer* SharedBuffer::Reset(UInt32 new_size) const
{
    // cheap-o-reset.
    SharedBuffer* sb = Alloc(new_size);
    if (sb) {
        Release();
    }
    return sb;
}

void SharedBuffer::Acquire() const
{
    atomic_inc(&mRefs);
}

Int32 SharedBuffer::Release(UInt32 flags) const
{
#ifdef _linux
    Int32 prev = 1;
    if (IsOnlyOwner() || ((prev = atomic_dec(&mRefs)) == 1)) {
        mRefs = 0;
        if ((flags & eKeepStorage) == 0) {
            free(const_cast<SharedBuffer*>(this));
        }
    }
    return --prev;
#else
    Int32 curr = 0;
    if (IsOnlyOwner() || ((curr = atomic_dec(&mRefs)) == 0)) {
        mRefs = 0;
        if ((flags & eKeepStorage) == 0) {
            free(const_cast<SharedBuffer*>(this));
        }
    }
    return curr;
#endif
}


_ELASTOS_NAMESPACE_END
