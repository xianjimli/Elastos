
#ifndef __HH_AUTOSTRINGARRAY_H
#define __HH_AUTOSTRINGARRAY_H

#include <elastos.h>

_ELASTOS_NAMESPACE_BEGIN

#define AUTOSTRINGARRAY_COMPARE(_op_)                          \
Boolean operator _op_ (const AutoStringArray& o) const {       \
    return mPtr _op_ o.mPtr;                                   \
}                                                              \
Boolean operator _op_ (const ArrayOf<String>* o) const {       \
    return mPtr _op_ o;                                        \
}

// Auto free the buffer in the String object.
class AutoStringArray
{
public:
    AutoStringArray() : mPtr(0) {}

    AutoStringArray(ArrayOf<String>* other) : mPtr(other) {}

    ~AutoStringArray()
    {
        if (mPtr) {
            Int32 len = mPtr->GetLength();
            for (Int32 i = 0; i < len; ++i) {
                String::Free((*mPtr)[i]);
            }
            ArrayOf<String>::Free(mPtr);
        }
    }

    // Assignment

    AutoStringArray& operator = (ArrayOf<String>* other)
    {
        mPtr = other;
        return *this;
    }
    AutoStringArray& operator = (const AutoStringArray& other)
    {
        mPtr = other.mPtr;
        return *this;
    }

    // Accessors

    ArrayOf<String>& operator* () const  { return *mPtr; }
    ArrayOf<String>* operator-> () const { return mPtr;  }
    ArrayOf<String>* Get() const         { return mPtr; }
    operator ArrayOf<String>*() const    { return mPtr; }

    // Operators

    AUTOSTRINGARRAY_COMPARE(==)
    AUTOSTRINGARRAY_COMPARE(!=)

private:
    ArrayOf<String>*    mPtr;
};

_ELASTOS_NAMESPACE_END

#endif // __HH_AUTOSTRINGARRAY_H

