#include "Number.h"

Number::Number()
{
}

Byte Number::ByteValue()
{
    return (Byte)Int32Value();
}

Int16 Number::Int16Value()
{
    return (Int16)Int32Value();
}

Double Number::DoubleValue()
{
    return 0.0;
}

Float Number::FloatValue()
{
    return 0.0;
}

Int32 Number::Int32Value()
{
    return 0;
}

Int64 Number::Int64Value()
{
    return 0L;
}