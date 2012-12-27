
#include "cmdef.h"
#include "NIOAccess.h"
#include "Buffer.h"

Int64 NIOAccess::GetBasePointer(
    /* [in] */ IBuffer* b)
{
    // if (b instanceof DirectBuffer) {
    //     PlatformAddress address = ((DirectBuffer) b).getEffectiveAddress();
    //     if (address == null) {
    //         return 0L;
    //     }
    //     return address.toInt() + (b.position() << b._elementSizeShift);
    // }
    // return 0L;
    return -1;
}

/**
 * Returns the number of bytes remaining in the given Buffer. That is,
 * this scales <code>remaining()</code> by the byte-size of elements
 * of this Buffer.
 *
 * @param Buffer b the Buffer to be queried
 * @return the number of remaining bytes
 */
Int32 NIOAccess::GetRemainingBytes(
    /* [in] */ IBuffer* b)
{
    assert(b);
    Buffer* buf = NULL;
    if (b->Probe(EIID_Buffer) != NULL) {
        buf = (Buffer*)b->Probe(EIID_Buffer);
        return (buf->mLimit - buf->mPosition) << buf->mElementSizeShift;
    }
    else {
        return -1;
    }
}

/**
 * Returns the underlying Java array containing the data of the
 * given Buffer, or null if the Buffer is not backed by a Java array.
 *
 * @param Buffer b the Buffer to be queried
 * @return the Java array containing the Buffer's data, or null if
 * there is none
 */
//static Object getBaseArray(Buffer b);

/**
 * Returns the offset in bytes from the start of the underlying
 * Java array object containing the data of the given Buffer to
 * the actual start of the data. This method is only meaningful if
 * getBaseArray() returns non-null.
 *
 * @param Buffer b the Buffer to be queried
 * @return the data offset in bytes to the start of this Buffer's data
 */
Int32 NIOAccess::GetBaseArrayOffset(
    /* [in] */ IBuffer* b)
{
    assert(b);
    Buffer* buf = NULL;
    if (b->Probe(EIID_Buffer) != NULL) {
        buf = (Buffer*)b->Probe(EIID_Buffer);
        Boolean hasArray;
        b->HasArray(&hasArray);
        if (hasArray) {
            Int32 offset;
            b->ArrayOffset(&offset);
            return offset<< buf->mElementSizeShift;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}
