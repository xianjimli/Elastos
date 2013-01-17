
#ifndef __URLSTREAMHANDLER_H__
#define __URLSTREAMHANDLER_H__

#include "Elastos.Net_server.h"

// extern "C" const InterfaceID EIID_URLStreamHandler;

/**
 * The abstract class {@code URLStreamHandler} is the base for all classes which
 * can handle the communication with a URL object over a particular protocol
 * type.
 */
class URLStreamHandler
{
public:
    /**
     * Establishes a new connection to the resource specified by the URL {@code
     * u}. Since different protocols also have unique ways of connecting, it
     * must be overwritten by the subclass.
     *
     * @param u
     *            the URL to the resource where a connection has to be opened.
     * @return the opened URLConnection to the specified resource.
     * @throws IOException
     *             if an I/O error occurs during opening the connection.
     */
    virtual CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [out] */ IURLConnection** urlConnection) = 0;

    /**
     * Establishes a new connection to the resource specified by the URL {@code
     * u} using the given {@code proxy}. Since different protocols also have
     * unique ways of connecting, it must be overwritten by the subclass.
     *
     * @param u
     *            the URL to the resource where a connection has to be opened.
     * @param proxy
     *            the proxy that is used to make the connection.
     * @return the opened URLConnection to the specified resource.
     * @throws IOException
     *             if an I/O error occurs during opening the connection.
     * @throws IllegalArgumentException
     *             if any argument is {@code null} or the type of proxy is
     *             wrong.
     * @throws UnsupportedOperationException
     *             if the protocol handler doesn't support this method.
     */
    virtual CARAPI OpenConnectionEx(
        /* [in] */ IURL* u,
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** urlConnection);

    /**
     * Parses the clear text URL in {@code str} into a URL object. URL strings
     * generally have the following format:
     * <p>
     * http://www.company.com/java/file1.java#reference
     * <p>
     * The string is parsed in HTTP format. If the protocol has a different URL
     * format this method must be overridden.
     *
     * @param u
     *            the URL to fill in the parsed clear text URL parts.
     * @param str
     *            the URL string that is to be parsed.
     * @param start
     *            the string position from where to begin parsing.
     * @param end
     *            the string position to stop parsing.
     * @see #toExternalForm
     * @see URL
     */
    virtual CARAPI ParseURL(
        /* [in] */ IURL* u,
        /* [in] */ const String& str,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Sets the fields of the URL {@code u} to the values of the supplied
     * arguments.
     *
     * @param u
     *            the non-null URL object to be set.
     * @param protocol
     *            the protocol.
     * @param host
     *            the host name.
     * @param port
     *            the port number.
     * @param file
     *            the file component.
     * @param ref
     *            the reference.
     * @deprecated use setURL(URL, String String, int, String, String, String,
     *             String, String) instead.
     */
    // @Deprecated
    virtual CARAPI SetURL(
        /* [in] */ IURL* u,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ const String& ref);

    /**
     * Sets the fields of the URL {@code u} to the values of the supplied
     * arguments.
     *
     * @param u
     *            the non-null URL object to be set.
     * @param protocol
     *            the protocol.
     * @param host
     *            the host name.
     * @param port
     *            the port number.
     * @param authority
     *            the authority.
     * @param userInfo
     *            the user info.
     * @param file
     *            the file component.
     * @param query
     *            the query.
     * @param ref
     *            the reference.
     */
    virtual CARAPI SetURLEx(
        /* [in] */ IURL* u,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& authority,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& file,
        /* [in] */ const String& query,
        /* [in] */ const String& ref);

    /**
     * Returns the clear text representation of a given URL using HTTP format.
     *
     * @param url
     *            the URL object to be converted.
     * @return the clear text representation of the specified URL.
     * @see #parseURL
     * @see URL#toExternalForm()
     */
    virtual CARAPI ToExternalForm(
        /* [in] */ IURL* url,
        /* [out] */ String* s);

    /**
     * Compares two URL objects whether they represent the same URL. Two URLs
     * are equal if they have the same file, host, port, protocol, query, and
     * reference components.
     *
     * @param url1
     *            the first URL to compare.
     * @param url2
     *            the second URL to compare.
     * @return {@code true} if the URLs are the same, {@code false} otherwise.
     * @see #hashCode
     */
    virtual CARAPI Equals(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isEquals);

    /**
     * Returns the default port of the protocol used by the handled URL. The
     * current implementation returns always {@code -1}.
     *
     * @return the appropriate default port number of the protocol.
     */
    virtual CARAPI GetDefaultPort(
        /* [out] */ Int32* portNum);

    /**
     * Returns the host address of the given URL.
     *
     * @param url
     *            the URL object where to read the host address from.
     * @return the host address of the specified URL.
     */
    virtual CARAPI GetHostAddress(
        /* [in] */ IURL* url,
        /* [out] */ IInetAddress** hostAddress);

    /**
     * Returns the hashcode value for the given URL object.
     *
     * @param url
     *            the URL to determine the hashcode.
     * @return the hashcode of the given URL.
     */
    virtual CARAPI HashCode(
        /* [in] */ IURL* url,
        /* [out] */ Int32* code);

    /**
     * Compares two URL objects whether they refer to the same host.
     *
     * @param a the first URL to be compared.
     * @param b the second URL to be compared.
     * @return {@code true} if both URLs refer to the same host, {@code false}
     *         otherwise.
     */
    virtual CARAPI HostsEqual(
        /* [in] */ IURL* a,
        /* [in] */ IURL* b,
        /* [out] */ Boolean* isEqual);

    /**
     * Compares two URL objects whether they refer to the same file. In the
     * comparison included are the URL components protocol, host, port and file.
     *
     * @param url1
     *            the first URL to be compared.
     * @param url2
     *            the second URL to be compared.
     * @return {@code true} if both URLs refer to the same file, {@code false}
     *         otherwise.
     */
    virtual CARAPI SameFile(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isSame);

private:
    /*
     * If the URL host is empty while protocal is file, the host is regarded as
     * localhost.
     */
    static CARAPI_(String) GetHost(
        /* [in] */ IURL* url);
};

#endif //__URLSTREAMHANDLER_H__
