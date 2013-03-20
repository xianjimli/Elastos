#ifndef __ROUNDINGMODE_H__
#define __ROUNDINGMODE_H__

#include "cmdef.h"
#include "Elastos.Math_server.h"
#include <elastos.h>

using namespace Elastos;

class RoundingMode {
private:
    /** The old constant of <code>BigDecimal</code>. */
    Int32 mBigDecimalRM;

protected:
    /** It sets the old constant. */
    CARAPI Init(
        /* [in] */ Int32 rm);

public:
    /**
     * Converts rounding mode constants from class {@code BigDecimal} into
     * {@code RoundingMode} values.
     *
     * @param mode
     *            rounding mode constant as defined in class {@code BigDecimal}
     * @return corresponding rounding mode object
     */
    static CARAPI ValueOf(
        /* [in] */ Int32 mode,
        /* [out] */ RRoundingMode* enumNumber);
};
#endif //__ROUNDINGMODE_H__