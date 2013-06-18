
#ifndef __ATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__
#define __ATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__

#include "Elastos.Text_server.h"
#include <elastos/AutoPtr.h>


class AttributedCharacterIteratorAttribute
{
protected:
    /**
     * The constructor for an {@code Attribute} with the name passed.
     *
     * @param name
     *            the name of the new {@code Attribute}.
     */
    CARAPI Init(
        /* [in] */ const String& name);

    virtual CARAPI GetClassID(
        /* [out] */ ClassID* clsid) = 0;

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
    const static AutoPtr<IAttributedCharacterIteratorAttribute> INPUT_METHOD_SEGMENT;

    /**
     * The attribute describing the language of a character. The value
     * objects are of type {@code Locale} or a subtype of it.
     */
    const static AutoPtr<IAttributedCharacterIteratorAttribute> LANGUAGE;
    /**
     * For languages that have different reading directions of text (like
     * Japanese), this attribute allows to define which reading should be
     * used. The value objects are of type {@code Annotation} which
     * contain a {@code String}.
     */
    const static AutoPtr<IAttributedCharacterIteratorAttribute> READING;

private:
    String mName;
};

#endif //__ATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__
