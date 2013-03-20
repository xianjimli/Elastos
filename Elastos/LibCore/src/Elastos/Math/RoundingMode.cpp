#include "RoundingMode.h"

ECode RoundingMode::Init(
    /* [in] */ Int32 rm) {
    mBigDecimalRM = rm;
    return NOERROR;
}

ECode RoundingMode::ValueOf(
        /* [in] */ Int32 mode,
        /* [out] */ RRoundingMode* enumNumber)
{
    switch (mode) {
        case 2:
            *enumNumber = RoundingMode_CEILING;
            return NOERROR;
        case 1:
            *enumNumber = RoundingMode_DOWN;
            return NOERROR;
        case 3:
            *enumNumber = RoundingMode_FLOOR;
            return NOERROR;
        case 5:
            *enumNumber = RoundingMode_HALF_DOWN;
            return NOERROR;
        case 6:
            *enumNumber = RoundingMode_HALF_EVEN;
            return NOERROR;
        case 4:
            *enumNumber = RoundingMode_HALF_UP;
            return NOERROR;
        case 7:
            *enumNumber = RoundingMode_UNNECESSARY;
            return NOERROR;
        case 0:
            *enumNumber = RoundingMode_UP;
            return NOERROR;
        default:
            //throw new IllegalArgumentException("Invalid rounding mode");
            //return E_ILLEGAL_ARGUMENT_EXCEPTION;
        return NOERROR;
    }
}