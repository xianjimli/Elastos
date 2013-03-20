#ifndef __ATTRIBUTEDCHARACTERITERATOR_ATTRIBUTE_H__
#define __ATTRIBUTEDCHARACTERITERATOR_ATTRIBUTE_H__

#include <elastos.h>
#include "Elastos.Text_server.h"
#include <elastos/AutoPtr.h>
using namespace Elastos;

class AttributedCharacterIterator_Attribute {
public:
    //CARAPI_(void) Init();

    /**
     * Compares this attribute with the specified object. Checks if both
     * objects are the same instance. It is defined final so all subclasses
     * have the same behavior for this method.
     *
     * @param object
     *            the object to compare against.
     * @return {@code true} if the object passed is equal to this instance;
     *         {@code false} otherwise.
     */
    //@Override
    //public final boolean equals(Object object) {
    //    return this == object;
    //}

    /**
     * Calculates the hash code for objects of type {@code Attribute}. It
     * is defined final so all sub types calculate their hash code
     * identically.
     *
     * @return the hash code for this instance of {@code Attribute}.
     */
    //@Override
    //public final int hashCode() {
    //    return super.hashCode();
    //}

    /**
     * Returns the name of the class followed by a "(", the name of the
     * attribute, and a ")".
     *
     * @return the string representing this instance.
     */
    //@Override
    //public String toString() {
    //    return getClass().getName() + '(' + getName() + ')';
    //}
protected:
    /**
     * The constructor for an {@code Attribute} with the name passed.
     *
     * @param name
     *            the name of the new {@code Attribute}.
     */
    CARAPI_(void) Init(
        /* [in] */ String name);

    /**
     * Returns the name of this attribute.
     *
     * @return the name of this attribute.
     */
    virtual CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Resolves a deserialized instance to the correct constant attribute.
     *
     * @return the {@code Attribute} this instance represents.
     * @throws InvalidObjectException
     *             if this instance is not of type {@code Attribute.class}
     *             or if it is not a known {@code Attribute}.
     */
    virtual CARAPI ReadResolve(
        /* [out] */ IInterface** resolve);

public:
    /**
     * This attribute marks segments from an input method. Most input
     * methods create these segments for words.
     *
     * The value objects are of the type {@code Annotation} which contain
     * {@code null}.
     */
    const static AutoPtr<IAttributedCharacterIterator_Attribute> INPUT_METHOD_SEGMENT;

    /**
     * The attribute describing the language of a character. The value
     * objects are of type {@code Locale} or a subtype of it.
     */
    const static AutoPtr<IAttributedCharacterIterator_Attribute> LANGUAGE;
    /**
     * For languages that have different reading directions of text (like
     * Japanese), this attribute allows to define which reading should be
     * used. The value objects are of type {@code Annotation} which
     * contain a {@code String}.
     */
    const static AutoPtr<IAttributedCharacterIterator_Attribute> READING;

private:
    //const static Int64 serialVersionUID = -9142742483513960612L;

    String mName;
};
#endif //__ATTRIBUTEDCHARACTERITERATOR_ATTRIBUTE_H__