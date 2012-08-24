
#ifndef __HH_SYSTEM_PROPERTIES_H
#define __HH_SYSTEM_PROPERTIES_H

#include "ext/frameworkext.h"
#include <elastos.h>
using namespace Elastos;

/**
 * Gives access to the system properties store.  The system properties
 * store contains a list of string key-value pairs.
 *
 * {@hide}
 */
class SystemProperties
{
public:
    /**
     * Get the value for the given key.
     * @return an empty string if the key isn't found
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(String) Get(
        /* [in] */ CString key);

    /**
     * Get the value for the given key.
     * @return if the key isn't found, return def if it isn't null, or an empty string otherwise
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(String) Get(
        /* [in] */ CString key,
        /* [in] */ CString def);

    /**
     * Get the value for the given key, and return as an integer.
     * @param key the key to lookup
     * @param def a default value to return
     * @return the key parsed as an integer, or def if the key isn't found or
     *         cannot be parsed
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(Int32) GetInt32(
        /* [in] */ CString key,
        /* [in] */ Int32 def);

    /**
     * Get the value for the given key, and return as a long.
     * @param key the key to lookup
     * @param def a default value to return
     * @return the key parsed as a long, or def if the key isn't found or
     *         cannot be parsed
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(Int64) GetInt64(
        /* [in] */ CString key,
        /* [in] */ Int64 def);

    /**
     * Get the value for the given key, returned as a boolean.
     * Values 'n', 'no', '0', 'false' or 'off' are considered false.
     * Values 'y', 'yes', '1', 'true' or 'on' are considered true.
     * (case insensitive).
     * If the key does not exist, or has any other value, then the default
     * result is returned.
     * @param key the key to lookup
     * @param def a default value to return
     * @return the key parsed as a boolean, or def if the key isn't found or is
     *         not able to be parsed as a boolean.
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(Boolean) GetBoolean(
        /* [in] */ CString key,
        /* [in] */ Boolean def);

    /**
     * Set the value for the given key.
     * @throws IllegalArgumentException if the key exceeds 32 characters
     * @throws IllegalArgumentException if the value exceeds 92 characters
     */
    static CARAPI_(void) Set(
        /* [in] */ CString key,
        /* [in] */ CString val);

private:
    static CARAPI_(String) NativeGet(
        /* [in] */ CString key);

    static CARAPI_(String) NativeGet(
        /* [in] */ CString key,
        /* [in] */ CString def);

    static CARAPI_(Int32) NativeGetInt32(
        /* [in] */ CString key,
        /* [in] */ Int32 def);

    static CARAPI_(Int64) NativeGetInt64(
        /* [in] */ CString key,
        /* [in] */ Int64 def);

    static CARAPI_(Boolean) NativeGetBoolean(
        /* [in] */ CString key,
        /* [in] */ Boolean def);

    static CARAPI_(void) NativeSet(
        /* [in] */ CString key,
        /* [in] */ CString val);

public:
    static const Int32 PROP_NAME_MAX = 31;
    static const Int32 PROP_VALUE_MAX = 91;
};

#endif // __HH_SYSTEM_PROPERTIES_H

