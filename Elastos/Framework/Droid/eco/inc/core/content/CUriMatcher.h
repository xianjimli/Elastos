
#ifndef __CURIMATCHER_H__
#define __CURIMATCHER_H__
#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "_CUriMatcher.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

CarClass(CUriMatcher)
{
public:
    CUriMatcher();

    ~CUriMatcher();

    /**
     * Add a URI to match, and the code to return when this URI is
     * matched. URI nodes may be exact match string, the token "*"
     * that matches any text, or the token "#" that matches only
     * numbers.
     *
     * @param authority the authority to match
     * @param path the path to match. * may be used as a wild card for
     * any text, and # may be used as a wild card for numbers.
     * @param code the code that is returned when a URI is matched
     * against the given components. Must be positive.
     */
    CARAPI AddURI(
        /* [in] */ const String& authority,
        /* [in] */ const String& path,
        /* [in] */ Int32 code);

    /**
     * Try to match against the path in a url.
     *
     * @param uri       The url whose path we will match against.
     *
     * @return  The code for the matched node (added using addURI),
     * or -1 if there is no matched node.
     */
    CARAPI Match(
        /* [in] */ IUri* uri,
        /* [out] */ Int32* matchCode);

    /**
     * Creates the root node of the URI tree.
     *
     * @param code the code to match for the root URI
     */
    CARAPI constructor(
        /* [in] */ Int32 code);

protected:
    //static final Pattern PATH_SPLIT_PATTERN = Pattern.compile("/");
    //AutoPtr<CPattern> PATH_SPLIT_PATTERN;
private:
    const static Int32 EXACT = 0;
    const static Int32 NUMBER = 1;
    const static Int32 TEXT = 2;
 
    Int32 mCode;
    Int32 mWhich;
    String mText;
    List<AutoPtr<CUriMatcher> >* mChildren;

};

#endif // __CURIMATCHER_H__
