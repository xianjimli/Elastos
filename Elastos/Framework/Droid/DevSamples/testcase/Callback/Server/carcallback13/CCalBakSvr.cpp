#include "stdlib.h"
#include "stdio.h"
#include "CCalBakSvr.h"

ECode JCBTest2_OnEnterCoalescer(String oldStr, String *newStr)
{
    //printf("OnEnterCoalescer!\n");
    char* pWstr = new char[50]; // 分配了新内存
    strcpy(pWstr, oldStr);
    strcat(pWstr, *newStr);
    CThread::Sleep(300, NULL);
    *newStr = pWstr;  // 将合并结果保存到当前对象成员
    return NOERROR;    
}

ECode JCBTest3_OnRectCoalescer(Int32 x_old, Int32 y_old, Int32 width_old, Int32 height_old,
    Int32 *pX_new, Int32 *pY_new, Int32 *pWidth_new, Int32 *pHeight_new)
{
    return NOERROR;
}

ECode JCBTest3_OnArrayCoalescer(const BufferOf<Int32> & oldBuf, BufferOf<Int32> *pNewBuf)
{
    return NOERROR;
}

char * __cdecl _itoa(
    int val,
    char *buf,
    int radix
    )
{
    char *p;                /* pointer to traverse string */
    char *firstdig;         /* pointer to first digit */
    char temp;              /* temp char */
    unsigned digval;        /* value of digit */

    if (!buf) {
        return NULL;
    }

    if (radix < 2 || radix > 36) {
        *buf = '\0';
        return buf;
    }

    p = buf;

    firstdig = p;           /* save pointer to first digit */

    do {
        digval = (unsigned) (val % radix);
        val /= radix;       /* get next digit */

        /* convert to ascii and store */
        if (digval > 9)
            *p++ = (char) (digval - 10 + 'a'); /* a letter */
        else
            *p++ = (char) (digval + '0');       /* a digit */
    } while (val > 0);

    /* We now have the digit of the number in the buffer, but in reverse
       order.  Thus we reverse them now. */

    *p-- = '\0';            /* terminate string; p points to last digit */

    do {
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;   /* swap *p and *firstdig */
        --p;
        ++firstdig;         /* advance to next two digits */
    } while (firstdig < p); /* repeat until halfway */

    return buf;
}

ECode CCalBakSvr::Trigger(
    /* [in] */ Int32 nTriggerTimes,
    /* [in] */ const String& wstr)
{
    char wcstr[5] = "";
    printf("Trigger Start!\n");
    for (int i = 0; i < 18; i++) {
        printf("@%d@\n", i);
        Callback::OnEnter(_itoa(i, wcstr, 10));
        if (i % 5 == 0) {
            i++;
            printf("@%d@\n", i);
            Callback::UpdatingOnEnter(_itoa(i, wcstr, 10));
            CThread::Sleep(100, NULL);
        }
    }
    printf("Trigger Over!\n");
    return NOERROR;
}
