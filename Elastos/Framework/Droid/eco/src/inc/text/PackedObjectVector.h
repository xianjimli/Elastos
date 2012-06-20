#ifndef __PACKEDOBJECTVECTOR_H__
#define __PACKEDOBJECTVECTOR_H__

#include "ext/frameworkext.h"
#include "utils/ArrayUtils.h"
#include <elastos/Vector.h>
#include <elastos/AutoPtr.h>


class PackedObjectVector
{
public:
    PackedObjectVector(
        /* [in] */ Int32 columns);

    ~PackedObjectVector();

    CARAPI_(AutoPtr<IInterface>) GetValue(
        /* [in] */ Int32 row,
        /* [in] */ Int32 column);

    CARAPI_(void) SetValue(
        /* [in] */ Int32 row,
        /* [in] */ Int32 column,
        /* [in] */ IInterface* value);

    CARAPI_(void) InsertAt(
        /* [in] */ Int32 row,
        /* [in] */ ArrayOf<IInterface*>* values);

    CARAPI_(void) DeleteAt(
        /* [in] */ Int32 row,
        /* [in] */ Int32 count);

    CARAPI_(Int32) Size();

    CARAPI_(Int32) Width();

    CARAPI_(void) Dump();

private:
    CARAPI_(void) GrowBuffer();

    CARAPI_(void) MoveRowGapTo(
        /* [in] */ Int32 where);

private:
    Int32 mColumns;
    Int32 mRows;

    Int32 mRowGapStart;
    Int32 mRowGapLength;

    ArrayOf<IInterface*>* mValues;
};

#endif //__PACKEDOBJECTVECTOR_H__
