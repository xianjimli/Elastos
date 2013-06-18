#ifndef __BIDIRUN_H__
#define __BIDIRUN_H__

#include <elastos.h>

using namespace Elastos;

class BidiRun
{
public:
    BidiRun(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ Int32 level);

    CARAPI_(Int32) GetLevel();

    CARAPI_(Int32) GetLimit();

    CARAPI_(Int32) GetStart();

private:
    Int32 mStart;

    Int32 mLimit;

    Int32 mLevel;
};
#endif //__BIDIRUN_H__