
#include "cmdef.h"
#include "ChoiceFormat.h"
#include "CParsePosition.h"
#include "Math.h"
#include <Character.h>
#include <elastos/List.h>
#include <StringBuffer.h>

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
    /* [in] */ const ArrayOf<Double>& limits,
    /* [in] */ const ArrayOf<String>& formats)
{
    return SetChoices(limits, formats);
}

ECode ChoiceFormat::Init(
    /* [in] */ const String& tem)
{
    return ApplyPattern(tem);
}

ECode ChoiceFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    ArrayOf<Double>* limits = ArrayOf<Double>::Alloc(5);
    List<String>* formats = new List<String>();
    Int32 length = tem.GetLength();
    Int32 limitCount = 0, index = 0;
    StringBuffer buffer;

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> uslocale;
    localeHelper->GetUS((ILocale**)&uslocale);
    AutoPtr<INumberFormat> format;
    NumberFormat::GetInstance(uslocale, (INumberFormat**)&format);

    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);

    while (TRUE) {
        index = SkipWhitespace(tem, index);
        if (index >= length) {
            if (limitCount == limits->GetLength()) {
                mChoiceLimits = limits;
            }
            else {
                mChoiceLimits = ArrayOf<Double>::Alloc(limitCount);
                for (Int32 i = 0; i < limitCount; i++) {
                    (*mChoiceLimits)[i] = (*limits)[i];
                }
            }
            mChoiceFormats = ArrayOf<String>::Alloc(formats->GetSize());
            List<String>::Iterator it = formats->Begin();
            for (Int32 i = 0; it != formats->End(); ++it, ++i) {
                (*mChoiceFormats)[i] = *it;
            }
            return NOERROR;
        }

        position->SetIndex(index);
        AutoPtr<INumber> value;
        format->ParseEx(tem, position, (INumber**)&value);
        Int32 index;
        position->GetIndex(&index);
        index = SkipWhitespace(tem, index);
        Int32 errorIndex;
        position->GetErrorIndex(&errorIndex);
        if (errorIndex != -1 || index >= length) {
            // Fix Harmony 540
            mChoiceLimits = ArrayOf<Double>::Alloc(0);
            mChoiceFormats = ArrayOf<String>::Alloc(0);
            return NOERROR;
        }
        Char32 ch = tem.GetChar(index++);
        if (limitCount == limits->GetLength()) {
            ArrayOf<Double>* newLimits = ArrayOf<Double>::Alloc(limitCount * 2);
            for (Int32 i = 0; i < limitCount; i++) {
                (*newLimits)[i] = (*limits)[i];
            }
            limits = newLimits;
        }
        Double next = 0.0;
        switch (ch) {
            case '#':
            case 2264://'\u2264'
                value->DoubleValue(&next);
                break;
            case '<':
                Double d;
                value->DoubleValue(&d);
                next = NextDouble(d);
                break;
            default:
                //throw new IllegalArgumentException();
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (limitCount > 0 && next <= (*limits)[limitCount - 1]) {
            //throw new IllegalArgumentException();
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        buffer.SetLength(0);
        position->SetIndex(index);
        Boolean succeeded;
        UpTo(tem, position, buffer, '|', &succeeded);
        position->GetIndex(&index);
        (*limits)[limitCount++] = next;
        formats->PushBack(buffer.ToString());
    }

    return NOERROR;
}

ECode ChoiceFormat::FormatDoubleEx(
    /* [in] */ Double value,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* result)
 {
    for (Int32 i = mChoiceLimits->GetLength() - 1; i >= 0; i--) {
        if ((*mChoiceLimits)[i] <= value) {
            *result = buffer;
            result->Append((*mChoiceFormats)[i]);
            return NOERROR;
        }
    }

    if (mChoiceFormats->GetLength() == 0) {
        *result = buffer;
    }
    else {
        *result = buffer;
        result->Append((*mChoiceFormats)[0]);
    }

    return NOERROR;
}

ECode ChoiceFormat::FormatInt64Ex(
    /* [in] */ Int64 value,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* formattedString)
{
    return FormatDoubleEx((Double)value, buffer, field, formattedString);
}

ECode ChoiceFormat::GetFormats(
    /* [out, callee] */ ArrayOf<IInterface*>** arrayOfFormattedString)
{
    ArrayOf<IInterface*>* temp =
            ArrayOf<IInterface*>::Alloc(mChoiceFormats->GetLength());
    for (Int32 i = 0; i < mChoiceFormats->GetLength(); ++i) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New((*mChoiceFormats)[i], (ICharSequence**)&cs);
        (*temp)[i] = (IInterface*)cs.Get();
    }
    *arrayOfFormattedString = temp;
    return NOERROR;
}

