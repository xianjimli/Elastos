#include "ChoiceFormat.h"

ChoiceFormat::~ChoiceFormat()
{
    if (mChoiceLimits != NULL) {
        ArrayOf<Double>::Free(mChoiceLimits);
    }
    if (mChoiceFormats != NULL) {
        ArrayOf<String>::Free(mChoiceFormats);
    }
}

ECode ChoiceFormat::Init(
        /* [in] */ ArrayOf<Double>* limits,
        /* [in] */ ArrayOf<String>* formats)
{
    return SetChoices(limits, formats);
}

ECode ChoiceFormat::Init(
        /* [in] */ String tem)
{
    return ApplyPattern(tem);
}

ECode ChoiceFormat::ApplyPattern(
        /* [in] */ String tem)
{
    ArrayOf<Double>* limits = ArrayOf<Double>::Alloc(5);
    List<String>* formats = new List<String>();/* = new ArrayList<String>(); */
    Int32 length = (Int32)(tem.GetLength());
    Int32 limitCount = 0;
    Int32 index = 0;
    StringBuffer buffer("");

    AutoPtr<ILocale> locale_US;
//    CLocale::New(String("en"), String("US"), (ILocale**)&locale_US);
    AutoPtr<INumberFormat> format;
    NumberFormat::GetInstance((ILocale*)locale_US, (INumberFormat**)&format);

    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);

    while (TRUE) {
        index = SkipWhitespace(tem, index);
        if (index >= length) {
            if (limitCount == limits->GetLength()) {
                mChoiceLimits = limits->Clone();
            } else {
                mChoiceLimits = ArrayOf<Double>::Alloc(limitCount);
                for (Int32 i = 0; i < limitCount; i++) {
                    (*mChoiceLimits)[i] = (*limits)[i];
                }
            }
            mChoiceFormats = ArrayOf<String>::Alloc(formats->GetSize());
            List<String>::Iterator it = formats->Begin();

            for (Int32 i = 0;
                    i < (Int32)(formats->GetSize()), it != formats->End(); ++i, ++it) {
                (*mChoiceFormats)[i] = *it;
            }
            //return E_NULL_EXCEPTION;
            return NOERROR;
        }

        position->SetIndex(index);
        AutoPtr<INumber> value;
        format->ParseEx(tem, (IParsePosition*)position, (INumber**)&value);
        Int32 indexx;
        position->GetIndex(&indexx);
        index = SkipWhitespace(tem, indexx);
        Int32 errorIndex;
        position->GetErrorIndex(&errorIndex);
        if (errorIndex != -1 || index >= length) {
            // Fix Harmony 540
            mChoiceLimits = ArrayOf<Double>::Alloc(0);
            mChoiceFormats = ArrayOf<String>::Alloc(0);
            //return E_NULL_EXCEPTION;
            return NOERROR;
        }
        Char32 ch = tem.GetChar(index++);
        if (limitCount == limits->GetLength()) {
            ArrayOf<Double>* newLimits = ArrayOf<Double>::Alloc(limitCount * 2);
            for (Int32 i = 0; i < limitCount; i++) {
                (*newLimits)[i] = (*limits)[i];
            }
            limits = newLimits->Clone();
            ArrayOf<Double>::Free(newLimits);
        }
        Double next = 0.0;
        switch (ch) {
            case '#':
            case '\u2264':
//                next = value.doubleValue();
                break;
            case '<':
//                next = nextDouble(value.doubleValue());
                break;
            default:
                //throw new IllegalArgumentException();
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (limitCount > 0 && next <= (*limits)[limitCount - 1]) {
            //throw new IllegalArgumentException();
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        //buffer.setLength(0);
        buffer = String("");
        position->SetIndex(index);
        Boolean succeeded;
        UpTo(tem, position, buffer, '|', &succeeded);
        position->GetIndex(&index);
        (*limits)[limitCount++] = next;
        formats->PushBack(buffer.Substring(0, buffer.GetLength()));
    }
    ArrayOf<Double>::Free(limits);
    return NOERROR;
}

ECode ChoiceFormat::formatEx3(
        /* [in] */ Double value,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* result)
 {
    VALIDATE_NOT_NULL(result);
    for (Int32 i = mChoiceLimits->GetLength() - 1; i >= 0; i--)
    {
        if ((*mChoiceLimits)[i] <= value) {
            *buffer += (*mChoiceFormats)[i];
            return NOERROR;
        }
    }
    if (mChoiceFormats->GetLength() == 0) {
        *result = StringBuffer(*buffer);
    } else {
        *result = StringBuffer(*buffer += (*mChoiceFormats)[0]);
    }
    return NOERROR;
}

ECode ChoiceFormat::formatEx5(
        /* [in] */ Int64 value,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* formattedString)
{
    VALIDATE_NOT_NULL(formattedString);
    return formatEx3((Double)value, buffer, field, formattedString);
}

ECode ChoiceFormat::GetFormats(
        /* [out, callee] */ ArrayOf<IInterface*>** arrayOfFormattedString)
{
//    *arrayOfFormattedString = mChoiceFormats->Clone();
    return NOERROR;
}

ECode ChoiceFormat::GetLimits(
        /* [out, callee] */ ArrayOf<Double>** arrayOfDoubles)
{
    *arrayOfDoubles = mChoiceLimits->Clone();
    return NOERROR;
}

#define CAST_PTR(type, val) ((type*)(&(val)))
#define CAST(type, val) (*CAST_PTR(type, val))

const Int64 ChoiceFormat::INT64_NAN = 0X7FF8000000000000ULL;
const Double ChoiceFormat::DOUBLE_ZERO = 0.0;
const Double ChoiceFormat::POSITIVE_INFINITY = 1.0/DOUBLE_ZERO;
const Double ChoiceFormat::NEGATIVE_INFINITY = -1.0/DOUBLE_ZERO;

Int64 ChoiceFormat::DoubleToInt64Bits(
    /* [in] */ Double d)
{
    return d != d ? INT64_NAN : CAST(Int64, d);
}

Double ChoiceFormat::Int64BitsToDouble(
    /* [in] */ Int64 l) {
    return CAST(Double, l);
}

Double ChoiceFormat::NextDouble(
        /* [in] */ Double value)
{
    if (value == POSITIVE_INFINITY) {
        return value;
    }
    Int64 bits;
    // Handle -0.0
    if (value == 0) {
        bits = 0;
    } else {
        bits = DoubleToInt64Bits(value);
    }
    Int64 l = value < 0 ? bits - 1 : bits + 1;
    return Int64BitsToDouble(l);
}

Double ChoiceFormat::NextDouble(
        /* [in] */ Double value,
        /* [in] */ Boolean increment)
{
    return increment ? NextDouble(value) : PreviousDouble(value);
}

ECode ChoiceFormat::ParseEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value)
{
    Int32 offset;
    position->GetIndex(&offset);
    for (Int32 i = 0; i < mChoiceFormats->GetLength(); i++) {
        String str1 = string.Substring( offset, (Int32)((*mChoiceFormats)[i].GetLength()) );
        String str2 = (*mChoiceFormats)[i];
        if (str1.EqualsIgnoreCase(str2)) {
            position->SetIndex(offset + (Int32)((*mChoiceFormats)[i].GetLength()));
//            return new Double(choiceLimits[i]);
            return NOERROR;
        }
    }
    position->SetErrorIndex(offset);
//    return new Double(Double.NaN);
    return NOERROR;
}

