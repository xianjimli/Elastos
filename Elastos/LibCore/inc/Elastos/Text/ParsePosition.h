#ifndef __PARSEPOSITION_H__
#define __PARSEPOSITION_H__

#include <elastos.h>

using namespace Elastos;
/**
 * Tracks the current position in a parsed string. In case of an error the error
 * index can be set to the position where the error occurred without having to
 * change the parse position.
 */
class ParsePosition {

public:
    ParsePosition();

    virtual ~ParsePosition();
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
    //@Override
    //public boolean equals(Object object) {
    //    if (!(object instanceof ParsePosition)) {
    //        return false;
    //    }
    //    ParsePosition pos = (ParsePosition) object;
    //    return currentPosition == pos.currentPosition
    //            && errorIndex == pos.errorIndex;
    //}

    /**
     * Returns the index at which the parse could not continue.
     *
     * @return the index of the parse error or -1 if there is no error.
     */
    virtual CARAPI_(Int32) GetErrorIndex();

    /**
     * Returns the current parse position.
     *
     * @return the current position.
     */
    virtual CARAPI_(Int32) GetIndex();

    //@Override
    //public int hashCode() {
    //    return currentPosition + errorIndex;
    //}

    /**
     * Sets the index at which the parse could not continue.
     *
     * @param index
     *            the index of the parse error.
     */
    virtual CARAPI SetErrorIndex(
            /* [in] */ Int32 index);
    

    /**
     * Sets the current parse position.
     *
     * @param index
     *            the current parse position.
     */
    virtual CARAPI SetIndex(
            /* [in] */ Int32 index);

    /**
     * Returns the string representation of this parse position.
     *
     * @return the string representation of this parse position.
     */
    //@Override
    //public String toString() {
    //    return getClass().getName() + "[index=" + currentPosition
    //            + ", errorIndex=" + errorIndex + "]";
    //}

protected:
    /**
     * Constructs a new {@code ParsePosition} with the specified index.
     *
     * @param index
     *            the index to begin parsing.
     */
    CARAPI_(void) Init(
        /* [in] */ Int32 index);

private:
    Int32 mCurrentPosition;
    Int32 mErrorIndex;
};
#endif //__PARSEPOSITION_H__