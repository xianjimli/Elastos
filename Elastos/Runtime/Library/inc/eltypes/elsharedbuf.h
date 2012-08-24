
#ifndef __ELSHAREDBUF_H__
#define __ELSHAREDBUF_H__

#include <elatypes.h>

#ifdef __cplusplus

_ELASTOS_NAMESPACE_BEGIN

class SharedBuffer
{
public:

    /* flags to use with release() */
    enum {
        eKeepStorage = 0x00000001
    };

    /*! allocate a buffer of size 'size' and acquire() it.
     *  call release() to free it.
     */
    static SharedBuffer* Alloc(UInt32 size);

    /*! free the memory associated with the SharedBuffer.
     * Fails if there are any users associated with this SharedBuffer.
     * In other words, the buffer must have been release by all its
     * users.
     */
    static Int32 Dealloc(const SharedBuffer* released);

    //! get the SharedBuffer from the data pointer
    static inline const SharedBuffer* GetSharedBuffer(const void* data);

    //! access the data for read
    inline const void* GetData() const;

    //! access the data for read/write
    inline void* GetData();

    //! get size of the buffer
    inline UInt32 GetSize() const;

    //! get back a SharedBuffer object from its data
    static inline SharedBuffer* GetBufferFromData(void* data);

    //! get back a SharedBuffer object from its data
    static inline const SharedBuffer* GetBufferFromData(const void* data);

    //! get the size of a SharedBuffer object from its data
    static inline UInt32 GetSizeFromData(const void* data);

    //! edit the buffer (get a writtable, or non-const, version of it)
    SharedBuffer* Edit() const;

    //! edit the buffer, resizing if needed
    SharedBuffer* EditResize(UInt32 size) const;

    //! like edit() but fails if a copy is required
    SharedBuffer* AttemptEdit() const;

    //! resize and edit the buffer, loose it's content.
    SharedBuffer* Reset(UInt32 size) const;

    //! acquire/release a reference on this buffer
    void Acquire() const;

    /*! release a reference on this buffer, with the option of not
     * freeing the memory associated with it if it was the last reference
     * returns the previous reference count
     */
    Int32 Release(UInt32 flags = 0) const;

    //! returns wether or not we're the only owner
    inline Boolean IsOnlyOwner() const;


private:
    inline SharedBuffer() { }
    inline ~SharedBuffer() { }
    inline SharedBuffer(const SharedBuffer&);

    // 16 bytes. must be sized to preserve correct alingment.
    mutable Int32 mRefs;
    UInt32 mSize;
    UInt32 mReserved[2];
};

// ---------------------------------------------------------------------------

const SharedBuffer* SharedBuffer::GetSharedBuffer(const void* data)
{
    return data? reinterpret_cast<const SharedBuffer *>(data) - 1 : 0;
}

const void* SharedBuffer::GetData() const
{
    return this + 1;
}

void* SharedBuffer::GetData()
{
    return this + 1;
}

UInt32 SharedBuffer::GetSize() const
{
    return mSize;
}

SharedBuffer* SharedBuffer::GetBufferFromData(void* data)
{
    return ((SharedBuffer*)data) - 1;
}

const SharedBuffer* SharedBuffer::GetBufferFromData(const void* data)
{
    return ((const SharedBuffer*)data) - 1;
}

UInt32 SharedBuffer::GetSizeFromData(const void* data)
{
    return (((const SharedBuffer*)data) - 1)->mSize;
}

Boolean SharedBuffer::IsOnlyOwner() const
{
    return (mRefs == 1);
}

_ELASTOS_NAMESPACE_END

#endif

#endif //__ELSHAREDBUF_H__
