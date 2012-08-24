//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifdef _linux
#include <binder/ProcessState.h>
#endif
#include <elatypes.h>
#include <elapi.h>

_ELASTOS_NAMESPACE_USING

extern "C" UInt32 StartServiceCentral();
extern "C" UInt32 StopServiceCentral();

extern void InitTLS();
extern void UninitTLS();

#ifndef _android   
extern ECode InitROT();
extern void UninitROT();

extern ECode InitMIL();
extern void UninitMIL();

extern void InitProxyEntry();
extern void UninitProxyEntry();
#endif

extern pthread_mutex_t g_LocModListLock;

#define EXIT_IFFAILED(func)  do { \
        ec = func(); \
        if (FAILED(ec)) { \
            goto E_FAIL_EXIT; \
        } \
    }while(0)

Boolean AttachElastosDll()
{
#ifdef _win32
    ECode ec = NOERROR;
#endif
    pthread_mutexattr_t recursiveAttr;

    InitTLS();
    
#ifndef _android    
    InitROT();
    InitMIL();
    InitProxyEntry();
#ifdef _linux
    android::ProcessState::self()->startThreadPool();
#endif
#endif
    
    pthread_mutexattr_init(&recursiveAttr);
    pthread_mutexattr_settype(&recursiveAttr, PTHREAD_MUTEX_RECURSIVE);
    if (pthread_mutex_init(&g_LocModListLock, &recursiveAttr)) {
        goto E_FAIL_EXIT;
    }
    pthread_mutexattr_destroy(&recursiveAttr);

    return TRUE;

E_FAIL_EXIT:
    return FALSE;
}

void DetachElastosDll()
{
    pthread_mutex_destroy(&g_LocModListLock);

#ifndef _android       
    UninitProxyEntry();
    UninitMIL();
    UninitROT();
#endif
    
    UninitTLS();
}

