#ifndef __MESSAGEFORMAT_H__
#define __MESSAGEFORMAT_H__
#include "cmdef.h"
#include <elastos.h>
#include <StringBuffer.h>
#include <elastos/Vector.h>
#include <Character.h>

#include "Elastos.Text_server.h"
#include "Format.h"
#include "CParsePosition.h"
#include "CFieldPosition.h"
#include "NumberFormat.h"
#include "DateFormat.h"
#include "CAttributedString.h"
#include "CDecimalFormatSymbols.h"
#include "CDecimalFormat.h"
#include "CSimpleDateFormat.h"
#include "CChoiceFormat.h"


using namespace Elastos;

class MessageFormat : public Format {
public:
    /**
     * The instances of this inner class are used as attribute keys in
     * {@code AttributedCharacterIterator} that the
     * {@link MessageFormat#formatToCharacterIterator(Object)} method returns.
     * <p>
     * There is no public constructor in this class, the only instances are the
     * constants defined here.
     */
    /* static */ class MessageFormatField : public Format::Field {
        protected:
        /**
         * Constructs a new instance of {@code MessageFormat.Field} with the
         * given field name.
         *
         * @param fieldName
         *            the field name.
         */
        CARAPI Init(
            /* [in] */ String fieldName);
        //{
        //    super(fieldName);
        //}

        /**
         * Resolves instances that are deserialized to the constant
         * {@code MessageFormat.Field} values.
         *
         * @return the resolved field object.
         * @throws InvalidObjectException
         *             if an error occurs while resolving the field object.
         */
        //@Override
        CARAPI ReadResolve(
            /* [out] */ IInterface** object);

        public:
        /**
         * This constant stands for the message argument.
         */
        //public static final Field ARGUMENT = new Field("message argument field");
        const static AutoPtr<IMessageFormatField> ARGUMENT;

        //private:
        //const static Int64 serialVersionUID = 7899943957617360810L;
    };

private:
    /**
     * An inner class to store attributes, values, start and end indices.
     * Instances of this inner class are used as elements for the fields vector
     */
    /*static*/ class FieldContainer {

    public:

        FieldContainer(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
            /* [in] */ IInterface* value);

    public:

        Int32 mStart, mEnd;

        AutoPtr<IAttributedCharacterIteratorAttribute> mAttribute;

        AutoPtr<IInterface> mValue;
    };

public:

    virtual ~MessageFormat();
    /**
     * Constructs a new {@code MessageFormat} using the specified pattern and {@code locale}.
     *
     * @param template
     *            the pattern.
     * @param locale
     *            the locale.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    CARAPI_(void) Init(
        /* [in] */ String tem,
        /* [in] */ ILocale* locale);

    /**
     * Constructs a new {@code MessageFormat} using the specified pattern and
     * the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     *
     * @param template
     *            the pattern.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    CARAPI_(void) Init(
        /* [in] */ String tem);

    /**
     * Changes this {@code MessageFormat} to use the specified pattern.
     *
     * @param template
     *            the new pattern.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
    virtual CARAPI ApplyPattern(
        /* [in] */ String tem);

    /**
     * Returns a new instance of {@code MessageFormat} with the same pattern and
     * formats as this {@code MessageFormat}.
     *
     * @return a shallow copy of this {@code MessageFormat}.
     * @see java.lang.Cloneable
     */
//    @Override
//    public Object clone() {
//        MessageFormat clone = (MessageFormat) super.clone();
//        Format[] array = new Format[formats.length];
//        for (int i = formats.length; --i >= 0;) {
//            if (formats[i] != null) {
//                array[i] = (Format) formats[i].clone();
//            }
//        }
//        clone.formats = array;
//        return clone;
//    }

    /**
     * Compares the specified object to this {@code MessageFormat} and indicates
     * if they are equal. In order to be equal, {@code object} must be an
     * instance of {@code MessageFormat} and have the same pattern.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if the specified object is equal to this
     *         {@code MessageFormat}; {@code false} otherwise.
     * @see #hashCode
     */
//    @Override
//    public boolean equals(Object object) {
//        if (this == object) {
//            return true;
//        }
//        if (!(object instanceof MessageFormat)) {
//            return false;
//        }
//        MessageFormat format = (MessageFormat) object;
//        if (maxOffset != format.maxOffset) {
//            return false;
//        }
        // Must use a loop since the lengths may be different due
        // to serialization cross-loading
