#ifndef __ATTRIBUTEDSTRING_H__
#define __ATTRIBUTEDSTRING_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <elastos/Map.h>
#include <elastos/List.h>
#include <elastos/HashSet.h>
#include <elastos/ElRefBase.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IAttributedCharacterIteratorAttribute> >
{
    size_t operator()(AutoPtr<IAttributedCharacterIteratorAttribute> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

/**
 * Holds a string with attributes describing the characters of
 * this string.
 */
class AttributedString
{
public:
    struct Range
    {
    public:
        Range(
            /* [in] */ Int32 s,
            /* [in] */ Int32 e,
            /* [in] */ IInterface* v);
    public:
        Int32 mStart;
        Int32 mEnd;
        AutoPtr<IInterface> mValue;
    };

    class AttributedIterator
            : public IAttributedCharacterIterator
            , public ElRefBase
    {
    public:
        AttributedIterator(
            /* [in] */ AttributedString* attrString);

        AttributedIterator(
            /* [in] */ AttributedString* attrString,
            /* [in] */ IObjectContainer* attributes,
            /* [in] */ Int32 begin,
            /* [in] */ Int32 end);

        ~AttributedIterator();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Clone(
            /* [out] */ IInterface** copy);

        CARAPI Current(
            /* [out] */ Char32* value);

        CARAPI First(
            /* [out] */ Char32* value);

        /**
         * Returns the begin index in the source string.
         *
         * @return the index of the first character to iterate.
         */
        CARAPI GetBeginIndex(
            /* [out] */ Int32* index);

        /**
         * Returns the end index in the source String.
         *
         * @return the index one past the last character to iterate.
         */
        CARAPI GetEndIndex(
            /* [out] */ Int32* index);

        /**
         * Returns the current index in the source String.
         *
         * @return the current index.
         */
        CARAPI GetIndex(
            /* [out] */ Int32* index);

        /**
         * Returns a set of attributes present in the {@code AttributedString}.
         * An empty set returned indicates that no attributes where defined.
         *
         * @return a set of attribute keys that may be empty.
         */
        CARAPI GetAllAttributeKeys(
            /* [out] */ IObjectContainer** allAttributedKeys);

        CARAPI GetAttribute(
            /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
            /* [out] */ IInterface** instance);

        // CARAPI GetAttributes(
        //         /* [out] */ Map<IAttributedCharacterIteratorAttribute*, IInterface*>* attributes);

        CARAPI GetRunLimit(
            /* [out] */ Int32* runLimit);

        CARAPI GetRunLimitEx(
            /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
            /* [out] */ Int32* runLimit);

        CARAPI GetRunLimitEx2(
            /* [in] */ IObjectContainer* attributes,
            /* [out] */ Int32* runLimit);

        CARAPI GetRunStart(
            /* [out] */ Int32* index);

        CARAPI GetRunStartEx(
            /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
            /* [out] */ Int32* index);

        CARAPI GetRunStartEx2(
            /* [in] */ IObjectContainer* attributes,
            /* [out] */ Int32* index);

        CARAPI Last(
            /* [out] */ Char32* lastValue);

        CARAPI Next(
            /* [out] */ Char32* nextValue);

        CARAPI Previous(
            /* [out] */ Char32* previousValue);

        CARAPI SetIndex(
            /* [in] */ Int32 location,
            /* [out] */ Char32* newChar);

    private:
        CARAPI_(Boolean) InRange(
            /* [in] */ Range* range);

        CARAPI_(Boolean) InRange(
            /* [in] */ List<Range*>* ranges);

        CARAPI_(AutoPtr<IInterface>) CurrentValue(
            /* [in] */ List<Range*>* ranges);

        CARAPI_(Int32) RunLimit(
            /* [in] */ List<Range*>* ranges);

        CARAPI_(Int32) RunStart(
            /* [in] */ List<Range*>* ranges);

    private:
        Int32 mBegin;
        Int32 mEnd;
        Int32 mOffset;

        AttributedString* mAttrString;

        HashSet<AutoPtr<IAttributedCharacterIteratorAttribute> >* mAttributesAllowed;

    };

public:
    ~AttributedString();

    /**
     * Constructs an {@code AttributedString} from an {@code
     * AttributedCharacterIterator}, which represents attributed text.
     *
     * @param iterator
     *            the {@code AttributedCharacterIterator} that contains the text
     *            for this attributed string.
     */
    CARAPI Init(
        /* [in] */ IAttributedCharacterIterator* iterator);

    /**
     * Constructs an {@code AttributedString} from a range of the text contained
     * in the specified {@code AttributedCharacterIterator}, starting at {@code
     * start}, ending at {@code end} and it will copy the attributes defined in
     * the specified set. If the set is {@code null} then all attributes are
     * copied.
     *
     * @param iterator
     *            the {@code AttributedCharacterIterator} that contains the text
     *            for this attributed string.
     * @param start
     *            the start index of the range of the copied text.
     * @param end
     *            the end index of the range of the copied text.
     * @param attributes
     *            the set of attributes that will be copied, or all if it is
     *            {@code null}.
     * @throws IllegalArgumentException
     *             if {@code start} is less than first index of
     *             {@code iterator}, {@code end} is greater than the last index +
     *             1 in {@code iterator} or if {@code start > end}.
     */
    CARAPI Init(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IObjectContainer* attributes);

    /**
     * Constructs an {@code AttributedString} from a range of the text contained
     * in the specified {@code AttributedCharacterIterator}, starting at {@code
     * start} and ending at {@code end}. All attributes will be copied to this
     * attributed string.
     *
     * @param iterator
     *            the {@code AttributedCharacterIterator} that contains the text
     *            for this attributed string.
     * @param start
     *            the start index of the range of the copied text.
     * @param end
     *            the end index of the range of the copied text.
     * @throws IllegalArgumentException
     *             if {@code start} is less than first index of
     *             {@code iterator}, {@code end} is greater than the last
     *             index + 1 in {@code iterator} or if {@code start > end}.
     */
    CARAPI Init(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Creates an {@code AttributedString} from the given text.
     *
     * @param value
     *            the text to take as base for this attributed string.
     */
    CARAPI Init(
        /* [in] */ String value);

    /**
     * Creates an {@code AttributedString} from the given text and the
     * attributes. The whole text has the given attributes applied.
     *
     * @param value
     *            the text to take as base for this attributed string.
     * @param attributes
     *            the attributes that the text is associated with.
     * @throws IllegalArgumentException
     *             if the length of {@code value} is 0 but the size of {@code
     *             attributes} is greater than 0.
     * @throws NullPointerException
     *             if {@code value} is {@code null}.
     */
//    public AttributedString(String value,
//            Map<? extends AttributedCharacterIterator.Attribute, ?> attributes) {
/*        if (value == null) {
            throw new NullPointerException();
        }
        if (value.length() == 0 && !attributes.isEmpty()) {
            throw new IllegalArgumentException("Cannot add attributes to empty string");
        }
        text = value;
        attributeMap = new HashMap<Attribute, List<Range>>(
                (attributes.size() * 4 / 3) + 1);
        Iterator<?> it = attributes.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<?, ?> entry = (Map.Entry<?, ?>) it.next();
            ArrayList<Range> ranges = new ArrayList<Range>(1);
            ranges.add(new Range(0, text.length(), entry.getValue()));
            attributeMap.put((AttributedCharacterIterator.Attribute) entry
                    .getKey(), ranges);
        }
    }*/

    /**
     * Applies a given attribute to this string.
     *
     * @param attribute
     *            the attribute that will be applied to this string.
     * @param value
     *            the value of the attribute that will be applied to this
     *            string.
     * @throws IllegalArgumentException
     *             if the length of this attributed string is 0.
     * @throws NullPointerException
     *             if {@code attribute} is {@code null}.
     */
    virtual CARAPI AddAttribute(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ IInterface* value);

    /**
     * Applies a given attribute to the given range of this string.
     *
     * @param attribute
     *            the attribute that will be applied to this string.
     * @param value
     *            the value of the attribute that will be applied to this
     *            string.
     * @param start
     *            the start of the range where the attribute will be applied.
     * @param end
     *            the end of the range where the attribute will be applied.
     * @throws IllegalArgumentException
     *             if {@code start < 0}, {@code end} is greater than the length
     *             of this string, or if {@code start >= end}.
     * @throws NullPointerException
     *             if {@code attribute} is {@code null}.
     */
    virtual CARAPI AddAttributeEx(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ IInterface* value,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Applies a given set of attributes to the given range of the string.
     *
     * @param attributes
     *            the set of attributes that will be applied to this string.
     * @param start
     *            the start of the range where the attribute will be applied.
     * @param end
     *            the end of the range where the attribute will be applied.
     * @throws IllegalArgumentException
     *             if {@code start < 0}, {@code end} is greater than the length
     *             of this string, or if {@code start >= end}.
     */
/*    public void addAttributes(
            Map<? extends AttributedCharacterIterator.Attribute, ?> attributes,
            int start, int end) {
        Iterator<?> it = attributes.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<?, ?> entry = (Map.Entry<?, ?>) it.next();
            addAttribute(
                    (AttributedCharacterIterator.Attribute) entry.getKey(),
                    entry.getValue(), start, end);
        }
    }*/

    /**
     * Returns an {@code AttributedCharacterIterator} that gives access to the
     * complete content of this attributed string.
     *
     * @return the newly created {@code AttributedCharacterIterator}.
     */
    virtual CARAPI GetIterator(
        /* [out] */ IAttributedCharacterIterator** iterator);

    /**
     * Returns an {@code AttributedCharacterIterator} that gives access to the
     * complete content of this attributed string. Only attributes contained in
     * {@code attributes} are available from this iterator if they are defined
     * for this text.
     *
     * @param attributes
     *            the array containing attributes that will be in the new
     *            iterator if they are defined for this text.
     * @return the newly created {@code AttributedCharacterIterator}.
     */
    virtual CARAPI GetIteratorEx(
        /* [in] */ IObjectContainer* attributes,
        /* [out] */ IAttributedCharacterIterator** iterator);

    /**
     * Returns an {@code AttributedCharacterIterator} that gives access to the
     * contents of this attributed string starting at index {@code start} up to
     * index {@code end}. Only attributes contained in {@code attributes} are
     * available from this iterator if they are defined for this text.
     *
     * @param attributes
     *            the array containing attributes that will be in the new
     *            iterator if they are defined for this text.
     * @param start
     *            the start index of the iterator on the underlying text.
     * @param end
     *            the end index of the iterator on the underlying text.
     * @return the newly created {@code AttributedCharacterIterator}.
     */
    virtual CARAPI GetIteratorEx2(
        /* [in] */ IObjectContainer* attributes,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IAttributedCharacterIterator** iterator);

public:
    String mText;

    Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >* mAttributeMap;
};
#endif //__ATTRIBUTEDSTRING_H__