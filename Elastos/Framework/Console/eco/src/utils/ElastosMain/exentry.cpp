//
// Copyright (c) elastos.org
//

#include <elasysapi.h>
#include <stdlib.h>
#include "crt0.h"

_ELASTOS_NAMESPACE_USING

#ifndef MAXIMUM_ARGC
#define MAXIMUM_ARGC                    128
#define MAXIMUM_ARGS_LENGTH             1024   // Contains the L'\0'
#endif

static int ParseArgs(char *wargv[], char *szArgs);

extern Int32 g_lNeedCleanUp;
EXTERN_C PATEXIT_DATA g_pDllExitStack;

#if !defined(_MSVC)
static _ELASTOS Boolean GetIsDebug()
{
    IProcess *pCurPro = CProcess::GetCurrent();
    IMemtrace *iMemtrace = NULL;
    Boolean bDebug = FALSE;
    iMemtrace = IMemtrace::Probe(pCurPro);
    if (!iMemtrace) {
        return FALSE;
    }
    iMemtrace->GetTraceDebug(&bDebug);
    return bDebug;
}

#define DEBUG_PROGRAM GetIsDebug()
#else
#define DEBUG_PROGRAM 0
#endif

#ifdef _GNUC
#define _CRTIMP  __attribute__ ((dllimport))
#else
#define _CRTIMP  __declspec(dllimport)
#endif

#ifdef _INTEL
// BUGBUG:Intel compiler bug.
//        Intel compiler force use c++ symbol for "wmain" function.
EXTERN_C int __cdecl _wmain(int argc, wchar_t* wargv[]);
#define wmain _wmain
#endif

EXTERN_C void mainCRTStartup(void);

void mainCRTStartup(void)
{
    int argc;
    char *argv[MAXIMUM_ARGC];

    //WStringBuf_<MAXIMUM_FULL_PATH_LENGTH> esbCmdName;
    //WStringBuf_<MAXIMUM_ARGS_LENGTH> esbCmdArgs;

    g_pDllExitStack = (PATEXIT_DATA)-1;
    _crtinit(1);
    __exeatexit(_dtorlist);

    FILE *cmdline = fopen("/proc/self/cmdline", "rb");
    char *arg = NULL;
    size_t size = 0;
    if (getdelim(&arg, &size, 0, cmdline) == -1) {
    	//puts(arg);
    	exit(-1);
    }

    //_Impl_GetStartInfo(&esbCmdName, &esbCmdArgs);

    argc = ParseArgs(argv, arg);

    if (DEBUG_PROGRAM) DebugBreak();
    BufferOf<String> box((String *)argv, argc + 1, argc);
    PVoid pAppletObj;
    PInterface pCallbackContext;
    StringBuf_<260> sCurrentPath;

//old version
#if 0
    ECode ec = _Impl_TApplet_Create(&pAppletObj, &pCallbackContext);
    if (FAILED(ec)) exit(ec);;

    _CThread_SetLocalObject(TL_CALLBACK_SLOT, pCallbackContext);
    _Impl_CallbackSink_SetDefaultCallbackContext(pCallbackContext);

    ec = _Impl_TApplet_Prepare(pAppletObj);
    if (FAILED(ec)) exit(ec);
#endif

    IInterface *pOrgCallbackContext;
    IInterface *m_pCallbackContext;

    _Impl_CallbackSink_InitCallbackContext(&m_pCallbackContext);
    pOrgCallbackContext = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
    if (NULL != pOrgCallbackContext) {
        pOrgCallbackContext->Release();
    }
    pthread_setspecific(TL_CALLBACK_SLOT, m_pCallbackContext);

    ec = ElastosMain(box);

    //ec = _Impl_TApplet_DoWork(pAppletObj, ec);
    ec = _Impl_CallbackSink_TryToHandleEvents(m_pCallbackContext);

    exit(ec);
}

static int ParseArgs(char *argv[], char *szArgs)
{
    int argc;
    char *pCurPos;

    argc = 1; // at least one argument(the program name)
    argv[0] = szArgs;
    pCurPos = szArgs;

    // TODO: handle back slash
    for (;;) {
        if (0 == *pCurPos) break;
//        SkipBlank(pCurPos);
        while (*pCurPos == ' ' || *pCurPos == '\t') pCurPos++;
        if (0 == *pCurPos) break;

        if ('\"' == *pCurPos) {
            argv[argc++] = ++pCurPos; // skip double-quote
            while (('\"' != *pCurPos) && (0 != *pCurPos)) {
                pCurPos++;
            }
            if (0 == *pCurPos) break;
            *pCurPos = 0;  // end of one arg
            pCurPos++;
        }
        else {
            argv[argc++] = pCurPos;
            while ((' ' != *pCurPos) && ('\t' != *pCurPos)
                && (0 != *pCurPos)) {
                pCurPos++;
            }
            if (0 == *pCurPos) break;
            *pCurPos = 0;  // end of one arg
            pCurPos++;
        }
        if (argc == MAXIMUM_ARGC - 1) break;
    }
    argv[argc] = NULL;

    return argc;
}
