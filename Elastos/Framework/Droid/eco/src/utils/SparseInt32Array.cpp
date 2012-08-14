
#include "utils/SparseInt32Array.h"

/**
 * Creates a new SparseInt32Array containing no mappings.
 */
//SparseInt32Array::SparseInt32Array() {
//    this(10);
//}

/**
 * Creates a new SparseInt32Array containing no mappings that will not
 * require any additional memory allocation to store the specified
 * number of mappings.
 */
SparseInt32Array::SparseInt32Array(
    /* [in] */ Int32 initialCapacity)
{
    initialCapacity = ArrayUtils::IdealInt32ArraySize(initialCapacity);

    /*mKeys = new Int32[initialCapacity];
    mValues = new Int32[initialCapacity];*/
    mKeys = ArrayOf<Int32>::Alloc(initialCapacity);
    mValues = ArrayOf<Int32>::Alloc(initialCapacity);
    mSize = 0;
}

/**
 * Gets the Int32 mapped from the specified key, or <code>0</code>
 * if no such mapping has been made.
 */
Int32 SparseInt32Array::Get(
    /* [in] */ Int32 key)
{
    return Get(key, 0);
}

/**
 * Gets the Int32 mapped from the specified key, or the specified value
 * if no such mapping has been made.
 */
Int32 SparseInt32Array::Get(
    /* [in] */ Int32 key,
    /* [in] */ Int32 valueIfKeyNotFound)
{
    Int32 i = BinarySearch(mKeys, 0, mSize, key);

    if (i < 0) {
        return valueIfKeyNotFound;
    } else {
        return (*mValues)[i];
    }
}

/**
 * Removes the mapping from the specified key, if there was any.
 */
void SparseInt32Array::Delete(
    /* [in] */ Int32 key)
{
    Int32 i = BinarySearch(mKeys, 0, mSize, key);

    if (i >= 0) {
        RemoveAt(i);
    }
}

/**
 * Removes the mapping at the given index.
 */
void SparseInt32Array::RemoveAt(
    /* [in] */ Int32 index)
{
    //System.arraycopy(mKeys, index + 1, mKeys, index, mSize - (index + 1));
    memmove(&(*mKeys)[index], &(*mKeys)[index + 1], (mSize - index - 1) * sizeof(Int32));
    //System.arraycopy(mValues, index + 1, mValues, index, mSize - (index + 1));
    memmove(&(*mValues)[index], &(*mValues)[index + 1], (mSize - index - 1) * sizeof(Int32));
    mSize--;
}

/**
 * Adds a mapping from the specified key to the specified value,
 * replacing the previous mapping from the specified key if there
 * was one.
 */
void SparseInt32Array::Put(
    /* [in] */ Int32 key,
    /* [in] */ Int32 value)
{
    Int32 i = BinarySearch(mKeys, 0, mSize, key);

    if (i >= 0) {
        (*mValues)[i] = value;
    } else {
        i = ~i;

        if (mSize >= mKeys->GetLength()) {
            Int32 n = ArrayUtils::IdealInt32ArraySize(mSize + 1);

            ArrayOf<Int32>* nkeys = ArrayOf<Int32>::Alloc(n);
            ArrayOf<Int32>* nvalues = ArrayOf<Int32>::Alloc(n);

            // Log.e("SparseInt32Array", "grow " + mKeys.length + " to " + n);
            //System.arraycopy(mKeys, 0, nkeys, 0, mKeys.length);
            memcpy(nkeys->GetPayload(), mKeys->GetPayload(), mKeys->GetLength() * sizeof(Int32));
            memcpy(nvalues->GetPayload(), mValues->GetPayload(), mValues->GetLength() * sizeof(Int32));

            mKeys = nkeys;
            mValues = nvalues;
        }

        if (mSize - i != 0) {
            // Log.e("SparseInt32Array", "move " + (mSize - i));
            //System.arraycopy(mKeys, i, mKeys, i + 1, mSize - i);
            memmove(&(*mKeys)[i+1], &(*mKeys)[i], (mSize - i) * sizeof(Int32));
            //System.arraycopy(mValues, i, mValues, i + 1, mSize - i);
            memmove(&(*mValues)[i+1], &(*mValues)[i], (mSize - i) * sizeof(Int32));
        }

        (*mKeys)[i] = key;
        (*mValues)[i] = value;
        mSize++;
    }
}

