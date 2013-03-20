#include "MessageFormat.h"

const AutoPtr<IMessageFormat_Field> MessageFormat::MessageFormat_Field::ARGUMENT;


MessageFormat::~MessageFormat()
{

    if (mStrings != NULL) {
        ArrayOf<String>::Free(mStrings);
    }
    if (mArgumentNumbers != NULL) {
        ArrayOf<Int32>::Free(mArgumentNumbers);
    }
    if (mFormats != NULL) {
        ArrayOf< IFormat* >::Free(mFormats);
    }
}

void MessageFormat::Init(
        /* [in] */ String tem,
        /* [in] */ ILocale* locale)
{
    mLocale = locale;
    ApplyPattern(tem);
}

void MessageFormat::Init(
        /* [in] */ String tem)
{
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    Init(tem, (ILocale*)pLocale);
}

ECode MessageFormat::ApplyPattern(
    /* [in] */ String tem)
{
    Int32 length = (Int32)(tem.GetLength());
    StringBuffer* buffer = new StringBuffer("");
    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    Vector<String>* localStrings = new Vector<String>();
    Int32 argCount = 0;
    ArrayOf<Int32>* args = ArrayOf<Int32>::Alloc(10);
    Int32 maxArg = -1;
    Vector< AutoPtr<IFormat> >* localFormats = new Vector< AutoPtr<IFormat> >();
    Int32 index;
    position->GetIndex(&index);
    while (index < length) {
        if (Format::UpTo(tem, position, buffer, '{')) {
            Int32 arg = 0;
            Int32 offset;
            position->GetIndex(&offset);
            if (offset >= length) {
                //throw new IllegalArgumentException("Invalid argument number");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            // Get argument number
            Char32 ch;
            while ((ch = tem.GetChar(offset++)) != '}' && ch != ',') {
                if (ch < '0' && ch > '9') {
                    //throw new IllegalArgumentException("Invalid argument number");
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }

                arg = arg * 10 + (ch - '0');

                if (arg < 0 || offset >= length) {
                    //throw new IllegalArgumentException("Invalid argument number");
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }
            offset--;
            position->SetIndex(offset);
            AutoPtr<IFormat> f;
            ParseVariable(tem, position, (IFormat**)&f);
            localFormats->PushBack(f);
            if (argCount >= args->GetLength()) {
                ArrayOf<Int32>* newArgs = ArrayOf<Int32>::Alloc(args->GetLength() * 2);
                //System.arraycopy(args, 0, newArgs, 0, args.length);
                for (Int32 i = 0; i < args->GetLength(); i++) {
                    (*newArgs)[i] = (*args)[i];
                }
                args = newArgs->Clone();
            }
            (*args)[argCount++] = arg;
            if (arg > maxArg) {
                maxArg = arg;
            }
        }
        localStrings->PushBack(buffer->Substring(0, buffer->GetLength()));
        //buffer.setLength(0);
        *buffer = String("");
    }
    mStrings = ArrayOf<String>::Alloc(localStrings->GetSize());
    for (Int32 i = 0; i < (Int32)(localStrings->GetSize()); i++) {
        (*mStrings)[i] = (*localStrings)[i];
    }
    mArgumentNumbers = args->Clone();
    mFormats = ArrayOf<IFormat* >::Alloc(argCount);
    for (Int32 i = 0; i < argCount; i++) {
        (*mFormats)[i] = (*localFormats)[i];
    }
    mMaxOffset = argCount - 1;
    mMaxArgumentIndex = maxArg;

    return NOERROR;
}

ECode MessageFormat::FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** characterIterator)
{
    if (object == NULL) {
        //throw new NullPointerException();
        return NOERROR;
    }

    StringBuffer* buffer = new StringBuffer("");
    Vector<FieldContainer*>* fields = new Vector<FieldContainer*>();

    // format the message, and find fields
    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    StringBuffer* sb = new StringBuffer("");
    FormatImpl((ArrayOf< IInterface* >*) object, buffer, position, fields, sb);

    // create an AttributedString with the formatted buffer
//    AttributedString as = new AttributedString(buffer.toString());

    // add MessageFormat field attributes and values to the AttributedString
//    for (int i = 0; i < fields.size(); i++) {
//        FieldContainer fc = fields.elementAt(i);
//        as.addAttribute(fc.attribute, fc.value, fc.start, fc.end);
//    }

    // return the CharacterIterator from AttributedString
//    return as.getIterator();
    return NOERROR;
}

ECode MessageFormat::formatEx2(
        /* [in] */ ArrayOf< IInterface* >* objects,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* value)
{
    return FormatImpl(objects, buffer, field, NULL, value);
}

ECode MessageFormat::FormatImpl(
        /* [in] */ ArrayOf< IInterface* >* objects,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [in] */ Vector<FieldContainer*>* fields,
        /* [out] */ StringBuffer* value)
{
    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);

    for (Int32 i = 0; i <= mMaxOffset; i++) {
        *buffer += (*mStrings)[i];
        Int32 begin = buffer->GetLength();
        AutoPtr<IInterface> arg;
        if (objects != NULL && (*mArgumentNumbers)[i] < objects->GetLength()) {
            arg = (*objects)[(*mArgumentNumbers)[i]];
        } else {
            *buffer += '{';
            *buffer += (*mArgumentNumbers)[i];
            *buffer += '}';
            HandleArgumentField(begin, buffer->GetLength(), (*mArgumentNumbers)[i],
                    position, fields);
            continue;
        }
        AutoPtr<IFormat> format = (*mFormats)[i];
        if (format == NULL || arg == NULL) {
            if (arg->Probe(EIID_INumber) != NULL) {
                NumberFormat::GetInstance((INumberFormat**)&format);
            } else if (arg->Probe(EIID_IDate) != NULL) {
                DateFormat::GetInstance((IDateFormat**)&format);
            } else {
                *buffer += arg;
                HandleArgumentField(begin, buffer->GetLength(),
                        (*mArgumentNumbers)[i], position, fields);
                continue;
            }
        }
//        if (format instanceof ChoiceFormat) {
//            String result = format.format(arg);
//            MessageFormat mf = new MessageFormat(result);
//            mf.setLocale(locale);
//            mf.format(objects, buffer, passedField);
//            handleArgumentField(begin, buffer.length(), argumentNumbers[i],
//                    position, fields);
//            handleformat(format, arg, begin, fields);
//        } else {
//            format.format(arg, buffer, passedField);
//            handleArgumentField(begin, buffer.length(), argumentNumbers[i],
//                    position, fields);
//            handleformat(format, arg, begin, fields);
//        }
    }
//    if (maxOffset + 1 < strings.length) {
//        buffer.append(strings[maxOffset + 1]);
//    }
//    return buffer;
        return NOERROR;
}

