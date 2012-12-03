//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef _CRT0_H_
#define _CRT0_H_

#include <elatypes.h>
#include <eladef.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

typedef void (__cdecl * EXIT_FUNCTION)(void);

typedef struct _ATEXIT_DATA {
    unsigned int Entries;
    EXIT_FUNCTION Array[1];
} ATEXIT_DATA, *PATEXIT_DATA;

//
// Definition of the argument values for the exit() function
//
#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1
#define EXE_ENTER_FLAG 0x10
#define EXE_EXIT_FLAG  0x0F
#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH_KERNEL 5
#define DLL_PROCESS_ATTACH_KERNEL 6

typedef void (__cdecl *PF)(void);

EXTERN_C int __cdecl main(int argc, char* argv[]);
EXTERN_C _ELASTOS Boolean __stdcall DllMain(_ELASTOS PVoid, _ELASTOS UInt32, _ELASTOS PVoid);
EXTERN_C void __cdecl exit(int code);
EXTERN_C void _crtinit(int startup);
EXTERN_C void _dtorlist();
EXTERN_C int __cdecl __doatexit(EXIT_FUNCTION Func,  PATEXIT_DATA *ppFuncStack);
EXTERN_C int __cdecl __exeatexit(EXIT_FUNCTION Func);

#ifdef _GNUC
EXTERN_C PF __CTOR_LIST__[];
EXTERN_C PF __DTOR_LIST__[];
#else
EXTERN_C PF __xi_a[], __xi_z[]; /* C initializers */
EXTERN_C PF __xc_a[], __xc_z[]; /* C++ initializers */
EXTERN_C PF __xp_a[], __xp_z[]; /* C pre-terminators */
EXTERN_C PF __xt_a[], __xt_z[]; /* C terminators */
#endif

#endif // _CRT0_H_
