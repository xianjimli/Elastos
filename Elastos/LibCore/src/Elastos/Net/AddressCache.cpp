
#include "AddressCache.h"
#include <elastos/System.h>

using namespace Elastos::Core;

/**
 * When the cache contains more entries than this, we start dropping the oldest ones.
 * This should be a power of two to avoid wasted space in our custom map.
 */
const Int32 AddressCache::MAX_ENTRIES;

// This isn't used by our HashMap implementation, but the API demands it.
const Float AddressCache::DEFAULT_LOAD_FACTOR;

// Default time-to-live for positive cache entries. 600 seconds (10 minutes).
const Int64 AddressCache::DEFAULT_POSITIVE_TTL_NANOS;
// Default time-to-live for negative cache entries. 10 seconds.
const Int64 AddressCache::DEFAULT_NEGATIVE_TTL_NANOS;

// Failed lookups are represented in the cache my mappings to this empty array.
ArrayOf_<IInetAddress*, 1> AddressCache::NO_ADDRESSES;

AddressCache::AddressCache()
{
//    // We pass 'true' so removeEldestEntry removes the least-recently accessed entry, rather
//    // than the least-recently inserted.
//    map = new LinkedHashMap<String, AddressCacheEntry>(0, DEFAULT_LOAD_FACTOR, true) {
//        @Override protected boolean removeEldestEntry(Entry<String, AddressCacheEntry> eldest) {
//            // By the time this method is called, the new entry has already been inserted and
//            // the map will have grown to accommodate it. Using == lets us prevent resizing.
//            return size() == MAX_ENTRIES;
//        }
//    };
    mMap = new HashMap<String, AddressCacheEntry*>(11);
}

AddressCache::~AddressCache()
{
    HashMap<String, AddressCacheEntry*>::Iterator it = mMap->Begin();
    for (; it != mMap->End(); ++it) {
        if (it->mSecond != NULL) {
            delete it->mSecond;
            it->mSecond = NULL;
        }
        mMap->Erase(it);
    }
}

/**
 * Returns the cached addresses associated with 'hostname'. Returns null if nothing is known
 * about 'hostname'. Returns an empty array if 'hostname' is known not to exist.
 */
ArrayOf<IInetAddress*>* AddressCache::Get(
    /* [in] */ const String& hostname)
{
    AddressCacheEntry* entry = NULL;
    {
        Mutex::Autolock lock(mMapLock);
        HashMap<String, AddressCacheEntry*>::Iterator it = mMap->Find(hostname);
        if (it != mMap->End()) entry = it->mSecond;
    }
    // Do we have a valid cache entry?
    if (entry != NULL && entry->mExpiryNanos >= System::GetNanoTime()) {
        return entry->mAddresses;
    }
    // Either we didn't find anything, or it had expired.
    // No need to remove expired entries: the caller will provide a replacement shortly.
    return NULL;
}

/**
 * Associates the given 'addresses' with 'hostname'. The association will expire after a
 * certain length of time.
 */
void AddressCache::Put(
    /* [in] */ const String& hostname,
    /* [in] */ ArrayOf<IInetAddress*>* addresses)
{
    // Calculate the expiry time.
    Boolean isPositive = (addresses->GetLength() > 0);
    CString propertyName = isPositive ? "networkaddress.cache.ttl" : "networkaddress.cache.negative.ttl";
    Int64 defaultTtlNanos = isPositive ? DEFAULT_POSITIVE_TTL_NANOS : DEFAULT_NEGATIVE_TTL_NANOS;
    // Fast-path the default case...
    Int64 expiryNanos = System::GetNanoTime() + defaultTtlNanos;
//    if (System.getSecurityManager() != null || System.getProperty(propertyName, null) != null) {
//        // ...and let those using a SecurityManager or custom properties pay full price.
//        expiryNanos = customTtl(propertyName, defaultTtlNanos);
//        if (expiryNanos == Long.MIN_VALUE) {
//            return;
//        }
//    }
    // Update the cache.
    {
        Mutex::Autolock lock(mMapLock);
        HashMap<String, AddressCacheEntry*>::Iterator it = mMap->Find(hostname);
        if (it != mMap->End() && it->mSecond != NULL) {
            delete it->mSecond;
            it->mSecond = NULL;
        }
        (*mMap)[hostname] = new AddressCacheEntry(addresses, expiryNanos);
    }
}

/**
 * Records that 'hostname' is known not to have any associated addresses. (I.e. insert a
 * negative cache entry.)
 */
void AddressCache::PutUnknownHost(
    /* [in] */ const String& hostname)
{
    Put(hostname, &NO_ADDRESSES);
}

Int64 AddressCache::CustomTtl(
    /* [in] */ const String& propertyName,
    /* [in] */ Int64 defaultTtlNanos)
{
//    String ttlString = AccessController.doPrivileged(new PriviAction<String>(propertyName, null));
//    if (ttlString == null) {
//        return System.nanoTime() + defaultTtlNanos;
//    }
//    try {
//        long ttlS = Long.parseLong(ttlString);
//        // For the system properties, -1 means "cache forever" and 0 means "don't cache".
//        if (ttlS == -1) {
//            return Long.MAX_VALUE;
//        } else if (ttlS == 0) {
//            return Long.MIN_VALUE;
//        } else {
//            return System.nanoTime() + ttlS * 1000000000L;
//        }
//    } catch (NumberFormatException ex) {
//        return System.nanoTime() + defaultTtlNanos;
//    }
    return 0;
}
