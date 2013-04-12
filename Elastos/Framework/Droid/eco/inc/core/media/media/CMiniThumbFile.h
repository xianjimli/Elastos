
#ifndef __CMINITHUMBFILE_H__
#define __CMINITHUMBFILE_H__

#include "_CMiniThumbFile.h"

#include "media/MiniThumbFile.h"

CarClass(CMiniThumbFile), public MiniThumbFile
{
public:
    CARAPI Deactivate();

    CARAPI GetMagic(
        /* [in] */ Int64 id,
        /* [out] */ Int64 * pNumber);

    CARAPI SaveMiniThumbToFile(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Int64 id,
        /* [in] */ Int64 magic);

    CARAPI GetMiniThumbFromFile(
        /* [in] */ Int64 id,
        /* [in] */ const ArrayOf<Byte> & data,
        /* [out, callee] */ ArrayOf<Byte> ** ppMiniThumb);

    CARAPI constructor(
        /* [in] */ IUri * pUri);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMINITHUMBFILE_H__