//        for (int i = 0; i <= maxOffset; i++) {
//            if (argumentNumbers[i] != format.argumentNumbers[i]) {
//                return false;
//            }
//        }
//        return locale.equals(format.locale)
//                && Arrays.equals(strings, format.strings)
//                && Arrays.equals(formats, format.formats);
//    }

    /**
     * Formats the specified object using the rules of this message format and
     * returns an {@code AttributedCharacterIterator} with the formatted message and
     * attributes. The {@code AttributedCharacterIterator} returned also includes the
     * attributes from the formats of this message format.
     *
     * @param object
     *            the object to format.
     * @return an {@code AttributedCharacterIterator} with the formatted message and
     *         attributes.
     * @throws IllegalArgumentException
     *            if the arguments in the object array cannot be formatted
     *            by this message format.
     */
    //@Override
    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** characterIterator);

    /**
     * Converts the specified objects into a string which it appends to the
     * specified string buffer using the pattern of this message format.
     * <p>
     * If the {@code field} member of the specified {@code FieldPosition} is
     * {@code MessageFormat.Field.ARGUMENT}, then the begin and end index of
     * this field position is set to the location of the first occurrence of a
     * message format argument. Otherwise, the {@code FieldPosition} is ignored.
     *
     * @param objects
     *            the array of objects to format.
     * @param buffer
     *            the target string buffer to append the formatted message to.
     * @param field
     *            on input: an optional alignment field; on output: the offsets
     *            of the alignment field in the formatted text.
     * @return the string buffer.
     */
    CARAPI FormatObjects(
        /* [in] */ ArrayOf< IInterface* >* objects,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* value);

    /**
     * Converts the specified objects into a string which it appends to the
     * specified string buffer using the pattern of this message format.
     * <p>
     * If the {@code field} member of the specified {@code FieldPosition} is
     * {@code MessageFormat.Field.ARGUMENT}, then the begin and end index of
     * this field position is set to the location of the first occurrence of a
     * message format argument. Otherwise, the {@code FieldPosition} is ignored.
     * <p>
     * Calling this method is equivalent to calling
     * <blockquote>
     *
     * <pre>
     * format((Object[])object, buffer, field)
     * </pre>
     *
     * </blockquote>
     *
     * @param object
     *            the object to format, must be an array of {@code Object}.
     * @param buffer
     *            the target string buffer to append the formatted message to.
     * @param field
     *            on input: an optional alignment field; on output: the offsets
     *            of the alignment field in the formatted text.
     * @return the string buffer.
     * @throws ClassCastException
     *             if {@code object} is not an array of {@code Object}.
     */
    //@Override
    CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* value);

    /**
     * Formats the supplied objects using the specified message format pattern.
     *
     * @param format the format string (see {@link java.util.Formatter#format})
     * @param args
     *            the list of arguments passed to the formatter. If there are
     *            more arguments than required by {@code format},
     *            additional arguments are ignored.
     * @return the formatted result.
     * @throws IllegalArgumentException
     *            if the pattern cannot be parsed.
     */
//    public static String format(String format, Object... args) {
//        if (args != null) {
//            for (int i = 0; i < args.length; i++) {
//                if (args[i] == null) {
//                    args[i] = "null";
//                }
//            }
//        }
//        return new MessageFormat(format).format(args);
//    }

    /**
     * Returns the {@code Format} instances used by this message format.
     *
     * @return an array of {@code Format} instances.
     */
    virtual CARAPI GetFormats(
        /* [out] */ ArrayOf< IFormat* >** arrayOfInstances);

    /**
     * Returns the formats used for each argument index. If an argument is
     * placed more than once in the pattern string, then this returns the format
     * of the last one.
     *
     * @return an array of formats, ordered by argument index.
     */
    virtual CARAPI GetFormatsByArgumentIndex(
        /* [out] */ ArrayOf< IFormat* >** arrayOfInstances);

    /**
     * Sets the format used for the argument at index {@code argIndex} to
     * {@code format}.
     *
     * @param argIndex
     *            the index of the format to set.
     * @param format
     *            the format that will be set at index {@code argIndex}.
     */
    virtual CARAPI SetFormatByArgumentIndex(
        /* [in] */ Int32 argIndex,
        /* [in] */ IFormat* format);

    /**
     * Sets the formats used for each argument. The {@code formats} array
     * elements should be in the order of the argument indices.
     *
     * @param formats
     *            the formats in an array.
     */
    virtual CARAPI SetFormatsByArgumentIndexEx(
        /* [in] */ ArrayOf< IFormat* >* formats);

    /**
     * Returns the locale used when creating formats.
     *
     * @return the locale used to create formats.
     */
    virtual CARAPI GetLocale(
        /* [out] */ ILocale** locale);