ECode MessageFormat::HandleArgumentField(
        /* [in] */ Int32 begin, 
        /* [in] */ Int32 end, 
        /* [in] */ Int32 argIndex,
        /* [in] */ IFieldPosition* position, 
        /* [in] */ Vector<FieldContainer*>* fields)
{
    if (fields != NULL) {
        FieldContainer* fc = new FieldContainer(begin, end, 
            (IAttributedCharacterIterator_Attribute*)MessageFormat_Field::ARGUMENT, 
            (IInterface*)argIndex);
        fields->PushBack(fc);
    } else {
        Int32 endIndex;
        position->GetEndIndex(&endIndex);
        AutoPtr<IFormat_Field> fa;
        position->GetFieldAttribute((IFormat_Field**)&fa);
        if (position != NULL
                && fa == MessageFormat_Field::ARGUMENT
                && endIndex == 0) {
            position->SetBeginIndex(begin);
            position->SetEndIndex(end);
        }
    }
    return NOERROR;
}

MessageFormat::FieldContainer::FieldContainer(
        /* [in] */ Int32 start, 
        /* [in] */ Int32 end,
        /* [in] */ IAttributedCharacterIterator_Attribute* attribute, 
        /* [in] */ IInterface* value)
{
    mStart = start;
    mEnd = end;
    mAttribute = attribute;
    mValue = value;
}

