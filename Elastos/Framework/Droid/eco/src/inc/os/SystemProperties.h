
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
     * Get the value for the given key, and return as an integer.
     * @param key the key to lookup
     * @param def a default value to return
     * @return the key parsed as an integer, or def if the key isn't found or
     *         cannot be parsed
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI_(Int32) GetInt32(
        /* [in] */ String key,
        /* [in] */ Int32 def);

    /**
     * Get the value for the given key.
     * @return an empty string if the key isn't found
     * @throws IllegalArgumentException if the key exceeds 32 characters
     */
    static CARAPI Get(
        /* [in] */ String key,
        /* [in] */ String* value) { return E_NOT_IMPLEMENTED; }
};

#endif // __HH_SYSTEM_PROPERTIES_H

