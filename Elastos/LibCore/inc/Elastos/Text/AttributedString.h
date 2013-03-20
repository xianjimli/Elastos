#ifndef __ATTRIBUTEDSTRING_H__
#define __ATTRIBUTEDSTRING_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <elastos/Map.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/Set.h>
#include <elastos/HashSet.h>
#include <StringBuffer.h>
#include "ext/frameworkext.h"

extern "C" const InterfaceID EIID_Range;

using namespace Elastos;

/**
 * Holds a string with attributes describing the characters of
 * this string.
 */
class AttributedString {
public:

    class Range {
    public:
        CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);

        Range(
            /* [in] */ Int32 s,
            /* [in] */ Int32 e,
            /* [in] */ IInterface* v);
    public:
        Int32 start;
        Int32 end;
        AutoPtr<IInterface> value;
    };

    class AttributedIterator : public IAttributedCharacterIterator {
    public:
        /**
         * Returns a new {@code AttributedIterator} with the same source string,
         * begin, end, and current index as this attributed iterator.
         *
         * @return a shallow copy of this attributed iterator.
         * @see java.lang.Cloneable
         */
    /*
        @Override
        @SuppressWarnings("unchecked")
        public Object clone() {
            try {
                AttributedIterator clone = (AttributedIterator) super.clone();
                if (attributesAllowed != null) {
                    clone.attributesAllowed = (HashSet<Attribute>) attributesAllowed
                            .clone();
                }
                return clone;
            } catch (CloneNotSupportedException e) {
                throw new AssertionError(e); // android-changed
            }
        }
    */
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
            /* [out] */ Set<IAttributedCharacterIterator_Attribute* >* allAttributedKeys);

        CARAPI GetAttribute(
                /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
                /* [out] */ IInterface** instance);

        CARAPI GetAttributes(
                /* [out] */ Map<IAttributedCharacterIterator_Attribute*, IInterface*>* attributes);

        CARAPI GetRunLimit(
                /* [out] */ Int32* runLimit);

        CARAPI GetRunLimit(
                /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
                /* [out] */ Int32* runLimit);
/*
        public int getRunLimit(Set<? extends Attribute> attributes) {
            int limit = end;
            Iterator<? extends Attribute> it = attributes.iterator();
            while (it.hasNext()) {
                AttributedCharacterIterator.Attribute attribute = it.next();
                int newLimit = getRunLimit(attribute);
                if (newLimit < limit) {
                    limit = newLimit;
                }
            }
            return limit;
        }*/

        CARAPI GetRunStart(
            /* [out] */ Int32* runStart);

        CARAPI GetRunStart(
            /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
            /* [out] */ Int32* runStart);
/*
        public int getRunStart(Set<? extends Attribute> attributes) {
            int start = begin;
            Iterator<? extends Attribute> it = attributes.iterator();
            while (it.hasNext()) {
                AttributedCharacterIterator.Attribute attribute = it.next();
                int newStart = getRunStart(attribute);
                if (newStart > start) {
                    start = newStart;
                }
            }
            return start;
        }
*/
        CARAPI Last(
            /* [out] */ Char32* lastValue);

        CARAPI Next(
            /* [out] */ Char32* nextValue);

        CARAPI Previous(
            /* [out] */ Char32* previousValue);

        CARAPI SetIndex(
            /* [in] */ Int32 location,
            /* [out] */ Char32* newIndex);

    protected:
        AttributedIterator(
            /* [in] */ AttributedString* attrString);

        AttributedIterator(
            /* [in] */ AttributedString* attrString,
            /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute*>* attributes,
            /* [in] */ Int32 begin,
            /* [in] */ Int32 end);

    private:
        CARAPI InRange(
            /* [in] */ Range* range,
            /* [out] */ Boolean* result);

        CARAPI InRange(
            /* [in] */ List<Range*>* ranges,
            /* [out] */ Boolean* result);

        CARAPI CurrentValue(
            /* [in] */ List<Range*>* ranges,
            /* [out] */ IInterface** currentValue);

        CARAPI RunLimit(
            /* [in] */ List<Range*>* ranges,
            /* [out] */ Int32* limitValue);

        CARAPI RunStart(
            /* [in] */ List<Range*>* ranges,
            /* [out] */ Int32* runStart);

    private:
        Int32 begin, end, offset;

        AttributedString* attrString;

        HashSet<IAttributedCharacterIterator_Attribute*>* attributesAllowed;

    };

public:
    AttributedString();

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
        /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute*>* attributes);

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
        /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
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
        /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
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
            /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute*>* attributes,
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
            /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute*>* attributes,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IAttributedCharacterIterator** iterator);

private:
    AttributedString(
        /* [in] */ IAttributedCharacterIterator* iterator, 
        /* [in] */ Int32 start,
        /* [in] */ Int32 end, 
        /* [in] */ Set<IAttributedCharacterIterator_Attribute* >* attributes);

public:
    String text;

    Map<IAttributedCharacterIterator_Attribute*, List<Range*>* >* attributeMap;
};
#endif //__ATTRIBUTEDSTRING_H__