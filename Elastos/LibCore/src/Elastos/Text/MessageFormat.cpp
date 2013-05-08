#include "MessageFormat.h"
#include "CMessageFormatField.h"
#include "CDateFormatHelper.h"


static AutoPtr<IMessageFormatField> sInit(const String& name) {
    AutoPtr<IMessageFormatField> field;
    CMessageFormatField::NewByFriend(name, (CMessageFormatField**)&field);
    return field.Get();
}

const AutoPtr<IMessageFormatField> MessageFormat::MessageFormatField::ARGUMENT
    = sInit(String("message argument field"));


ECode MessageFormat::MessageFormatField::Init(
    /* [in] */ String fieldName)
{
    return NOERROR;
}

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
    StringBuffer buffer("");
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
        Boolean succeeded;
        if (Format::UpTo(tem, position, buffer, '{', &succeeded), succeeded) {
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
        localStrings->PushBack(buffer.Substring(0, buffer.GetLength()));
        //buffer.setLength(0);
        buffer = String("");
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
        return E_NULL_POINTER_EXCEPTION;
    }

    StringBuffer* buffer = new StringBuffer("");
    Vector<FieldContainer*> *fields = new Vector<FieldContainer*>();

    // format the message, and find fields
    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    StringBuffer* sb = new StringBuffer("");
    FormatImpl((ArrayOf< IInterface* >*) object, buffer, position, fields, sb);

    // create an AttributedString with the formatted buffer
    AutoPtr<IAttributedString> as;
    CAttributedString::New(buffer->Substring(0, buffer->GetLength()), (IAttributedString**)&as);

    // add MessageFormat field attributes and values to the AttributedString
    for (Int32 i = 0; i < (Int32)fields->GetSize(); i++) {
        FieldContainer *fc = (*fields)[i];
        FAIL_RETURN(as->AddAttributeEx(fc->mAttribute, fc->mValue, fc->mStart, fc->mEnd));
    }

    // return the CharacterIterator from AttributedString
    return as->GetIterator(characterIterator);
}

ECode MessageFormat::FormatObjects(
    /* [in] */ ArrayOf< IInterface* >* objects,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* value)
{
    return FormatImpl(objects, new StringBuffer(buffer.string()), field, NULL, new StringBuffer(value->string()));
}