ECode ChoiceFormat::GetLimits(
    /* [out, callee] */ ArrayOf<Double>** arrayOfDoubles)
{
    VALIDATE_NOT_NULL(arrayOfDoubles);
    *arrayOfDoubles = mChoiceLimits;
    return NOERROR;
}

Double ChoiceFormat::NextDouble(
    /* [in] */ Double value)
{
    if (value == Math::DOUBLE_POSITIVE_INFINITY) {
        return value;
    }
    Int64 bits;
    // Handle -0.0
    if (value == 0) {
        bits = 0;
    }
    else {
        bits = Math::DoubleToInt64Bits(value);
    }

    return Math::Int64BitsToDouble(value < 0 ? bits - 1 : bits + 1);
}

Double ChoiceFormat::NextDouble(
    /* [in] */ Double value,
    /* [in] */ Boolean increment)
{
    return increment ? NextDouble(value) : PreviousDouble(value);
}

ECode ChoiceFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** value)
{
    Int32 offset;
    position->GetIndex(&offset);
    for (Int32 i = 0; i < mChoiceFormats->GetLength(); i++) {
        String str = string.Substring(offset, (*mChoiceFormats)[i].GetLength());
        if (str.StartWith((*mChoiceFormats)[i])) {
            position->SetIndex(offset + (*mChoiceFormats)[i].GetLength());
            AutoPtr<IDouble> d;
            ASSERT_SUCCEEDED(CDouble::New((*mChoiceLimits)[i], (IDouble**)&d));
            //todo: all interface such as IDouble should extends INumber
            //*value = (INumber*)d.Get();
            assert(0);
            return NOERROR;
        }
    }
    position->SetErrorIndex(offset);
    AutoPtr<IDouble> d;
    ASSERT_SUCCEEDED(CDouble::New(Math::DOUBLE_NAN, (IDouble**)&d));
    //todo: all interface such as IDouble should extends INumber
    //*value = (INumber*)d.Get();
    assert(0);
    return NOERROR;
}

Double ChoiceFormat::PreviousDouble(
    /* [in] */ Double value)
{
    if (value == Math::DOUBLE_NEGATIVE_INFINITY) {
        return value;
    }
    Int64 bits;
    // Handle 0.0
    if (value == 0) {
        bits = 0x8000000000000000ll;
    }
    else {
        bits = Math::DoubleToInt64Bits(value);
    }
    return Math::Int64BitsToDouble(value <= 0 ? bits + 1 : bits - 1);
}

ECode ChoiceFormat::SetChoices(
    /* [in] */ const ArrayOf<Double>& limits,
    /* [in] */ const ArrayOf<String>& formats)
{
    if (limits.GetLength() != formats.GetLength()) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mChoiceLimits = limits.Clone();
    mChoiceFormats = formats.Clone();
    return NOERROR;
}

Int32 ChoiceFormat::SkipWhitespace(
    /* [in] */ const String& string,
    /* [in] */ Int32 index)
{
    Int32 length = string.GetLength();
    while (index < length && Character::IsWhitespace(string.GetChar(index))) {
        index++;
    }
    return index;
}

ECode ChoiceFormat::ToPattern(
    /* [out] */ String* pattern)
{
    StringBuffer buffer;
    for (Int32 i = 0; i < mChoiceLimits->GetLength(); i++) {
        if (i != 0) {
            buffer += '|';
        }
        String previous = String::FromDouble(PreviousDouble((*mChoiceLimits)[i]));
        String limit = String::FromDouble((*mChoiceLimits)[i]);
        if (previous.GetLength() < limit.GetLength()) {
            buffer += previous;
            buffer += '<';
        }
        else {
            buffer += limit;
            buffer += '#';
        }
        Boolean quote = ((*mChoiceFormats)[i].IndexOf('|') != -1);
        if (quote) {
            buffer += '\'';
        }
        buffer += (*mChoiceFormats)[i];
        if (quote) {
            buffer += '\'';
        }
    }
    *pattern = buffer.ToString();
    return NOERROR;
}