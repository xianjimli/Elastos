#ifndef __NUMBER_H__
#define __NUMBER_H__

#include <elastos.h>

using namespace Elastos;
/**
 * The abstract superclass of the classes which represent numeric base types
 * (that is {@link Byte}, {@link Short}, {@link Integer}, {@link Long},
 * {@link Float}, and {@link Double}.
 */
class Number
{
public:
    /**
     * Empty default constructor.
     */
    Number();

    /**
     * Returns this object's value as a byte. Might involve rounding and/or
     * truncating the value, so it fits into a byte.
     *
     * @return the primitive byte value of this object.
     */
    virtual CARAPI_(Byte) ByteValue();

    /**
     * Returns this object's value as a double. Might involve rounding.
     *
     * @return the primitive double value of this object.
     */
    //virtual CARAPI_(Double) DoubleValue() = 0;
    virtual CARAPI_(Double) DoubleValue();

    /**
     * Returns this object's value as a float. Might involve rounding.
     *
     * @return the primitive float value of this object.
     */
    //virtual CARAPI_(Float) FloatValue() = 0;
    virtual CARAPI_(Float) FloatValue();

    /**
     * Returns this object's value as an int. Might involve rounding and/or
     * truncating the value, so it fits into an int.
     *
     * @return the primitive int value of this object.
     */
    //virtual CARAPI_(Int32) Int32Value() = 0;
    virtual CARAPI_(Int32) Int32Value();

    /**
     * Returns this object's value as a long. Might involve rounding and/or
     * truncating the value, so it fits into a long.
     *
     * @return the primitive long value of this object.
     */
    //virtual CARAPI_(Int64) Int64Value() = 0;
    virtual CARAPI_(Int64) Int64Value();

    /**
     * Returns this object's value as a short. Might involve rounding and/or
     * truncating the value, so it fits into a short.
     *
     * @return the primitive short value of this object.
     */
    virtual CARAPI_(Int16) Int16Value();

private:
    const static Int64 serialVersionUID = -8742448824652078965L;
};
#endif //__NUMBER_H__