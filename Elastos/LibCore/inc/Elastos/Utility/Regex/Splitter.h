
#ifndef __SPLITTER_H__
#define __SPLITTER_H__

#include "Elastos.Utility.Regex_server.h"
#include "cmdef.h"
#include <elastos/List.h>

/**
 * Used to make {@code String.split} fast (and to help {@code Pattern.split} too).
 * @hide
 */
class Splitter
{
private:
    static const CString METACHARACTERS;

public:
    Splitter(
        /* [in] */ const String& text,
        /* [in] */ ArrayOf<Int32>* offsets);

    /**
     * Returns a result equivalent to {@code s.split(separator, limit)} if it's able
     * to compute it more cheaply than ICU, or null if the caller should fall back to
     * using ICU.
     */
    static CARAPI_(ArrayOf<String>*) FastSplit(
        /* [in] */ const String& re,
        /* [in] */ const String& input,
        /* [in] */ Int32 limit);

    static  CARAPI_(ArrayOf<String>*) Split(
        /* [in] */ IPattern* pattern,
        /* [in] */ const String& re,
        /* [in] */ const String& input,
        /* [in] */ Int32 limit);

    static  CARAPI_(ArrayOf<String>*) FinishSplit(
        /* [in] */ List<String>* list,
        /* [in] */ const String& input,
        /* [in] */ Int32 begin,
        /* [in] */ Int32 maxSize,
        /* [in] */ Int32 limit);

private:
    /**
     * Holds the original input text.
     */
    String mText;

    /**
     * Holds the offsets of the groups in the input text. The first two
     * elements specifiy start and end of the zero group, the next two specify
     * group 1, and so on.
     */
    ArrayOf<Int32>* mOffsets;
};

#endif //__SPLITTER_H__
