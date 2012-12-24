#ifdef _PARAMETERMETADATA_H_
#define _PARAMETERMETADATA_H_

#include <elastos.h>
#include "Wrapper.h"

using namespace Elastos;

class ParameterMetaData : public Wrapper{
	/**
     * Gets the fully-qualified name of the Java class which should be passed as
     * a parameter to the method {@code PreparedStatement.setObject}.
     *
     * @param paramIndex
     *            the index number of the parameter, where the first parameter
     *            has index 1.
     * @return the fully qualified Java class name of the parameter with the
     *         specified index. This class name is used for custom mapping
     *         between SQL types and Java objects.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI String GetParameterClassName(
        /** [in] **/ Int32 paramIndex,
        /** [out] **/ String* str)=0;

    /**
     * Gets the number of parameters in the {@code PreparedStatement} for which
     * this {@code ParameterMetaData} contains information.
     *
     * @return the number of parameters.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetParameterCount(
        /** [in] **/ Int32* count)=0;

    /**
     * Gets the mode of the specified parameter. Can be one of:
     * <ul>
     * <li>ParameterMetaData.parameterModeIn</li>
     * <li>ParameterMetaData.parameterModeOut</li>
     * <li>ParameterMetaData.parameterModeInOut</li>
     * <li>ParameterMetaData.parameterModeUnknown</li>
     * </ul>
     *
     * @param paramIndex
     *            the index number of the parameter, where the first parameter
     *            has index 1.
     * @return the parameter's mode.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetParameterMode(
        /** [in] **/Int32 paramIndex,
        /** [out] **/ Int32* mode)=0;

    /**
     * Gets the SQL type of a specified parameter.
     *
     * @param paramIndex
     *            the index number of the parameter, where the first parameter
     *            has index 1.
     * @return the SQL type of the parameter as defined in {@code
     *         java.sql.Types}.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetParameterType(
        /** [in] **/Int32 paramIndex,
        /** [out] **/ Int32* type)=0;

    /**
     * Gets the database-specific type name of a specified parameter.
     *
     * @param paramIndex
     *            the index number of the parameter, where the first parameter
     *            has index 1.
     * @return the type name for the parameter as used by the database. A
     *         fully-qualified name is returned if the parameter is a <i>User
     *         Defined Type</i> (UDT).
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetParameterTypeName(
        /** [in] **/Int32 paramIndex,
        /** [out] **/String* name)=0;

    /**
     * Gets the number of decimal digits for a specified parameter.
     *
     * @param paramIndex
     *            the index number of the parameter, where the first parameter
     *            has index 1.
     * @return the number of decimal digits ("the precision") for the parameter.
     *         {@code 0} if the parameter is not a numeric type.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetPrecision(
        /** [in] **/Int32 paramIndex,
        /** [out] **/ Int32* result)=0;

    /**
     * Gets the number of digits after the decimal point for a specified
     * parameter.
     *
     * @param paramIndex
     *            the index number of the parameter, where the first parameter
     *            has index 1.
     * @return the number of digits after the decimal point ("the scale") for
     *         the parameter. {@code 0} if the parameter is not a numeric type.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI GetScale(
        /** [in] **/Int32 paramIndex,
        /** [out] **/ Int32* result)=0;

    /**
     * Gets whether {@code null} values are allowed for the specified parameter.
     * The returned value is one of:
     * <ul>
     * <li>ParameterMetaData.parameterNoNulls</li>
     * <li>ParameterMetaData.parameterNullable</li>
     * <li>ParameterMetaData.parameterNullableUnknown</li>
     * </ul>
     *
     * @param paramIndex
     *            the index number of the parameter, where the first parameter
     *            has index 1.
     * @return the int code indicating the nullability of the parameter.
     * @throws SQLException
     *             if a database error is encountered.
     */
    virtual CARAPI IsNullable(
        /** [in] **/Int32 paramIndex,
        /** [out] **/ Int32* result)=0;

    /**
     * Gets whether values for the specified parameter can be signed numbers.
     *
     * @param paramIndex
     *            the index number of the parameter, where the first parameter
     *            has index 1.
     * @return {@code true} if values can be signed numbers for this parameter,
     *         {@code false} otherwise.
     * @throws SQLException
     *             if a database error happens.
     */
    virtual CARAPI IsSigned(
        /** [in] **/Int32 paramIndex,
        /** [out] **/Boolean* result)=0;

public:
    /**
     * Indicates that the parameter mode is {@code IN}.
     */
    static const Int32 parameterModeIn;// = 1;

    /**
     * Indicates that the parameter mode is {@code INOUT}.
     */
    static const Int32 parameterModeInOut;// = 2;

    /**
     * Indicates that the parameter mode is {@code OUT}.
     */
    static const Int32 parameterModeOut;// = 4;

    /**
     * Indicates that the parameter mode is not known.
     */
    static const Int32 parameterModeUnknown;// = 0;

    /**
     * Indicates that a parameter is not permitted to be {@code NULL}.
     */
    static const Int32 parameterNoNulls;// = 0;

    /**
     * Indicates that a parameter is permitted to be {@code NULL}.
     */
    static const Int32 parameterNullable;// = 1;

    /**
     * Indicates that whether a parameter is allowed to be {@code null} or not
     * is not known.
     */
    static const Int32 parameterNullableUnknown;// = 2;
};
#endif//_PARAMETERMETADATA_H_
