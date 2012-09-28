
#ifndef __ADDRESSCACHE_H__
#define __ADDRESSCACHE_H__

#include "cmdef.h"
#include "Elastos.Net_server.h"
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

/**
 * Implements caching for {@code InetAddress}. We use a unified cache for both positive and negative
 * cache entries.
 */
class AddressCache : public ElRefBase
{
private:
    class AddressCacheEntry
    {
    public:
        AddressCacheEntry(
            /* [in] */ ArrayOf<IInetAddress*>* addresses,
            /* [in] */ Int64 expiryNanos)
            : mAddresses(addresses)
            , mExpiryNanos(expiryNanos)
        {}

    public:
        // The addresses. May be the empty array for a negative cache entry.
        ArrayOf<IInetAddress*>* mAddresses;

        /**
         * The absolute expiry time in nanoseconds. Nanoseconds from System.nanoTime is ideal
         * because -- unlike System.currentTimeMillis -- it can never go backwards.
         *
         * Unless we need to cope with DNS TTLs of 292 years, we don't need to worry about overflow.
         */
        Int64 mExpiryNanos;
    };

public:
    AddressCache();

    ~AddressCache();

    /**
     * Returns the cached addresses associated with 'hostname'. Returns null if nothing is known
     * about 'hostname'. Returns an empty array if 'hostname' is known not to exist.
     */
    CARAPI_(ArrayOf<IInetAddress*>*) Get(
        /* [in] */ const String& hostname);

    /**
     * Associates the given 'addresses' with 'hostname'. The association will expire after a
     * certain length of time.
     */
    CARAPI_(void) Put(
        /* [in] */ const String& hostname,
        /* [in] */ ArrayOf<IInetAddress*>* addresses);

    /**
     * Records that 'hostname' is known not to have any associated addresses. (I.e. insert a
     * negative cache entry.)
     */
    CARAPI_(void) PutUnknownHost(
        /* [in] */ const String& hostname);

private:
    CARAPI_(Int64) CustomTtl(
        /* [in] */ const String& propertyName,
        /* [in] */ Int64 defaultTtlNanos);

private:
    /**
     * When the cache contains more entries than this, we start dropping the oldest ones.
     * This should be a power of two to avoid wasted space in our custom map.
     */
    static const Int32 MAX_ENTRIES = 512;

    // This isn't used by our HashMap implementation, but the API demands it.
    static const Float DEFAULT_LOAD_FACTOR = .75f;

    // Default time-to-live for positive cache entries. 600 seconds (10 minutes).
    static const Int64 DEFAULT_POSITIVE_TTL_NANOS = 600 * 1000000000ll;
    // Default time-to-live for negative cache entries. 10 seconds.
    static const Int64 DEFAULT_NEGATIVE_TTL_NANOS = 10 * 1000000000ll;

    // Failed lookups are represented in the cache my mappings to this empty array.
    static ArrayOf_<IInetAddress*, 1> NO_ADDRESSES;

    // The actual cache.
    HashMap<String, AddressCacheEntry*>* mMap;
    Mutex mMapLock;
};

#endif //__ADDRESSCACHE_H__