ECode MessageFormat::Handleformat(
        /* [in] */ IFormat* format, 
        /* [in] */ IInterface* arg, 
        /* [in] */ Int32 begin,
        /* [in] */ Vector<FieldContainer*>* fields)
{
    if (fields != NULL) {
        AutoPtr<IAttributedCharacterIterator> iterator;
        format->FormatToCharacterIterator(arg, (IAttributedCharacterIterator**)&iterator);
        Int32 index, endIndex;
        iterator->GetIndex(&index);
        iterator->GetEndIndex(&endIndex);
        while (index != endIndex) {
            Int32 start;
            iterator->GetRunStart(&start);
            Int32 end;
            iterator->GetRunLimit(&end);

//            Iterator<?> it = iterator.getAttributes().keySet().iterator();
//            while (it.hasNext()) {
//                AttributedCharacterIterator.Attribute attribute = (AttributedCharacterIterator.Attribute) it
//                        .next();
//                Object value = iterator.getAttribute(attribute);
//                fields.add(new FieldContainer(begin + start, begin + end,
//                        attribute, value));
//            }
            Char32 character;
            iterator->SetIndex(end, &character);
        }
    }
    return NOERROR;
}

ECode MessageFormat::formatEx(
        /* [in] */ IInterface* object, 
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* value)
{
    VALIDATE_NOT_NULL(value);
    return formatEx2((ArrayOf< IInterface* >*) object, buffer, field, value);
}

ECode MessageFormat::GetFormats(
        /* [out] */ ArrayOf< IFormat* >** arrayOfInstances)
{
    *arrayOfInstances = mFormats->Clone();
    return NOERROR;
}

ECode MessageFormat::GetFormatsByArgumentIndex(
        /* [out] */ ArrayOf< IFormat* >** arrayOfInstances)
{
    ArrayOf< IFormat* >* answer = ArrayOf< IFormat* >::Alloc(mMaxArgumentIndex + 1);
    for (Int32 i = 0; i < mMaxOffset + 1; i++) {
        (*answer)[(*mArgumentNumbers)[i]] = (*mFormats)[i];
    }
    *arrayOfInstances = answer->Clone();
    return NOERROR;
}

ECode MessageFormat::SetFormatByArgumentIndex(
        /* [in] */ Int32 argIndex, 
        /* [in] */ IFormat* format)
{
    for (Int32 i = 0; i < mMaxOffset + 1; i++) {
        if ((*mArgumentNumbers)[i] == argIndex) {
            (*mFormats)[i] = format;
        }
    }
    return NOERROR;
}

ECode MessageFormat::SetFormatsByArgumentIndexEx(
        /* [in] */ ArrayOf< IFormat* >* formats)
{
    for (Int32 j = 0; j < mFormats->GetLength(); j++) {
        for (Int32 i = 0; i < mMaxOffset + 1; i++) {
            if ((*mArgumentNumbers)[i] == j) {
                (*mFormats)[i] = (*mFormats)[j];
            }
        }
    }
    return NOERROR;
}

ECode MessageFormat::GetLocale(
        /* [out] */ ILocale** locale)
{
    *locale = mLocale;
    return NOERROR;
}

ECode MessageFormat::Parse(
        /* [in] */ String string,
        /* [out, callee] */ ArrayOf< IInterface* >** value)
{
    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    ArrayOf< IInterface* >* result;
    ParseEx(string, position, &result);
    Int32 index;
    position->GetIndex(&index);
    if (index == 0) {
        //throw new ParseException("Parse failure", position.getErrorIndex());
        return NOERROR;
    }
    *value = result->Clone();
    return NOERROR;
}

