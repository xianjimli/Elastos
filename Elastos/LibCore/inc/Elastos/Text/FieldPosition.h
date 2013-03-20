#ifndef __FIELDPOSITION_H__
#define __FIELDPOSITION_H__

#include <elastos.h>
#include <elastos/AutoPtr.h>
//#include <Elastos.Core.h>
#include "Elastos.Text_server.h"


using namespace Elastos;
/**
 * Identifies fields in formatted strings. If a {@code FieldPosition} is passed
 * to the format method with such a parameter, then the indices will be set to
 * the start and end indices of the field in the formatted string.
 * <p>
 * A {@code FieldPosition} can be created by using the integer constants in the
 * various format classes (for example {@code NumberFormat.INTEGER_FIELD}) or
 * one of the fields of type {@code Format.Field}.
 * <p>
 * If more than one field information is needed, the method
 * {@link NumberFormat#formatToCharacterIterator(Object)} should be used.
 */
class FieldPosition
{
public:
    /**
     * Compares the specified object to this field position and indicates if
     * they are equal. In order to be equal, {@code object} must be an instance
     * of {@code FieldPosition} with the same field, begin index and end index.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if the specified object is equal to this field
     *         position; {@code false} otherwise.
     * @see #hashCode
     */
    //@Override
    //public boolean equals(Object object) {
    //    if (!(object instanceof FieldPosition)) {
    //        return false;
    //    }
    //    FieldPosition pos = (FieldPosition) object;
    //    return myField == pos.myField && myAttribute == pos.myAttribute
    //            && beginIndex == pos.beginIndex && endIndex == pos.endIndex;
    //}

    /**
     * Returns the index of the beginning of the field.
     *
     * @return the first index of the field.
     */
    virtual CARAPI GetBeginIndex(
        /* [out] */ Int32* beginIndex);

    /**
     * Returns the index one past the end of the field.
     *
     * @return one past the index of the last character in the field.
     */
    virtual CARAPI GetEndIndex(
        /* [out] */ Int32* endIndex);

    /**
     * Returns the field which is being identified.
     *
     * @return the field constant.
     */
    virtual CARAPI GetField(
        /* [out] */ Int32* field);

    /**
     * Returns the attribute which is being identified.
     *
     * @return the field.
     */
    virtual CARAPI GetFieldAttribute(
        /* [out] */ IFormat_Field** fieldAttribute);

    //@Override
    //public int hashCode() {
    //    int attributeHash = (myAttribute == null) ? 0 : myAttribute.hashCode();
    //    return attributeHash + myField * 10 + beginIndex * 100 + endIndex;
    //}

    /**
     * Sets the index of the beginning of the field.
     *
     * @param index
     *            the index of the first character in the field.
     */
    virtual CARAPI SetBeginIndex(
        /* [in] */ Int32 index);

    /**
     * Sets the index of the end of the field.
     *
     * @param index
     *            one past the index of the last character in the field.
     */
    virtual CARAPI SetEndIndex(
        /* [in] */ Int32 index);

    /**
     * Returns the string representation of this field position.
     *
     * @return the string representation of this field position.
     */
    //@Override
    //public String toString() {
    //    return getClass().getName() + "[attribute=" + myAttribute + ", field="
    //            + myField + ", beginIndex=" + beginIndex + ", endIndex="
    //            + endIndex + "]";
    //}

protected:
    FieldPosition();

    virtual ~FieldPosition();

    /**
     * Constructs a new {@code FieldPosition} for the specified field.
     *
     * @param field
     *            the field to identify.
     */
    CARAPI_(void) Init(
        /* [in] */ Int32 field);

    /**
     * Constructs a new {@code FieldPosition} for the specified {@code Field}
     * attribute.
     *
     * @param attribute
     *            the field attribute to identify.
     */
    CARAPI_(void) Init(
        /* [in] */ IFormat_Field* attribute);

    /**
     * Constructs a new {@code FieldPosition} for the specified {@code Field}
     * attribute and field id.
     *
     * @param attribute
     *            the field attribute to identify.
     * @param field
     *            the field to identify.
     */
    CARAPI_(void) Init(
        /* [in] */ IFormat_Field* attribute,
        /* [in] */ Int32 field);

    virtual CARAPI Clear();

private:
    Int32 mMyField;
    Int32 mBeginIndex;
    Int32 mEndIndex;

    AutoPtr<IFormat_Field> mMyAttribute;
};
#endif //__FIELDPOSITION_H__