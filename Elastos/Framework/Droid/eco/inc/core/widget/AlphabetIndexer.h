
#ifndef __ALPHABETINDEXER_H__
#define __ALPHABETINDEXER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "utils/SparseInt32Array.h"

/**
 * A helper class for adapters that implement the SectionIndexer interface.
 * If the items in the adapter are sorted by simple alphabet-based sorting, then
 * this class provides a way to do fast indexing of large lists using binary search.
 * It caches the indices that have been determined through the binary search and also
 * invalidates the cache if changes occur in the cursor.
 * <p/>
 * Your adapter is responsible for updating the cursor by calling {@link #setCursor} if the
 * cursor changes. {@link #getPositionForSection} method does the binary search for the starting
 * index of a given section (alphabet).
 */
class AlphabetIndexer
{
public:
    AlphabetIndexer();

    /**
     * Constructs the indexer.
     * @param cursor the cursor containing the data set
     * @param sortedColumnIndex the column number in the cursor that is sorted
     *        alphabetically
     * @param alphabet string containing the alphabet, with space as the first character.
     *        For example, use the string " ABCDEFGHIJKLMNOPQRSTUVWXYZ" for English indexing.
     *        The characters must be uppercase and be sorted in ascii/unicode order. Basically
     *        characters in the alphabet will show up as preview letters.
     */
    AlphabetIndexer(
        /* [in] */ ICursor* cursor, 
        /* [in] */ Int32 sortedColumnIndex, 
        /* [in] */ ICharSequence* alphabet);

    ~AlphabetIndexer();

    /**
     * Returns the section array constructed from the alphabet provided in the constructor.
     * @return the section array
     */
    virtual CARAPI_(AutoPtr<IObjectContainer>) GetSections();

    /**
     * Sets a new cursor as the data set and resets the cache of indices.
     * @param cursor the new cursor to use as the data set
     */
    virtual CARAPI SetCursor(
        /* [in] */ ICursor* cursor);

    /**
     * Performs a binary search or cache lookup to find the first row that
     * matches a given section's starting letter.
     * @param sectionIndex the section to search for
     * @return the row index of the first occurrence, or the nearest next letter.
     * For instance, if searching for "T" and no "T" is found, then the first
     * row starting with "U" or any higher letter is returned. If there is no
     * data following "T" at all, then the list size is returned.
     */
    virtual CARAPI_(Int32) GetPositionForSection(
        /* [in] */ Int32 sectionIndex);

    /**
     * Returns the section index for a given position in the list by querying the item
     * and comparing it with all items in the section array.
     */
    virtual CARAPI_(Int32) GetSectionForPosition(
        /* [in] */ Int32 position);

    /*
     * @hide
     */
    virtual CARAPI OnChanged();

    /*
     * @hide
     */
    virtual CARAPI OnInvalidated();

protected:

    /**
     * Default implementation compares the first character of word with letter.
     */
    virtual CARAPI_(Int32) Compare(
        /* [in] */ String word, 
        /* [in] */ String letter);

    /**
     * Cursor that is used by the adapter of the list view.
     */
    AutoPtr<ICursor> mDataCursor;

    /**
     * The index of the cursor column that this list is sorted on.
     */
    Int32 mColumnIndex;

    /**
     * The string of characters that make up the indexing sections.
     */
    AutoPtr<ICharSequence> mAlphabet;

private:
    /**
     * Cached length of the alphabet array.
     */
    Int32 mAlphabetLength;

    /**
     * This contains a cache of the computed indices so far. It will get reset whenever
     * the dataset changes or the cursor changes.
     */
    SparseInt32Array* mAlphaMap;

    /**
     * Use a collator to compare strings in a localized manner.
     */
    //java.text.Collator mCollator;

    /**
     * The section array converted from the alphabet string.
     */
    ArrayOf<String>* mAlphabetArray;

};

#endif