ECode MessageFormat::FormatImpl(
    /* [in] */ ArrayOf< IInterface* >* objects,
    /* [in] */ StringBuffer* buffer,
    /* [in] */ IFieldPosition* position,
    /* [in] */ Vector<FieldContainer*>* fields,
    /* [out] */ StringBuffer* value)
{
    AutoPtr<IFieldPosition> passedField;
    CFieldPosition::New(0, (IFieldPosition**)&passedField);

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
                AutoPtr<IDateFormatHelper> dfh;
                CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&dfh);
                dfh->GetInstance((IDateFormat**)&format);
            } else {
                *buffer += arg;
                HandleArgumentField(begin, buffer->GetLength(),
                        (*mArgumentNumbers)[i], position, fields);
                continue;
            }
        }
        if (format->Probe(EIID_IChoiceFormat) != NULL) {
            String result;
            format->FormatObject(arg, &result);
            AutoPtr<IMessageFormat> mf;
            //CMessageFormat::New(result, (IMessageFormat**)&mf);
            mf->SetLocale(mLocale);
            mf->FormatObjects(objects, buffer->Substring(0, buffer->GetLength()), passedField, &result);
            HandleArgumentField(begin, buffer->GetLength(), (*mArgumentNumbers)[i], position, fields);
            Handleformat(format, arg, begin, fields);
        } else {
            String result;
            format->FormatObjectEx(arg, buffer->Substring(0, buffer->GetLength()), passedField, &result);
            HandleArgumentField(begin, buffer->GetLength(), (*mArgumentNumbers)[i], position, fields);
            Handleformat(format, arg, begin, fields);
        }
    }
    if (mMaxOffset + 1 < mStrings->GetLength()) {
        (*buffer) += (*mStrings)[mMaxOffset + 1];
    }
    value = buffer;
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
            (IAttributedCharacterIteratorAttribute*)MessageFormatField::ARGUMENT,
            (IInterface*)argIndex);
        fields->PushBack(fc);
    } else {
        Int32 endIndex;
        position->GetEndIndex(&endIndex);
        AutoPtr<IFormatField> fa;
        position->GetFieldAttribute((IFormatField**)&fa);
        if (position != NULL
                && fa == MessageFormatField::ARGUMENT
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
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
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

ECode MessageFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    return FormatObjects((ArrayOf< IInterface* >*) object, buffer, field, value);
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
        return E_PARSE_EXCEPTION;
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
    StringBuffer buffer("");
    Int32 index;
    position->GetIndex(&index);
    ch = (Char32)(string.GetChar(index - 1));

    AutoPtr<IDateFormat> v1, v2;
    AutoPtr<IDateFormatHelper> dfh;
    CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&dfh);
    switch (type) {
        case 0: // time
        case 1: // date
            if (ch == '}') {
                dfh->GetDateInstanceEx2(
                        IDateFormat_DEFAULT, (ILocale*)mLocale, (IDateFormat**)&v1);
                dfh->GetTimeInstanceEx2(
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
                Boolean succeeded;
                Format::UpToWithQuotes(string, position, buffer, '}', '{', &succeeded);
                AutoPtr<ISimpleDateFormat> sdf;
                CSimpleDateFormat::New(buffer.Substring(0, buffer.GetLength()), mLocale, (ISimpleDateFormat**)&sdf);
                *value = sdf.Get();
                (*value)->AddRef();
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
            dfh->GetDateInstanceEx2(
                    dateStyle, (ILocale*)mLocale, (IDateFormat**)&v1);
            dfh->GetTimeInstanceEx2(
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
                Boolean succeeded;
                Format::UpToWithQuotes(string, position, buffer, '}', '{', &succeeded);
                AutoPtr<IDecimalFormatSymbols> dfs;
                CDecimalFormatSymbols::New(mLocale, (IDecimalFormatSymbols**)&dfs);
                AutoPtr<IDecimalFormat> df;
                CDecimalFormat::New(buffer.Substring(0, buffer.GetLength()), dfs, (IDecimalFormat**)&df);
                *value = df.Get();
                (*value)->AddRef();
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
        Boolean succeeded;
        Format::UpToWithQuotes(string, position, buffer, '}', '{', &succeeded);
//    } catch (IllegalArgumentException e) {
        // ignored
//    }
    AutoPtr<IChoiceFormat> cf;
    CChoiceFormat::New(buffer.Substring(0, buffer.GetLength()), (IChoiceFormat**)&cf);
    *value = cf.Get();
    (*value)->AddRef();
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
        AutoPtr<IFormat> format = (*mFormats)[i];
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
        if (format->Probe(EIID_IDecimalFormat) != NULL) {
            //try {
            String pattern;
            IDecimalFormat* df = reinterpret_cast<IDecimalFormat*>(format->Probe(EIID_IDecimalFormat));
            df->ToPattern(&pattern);
            AutoPtr<IDecimalFormatSymbols> dfs;
            CDecimalFormatSymbols::New(mLocale, (IDecimalFormatSymbols**)&dfs);
            AutoPtr<IDecimalFormat> dff;
            CDecimalFormat::New(pattern, dfs, (IDecimalFormat**)&dff);
            (*mFormats)[i] = dff.Get();
            ((*mFormats)[i])->AddRef();
            //} catch (NullPointerException npe){
            //    formats[i] = null;
            //}
        } else if (format->Probe(EIID_ISimpleDateFormat) != NULL) {
            //try {
            String pattern;
            ISimpleDateFormat* sdf = reinterpret_cast<ISimpleDateFormat*>(format->Probe(EIID_ISimpleDateFormat));
            sdf->ToPattern(&pattern);
            AutoPtr<ISimpleDateFormat> sdff;
            CSimpleDateFormat::New(pattern, mLocale, (ISimpleDateFormat**)&sdff);
            (*mFormats)[i] = sdff.Get();
            ((*mFormats)[i])->AddRef();
            //} catch (NullPointerException npe) {
            //    formats[i] = null;
            //}
        }
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

    INumberFormat* nff = reinterpret_cast<INumberFormat*>(format->Probe(EIID_INumberFormat));
    AutoPtr<INumberFormat> nfn;
    NumberFormat::GetNumberInstance(mLocale, (INumberFormat**)&nfn);
    AutoPtr<INumberFormat> nfi;
    NumberFormat::GetIntegerInstance(mLocale, (INumberFormat**)&nfi);
    AutoPtr<INumberFormat> nfc;
    NumberFormat::GetCurrencyInstance(mLocale, (INumberFormat**)&nfc);
    AutoPtr<INumberFormat> nfp;
    NumberFormat::GetPercentInstance(mLocale, (INumberFormat**)&nfp);
    if (nff == nfn.Get()) {
        // Empty block
    } else if (nff == nfi.Get()) {
        *buffer += ",integer";
    } else if (nff == nfc.Get()) {
        *buffer += ",currency";
    } else if (nff == nfp.Get()) {
        *buffer += ",percent";
    } else {
        *buffer += ',';
        IDecimalFormat* nfff = reinterpret_cast<IDecimalFormat*>(format->Probe(EIID_IDecimalFormat));
        nfff->ToPattern(value);
        return NOERROR;
    }
    *value = String(NULL);
    return NOERROR;
}

ECode MessageFormat::DecodeSimpleDateFormat(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ IFormat* format,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    IDateFormat* df = reinterpret_cast<IDateFormat*>(format->Probe(EIID_IDateFormat));
    AutoPtr<IDateFormatHelper> dfh;
    CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&dfh);
    AutoPtr<IDateFormat> dft;
    dfh->GetTimeInstanceEx2(IDateFormat_DEFAULT, mLocale, (IDateFormat**)&dft);
    AutoPtr<IDateFormat> dfd;
    dfh->GetDateInstanceEx2(IDateFormat_DEFAULT, mLocale, (IDateFormat**)&dfd);
    AutoPtr<IDateFormat> dft2;
    dfh->GetTimeInstanceEx2(IDateFormat_SHORT, mLocale, (IDateFormat**)&dft2);
    AutoPtr<IDateFormat> dfd2;
    dfh->GetDateInstanceEx2(IDateFormat_SHORT, mLocale, (IDateFormat**)&dfd2);
    AutoPtr<IDateFormat> dft3;
    dfh->GetTimeInstanceEx2(IDateFormat_LONG, mLocale, (IDateFormat**)&dft3);
    AutoPtr<IDateFormat> dfd3;
    dfh->GetDateInstanceEx2(IDateFormat_LONG, mLocale, (IDateFormat**)&dfd3);
    AutoPtr<IDateFormat> dft4;
    dfh->GetTimeInstanceEx2(IDateFormat_FULL, mLocale, (IDateFormat**)&dft4);
    AutoPtr<IDateFormat> dfd4;
    dfh->GetDateInstanceEx2(IDateFormat_FULL, mLocale, (IDateFormat**)&dfd4);
    if (df == dft.Get()) {
        *buffer += ",time";
    } else if (df == dfd.Get()) {
        *buffer += ",date";
    } else if (df == dft2.Get()) {
        *buffer += ",time,short";
    } else if (df == dfd2.Get()) {
        *buffer += ",date,short";
    } else if (df == dft3.Get()) {
        *buffer += ",time,long";
    } else if (df == dfd3.Get()) {
        *buffer += ",date,long";
    } else if (df == dft4.Get()) {
        *buffer += ",time,full";
    } else if (df == dfd4.Get()) {
        *buffer += ",date,full";
    } else {
        *buffer += ",date,";
        ISimpleDateFormat* sdf = reinterpret_cast<ISimpleDateFormat*>(format->Probe(EIID_ISimpleDateFormat));
        sdf->ToPattern(value);
        return NOERROR;
    }
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
        if (format->Probe(EIID_IChoiceFormat) != NULL) {
            *buffer += ",choice,";
            IChoiceFormat* cf = reinterpret_cast<IChoiceFormat*>(format->Probe(EIID_IChoiceFormat));
            cf->ToPattern(&pattern);
        } else if (format->Probe(EIID_IDecimalFormat) != NULL) {
            DecodeDecimalFormat(buffer, (IFormat*)format, value);
        } else if (format->Probe(EIID_ISimpleDateFormat) != NULL) {
            DecodeSimpleDateFormat(buffer, (IFormat*)format, value);
        } else if (format != NULL) {
            //throw new IllegalArgumentException("Unknown format");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
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

ECode MessageFormat::MessageFormatField::ReadResolve(
    /* [out] */ IInterface** object)
{
    assert(0);
    //String name = this.getName();
    //if (Objects.equal(name, ARGUMENT.getName())) {
    //    return ARGUMENT;
    //}
    //throw new InvalidObjectException("Not a valid MessageFormat.Field, subclass should override readResolve()");
    return NOERROR;
}