/**
 * Returns the number of key-value mappings that this SparseInt32Array
 * currently stores.
 */
Int32 SparseInt32Array::Size()
{
    return mSize;
}

/**
 * Given an index in the range <code>0...size()-1</code>, returns
 * the key from the <code>index</code>th key-value mapping that this
 * SparseInt32Array stores.
 */
Int32 SparseInt32Array::KeyAt(
    /* [in] */ Int32 index)
{
    return (*mKeys)[index];
}

/**
 * Given an index in the range <code>0...size()-1</code>, returns
 * the value from the <code>index</code>th key-value mapping that this
 * SparseInt32Array stores.
 */
Int32 SparseInt32Array::ValueAt(
    /* [in] */ Int32 index)
{
    return (*mValues)[index];
}

/**
 * Returns the index for which {@link #keyAt} would return the
 * specified key, or a negative number if the specified
 * key is not mapped.
 */
Int32 SparseInt32Array::IndexOfKey(
    /* [in] */ Int32 key)
{
    return BinarySearch(mKeys, 0, mSize, key);
}

/**
 * Returns an index for which {@link #valueAt} would return the
 * specified key, or a negative number if no keys map to the
 * specified value.
 * Beware that this is a linear search, unlike lookups by key,
 * and that multiple keys can map to the same value and this will
 * find only one of them.
 */
Int32 SparseInt32Array::IndexOfValue(
    /* [in] */ Int32 value)
{
    for (Int32 i = 0; i < mSize; i++)
        if ((*mValues)[i] == value)
            return i;

    return -1;
}

/**
 * Removes all key-value mappings from this SparseInt32Array.
 */
void SparseInt32Array::Clear()
{
    mSize = 0;
}

/**
 * Puts a key/value pair into the array, optimizing for the case where
 * the key is greater than all existing keys in the array.
 */
void SparseInt32Array::Append(
    /* [in] */ Int32 key,
    /* [in] */ Int32 value)
{
    if (mSize != 0 && key <= (*mKeys)[mSize - 1]) {
        Put(key, value);
        return;
    }

    Int32 pos = mSize;
    if (pos >= mKeys->GetLength()) {
        Int32 n = ArrayUtils::IdealInt32ArraySize(pos + 1);

        ArrayOf<Int32>* nkeys = ArrayOf<Int32>::Alloc(n);
        ArrayOf<Int32>* nvalues = ArrayOf<Int32>::Alloc(n);

        // Log.e("SparseInt32Array", "grow " + mKeys.length + " to " + n);
        //System.arraycopy(mKeys, 0, nkeys, 0, mKeys.length);
        memcpy(nkeys->GetPayload(), mKeys->GetPayload(), mKeys->GetLength() * sizeof(Int32));
        //System.arraycopy(mValues, 0, nvalues, 0, mValues.length);
        memcpy(nvalues->GetPayload(), mValues->GetPayload(), mValues->GetLength() * sizeof(Int32));

        mKeys = nkeys;
        mValues = nvalues;
    }

    (*mKeys)[pos] = key;
    (*mValues)[pos] = value;
    mSize = pos + 1;
}

Int32 SparseInt32Array::BinarySearch(
    /* [in] */ ArrayOf<Int32>* a,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [in] */ Int32 key)
{
    Int32 high = start + len, low = start - 1, guess;

    while (high - low > 1) {
        guess = (high + low) / 2;

        if ((*a)[guess] < key)
            low = guess;
        else
            high = guess;
    }

    if (high == start + len)
        return ~(start + len);
    else if ((*a)[high] == key)
        return high;
    else
        return ~high;
}

void SparseInt32Array::CheckIntegrity()
{
    /*for (Int32 i = 1; i < mSize; i++) {
        if (mKeys[i] <= mKeys[i - 1]) {
            for (Int32 j = 0; j < mSize; j++) {
                Log.e("FAIL", j + ": " + mKeys[j] + " -> " + mValues[j]);
            }

            throw new RuntimeException();
        }
    }*/
}


