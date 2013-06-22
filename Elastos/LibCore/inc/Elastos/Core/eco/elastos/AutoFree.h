
#ifndef __ELASTOS_AUTOFREE_H__
#define __ELASTOS_AUTOFREE_H__

#include <elastos.h>

_ELASTOS_NAMESPACE_BEGIN

#define AUTOFREE_COMPARE(_op_)                                           \
inline Boolean operator _op_ (const AutoFree<T>& o) const {           \
    return mPtr _op_ o.mPtr;                                    \
}                                                               \
inline Boolean operator _op_ (const T* o) const {               \
    return mPtr _op_ o;                                         \
}

template <typename T>
class AutoFree
{
public:
    inline AutoFree() : mPtr(0) {}

    AutoFree(T* other) : mPtr(other) {}

    ~AutoFree() { if (mPtr) T::Free(mPtr); }

    // Copy construct

    AutoFree(const AutoFree<T>& other) : mPtr(0)
    {
        if (other != NULL) {
            mPtr = other->Assign();
        }
    }

    // Assignment

    AutoFree& operator = (T* other)
    {
    	if (mPtr == other) {
			return *this;
		}

        if (mPtr) {
            T::Free(mPtr);
        }

        mPtr = other;

        return *this;
    }

    AutoFree& operator = (const AutoFree<T>& other)
    {
    	if (mPtr == other) {
    		return *this;
    	}

        if (mPtr) {
            T::Free(mPtr);
            mPtr = 0;
        }

        if (other != NULL) {
            mPtr = other->Assign();
        }

        return *this;
    }

    // Accessors
    inline  T& operator* () const  { return *mPtr; }
    inline  T* operator-> () const { return mPtr;  }
    inline  T* Get() const         { return mPtr; }
    inline  operator T*() const { return mPtr; }

    // Operators

    AUTOFREE_COMPARE(==)
    AUTOFREE_COMPARE(!=)

private:
    T*              mPtr;
};

_ELASTOS_NAMESPACE_END

#endif //__ELASTOS_AUTOFREE_H__
