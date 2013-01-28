#ifdef _DRIVERMANAGER_H_
#define _DRIVERMANAGER_H_

#include <elastos.h>
#include <AutoPtr.h>


using namespace Elastos;

class DriverManager {
    static {
        LoadInitialDrivers();
    }

    /**
     * Removes a driver from the {@code DriverManager}'s registered driver list.
     * This will only succeed when the caller's class loader loaded the driver
     * that is to be removed. If the driver was loaded by a different class
     * loader, the removal of the driver fails silently.
     * <p>
     * If the removal succeeds, the {@code DriverManager} will not use this
     * driver in the future when asked to get a {@code Connection}.
     *
     * @param driver
     *            the JDBC driver to remove.
     * @throws SQLException
     *             if there is a problem interfering with accessing the
     *             database.
     */
    CARAPI static DeregisterDriver(
        /**[in]**/IDriver* driver);

    /**
     * Attempts to establish a connection to the given database URL.
     *
     * @param url
     *            a URL string representing the database target to connect with.
     * @return a {@code Connection} to the database identified by the URL.
     *         {@code null} if no connection can be established.
     * @throws SQLException
     *             if there is an error while attempting to connect to the
     *             database identified by the URL.
     */
    CARAPI static GetConnection(
        /**[in]**/String url,
        /**[out]**/IConnection** result);

    /**
     * Attempts to establish a connection to the given database URL.
     *
     * @param url
     *            a URL string representing the database target to connect with
     * @param info
     *            a set of properties to use as arguments to set up the
     *            connection. Properties are arbitrary string/value pairs.
     *            Normally, at least the properties {@code "user"} and {@code
     *            "password"} should be passed, with appropriate settings for
     *            the user ID and its corresponding password to get access to
     *            the corresponding database.
     * @return a {@code Connection} to the database identified by the URL.
     *         {@code null} if no connection can be established.
     * @throws SQLException
     *             if there is an error while attempting to connect to the
     *             database identified by the URL.
     */
    CARAPI static GetConnectionEx(
        /**[in]**/String url, 
        /**[in]**/Properties info,
        /**[out]**/IConnection** result);

    /**
     * Attempts to establish a connection to the given database URL.
     *
     * @param url
     *            a URL string representing the database target to connect with.
     * @param user
     *            a user ID used to login to the database.
     * @param password
     *            a password for the user ID to login to the database.
     * @return a {@code Connection} to the database identified by the URL.
     *         {@code null} if no connection can be established.
     * @throws SQLException
     *             if there is an error while attempting to connect to the
     *             database identified by the URL.
     */
    CARAPI static GetConnectionEx2(
        /**[in]**/String url, 
        /**[in]**/String user,
        /**[in]**/String password,
        /**[out]**/IConnection** result);

    /**
     * Tries to find a driver that can interpret the supplied URL.
     *
     * @param url
     *            the URL of a database.
     * @return a {@code Driver} that matches the provided URL. {@code null} if
     *         no {@code Driver} understands the URL
     * @throws SQLException
     *             if there is any kind of problem accessing the database.
     */
    CARAPI static GetDriver(
        /**[in]**/String url,
        /**[out]**/IDriver** result);

    /**
     * Returns an {@code Enumeration} that contains all of the loaded JDBC
     * drivers that the current caller can access.
     *
     * @return An {@code Enumeration} containing all the currently loaded JDBC
     *         {@code Drivers}.
     */
//    CARAPI static Enumeration<Driver> GetDrivers();

    /**
     * Returns the login timeout when connecting to a database in seconds.
     *
     * @return the login timeout in seconds.
     */
    CARAPI static GetLoginTimeout(
        /**[out]**/Int32* result);

    /**
     * Gets the log {@code PrintStream} used by the {@code DriverManager} and
     * all the JDBC Drivers.
     *
     * @deprecated use {@link #getLogWriter()} instead.
     * @return the {@code PrintStream} used for logging activities.
     */
    //@Deprecated
    CARAPI static GetLogStream(
        /**[out]**/IPrintStream** stream);

    /**
     * Retrieves the log writer.
     *
     * @return A {@code PrintWriter} object used as the log writer. {@code null}
     *         if no log writer is set.
     */
    CARAPI static GetLogWriter(
        /**[out]**/IPrintWriter** result);

    /**
     * Prints a message to the current JDBC log stream. This is either the
     * {@code PrintWriter} or (deprecated) the {@code PrintStream}, if set.
     *
     * @param message
     *            the message to print to the JDBC log stream.
     */
    CARAPI static Println(
        /**[in]**/String message);

    /**
     * Registers a given JDBC driver with the {@code DriverManager}.
     * <p>
     * A newly loaded JDBC driver class should register itself with the
     * {@code DriverManager} by calling this method.
     *
     * @param driver
     *            the {@code Driver} to register with the {@code DriverManager}.
     * @throws SQLException
     *             if a database access error occurs.
     */
    CARAPI static RegisterDriver(
        /**[in]**/IDriver* driver);

    /**
     * Sets the login timeout when connecting to a database in seconds.
     *
     * @param seconds
     *            seconds until timeout. 0 indicates wait forever.
     */
    CARAPI static SetLoginTimeout(
        /**[in]**/Int32 seconds);

    /**
     * Sets the print stream to use for logging data from the {@code
     * DriverManager} and the JDBC drivers.
     *
     * @deprecated Use {@link #setLogWriter} instead.
     * @param out
     *            the {@code PrintStream} to use for logging.
     */
    //@Deprecated
    CARAPI static SetLogStream(
        /**[in]**/IPrintStream* stream);

    /**
     * Sets the {@code PrintWriter} that is used by all loaded drivers, and also
     * the {@code DriverManager}.
     *
     * @param out
     *            the {@code PrintWriter} to be used.
     */
    CARAPI static SetLogWriter(
        /**[in]**/IPrintWriter* stream);

private:
    /*
     * Loads the set of JDBC drivers defined by the Property "jdbc.drivers" if
     * it is defined.
     */
    CARAPI static LoadInitialDrivers();

    /*
     * A private constructor to prevent allocation
     */
    DriverManager();

    /*
     * Method which checks to see if setting a logging stream is allowed by the
     * Security manager
     */
    CARAPI static CheckLogSecurity();

    /**
     * Determines whether the supplied object was loaded by the given {@code ClassLoader}.
     *
     * @param theObject
     *            the object to check.
     * @param theClassLoader
     *            the {@code ClassLoader}.
     * @return {@code true} if the Object does belong to the {@code ClassLoader}
     *         , {@code false} otherwise
     */
    CARAPI static IsClassFromClassLoader(
        /**[in]**/IInterface* theObject,
        /**[in]**/IClassLoader* theClassLoader,
        /**[out]**/Boolean* result);
private:
    /*
     * Facilities for logging. The Print Stream is deprecated but is maintained
     * here for compatibility.
     */
    static AutoPtr<IPrintStream> mThePrintStream;

    static AutoPtr<IPrintWriter> mThePrintWriter;

    // Login timeout value - by default set to 0 -> "wait forever"
    static Int32 mLoginTimeout = 0;

    /*
     * Set to hold Registered Drivers - initial capacity 10 drivers (will expand
     * automatically if necessary.
     */
    static const List<Driver> mTheDrivers = new ArrayList<Driver>(10);

    // Permission for setting log
    static const AutoPtr<ISQLPermission> mLogPermission = new SQLPermission("setLog");
};
#endif//_DRIVERMANAGER_H_