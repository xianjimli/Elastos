
#include "utils/SparseBooleanArray.h"


/**
 * Creates a new SparseBooleanArray containing no mappings.
 */
//SparseBooleanArray::SparseBooleanArray()
//{
//    this(10);
//}

/**
 * Creates a new SparseBooleanArray containing no mappings that will not
 * require any additional memory allocation to store the specified
 * number of mappings.
 */
SparseBooleanArray::SparseBooleanArray(
    /* [in] */ Int32 initialCapacity)
{
    initialCapacity = ArrayUtils::IdealInt32ArraySize(initialCapacity);

    mKeys = ArrayOf<Int32>::Alloc(initialCapacity);
    mValues = ArrayOf<Boolean>::Alloc(initialCapacity);
    mSize = 0;
}

SparseBooleanArray::~SparseBooleanArray()
{
    ArrayOf<Int32>::Free(mKeys);
    ArrayOf<Boolean>::Free(mValues);
}

/**
 * Gets the Boolean mapped from the specified key, or <code>false</code>
 * if no such mapping has been made.
 */
Boolean SparseBooleanArray::Get(
    /* [in] */ Int32 key)
{
    return Get(key, false);
}

/**
 * Gets the Boolean mapped from the specified key, or the specified value
 * if no such mapping has been made.
 */
Boolean SparseBooleanArray::Get(
    /* [in] */ Int32 key,
    /* [in] */ Boolean valueIfKeyNotFound)
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
void SparseBooleanArray::Delete(
    /* [in] */ Int32 key)
{
    Int32 i = BinarySearch(mKeys, 0, mSize, key);

    if (i >= 0) {
        //System.arraycopy(mKeys, i + 1, mKeys, i, mSize - (i + 1));
        memmove(mKeys->GetPayload() + i, mKeys->GetPayload() + i + 1, (mSize - (i + 1)) * sizeof(Int32));

        //System.arraycopy(mValues, i + 1, mValues, i, mSize - (i + 1));
        memmove(mValues->GetPayload() + i, mValues->GetPayload() + i + 1, (mSize - (i + 1)) * sizeof(Boolean));

        mSize--;
    }
}

/**
 * Adds a mapping from the specified key to the specified value,
 * replacing the previous mapping from the specified key if there
 * was one.
 */
void SparseBooleanArray::Put(
    /* [in] */ Int32 key,
    /* [in] */ Boolean value)
{
    Int32 i = BinarySearch(mKeys, 0, mSize, key);

    if (i >= 0) {
        (*mValues)[i] = value;
    } else {
        i = ~i;

        if (mSize >= mKeys->GetLength()) {
            Int32 n = ArrayUtils::IdealInt32ArraySize(mSize + 1);

            ArrayOf<Int32>* nkeys = ArrayOf<Int32>::Alloc(n);
            ArrayOf<Boolean>* nvalues = ArrayOf<Boolean>::Alloc(n);

            // Log.e("SparseBooleanArray", "grow " + mKeys.length + " to " + n);
            //System.arraycopy(mKeys, 0, nkeys, 0, mKeys.length);
            memcpy(nkeys->GetPayload(), mKeys->GetPayload(), mKeys->GetLength() * sizeof(Int32));

            //System.arraycopy(mValues, 0, nvalues, 0, mValues.length);
            memcpy(nvalues->GetPayload(), mValues->GetPayload(), mValues->GetLength() * sizeof(Boolean));

            mKeys = nkeys;
            mValues = nvalues;
        }

        if (mSize - i != 0) {
            // Log.e("SparseBooleanArray", "move " + (mSize - i));
            //System.arraycopy(mKeys, i, mKeys, i + 1, mSize - i);
            memmove(mKeys->GetPayload() + i + 1, mKeys->GetPayload() + i, (mSize - i) * sizeof(Int32));

            //System.arraycopy(mValues, i, mValues, i + 1, mSize - i);
            memmove(mValues->GetPayload() + i + 1, mValues->GetPayload() + i, (mSize - i) * sizeof(Boolean));
        }

        (*mKeys)[i] = key;
        (*mValues)[i] = value;
        mSize++;
    }
}

/**
 * Returns the number of key-value mappings that this SparseBooleanArray
 * currently stores.
 */
Int32 SparseBooleanArray::Size()
{
    return mSize;
}

/**
 * Given an index in the range <code>0...size()-1</code>, returns
 * the key from the <code>index</code>th key-value mapping that this
 * SparseBooleanArray stores.
 */
Int32 SparseBooleanArray::KeyAt(
    /* [in] */ Int32 index)
{
    return (*mKeys)[index];
}

/**
 * Given an index in the range <code>0...size()-1</code>, returns
 * the value from the <code>index</code>th key-value mapping that this
 * SparseBooleanArray stores.
 */
Boolean SparseBooleanArray::ValueAt(
    /* [in] */ Int32 index)
{
    return (*mValues)[index];
}

/**
 * Returns the index for which {@link #keyAt} would return the
 * specified key, or a negative number if the specified
 * key is not mapped.
 */
Int32 SparseBooleanArray::IndexOfKey(
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
Int32 SparseBooleanArray::IndexOfValue(
    /* [in] */ Boolean value)
{
    for (Int32 i = 0; i < mSize; i++)
        if ((*mValues)[i] == value)
            return i;

    return -1;
}

/**
 * Removes all key-value mappings from this SparseBooleanArray.
 */
void SparseBooleanArray::Clear()
{
    mSize = 0;
}

/**
 * Puts a key/value pair into the array, optimizing for the case where
 * the key is greater than all existing keys in the array.
 */
void SparseBooleanArray::Append(
    /* [in] */ Int32 key,
    /* [in] */ Boolean value)
{
    if (mSize != 0 && key <= (*mKeys)[mSize - 1]) {
        Put(key, value);
        return;
    }

    Int32 pos = mSize;
    if (pos >= mKeys->GetLength()) {
        Int32 n = ArrayUtils::IdealInt32ArraySize(pos + 1);

        //Int32[] nkeys = new Int32[n];
        ArrayOf<Int32>* nkeys = ArrayOf<Int32>::Alloc(n);

        //Boolean[] nvalues = new Boolean[n];
        ArrayOf<Boolean>* nvalues = ArrayOf<Boolean>::Alloc(n);

        // Log.e("SparseBooleanArray", "grow " + mKeys.length + " to " + n);
        //System.arraycopy(mKeys, 0, nkeys, 0, mKeys.length);
        memcpy(nkeys->GetPayload(), mKeys->GetPayload(), mKeys->GetLength() * sizeof(Boolean));

        //System.arraycopy(mValues, 0, nvalues, 0, mValues.length);
        memcpy(nvalues->GetPayload(), mValues->GetPayload(), mValues->GetLength() * sizeof(Boolean));

        mKeys = nkeys;
        mValues = nvalues;
    }

    (*mKeys)[pos] = key;
    (*mValues)[pos] = value;
    mSize = pos + 1;
}

Int32 SparseBooleanArray::BinarySearch(
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

void SparseBooleanArray::CheckIntegrity()
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