ECode MessageFormat::ParseEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out, callee] */ ArrayOf< IInterface* >** value)
{
    if (string.IsNull()) {
        *value = ArrayOf< IInterface* >::Alloc(0);
        return NOERROR;
    }
    AutoPtr<IParsePosition> internalPos;
    CParsePosition::New(0, (IParsePosition**)&internalPos);
    Int32 offset;
    position->GetIndex(&offset);
    ArrayOf< IInterface* >* result = ArrayOf< IInterface* >::Alloc(mMaxArgumentIndex + 1);
    for (Int32 i = 0; i <= mMaxOffset; i++) {
        String sub = (*mStrings)[i];
        String str = string.Substring(offset, (Int32)(sub.GetLength()) );
        Boolean isEqual = str.Equals(sub) ? TRUE : FALSE;
        if (!isEqual) {
            position->SetErrorIndex(offset);
            *value = NULL;
            return NOERROR;
        }
        offset += (Int32)(sub.GetLength());
        AutoPtr<IInterface> parse;
        AutoPtr<IFormat> format = (*mFormats)[i];
        if (format == NULL) {
            if (i + 1 < mStrings->GetLength()) {
                Int32 next = string.IndexOf((*mStrings)[i + 1], offset);
                if (next == -1) {
                    position->SetErrorIndex(offset);
                    *value = NULL;
                    return NOERROR;
                }
                //parse = string.Substring(offset, next);
                offset = next;
            } else {
                //parse = string.Substring(offset);
                offset = (Int32)(string.GetLength());
            }
        } else {
            internalPos->SetIndex(offset);
            format->ParseObjectEx(string, (IParsePosition*)internalPos, (IInterface**)&parse);
            Int32 errorIndex;
            internalPos->GetErrorIndex(&errorIndex);
            if (errorIndex != -1) {
                position->SetErrorIndex(offset);
                *value = NULL;
                return NOERROR;
            }
            internalPos->GetIndex(&offset);
        }
        (*result)[(*mArgumentNumbers)[i]] = parse;
    }
    if (mMaxOffset + 1 < mStrings->GetLength()) {
        String sub = (*mStrings)[mMaxOffset + 1];
        String str = string.Substring(offset, (Int32)(sub.GetLength()) );
        Boolean isEqual = str.Equals(sub) ? TRUE : FALSE;
        if (!isEqual) {
            position->SetErrorIndex(offset);
            *value = NULL;
            return NOERROR;
        }
        offset += (Int32)(sub.GetLength());
    }
    position->SetIndex(offset);
    *value = result->Clone();
    return NOERROR;
}

ECode MessageFormat::ParseObjectEx(
        /* [in] */ String string, 
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** arrayOfObjects)
{
    return ParseEx(string, position, (ArrayOf< IInterface* >**)arrayOfObjects);
}

ECode MessageFormat::Match(
        /* [in] */ String string, 
        /* [in] */ IParsePosition* position, 
        /* [in] */ Boolean last,
        /* [in] */ ArrayOf<String>* tokens,
        /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 length = string.GetLength();
    Int32 offset;
    position->GetIndex(&offset);
    Int32 token = -1;
    while (offset < length && Character::IsWhitespace( (Char32)(string.GetChar(offset)) ) ) {
        offset++;
    }
    for (Int32 i = tokens->GetLength(); --i >= 0; ) {
        String str1 = string.Substring( offset, (Int32)((*tokens)[i].GetLength()) );
        String str2 = (*tokens)[i];
        if (str1.EqualsIgnoreCase(str2)) {
            token = i;
            break;
        }
    }
    if (token == -1) {
        *value = -1;
        return NOERROR;
    }
    offset += (*tokens)[token].GetLength();
    while (offset < length && Character::IsWhitespace( (Char32)(string.GetChar(offset)) ) ) {
        offset++;
    }
    Char32 ch;
    if (offset < length
            && (( ch = (Char32)(string.GetChar(offset)) ) == '}' || (!last && ch == ','))) {
        position->SetIndex(offset + 1);
        *value = token;
        return NOERROR;
    }
    *value = -1;
    return NOERROR;
}