//    @Override
//    public int hashCode() {
//        int hashCode = 0;
//        for (int i = 0; i <= maxOffset; i++) {
//            hashCode += argumentNumbers[i] + strings[i].hashCode();
//            if (formats[i] != null) {
//                hashCode += formats[i].hashCode();
//            }
//        }
//        if (maxOffset + 1 < strings.length) {
//            hashCode += strings[maxOffset + 1].hashCode();
//        }
//        if (locale != null) {
//            return hashCode + locale.hashCode();
//        }
//        return hashCode;
//    }

    /**
     * Parses the message arguments from the specified string using the rules of
     * this message format.
     *
     * @param string
     *            the string to parse.
     * @return the array of {@code Object} arguments resulting from the parse.
     * @throws ParseException
     *            if an error occurs during parsing.
     */
    virtual CARAPI Parse(
        /* [in] */ String string,
        /* [out, callee] */ ArrayOf< IInterface* >** results);

    /**
     * Parses the message argument from the specified string starting at the
     * index specified by {@code position}. If the string is successfully
     * parsed then the index of the {@code ParsePosition} is updated to the
     * index following the parsed text. On error, the index is unchanged and the
     * error index of {@code ParsePosition} is set to the index where the error
     * occurred.
     *
     * @param string
     *            the string to parse.
     * @param position
     *            input/output parameter, specifies the start index in
     *            {@code string} from where to start parsing. If parsing is
     *            successful, it is updated with the index following the parsed
     *            text; on error, the index is unchanged and the error index is
     *            set to the index where the error occurred.
     * @return the array of objects resulting from the parse, or {@code null} if
     *         there is an error.
     */
    virtual CARAPI ParseEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out, callee] */ ArrayOf< IInterface* >** result);

    /**
     * Parses the message argument from the specified string starting at the
     * index specified by {@code position}. If the string is successfully
     * parsed then the index of the {@code ParsePosition} is updated to the
     * index following the parsed text. On error, the index is unchanged and the
     * error index of {@code ParsePosition} is set to the index where the error
     * occurred.
     *
     * @param string
     *            the string to parse.
     * @param position
     *            input/output parameter, specifies the start index in
     *            {@code string} from where to start parsing. If parsing is
     *            successful, it is updated with the index following the parsed
     *            text; on error, the index is unchanged and the error index is
     *            set to the index where the error occurred.
     * @return the array of objects resulting from the parse, or {@code null} if
     *         there is an error.
     */
    //@Override
    CARAPI ParseObjectEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** arrayOfObjects);

    /**
     * Sets the specified format used by this message format.
     *
     * @param offset
     *            the index of the format to change.
     * @param format
     *            the {@code Format} that replaces the old format.
     */
    virtual CARAPI SetFormat(
        /* [in] */ Int32 offset,
        /* [in] */ IFormat* format);

    /**
     * Sets the formats used by this message format.
     *
     * @param formats
     *            an array of {@code Format}.
     */
    virtual CARAPI SetFormats(
        /* [in] */ ArrayOf< IFormat* >* formats);

    /**
     * Sets the locale to use when creating {@code Format} instances. Changing
     * the locale may change the behavior of {@code applyPattern},
     * {@code toPattern}, {@code format} and {@code formatToCharacterIterator}.
     *
     * @param locale
     *            the new locale.
     */
    virtual CARAPI SetLocale(
        /* [in] */ ILocale* locale);

    /**
     * Returns the pattern of this message format.
     *
     * @return the pattern.
     */
    virtual CARAPI ToPattern(
        /* [out] */ String* value);

