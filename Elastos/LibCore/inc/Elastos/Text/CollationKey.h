#ifndef __COLLATIONKEY_H__
#define __COLLATIONKEY_H__

#include <elastos.h>
#include "cmdef.h"
#include "Elastos.Text_server.h"

extern "C" const InterfaceID EIID_CollationKey;

class CollationKey {
public:
    virtual CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid) = 0;
    /**
     * Compares this collation key to the given collation key.
     *
     * @param value the other collation key.
     * @return a negative value if this key is less than {@code value},
     *         0 if they are equal, and a positive value if this key is greater.
     */
    virtual CARAPI CompareTo(
        /* [in] */ ICollationKey* value,
        /* [out] */ Int32* result) = 0;

    /**
     * Returns the string from which this collation key was created.
     *
     * @return the source string of this collation key.
     */
    virtual CARAPI GetSourceString(
        /* [out] */ String* sourceString);

    /**
     * Returns this collation key as a byte array.
     *
     * @return an array of bytes.
     */
    virtual CARAPI GetByteArray(
        /* [out, callee] */ ArrayOf<Byte>** array) = 0;

protected:
    CollationKey();

    CARAPI Init(
        /* [in] */ String source);

private:
    String source;
};
#endif //__COLLATIONKEY_H__