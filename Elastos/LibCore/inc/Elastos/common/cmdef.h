
#ifndef __CMDEF_H__
#define __CMDEF_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#define FAIL_RETURN(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return ec; \
    } while(0);

#define ASSERT_SUCCEEDED(expr) assert(SUCCEEDED(expr));
#define VALIDATE_NOT_NULL(x) if (!x) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }

#define CLONE_ARRAY_OF_STRING(dest, src) \
    if (src != NULL) { \
        dest = ArrayOf<String>::Alloc(src->GetLength()); \
        for (Int32 i = 0; i < src->GetLength(); ++i) { \
            (*dest)[i] = (*src)[i]; \
        } \
    } \
    else dest = NULL;

#define FREE_ARRAY_OF_STRING(var) \
    if (var != NULL) { \
        for (Int32 i = 0; i < var->GetLength(); ++i) { \
            (*var)[i] = NULL; \
        } \
        ArrayOf<String>::Free(var); \
        var = NULL; \
    }

#include <errno.h>

/*
 * TEMP_FAILURE_RETRY is defined by some, but not all, versions of
 * <unistd.h>. (Alas, it is not as standard as we'd hoped!) So, if it's
 * not already defined, then define it here.
 */
#ifndef TEMP_FAILURE_RETRY
/* Used to retry syscalls that can return EINTR. */
#define TEMP_FAILURE_RETRY(exp) ({         \
    typeof (exp) _rc;                      \
    do {                                   \
        _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })
#endif


#endif //__CMDEF_H__
