#ifdef _STRUVT_H_
#define _STRUVT_H_

#include <elastos.h>

using namespace Elastos;

class Struct {
public:
    /**
     * Gets the SQL Type name of the SQL structured type that this {@code
     * Struct} represents.
     *
     * @return the fully qualified name of SQL structured type.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetSQLTypeName(
        /** [out] **/String* name)=0;

    /**
     * Gets the values of the attributes of this SQL structured type. This
     * method uses the type map associated with the {@link Connection} for
     * customized type mappings. Where there is no entry in the type mapping
     * which matches this structured type, the JDBC driver uses the standard
     * mapping.
     *
     * @return an {@code Object} array containing the ordered attributes.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetAttributes(
        /** [out] **/ArrayOf<IInterface>* obj)=0;

    /**
     * Gets the values of the attributes of this SQL structured type. This
     * method uses the supplied type mapping to determine how to map SQL types
     * to their corresponding Java objects. In the
     * case where there is no entry in the type mapping which matches this
     * structured type, the JDBC driver uses the default mapping. The {@code
     * Connection} type map is <i>never</i> utilized by this method.
     *
     * @param theMap
     *            a Map describing how SQL Type names are mapped to classes.
     * @return an Object array containing the ordered attributes,.
     * @throws SQLException
     *             if a database error occurs.
     */
//    virtual CARAPI Object[] GetAttributes(Map<String, Class<?>> theMap)=0;
};
#endif//_STRUVT_H_