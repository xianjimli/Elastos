#ifndef __BIDIRUN_H__
#define __BIDIRUN_H__

#include <elastos.h>

using namespace Elastos;

class BidiRun
{
public:
    CARAPI Init(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ Int32 level);

    virtual CARAPI GetLevel(
        /* [out] */ Int32 * level);

    virtual CARAPI GetLimit(
        /* [out] */ Int32 * limit);

    virtual CARAPI GetStart(
        /* [out] */ Int32 * start);

private:
    Int32 mStart;

    Int32 mLimit;

    Int32 mLevel;
};
#endif //__BIDIRUN_H__