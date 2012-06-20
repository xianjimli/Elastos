
#include "utils/ArrayUtils.h"


Int32 ArrayUtils::IdealByteArraySize(
    /* [in] */ Int32 need)
{
    for (Int32 i = 4; i < 32; i++) {
        if (need <= (1 << i) - 12) {
            return (1 << i) - 12;
        }
    }

    return need;
}

Int32 ArrayUtils::IdealBooleanArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need);
}

Int32 ArrayUtils::IdealInt16ArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 2) / 2;
}

Int32 ArrayUtils::IdealChar8ArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need);
}

Int32 ArrayUtils::IdealInt32ArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 4) / 4;
}

Int32 ArrayUtils::IdealFloatArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 4) / 4;
}

Int32 ArrayUtils::IdealObjectArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 4) / 4;
}

Int32 ArrayUtils::IdealInt64ArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 8) / 8;
}
