
#ifndef __HH_ELASTOA_NIO_UTILS_H
#define __HH_ELASTOA_NIO_UTILS_H

namespace Elastos {

class AutoBufferPointer {
public:
    AutoBufferPointer(
        /* [in] */ IBuffer* nioBuffer) {}

    ~AutoBufferPointer() {}

    void* Pointer() const { return mPointer; }

private:
    void*   mPointer;
//	    jarray  fArray;
    Int32    mRemaining;
};

}  /* namespace Elastos */

#endif // __HH_ELASTOA_NIO_UTILS_H