private:

    CARAPI FormatImpl(
        /* [in] */ ArrayOf< IInterface* >* objects,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [in] */ Vector<FieldContainer*>* fields,
        /* [out] */ StringBuffer* value);

    /**
     * Adds a new FieldContainer with MessageFormat.Field.ARGUMENT field,
     * argIndex, begin and end index to the fields vector, or sets the
     * position's begin and end index if it has MessageFormat.Field.ARGUMENT as
     * its field attribute.
     */
    CARAPI HandleArgumentField(
        /* [in] */ Int32 begin,
        /* [in] */ Int32 end,
        /* [in] */ Int32 argIndex,
        /* [in] */ IFieldPosition* position,
        /* [in] */ Vector<FieldContainer*>* fields);

    /**
     * If fields vector is not null, find and add the fields of this format to
     * the fields vector by iterating through its AttributedCharacterIterator
     *
     * @param format
     *            the format to find fields for
     * @param arg
     *            object to format
     * @param begin
     *            the index where the string this format has formatted begins
     * @param fields
     *            fields vector, each entry in this vector are of type
     *            FieldContainer.
     */
    CARAPI Handleformat(
        /* [in] */ IFormat* format,
        /* [in] */ IInterface* arg,
        /* [in] */ Int32 begin,
        /* [in] */ Vector<FieldContainer*>* fields);

    CARAPI Match(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [in] */ Boolean last,
        /* [in] */ ArrayOf<String>* tokens,
        /* [out] */ Int32* value);

    CARAPI ParseVariable(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IFormat** value);

    CARAPI DecodeDecimalFormat(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFormat* format,
        /* [out] */ String* value);

    CARAPI DecodeSimpleDateFormat(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFormat* format,
        /* [out] */ String* value);

    CARAPI AppendQuoted(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ String string);

/*    private static final ObjectStreamField[] serialPersistentFields = {
            new ObjectStreamField("argumentNumbers", int[].class),
            new ObjectStreamField("formats", Format[].class),
            new ObjectStreamField("locale", Locale.class),
            new ObjectStreamField("maxOffset", Integer.TYPE),
            new ObjectStreamField("offsets", int[].class),
            new ObjectStreamField("pattern", String.class), };

    private void writeObject(ObjectOutputStream stream) throws IOException {
        ObjectOutputStream.PutField fields = stream.putFields();
        fields.put("argumentNumbers", argumentNumbers);
        Format[] compatibleFormats = formats;
        fields.put("formats", compatibleFormats);
        fields.put("locale", locale);
        fields.put("maxOffset", maxOffset);
        int offset = 0;
        int offsetsLength = maxOffset + 1;
        int[] offsets = new int[offsetsLength];
        StringBuilder pattern = new StringBuilder();
        for (int i = 0; i <= maxOffset; i++) {
            offset += strings[i].length();
            offsets[i] = offset;
            pattern.append(strings[i]);
        }
        if (maxOffset + 1 < strings.length) {
            pattern.append(strings[maxOffset + 1]);
        }
        fields.put("offsets", offsets);
        fields.put("pattern", pattern.toString());
        stream.writeFields();
    }

    private void readObject(ObjectInputStream stream) throws IOException,
            ClassNotFoundException {
        ObjectInputStream.GetField fields = stream.readFields();
        argumentNumbers = (int[]) fields.get("argumentNumbers", null);
        formats = (Format[]) fields.get("formats", null);
        locale = (Locale) fields.get("locale", null);
        maxOffset = fields.get("maxOffset", 0);
        int[] offsets = (int[]) fields.get("offsets", null);
        String pattern = (String) fields.get("pattern", null);
        int length;
        if (maxOffset < 0) {
            length = pattern.length() > 0 ? 1 : 0;
        } else {
            length = maxOffset
                    + (offsets[maxOffset] == pattern.length() ? 1 : 2);
        }
        strings = new String[length];
        int last = 0;
        for (int i = 0; i <= maxOffset; i++) {
            strings[i] = pattern.substring(last, offsets[i]);
            last = offsets[i];
        }
        if (maxOffset + 1 < strings.length) {
            strings[strings.length - 1] = pattern.substring(last, pattern
                    .length());
        }
    }*/

private:
    AutoPtr<ILocale> mLocale;

    mutable ArrayOf<String>* mStrings;

    ArrayOf<Int32>* mArgumentNumbers;

    ArrayOf< IFormat* >* mFormats;

    Int32 mMaxOffset;

    mutable Int32 mMaxArgumentIndex;
};
#endif //__MESSAGEFORMAT_H__