ECode MessageFormat::ParseVariable(
        /* [in] */ String string, 
        /* [in] */ IParsePosition* position,
        /* [out] */ IFormat** value)
{
    Int32 length = (Int32)(string.GetLength());
    Int32 offset;
    position->GetIndex(&offset);
    Char32 ch;
    if (offset >= length || ((ch = string.GetChar(offset++)) != '}' && ch != ',')) {
        //throw new IllegalArgumentException("Missing element format");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    position->SetIndex(offset);
    if (ch == '}') {
        *value = NULL;
        return NOERROR;
    }
    Int32 type;
    ArrayOf<String>* tokens1 = ArrayOf<String>::Alloc(4);
    ArrayOf<String>* tokens2 = ArrayOf<String>::Alloc(4);
    (*tokens1)[0] = String("time");
    (*tokens1)[1] = String("date");
    (*tokens1)[2] = String("number");
    (*tokens1)[3] = String("choice");
    Match(string, position, FALSE, tokens1, &type);
    if (type == -1) {
        //throw new IllegalArgumentException("Unknown element format");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ArrayOf<String>::Free(tokens1);
    StringBuffer* buffer = new StringBuffer("");
    Int32 index;
    position->GetIndex(&index);
    ch = (Char32)(string.GetChar(index - 1));

    AutoPtr<IDateFormat> v1, v2;
    switch (type) {
        case 0: // time 
        case 1: // date           
            if (ch == '}') {
                DateFormat::GetDateInstance(
                        IDateFormat_DEFAULT, (ILocale*)mLocale, (IDateFormat**)&v1);
                DateFormat::GetTimeInstance(
                        IDateFormat_DEFAULT, (ILocale*)mLocale, (IDateFormat**)&v2);
                *value = (type == 1) ? (IFormat*)v1 : (IFormat*)v2;
                return NOERROR;
            }
            (*tokens2)[0] = String("full");
            (*tokens2)[1] = String("long");
            (*tokens2)[2] = String("medium");
            (*tokens2)[3] = String("short");
            Int32 dateStyle;
            Match(string, position, TRUE, tokens2, &dateStyle);
            if (dateStyle == -1) {
                Format::UpToWithQuotes(string, position, buffer, '}', '{');
//                return new SimpleDateFormat(buffer.toString(), locale);
                return NOERROR;
            }
            switch (dateStyle) {
                case 0:
                    dateStyle = IDateFormat_FULL;
                    break;
                case 1:
                    dateStyle = IDateFormat_LONG;
                    break;
                case 2:
                    dateStyle = IDateFormat_MEDIUM;
                    break;
                case 3:
                    dateStyle = IDateFormat_SHORT;
                    break;
            }
            DateFormat::GetDateInstance(
                    dateStyle, (ILocale*)mLocale, (IDateFormat**)&v1);
            DateFormat::GetTimeInstance(
                    dateStyle, (ILocale*)mLocale, (IDateFormat**)&v2);
            *value = (type == 1) ? (IFormat*)v1 : (IFormat*)v2;
            return NOERROR;
        case 2: // number
            if (ch == '}') {
                // BEGIN android-changed
                NumberFormat::GetInstance(mLocale, (INumberFormat**)value);
                // END android-changed
                return NOERROR;
            }
            Int32 numberStyle;
            ArrayOf<String>* tokens3 = ArrayOf<String>::Alloc(3);
            (*tokens3)[0] = String("currency");
            (*tokens3)[1] = String("percent");
            (*tokens3)[2] = String("integer");
            Match(string, position, TRUE, tokens3, &numberStyle);
            if (numberStyle == -1) {
                Format::UpToWithQuotes(string, position, buffer, '}', '{');
//                return new DecimalFormat(buffer.toString(),
//                        new DecimalFormatSymbols(locale));
                return NOERROR;
            }
            ArrayOf<String>::Free(tokens3);
            switch (numberStyle) {
                case 0: // currency
                    NumberFormat::GetCurrencyInstance(mLocale, (INumberFormat**)value);
                    return NOERROR;
                case 1: // percent
                    NumberFormat::GetPercentInstance(mLocale, (INumberFormat**)value);
                    return NOERROR;
            }
            NumberFormat::GetIntegerInstance(mLocale, (INumberFormat**)value);
            return NOERROR;
    }
    // choice
//    try {
        Format::UpToWithQuotes(string, position, buffer, '}', '{');
//    } catch (IllegalArgumentException e) {
        // ignored
//    }
//    return new ChoiceFormat(buffer.toString());
    if (tokens2 != NULL) {
        ArrayOf<String>::Free(tokens2);
    }
    return NOERROR;
}

ECode MessageFormat::SetFormat(
        /* [in] */ Int32 offset, 
        /* [in] */ IFormat* format)
{
    (*mFormats)[offset] = format;
    return NOERROR;
}

ECode MessageFormat::SetFormats(
        /* [in] */ ArrayOf< IFormat* >* formats)
{
    Int32 min = mFormats->GetLength();
    if (formats->GetLength() < min) {
        min = formats->GetLength();
    }
    for (Int32 i = 0; i < min; i++) {
        (*mFormats)[i] = (*formats)[i];
    }
    return NOERROR;
}

ECode MessageFormat::SetLocale(
        /* [in] */ ILocale* locale)
{
    mLocale = locale;
    for (Int32 i = 0; i <= mMaxOffset; i++) {
//        AutoPtr<IFormat> format = (*mFormats)[i];
        // BEGIN android-removed
        //if (format instanceof DecimalFormat) {
        //     formats[i] = new DecimalFormat(((DecimalFormat) format)
        //             .toPattern(), new DecimalFormatSymbols(locale));
        //} else if (format instanceof SimpleDateFormat) {
        //     formats[i] = new SimpleDateFormat(((SimpleDateFormat) format)
        //             .toPattern(), locale);
        //}
        // END android-removed
        // BEGIN android-added
        // java specification undefined for null argument, change into
        // a more tolerant implementation
//        if (format->Probe(EIID_IDecimalFormat) != NULL) {
            //try {
//                formats[i] = new DecimalFormat(((DecimalFormat) format)
//                        .toPattern(), new DecimalFormatSymbols(locale));
            //} catch (NullPointerException npe){
            //    formats[i] = null;
            //}
//        } else if (format->Probe(EIID_ISimpleDateFormat) != NULL) {
            //try {
//                formats[i] = new SimpleDateFormat(((SimpleDateFormat) format)
//                        .toPattern(), locale);
            //} catch (NullPointerException npe) {
            //    formats[i] = null;
            //}
//        }
        // END android-added
    }
    return NOERROR;
}

ECode MessageFormat::DecodeDecimalFormat(
        /* [in] */ StringBuffer* buffer, 
        /* [in] */ IFormat* format,
        /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *buffer += ",number";
//    if (format.equals(NumberFormat.getNumberInstance(locale))) {
        // Empty block
//    } else if (format.equals(NumberFormat.getIntegerInstance(locale))) {
        *buffer += ",integer";
//    } else if (format.equals(NumberFormat.getCurrencyInstance(locale))) {
        *buffer += ",currency";
//    } else if (format.equals(NumberFormat.getPercentInstance(locale))) {
        *buffer += ",percent";
//    } else {
        *buffer += ',';
//        return ((DecimalFormat) format).toPattern();
//    }
    *value = String(NULL);
    return NOERROR;
}

ECode MessageFormat::DecodeSimpleDateFormat(
        /* [in] */ StringBuffer* buffer, 
        /* [in] */ IFormat* format,
        /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
//    if (format.equals(DateFormat.getTimeInstance(DateFormat.DEFAULT, locale))) {
        *buffer += ",time";
//    } else if (format.equals(DateFormat.getDateInstance(DateFormat.DEFAULT,
//            locale))) {
        *buffer += ",date";
//    } else if (format.equals(DateFormat.getTimeInstance(DateFormat.SHORT,
//            locale))) {
        *buffer += ",time,short";
//    } else if (format.equals(DateFormat.getDateInstance(DateFormat.SHORT,
//            locale))) {
        *buffer += ",date,short";
//    } else if (format.equals(DateFormat.getTimeInstance(DateFormat.LONG,
//            locale))) {
        *buffer += ",time,long";
//    } else if (format.equals(DateFormat.getDateInstance(DateFormat.LONG,
//            locale))) {
        *buffer += ",date,long";
//    } else if (format.equals(DateFormat.getTimeInstance(DateFormat.FULL,
//            locale))) {
        *buffer += ",time,full";
//    } else if (format.equals(DateFormat.getDateInstance(DateFormat.FULL,
//            locale))) {
        *buffer += ",date,full";
//    } else {
        *buffer += ",date,";
//        return ((SimpleDateFormat) format).toPattern();
//    }
    *value = String(NULL);
    return NOERROR;
}

ECode MessageFormat::ToPattern(
        /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    StringBuffer* buffer = new StringBuffer("");
    for (Int32 i = 0; i <= mMaxOffset; i++) {
        AppendQuoted(buffer, (*mStrings)[i]);
        *buffer += '{';
        *buffer += (*mArgumentNumbers)[i];
        AutoPtr<IFormat> format = (*mFormats)[i];
        String pattern = String(NULL);
//        if (format->Probe(EIID_IChoiceFormat) != NULL) {
            *buffer += ",choice,";
//            pattern = ((ChoiceFormat) format).toPattern();
//        } else if (format instanceof DecimalFormat) {
            DecodeDecimalFormat(buffer, (IFormat*)format, value);
//        } else if (format instanceof SimpleDateFormat) {
            DecodeSimpleDateFormat(buffer, (IFormat*)format, value);
//        } else if (format != null) {
            //throw new IllegalArgumentException("Unknown format");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        }
        if (!(pattern.IsNull()) ) {
            Boolean quote = FALSE;
            Int32 index = 0;
            Int32 length = (Int32)(pattern.GetLength());
            Int32 count = 0;
            while (index < length) {
                Char32 ch = pattern.GetChar(index++);
                if (ch == '\'') {
                    quote = !quote;
                }
                if (!quote) {
                    if (ch == '{') {
                        count++;
                    }
                    if (ch == '}') {
                        if (count > 0) {
                            count--;
                        } else {
                            *buffer += "'}";
                            ch = '\'';
                        }
                    }
                }
                *buffer += ch;
            }
        }
        *buffer += '}';
    }
    if (mMaxOffset + 1 < mStrings->GetLength()) {
        AppendQuoted(buffer, (*mStrings)[mMaxOffset + 1]);
    }
    *value = buffer->Substring(0, buffer->GetLength());
    return NOERROR;
}

ECode MessageFormat::AppendQuoted(
        /* [in] */ StringBuffer* buffer, 
        /* [in] */ String string)
{
    Int32 length = string.GetLength();
    for (Int32 i = 0; i < length; i++) {
        Char32 ch = string.GetChar(i);
        if (ch == '{' || ch == '}') {
            *buffer += '\'';
            *buffer += ch;
            *buffer += '\'';
        } else {
            *buffer += ch;
        }
    }
    return NOERROR;
}

ECode MessageFormat::MessageFormat_Field::ReadResolve(
        /* [out] */ IInterface** object)
{
    //String name = this.getName();
    //if (Objects.equal(name, ARGUMENT.getName())) {
    //    return ARGUMENT;
    //}
    //throw new InvalidObjectException("Not a valid MessageFormat.Field, subclass should override readResolve()");
    return NOERROR;
}