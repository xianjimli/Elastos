#ifndef __ICUCOLLATIONKEY_H__
#define __ICUCOLLATIONKEY_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include "CollationKey.h"
#include <Math.h>

using namespace Elastos;
using namespace Elastos::Core;

class ICUCollationKey : public CollationKey
{
public:
    CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);
    CARAPI Init(
        /* [in] */ String source,
        /* [in] */ ArrayOf<Byte>* bytes);

    ~ICUCollationKey();

    CARAPI CompareTo(
        /* [in] */ ICollationKey * value,
        /* [out] */ Int32 * result);

    /**
     * Checks if target object is equal to this object.
     * Target is first casted to CollationKey and bitwise compared.
     * @param target comparison object
     * @return true if both objects are equal, false otherwise
     * @stable ICU 2.4
     */
    CARAPI Equals(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * result);

    /**
     * Creates a hash code for this CollationKey.
     * Compute the hash by iterating sparsely over about 32 (up to 63) bytes
     * spaced evenly through the string.  For each byte, multiply the previous
     * hash value by a prime number and add the new byte in, like a linear
     * congruential random number generator, producing a pseudo-random
     * deterministic value well distributed over the output range.
     * @return hash value of collation key. Hash value is never 0.
     * @stable ICU 2.4
     */
    CARAPI HashCode(
        /* [out] */ Int32 * value);

    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte> ** array);
private:
    /**
     * The key.
     */
    ArrayOf<Byte>* mBytes;

    /**
     * Cached hash value.
     */
    Int32 hashCode;

};
#endif //__ICUCOLLATIONKEY_H__