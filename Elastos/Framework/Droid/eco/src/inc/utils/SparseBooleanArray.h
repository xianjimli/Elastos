
#ifndef __SPARSEBOOLEANARRAY_H__
#define __SPARSEBOOLEANARRAY_H__

#include "utils/ArrayUtils.h"

/**
 * SparseBooleanArrays map integers to booleans.
 * Unlike a normal array of booleans
 * there can be gaps in the indices.  It is intended to be more efficient
 * than using a HashMap to map Integers to Booleans.
 */
class SparseBooleanArray 
{
public:
    /**
     * Creates a new SparseBooleanArray containing no mappings.
     */
    //SparseBooleanArray();

    /**
     * Creates a new SparseBooleanArray containing no mappings that will not
     * require any additional memory allocation to store the specified
     * number of mappings.
     */
    SparseBooleanArray(
        /* [in] */ Int32 initialCapacity = 10);

    ~SparseBooleanArray();

    /**
     * Gets the Boolean mapped from the specified key, or <code>false</code>
     * if no such mapping has been made.
     */
    CARAPI_(Boolean) Get(
        /* [in] */ Int32 key);

    /**
     * Gets the Boolean mapped from the specified key, or the specified value
     * if no such mapping has been made.
     */
    CARAPI_(Boolean) Get(
        /* [in] */ Int32 key, 
        /* [in] */ Boolean valueIfKeyNotFound);

    /**
     * Removes the mapping from the specified key, if there was any.
     */
    CARAPI_(void) Delete(
        /* [in] */ Int32 key);

    /**
     * Adds a mapping from the specified key to the specified value,
     * replacing the previous mapping from the specified key if there
     * was one.
     */
    CARAPI_(void) Put(
        /* [in] */ Int32 key, 
        /* [in] */ Boolean value);

    /**
     * Returns the number of key-value mappings that this SparseBooleanArray
     * currently stores.
     */
    CARAPI_(Int32) Size();

    /**
     * Given an index in the range <code>0...size()-1</code>, returns
     * the key from the <code>index</code>th key-value mapping that this
     * SparseBooleanArray stores.  
     */
    CARAPI_(Int32) KeyAt(
        /* [in] */ Int32 index);
    
    /**
     * Given an index in the range <code>0...size()-1</code>, returns
     * the value from the <code>index</code>th key-value mapping that this
     * SparseBooleanArray stores.  
     */
    CARAPI_(Boolean) ValueAt(
        /* [in] */ Int32 index);

    /**
     * Returns the index for which {@link #keyAt} would return the
     * specified key, or a negative number if the specified
     * key is not mapped.
     */
    CARAPI_(Int32) IndexOfKey(
        /* [in] */ Int32 key);

    /**
     * Returns an index for which {@link #valueAt} would return the
     * specified key, or a negative number if no keys map to the
     * specified value.
     * Beware that this is a linear search, unlike lookups by key,
     * and that multiple keys can map to the same value and this will
     * find only one of them.
     */
    CARAPI_(Int32) IndexOfValue(
        /* [in] */ Boolean value);

    /**
     * Removes all key-value mappings from this SparseBooleanArray.
     */
    CARAPI_(void) Clear();

    /**
     * Puts a key/value pair into the array, optimizing for the case where
     * the key is greater than all existing keys in the array.
     */
    CARAPI_(void) Append(
        /* [in] */ Int32 key, 
        /* [in] */ Boolean value);
   
private:
    static CARAPI_(Int32) BinarySearch(
        /* [in] */ ArrayOf<Int32>* a, 
        /* [in] */ Int32 start, 
        /* [in] */ Int32 len, 
        /* [in] */ Int32 key);

    CARAPI_(void) CheckIntegrity();

    ArrayOf<Int32>* mKeys;
    ArrayOf<Boolean>* mValues;
    Int32 mSize;
};

#endif
