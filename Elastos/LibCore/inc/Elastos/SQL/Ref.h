#ifdef _REF_H_
#define _REF_H_

#include <elastos.h>

using namespace Elastos;

class Ref {
	/**
     * Gets the fully-qualified SQL name of the SQL structured type that this
     * {@code Ref} references.
     *
     * @return the fully qualified name of the SQL structured type.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI getBaseTypeName(
        /** [out] **/String* name)=0;

    /**
     * Gets the SQL structured type instance referenced by this {@code Ref}.
     *
     * @return a Java object whose type is defined by the mapping for the SQL
     *         structured type.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI getObject(
        /** [out] **/IInterface** obj)=0;

    /**
     * Returns the associated object and uses the relevant mapping to convert it
     * to a Java type.
     *
     * @param map
     *            the mapping for type conversion.
     * @return a Java object whose type is defined by the mapping for the SQL
     *         structured type.
     * @throws SQLException
     *             if there is a database error.
     */
 //   virtual CARAPI Object getObject(Map<String, Class<?>> map)=0;

    /**
     * Sets the value of the structured type that this {@code Ref} references to
     * a supplied object.
     *
     * @param value
     *            the {@code Object} representing the new SQL structured type
     *            that this {@code Ref} references.
     * @throws SQLException
     *             if there is a database error.
     */
    virtual CARAPI setObject(
        /** [in] **/IInterface* value)=0;
};
#endif//_REF_H_