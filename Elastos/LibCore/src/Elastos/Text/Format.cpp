
#include "cmdef.h"
#include "Format.h"
#include "CFieldPosition.h"
#include "CParsePosition.h"
#include "CAttributedString.h"


Format::Format()
{}

ECode Format::FormatObject(
    /* [in] */ IInterface* object,
    /* [out] */ String* value)
{
    AutoPtr<IFieldPosition> field;
    FAIL_RETURN(CFieldPosition::New(0, (IFieldPosition**)&field));
    return FormatObjectEx(object, String(NULL), field, value);
}

ECode Format::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** characterIterator)
{
    String value;
    FormatObject(object, &value);
    AutoPtr<IAttributedString> as;
    CAttributedString::New(value, (IAttributedString**)&as);
    as->GetIterator(characterIterator);
    return NOERROR;
}

ECode Format::ParseObject(
        /* [in] */ const String& string,
        /* [out] */ IInterface** object)
{
    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    FAIL_RETURN(ParseObjectEx(string, position, object));
    Int32 index;
    position->GetIndex(&index);
    if (index == 0) {
        //throw new ParseException("parse failure", position.getErrorIndex());
        return E_PARSE_EXCEPTION;
    }
    return NOERROR;
}

ECode Format::UpTo(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [in] */ StringBuffer& buffer,
    /* [in] */ Char32 stop,
    /* [out] */ Boolean* succeeded)
{
    Int32 index, length;
    position->GetIndex(&index);
    length = string.GetCharCount();
    Boolean lastQuote = FALSE, quote = FALSE;
    while (index < length) {
        Char32 ch = string.GetChar(index++);
        if (ch == '\'') {
            if (lastQuote) {
                buffer += '\'';
            }
            quote = !quote;
            lastQuote = TRUE;
        }
        else if (ch == stop && !quote) {
            position->SetIndex(index);
            *succeeded = TRUE;
            return NOERROR;
        }
        else {
            lastQuote = FALSE;
            buffer += ch;
        }
    }
    position->SetIndex(index);
    *succeeded = FALSE;
    return NOERROR;
}

ECode Format::UpToWithQuotes(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [in] */ StringBuffer& buffer,
    /* [in] */ Char32 stop,
    /* [in] */ Char32 start,
    /* [out] */ Boolean* succeeded)
{
    Int32 index, length, count = 1;
    position->GetIndex(&index);
    length = string.GetCharCount();
    Boolean quote = FALSE;
    while (index < length) {
        Char32 ch = string.GetChar(index++);
        if (ch == '\'') {
            quote = !quote;
        }
        if (!quote) {
            if (ch == stop) {
                count--;
            }
            if (count == 0) {
                position->SetIndex(index);
                *succeeded = TRUE;
                return NOERROR;
            }
            if (ch == start) {
                count++;
            }
        }
        buffer += ch;
    }
    //throw new IllegalArgumentException("Unmatched braces in the pattern");
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}
