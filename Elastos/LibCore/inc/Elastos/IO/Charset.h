#ifndef __CHARSET_H__
#define __CHARSET_H__

#include <elastos.h>
#include <Elastos.Core.h>
#include "Elastos.IO_server.h"

using namespace Elastos;

class Charset
{
public:
    Charset();

    /**
     * Constructs a <code>Charset</code> object. Duplicated aliases are
     * ignored.
     *
     * @param canonicalName
     *            the canonical name of the charset.
     * @param aliases
     *            an array containing all aliases of the charset. May be null.
     * @throws IllegalCharsetNameException
     *             on an illegal value being supplied for either
     *             <code>canonicalName</code> or for any element of
     *             <code>aliases</code>.
     */
    CARAPI Init(
        /**in**/ String canonicalName, 
        /**in**/ const ArrayOf<String>& aliases);

    /**
     * Gets the name of this charset for the default locale.
     *
     * <p>The default implementation returns the canonical name of this charset.
     * Subclasses may return a localized display name.
     *
     * @return the name of this charset for the default locale.
     */
    CARAPI DisplayName(
        /**out**/ String* name);

    /**
     * Returns the system's default charset. This is determined during VM startup, and will not
     * change thereafter. On Android, the default charset is UTF-8.
     */
    static CARAPI DefaultCharset(
        /**out**/ ICharset** charset);
};
#endif//__CHARSET_H__