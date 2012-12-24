#ifdef _DRIIVER_H_
#define _DRIIVER_H_

#include <elastos.h>

using namespace Elastos;

class Driver {
public:
	/**
     * Returns whether the driver thinks that it can open a connection to the
     * given URL.
     *
     * @param url
     *            the URL to connect to.
     * @return {@code true} if the driver thinks that is can open a connection
     *         to the supplied URL, {@code false} otherwise. Typically, the
     *         driver will respond {@code true} if it thinks that it can handle
     *         the subprotocol specified by the driver.
     * @throws SQLException
     *          if a database error occurs.
     */
    virtual CARAPI AcceptsURL(
    	/** [in] **/ String url,
    	/** [out] **/ Boolean *result)=0;

    /**
     * Attempts to make a database connection to a data source specified by a
     * supplied URL.
     *
     * @param url
     *            the URL to connect.
     * @param info
     *            some properties that should be used in establishing the
     *            connection. The properties consist of name/value pairs of
     *            strings. Normally, a connection to a database requires at
     *            least two properties - for {@code "user"} and {@code
     *            "password"} in order to pass authentication to the database.
     * @return the connection to the database.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI Connect(
        /** [in] **/ String url, 
        /** [in] **/ IProperties* info,
        /** [out] **/IConnection** result)=0;

    /**
     * Gets the driver's major version number.
     *
     * @return the major version number of the driver - typically starts at 1.
     */
    virtual CARAPI GetMajorVersion(
        /** [out] **/Int32* version);

    /**
     * Gets the driver's minor version number.
     *
     * @return the minor version number of the driver - typically starts at 0.
     */
    virtual CARAPI GetMinorVersion(
        /** [out] **/Int32* version);

    /**
     * Gets information about possible properties for this driver.
     * <p>
     * This method is intended to provide a listing of possible properties that
     * the client of the driver must supply in order to establish a connection
     * to a database. Note that the returned array of properties may change
     * depending on the supplied list of property values.
     *
     * @param url
     *            the URL of the database. An application may call this method
     *            iteratively as the property list is built up - for example,
     *            when displaying a dialog to an end-user as part of the
     *            database login process.
     * @param info
     *            a set of tag/value pairs giving data that a user may be
     *            prompted to provide in order to connect to the database.
     * @return an array of {@code DriverPropertyInfo} records which provide
     *         details on which additional properties are required (in addition
     *         to those supplied in the {@code info} parameter) in order to
     *         connect to the database.
     * @throws SQLException
     *             if a database error occurs.
     */
    virtual CARAPI GetPropertyInfo(
        /** [in] **/String url, 
        /** [in] **/IProperties* info,
        /** [out,collee] **/ArrayOf<IDriverPropertyInfo*> info)=0;

    /**
     * Reports whether this driver is a genuine JDBC CompliantTM driver. The
     * driver may only return {@code true} if it passes all the JDBC compliance
     * tests.
     * <p>
     * A driver may not be fully compliant if the underlying database has
     * limited functionality.
     *
     * @return {@code true} if the driver is fully JDBC compliant, {@code false}
     *         otherwise.
     */
    virtual CARAPI JdbcCompliant(
        /** [out] **/ Boolean *result);
};
#endif //_DRIIVER_H_