Double ChoiceFormat::PreviousDouble(
        /* [in] */ Double value)
{
    if (value == NEGATIVE_INFINITY) {
        return value;
    }
    Int64 bits;
    // Handle 0.0
    if (value == 0) {
        bits = 0x8000000000000000L;
    } else {
        bits = DoubleToInt64Bits(value);
    }
    Int64 l = value <= 0 ? bits + 1 : bits - 1;
    return Int64BitsToDouble(l);
}

ECode ChoiceFormat::SetChoices(
        /* [in] */ ArrayOf<Double>* limits,
        /* [in] */ ArrayOf<String>* formats)
{
    if (limits->GetLength() != formats->GetLength()) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mChoiceLimits = limits->Clone();
    mChoiceFormats = formats->Clone();
    return NOERROR;
}

Int32 ChoiceFormat::SkipWhitespace(
        /* [in] */ String string,
        /* [in] */ Int32 index)
{
    Int32 length = (Int32)(string.GetLength());
    while (index < length && Character::IsWhitespace(string.GetChar(index))) {
        index++;
    }
    return index;
}

ECode ChoiceFormat::ToPattern(
        /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);

    StringBuffer* buffer = new StringBuffer("");
    for (Int32 i = 0; i < mChoiceLimits->GetLength(); i++) {
        if (i != 0) {
            (*buffer) += '|';
        }
        String previous = String::FromDouble(PreviousDouble((*mChoiceLimits)[i]));
        String limit = String::FromDouble((*mChoiceLimits)[i]);
        if (previous.GetLength() < limit.GetLength()) {
            *buffer += previous;
            *buffer += '<';
        } else {
            *buffer += limit;
            *buffer += '#';
        }
        Boolean quote = ((*mChoiceFormats)[i].IndexOf('|') != -1);
        if (quote) {
            *buffer += '\'';
        }
        *buffer += (*mChoiceFormats)[i];
        if (quote) {
            *buffer += '\'';
        }
    }
    *pattern = buffer->Substring(0, buffer->GetLength());
    return NOERROR;
}