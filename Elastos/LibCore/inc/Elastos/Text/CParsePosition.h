
#ifndef __CPARSEPOSITION_H__
#define __CPARSEPOSITION_H__

#include "_CParsePosition.h"

CarClass(CParsePosition)
{
public:
    CParsePosition();

    /**
     * Constructs a new {@code ParsePosition} with the specified index.
     *
     * @param index
     *            the index to begin parsing.
     */
    CARAPI constructor(
        /* [in] */ Int32 index);

    /**
     * Compares the specified object to this {@code ParsePosition} and indicates
     * if they are equal. In order to be equal, {@code object} must be an
     * instance of {@code ParsePosition} and it must have the same index and
     * error index.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if the specified object is equal to this
     *         {@code ParsePosition}; {@code false} otherwise.
     * @see #hashCode
     */
//    @Override
//    public boolean equals(Object object) {
//        if (!(object instanceof ParsePosition)) {
//            return false;
//        }
//        ParsePosition pos = (ParsePosition) object;
//        return currentPosition == pos.currentPosition
//                && errorIndex == pos.errorIndex;
//    }

    /**
     * Returns the index at which the parse could not continue.
     *
     * @return the index of the parse error or -1 if there is no error.
     */
    CARAPI GetErrorIndex(
        /* [out] */ Int32* index);

    CARAPI GetIndex(
        /* [out] */ Int32* index);

//    @Override
//    public int hashCode() {
//        return currentPosition + errorIndex;
//    }

    CARAPI SetErrorIndex(
        /* [in] */ Int32 index);

    CARAPI SetIndex(
        /* [in] */ Int32 index);

    /**
     * Returns the string representation of this parse position.
     *
     * @return the string representation of this parse position.
     */
//    @Override
//    public String toString() {
//        return getClass().getName() + "[index=" + currentPosition
//                + ", errorIndex=" + errorIndex + "]";
//    }

private:
    Int32 mCurrentPosition;
    Int32 mErrorIndex;
};

#endif // __CPARSEPOSITION_H__
