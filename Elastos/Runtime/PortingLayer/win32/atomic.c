#include "sys/atomics.h"
#include <windows.h>

int atomic_inc(int *ptr)
{
    return InterlockedIncrement((long *)ptr);
}

int atomic_dec(int *ptr)
{
    return InterlockedDecrement((long *)ptr);
}

int atomic_swap(int _new, int *ptr)
{
    return InterlockedExchangeAdd((long *)ptr, _new);
}

int atomic_cmpxchg(int old, int _new, int *ptr)
{
    return InterlockedCompareExchange((long *)ptr, _new, old);
}
