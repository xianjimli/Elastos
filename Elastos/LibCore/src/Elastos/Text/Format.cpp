#include "cmdef.h"
#include "Format.h"
#include <StringBuffer.h>
#include "CFieldPosition.h"
#include "CParsePosition.h"

Format::Format()
{}

ECode Format::format(
        /* [in] */ IInterface* object,
        /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    AutoPtr<IFieldPosition> field;
    CFieldPosition::New(0, (IFieldPosition**)&field);
    StringBuffer* buffer = new StringBuffer(NULL);
    StringBuffer* sb = new StringBuffer(NULL);
    formatEx(object, buffer, field, sb);
    *value = sb->Substring(0, sb->GetLength());
    return NOERROR;
}

ECode Format::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** characterIterator)
{
    //return new AttributedString(format(object)).getIterator();
    return NOERROR;
}

ECode Format::ParseObject(
        /* [in] */ String string,
        /* [out] */ IInterface** object)
{
    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    AutoPtr<IInterface> result;
    ParseObjectEx(string, (IParsePosition*)position, (IInterface**)&result);
    Int32 index;
    position->GetIndex(&index);
    if (index == 0) {
        //throw new ParseException("parse failure", position.getErrorIndex());
        return NOERROR;
    }
    object = (IInterface**)&result;
    return NOERROR;
}

Boolean Format::UpTo(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Char32 stop)
{
    Int32 index, length = string.GetLength();
    position->GetIndex(&index);
    Boolean lastQuote = FALSE, quote = FALSE;

    while (index < length) {
        //char ch = string.charAt(index++);
        Int32 i = string.GetChar(index++);
        Char32 ch = (Char32)i;
        if (ch == '\'') {
            if (lastQuote) {
                *buffer += '\'';
            }
            quote = !quote;
            lastQuote = TRUE;
        } else if (ch == stop && !quote) {
            position->SetIndex(index);
            return TRUE;
        } else {
            lastQuote = FALSE;
            *buffer += ch;
        }
    }
    position->SetIndex(index);
    return FALSE;
}

Boolean Format::UpToWithQuotes(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Char32 stop,
        /* [in] */ Char32 start)
{
    Int32 index, length = string.GetLength(), count = 1;
    position->GetIndex(&index);

    Boolean quote = FALSE;
    while (index < length) {
        Int32 i = string.GetChar(index++);
        Char32 ch = (Char32)i;
        if (ch == '\'') {
            quote = !quote;
        }
        if (!quote) {
            if (ch == stop) {
                count--;
            }
            if (count == 0) {
                position->SetIndex(index);
                return TRUE;
            }
            if (ch == start) {
                count++;
            }
        }
        *buffer += ch;
    }
    //throw new IllegalArgumentException("Unmatched braces in the pattern");
    return quote;
}

Format::Format_Field::Format_Field(
        /* [in] */ String fn)
{
    AttributedCharacterIterator_Attribute::Init(fn);
}