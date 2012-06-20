
#ifndef __CDIRECTIONS_H__
#define __CDIRECTIONS_H__

#include "_CDirections.h"

CarClass(CDirections)
{
public:
    ~CDirections();

    CARAPI GetDirections(
        /* [out] */ ArrayOf<Int16>** directions);

    // The values in mDirections are the offsets from the first character
    // in the line to the next flip in direction.  Runs at even indices
    // are left-to-right, the others are right-to-left.  So, for example,
    // a line that starts with a right-to-left run has 0 at mDirections[0],
    // since the 'first' (ltr) run is zero length.
    //
    // The code currently assumes that each run is adjacent to the previous
    // one, progressing in the base line direction.  This isn't sufficient
    // to handle nested runs, for example numeric text in an rtl context
    // in an ltr paragraph.
    /* package */ constructor(
        /* [in] */ const ArrayOf<Int16>& dirs);

private:
    ArrayOf<Int16>* mDirections;
};

#endif //__CDIRECTIONS_H__
